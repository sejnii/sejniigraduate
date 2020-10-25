#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

struct linfo
{
	int sm;
	int tb;
	double time;
};
struct kernel_info
{
	string name;
	double exec_time;
	int category; // 0 : L1, 1 : Mem, 2 : Compute
};
bool cmp(const kernel_info &a, const kernel_info &b)
{
	if (a.category < b.category)
		return true;
	else if (a.category == b.category)
		if (a.exec_time < b.exec_time)
			return true;
	return false;
}

double query_exec_time(string app_name)
{
	if (app_name.compare("cutcp") == 0)
		return 46;
	else if (app_name.compare("spmv") == 0)
		return 68;
	else if (app_name.compare("lbm") == 0)
		return 94;
	else if (app_name.compare("stencil") == 0)
		return 59;
	else if (app_name.compare("tpacf") == 0)
		return 0;
	else if (app_name.compare("lava") == 0)
		return 58;
	else if (app_name.compare("bs") == 0)
		return 25;
	else if (app_name.compare("fdtd") == 0)
		return 17;
	else if (app_name.compare("qs") == 0)
		return 69;
	else if (app_name.compare("sg") == 0)
		return 55;
	else if (app_name.compare("cp") == 0)
		return 8.7;
	else if (app_name.compare("rd") == 0)
		return 30;
	else if (app_name.compare("cov") == 0)
		return 339;
	else if (app_name.compare("sy") == 0)
		return 72;
	else if (app_name.compare("conv") == 0)
		return 9.7;
	else if (app_name.compare("nw") == 0)
		return 23;
	else if (app_name.compare("hs") == 0)
		return 33;
	else if (app_name.compare("dxtc") == 0)
		return 14.8;
	else if (app_name.compare("bo") == 0)
		return 27.5;
}

double query_dp_gflops(string app_name)
{

	if (app_name.compare("lava") == 0)
		return 184.2948;
	else if (app_name.compare("sy") == 0)
		return 77.4519432434177;
	else
		return 0;
}

double query_sp_gflops(string app_name)
{

	if (app_name.compare("cutcp") == 0)
		return 2480.79336363317;
	else if (app_name.compare("spmv") == 0)
		return 81.5301748030739;
	else if (app_name.compare("lbm") == 0)
		return 70.5981342269178;
	else if (app_name.compare("stencil") == 0)
		return 287.680001968426;
	else if (app_name.compare("tpacf") == 0)
		return 0;

	else if (app_name.compare("bs") == 0)
		return 1486.04754303538;

	else if (app_name.compare("fdtd") == 0)
		return 281.817469285619;
	else if (app_name.compare("qs") == 0)
		return 1743.54469579885;

	else if (app_name.compare("cp") == 0)
		return 2693.35062015917;
	else if (app_name.compare("sg") == 0)
		return 4204.03951793197;
	else if (app_name.compare("rd") == 0)
		return 38.3986082727578;
	else if (app_name.compare("cov") == 0)
		return 5.4781127872039;
	else if (app_name.compare("conv") == 0)
		return 356.810413101572;
	else if (app_name.compare("nw") == 0)
		return 0;
	else if (app_name.compare("hs") == 0)
		return 521.536896833375;
	else if (app_name.compare("dxtc") == 0)
		return 2455.82746542285;
	else if (app_name.compare("bo") == 0)
		return 2444.242942;

	else
		return 0;
}

double query_thrpt(string app_name)
{
	if (app_name.compare("cutcp") == 0)
		return 1.9876;
	else if (app_name.compare("spmv") == 0)
		return 349.135;
	else if (app_name.compare("lbm") == 0)
		return 68.182;
	else if (app_name.compare("stencil") == 0)
		return 314.13;
	else if (app_name.compare("tpacf") == 0)
		return 0;
	else if (app_name.compare("lava") == 0)
		return 18.7299;
	else if (app_name.compare("bs") == 0)
		return 397.72;

	else if (app_name.compare("fdtd") == 0)
		return 99.52;
	else if (app_name.compare("qs") == 0)
		return 219.44349;

	else if (app_name.compare("cp") == 0)
		return 0.022;
	else if (app_name.compare("sg") == 0)
		return 71.4116;
	else if (app_name.compare("rd") == 0)
		return 16.411;
	else if (app_name.compare("cov") == 0)
		return 23.3328843;
	else if (app_name.compare("sy") == 0)
		return 76.792;
	else if (app_name.compare("conv") == 0)
		return 159.879;
	else if (app_name.compare("nw") == 0)
		return 80.58;
	else if (app_name.compare("hs") == 0)
		return 360.54;
	else if (app_name.compare("dxtc") == 0)
		return 161.4853;
	else if (app_name.compare("bo") == 0)
		return 2.6176;
}

float query_epc(string app_name)
{
	if (app_name.compare("cutcp") == 0)
		return 5.67;
	else if (app_name.compare("spmv") == 0)
		return 0.72;
	else if (app_name.compare("lbm") == 0)
		return 0.59;
	else if (app_name.compare("stencil") == 0)
		return 5.68;
	else if (app_name.compare("tpacf") == 0)
		return 0;
	else if (app_name.compare("lava") == 0)
		return 0.17;
	else if (app_name.compare("bs") == 0)
		return 4.04;

	else if (app_name.compare("fdtd") == 0)
		return 0.55;
	else if (app_name.compare("qs") == 0)
		return 10.06;

	else if (app_name.compare("cp") == 0)
		return 3.4;
	else if (app_name.compare("sg") == 0)
		return 4.02;
	else if (app_name.compare("rd") == 0)
		return 0.52;
	else if (app_name.compare("cov") == 0)
		return 0.08;
	else if (app_name.compare("sy") == 0)
		return 0.12;
	else if (app_name.compare("conv") == 0)
		return 0.86;
	else if (app_name.compare("nw") == 0)
		return 0.31;
	else if (app_name.compare("hs") == 0)
		return 3.4;
	else if (app_name.compare("dxtc") == 0)
		return 3.74;
	else if (app_name.compare("bo") == 0)
		return 5.92;
}

int query_reg(string app_name)
{
	if (app_name.compare("cutcp") == 0)
		return 3328;
	else if (app_name.compare("spmv") == 0)
		return 5148;
	else if (app_name.compare("lbm") == 0)
		return 4800;
	else if (app_name.compare("stencil") == 0)
		return 4096;
	else if (app_name.compare("tpacf") == 0)
		return 7936;
	else if (app_name.compare("lava") == 0)
		return 7168;
	else if (app_name.compare("bs") == 0)
		return 2944;

	else if (app_name.compare("fdtd") == 0)
		return 5120;
	else if (app_name.compare("qs") == 0)
		return 15360;
	else if (app_name.compare("cp") == 0)
		return 4224;
	else if (app_name.compare("sg") == 0)
		return 5376;
	else if (app_name.compare("rd") == 0)
		return 4608;
	else if (app_name.compare("cov") == 0)
		return 5632;
	else if (app_name.compare("sy") == 0)
		return 6144;
	else if (app_name.compare("conv") == 0)
		return 5120;
	else if (app_name.compare("nw") == 0)
		return 656;
	else if (app_name.compare("hs") == 0)
		return 8192;
	else if (app_name.compare("dxtc") == 0)
		return 4032;
	else if (app_name.compare("bo") == 0)
		return 4096;
}

int query_smem(string app_name)
{
	if (app_name.compare("cutcp") == 0)
		return 4019;
	else if (app_name.compare("spmv") == 0)
		return 0;
	else if (app_name.compare("lbm") == 0)
		return 0;
	else if (app_name.compare("stencil") == 0)
		return 1000;
	else if (app_name.compare("tpacf") == 0)
		return 13000;
	else if (app_name.compare("lava") == 0)
		return 7200;
	else if (app_name.compare("bs") == 0)
		return 0;
	else if (app_name.compare("fdtd") == 0)
		return 1500;
	else if (app_name.compare("qs") == 0)
		return 0;
	else if (app_name.compare("sg") == 0)
		return 512;
	else if (app_name.compare("cp") == 0)
		return 0;
	else if (app_name.compare("rd") == 0)
		return 2000;
	else if (app_name.compare("cov") == 0)
		return 0;
	else if (app_name.compare("sy") == 0)
		return 0;
	else if (app_name.compare("conv") == 0)
		return 0;
	else if (app_name.compare("nw") == 0)
		return 2180;
	else if (app_name.compare("hs") == 0)
		return 0;
	else if (app_name.compare("dxtc") == 0)
		return 2048;
	else if (app_name.compare("bo") == 0)
		return 516;
}

int query_category(string app_name){
// 0 : L1, 1 : Mem, 2 : Compute
if (app_name.compare("cutcp") == 0)
		return 2;
	else if (app_name.compare("spmv") == 0)
		return 1;
	else if (app_name.compare("lbm") == 0)
		return 0;
	else if (app_name.compare("stencil") == 0)
		return 1;
	else if (app_name.compare("tpacf") == 0)
		return 2;
	else if (app_name.compare("lava") == 0)
		return 2;
	else if (app_name.compare("bs") == 0)
		return 1;
	else if (app_name.compare("fdtd") == 0)
		return 2;
	else if (app_name.compare("qs") == 0)
		return 2;
	else if (app_name.compare("sg") == 0)
		return 2;
	else if (app_name.compare("cp") == 0)
		return 2;
	else if (app_name.compare("rd") == 0)
		return 1;
	else if (app_name.compare("cov") == 0)
		return 1;
	else if (app_name.compare("sy") == 0)
		return 1;
	else if (app_name.compare("conv") == 0)
		return 0;
	else if (app_name.compare("nw") == 0)
		return 1;
	else if (app_name.compare("hs") == 0)
		return 1;
	else if (app_name.compare("dxtc") == 0)
		return 2;
	else if (app_name.compare("bo") == 0)
		return 2;


}

void get_info(string app_name, vector<vector<double>> *app, int iter)
{
	string line;
	ifstream file;

	file.open(app_name + ".txt");

	if (file.is_open())
	{
		while (getline(file, line))
		{
			istringstream iss(line);
			string word;
			iss >> word; // sm
			int sm = stoi(word);

			iss >> word; // tb

			iss >> word;
			double time = stod(word);
			app->resize(31);
			//	app.resize(31);
			app->at(sm).push_back(time);
			//		app.at(sm).push_back(time);

			//cout << line << endl;
		}
	}
}

void get_info(string app_name, vector<double> *infovec)
{

	int sm = 0, tb = 0;
	double time = 0;
	string line;
	ifstream file;

	file.open(app_name + ".txt");

	if (file.is_open())
	{
		while (getline(file, line))
		{
			istringstream iss(line);
			string word;
			iss >> word; // sm
			sm = stoi(word);

			iss >> word; // tb
			tb = stoi(word);

			iss >> word;
			time = stod(word);

			//infovec.push_back({ sm, tb, time });
			infovec[sm].push_back(time);
		}
	}
}
