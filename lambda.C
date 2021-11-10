//
// Created by mikhail on 5/6/21.
//

#include "lambda.h"

void lambda(){
  gROOT->Macro( "/home/user/cbmdir/flow_drawing_tools/example/style.cc" );
//   auto leg1 = new TLegend( 0.65, 0.75, 0.9, 0.945 );
  bool is_first_canvas = true;
  
  SetFileName("/home/user/cbmdir/working/qna/cl.dcmqgsm.12agev.defcuts.3122.set1.sgnl_1.root");
  SetSelectAxis("centrality");
  SetSProjectionAxis("rapidity");
  SetSliceAxis("pT");
  
  TFile* fileIn = TFile::Open(fileName_.c_str(), "open");
  auto* dc = (Qn::DataContainer<Qn::StatCollect,Qn::Axis<double>>*)fileIn->Get<Qn::DataContainer<Qn::StatCollect,Qn::Axis<double>>>("sim/u_sim_PLAIN.Q_psi_PLAIN.x1x1");
  for(auto& ax : axes) {
    Qn::Axis<double> qnaxis = dc->GetAxis(ax.sim_name_);
    for(int i=0; i<=qnaxis.size(); i++) {
      ax.bin_edges_.push_back(qnaxis.GetLowerBinEdge(i));
    }
  }
  
  TFile* fileOut = TFile::Open("fileOut.root", "recreate");
    
  for(int iEdge=0; iEdge<axes.at(kSelect).bin_edges_.size()-1; iEdge++){

    auto v1_rec = DoubleDifferentialCorrelation( fileName_.c_str(),
                                                {"rec/RESCALED/u_rec_RESCALED.Q_psi_PLAIN.x1x1",
                                                "rec/RESCALED/u_rec_RESCALED.Q_psi_PLAIN.y1y1"} );
    v1_rec.SetSliceVariable(axes.at(kSlice).title_.c_str(), axes.at(kSlice).unit_.c_str());
    v1_rec.Scale(2);
    v1_rec.SetMarker(kFullSquare);
    v1_rec.SetPalette( {kOrange+1, kSpring-4, kGreen+2, kAzure-4, kRed, kViolet, kBlue } );
    v1_rec.Select({{axes.at(kSelect).reco_name_.c_str(),
                    {axes.at(kSelect).bin_edges_.at(iEdge), axes.at(kSelect).bin_edges_.at(iEdge+1)}}});
    v1_rec.SetProjectionAxis({axes.at(kProjection).reco_name_.c_str(), axes.at(kProjection).bin_edges_});
    v1_rec.SetSliceAxis({axes.at(kSlice).reco_name_.c_str(), axes.at(kSlice).bin_edges_});
    v1_rec.ShiftSliceAxis(axes.at(kSlice).shift_);
    v1_rec.Calculate();
    v1_rec.ShiftProjectionAxis(axes.at(kProjection).shift_);

    auto v1_sim = DoubleDifferentialCorrelation( fileName_.c_str(),
                                                {"sim/u_sim_PLAIN.Q_psi_PLAIN.x1x1",
                                                "sim/u_sim_PLAIN.Q_psi_PLAIN.y1y1"} );
    v1_sim.SetSliceVariable(axes.at(kSlice).title_.c_str(), axes.at(kSlice).unit_.c_str());
    v1_sim.Scale(2);
    v1_sim.SetMarker(-1);
    v1_sim.SetPalette( {kOrange+1, kSpring-4, kGreen+2, kAzure-4, kRed, kViolet, kBlue } );
    v1_sim.Select({{axes.at(kSelect).sim_name_.c_str(),
                    {axes.at(kSelect).bin_edges_.at(iEdge), axes.at(kSelect).bin_edges_.at(iEdge+1)}}});
    v1_sim.SetProjectionAxis({axes.at(kProjection).sim_name_.c_str(), axes.at(kProjection).bin_edges_});
    v1_sim.SetSliceAxis({axes.at(kSlice).sim_name_.c_str(), axes.at(kSlice).bin_edges_});
    v1_rec.ShiftSliceAxis(axes.at(kSlice).shift_);
    v1_sim.Calculate();
    v1_sim.ShiftProjectionAxis(axes.at(kProjection).shift_);

    HeapPicture pic( (axes.at(kSelect).name_ + "_" + std::to_string(iEdge)).c_str(), {1000, 1000});
    
    pic.AddText({0.2, 0.90, "Au+Au, DCM-QGSM-SMM, 12A GeV/c"}, 0.025);
    pic.AddText({0.2, 0.87, (to_string_with_precision(axes.at(kSelect).bin_edges_.at(iEdge) + axes.at(kSelect).shift_, axes.at(kSelect).precision_) +
                            " < " + axes.at(kSelect).title_ +
                            " < " + to_string_with_precision(axes.at(kSelect).bin_edges_.at(iEdge+1) + axes.at(kSelect).shift_, axes.at(kSelect).precision_) +
                            ", " + axes.at(kSelect).unit_).c_str()}, 0.025);

    auto leg1 = new TLegend( 0.3, 0.15 );
//     leg1->SetBorderSize(1);
    
    for( auto obj : v1_rec.GetProjections() ){
      pic.AddDrawable( obj );
      leg1->AddEntry( obj->GetPoints(), obj->GetTitle().c_str(), "P" );
    }
    for( auto obj : v1_sim.GetProjections() ){
      pic.AddDrawable( obj );
    }

    pic.SetAxisTitles({axes.at(kProjection).title_.c_str(), "v_{1} {#Lambda}"});
    pic.AddLegend(leg1);

    pic.CustomizeXRange();
    pic.CustomizeYRange();
    pic.Draw();
    
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
  
  
//   pic.Save("./v1_au123_urqmd_pi_neg_pT_y","png");
//   pic.Save("./v1_au123_urqmd_pi_neg_pT_y","root");
}


