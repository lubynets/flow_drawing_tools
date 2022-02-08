void resolution_3s()
{
  gROOT->Macro( "/home/user/cbmdir/flow_drawing_tools/example/style.cc" );
  
  std::string evegen = "dcmqgsm";
//   std::string evegen = "urqmd";
  
  std::string pbeam = "12";
//   std::string pbeam = "3.3";
  
  std::string fileNameMc = "/home/user/cbmdir/working/qna/resolutions/psd." + evegen + "." + pbeam + "agev.root";
  std::string fileNameReco = "/home/user/cbmdir/working/qna/resolutions/res." + evegen + "." + pbeam + "agev.root";
  
  MultiCorrelation multicor_mc;
  MultiCorrelation multicor_rec;
  
  multicor_mc.SetPalette( {kBlue, kBlue, kRed, kRed, kGreen+2, kGreen+2, kBlack, kBlack} );
  multicor_rec.SetPalette( {kBlue, kBlue, kRed, kRed, kGreen+2, kGreen+2, kBlack, kBlack} );
  
  multicor_mc.SetMarkers( {-1, -2, -1, -2, -1, -2, -1, -2} );
  multicor_rec.SetMarkers( {kFullSquare, kOpenSquare, kFullSquare, kOpenSquare, kFullSquare, kOpenSquare, kFullSquare, kOpenSquare} );
  
  std::vector<std::string> correls{"psd1", "psd2", "psd3"};
  std::vector<std::string> components{"x1x1", "y1y1"};
  
  for(auto& corr : correls)
    for(auto& comp : components)
    {
      multicor_mc.AddCorrelation(fileNameMc, {"Qpsi/" + corr + "_RECENTERED.Q_psi_PLAIN." + comp}, "mc_" + corr + "_" + comp);
      multicor_rec.AddCorrelation(fileNameReco, {"res_" + corr + "_via_sts_p." + comp}, "rec_" + corr + "_" + comp);
//       multicor_rec.AddCorrelation(fileNameReco, {"res_" + corr + "_via_sts_pipos." + comp}, "rec_" + corr + "_" + comp);
    }
        
  multicor_mc.Scale(2);
    
  HeapPicture pic("picture", {1000, 1000});
  
//   pic.AddText({0.2, 0.93, "3 sub-event"}, 0.025);
//   pic.AddText({0.2, 0.90, "Au+Au"}, 0.025);
// //   pic.AddText({0.2, 0.87, "DCM-QGSM-SMM"}, 0.025);
//   pic.AddText({0.2, 0.87, "UrQMD"}, 0.025);
//   pic.AddText({0.2, 0.84, "12A GeV/c"}, 0.025);
// //   pic.AddText({0.2, 0.84, "3.3A GeV/c"}, 0.025);
//   
//   pic.AddText({0.2, 0.78, "MC: R^{A}_{x} = 2#LTQ^{A}_{x}Q^{#Psi}_{x}#GT"}, 0.02);
//   pic.AddText({0.2, 0.72, "REC: R^{A}_{x} = #sqrt{#frac{2#LTQ^{A}_{x}Q^{B}_{x}#GT#LTQ^{A}_{x}Q^{C}_{x}#GT}{#LTQ^{B}_{x}Q^{C}_{x}#GT}}"}, 0.02);
  
  pic.AddText({0.2, 0.93, "4 sub-event"}, 0.025);
  pic.AddText({0.2, 0.90, "Au+Au"}, 0.025);
  pic.AddText({0.2, 0.87, "DCM-QGSM-SMM"}, 0.025);
//   pic.AddText({0.2, 0.87, "UrQMD"}, 0.025);
  pic.AddText({0.2, 0.84, "12A GeV/c"}, 0.025);
//   pic.AddText({0.2, 0.84, "3.3A GeV/c"}, 0.025);
  
  pic.AddText({0.2, 0.78, "MC: R^{A}_{x} = 2#LTQ^{A}_{x}Q^{#Psi}_{x}#GT"}, 0.02);
  
  auto leg1 = new TLegend();
  leg1->SetBorderSize(1);
  
  for(auto& obj : multicor_mc.GetCorrelations()) {
    obj->SetCalculateSystematicsFromVariation(false);
    pic.AddDrawable(obj);
  }
  
  for(auto& obj : multicor_rec.GetCorrelations()) {
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
  
  leg1->AddEntry(grx, "  REC", "P");
  leg1->AddEntry(grx, "  MC", "L");
  leg1->AddEntry(grx, "  X", "LP");
  leg1->AddEntry(gry, "  Y", "LP");
  leg1->AddEntry(multicor_rec.GetCorrelations().at(0)->GetPoints(), "  PSD1           ", "P");
  leg1->AddEntry(multicor_rec.GetCorrelations().at(2)->GetPoints(), "  PSD2           ", "P");
  leg1->AddEntry(multicor_rec.GetCorrelations().at(4)->GetPoints(), "  PSD3           ", "P");
  
  pic.SetAxisTitles( {"Centrality, %", "R_{1}"} );
  
  pic.CustomizeXRange();
  pic.CustomizeYRange();  
  pic.AddLegend(leg1);
  pic.CustomizeLegend(leg1);
  pic.Draw();
  pic.Save("fileOut", "pdf");
  pic.Save("fileOut", "root");
}