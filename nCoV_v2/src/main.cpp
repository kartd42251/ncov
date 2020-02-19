#include <iostream>
#include <vector>
#include <fstream>
#include <time.h>
#include <math.h>
#include <string>
#include <unistd.h>

#include "train.h"
using namespace std;

int main(int argc,char** argv){
	vector<Infect_Data> train_data;
	vector<Infect_Data> test_data;
	vector<int> predict_data;
    const int sample_num = 24;
	int epoch = 0;
	if(argc==2)
		epoch = stoi(argv[1]);
	else 
		epoch = 10;
	
	get_data(&train_data,"./datafile/outsideWH.txt",sample_num);	
	ofstream of("datafile/adjust.txt",ios::trunc);
	Infect_Data lastday;
	Model Model1(epoch);
	Model1.train(train_data,&lastday);
	Model1.predict(30,sample_num,lastday);

	return 0;
}
