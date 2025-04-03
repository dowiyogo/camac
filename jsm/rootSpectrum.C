#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <TFile.h>
#include <TH1I.h>
#include <TH1F.h>
#include <iostream>

int main(int argc,char *argv[]){
	if(argc<3)
		return 1;

	std::string fileName = argv[2];
	std::ostringstream nombre,command,histo;
	std::cout << argv[3] << " <-Numero de veces que se correra el script" << std::endl;
	nombre<<fileName<<".root";
	TFile* file_final = new TFile(nombre.rdbuf()->str().c_str(),"RECREATE");
	std::cout << "Filename: " << nombre.rdbuf()->str() << std::endl;
	nombre.rdbuf()->str(""); //borrar el buffer
	TH1F* hist;
	/*Primera parte crea los histogramas y obtiene los datos*/
        int max_i = atoi(argv[3]);
	for (int i=1;i<=max_i;i++){
		command<<"./spectra "<<argv[1]<<" "<<fileName<<i<<".dat"<<std::endl;
		std::cout<< "command: " << command.rdbuf()->str() << std::endl;
		std::cout<<"Iteracion numero :"<<i<<std::endl;		
		system(command.rdbuf()->str().c_str());
		command.rdbuf()->str(""); //borrar el buffer
		
		histo<<"histo"<<i;
		std::cout<<"Ahora se guardara el histograma de nombre : "<<histo.rdbuf()->str().c_str()<<std::endl;
	
		hist=new TH1F(histo.rdbuf()->str().c_str(),"histograma",1200,0,1200);
		hist->SetXTitle("Canales");
		hist->SetYTitle("Cuentas");
		hist->SetAxisRange(0,1200,"X");
		
		double Dato=0;// inicializar valores

		nombre<<fileName<<i<<".dat";	
		ifstream lisfile (nombre.rdbuf()->str().c_str());
		while(! lisfile.eof()) {
			lisfile>>Dato;
			hist->Fill(Dato);
		}
		lisfile.close();
                hist->Write(histo.rdbuf()->str().c_str());
	        nombre.rdbuf()->str(""); //borrar el buffer
		histo.rdbuf()->str(""); //borrar el buffer
		delete hist;
		 		
		}
	file_final->Write();
	file_final->Close();
	delete file_final;
 
	return 0;
}

