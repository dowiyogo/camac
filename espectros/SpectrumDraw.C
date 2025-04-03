#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <TFile.h>
#include <TH1I.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <iostream>
#include <math.h>
#include "TApplication.h"

int main(int argc,char *argv[]){
        TApplication  *rootapp=new TApplication("App",&argc,argv);
	std::ostringstream histo;
	TFile* file = new TFile("background_1_43kv.root");
	TH1F* hist [10];
	TCanvas* canvas=new TCanvas("Histogramas");
	std::cout<<"el canvas se divide en 2,5"<<std::endl;
	canvas->Divide(2,5);
	for (int i=1;i<=10;i++){
		std::cout<<"Iteracion numero :"<<i<<std::endl;		
		histo<<"histo"<<i;
		std::cout<<"Ahora se guardara el histograma de nombre : "<<histo.rdbuf()->str().c_str()<<std::endl;
		hist[i-1] = (TH1F*) file->Get(histo.rdbuf()->str().c_str());
		histo.rdbuf()->str(""); //borrar el buffer
		canvas->cd(i);
		hist[i-1]->Draw();
		canvas->Update();
		}
//	file->Close();
//	delete file;
        rootapp->Run();		
	return 0;
}
