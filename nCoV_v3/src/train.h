#include <iostream>
#include <time.h>
#include <vector>
#include <fstream>
using namespace std;
#define EXP 2.71828182846
#define START_DAY 20
const int N=57370000;
//const int N=1400050000;

struct Infect_Data{
	int day;
	float S,E,I,R;
};

class Model{
public:
	int epoch;
	const float gamma = 0.07;
	float alpha = 0.2;
	float bias  = -3;
	float c=3.0;
	Model(int e):epoch(e){}
	float loss(vector<float> predict,vector<Infect_Data> data,char mode);	
	void train(vector<Infect_Data> data,Infect_Data* lastday);
	void evaluate(vector<Infect_Data> data);
	void predict(int day,int nowday,Infect_Data data);
	void update(vector<Infect_Data>,vector<float>*,vector<float>*,vector<float>*,vector<float>*);
	void pre_daily_data(int,int,vector<Infect_Data>,vector<float>*,vector<float>*,vector<float>*,vector<float>*,Infect_Data*);
};
void get_data(vector<Infect_Data>* data,string addr,int num){
	ifstream sample(addr);
	
	for(int i = 0 ; i<num; i++){
		char str[10];
		Infect_Data t;
		t.day = i;
		sample.getline(str,10);
		t.I = stof(str);
		sample.getline(str,10);	
		t.R = stof(str);
		t.S = N - t.R - t.I;
		data->push_back(t);
		//print and check the data we get 
		cout << t.day << " " << t.S  << " "<< t.I << " " << t.R << endl;
	}
}	
void write_data(vector<float> S_pre,vector<float> I_pre,vector<float> R_pre,string addr,int nowday){
	ofstream opf(addr,ios::app);
	for(int i =0;i<S_pre.size();i++){
		//if staement used to format month and date
		if(START_DAY+nowday+i <=31)
			opf << "1/"<< START_DAY+nowday+i << " " << S_pre[i] << " " << I_pre[i] << " " << R_pre[i] << endl;
		else if(START_DAY+nowday+i <=31+28)
			opf << "2/"<< nowday+i-11 << " " << S_pre[i] << " " << I_pre[i] << " " << R_pre[i] << endl;
		else 
			opf << "3/"<< nowday+i-11-28 << " " << S_pre[i] << " " << I_pre[i] << " " << R_pre[i] << endl;
	}
}
#define temp pow(EXP,alpha*(bias+i))
#define pre  -2*(data[i].I - (*I_pre)[i])*((*I_pre)[i])*i
void Model::update(vector<Infect_Data> data,vector<float>* S_pre,vector<float>* E_pre,vector<float>* I_pre,vector<float>* R_pre){
	float alpha_eta  =0.0000000001;
	float bias_eta   =0.0000001;
	float c_eta      =0.0000001;
	float alpha_temp =0.0;
	float bias_temp  =0.0;
	float c_temp     =0.0;

	for(int i =0;i<data.size();i++){
		alpha_temp +=   pre * -c*(bias+i)*temp*(temp-1)*pow(temp+1,-3);
		bias_temp  +=  	pre * -alpha*c*temp*(temp-1)*pow(temp+1,-3);
		c_temp     +=	pre * pow(temp,-1)/pow(pow(temp,-1)+1,2);
	}

	cout << "alpha: " << alpha << " bias: " << bias << " c: " << c <<   endl;
	cout << "##DEBUGGER## " <<  alpha_eta*alpha_temp << " " << bias_eta*bias_temp <<" "<< c_eta*c_temp << endl << endl;
	alpha -= alpha_eta*alpha_temp;
	bias  -= bias_eta*bias_temp;
	c     -= c_eta*c_temp;
}
void Model::pre_daily_data(int e,int i,vector<Infect_Data> data,vector<float>* S_pre,vector<float>* E_pre,vector<float>* I_pre,vector<float>* R_pre,Infect_Data* last_day){
	float beta  = c * pow(EXP,-alpha*(i+bias)) / pow(1+pow(EXP,-alpha*(i+bias)),2);
	float beta2 = 0;
	Infect_Data diff;
	cout << "BETA  " << beta << endl;
	cout << "BETA2 " << beta2 << endl;
	if(i==0){
		(*S_pre).push_back(data[0].S);
		(*E_pre).push_back(data[0].E);
		(*I_pre).push_back(data[0].I);
		(*R_pre).push_back(data[0].R);	
	}
	else{
		diff.S =  -beta*data[i-1].S*data[i-1].I/N - beta2*data[i-1].E*data[i-1].S/N;
		diff.E =  beta*data[i-1].I*data[i-1].S/N - alpha*data[i-1].E;
		diff.I =  alpha*data[i-1].E - gamma*data[i-1].I;	
		diff.R =  gamma*data[i-1].I;

		(*S_pre).push_back((*S_pre)[i-1]+diff.S);
		(*E_pre).push_back((*E_pre)[i-1]+diff.E);
		(*I_pre).push_back((*I_pre)[i-1]+diff.I);
		(*R_pre).push_back((*R_pre)[i-1]+diff.R);
	}
	if(e==epoch-1){
		last_day->S = (*S_pre)[data.size()-1];
		last_day->E = (*E_pre)[data.size()-1];
		last_day->I = (*I_pre)[data.size()-1];
		last_day->R = (*R_pre)[data.size()-1];
	}
}
void Model::train(vector<Infect_Data> data,Infect_Data* lastday){
	//change weights in each epoch
	ofstream loss_re("datafile/lossere.txt");

	for(int e=0;e<epoch;e++){
		vector<float> S_pre;
		vector<float> E_pre;
		vector<float> I_pre;
		vector<float> R_pre;
		for(int i =0;i<data.size();i++){		
			pre_daily_data(e,i,data,&S_pre,&E_pre,&I_pre,&R_pre,lastday);
		}	

		//write the data if in the last iteration
		if(e==epoch-1)
			write_data(S_pre,I_pre,R_pre,"datafile/adjust.txt",0);

		cout << "=======================================" << endl;
		cout << "Epoch: " << e+1 << "/" << epoch << endl;
		//print out how much the error is
		cout << "ILOSS: " << loss(I_pre,data,'I') << endl<<endl;
		loss_re << loss(I_pre,data,'I') << endl;	
		//cout << "RLOSS: " << loss(I_pre,data,'R') << endl;		

		//do mahcine learning calculation. update params
		update(data,&S_pre,&E_pre,&I_pre,&R_pre);
		//cin.get();
	}
	
}
float Model::loss(vector<float> predict,vector<Infect_Data> data,char mode){
	float loss_sum = 0.0;
	//if calculating I lost 
	if(mode=='I'){
		for(int i = 0 ;i<predict.size();i++)
			loss_sum += abs(data[i].I-predict[i]);
		return (loss_sum)/data.size();	
	}
	//if calculating R lost 
	else{ 
		for(int i = 0 ;i<predict.size();i++)
			loss_sum += pow(data[i].R-predict[i],2);
		return sqrt(loss_sum);	
	}
}
void Model::predict(int day,int nowday,Infect_Data data){
	vector<float> S_pre;
	vector<float> I_pre;
	vector<float> R_pre;

	for(int i = 0;i<day;i++){
		Infect_Data diff;
		if(i==0){
				S_pre.push_back(data.S);
				I_pre.push_back(data.I);
				R_pre.push_back(data.R);
		}
		else{
			float beta = 1*pow(1+pow(EXP,alpha*(nowday+i)+bias)*c,-1);
			diff.S = -beta*S_pre[i-1]*I_pre[i-1]/N;
			diff.I =  beta*S_pre[i-1]*I_pre[i-1]/N - gamma*I_pre[i-1];
			diff.R =  gamma*I_pre[i-1];
			S_pre.push_back(S_pre[i-1]+diff.S);
			I_pre.push_back(I_pre[i-1]+diff.I);
			R_pre.push_back(R_pre[i-1]+diff.R);	
		}
	}

	// for(int i =0;i<day-1;i++)
	// 	cout << "day" << i+nowday << ": " << S_pre[i+1] << " " << I_pre[i+1] << " " << R_pre[i+1] << endl; 
	
	string addr = "datafile/adjust.txt";
	S_pre.erase(S_pre.begin());
	I_pre.erase(I_pre.begin());
	R_pre.erase(R_pre.begin());
	write_data(S_pre,I_pre,R_pre,addr,nowday);
}
