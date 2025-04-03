//{
//  gROOT->Reset();
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <TFile.h>
#include <TH1I.h>
#include <TH1F.h>
#include <TServerSocket.h>
#include <TSocket.h>
#include <iostream>
#include <TUnixSystem.h>

int main(int argc,char *argv[]){
ofstream myfile;
myfile.open ("example.txt");
//TNtuple *QDC = new TNtuple("QDC","Channels","pmt:ch1:ch2:ch3:ch4:ch5:ch6:ch7:ch8:ch9:ch10:ch11:ch12:ch13:ch14:ch15:ch16");
TServerSocket *ss = new TServerSocket(6666,kTRUE); 
TSocket *socket = ss->Accept();
ss->SetOption(kNoDelay,1);
char buf[86];
//std::ostringstream buf;
//TString *buf=new TString();
int i=0;
for (i=0;i<1000;i++) {
	Int_t status = socket->RecvRaw(buf,86);
	if (status<=0) {
	std::cout << "Socket error!" <<"\n";}
	else {myfile<<buf<<"\n";
	std::cout<<"cuentas "<<i<<"\r";
	fflush(NULL);
	}
}
myfile.close();
 return 0;
}
