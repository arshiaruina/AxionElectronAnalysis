#ifndef ROOTCONFIG_H
#define ROOTCONFIG_H 1

//root
#include "TROOT.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TColor.h"
#include "TAxis.h"
#include "TGaxis.h"
//definitions
#define ROOTCONFIG_CANVAS_WIDTH 1600.
#define ROOTCONFIG_CANVAS_HEIGHT 1000.
#define ROOTCONFIG_SQUARECANVAS_WIDTH 720.
#define ROOTCONFIG_SQUARECANVAS_HEIGHT 600.
#define ROOTCONFIG_CANVAS_MARGIN_LEFT 0.15
#define ROOTCONFIG_CANVAS_MARGIN_RIGHT 0.05
#define ROOTCONFIG_CANVAS_MARGIN_BOTTOM 0.15
#define ROOTCONFIG_CANVAS_MARGIN_TOP 0.1
#define ROOTCONFIG_SQUARECANVAS_MARGIN_LEFT 0.15
#define ROOTCONFIG_SQUARECANVAS_MARGIN_RIGHT 0.225
#define ROOTCONFIG_SQUARECANVAS_MARGIN_BOTTOM 0.15
#define ROOTCONFIG_SQUARECANVAS_MARGIN_TOP 0.1
#define ROOTCONFIG_FONT_TIMESNEWROMAN 132
#define ROOTCONFIG_FONT_ARIAL 42
namespace rootConfig
{
inline void applySettings()
{
	gStyle->SetOptStat(0);
	gStyle->SetTitleBorderSize(0);

	gStyle->SetLabelSize(0.05,"xyz");
	gStyle->SetTitleSize(0.05,"xyz");

	gStyle->SetTextFont(ROOTCONFIG_FONT_ARIAL);
	gStyle->SetLabelFont(ROOTCONFIG_FONT_ARIAL,"xyz");
	gStyle->SetTitleFont(ROOTCONFIG_FONT_ARIAL,"xyz");
	gStyle->SetStatFont(ROOTCONFIG_FONT_ARIAL);
	gStyle->SetLegendFont(ROOTCONFIG_FONT_ARIAL);

	gStyle->SetTitleOffset(1.15,"x");
	gStyle->SetTitleOffset(1.25,"y");
	gStyle->SetTitleOffset(1.7,"z");

	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);

	gStyle->SetOptFit(11);
	gStyle->SetStatX(.9);
	gStyle->SetStatY(.85);
	gStyle->SetStatW(.15);

	gStyle->SetTitleX(0.5);
	gStyle->SetTitleAlign(23);

	gStyle->SetEndErrorSize(10);

	const Int_t NRGBs = 2;
	const Int_t NCont = 255;

	Double_t stops[NRGBs] = { 0.00, 1.00};
	Double_t red[NRGBs]   = { 0.00, 1.00};
	Double_t green[NRGBs] = { 0.00, 0.00};
	Double_t blue[NRGBs]  = { 1.00, 0.00};
	TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
	gStyle->SetNumberContours(NCont);

	TGaxis::SetExponentOffset(0.025,0.01,"z");
	TGaxis::SetExponentOffset(-0.01, 0.01, "y");
	TGaxis::SetExponentOffset(-0.05, 0.01, "x");

	gROOT->ForceStyle();
}

}

#endif
