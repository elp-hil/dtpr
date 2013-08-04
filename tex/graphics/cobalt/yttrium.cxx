#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>

#include <KaLi.h>
#include <KLHistogramEvent.h>
#include "TApplication.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TF1.h"
#include "TGraph.h"
#include "TFile.h"
#include "TFolder.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TTree.h"
#include "TBranch.h"

#include "run.h"

int main(int argc, char* argv[]) {
	TApplication *Rate = new TApplication("Rate", &argc, argv);
	TCanvas *rates = new TCanvas("rates");
	rates->Connect("Closed()", "TApplication", Rate, "Terminate()");
	TGraph *plot = new TGraph;
	KaLi::KLDataManager* gDataManager = new KaLi::KLDataManager(
			"http://ipekatrinadei.ka.fzk.de/katrin/kali.fcgi");
	std::fstream file;
	file.open("/kalinka/home/rovedo/Analysis/yttriumRate/rate.dat");
	if(!file.is_open()){
		std::cout << "Error opening file..." << std::endl;
	}
	if (argc < 2) {
		std::cerr << "Usage: rate <startrun> <endrun>" << std::endl
				<< "or rate <run>" << std::endl;
		return 0;
	}
	Int_t startrun, endrun;
	std::stringstream startstring(argv[1]);
	startstring >> startrun;
	if (argc > 2) {
		std::stringstream endstring(argv[2]);
		endstring >> endrun;
	} else {
		endrun = startrun;
	}
	if (startrun > endrun) {
		std::cout << "startrun must be smaller than endrun ! Aborting!"
				<< std::endl;
		return -1;
	}
	std::vector<Double_t> averageRate;
	for (Int_t currentrun = startrun; currentrun <= endrun; currentrun++) {
		std::stringstream currentrunstring;
		currentrunstring << currentrun;
		KaLi::KLRunIdentifier run_identifier("myo" + currentrunstring.str());
		KaLi::KLRun *RUN = gDataManager->GetRun(run_identifier, true);
		const KaLi::KLHistogramEvent *histogramEvent = 0;
		for (UInt_t i = 0; i < RUN->GetNumberOfSubRuns(); i++) {
			UInt_t tempNEvents = 0;
			const KaLi::KLSubRunEvents *subRun = RUN->GetSubRun(i)->GetEvents();
			for (UInt_t j = 0; j < subRun->GetNEvents(histogramEvent); j++) {
				subRun->Get(j, histogramEvent);
				//UInt_t last = histogramEvent->GetHistogramLength() < 100 ? histogramEvent->GetHistogramLength() : 100;
				UInt_t last = histogramEvent->GetHistogramLength();
				for (UInt_t k = 50; k < last; k++) {
					tempNEvents += histogramEvent->GetHistData(k);
				}

			}

//		const KaLi::KLRunEvents *run =
//				new KaLi::KLRunEvents(
//						"/kalinka/storage/TBcube/rovedo/Root/myo00000871.orcaroot.root");
//
//		const KaLi::KLHistogramEvent *histogramEvent = 0;
//
//		for (UInt_t i = 0; i < run->GetNSubRuns(); i++) {
//			UInt_t tempNEvents = 0;
//			//for (UInt_t i = 0; i < RUN->GetNumberOfSubRuns(); i++){
//			KaLi::KLSubRunEvents *subRun = new KaLi::KLSubRunEvents(i, run);
//			for(UInt_t j = 0; j < subRun->GetNEvents(histogramEvent); j++){
//				subRun->Get(j, histogramEvent);
//				UInt_t last = histogramEvent->GetHistogramLength();
//				for(UInt_t k = 0; k < last; k++){
//					tempNEvents += histogramEvent->GetHistData(k);
//				}
//
//			}

			Float_t position = 0;
//sampling mode:
			position = i * 0.5 + 24.9 - 3;
//scan pmt positions only:
//			switch (i) {
//			case 0:
//				position = 8.4;
//				break;
//			case 1:
//				position = 24.9;
//				break;
//			case 2:
//				position = 40.1;
//				break;
//			case 3:
//				position = 56.5;
//				break;
//			}
			Double_t tempDuration =
					subRun->GetHeader()->GetRunTimes()->GetRunStop()
							- subRun->GetHeader()->GetRunTimes()->GetRunStart();
			file << position << " "
					<< static_cast<Double_t>(tempNEvents) / tempDuration << " "
					<< sqrt(tempNEvents) << " " << tempDuration << "\n";
			plot->SetPoint(i, position,
					static_cast<Double_t>(tempNEvents) / tempDuration);
			Double_t timeError = (Double_t) tempNEvents
					/ ((Double_t) tempDuration * tempDuration - 1 / 4);
			std::cout << "Duration subRun " << i << ": "
					<< subRun->GetHeader()->GetRunTimes()->GetRunStop()
							- subRun->GetHeader()->GetRunTimes()->GetRunStart()
					<< " with " << tempNEvents
					<< " Events and an time induced error of" << timeError
					<< std::endl;
//				RUN->GetSubRun(i)->GetHeader().GetStart().AsDouble();
//				UInt_t tempNEvents = RUN->GetSubRun(i)->GetEvents()->GetNEvents(histogramEvent);
//				Double_t tempDuration = (RUN->GetSubRun(i)->GetHeader().GetEnd().AsLongDouble() -
//				RUN->GetSubRun(i)->GetHeader().GetStart().AsLongDouble());
//				std::cout << "tempEvents: " << tempNEvents << ", tempDuration: " << tempDuration << std::endl;
			averageRate.push_back(
					static_cast<Double_t>(tempNEvents) / tempDuration);
			std::cout << "average Rate subRun " << i << ": "
					<< averageRate.at(i) << std::endl;
//				/*subRuns.push_back( RUN->GetSubRun(i)->GetEvents());
//				nEvents.push_back(subRuns.at(i)->GetNEvents(histogramEvent));*/
		}
//		plot->SetPoint(0, 10, averageRate.at(0));
//		plot->SetPoint(1, 25, averageRate.at(1));
//		plot->SetPoint(2, 40, averageRate.at(2));
//		plot->SetPoint(3, 55, averageRate.at(3));
		rates->cd(0);
		plot->Draw();
		plot->SetMarkerColor(2);
		plot->SetMarkerStyle(2);
		plot->SetLineStyle(1);
		plot->SetLineColor(4);
		plot->SetLineWidth(1);
		plot->SetTitle("Module xy");
		plot->GetXaxis()->SetTitle("position [cm]");
		plot->GetYaxis()->SetTitle("cps");
		plot->Draw("ACP");
		TGraph *PMTs = new TGraph;
		PMTs->SetPoint(0, 65/2 - 24.1, averageRate.at(0));
		PMTs->SetPoint(1, 65/2 - 7.6, averageRate.at(0));
		PMTs->SetPoint(2, 65/2 + 7.6, averageRate.at(0));
		PMTs->SetPoint(3, 65/2 + 24.1, averageRate.at(0));
		PMTs->SetMarkerStyle(26);
		PMTs->Draw("P");

		rates->Update();
		Rate->Run();
		delete RUN;
		//system(("rm -r /kalinka/home/rovedo/.cache/kali/" + run_identifier.GetName() + "/").c_str());
	}
}
