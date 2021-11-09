//
// Created by mikhail on 5/6/21.
//

#include "lambda.h"

void lambda(){
  gROOT->Macro( "/home/user/cbmdir/flow_drawing_tools/example/style.cc" );
  auto leg1 = new TLegend( 0.5, 0.75, 0.9, 0.945 );
  bool leg_formed = false;
  
  SetFileName("/home/user/cbmdir/working/qna/cl.dcmqgsm.12agev.defcuts.3122.set1.sgnl_1.root");
  SetSelectAxisBinEdges({0, 10, 20, 40, 70, 100});                                                  // read from qn file
  SetSliceAxisBinEdges({0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6});                                                  // read from qn file
  
  TFile* fileOut = TFile::Open("fileOut.root", "recreate");
  
  for(int iEdge=0; iEdge<select_axis_bin_edges_.size()-1; iEdge++){

    auto v1_rec = DoubleDifferentialCorrelation( fileName_.c_str(),
                                                {"rec/RESCALED/u_rec_RESCALED.Q_psi_PLAIN.x1x1",
                                                "rec/RESCALED/u_rec_RESCALED.Q_psi_PLAIN.y1y1"} );
    v1_rec.SetSliceVariable("p_{T}", "GeV/c");
    v1_rec.Scale(2);
    v1_rec.SetMarker(kFullSquare);
    v1_rec.SetPalette( {kOrange+1, kSpring-4, kGreen+2, kAzure-4, kRed, kViolet, kBlue } );
    v1_rec.Select({{"AnaEventHeader_centrality_tracks", {select_axis_bin_edges_.at(iEdge), select_axis_bin_edges_.at(iEdge+1)}}});
    v1_rec.SetProjectionAxis({"ReconstructedParticles_rapidity", 9, 0.92179005, 2.7217901});
    v1_rec.SetSliceAxis({"ReconstructedParticles_pT", slice_axis_bin_edges_});
    v1_rec.ShiftSliceAxis(0.);
    v1_rec.Calculate();
    v1_rec.ShiftProjectionAxis(-1.6217901);

    auto v1_sim = DoubleDifferentialCorrelation( fileName_.c_str(),
                                                {"sim/u_sim_PLAIN.Q_psi_PLAIN.x1x1",
                                                "sim/u_sim_PLAIN.Q_psi_PLAIN.y1y1"} );
    v1_sim.SetSliceVariable("p_{T}", "GeV/c");
    v1_sim.Scale(2);
    v1_sim.SetMarker(-1);
    v1_sim.SetPalette( {kOrange+1, kSpring-4, kGreen+2, kAzure-4, kRed, kViolet, kBlue } );
    v1_sim.Select({{"AnaEventHeader_centrality_tracks", {0, 10}}});
    v1_sim.SetProjectionAxis({"SimParticles_rapidity", 9, 0.92179005, 2.7217901});
    v1_sim.SetSliceAxis({"SimParticles_pT", slice_axis_bin_edges_});
    v1_rec.ShiftSliceAxis(0.);
    v1_sim.Calculate();
    v1_sim.ShiftProjectionAxis(-1.6217901);

    HeapPicture pic( ("v1_" + std::to_string(iEdge)).c_str(), {1000, 1000});

  //   pic.AddText({0.2, 0.9, "HADES Au+Au@1.23A GeV"}, 0.025);
  //   pic.AddText({0.2, 0.87, "#pi^{+}; TOF+RPC 20-30%"}, 0.025);
  //   pic.AddText({0.2, 0.84, "w. eff; Qn:RTR; Err:bstrap"}, 0.025);
  //   pic.AddText({0.2, 0.81, "SP:W1,3[Mb;W3,1]"}, 0.025);
  //   pic.AddText({0.2, 0.78, "w/o occ. corr."}, 0.025);

    for( auto obj : v1_rec.GetProjections() ){
      pic.AddDrawable( obj );
      if(!leg_formed)
        leg1->AddEntry( obj->GetPoints(), obj->GetTitle().c_str(), "P" );
    }
    leg_formed = true;
    for( auto obj : v1_sim.GetProjections() ){
      pic.AddDrawable( obj );
    }

    pic.SetAxisTitles({"y_{CM}", "v_{1}"});
    pic.AddLegend(leg1);

  //   pic.SetXRange({0.72179005, 2.9217901});
    pic.SetXRange({-0.8, 1.2});
    pic.CustomizeYRange();
    pic.Draw();
    
    fileOut->cd();
    pic.GetCanvas()->Write();
  }
  
  fileOut->Close();
  
  
//   pic.Save("./v1_au123_urqmd_pi_neg_pT_y","png");
//   pic.Save("./v1_au123_urqmd_pi_neg_pT_y","root");
}


