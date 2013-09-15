#include<fstream>
#include<istream>
#include<iostream>
#include<vector>
#include<iterator>
#include<algorithm>

#include"TGraph.h"
#include"TCanvas.h"
#include"TROOT.h"
#include"TStyle.h"
#include"TApplication.h"
#include"TAxis.h"

using namespace std;
void ApplyCustomStyle()
{//ROOT-Style wird geändert, macht die Plots schöner und entfernt grauen Hintergrund
    gROOT->SetStyle("Plain");
    gStyle->SetFitFormat("5.4f");
    gStyle->SetPalette(1);
    gStyle->SetCanvasColor(0);
    gStyle->SetPadColor(0);
    gStyle->SetErrorX(0);
    gStyle->SetLineWidth(2);
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    gStyle->SetOptFit(0);
    gStyle->SetTitleAlign(22);
    gStyle->SetTitleX(0.5);
    gStyle->SetBarWidth(1000);
    gStyle->SetBarOffset(0);
    gStyle->SetDrawBorder(0);
    gStyle->SetOptLogx(0);
    gStyle->SetOptLogy(0);
    gStyle->SetOptLogz(0);
    gStyle->SetOptDate(0);
    gStyle->SetNumberContours(20);

    gStyle->SetAxisColor(1, "x");
    gStyle->SetAxisColor(1, "y");
    gStyle->SetAxisColor(1, "z");
    gStyle->SetLabelColor(1, "x");
    gStyle->SetLabelColor(1, "y");
    gStyle->SetLabelColor(1, "z");
    gStyle->SetLabelFont(42, "x");
    gStyle->SetLabelFont(42, "y");
    gStyle->SetLabelFont(42, "z");
    gStyle->SetLabelOffset(0.005, "x");
    gStyle->SetLabelOffset(0.005, "y");
    gStyle->SetLabelOffset(0.005, "z");
    gStyle->SetLabelSize(0.035, "x");
    gStyle->SetLabelSize(0.035, "y");
    gStyle->SetLabelSize(0.035, "z");
    gStyle->SetTitleOffset(1.0, "x");
    gStyle->SetTitleOffset(1.0, "y");
    gStyle->SetTitleOffset(1.0, "z");
    gStyle->SetTitleSize(0.04, "x");
    gStyle->SetTitleSize(0.04, "y");
    gStyle->SetTitleSize(0.04, "z");
    gStyle->SetTitleColor(1, "x");
    gStyle->SetTitleColor(1, "y");
    gStyle->SetTitleColor(1, "z");
    gStyle->SetTitleFont(42, "x");
    gStyle->SetTitleFont(42, "y");
    gStyle->SetTitleFont(42, "z");
}

int main(int argc, char** argv)
{
    ApplyCustomStyle();
    TApplication * App = new TApplication("App", &argc, argv );

    ifstream input("./dat/sig1.dat");
    istream_iterator<double> start(input), end;
    vector<double> numbers(start, end);
    
    const int N = numbers.size() / 2;
    double U[N],t[N];
    for(int i=0; i < N; i++){
        t[i] = numbers.at(i*2);
        U[i] = numbers.at(i*2+1);
        //cout << t[i] << '\t' << U[i] << endl;
    }
    input.close();

    TGraph* timeGr = new TGraph(N, t, U);
    
    TCanvas* canvas = new TCanvas("canvas","Geometry Canvas",5,5,800,600);
    canvas->SetBorderMode(0);
    canvas->SetFillColor(kWhite);
    
    timeGr->SetLineColor(kBlue);
    timeGr->SetMarkerColor(kBlue);
    
    timeGr->SetTitle("Veto Signal");
    
    timeGr->Draw("AL");
    
    timeGr->GetXaxis()->SetTitle("Time [s]");
    timeGr->GetYaxis()->SetTitle("Voltage [V]");

    App->Run();
}
