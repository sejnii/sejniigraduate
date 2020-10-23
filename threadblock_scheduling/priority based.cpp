// threadblock_scheduling.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#define MAXREG 65536
#define MAXSMEM 49152
using namespace std;

struct linfo {
	int sm;
	int tb;
	double time;

};

struct SM {
	int reg;
	int smem;
	int app_count;
	vector<string> app_list;
};


struct SM sms[31];

void initsm() {
	for (int i = 1; i < 31; i++) {
		sms[i].reg = MAXREG;
		sms[i].smem = MAXSMEM;
		sms[i].app_count = 0;
		sms[i].app_list.clear();
	}

}

string app_name[4];

void find_tb_convergence(double rate, linfo* saturate_app, vector<double>* app);
void find_sm_convergence(double sm_rate, linfo* saturate_app, linfo* sat_point_app);
void allocate_sm(int* sm_cnt, linfo* sat_point_app, int cnt_turn, int app_num, int* reg, int* smem);
int query_reg(string app_name);
int query_smem(string app_name);
string toString(vector<string> applist);

vector<double> app1[31];
vector<double> app2[31];
vector<double> app3[31];
vector<double> app4[31];


linfo saturate_app1[31]; // 각 SM 개수에서의 saturation point 저장
linfo saturate_app2[31];
linfo saturate_app3[31];
linfo saturate_app4[31];


int main()
{
	int sm = 0, tb = 0;
	double time = 0;
	int concurrent_apps = 3;



	cout << "1번 응용의 이름, sm, tb, time 정보를 입력하세요" << endl;
	cin >> app_name[0];
	while ((cin >> sm) && sm != -1)
	{
		cin >> tb;
		cin >> time;
		app1[sm].push_back(time);
	}
	cout << "2번 응용의 이름 sm, tb, time 정보를 입력하세요" << endl;
	cin >> app_name[1];
	while ((cin >> sm) && sm != -1)
	{
		cin >> tb;
		cin >> time;

		app2[sm].push_back(time);
	}

	cout << "3번 응용의 이름 sm, tb, time 정보를 입력하세요" << endl;
	cin >> app_name[2];
	while ((cin >> sm) && sm != -1)
	{
		cin >> tb;
		cin >> time;

		app3[sm].push_back(time);
	}
	if (concurrent_apps == 4) {
		cout << "4번 응용의 이름 sm, tb, time 정보를 입력하세요" << endl;
		cin >> app_name[3];
		while ((cin >> sm) && sm != -1)
		{
			cin >> tb;
			cin >> time;

			app4[sm].push_back(time);
		}
	}



	int reg[4];
	int smem[4];

	reg[0] = query_reg(app_name[0]); smem[0] = query_smem(app_name[0]);
	reg[1] = query_reg(app_name[1]); smem[1] = query_smem(app_name[1]);
	reg[2] = query_reg(app_name[2]); smem[2] = query_smem(app_name[2]);
	if (concurrent_apps == 4) {
		reg[3] = query_reg(app_name[3]); smem[3] = query_smem(app_name[3]);
	}



#if 0
	for (int i = 5; i < 31; i += 5) {
		for (int j = 0; j < app1[i].size(); j++)
			cout << i << " " << j + 1 << " " << app1[i][j] << endl;

	}
	cout << endl;
	for (int i = 5; i < 31; i += 5) {
		for (int j = 0; j < app2[i].size(); j++)
			cout << i << " " << j + 1 << " " << app2[i][j] << endl;

	}
	for (int i = 5; i < 31; i += 5) {
		for (int j = 0; j < app3[i].size(); j++)
			cout << i << " " << j + 1 << " " << app3[i][j] << endl;

	}
#endif
	double rate;
	cout << "convergence를 판단할 rate을 입력하세요 ex> 0.05 -> 5%를 의미함" << endl;

	cin >> rate;

	find_tb_convergence(rate, saturate_app1, app1);
	find_tb_convergence(rate, saturate_app2, app2);
	find_tb_convergence(rate, saturate_app3, app3);
	if (concurrent_apps == 4)
		find_tb_convergence(rate, saturate_app4, app4);

	for (int i = 5; i < 31; i += 5) {
		cout << saturate_app1[i].sm << " " << saturate_app1[i].tb << " " << saturate_app1[i].time << endl;
	}
	for (int i = 5; i < 31; i += 5) {
		cout << saturate_app2[i].sm << " " << saturate_app2[i].tb << " " << saturate_app2[i].time << endl;
	}
	for (int i = 5; i < 31; i += 5) {
		cout << saturate_app3[i].sm << " " << saturate_app3[i].tb << " " << saturate_app3[i].time << endl;
	}

	if (concurrent_apps == 4)
		for (int i = 5; i < 31; i += 5) {
			cout << saturate_app4[i].sm << " " << saturate_app4[i].tb << " " << saturate_app4[i].time << endl;
		}


	while (1) {
		double sm_rate;
		cout << "convergence를 판단할 rate을 입력하세요 ex> 0.05 -> 5%를 의미함" << endl;
		cin >> sm_rate;
		linfo sat_point_app[4];

		bool flag = true; // 모든 app을 할당해줄 수 없다면 false

		find_sm_convergence(sm_rate, saturate_app1, &sat_point_app[0]);
		find_sm_convergence(sm_rate, saturate_app2, &sat_point_app[1]);
		find_sm_convergence(sm_rate, saturate_app3, &sat_point_app[2]);
		if (concurrent_apps == 4)
			find_sm_convergence(sm_rate, saturate_app4, &sat_point_app[3]);

		cout << sat_point_app[0].sm << " " << sat_point_app[0].tb << " " << sat_point_app[0].time << endl;
		cout << sat_point_app[1].sm << " " << sat_point_app[1].tb << " " << sat_point_app[1].time << endl;
		cout << sat_point_app[2].sm << " " << sat_point_app[2].tb << " " << sat_point_app[2].time << endl;
		if (concurrent_apps == 4)
			cout << sat_point_app[3].sm << " " << sat_point_app[3].tb << " " << sat_point_app[3].time << endl;

		initsm();


		for (int i = 0; i < concurrent_apps; i++) { // app 하나씩 확인
			int sm_cnt = 0;
			allocate_sm(&sm_cnt, sat_point_app, 0, i, reg, smem); //sm 하나씩 확인 -> app count == 0 인 sm 찾기
			allocate_sm(&sm_cnt, sat_point_app, 1, i, reg, smem); //sm 하나씩 확인 -> app count == 1 인 sm 찾기
			allocate_sm(&sm_cnt, sat_point_app, 2, i, reg, smem); //sm 하나씩 확인 -> app count == 2 인 sm 찾기

			cout << "sm_cnt : " << sm_cnt << endl;
			if (sm_cnt < sat_point_app[i].sm) {
				flag = false;
				break;
			}
		}

		if (flag) {
			cout << "rate : " << sm_rate << "는 할당 가능합니다." << endl;
			cout << "할당 정보" << endl;
			for (int i = 1; i < 31; i++) {
				cout << "sm num : " << i << ", app list : " << toString(sms[i].app_list) << ", remained reg : " << sms[i].reg << ", remaind smem : " << sms[i].smem << endl;

			}
		}

		else
		{
			cout << "rate : " << sm_rate << "는 할당 불가능합니다." << endl;
		}
	}

}

void find_tb_convergence(double rate, linfo* saturate_app, vector<double>* app) {


	for (int i = 5; i < 31; i += 5) {
		for (int j = 0; j < app[i].size(); j++) {
			if ((j+1 != app[i].size()) && (app[i][j] * (1.00 - rate) < app[i][j + 1])) { //converge라고 판단
				if ((j+2 != app[i].size()) && app[i][j] * (1.00 - rate) * (1.00 - rate) < app[i][j + 2]) {
					if ( (j+3 != app[i].size()) && (app[i][j] * (1.00 - rate) * (1.00 - rate) * (1.00 - rate) < app[i][j + 3])) {
						saturate_app[i].sm = i;
						saturate_app[i].tb = j + 1;
						saturate_app[i].time = app[i][j];
						break;
					}
				}
			}

			if (j == app[i].size() - 1) {
				saturate_app[i].sm = i;
				saturate_app[i].tb = j + 1;
				saturate_app[i].time = app[i][j];
			}
		}
	}
}

void find_sm_convergence(double sm_rate, linfo* saturate_app, linfo* sat_point_app) {

	for (int i = 5; i < 31; i += 5) {
		if ((i+5 <= 30) &&  (saturate_app[i].time * (1.00 - sm_rate) < saturate_app[i + 5].time)) {
			if ((i + 10 <= 30) && (saturate_app[i].time * (1.00 - sm_rate) * (1.00 - sm_rate) < saturate_app[i + 10].time)) {
				if ((i + 15 <= 30) && (saturate_app[i].time * (1.00 - sm_rate) * (1.00 - sm_rate) * (1.00 - sm_rate) < saturate_app[i + 15].time)) {

					sat_point_app->sm = saturate_app[i].sm;
					sat_point_app->tb = saturate_app[i].tb;
					sat_point_app->time = saturate_app[i].time;
					break;
				}
			}
		}
		if (i == 30) {
			sat_point_app->sm = saturate_app[i].sm;
			sat_point_app->tb = saturate_app[i].tb;
			sat_point_app->time = saturate_app[i].time;
		}


	}

}


void allocate_sm(int* sm_cnt, linfo* sat_point_app, int cnt_turn, int app_num, int* reg, int* smem) {
	int i = app_num;
	for (int j = 1; j < 31; j++) { //sm 하나씩 확인 -> app count == 0 인 sm 찾기
		if (*sm_cnt < sat_point_app[i].sm) {
			if (sms[j].app_count == cnt_turn) {
				if (sms[j].reg - reg[i] * sat_point_app[i].tb > 0 && sms[j].smem - smem[i] * sat_point_app[i].tb > 0) {
					sms[j].reg -= reg[i] * sat_point_app[i].tb;
					sms[j].smem -= smem[i] * sat_point_app[i].tb;
					sms[j].app_count++;
					sms[j].app_list.push_back(app_name[i]);
					(*sm_cnt)++;
				}
			}
		}
		else
			return;
	}
}

string toString(vector<string> applist) {
	string s = " ";
	for (int i = 0; i < applist.size(); i++) {
		s += applist[i] + " ";
	}

	return s;
}