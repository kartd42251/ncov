#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
int main(){
	string t0,t1,t2;
	vector<int> Idata,Rdata;
	int a,b;
	
	ifstream ifile("./data.txt");
	while(ifile >> t0 >> t1 >> a >> t2 >> b){
		Idata.push_back(a-b);
		Rdata.push_back(b);
	}

	for(int i=0;i<Idata.size();i++){

		cout << i+1 << " : " << Idata[i] << " ";
		cout << Rdata[i] << endl;
	}
}
