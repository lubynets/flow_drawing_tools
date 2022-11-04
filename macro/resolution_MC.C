void resolution_MC() {
  gROOT->Macro( "/home/oleksii/cbmdir/flow_drawing_tools/example/style.cc" );
    
  std::string fileName = "/home/oleksii/cbmdir/working/qna/simtracksflow/v1andR1.stf.root";
    
  std::vector<std::string> correls{"psd1_RECENTERED", "psd2_RECENTERED", "psd3_RECENTERED", "Q_spec_PLAIN"};
  std::vector<std::string> components{"x1x1", "y1y1"};
    
  MultiCorrelation multicor_mc;
  multicor_mc.SetPalette( {kBlue, kBlue, kRed, kRed, kGreen+2, kGreen+2, kBlack, kBlack} );
  multicor_mc.SetMarkers( {-1, -2, -1, -2, -1, -2, -1, -2} );
  
  for(auto& corr : correls) {
    for(auto& comp : components) {
      multicor_mc.AddCorrelation(fileName, {"R1/res." + corr + "." + comp}, "mc_" + corr + "_" + comp);
    }
  }
            
  HeapPicture pic("picture", {1000, 1000});
  pic.AddText({0.2, 0.89, "Au+Au"}, 0.025);
  pic.AddText({0.2, 0.86, "DCM-QGSM-SMM"}, 0.025);
  pic.AddText({0.2, 0.83, "12A GeV/c"}, 0.025);
  pic.AddText({0.2, 0.77, "MC: R^{A}_{x} = 2#LTQ^{A}_{x}Q^{#Psi}_{x}#GT"}, 0.02);
  
  auto leg1 = new TLegend();
  leg1->SetBorderSize(1);
  
  for(auto& obj : multicor_mc.GetCorrelations()) {
    obj->SetCalculateSystematicsFromVariation(false);
    pic.AddDrawable(obj);
  }

  TGraph* grx = new TGraph();
  grx->SetMarkerStyle(kFullSquare);
  grx->SetLineStyle(1);
  grx->SetMarkerColor(kBlack);
  grx->SetLineColor(kBlack);
  TGraph* gry = new TGraph();
  gry->SetMarkerStyle(kOpenSquare);
  gry->SetLineStyle(2);
  gry->SetMarkerColor(kBlack);
  gry->SetLineColor(kBlack);
  
  leg1->AddEntry(grx, "  X", "L");
  leg1->AddEntry(gry, "  Y", "L");
  leg1->AddEntry(multicor_mc.GetCorrelations().at(0)->GetPoints(), "  PSD1           ", "L");
  leg1->AddEntry(multicor_mc.GetCorrelations().at(2)->GetPoints(), "  PSD2           ", "L");
  leg1->AddEntry(multicor_mc.GetCorrelations().at(4)->GetPoints(), "  PSD3           ", "L");
  leg1->AddEntry(multicor_mc.GetCorrelations().at(6)->GetPoints(), "  spec           ", "L");
  
  pic.SetAxisTitles( {"b, fm", "R_{1}"} );
  pic.CustomizeXRange();
  pic.CustomizeYRange();  
  pic.AddLegend(leg1);
  pic.CustomizeLegend(leg1);
//   pic.SetGridX();
//   pic.SetGridY();
  pic.Draw();
//   pic.Save("fileOut", "png");
  
  TFile* fileOut = TFile::Open("fileOut.root", "recreate");
  fileOut->cd();
  pic.GetCanvas()->Write();
  fileOut->Close();  
    
  pic.GetCanvas()->Print("fileOut.pdf", "pdf");
}
