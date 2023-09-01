//
// Created by mikhail on 8/11/21.
//

#include "multi_correlation.h"

ClassImp(MultiCorrelation);

void MultiCorrelation::ApplyStyle() {
  std::vector<int> colors;
  if( correlations_.size() < palette_.size() ){
    auto bias = palette_.size() / correlations_.size();
    for( size_t i=0; i<correlations_.size(); ++i )
      colors.push_back(palette_.at(i*bias));
  }else{
    auto bias=0;
    auto position=0;
    for( size_t i=0; i<correlations_.size(); ++i ) {
      if( i<palette_.size() ) {
        colors.push_back(palette_.at(position+bias));
        position++;
      } else{
        position=0;
        bias+=3;
      }
    }
  }
  int i=0;
  if( markers_.size() == 1 ) markers_ = std::vector<int>(correlations_.size(), markers_.front());
  if(is_sys_errors_.size() == 1 ) is_sys_errors_ = std::vector<bool>(correlations_.size(), is_sys_errors_.front());
  if( is_fill_lines_.size() == 1 ) is_fill_lines_ = std::vector<bool>(correlations_.size(), is_fill_lines_.front());
  for( auto correlation : correlations_ ){
    correlation->SetStyle(colors.at(i), markers_.at(i));
    correlation->SetCalculateSystematicsFromVariation(is_sys_errors_.at(i));
    correlation->SetIsFillLine(is_fill_lines_.at(i));
    ++i;
  }
}
void MultiCorrelation::Scale(double scale) {
  for( auto &corr : correlations_ ){
    corr->Scale(scale);
  }
}
void MultiCorrelation::Rebin(const std::vector<Qn::AxisD> &axes) {
  for( auto &corr : correlations_ ){
    corr->Rebin(axes);
  }
}
void MultiCorrelation::Select(const std::vector<Qn::AxisD> &axes) {
  for( auto &corr : correlations_ ){
    corr->Select(axes);
  }
}
void MultiCorrelation::Project(const std::vector<std::string>& axes) {
  for( auto &corr : correlations_ ){
    corr->Project(axes);
  }
}
void MultiCorrelation::RebinAndSelect(const std::vector<Qn::AxisD>& axes) {
  this->Rebin(axes);
  this->Select(axes);
}
void MultiCorrelation::SetErrorOption(const std::string &error_option) {
  for( auto &corr : correlations_ ){
    corr->SetErrorOption(error_option);
  }
}
void MultiCorrelation::AddCorrelation(const std::string& file,
                                      const std::vector<std::string>& objects,
                                      const std::string& title) {
  correlations_.push_back( new Correlation( file, objects, title ) );
}

void MultiCorrelation::SlightShiftXAxis( float value ) {
  const int ncorrelations = correlations_.size();
  for(int i=0; i<ncorrelations; i++) {
    const float shift = (-1.*ncorrelations/2 + 0.5 + i)*value;
    for(int j=0; j<correlations_.at(i)->GetPoints()->GetN(); j++){
      auto x = correlations_.at(i)->GetPoints()->GetPointX(j);
      correlations_.at(i)->GetPoints()->SetPointX(j, x+shift);
    }
  }
}

void MultiCorrelation::RenameAxis(const std::string& from, const std::string& to) {
  for(auto& cl : correlations_) {
    try {
      cl->RenameAxis(from, to);
      std::cout << "INFO: Success MultiCorrelation::RenameAxis("<< from << ", " << to << ") to " <<
      cl->GetTitle() << "\n";
    } catch (std::exception&) {
      std::cout << "Warning: MultiCorrelation::RenameAxis("<< from << ", " << to << ") is not applicable to " <<
      cl->GetTitle() << "\n";
    }
  }
}

void MultiCorrelation::ShiftXaxis(float value) {
  for(auto& cl : correlations_) {
    cl->ShiftXaxis(value);
  }
}
