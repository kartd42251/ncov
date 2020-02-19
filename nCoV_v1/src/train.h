#include <iostream>
#include <time.h>
#include <vector>
#include <fstream>
using namespace std;
struct Data{
	int day;
	int num;
};

class Model{
public:
	int epoch;
	void set_epoch(int);
	float loss(vector<float> predict,vector<Data> data);	
};
class ModelTR:public Model{
public:
	float w1=0.0,w2=0.0,w3=0.0,b=0.0;
	void train(vector<Data> data);
	void evaluate(vector<Data> data);
	void predict(vector<int> data);

};

void get_data(vector<Data>* data,string addr,int num){
	ifstream sample(addr);
	for(int i = 0 ; i<num; i++){
		char str[10];
		Data t;
			
		t.day = i+1;
		sample.getline(str,10);	
		t.num = stoi(str);
		
		data->push_back(t);
		cout << t.day << " " << t.num << endl;
	}
}	
void Model::set_epoch(int e){
	epoch = e;
}
float Model::loss(vector<float> predict,vector<Data> data){
	float loss_sum = 0.0;
	for(int i = 0 ;i<predict.size();i++)
		loss_sum += pow(data[i].num-predict[i],2);
	return sqrt(loss_sum);	
}
void ModelTR::train(vector<Data> data){
	float w3_eta = 0.0000000001;
	float w2_eta = 0.00000001;
	float w1_eta = 0.000001;
	float b_eta = 0.000001;
	for(int e = 0;e<epoch;e++){
		vector<float> predict;
		for(int i =0;i<data.size();i++)
			predict.push_back ( w3*pow(data[i].day,3) + w2*pow(data[i].day,2) + w1*data[i].day + b );
				
		cout << "w3: "<< w3 << " w2: " << w2 << " w1: " << w1 << " b: " << b << endl;
		cout << "======================================="<< endl;
		cout << "Epoch: " << e+1 << "/"  << epoch << endl;
		cout << "LOSS: " << loss(predict,data) << endl;
		cout << endl;

		//update//
		float w1_temp = 0.0;
		float w2_temp = 0.0;
		float w3_temp = 0.0;
		float b_temp = 0.0;
		
		for(int i =0;i<data.size();i++){
			w3_temp += -2*(data[i].num-predict[i]) * pow((data[i].day),3);
			w2_temp += -2*(data[i].num-predict[i]) * pow((data[i].day),2);
			w1_temp += -2*(data[i].num-predict[i]) * (data[i].day);
			b_temp  += -2*(data[i].num-predict[i]);
		}
		cout << "##DEBUGGER##" << endl << w3_temp << " " << w2_temp << " " << w1_temp << " " << b_temp<< endl << endl;
		//adjust
		w1 -= w1_eta * w1_temp;
		w2 -= w2_eta * w2_temp;
		w3 -= w3_eta * w3_temp;
		b  -= b_eta  * b_temp;	

	}
}
void ModelTR::evaluate(vector<Data> data){
	float sum= 0.0;
	vector<float> pre_y;
	for(int i = data[0].day ;i<data.size();i++)
		pre_y.push_back( w3 * pow(data[i].day,3) + w2 * pow(data[i].day,2) + w1 * data[i].day + b );
	
	for(int i = data[0].day ;i<data[0].day+data.size();i++){
		sum+= pow((data[i].num-pre_y[i]),2);
		cout << data[i].day << endl;
		cout << "#2: "<< data.size() << endl;
		//cout << w3 * pow(data[i].day,3) + w2 * pow(data[i].day,2) + w1 * data[i].day + b << endl;
		//cout << pre_y[i] << endl;
	}
	cout << "TestLoss: " << sqrt(sum/data.size()) << endl;
}
void ModelTR::predict(vector<int> data){
	vector<int> pre_y;
	for(int i = 0 ;i<data.size();i++){
		int result = ( w3 * pow(data[i],3) + w2 * pow(data[i],2) + w1 * data[i] + b );
		pre_y.push_back(result);
		cout << "\033[41;33mday " <<i+data[0] <<" predict case: " << result << "\033[0;0m " << endl;
	}
}

