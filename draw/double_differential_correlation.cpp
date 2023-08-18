//
// Created by mikhail on 5/7/21.
//

#include "double_differential_correlation.h"

#include "QnToolsHelper.h"

#include <iomanip>

void DoubleDifferentialCorrelation::Calculate() {
  correlation_.SetErrors(error_type_);
  for( auto& container : combinations_ ) {
    container.SetErrors(error_type_);
  }
#ifdef DiscriminatorMode
  for(auto& av : correlation_) {
    av.SetMeanType(mean_type_);
  }
  for( auto& container : combinations_ ) {
    for(auto& av : container) {
      av.SetMeanType(mean_type_);
    }
  }
#endif
  this->Rebin({projection_axis_, slice_axis_});
  for( size_t slice_bin=0; slice_bin <slice_axis_.size(); slice_bin++ ){
    std::string name{ slice_axis_.Name() };
    auto lo = slice_axis_.GetLowerBinEdge(slice_bin);
    auto hi = slice_axis_.GetUpperBinEdge(slice_bin);
    auto proj_container = correlation_.Select( {name, 1, lo, hi} );
    proj_container = proj_container.Projection({projection_axis_.Name()});
    lo += slice_axis_shift_;
    hi += slice_axis_shift_;
    std::string graph_name{ name+"_"+std::to_string(slice_bin) };
    if(draw_errors_as_mean_.first == false) {
      projection_points_.push_back( Qn::ToTGraph( proj_container ) );
      if(calculate_systematics_from_variation_) {
        std::vector<Qn::DataContainerStatMagic> variations;
        for( const auto& combination : combinations_ ){
          variations.emplace_back( proj_container - combination.Select( {name, 1, lo, hi} ).Projection({projection_axis_.Name()}) );
        }
        combinations_points_.push_back( Qn::ToTGraph( proj_container ) );
        auto x_hi = proj_container.GetAxes().front().GetLowerBinEdge(0);
        auto x_lo = proj_container.GetAxes().front().GetLowerBinEdge(proj_container.GetAxes().front().GetNBins());
        auto x_err = (x_hi - x_lo) / 40;
        for( int i=0; i<combinations_points_.back()->GetN(); ++i ){
          double y_err=0;
          for( auto var : variations ){
            y_err += var.At(i).Mean()*var.At(i).Mean();
          }
          y_err = std::sqrt(y_err/variations.size());
          combinations_points_.back()->SetPointError(i, x_err, y_err);
        }
      }
    } else {
      projection_points_.push_back( ErrorsToTGraph(proj_container, draw_errors_as_mean_.second) );
    }
    projection_points_.back()->SetName( graph_name.c_str() );
    std::ostringstream stream_lo;
    stream_lo << std::setprecision(2) << lo;
    std::ostringstream stream_hi;
    stream_hi << std::setprecision(2) << hi;
    std::string title{ stream_lo.str()+" - "+stream_hi.str() };
    projection_points_.back()->SetTitle(title.c_str());
  }
  FillGraphs();
}
void DoubleDifferentialCorrelation::FillGraphs() {
  std::vector<int> colors;
  if( projection_points_.size() < palette_.size() && bias_palette_ ){
    auto bias = palette_.size() / projection_points_.size();
    for( size_t i=0; i<projection_points_.size(); ++i )
      colors.push_back(palette_.at(i*bias));
  }else{
    auto bias=0;
    auto position=0;
    while( projection_points_.size() > colors.size() ){
      if( position >= palette_.size() ){
        position=0;
        bias++;
      }
      colors.push_back( palette_.at( position )+bias );
      position++;
    }
  }
  int i=0;
  for( auto graph : projection_points_ ){
    projections_.push_back( new Graph );
    projections_.back()->SetPoints( graph );
    if(calculate_systematics_from_variation_) projections_.back()->SetSysErrorPoints( combinations_points_.at(i) );
    projections_.back()->SetTitle(graph->GetTitle());
    projections_.back()->SetStyle(colors.at(i), marker_);
    projections_.back()->SetIsFillLine(is_fill_line_);
    projections_.back()->SetErrorOption(error_option_);
    graph->SetLineColor(colors.at(i));
    graph->SetMarkerColor(colors.at(i));
    graph->SetMarkerColor(marker_);
    ++i;
  }
  ExeShiftProjectionAxis();
  ExeSlightShiftProjectionAxis();
}
void DoubleDifferentialCorrelation::SaveToFile(const std::string &file_name) {
  auto file_out = TFile::Open( file_name.c_str(), "recreate" );
  file_out->cd();
  for (auto  prj : projection_points_)
    prj->Write();
  file_out->cd("/");
  file_out->mkdir("slopes");
  file_out->cd("slopes");
}
void DoubleDifferentialCorrelation::SetErrorOption(
    const std::string &error_option) {
  error_option_ = error_option;
}

void DoubleDifferentialCorrelation::RecalculateProjectionAxis( const std::vector<double>& x_axis ){
  for(auto& p : projections_)
    p->RecalculateXaxis(x_axis);  
}

void DoubleDifferentialCorrelation::ExeShiftProjectionAxis(){
  for(auto& p : projections_)
    p->ShiftXaxis(shift_projection_axis_);
}

void DoubleDifferentialCorrelation::ExeSlightShiftProjectionAxis() {
  const int nprojections = projections_.size();
  const float gap = slight_shift_projection_axis_.first;
  const float moveall = slight_shift_projection_axis_.second;
  for(int i=0; i<nprojections; i++) {
    const float shift = (-1.*nprojections/2 + 0.5 + i)*gap + moveall;
    projections_.at(i)->ShiftXaxis(shift);
  }
}

DoubleDifferentialCorrelation Plus(const DoubleDifferentialCorrelation& lhs, const DoubleDifferentialCorrelation& rhs) {
  DoubleDifferentialCorrelation result = lhs;
  result.correlation_ = lhs.correlation_ + rhs.correlation_;
  result.projection_points_.clear();
  result.projections_.clear();
  result.Calculate();

  return result;
}

DoubleDifferentialCorrelation Minus(const DoubleDifferentialCorrelation& lhs, const DoubleDifferentialCorrelation& rhs) {
  DoubleDifferentialCorrelation result = lhs;
  result.correlation_ = lhs.correlation_ - rhs.correlation_;
  result.projection_points_.clear();
  result.projections_.clear();
  result.Calculate();

  return result;
}

DoubleDifferentialCorrelation Multiply(const DoubleDifferentialCorrelation& lhs, const DoubleDifferentialCorrelation& rhs) {
  DoubleDifferentialCorrelation result = lhs;
  result.correlation_ = lhs.correlation_ * rhs.correlation_;
  result.projection_points_.clear();
  result.projections_.clear();
  result.Calculate();

  return result;
}

DoubleDifferentialCorrelation Divide(const DoubleDifferentialCorrelation& lhs, const DoubleDifferentialCorrelation& rhs) {
  DoubleDifferentialCorrelation result = lhs;
  result.correlation_ = lhs.correlation_ / rhs.correlation_;
  result.projection_points_.clear();
  result.projections_.clear();
  result.Calculate();

  return result;
}

void DoubleDifferentialCorrelation::DivideValueByError() {
  projections_.clear();
  for(auto& pp : projection_points_) {
    if(pp == nullptr) {
      throw std::runtime_error("DoubleDifferentialCorrelation::DivideValueByError() - projection_points_[] is nullptr");
    }

    for(int ip=0; ip<pp->GetN(); ip++) {
      const float y = pp->GetPointY(ip);
      const float ey = pp->GetErrorY(ip);
      pp->SetPointY(ip, y/ey);
      pp->SetPointError(ip, 0., 0.);
    }
  }
  FillGraphs();
}

void DoubleDifferentialCorrelation::RenameAxis(const std::string& from, const std::string& to) {
  std::vector<Qn::AxisD>& axes = correlation_.GetAxes();
  for(Qn::AxisD& ax : axes) {
    if(ax.Name() == from) {
      ax.SetName(to);
      return;
    }
  }
  throw std::runtime_error("DoubleDifferentialCorrelation::RenameAxis() - axis from is absent");
}

#ifdef DiscriminatorMode
  void DoubleDifferentialCorrelation::SetMeanType(Qn::Stat::ErrorType type) { mean_type_ = type; }
#else
  void DoubleDifferentialCorrelation::SetMeanType(Qn::Stat::ErrorType type) {
    std::cout << "Warning! DoubleDifferentialCorrelation::SetMeanType() is applicable only in QnDiscriminator mode\n";
  }
#endif
