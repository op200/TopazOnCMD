#include<iostream>
#include<string>
#include<algorithm>
#include<fstream>
#include<iomanip>
#include<Windows.h>

using namespace std;

void inputToContinue(string reference){
	string input;
	cout<<"输入\""<<reference<<"\"以继续"<<endl;
	for(;;){
		getline(cin,input);
		if(input==reference)return;
	}
}

void exeExit(){
	string input;
	cout<<R"(输入"exit"退出程序)"<<endl;
	for(;;){
		getline(cin,input);
		if(input=="exit")return;
	}
}

int getSubstringNum(string inputStr,const char*Substring){//获取子串个数
	size_t findStart,Num=0;
	for(findStart=0;(findStart=inputStr.find(Substring,findStart)+1)-1!=string::npos;Num++);
	return Num;
}

void replaceAllSubstring(string&inputStr,const char*beRe,const char*reTo){//替换所有子串
	size_t findStart=inputStr.find(beRe);
	for(;findStart!=string::npos;findStart=inputStr.find(beRe)){
		inputStr.replace(findStart,strlen(beRe),reTo);
	}
}

int main(){
	system("color 70");
	string
		option00,

		videoC_x265=R"(-c:v libx265 -pix_fmt yuv420p10le -x265-params )",
		param1=R"(crf=19:qpmin=13:qpmax=24:me=star:merange=57:aq-strength=1.2:tu-intra-depth=4)",
		param2=R"(:aq-mode=4:bframes=16:ref=8:rd=5:subme=3:rc-lookahead=80:sao=0:rect=1:amp=0)",
		param3=R"(:rdoq-level=0:psy-rdoq=0.8:psy-rd=2:qcomp=0.72:weightb=1:deblock=-1,-1:min-keyint=2)",
		param4=R"(:cbqpoffs=-1:crqpoffs=-1:keyint=250:pbratio=1.2)",
		option01=videoC_x265+"\""+param1+param2+param3+param4+"\"",

		option02=R"(-c:v h264_nvenc -preset lossless -pix_fmt yuv420p10le)",

		param5=R"(crf=20:qpmin=13:qpmax=24:me=umh:merange=44:aq-strength=1.2:tu-intra-depth=3)",
		param6=R"(:rdoq-level=0:psy-rdoq=0.8:psy-rd=2:qcomp=0.7:weightb=1:deblock=-1,-1:min-keyint=2)",
		param7=R"(:cbqpoffs=-1:crqpoffs=-1:keyint=250:pbratio=1.2:min-cu-size=32:limit-tu=2)",
		option03=videoC_x265+"\""+param5+param2+param6+param7+"\"";


	{//读取ini，以配置环境变量
		_putenv("line=———————————————————————————————————");
		char fillCh='-';string inputStr;
		ifstream ini;
		ini.open("TopazOnCMD.ini");
		cout<<left<<setfill('*')<<setw(160)<<"读取到的环境变量设置如下:"<<endl;
		for(;getline(ini,inputStr);putchar('\n')){

			if(fillCh=='+')fillCh='-';else fillCh='+';
			cout<<left<<setfill(fillCh)<<setw(120)<<"\""+inputStr+"\"";

			if(_putenv(inputStr.c_str()))
				cout<<"配置失败";
			else cout<<"配置成功";

		}
		ini.close();
		cout<<left<<setfill('*')<<setw(160)<<"读取结束<END>"<<endl;
	}

	//输出ffmpeg版本
	cout<<"\n\n"<<left<<setfill('*')<<setw(120)<<"TVAI_ffmpeg版本为:"<<endl;
	system("%TVAI_ffmpeg% -version");
	cout<<"\n\n"<<left<<setfill('*')<<setw(120)<<"Output_ffmpeg版本为:"<<endl;
	system("%Output_ffmpeg% -version");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED);
	cout<<left<<setfill('*')<<setw(120)<<"仔细检查各项信息，确认无误后再进行下一步!"<<endl;
	inputToContinue("continue");
	system("color 70");
	system("cls");


	//传入ffmpeg命令
	string ffmpegCMD;
reInputffmpegCMD:
	cout<<"输入ffmpeg命令:\n";
	getline(cin,ffmpegCMD);


	//检查命令合法性
	{
		int CvideoNum=getSubstringNum(ffmpegCMD,R"("-c:v")");//检查"-c:v"
		if(CvideoNum<1){
			cout<<"-输入的命令有bug-\n<视频编码器位置缺失>\n(!未知错误)"<<endl;
			exeExit();
			return 1;
		}
		else if(CvideoNum>1){
			cout<<"-输入的命令有bug-\n<视频编码器位置("<<CvideoNum<<")大于1>\n(!检查Topaz设置)"<<endl;
			exeExit();
			return 1;
		}

		int CaudioNum=getSubstringNum(ffmpegCMD,R"("-an")");//检查"-an"
		if(CaudioNum<1){
			cout<<"-输入的命令有bug-\n<静音命令位置缺失>\n(!仅支持静音输出\n!检查Topaz设置)"<<endl;
			exeExit();
			return 1;
		}
		else if(CaudioNum>1){
			cout<<"-输入的命令有bug-\n<静音命令位置("<<CaudioNum<<")大于1>\n(!未知错误)"<<endl;
			exeExit();
			return 1;
		}
	}

	if(ffmpegCMD.find("/")!=string::npos){//检查"/"
		cout<<"\n-输入的命令warring-\n<'/'可能非法>\n"
			<<"选择  C:自动替换'/'  R:重新输入命令"<<endl;
		if(system("choice /c cr")==1){
			replaceAllSubstring(ffmpegCMD,":\\/",":\\"),replaceAllSubstring(ffmpegCMD,"/","\\");
			cout<<"\n替换后的命令:\n"<<ffmpegCMD<<"\n\n选择  C:继续程序  R:重新输入命令"<<endl;
			if(system("choice /c cr")==2)goto reInputffmpegCMD;
		}
		else goto reInputffmpegCMD;

	}


	//替换子串
	cout<<"输入编号(整数)以选择option预设:\n"
		<<"编号  说明\n"
		<<"00:   自定义\n"
		<<"01:   x265  高画质-快速\n"
		<<"02:   h264_nvenc-lossless\n"
		<<"03:   x265  高画质-快速-高分辨率速率优化参数\n"
		<<endl;
	int optionNum;
reselectOptionNum:
	cin>>optionNum;
	while(cin.fail()){
		cin.clear();
		cin.ignore();
		cout<<"非法输入!"<<endl;
		cin>>optionNum;
	}
	string*replaceOption;
	switch(optionNum){
		case 0:
			replaceOption=&option00;
			cout<<"自定义编码器参数示例:\n-c:v libx264 -preset slow -x264-params \"crf=24:qcomp=0.65\" -pix_fmt yuv420p10le"<<endl;
			getline(cin,option00);
			break;
		case 1:
			replaceOption=&option01;break;
		case 2:
			replaceOption=&option02;break;
		case 3:
			replaceOption=&option03;break;
		default:
			cout<<"该option不存在!"<<endl;
			goto reselectOptionNum;
	}
	size_t replaceStart=ffmpegCMD.find(R"("-c:v")"),replaceEnd=ffmpegCMD.find(R"("-an")");
	ffmpegCMD.replace(replaceStart,replaceEnd-replaceStart-1," -pix_fmt yuv422p ");
	ffmpegCMD.replace(ffmpegCMD.find("ffmpeg"),6,"%TVAI_ffmpeg%");
	ffmpegCMD.replace(ffmpegCMD.find("^^^"),3,
		R"( -f yuv4mpegpipe - | %Output_ffmpeg% -hide_banner -f yuv4mpegpipe -i pipe:0 )"+*replaceOption+" ");


	//开始运行ffmpeg
	//system("echo %line%>>编码日志.log");
	//system(R"(echo %date:~0,10%--%time:=0%>>编码日志.log)");

	system(ffmpegCMD.c_str());

	//system(R"(echo %date:~0,10%--%time:=0%>>编码日志.log)");
	//system("echo %line%>>编码日志.log");


	exeExit();
	return 0;
}