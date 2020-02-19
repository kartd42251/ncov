#include <iostream>
#include <vector>
#include <fstream>
#include <time.h>
#include <math.h>
#include <string>
#include <unistd.h>

#include "train.h"
using namespace std;


//test
int main(int argc,char** argv){
	vector<Data> train_data;
	vector<Data> test_data;
	vector<int> predict_data;
	const int day_now = 19;
    const int sample_num = 19;
    const int test_num = 2;
	int epoch = 0 ;
	if(argc==2)
		epoch = stoi(argv[1]);
	else 
		epoch = 10;
	
	get_data(&train_data,"./datafile/sample.txt",sample_num);	
	//get_data(&test_data,"./datafile/test.txt",test_num);	
	
	for(int i =0;i<7;i++)
		predict_data.push_back(1+i+day_now);

	ModelTR Model1;
    Model1.set_epoch(epoch);
	Model1.train(train_data);
	//git Model1.evaluate(test_data);
	Model1.predict(predict_data);
	return 0;
}
