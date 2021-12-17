void resolution()
{
  gROOT->Macro( "/home/user/cbmdir/flow_drawing_tools/example/style.cc" );
  
  std::string fileNameMc = "/home/user/cbmdir/working/qna/resolutions/psd.dcmqgsm.12agev.root";
  std::string fileNameReco = "/home/user/cbmdir/working/qna/resolutions/res.dcmqgsm.12agev.root";
  
  MultiCorrelation multicor_mc;
  MultiCorrelation multicor_rec;
  
  multicor_mc.SetPalette(kBlue, kRed, kGreen+2);
  multicor_rec.SetPalette(kBlue, kRed, kGreen+2);
  
  std::vector<std::string> correls{"psd1", "psd2", "psd3"};
  std::vector<std::string> components{"x1x1", "y1y1"};
  
  for(auto& corr : correls)
    for(auto& comp : components)
    {
      multicor_mc.AddCorrelation(fileNameMc, {"Qpsi/RESCALED/" + corr + "_RESCALED.Q_psi_PLAIN." + comp}, "mc_" + corr + "_" + comp);
      multicor_rec.AddCorrelation(fileNameReco, {corr + "_RESCALED." + comp}, "rec_" + corr + "_" + comp);
    }
    
  HeapPicture pic("picture", {1000, 1000});
  
  for(auto& obj : multicor_mc.GetCorrelations())
    pic.AddDrawable(obj);
  
  for(auto& obj : multicor_rec.GetCorrelations())
    pic.AddDrawable(obj);
  
  pic.CustomizeXRange();
  pic.CustomizeYRange();  
  
  pic.Draw();
  pic.Save("fileOut", "png");
}