/*
* Root file for making live histograms
* NIKHEF Project 2010
*
*/
#include <iostream.h>
#include <stdlib.h>
void setStyle() {
gStyle->SetLineWidth(2.);
gStyle->SetTextSize(1.1);
gStyle->SetLabelSize(0.06,"xy");
gStyle->SetTitleSize(0.06,"xy");
gStyle->SetTitleOffset(1.2,"x");
gStyle->SetTitleOffset(1.0,"y");
gStyle->SetPadTopMargin(0.1);
gStyle->SetPadRightMargin(0.1);
gStyle->SetPadBottomMargin(0.16);
gStyle->SetPadLeftMargin(0.12);
}
void Monitor(TString server, Int_t port, Float_t xleft, Float_t xright, Int_t nbins) {
setStyle();
//initialize histogram objects
TH1F *PMT[12];
for (Int_t i=0;i<12;i++) {
	stringstream ss (stringstream::in | stringstream::out) ;
	ss << "pmt" << i;
	TString pmtname=ss.str();
	PMT[i] = new TH1F(pmtname,pmtname,nbins,xleft,xright);
}
TCanvas *c1 = new TCanvas("c1","c1",1200,800);
c1->SetTitle("Histograms");
c1->SetBorderMode(0);
c1->SetFillColor(kWhite);
c1->Divide(4,3);
for (Int_t i=0; i<12; i++) {
	c1->cd(i+1);
	PMT[i]->Draw();
}
cout << "Connecting to " << server << " on port " << port << std::endl;
	TSocket *socket = new TSocket(server,port);
char buf[16];
while (1) {
	Int_t status = socket->RecvRaw(buf,sizeof(buf));
	if (status<=0) {
	std::cout << "Socket error!" << std::endl;
	break;
}
else {
//<< operater expects 0 termination, so might output garbage till 0 byte
	std::cout << "received data: " << buf << std::endl;
}
c1->Update(); //update canvas, otherwise it only draws at exit
}
	std::cout << "Connection closed" << std::endl;
}
/* to save data:
* TNtuple *events = new TNtuple("events","event list","n:p0:p1:p2:p3:p4:p5:p6:p7:p8:p9:p10:
* then use ie events->Fill(0,1241,4214, ...)
*/

