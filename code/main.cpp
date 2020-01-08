#include <iostream>
#include <queue>
#include <ctime>
#include <cstdlib>
using namespace std;

#define maxp 40000

class Customer {
public:
	double wt;//wait time
	int prefer;//prefer which dish
	bool picked;//if he has picked his dish
	Customer(){}
	Customer(int n):wt(0),picked(false){
		prefer = genPrefer(n);
	}
	int genPrefer(int n) {
		//[1]equal probability
		//return rand() % n;

		//[2]inequal probability - linear - popular first
		int base = 0;
		for (int i = 1; i <= n; i++) base += i;
		int seed = rand() % base;
		int sum = 0;
		for (int i = 1; i <= n; i++) {
			if (seed >= sum && seed < sum + i) {
				return 30 - i;
			}
			sum += i;
		}
	}
};

int main() {
	//freopen("data.in", "r", stdin);
	int nw = 30;//number of windows
	int np = 40000;//number of total people
	int l; //length of a line
	int cnt;//number of people out
	double pick = 1.0f;//time for picking up dish
	Customer cq[maxp];//queue of customers
	//intialize
	srand(unsigned(time(NULL)));
	for (int i = 0; i < np; i++) {
		cq[i] = Customer(nw);
	}
	for (l = 20; l <= 90; l += 10) {
		cnt = 0;
		for (int i = 0; i < np; i++) {
			cq[i].picked = false;
			cq[i].wt = 0;
		}
		//start service
		int front = 0, tail = 0;//[front,tail] is the people in line
		//int kk = 0;
		while (cnt < np) {
			//cout << cnt << endl;
			//calculate blocking time
			//if (tail == np - 1) {
			//	cout << kk++ << endl;
			//}
			bool flag = false;
			for (int i = front; i <= tail; i++) {
				//perfer dish's window area:[prefer/nw,(prefer+1)/nw]
				//the position of customer:(tail-i)/l
				if (cq[i].picked == false) {
					double pwl = double(cq[i].prefer) / nw;
					double pwr = double(cq[i].prefer + 1) / nw;
					double pc = double(tail - i) / l;
					if (tail == np - 1) {
						//front -> l-1, front+1 -> l-2
						pc = double(l - i - 1 + front) / l;
					}
					//pick his favourite dish
					if (pc >= pwl && pc <= pwr) {
						cq[i].picked = true;
						//only one-time block
						if (flag == false) {
							for (int i = front; i <= tail; i++) {
								cq[i].wt += pick;
							}
							flag = true;
						}
					}
				}
			}
			//unfull
			if (tail == np - 1) {//out
				front++;
				cnt++;
			}
			else if (tail - front + 1 < l) {
				tail++;//in
			}
			//full
			else {
				front++;
				tail++;
				cnt++;
			}
		}
		double E = 0;
		for (int i = 0; i < np; i++) {
			E += cq[i].wt;
			//cout << cq[i].wt << endl;
		}
		E /= np;
		cout << "l:" << l << "\t" << "E:" << E << endl;
	}
	return 0;
}