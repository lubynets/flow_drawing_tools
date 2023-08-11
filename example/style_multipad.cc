//
// Created by mikhail on 3/5/21.
//
{
gStyle->SetCanvasPreferGL(true);
gStyle->SetPadLeftMargin(0.15);
gStyle->SetPadRightMargin(0.05);
gStyle->SetPadBottomMargin(0.1);
gStyle->SetPadTopMargin(0.05);
gStyle->SetLegendBorderSize(0);
gStyle->SetFrameLineWidth(2);
gStyle->SetMarkerSize(2);
gStyle->SetLineWidth(2);

gStyle->SetTextFont(23);
gStyle->SetTextSize(20);

gStyle->SetTitleFont(63, "XY");
gStyle->SetLabelFont(63, "XY");

gStyle->SetTitleSize(20, "XY");
gStyle->SetTitleSize(0.01, "Z");

gStyle->SetLabelSize(20, "XY");
gStyle->SetLabelSize(0.04, "Z");

gStyle->SetLabelOffset(0.003, "X");
gStyle->SetLabelOffset(0.01, "Y");
gStyle->SetLabelOffset(0.05, "Z");

gStyle->SetTitleOffset(2.0, "X");
gStyle->SetTitleOffset(3.5, "Y");
gStyle->SetTitleOffset(0.05, "Z");

gStyle->SetNdivisions(508, "Y");

gStyle->SetOptStat(0);
}
