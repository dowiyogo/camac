#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>



int main(int argc,char *argv[]){
	if(argc<2)
		{return 1;}
	FILE *outfile;
	std::ostringstream command;
	std::string fileName = argv[2];
	char filelist[50];
	sprintf(filelist,"%s.lst",argv[2]);
	if( !(outfile=fopen(filelist,"wr")) ){	/*If file can't be opened for writting, fopen returns NULL, that's why if is true if fopen is NULL*/
			printf("couldn't open file: %s\n",filelist);
			return 1;
	}
	std::cout << argv[3] << " <-Numero de veces que se correra el script" << std::endl;
	std::cout << "Filename List: " << argv[2] <<".lst" << std::endl;

        int max_i = atoi(argv[3]);
	for (int i=1;i<=max_i;i++){
		fprintf(outfile,"%s%d.dat\n",argv[2],i);
		command<<"./spectra "<<argv[1]<<" "<<fileName<<i<<".dat"<<std::endl;
		std::cout<< "command: " << command.rdbuf()->str() << std::endl;
		std::cout<<"Iteracion numero :"<<i<<std::endl;		
		system(command.rdbuf()->str().c_str());
		command.rdbuf()->str(""); //borrar el buffer
	}
 	if( fclose(outfile)){	//fclose returns EOF if it fails. In C every value BUT 0 are TRUE. Closing datosoff
		printf("couldn't close file: %s\n",filelist);
		return 1;
	}
	return 0;
}

