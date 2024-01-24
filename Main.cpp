#include<iostream>
#include<string>
#include<algorithm>
#include<fstream>
#include<iomanip>
#include<Windows.h>
#include<regex>

using namespace std;

void inputToContinue(string reference){
	string input;
	cout<<"����\""<<reference<<"\"�Լ���"<<endl;
	for(;;){
		getline(cin,input);
		if(input==reference)return;
	}
}

void exeExit(){
	string input;
	cout<<R"(����"exit"�˳�����)"<<endl;
	for(;;){
		getline(cin,input);
		if(input=="exit")return;
	}
}

int getSubstringNum(string inputStr,const char*Substring){//��ȡ�Ӵ�����
	size_t findStart,Num=0;
	for(findStart=0;(findStart=inputStr.find(Substring,findStart)+1)-1!=string::npos;Num++);
	return Num;
}

int getSubstringNumWithRegex(string inputStr,regex Substring){//��������ʽ��ȡ�Ӵ�����
	sregex_iterator end,begin(inputStr.begin(),inputStr.end(),Substring);
	return distance(begin,end);
}

void replaceAllSubstring(string&inputStr,const char*beRe,const char*reTo){//�滻�����Ӵ�
	size_t findStart=inputStr.find(beRe);
	for(;findStart!=string::npos;findStart=inputStr.find(beRe)){
		inputStr.replace(findStart,strlen(beRe),reTo);
	}
}

int main(){
	system("color 70");
	string
		ffmpegCMD,

		option00,

		videoC_x265=R"(-c:v libx265 -pix_fmt yuv420p10le -x265-params )",
		param1=R"(crf=19:qpmin=13:qpmax=24:me=star:merange=57:aq-strength=1.2:tu-intra-depth=4)",
		param2=R"(:aq-mode=4:bframes=16:ref=8:rd=5:subme=3:rc-lookahead=80:sao=0:rect=1:amp=0)",
		param3=R"(:rdoq-level=0:psy-rdoq=0.8:psy-rd=2:qcomp=0.7:weightb=1:deblock=-1,-1:min-keyint=2)",
		param4=R"(:cbqpoffs=-1:crqpoffs=-1:keyint=250:pbratio=1.2)",
		option01=videoC_x265+"\""+param1+param2+param3+param4+"\"",

		param5=R"(crf=20:qpmin=13:qpmax=24:me=umh:merange=44:aq-strength=1.2:tu-intra-depth=3)",
		option02=videoC_x265+"\""+param5+param2+param3+param4+":min-cu-size=32:limit-tu=2\"",

		option11=R"(-c:v h264_nvenc -preset lossless -pix_fmt yuv420p10le)";


	//��ȡini�������û�������
	{
		char fillCh='-';string inputStr;
		ifstream ini;
		ini.open("TopazOnCMD.ini");
		cout<<left<<setfill('*')<<setw(160)<<"��ȡ���Ļ���������������:"<<endl;
		for(;getline(ini,inputStr);putchar('\n')){

			if(fillCh=='+')fillCh='-';else fillCh='+';
			cout<<left<<setfill(fillCh)<<setw(120)<<"\""+inputStr+"\"";

			if(_putenv(inputStr.c_str()))
				cout<<"����ʧ��";
			else cout<<"���óɹ�";

		}
		ini.close();
		cout<<left<<setfill('*')<<setw(160)<<"��ȡ����<END>"<<endl;
	}

	//���ffmpeg�汾
	cout<<"\n\n"<<left<<setfill('*')<<setw(120)<<"TVAI_ffmpeg�汾Ϊ:"<<endl;
	system("%TVAI_ffmpeg% -version");
	cout<<"\n\n"<<left<<setfill('*')<<setw(120)<<"Output_ffmpeg�汾Ϊ:"<<endl;
	system("%Output_ffmpeg% -version");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED);
	cout<<left<<setfill('*')<<setw(120)<<"��ϸ��������Ϣ��ȷ��������ٽ�����һ��!"<<endl;
	inputToContinue("continue");
	system("color 70");
	system("cls");


	//����ffmpeg����
InputffmpegCMD:
	cout<<"����ffmpeg����:\n";
	getline(cin,ffmpegCMD);

	//�������Ϸ��Բ������滻
	{

		int CvideoNum=getSubstringNum(ffmpegCMD,R"("-c:v")");//���"-c:v"
		if(CvideoNum<1){
			cout<<"-�����������bug-\n<��Ƶ������λ��ȱʧ>\n(!δ֪����)"<<endl;
			exeExit();
			return 1;
		}
		if(CvideoNum>1){
			cout<<"-�����������bug-\n<��Ƶ������λ��("<<CvideoNum<<")����1>\n(!���Topaz����)"<<endl;
			exeExit();
			return 1;
		}

		int CaudioNum=getSubstringNum(ffmpegCMD,R"("-an")");//���"-an"
		if(CaudioNum<1){
			cout<<"-�����������bug-\n<��������λ��ȱʧ>\n(!��֧�־������\n!���Topaz����)"<<endl;
			exeExit();
			return 1;
		}
		if(CaudioNum>1){
			cout<<"-�����������bug-\n<��������λ��("<<CaudioNum<<")����1>\n(!δ֪����)"<<endl;
			exeExit();
			return 1;
		}

		//���"/"
		if(ffmpegCMD.find("/")!=string::npos){
			cout<<"\n-���������warning-\n<'/'���ܷǷ�>\n"
				<<"ѡ��  C:�Զ��滻'/'  R:������������  P:����warning"<<endl;
			switch(system("choice /c crp")){
				case 1:
					replaceAllSubstring(ffmpegCMD,":\\/",":\\"),replaceAllSubstring(ffmpegCMD,"/","\\");
					cout<<"\n�滻�������:\n"<<ffmpegCMD<<"\n\nѡ��  C:��������  R:������������"<<endl;
					if(system("choice /c cr")==2)goto InputffmpegCMD;
					break;
				case 2:goto InputffmpegCMD;
				case 3:break;
			}
		}

		//·�����
		int pathPointNum=getSubstringNumWithRegex(ffmpegCMD,regex(R"("[a-zA-Z]:(\\[^\\/:*?<>|"\n]*)+")"));
		if(pathPointNum<2){
			cout<<"-�����������bug-\n<·��λ��λ��ȱʧ>"<<endl;
			exeExit();
			return 1;
		}
		if(pathPointNum>2){
			cout<<"-�����������bug-\n<·��λ��("<<pathPointNum<<")����2>"<<endl;
			exeExit();
			return 1;
		}
		//����"^^^"
		{
			regex regex(R"("[a-zA-Z]:(\\[^\\/:*?<>|"\n]*)+")");
			sregex_iterator end,pos(ffmpegCMD.begin(),ffmpegCMD.end(),regex);
			pos++;
			ffmpegCMD.insert(pos->position()," ^^^ ");
		}
	}


	//�滻�Ӵ�
	cout<<"������(����)��ѡ��optionԤ��:\n"
		<<"���  ˵��\n"
		<<"00:   �Զ���\n"
		<<"01:   x265  �߻���-����\n"
		<<"02:   x265  �߻���-����-�߷ֱ��������Ż�����\n"
		<<"11:   h264_nvenc-lossless\n"
		<<endl;
	int optionNum;
ReselectOptionNum:
	cin>>optionNum;
	while(cin.fail()){
		cin.clear();
		cin.ignore();
		cout<<"�Ƿ�����!"<<endl;
		cin>>optionNum;
	}
	cin.ignore();
	string*replaceOption;
	switch(optionNum){
		case 0:
			replaceOption=&option00;
			cout<<"�Զ������������ʾ��:\n-c:v libx264 -preset slow -x264-params \"crf=24:qcomp=0.65\" -pix_fmt yuv420p10le"<<endl;
			getline(cin,option00);
			break;
		case 1:
			replaceOption=&option01;break;
		case 2:
			replaceOption=&option02;break;
		case 11:
			replaceOption=&option11;break;
		default:
			cout<<"��option������!"<<endl;
			goto ReselectOptionNum;
	}
	size_t replaceStart=ffmpegCMD.find(R"("-c:v")"),replaceEnd=ffmpegCMD.find(R"("-an")");
	ffmpegCMD.replace(replaceStart,replaceEnd-replaceStart-1," -pix_fmt yuv422p ");
	ffmpegCMD.replace(ffmpegCMD.find("ffmpeg"),6,"%TVAI_ffmpeg%");
	ffmpegCMD.replace(ffmpegCMD.find("^^^"),3,
		R"( -f yuv4mpegpipe - | %Output_ffmpeg% -hide_banner -f yuv4mpegpipe -i pipe:0 )"+*replaceOption+" ");


	//��ʼ����ffmpeg
	system(ffmpegCMD.c_str());


	exeExit();
	return 0;
}