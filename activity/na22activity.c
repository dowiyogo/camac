#include "crate_lib.h"
#include "lecroy_2249A.h" 
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

//llamar a funcion asi..  ./na22 num_event filename.dat ej:  ./spectra 10000 ejemplo.dat
int events;
short CID, res;
char cmd_res[33];
char filename[50],aux[50],activity_file[50];
int i,t0;
int counter=1;
int data=0;
float wait_time;
float percent,remainingtime0,remainingtime1;
FILE *outfile,*actfile;
int year;
int month;
int day;
double sample;


short EnableScalerINHIBIT(short CID){
	if ( NOSOS(CID,2,1)<0){
		printf("Error: couldn't enable INHIBIT\n");
		return -1;
	}
	printf("Scaler count Inhibited\n");
	return 0;
}

short DisableScalerINHIBIT(short CID){
	if ( NOSOS(CID,2,0)<0){
		printf("Error: couldn't disable INHIBIT\n");
		return -1;
	}
	printf("Scaler count allowed\n");
	return 0;
}

void *IRQHandler(short CID, short irq_type, unsigned int irq_data){
	CRATE_OP op;
	if(counter >= events ){ 
		pthread_exit(NULL);
		return;
		}
	switch (irq_type){
		case COMBO_INT:
			usleep(60);
			op.F=2;				
			op.N=11;
			op.A=10;
			res=CSSA(CID,&op);
				if (res<0){
					printf("ERROR: Couldn't read module\n");
			}
			data=op.DATA;
			fprintf(outfile,"%d\t",data);
			op.A=11;
			res=CSSA(CID,&op);
				if (res<0){
					printf("ERROR: Couldn't read module\n");
			}
			data=op.DATA;
			fprintf(outfile,"%d\n",data);
			percent=(float)counter/events*100;
			remainingtime1=(100-percent)/(percent/((int)time(NULL)-t0));
			remainingtime0=(1-0.7)*remainingtime1+0.7*remainingtime0;
			printf("\t%3.0f%%...complete\tremaining time:%3.1f\tcounts:%d\r",percent,remainingtime0,counter);
			fflush(NULL);
			CMDSR(CID,"nim_cack 1\r",cmd_res,32);
			counter++;
			break;
		default: 
			break;
		}
	return;
}

int main(int argc,char *argv[]){

CRATE_INFO crInfo;
cmd_res[32]='\0';
	
	if (argc>8){		/*There are not arguments for input in this executable file*/
		printf("you can insert only seven parameters \n\
			na22 [<num_events>] [<filename>] [uCurie>] [<dd mm yyyy>] [<time_delay>]\n\
			time_delay= [min] uCuries = original activity , dd mm yyyy = manufacturing date\n\n"); 
		return 0;
	}
	else if(argc>7){
		sprintf(filename,"%s",argv[2]);
		events = atoi(argv[1]);
		wait_time = atof(argv[7]);
		sample = atof(argv[3]);
		day = atoi(argv[4]);
		month = atoi(argv[5]);
		year = atoi(argv[6]);		
		}
	else if(argc>6){
		sprintf(filename,"%s",argv[2]);
		events = atoi(argv[1]);
		wait_time = 0.;
		sample = atof(argv[3]);
		day = atoi(argv[4]);
		month = atoi(argv[5]);
		year = atoi(argv[6]);		
		}
	else if(argc>2){
		sprintf(filename,"%s",argv[2]);	
		events = atoi(argv[1]);
		wait_time = 0.;
		sample = 0;
		day = 0;
		month = 0;
		year = 0;	
	}
	else if(argc>1){
		sprintf(filename,"na22.dat");
		events=atoi(argv[1]);
		wait_time = 0.;
		sample = 0.;
		day = 0;
		month = 0;
		year = 0;	
	}
	else {
		sprintf(filename,"na22.dat");
		events=DEFAULT_EVENTS;
		wait_time = 0.;
		sample = 0.;
		day = 0;
		month = 0;
		year = 0;	
	}
	
	sprintf(activity_file,"%s.act",filename);
	
	float elap_min=0.;
	int twtf=0;
	wait_time=wait_time*60.;
	while(twtf!=(int)wait_time)
	{	
		printf("\tWaiting %3.2f[min] to measure Activity => Elapsed time is: %3.2f[min] (%d[s])\r",wait_time/60.,elap_min,twtf);
		fflush(NULL);
		elap_min=(float)twtf/60.;
		sleep(1);
		twtf++;
	}
	printf("\n\n----------------------------------------------------------------------\n");
  	printf ( "Sodium 22 ACTIVITY MEASUREMENT PROGRAM V1.0 -- by R.R.\n");
	printf("----------------------------------------------------------------------\n\n");


	printf("\n");
	if( !(outfile=fopen(filename,"wr")) ){	/*If file can't be opened for writting, fopen returns NULL, that's why if is true if fopen is NULL*/
			printf("ERROR: couldn't open file: %s\n",filename);
			return 0;
	}

	if( !(actfile=fopen(activity_file,"wr")) ){	/*If file can't be opened for writting, fopen returns NULL, that's why if is true if fopen is NULL*/
			printf("couldn't open file: %s\n",activity_file);
			return 0;
	}
	
	if ( (CID=Linit("192.168.1.102"))<0 ){		/*Linit needs CAMAC IP. CID is return from function Linit in Lecroy 2249A driver, output negative is error*/
			printf("ERROR: couldn't initialize device at ip: 192.168.1.102\n");
			return 0;
	}
	
  	time_t rawtime;
  	struct tm * timeinfo;

	time ( &rawtime );
  	timeinfo = localtime ( &rawtime );
	printf("----------------------------------------------------------------------\n");
  	printf ( "Starting Experiment - Local Time and Date: %s", asctime (timeinfo) );
	printf("----------------------------------------------------------------------\n\n");
	LdisableLAM(CID,11);
	LdisableLAM(CID,10);
	LdisableLAM(CID,9);
	double period_gen=Lgen(CID,616,186.2,0,1);
	double period_clock=LSetClock(CID,9,10000);
	EnableScalerINHIBIT(CID);
	LclearScaler(CID,10);
	printf("Coincidence Events to be measured: %d\n",events);
//	fprintf(outfile,"#PMT1\tPMT2\n",data); //chn10,chn11
	t0=(int)time(NULL);
	DisableScalerINHIBIT(CID);
	res=CRIRQ(CID,IRQHandler);
	CRGET(CID,&crInfo);
	pthread_join(crInfo.irq_tid,NULL);
	EnableScalerINHIBIT(CID);
	float rate=events/(float)((int)time(NULL)-t0);
	putchar('\n');
	putchar('\n');
	printf("By COMPUTER: Elapsed time: %d[s] Average Adquisition Rate: %5.2f[Hz]\n",(int)time(NULL)-t0,rate);

	double experiment_time=(((double)LreadScaler(CID,10,0))*period_gen)*1e-6;
	int coincidence=LreadScaler(CID,10,1);
	int PMT1=LreadScaler(CID,10,2);
	int PMT2=LreadScaler(CID,10,3);
	double dead_time=((double)LreadScaler(CID,10,11))*period_clock;
	double real_time=experiment_time-dead_time;
	double activity=events/real_time;
	printf("----------------------------------------------------------------------\n");
	printf("Real Experiment Time: %5.2f[s]\tDead Time: %5.2f[s]\n",experiment_time,dead_time,coincidence,PMT1,PMT2);
	printf("----------------------------------------------------------------------\n");
	printf("Coincidence Events:%d\tPMT1:%d\tPMT2:%d\n",coincidence,PMT1,PMT2);
	printf("----------------------------------------------------------------------\n");
	printf("PMT1 + PMT2 - Coincidence:%d => Total Activity:%5.2f[Beq]\n",(PMT1+PMT2-coincidence),((double)(PMT1+PMT2-coincidence))/experiment_time);
	printf("----------------------------------------------------------------------\n");
	printf("Coincidence Activity:%5.2f [Beq]\n",((double)coincidence)/experiment_time);
	printf("**********************************************************************\n");
	printf("* Experiment Time minus Dead Time:%5.2f[s] Activity:%5.2f[Beq] *\n",real_time,activity);
	printf("**********************************************************************\n\n");
	printf("********************** Labeled Activity Today ************************\n");

	//getting time from manufacturer
	  time_t rawtime2;

	if(((year==0)&&(month==0)&&(day==0)&&(sample==0.)))
	{
	char buffer[50];	  
	printf("Enter the manufacture year of the Na22 Source: ");	
	scanf("%d",&year);
	printf("Enter the manufacture month of the Na22 Source: ");	
	scanf("%d",&month);
	printf("Enter the manufacture day of the Na22 Source: ");	
	scanf("%d",&day);
	printf("Enter the original activity in uCuries: ");	
	scanf("%s",buffer);
	sample=atof(buffer);
	printf("You entered: %d/%d/%d %3.2f[uC]\n",day,month,year,sample);
	}
	  struct tm * timeinfo2;

	  /* get current timeinfo and modify it to the user's choice */
	  time ( &rawtime2 );
	  timeinfo2 = localtime ( &rawtime2 );
	  timeinfo2->tm_year = year - 1900;
	  timeinfo2->tm_mon = month - 1;
	  timeinfo2->tm_mday = day;
//	  mktime ( timeinfo2 ); //gives you seconds for that date
  	double original=sample*pow(10,-6)*3.7*pow(10,10);
	double decay=0.693147181/(2.6027*365*86400);
	double act_curie=sample*pow(2,-(time(NULL)-mktime(timeinfo2))/(2.6027*365*86400));
	double act_na22=original*exp(-decay*(time(NULL)-mktime(timeinfo2)));

	printf("Activity: %5.2f[Beq] in uCuries:%5.5f[uC]\n",act_na22,act_curie); 
	printf("************************* Expected Activity **************************\n");
	fprintf(actfile,"Data File\tActivity\tExperiment Time\tEvents\tTheorical Activity\n");
	sprintf(aux,"%s\t%10.3f\t%10.3f\t%d\t%10.3f",filename,activity,real_time,events,act_na22);
	fprintf(actfile,"%s\n",aux);

	if( fclose(actfile)){	//fclose returns EOF if it fails. In C every value BUT 0 are TRUE. Closing datosoff
		printf("couldn't close file: %s\n",activity_file);
		return 1;
	}

	if( fclose(outfile)){	//fclose returns EOF if it fails. In C every value BUT 0 are TRUE. Closing datosoff
		printf("couldn't close file: %s\n",filename);
		return 1;
	}

	//this lines will close connection with CAMAC, it needs CID
	if ( (Lclose(CID))<0 ){
			printf("couldn't close connection\n");
			return 1;	}	
	return 0;
}
