std::string fileName_;
std::vector<double> select_axis_bin_edges_;
std::vector<double> projection_axis_bin_edges_;
std::vector<double> slice_axis_bin_edges_;

struct Axis
{
  std::string name_;
  std::string sim_name_;
  std::string reco_name_;
  std::string title_;
  std::vector<double> bin_edges_;
};

std::vector<Axis> axes
{
  {"pT", "SimParticles_pT", "RecParticlesMcPid_pT", "p_{T}, GeV"}, {}, 
  {"rapidity", "SimParticles_rapidity", "RecParticlesMcPid_rapidity", "y_{CM}"}, {}, 
  {"centrality", "AnaEventHeader_tracks_centrality", "AnaEventHeader_tracks_centrality", "Centrality, %"}, {}
};
  
void SetFileName(const std::string& filename){
  fileName_ = filename;
}

void SetSelectAxisBinEdges(const std::vector<double>& vec){
  select_axis_bin_edges_ = vec;
}

void SetProjectionAxisBinEdges(const std::vector<double>& vec){
  projection_axis_bin_edges_ = vec;
}

void SetSliceAxisBinEdges(const std::vector<double>& vec){
  slice_axis_bin_edges_ = vec;
}