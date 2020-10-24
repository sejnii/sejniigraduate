// threadblock_scheduling.cpp : �� ���Ͽ��� 'main' �Լ��� ���Ե˴ϴ�. �ű⼭ ���α׷� ������ ���۵ǰ� ����˴ϴ�.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#define MAXREG 65536
#define MAXSMEM 49152
#define MAX_DP_GFLOPS 12150
#define MAX_SP_GFLOPS 379.7
#define MAX_THRPT 547.6
using namespace std;

struct linfo
{
	int sm;
	int tb;
	double time;
};

struct SM
{
	int reg;
	int smem;
	int app_count;
	vector<string> app_list;
};

struct kernel_info
{
	string name;
	double exec_time;
	int category; // 0 : L1, 1 : Mem, 2 : Compute
};
struct SM sms[31];

void initsm()
{
	for (int i = 1; i < 31; i++)
	{
		sms[i].reg = MAXREG;
		sms[i].smem = MAXSMEM;
		sms[i].app_count = 0;
		sms[i].app_list.clear();
	}
}

void input_app_name();
void find_tb_convergence(int app_num, double rate);
void find_sm_convergence(int app_num, double sm_rate);
void allocate_sm(int *sm_cnt, int cnt_turn, int app_num);
void can_allocate_sm(int *sm_cnt, int cnt_turn, int app_num);
bool l1_rule(string app_name, string l1_name);

int query_category(string app_name);
double query_thrpt(string app_name);
double query_sp_gflops(string app_name);
double query_dp_gflops(string app_name);
float query_epc(string app_name);
int query_reg(string app_name);
int query_smem(string app_name);
string toString(vector<string> applist);
void get_info(string app_name, vector<vector<double>> *app, int iter);
double query_exec_time(string app_name);
bool cmp(const kernel_info &a, const kernel_info &b);

vector<vector<vector<double>>> app;

vector<vector<linfo>> saturate_app; //TB ���� saturate TB ã�� ����(�� SM ��) �� ���� (�� �����)

vector<string> app_name;
vector<kernel_info> kernel_info_list;
vector<int> reg;
vector<int> smem;

vector<linfo> sat_point_app; //  SM ���� saturate SM ã�� ���� (�� �����)

vector<vector<string>> chosen_kernel; // ��� �� Chosen kernel���� list {A, B}, {C}, {D,E,F} .... �̷������� chosen kernel�� �Բ� ��ġ�� ������� �ǹ�

vector<bool> flags; // �̹� �����ٸ� �Ǿ����� Ȯ���ϴ� flag ���� -> 1�̸� �̹� �����ٸ� ��

int main()
{
	int sm = 0, tb = 0;
	double time = 0;
	int concurrent_apps = 2;

	cout << "Hierarchical scheduling" << endl
		 << endl;

	input_app_name();

	sort(kernel_info_list.begin(), kernel_info_list.end(), cmp); // Shortes Job First �� ����

#ifdef DEBUG
	cout << "Sorted kernels" << endl;
	for (int i = 0; i < kernel_info_list.size(); i++)
	{
		cout << kernel_info_list[i].name << " " << kernel_info_list[i].exec_time << endl;
	}
#endif

	flags.resize(kernel_info_list.size());
	for (int i = 0; i < kernel_info_list.size(); i++)
	{
		vector<vector<double>> tmp;
		get_info(kernel_info_list[i].name, &tmp, i);
		app.push_back(tmp);
	}

	for (int i = 0; i < kernel_info_list.size(); i++)
	{
		reg.push_back(query_reg(kernel_info_list[i].name));
		smem.push_back(query_smem(kernel_info_list[i].name));
	}

#ifdef DEBUG
	for (int iter = 0; iter < kernel_info_list.size(); iter++)
	{
		for (int i = 5; i < 31; i += 5)
		{
			for (int j = 0; j < app[iter][i].size(); j++)
				N cout << i << " " << j + 1 << " " << app[iter][i][j] << endl;
		}
	}

#endif

	double rate = 0.05;
	/*cout << "TB convergence�� �Ǵ��� rate�� �Է��ϼ��� ex> 0.05 -> 5%�� �ǹ���" << endl;

	cin >> rate;
	if (rate == -1)
		break;*/
	saturate_app.resize(app_name.size());
	for (int i = 0; i < app_name.size(); i++)
	{
		find_tb_convergence(i, rate);
	}

#ifdef DEBUG

	for (int iter = 0; iter < app_name.size(); iter++)
	{

		cout << app_name[iter] << endl;
		for (int i = 5; i < 31; i += 5)
		{
			cout << saturate_app[iter][i].sm << " " << saturate_app[iter][i].tb << " " << saturate_app[iter][i].time << endl;
		}
	}

#endif

	double sm_rate = 0.01;

	sat_point_app.resize(app_name.size());
	bool flag = true; // ��� app�� �Ҵ����� �� ���ٸ� false

	for (int i = 0; i < app_name.size(); i++)
	{
		find_sm_convergence(i, sm_rate);
	}

	for (int i = 0; i < app_name.size(); i++)
	{
		cout << kernel_info_list[i].name << " : ";
		cout << sat_point_app[i].sm << " " << sat_point_app[i].tb << " " << sat_point_app[i].time << endl;
	}

	/////////////////////////////////////������� ��� app���� saturation point ã��///////////////////////////////

	int iter = 0;
	while (1)
	{
		bool flag_iter = false;
		vector<string> tmp;
		initsm();
		double sum_sp_gflops = 0.0;
		double sum_dp_gflops = 0.0;
		double sum_thrpt = 0.0;
		bool flag_l = false;  //check if there is L1 sensitive in Chosen Kernels
		bool flag_l2 = false; // if any kernel is placed with L1 sensitive, other kernels should not be co-executed
		bool flag_m = false;  // check if there is M in CK
		bool flag_c = false;  // check if there is C with bigger EPC than 1
		string l1_name = "";
		for (int index = iter; index <= (index - iter); index++)
		{
			if ((flags[index] != 1) && (flag_l2 != true))
			{
				//bool flag = true;

				///////////////////// first stage ///////////////////////////////////
				int sm_cnt = 0;
				for (int i = 0; i < kernel_info_list.size(); i++)
				{
					can_allocate_sm(&sm_cnt, 0, index);
					if (sm_cnt >= sat_point_app[index].sm)
						break;
				}
				if (sm_cnt < sat_point_app[index].sm)
				{
					//flag = false;
					break;
				}
				else
				{
				}

				////////////////////// second stage /////////////////////////////////////////
				double sp_flops = query_sp_gflops(kernel_info_list[index].name);
				double dp_flops = query_dp_gflops(kernel_info_list[index].name);
				double thrpt = query_thrpt(kernel_info_list[index].name);

				if ((sum_sp_gflops + sp_flops <= MAX_SP_GFLOPS) && (sum_dp_gflops + dp_flops <= MAX_DP_GFLOPS) && (sum_thrpt + thrpt <= MAX_THRPT))
				{
					//flag = true;
				}
				else
				{
					//flag = false;
					break;
				}

				/////////////////////// third stage ////////////////////////////
				if (kernel_info_list[index].category == 0)
				{ // if this kernel is L1 se
					if (flag_l)
					{ //L1 sensitive can not be placed with L1 // if this flag is false, it would be the first kernel (L1 has the highest priority)
						//flag = false;
						break;
					}
					else
					{
						flag_l = true;
						//flag = true;
						//flag_iter =true;
					}
				}
				else
				{ // not L1 sensitive
					if (flag_l)
					{ // if there was L1 sensitive kernel
						if (!l1_rule(kernel_info_list[index].name, l1_name))
						{ // check if this kernel is suitable with L1 sensitive
							//flag = false;
							break;
						}
						else
						{
							flag_l2 = true;
							//	flag_iter = true;
						}
					}
					else
					{
						if (kernel_info_list[index].category == 1) // if this kernel is M
						{
							if (flag_m == 1)
							{
								//flag = false;
								break;
							}
							else
							{
								flag_m = true;
								//flag= true;
								flag_iter = true;
							}
						}
						else
						{ //if this kernel is C
							if (flag_c == 1)
							{
								//flag = false;
								break;
							}
							else
							{
								float epc = query_epc(kernel_info_list[index].name);
								if (epc > 1)
									flag_c = true;
								//	flag= true;
								//	flag_iter = true;
							}
						}
					}
				}
				// success every stage
				for (int i = 0; i <= (index - iter); i++)
				{
					allocate_sm(&sm_cnt, i, index);
				}

				/*
		if (sm_cnt < sat_point_app[index].sm) {
			flag = false;
					
		}*/


				
					tmp.push_back(kernel_info_list[index].name);
					flags[index] = 1;
					flag_iter = true;
				
			}
		}
		if (flag_iter == true)
		{
			cout << "static info scheduling result" << endl;
			cout << "chosen kernel [" << iter << "] : " << endl;
			for (int i = 1; i < 31; i++)
			{
				cout << "sm num : " << i << ", app list : " << toString(sms[i].app_list) << ", remained reg : " << sms[i].reg << ", remaind smem : " << sms[i].smem << endl;
			}
			chosen_kernel.push_back(tmp);
		}
		iter++;
		if (iter == kernel_info_list.size())
			break;
	}

	for (int i = 0; i < chosen_kernel.size(); i++)
	{
		for (int j = 0; j < chosen_kernel[i].size(); j++)
		{
			cout << chosen_kernel[i][j] << " ";
		}
		cout << endl;
	}
}

void input_app_name()
{

	int iter = 0;
	while (1)
	{

		string input;

		cout << iter << "�� ������ �̸��� �Է��ϼ���" << endl;

		cin >> input;

		if (input.compare("-1") == 0)
			break;
		else
		{
			app_name.push_back(input);
			double time = query_exec_time(input);
			int category = query_category(input);
			kernel_info ki;
			ki.name = input;
			ki.exec_time = time;
			ki.category = category;
			kernel_info_list.push_back(ki);
			/*
			vector<vector<double>> tmp;
			get_info(app_name[iter], &tmp, iter);
			app.push_back(tmp);
			*/
		}
		iter++;
	}
}

void find_tb_convergence(int num, double rate)
{
	saturate_app[num].resize(31);

	for (int i = 5; i < 31; i += 5)
	{
		for (int j = 0; j < app[num][i].size(); j++)
		{
			if ((j + 1 != app[num][i].size()) && (app[num][i][j] * (1.00 - rate) < app[num][i][j + 1]))
			{ //converge��� �Ǵ�
				if ((j + 2 != app[num][i].size()) && app[num][i][j] * (1.00 - rate) * (1.00 - rate) < app[num][i][j + 2])
				{
					if ((j + 3 != app[num][i].size()) && (app[num][i][j] * (1.00 - rate) * (1.00 - rate) * (1.00 - rate) < app[num][i][j + 3]))
					{
						linfo tmp;
						tmp.sm = i;
						tmp.tb = j + 1;
						tmp.time = app[num][i][j];
						saturate_app[num][i] = tmp;

						break;
					}
				}
			}

			if (j == app[num][i].size() - 1)
			{
				linfo tmp;
				tmp.sm = i;
				tmp.tb = j + 1;
				tmp.time = app[num][i][j];
				saturate_app[num][i] = tmp;
			}
		}
	}
}

void find_sm_convergence(int num, double sm_rate)
{
	linfo tmp;
	for (int i = 5; i < 31; i += 5)
	{
		if ((i + 5 <= 30) && (saturate_app[num][i].time * (1.00 - sm_rate) < saturate_app[num][i + 5].time))
		{
			if ((i + 10 <= 30) && (saturate_app[num][i].time * (1.00 - sm_rate) * (1.00 - sm_rate) < saturate_app[num][i + 10].time))
			{
				if ((i + 15 <= 30) && (saturate_app[num][i].time * (1.00 - sm_rate) * (1.00 - sm_rate) * (1.00 - sm_rate) < saturate_app[num][i + 15].time))
				{

					tmp.sm = saturate_app[num][i].sm;
					tmp.tb = saturate_app[num][i].tb;
					tmp.time = saturate_app[num][i].time;
					sat_point_app[num] = tmp;
					break;
				}
			}
		}
		if (i == 30)
		{
			tmp.sm = saturate_app[num][i].sm;
			tmp.tb = saturate_app[num][i].tb;
			tmp.time = saturate_app[num][i].time;
			sat_point_app[num] = tmp;
		}
	}
}

void allocate_sm(int *sm_cnt, int cnt_turn, int app_num)
{
	int i = app_num;
	for (int j = 1; j < 31; j++)
	{ //sm �ϳ��� Ȯ�� -> app count == cnt_turn �� sm ã��
		if (*sm_cnt < sat_point_app[i].sm)
		{
			if (sms[j].app_count == cnt_turn)
			{
				if (sms[j].reg - reg[i] * sat_point_app[i].tb > 0 && sms[j].smem - smem[i] * sat_point_app[i].tb > 0)
				{
					sms[j].reg -= reg[i] * sat_point_app[i].tb;
					sms[j].smem -= smem[i] * sat_point_app[i].tb;
					sms[j].app_count++;
					sms[j].app_list.push_back(kernel_info_list[i].name);
					(*sm_cnt)++;
				}
			}
		}
		else
			return;
	}
}

void can_allocate_sm(int *sm_cnt, int cnt_turn, int app_num)
{
	int i = app_num;
	for (int j = 1; j < 31; j++)
	{ //sm �ϳ��� Ȯ�� -> app count == cnt_turn �� sm ã��
		if (*sm_cnt < sat_point_app[i].sm)
		{
			if (sms[j].app_count == cnt_turn)
			{
				if (sms[j].reg - reg[i] * sat_point_app[i].tb > 0 && sms[j].smem - smem[i] * sat_point_app[i].tb > 0)
				{
					sms[j].reg -= reg[i] * sat_point_app[i].tb;
					sms[j].smem -= smem[i] * sat_point_app[i].tb;
					sms[j].app_count++;
					sms[j].app_list.push_back(kernel_info_list[i].name);
					(*sm_cnt)++;
				}
			}
		}
		else
			return;
	}
}
bool l1_rule(string app_name, string l1_name)
{
	if (l1_name.compare("lbm"))
	{
		if (app_name.compare("stencil"))
			return true;
		if (app_name.compare("fdtd"))
			return true;
		if (app_name.compare("qs"))
			return true;
		if (app_name.compare("sy"))
			return true;
	}
	else
	{
		if (app_name.compare("lava"))
			return true;
		if (app_name.compare("qs"))
			return true;
		if (app_name.compare("qs"))
			return true;
	}
}

string toString(vector<string> applist)
{
	string s = " ";
	for (int i = 0; i < applist.size(); i++)
	{
		s += applist[i] + " ";
	}

	return s;
}