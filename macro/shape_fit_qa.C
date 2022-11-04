#include "shape_fit_qa.h"

void shape_fit_qa() {
  gROOT->Macro( "/home/oleksii/cbmdir/flow_drawing_tools/example/style.cc" );

  std::string fileName = "/home/oleksii/cbmdir/working/qna/shapes/shape_fit.dcmqgsm.12agev.recpid.3122.root";

  std::vector<std::pair<std::string, std::string>> values {
    {"Params/sgnl", "Height"},
    {"Params/sgnl", "mu_shift"},
    {"Params/sgnl", "sigma"},
    {"Params/sgnl", "a1"},
    {"Params/sgnl", "n1"},
    {"Params/sgnl", "a2"},
    {"Params/sgnl", "n2"},
    {"Params/bckgr", "p0"},
    {"Params/bckgr", "p1"},
    {"Params/bckgr", "p2"},
    {"Params/bckgr", "p3"},
    {"Chi2s", "chi2_prefit"},
    {"Chi2s", "chi2_fit"}
  };

  SetAxis("centrality", "select");
  SetAxis("y", "projection");
  SetAxis("pT", "slice");

  TFile* fileIn = TFile::Open(fileName.c_str(), "open");
  auto* dc = (Qn::DataContainer<Qn::StatDiscriminator,Qn::Axis<double>>*)fileIn->Get<Qn::DataContainer<Qn::StatDiscriminator,Qn::Axis<double>>>("Params/sgnl/Height");
  assert(dc!=nullptr);
  for(auto& ax : axes) {
    Qn::Axis<double> qnaxis = dc->GetAxis(ax.name_);
    for(int i=0; i<=qnaxis.size(); i++) {
      ax.bin_edges_.push_back(qnaxis.GetLowerBinEdge(i));
    }
  }
  fileIn->Close();

  for(auto& va : values) {
    bool is_first_canvas = true;

    std::string dcName = va.first + "/" + va.second;

    for(int iEdge=0; iEdge<axes.at(kSelect).bin_edges_.size()-1; iEdge++){
      auto param = DoubleDifferentialCorrelation( fileName.c_str(), {dcName.c_str()});
      param.SetSliceVariable(axes.at(kSlice).title_.c_str(), axes.at(kSlice).unit_.c_str());
      param.SetMarker(kFullSquare);
      param.SetPalette({kOrange+1, kBlue, kGreen+2, kAzure-4, kSpring, kViolet, kRed});
      param.SetBiasPalette(false);
      param.Rebin({{axes.at(kSelect).name_.c_str(),
                      {axes.at(kSelect).bin_edges_.at(iEdge), axes.at(kSelect).bin_edges_.at(iEdge+1)}}});
      param.SetProjectionAxis({axes.at(kProjection).name_.c_str(), axes.at(kProjection).bin_edges_});
      param.SetSliceAxis({axes.at(kSlice).name_.c_str(), axes.at(kSlice).bin_edges_});
      param.ShiftSliceAxis(axes.at(kSlice).shift_);
      param.Calculate();
      param.ShiftProjectionAxis(axes.at(kProjection).shift_);

      HeapPicture pic( (axes.at(kSelect).name_ + "_" + std::to_string(iEdge)).c_str(), {1000, 1000});

      auto leg1 = new TLegend();
      leg1->SetBorderSize(1);
      leg1->SetHeader((axes.at(kSlice).title_+axes.at(kSlice).unit_).c_str());

      for( auto obj : param.GetProjections() ){
        pic.AddDrawable( obj );
        leg1->AddEntry( obj->GetPoints(), obj->GetTitle().c_str(), "P" );
      }

      pic.SetAxisTitles({(axes.at(kProjection).title_ + axes.at(kProjection).unit_).c_str(), va.second.c_str()});

      pic.CustomizeXRange();
      pic.CustomizeYRange();
      pic.AddLegend(leg1);
      pic.CustomizeLegend(leg1);
      pic.Draw();

      if(is_first_canvas)
        pic.GetCanvas()->Print((va.second + ".pdf(").c_str(), "pdf");
      else
        pic.GetCanvas()->Print((va.second + ".pdf").c_str(), "pdf");
      is_first_canvas = false;
    }
    TCanvas emptycanvas("", "", 1000, 1000);
    emptycanvas.Print((va.second + ".pdf)").c_str(), "pdf");

  }

}
