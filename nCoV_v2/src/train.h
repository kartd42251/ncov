#include <iostream>
#include <time.h>
#include <vector>
#include <fstream>
using namespace std;
#define E 2.71828182846
#define START_DAY 20
const int N=51470000;
//const int N=1400050000;

struct Infect_Data{
	int day;
	float S,I,R;
};

class Model{
public:
	int epoch;
	const float gamma = 0.07;
	float alpha = 0.1;
	float bias  = -20;
	float c     = 3.5;
	Model(int e):epoch(e){}
	float loss(vector<float> predict,vector<Infect_Data> data,char mode);	
	void train(vector<Infect_Data> data,Infect_Data* lastday);
	void evaluate(vector<Infect_Data> data);
	void predict(int day,int nowday,Infect_Data data);
	void gen_daily(vector<Infect_Data>,vector<float>*,vector<float>*,vector<float>*);
	void update(vector<Infect_Data>,vector<float>*,vector<float>*,vector<float>*);
};

void get_data(vector<Infect_Data>* data,string addr){
	string t0,t1,t2;
	Infect_Data t;
	int a,b;
	int cnt = 0;
	ifstream ifile(addr);
	while(ifile >> t0 >> t1 >> a >> t2 >> b){
		t.day = cnt;
		t.I = (a-b);
		t.R = (b);
		t.S = N-a;
		data->push_back(t);
		cnt++;
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

#define temp pow(E,alpha*(bias+i))
#define pre  -2*(data[i].I - (*I_pre)[i])*((*I_pre)[i])*i
void Model::update(vector<Infect_Data> data,vector<float>* S_pre,vector<float>* I_pre,vector<float>* R_pre){
	float alpha_eta  = 0.0000000000001;
	float bias_eta   = 0.000000001;
	float c_eta      = 0.0000000001;
	float alpha_temp = 0.0;
	float bias_temp  = 0.0;
	float c_temp     = 0.0;

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

void Model::gen_daily(vector<Infect_Data> data,vector<float>* S_pre,vector<float>* I_pre,vector<float>* R_pre){
	//cout << "BETA " << beta << endl;
	Infect_Data diff;

	for(int i = 0;i<data.size();i++){
		float beta = c * pow(E,-alpha*(i+bias)) / pow(1+pow(E,-alpha*(i+bias)),2);
		if(i==0){
			(*S_pre).push_back(data[0].S);
			(*I_pre).push_back(data[0].I);
			(*R_pre).push_back(data[0].R);	
		}
		else{
			diff.S = -beta*data[i-1].S*data[i-1].I/N;
			diff.I =  beta*data[i-1].S*data[i-1].I/N - gamma*data[i-1].I;
			diff.R =  gamma*data[i-1].I;
			(*S_pre).push_back((*S_pre)[i-1]+diff.S);
			(*I_pre).push_back((*I_pre)[i-1]+diff.I);
			(*R_pre).push_back((*R_pre)[i-1]+diff.R);
		}
	}
}
void Model::train(vector<Infect_Data> data,Infect_Data* last_day){
	//change weights in each epoch
	ofstream loss_re("datafile/lossere.txt");
	ofstream beta_re("datafile/betare.txt");
	for(int e=0;e<epoch;e++){
		vector<float> S_pre;
		vector<float> I_pre;
		vector<float> R_pre;
		vector<float> beta_recoder;
		// for(int i =0;i<data.size();i++)	
		// 	gen_daily(i,data,&S_pre,&I_pre,&R_pre);
		gen_daily(data,&S_pre,&I_pre,&R_pre);

		//write the data if in the last iteration
		if(e==epoch-1){
			write_data(S_pre,I_pre,R_pre,"datafile/adjust.txt",0);
			last_day->S = S_pre[data.size()-1];
			last_day->I = I_pre[data.size()-1];
			last_day->R = R_pre[data.size()-1];
			for(int i =0;i<data.size();i++)
				beta_re << (c * pow(E,-alpha*(i+bias)) / pow(1+pow(E,-alpha*(i+bias)),2)) << endl;
		}

		cout << "=======================================" << endl;
		cout << "Epoch: " << e+1 << "/" << epoch << endl;
		//print out how much the error is
		cout << "ILOSS: " << loss(I_pre,data,'I') << endl<<endl;
		loss_re << loss(I_pre,data,'I') << endl; 
		//cout << "RLOSS: " << loss(I_pre,data,'R') << endl;		
	
		update(data,&S_pre,&I_pre,&R_pre);
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

	for(int i = nowday;i<day+nowday;i++){
		Infect_Data diff;
		if(i==nowday){
				S_pre.push_back(data.S);
				I_pre.push_back(data.I);
				R_pre.push_back(data.R);
		}
		else{
			float beta = c * pow(E,-alpha*(i+bias)) / pow(1+pow(E,-alpha*(i+bias)),2);

			diff.S = -beta*S_pre[i-1-nowday]*I_pre[i-1-nowday]/N;
			diff.I =  beta*S_pre[i-1-nowday]*I_pre[i-1-nowday]/N - gamma*I_pre[i-1-nowday];
			diff.R =  gamma*I_pre[i-1-nowday];
			S_pre.push_back(S_pre[i-1-nowday]+diff.S);
			I_pre.push_back(I_pre[i-1-nowday]+diff.I);
			R_pre.push_back(R_pre[i-1-nowday]+diff.R);	
		}
	}

	// for(int i = 0 ; i<day-1 ; i++)
	// 	cout << "day" << i << ": " << S_pre[i] << " " << I_pre[i] << " " << R_pre[i] << endl; 
	
	string addr = "datafile/adjust.txt";
	S_pre.erase(S_pre.begin());
	I_pre.erase(I_pre.begin());
	R_pre.erase(R_pre.begin());
	write_data(S_pre,I_pre,R_pre,addr,nowday);
}
