// threadblock_scheduling.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <math.h>
#define MAXREG 65536
#define MAXSMEM 49152
using namespace std;
int query_reg(string app_name);
int query_smem(string app_name);
void even_allocate(string app_name, int app_count);
int main() {

	cout << "Even partitioning" << endl << endl;

	string app_name[3];
	int app_count = 2;
	cout << "1번 응용의 이름을 입력하세요" << endl;
	cin >> app_name[0];

	cout << "2번 응용의 이름을 입력하세요" << endl;
	cin >> app_name[1];

	if (app_count >= 3) {
		cout << "3번 응용의 이름을 입력하세요" << endl;
		cin >> app_name[2];
	}
	even_allocate(app_name[0],app_count);
	even_allocate(app_name[1], app_count);
	if (app_count >= 3) {
		even_allocate(app_name[2], app_count);
	}
}

void even_allocate(string app_name, int app_count) {

	int reg, smem;
	reg = query_reg(app_name); 
	smem = query_smem(app_name);

	if (smem != 0) {
		cout << app_name << "에게 할당된 TB 개수는 " << min(MAXREG /app_count / reg, MAXSMEM / app_count / smem) << "입니다." << endl;
	}
	else
		cout << app_name << "에게 할당된 TB 개수는 " <<MAXREG / app_count / reg << "입니다." << endl;
}

