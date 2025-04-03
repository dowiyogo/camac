#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <TFile.h>
#include <TH1I.h>
#include <TH1F.h>
#include <iostream>
#include <TNtuple.h>
#include <TPaveText.h>
#include "TTree.h"
#include "TMath.h"
#include <TH1.h>
#include <TF1.h>
#include <TGraphErrors.h>
#include <TROOT.h>
#include <TCanvas.h>


int main(int argc,char *argv[]){
	if(argc<3)
		return 1;

	std::string fileName = argv[2];
	std::ostringstream nombre,command,histo1,histo2;
	std::cout << argv[3] << " <-Numero de veces que se correra el script" << std::endl;
	nombre<<fileName<<".root";
	TFile* file_final = new TFile(nombre.rdbuf()->str().c_str(),"RECREATE");
	std::cout << "Filename: " << nombre.rdbuf()->str() << std::endl;
	nombre.rdbuf()->str(""); //borrar el buffer
	TH1F* h1,h2;
	Float_t* buffer1[],buffer2[];
	/*Primera parte crea los histogramas y obtiene los datos*/
        int max_i = atoi(argv[3]);
	for (int i=1;i<=max_i;i++){
		command<<"./spectra "<<argv[1]<<" "<<fileName<<i<<".dat"<<std::endl;
		std::cout<< "command: " << command.rdbuf()->str() << std::endl;
		std::cout<<"Iteracion numero :"<<i<<std::endl;		
		system(command.rdbuf()->str().c_str());
		command.rdbuf()->str(""); //borrar el buffer
		histo1<<"hist1_"<<i;
		histo2<<"hist2_"<<i;
		nombre<<fileName<<i<<".dat";			
		std::cout<<"Ahora se guardaran los histogramas de nombre : "<<histo1.rdbuf()->str().c_str()<<" y "<<histo2.rdbuf()->str().c_str()<<std::endl;
		TNtuple *tuplein = new TNtuple("tupla","tupla","CH1:CH2");
		tuplein->ReadFile(nombre.rdbuf()->str().c_str());
		h1 = (TH1F*) TH1F(histo1.rdbuf()->str().c_str(),"histograma",1200,0,1200);
		h2 = (TH1F*) TH1F(histo2.rdbuf()->str().c_str(),"histograma",1200,0,1200);
		
		h1->SetXTitle("Canales");
		h2->SetXTitle("Canales");
		h1->SetYTitle("Cuentas");
		h2->SetYTitle("Cuentas");
		h1->SetAxisRange(0,1200,"X");
		h2->SetAxisRange(0,1200,"X");
		
		buffer1[]= tuplein->GetArgs("CH1");
		buffer2[]= tuplein->GetArgs("CH1");

			h1->Fill(buffer1[j]);
			h2->Fill(buffer2[j]);
		
                h1->Write(histo1.rdbuf()->str().c_str());
		h2->Write(histo2.rdbuf()->str().c_str());
	        nombre.rdbuf()->str(""); //borrar el buffer
		histo1.rdbuf()->str(""); //borrar el buffer
		delete h1;
		delete h2;
		 		
		}
	file_final->Write();
	file_final->Close();
	delete file_final;
 
	return 0;
}

