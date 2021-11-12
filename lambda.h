std::string fileName_;
int kSlice=-1;
int kProjection=-1;
int kSelect=-1;

struct Axis
{
  std::string name_;
  std::string sim_name_;
  std::string reco_name_;
  std::string title_;
  std::string unit_;
  std::vector<double> bin_edges_;
  float shift_;
  int precision_;
};

std::vector<Axis> axes
{
  {"pT", "SimParticles_pT", "ReconstructedParticles_pT", "p_{T}", ", GeV/c", {}, 0., 1},
  {"rapidity", "SimParticles_rapidity", "ReconstructedParticles_rapidity", "y_{CM}", "", {}, -1.6217901, 1},
  {"centrality", "AnaEventHeader_centrality_tracks", "AnaEventHeader_centrality_tracks", "C", ", %", {}, 0., 0}
};
  
void SetFileName(const std::string& filename){
  fileName_ = filename;
}

void SetSelectAxis(const std::string& name){
  int i=0;
  for(auto& ax : axes){
    if(ax.name_ == name){
      kSelect = i;
      break;
    }
    i++;
  }
}

void SetProjectionAxis(const std::string& name){
  int i=0;
  for(auto& ax : axes){
    if(ax.name_ == name){
      kProjection = i;
      break;
    }
    i++;
  }
}

void SetSliceAxis(const std::string& name){
  int i=0;
  for(auto& ax : axes){
    if(ax.name_ == name){
      kSlice = i;
      break;
    }
    i++;
  }
}

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}
