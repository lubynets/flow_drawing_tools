#include "lambda.h"

void lambda_R_MC() {
  gROOT->Macro( "/home/user/cbmdir/flow_drawing_tools/example/style.cc" );
  bool is_first_canvas = true;
  
//   bool is_draw_difference = false;
  bool is_draw_difference = true;
  
//   std::string fileName = "/home/user/cbmdir/working/qna/correlations/aXmass/v1andR1.dcmqgsm.apr20.recpid.lightcuts1.3122.set4.sgnl.root";
//   std::string fileName = "/home/user/cbmdir/working/qna/correlations/aXmass/v1andR1.dcmqgsm.apr20.recpid.lightcuts1.310.set4.root";
  std::string fileName = "/home/user/cbmdir/working/qna/correlations/aXmass/v1andR1.dcmqgsm.apr20.recpid.defaultcuts.3312.set4.root";
  
//   std::string particle = "#Lambda";
//   std::string particle = "K^{0}_{S}";
  std::string particle = "#Xi^{-}";
  
  SetAxis("centrality", "select");
  SetAxis("rapidity", "projection");
  SetAxis("pT", "slice");
  std::string component_1 = "x1x1";
  std::string component_2 = "y1y1";
  std::string harmonic = "1";
  
//   std::string subevent = "psd1";
//   std::string subevent = "psd2";
  std::string subevent = "psd3";
  
//   SetAxis("rapidity", "select");
//   SetAxis("pT", "projection");
//   SetAxis("centrality", "slice");
//   std::string component_1 = "x2x2";
//   std::string component_2 = "y2y2";
//   std::string harmonic = "2";

  TFile* fileIn = TFile::Open(fileName.c_str(), "open");
  auto* dc = (Qn::DataContainer<Qn::StatCalculate,Qn::Axis<double>>*)fileIn->Get<Qn::DataContainer<Qn::StatCalculate,Qn::Axis<double>>>("v1/usimPsi/v1.u_sim_PLAIN.Q_psi_PLAIN.x1x1");
  
  assert(dc!=nullptr);
  for(auto& ax : axes) {
    Qn::Axis<double> qnaxis = dc->GetAxis(ax.sim_name_);
    for(int i=0; i<=qnaxis.size(); i++) {
      ax.bin_edges_.push_back(qnaxis.GetLowerBinEdge(i));
    }
  }
  
//   SetSliceAxisBinEdges({0, 0.6, 1.2, 1.5}); // K0S
  SetSliceAxisBinEdges({0.2, 1, 1.4}); // Xi-
  
  TFile* fileOut = TFile::Open("fileOut.root", "recreate");
  
  for(int iEdge=0; iEdge<axes.at(kSelect).bin_edges_.size()-1; iEdge++){
  
    std::string corrName_sim = "v1/usimPsi/v1.u_sim_PLAIN.Q_psi_PLAIN.";
    auto v1_sim = DoubleDifferentialCorrelation( fileName.c_str(),
                                                {(corrName_sim + component_1).c_str(),
                                                 (corrName_sim + component_2).c_str() } );
    
    v1_sim.SetSliceVariable(axes.at(kSlice).title_.c_str(), axes.at(kSlice).unit_.c_str());
    v1_sim.SetMarker(-1);
    v1_sim.SetPalette({kOrange+1, kSpring, kGreen+2, kAzure-4, kRed, kViolet, kBlue});
    v1_sim.Rebin({{axes.at(kSelect).sim_name_.c_str(),
                    {axes.at(kSelect).bin_edges_.at(iEdge), axes.at(kSelect).bin_edges_.at(iEdge+1)}}});
    v1_sim.SetProjectionAxis({axes.at(kProjection).sim_name_.c_str(), axes.at(kProjection).bin_edges_});
    v1_sim.SetSliceAxis({axes.at(kSlice).sim_name_.c_str(), axes.at(kSlice).bin_edges_});
    v1_sim.ShiftSliceAxis(axes.at(kSlice).shift_);
    v1_sim.Calculate();
    v1_sim.ShiftProjectionAxis(axes.at(kProjection).shift_);  
  

//     std::string corrName_PsiRP = "v1/uPsi/v1.u_rec_RESCALED.Q_psi_PLAIN.";
    std::string corrName_PsiRP = "v1/uPsi/v1.u_rec_sgnl_RESCALED.Q_psi_PLAIN.";
    auto v1_PsiRP = DoubleDifferentialCorrelation( fileName.c_str(),
                                                {(corrName_PsiRP + component_1).c_str(),
                                                 (corrName_PsiRP + component_2).c_str() } );
    
    v1_PsiRP.SetSliceVariable(axes.at(kSlice).title_.c_str(), axes.at(kSlice).unit_.c_str());
    v1_PsiRP.SetMarker(kOpenSquare);
    v1_PsiRP.SetPalette({kOrange+1, kSpring, kGreen+2, kAzure-4, kRed, kViolet, kBlue});
    v1_PsiRP.Rebin({{axes.at(kSelect).reco_name_.c_str(),
                    {axes.at(kSelect).bin_edges_.at(iEdge), axes.at(kSelect).bin_edges_.at(iEdge+1)}}});
    v1_PsiRP.SetProjectionAxis({axes.at(kProjection).reco_name_.c_str(), axes.at(kProjection).bin_edges_});
    v1_PsiRP.SetSliceAxis({axes.at(kSlice).reco_name_.c_str(), axes.at(kSlice).bin_edges_});
    v1_PsiRP.ShiftSliceAxis(axes.at(kSlice).shift_);
    v1_PsiRP.Calculate();
    v1_PsiRP.ShiftProjectionAxis(axes.at(kProjection).shift_);
    
    
//     std::string corrName_R_MC = "v1/uQ_R1_MC/v1.u_rec_RESCALED." + subevent + "_RECENTERED.res_MC.";
    std::string corrName_R_MC = "v1/uQ_R1_MC/v1.u_rec_sgnl_RESCALED." + subevent + "_RECENTERED.res_MC.";
    auto v1_R_MC = DoubleDifferentialCorrelation( fileName.c_str(),
                                                {(corrName_R_MC + component_1).c_str(),
                                                 (corrName_R_MC + component_2).c_str() } );
    
    v1_R_MC.SetSliceVariable(axes.at(kSlice).title_.c_str(), axes.at(kSlice).unit_.c_str());
    v1_R_MC.SetMarker(kFullSquare);
    v1_R_MC.SetPalette({kOrange+1, kSpring, kGreen+2, kAzure-4, kRed, kViolet, kBlue});
    v1_R_MC.Rebin({{axes.at(kSelect).reco_name_.c_str(),
                    {axes.at(kSelect).bin_edges_.at(iEdge), axes.at(kSelect).bin_edges_.at(iEdge+1)}}});
    v1_R_MC.SetProjectionAxis({axes.at(kProjection).reco_name_.c_str(), axes.at(kProjection).bin_edges_});
    v1_R_MC.SetSliceAxis({axes.at(kSlice).reco_name_.c_str(), axes.at(kSlice).bin_edges_});
    v1_R_MC.ShiftSliceAxis(axes.at(kSlice).shift_);
    v1_R_MC.Calculate();
    v1_R_MC.ShiftProjectionAxis(axes.at(kProjection).shift_);
    
    
    HeapPicture pic( (axes.at(kSelect).name_ + "_" + std::to_string(iEdge)).c_str(), {1000, 1000});
    
    pic.AddText({0.2, 0.90, particle.c_str()}, 0.035);
    pic.AddText({0.2, 0.87, "5M Au+Au"}, 0.025);
    pic.AddText({0.2, 0.84, "DCM-QGSM-SMM"}, 0.025);
    pic.AddText({0.2, 0.81, "12A GeV/c"}, 0.025);
//     pic.AddText({0.2, 0.81, "3.3A GeV/c"}, 0.025);

    pic.AddText({0.2, 0.78, (axes.at(kSelect).title_ + ": " + to_string_with_precision(axes.at(kSelect).bin_edges_.at(iEdge) + axes.at(kSelect).shift_, axes.at(kSelect).precision_) + 
                            " - " + to_string_with_precision(axes.at(kSelect).bin_edges_.at(iEdge+1) + axes.at(kSelect).shift_, axes.at(kSelect).precision_) + axes.at(kSelect).unit_).c_str()}, 0.025);
    
    auto leg1 = new TLegend();
    leg1->SetBorderSize(1);
    
    TLegendEntry* entry;
    
    if(!is_draw_difference) {
      entry = leg1->AddEntry("", "MC", "L");
      entry->SetLineColor(kBlack);
      entry->SetLineWidth(2);
    
      entry = leg1->AddEntry("", "#Psi^{RP}", "P");
      entry->SetMarkerSize(2);
      entry->SetMarkerStyle(kOpenSquare);
      
      entry = leg1->AddEntry("", ("#Psi^{" + subevent + "}(R_{1}^{true})").c_str(), "P");
      entry->SetMarkerSize(2);
      entry->SetMarkerStyle(kFullSquare);
    }
    
    leg1->SetHeader((axes.at(kSlice).title_+axes.at(kSlice).unit_).c_str());
    
    if(!is_draw_difference) {
      for( auto obj : v1_sim.GetProjections() ){
        pic.AddDrawable( obj );
      }    
      for( auto obj : v1_PsiRP.GetProjections() ){
        pic.AddDrawable( obj );
      }
      for( auto obj : v1_R_MC.GetProjections() ){
        pic.AddDrawable( obj );
        leg1->AddEntry( obj->GetPoints(), obj->GetTitle().c_str(), "P" );
      }
      pic.SetAxisTitles({(axes.at(kProjection).title_ + axes.at(kProjection).unit_).c_str(), ("v_{" + harmonic + "}").c_str()});
    }
//     else {    
//       pic.DrawZeroLine(false);
//       int i{0};
//       for( auto obj_sim : v1_sim.GetProjections() ) {
//         auto obj_psiRP = v1_PsiRP.GetProjections().at(i);
//         GraphSubtractor gr_sub;
//         gr_sub.SetMinuend(obj_psiRP);
//         gr_sub.SetSubtrahend(obj_sim);
//         gr_sub.IgnoreSubtrahendErrors();
//         gr_sub.Calculate();
//         auto obj_psiRP_sim = gr_sub.GetResult();
//         pic.AddDrawable(obj_psiRP_sim);
//         
//         auto obj_R_MC = v1_R_MC.GetProjections().at(i);
//         gr_sub.SetMinuend(obj_R_MC);
//         gr_sub.SetSubtrahend(obj_sim);
//         gr_sub.IgnoreSubtrahendErrors();
//         gr_sub.Calculate();
//         auto obj_R_MC_sim = gr_sub.GetResult();
//         pic.AddDrawable(obj_R_MC_sim);
//         
//         leg1->AddEntry( obj_R_MC->GetPoints(), obj_R_MC->GetTitle().c_str(), "P" );
//         
//         i++;
//       }
//       pic.SetAxisTitles({(axes.at(kProjection).title_ + axes.at(kProjection).unit_).c_str(),
//                         ("(v_{" + harmonic + "}^{rec} - v_{" + harmonic + "}^{mc})/#sigma_{v_{" + harmonic + "}^{rec}}").c_str()});
//     }
    else {    
      pic.DrawZeroLine(false);
      int i{0};
      for( auto obj_R_MC : v1_R_MC.GetProjections() ) {
        auto obj_psiRP = v1_PsiRP.GetProjections().at(i);
        GraphSubtractor gr_sub;
        gr_sub.SetMinuend(obj_R_MC);
        gr_sub.SetSubtrahend(obj_psiRP);
        gr_sub.IgnoreSubtrahendErrors();
        gr_sub.Calculate();
        auto obj_R_MC_psiRP = gr_sub.GetResult();
        pic.AddDrawable(obj_R_MC_psiRP);
        
        leg1->AddEntry( obj_R_MC->GetPoints(), obj_R_MC->GetTitle().c_str(), "P" );
        
        i++;
      }
      pic.SetAxisTitles({(axes.at(kProjection).title_ + axes.at(kProjection).unit_).c_str(),
                        ("(v_{" + harmonic + "}{#Psi^{" + subevent + "}(R_{1}^{true})} - v_{" + harmonic + "}{#Psi^{RP}})/#sigma_{v_{" + harmonic + "}{#Psi^{" + subevent + "}(R_{1}^{true})}}").c_str()});
    }
    
//     pic.SetXRange({-0.05, 0.95});
    pic.CustomizeXRange();
    pic.CustomizeYRange();
    pic.AddLegend(leg1);
    pic.CustomizeLegend(leg1);
    pic.Draw();
    
    TF1 oneline("oneline", "1", -100, 100);
    TF1 minusoneline("minusoneline", "-1", -100, 100);
    
    if(is_draw_difference) {
      oneline.Draw("same");
      minusoneline.Draw("same");
    }
    
    fileOut->cd();
    pic.GetCanvas()->Write();
    
    if(is_first_canvas)
      pic.GetCanvas()->Print("fileOut.pdf(", "pdf");
    else
      pic.GetCanvas()->Print("fileOut.pdf", "pdf");
    is_first_canvas = false;
    
  }
  
  TCanvas emptycanvas("", "", 1000, 1000);
  emptycanvas.Print("fileOut.pdf)", "pdf");
  
  fileOut->Close();  
  
}