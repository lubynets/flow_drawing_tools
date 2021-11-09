//
// Created by mikhail on 5/6/21.
//

void v1_au123_urqmd_pi_neg_pT_y(){
  gROOT->Macro( "/home/user/cbmdir/flow_drawing_tools/example/style.cc" );
  auto leg1 = new TLegend( 0.5, 0.75, 0.9, 0.945 );

  auto v1_bw = DoubleDifferentialCorrelation( "/home/user/cbmdir/working/qna/cl.dcmqgsm.12agev.defcuts.3122.set1.sgnl_1.root",
                                              {"rec/RESCALED/u_rec_RESCALED.Q_psi_PLAIN.x1x1",
                                               "rec/RESCALED/u_rec_RESCALED.Q_psi_PLAIN.y1y1"} );
  v1_bw.SetSliceVariable("p_{T}", "GeV/c");
  v1_bw.Scale(2);
  v1_bw.SetMarker(kFullSquare);
  v1_bw.SetPalette( {kOrange+1, kSpring-4, kGreen+2, kAzure-4, kRed, kViolet, kBlue } );
  v1_bw.Select({{"AnaEventHeader_centrality_tracks", {0, 10, 20, 40, 70, 100}}});
  v1_bw.SetProjectionAxis({"ReconstructedParticles_rapidity", 9, 0.92179005, 2.7217901});
  v1_bw.SetSliceAxis({"ReconstructedParticles_pT", 7, 0.2, 1.6});
  v1_bw.Calculate();

  auto v1_bw_true = DoubleDifferentialCorrelation( "/home/user/cbmdir/working/qna/cl.dcmqgsm.12agev.defcuts.3122.set1.sgnl_1.root",
                                              {"sim/u_sim_PLAIN.Q_psi_PLAIN.x1x1",
                                               "sim/u_sim_PLAIN.Q_psi_PLAIN.y1y1"} );
  v1_bw_true.SetSliceVariable("p_{T}", "GeV/c");
  v1_bw_true.Scale(2);
  v1_bw_true.SetMarker(-1);
  v1_bw_true.SetPalette( {kOrange+1, kSpring-4, kGreen+2, kAzure-4, kRed, kViolet, kBlue } );
  v1_bw_true.Select({{"AnaEventHeader_centrality_tracks", {0, 10, 20, 40, 70, 100}}});
  v1_bw_true.SetProjectionAxis({"SimParticles_rapidity", 9, 0.92179005, 2.7217901});
  v1_bw_true.SetSliceAxis({"SimParticles_pT", 7, 0.2, 1.6});
  v1_bw_true.Calculate();

  HeapPicture pic( "v1_au123_corrected_y_slices_pT", {1000, 1000});

  pic.AddText({0.2, 0.9, "HADES Au+Au@1.23A GeV"}, 0.025);
  pic.AddText({0.2, 0.87, "#pi^{+}; TOF+RPC 20-30%"}, 0.025);
  pic.AddText({0.2, 0.84, "w. eff; Qn:RTR; Err:bstrap"}, 0.025);
  pic.AddText({0.2, 0.81, "SP:W1,3[Mb;W3,1]"}, 0.025);
  pic.AddText({0.2, 0.78, "w/o occ. corr."}, 0.025);

  for( auto obj : v1_bw.GetProjections() ){
    pic.AddDrawable( obj );
    leg1->AddEntry( obj->GetPoints(), obj->GetTitle().c_str(), "P" );
  }
  for( auto obj : v1_bw_true.GetProjections() ){
    pic.AddDrawable( obj );
  }

  pic.SetAxisTitles({"y", "v_{1}"});
  pic.AddLegend(leg1);

  pic.SetXRange({0.72179005, 2.9217901});
  pic.SetYRange({-0.249, 0.449});
  pic.Draw();
  pic.Save("./v1_au123_urqmd_pi_neg_pT_y","png");
  pic.Save("./v1_au123_urqmd_pi_neg_pT_y","root");
}
