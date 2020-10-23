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
	vector<string> app_list;
};

bool my_cmp(const linfo& a, const linfo& b) {
	return a.time < b.time;
}
struct SM sms[30];

void initsm() {
	for (int i = 0; i < 30; i++) {
		sms[i].reg = MAXREG;
		sms[i].smem = MAXSMEM;
		sms[i].app_list.clear();
	}

}
int query_reg(string app_name);
int query_smem(string app_name);
void get_info(string app_name, vector<linfo> *infovec);

string app_name[3];
int main()
{
	int sm = 0, tb = 0;
	double time = 0;
	vector <linfo> infovec1, infovec2;

	cout << "Negotiation and leftover" << endl << endl;

	cout << "1번 응용의 이름을 입력하세요" << endl;
	cin >> app_name[0];
	get_info(app_name[0], &infovec1);

	cout << "2번 응용의 이름을 입력하세요" << endl;
	cin >> app_name[1];
	get_info(app_name[1], &infovec2);
	

	cout << "3번 응용의 이름을 입력하세요" << endl;
	cin >> app_name[2];
/*
	for (int i = 0; i < infovec1.size(); i++) {
		cout << infovec1[i].sm << " " << infovec1[i].tb << " " << infovec1[i].time << endl;

	}
	cout << endl;
	for (int i = 0; i < infovec2.size(); i++) {
		cout << infovec2[i].sm << " " << infovec2[i].tb << " " << infovec2[i].time << endl;

	}
	*/

	int reg1, smem1, reg2, smem2, reg3, smem3;
	reg1 = query_reg(app_name[0]); smem1 = query_smem(app_name[0]);
	reg2 = query_reg(app_name[1]); smem2 = query_smem(app_name[1]);
	reg3 = query_reg(app_name[2]); smem3 = query_smem(app_name[2]);



	sort(infovec1.begin(), infovec1.end(), my_cmp);
	sort(infovec2.begin(), infovec2.end(), my_cmp);
	


	initsm();
	// a app, b app 모두 best case 부터 시작
	int a=0, b = 0;
	int regtotal = 0, smemtotal = 0;
	bool flag = 0; //flag 1이면 스케줄링 성공
	


	while (a < infovec1.size() && b < infovec2.size() && flag ==0) {
		
		// a=0, b=0 

		// a app에 자원 할당, a app에 할당해주고 남은 자원 계산
		for (int i = 0; i < infovec1[a].sm; i++) {
			sms[i].reg -= infovec1[a].tb * reg1;
			sms[i].smem -= infovec1[a].tb * smem1;

		}

		// b app에 자원 할당해 줄 수 있는지 확인 -> 할당해 줄 수 있다면 flag 1
		int cnt = 0;
		for (int i = 0; i < 30; i++) {
			if ((sms[i].reg - infovec2[b].tb * reg2 >= 0) && (sms[i].smem - infovec2[b].tb * smem2 >= 0)) {
				sms[i].reg -= infovec2[b].tb * reg2;
				sms[i].smem -= infovec2[b].tb * smem2;

				cnt++;
			}

			if (cnt >= infovec2[b].sm) {
				flag = 1;
				break;
				
			}
		}

		if (flag == 1)
			break;

		// a-0, b=1 & a=1, b=0 일 때 둘 다 된다면 사용된 자원량을 가지고 판단
		int reg_used1 = 0, reg_used2=0;
		int smem_used1 = 0, smem_used2 = 0;

		// a=0, b=1
		initsm();
		b++;

		// a app에 자원 할당, a app에 할당해주고 남은 자원 계산
		for (int i = 0; i < infovec1[a].sm; i++) {
			sms[i].reg -= infovec1[a].tb * reg1;
			sms[i].smem -= infovec1[a].tb * smem1;

		}

		// b app에 자원 할당해 줄 수 있는지 확인 -> 할당해 줄 수 있다면 flag 1
		cnt = 0;
		for (int i = 0; i < 30; i++) {
			if ((sms[i].reg - infovec2[b].tb * reg2 >= 0) && (sms[i].smem - infovec2[b].tb * smem2 >= 0)) {
				sms[i].reg -= infovec2[b].tb * reg2;
				sms[i].smem -= infovec2[b].tb * smem2;
				cnt++;
			}

			if (cnt >= infovec2[b].sm) {
				
				flag = 1;
				reg_used1 = infovec1[a].tb * reg1 * infovec1[a].sm + infovec2[b].tb * reg2 * infovec2[b].sm;
				smem_used1 = infovec1[a].tb * smem1 * infovec1[a].sm + infovec2[b].tb * smem2 * infovec2[b].sm;
				
				break;
			}
		}

		// a=1, b=0
		initsm();
		b--; a++;

		// a app에 자원 할당, a app에 할당해주고 남은 자원 계산
		for (int i = 0; i < infovec1[a].sm; i++) {
			sms[i].reg -= infovec1[a].tb * reg1;
			sms[i].smem -= infovec1[a].tb * smem1;
			
		}

		// b app에 자원 할당해 줄 수 있는지 확인 -> 할당해 줄 수 있다면 flag 1
		cnt = 0;
		for (int i = 0; i < 30; i++) {
			if ((sms[i].reg - infovec2[b].tb * reg2 >= 0) && (sms[i].smem - infovec2[b].tb * smem2 >= 0)) {
				sms[i].reg -= infovec2[b].tb * reg2;
				sms[i].smem -= infovec2[b].tb * smem2;
			
				cnt++;
			}

			if (cnt >= infovec2[b].sm) {

				flag = 1;
				reg_used2 = infovec1[a].tb * reg1 * infovec1[a].sm + infovec2[b].tb * reg2 * infovec2[b].sm;
				smem_used2 = infovec1[a].tb * smem1 * infovec1[a].sm + infovec2[b].tb * smem2 * infovec2[b].sm;
				
				break;
			}
		}
		a--;

		if (flag == 1 && reg_used1 > 0 && reg_used2 > 0) {
			// a=0, b=1 일 때 register 덜 쓰면 이거 선택
			if (reg_used1 < reg_used2) {
				b++;
				break;
			}

			else {
				a++;
				break;
			}

		}
		else if (flag == 1) {
			if (reg_used1 > 0) {
				b++;
				break;
			}
			else {
				a++;
				break;
			}
		}

		a++; b++;

	}

	cout << "a : " << a << ", b: " << b << endl;
	initsm();
	for (int i = 0; i < infovec1[a].sm; i++) {
		sms[i].reg -= infovec1[a].tb * reg1;
		sms[i].smem -= infovec1[a].tb * smem1;
		sms[i].app_list.push_back(app_name[0]);
	}

	int cnt = 0;
	for (int i = 0; i < 30; i++) {
		if ((sms[i].reg - infovec2[b].tb * reg2 >= 0) && (sms[i].smem - infovec2[b].tb * smem2 >= 0)) {
			sms[i].reg -= infovec2[b].tb * reg2;
			sms[i].smem -= infovec2[b].tb * smem2;
			sms[i].app_list.push_back(app_name[1]);
			cnt++;
		}

		if (cnt >= infovec2[b].sm) {
			break;
		}
	}

	cout <<app_name[0]<< "의 SM 개수 : " << infovec1[a].sm << ", a app의 tb 개수 : " << infovec1[a].tb << endl;
	cout << app_name[1] << "의 SM 개수 : " << infovec2[b].sm << ", b app의 tb 개수 : " << infovec2[b].tb << endl;

	cout << "각 SM의 남은 자원량" << endl;


	
	
	int app3_sm_allocate[30];
	for (int i = 0; i < 30; i++) {
		//cout << reg3 << endl;
		//cout << sms[i].reg / reg3 << endl;
		//cout << sms[i].smem / smem3 << endl;
		int tb = 0;
		if (smem3 == 0)
			tb = sms[i].reg / reg3;
		else
			tb = min(sms[i].reg / reg3, sms[i].smem / smem3);
		app3_sm_allocate[i] = tb;
		cout <<app_name[2]<< "의 " << i << "번째 SM TB 개수 : " << tb<<endl;
		if (tb > 0)
			sms[i].app_list.push_back(app_name[2]);
	}


	for (int i = 0; i < 30; i++) {
		cout << "sm " << i << "번 : ";

		for (int j = 0; j < sms[i].app_list.size(); j++) {
			cout << sms[i].app_list[j];
			if (j != sms[i].app_list.size() - 1)
				cout << ", ";

		}
		cout << endl;

	}

}

