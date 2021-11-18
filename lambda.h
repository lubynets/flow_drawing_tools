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

void SetAxis(const std::string& name, const std::string& type) {
  int i=0;
  for(auto& ax : axes){
    if(ax.name_ == name){
      if(type == "select") {
        kSelect = i;
      }
      else if(type == "slice") {
        kSlice = i;
      }
      else if(type == "projection") {
        kProjection = i;
      }
      else
        std::cout << "No axis with type " << type << "\n";
      break;
    }
    i++;
  }
  if(i == axes.size())
    std::cout << "No axis with name " << name << "\n";
}

void IntegrateSelectAxis() {
  axes.at(kSelect).bin_edges_ = {*axes.at(kSelect).bin_edges_.begin(), *(axes.at(kSelect).bin_edges_.end()-1)};
}

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}
