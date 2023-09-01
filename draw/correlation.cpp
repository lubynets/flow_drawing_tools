//
// Created by mikhail on 2/28/21.
//

#include "correlation.h"

#include "Helper.h"
#include "QnToolsHelper.h"
ClassImp(Correlation);

Correlation::Correlation(const std::string &file_name,
                         const std::vector<std::string> &objects,
                         const std::string &title)
    : DrawableObject(file_name, objects, title) {
  std::vector<Qn::DataContainerStatMagic> containers;
  for( const auto& name : objects ){
#ifdef DiscriminatorMode
    try {
      containers.emplace_back(
          *(this->ReadObjectFromFile<Qn::DataContainerStatDiscriminator>(name)));
    } catch (std::exception&) {
      std::cout << "Warning: Correlation::Correlation(" <<
                   name << ") - StatCollect or StatCalculate will be casted to StatDiscriminator\n";
#endif
      try {
      containers.emplace_back(
          *(this->ReadObjectFromFile<Qn::DataContainerStatCalculate>(name)));
      } catch (std::exception&) {
        containers.emplace_back(
          *(this->ReadObjectFromFile<Qn::DataContainerStatCollect>(name)));
      }
#ifdef DiscriminatorMode
    }
#endif
  }
  average_ = containers.front();
  for( size_t i=1; i< containers.size(); ++i )
    average_ = average_ + containers.at(i);
  average_ = average_ / (double)containers.size();
  combinations_ = containers;
}
Correlation::~Correlation() {}

Correlation operator/( const Correlation& num, const Correlation& den){
  Correlation result;
  result.title_ = num.title_+"_ratio";
  result.marker_ = num.marker_;
  result.color_ = num.color_;
  auto num_container = num.average_;
  auto den_container = den.average_;
  auto res_container = num_container / den_container;
  result.average_ = res_container;

  return result;
}

void Correlation::RefreshPoints() {
  average_.SetErrors(error_type_);
#ifdef DiscriminatorMode
  for(auto& av : average_) {
    av.SetMeanType(mean_type_);
  }
#endif
  if(points_ == nullptr) {
    TGraphErrors* points{nullptr};
    if(draw_errors_as_mean_.test(0) == 0) {
      points = Qn::ToTGraph( average_ );
    } else {
      points = ErrorsToTGraph(average_, bool(draw_errors_as_mean_.test(1)));
    }
    points_ = Helper::TGraphErrorsToTGraphMultiErrors(points);
  }

  if( fit_ )
    points_->Fit(fit_);

  if( calculate_systematics_from_variation_ && points_->GetNYErrors() != 2 ){
    std::vector<Qn::DataContainerStatMagic> variations;
    for( const auto& combination : combinations_ ){
      variations.emplace_back( average_ - combination );
    }
    TGraphErrors* sys_error_points{nullptr};
    sys_error_points = Qn::ToTGraph(average_);
    for( int i=0; i<sys_error_points->GetN(); ++i ){
      auto x_hi = average_.GetAxes().front().GetLowerBinEdge(0);
      auto x_lo = average_.GetAxes().front().GetLowerBinEdge(average_.GetAxes().front().GetNBins());
      auto x_err = (x_hi - x_lo) / 60;
      double y_err=0;
      for( auto var : variations ){
        auto err = fabs(var.At(i).Mean());
        if( y_err < err )
        y_err += var.At(i).Mean()*var.At(i).Mean();
      }
      y_err = std::sqrt(y_err/variations.size());
      sys_error_points->SetPointError(i, x_err, y_err);
    }
    Helper::AddErrorsToTGraphMultiErrors(points_, sys_error_points);
  }
  this->SetMarkerStyle();
}

Correlation Plus(const Correlation& lhs, const Correlation& rhs) {
  Correlation result = lhs;
  result.average_ = lhs.average_ + rhs.average_;
  for(int ic=0; ic<lhs.combinations_.size(); ic++) {
    result.combinations_.at(ic) = lhs.combinations_.at(ic) + rhs.combinations_.at(ic);
  }

  return result;
}

Correlation Minus(const Correlation& lhs, const Correlation& rhs) {
  Correlation result = lhs;
  result.average_ = lhs.average_ - rhs.average_;
  for(int ic=0; ic<lhs.combinations_.size(); ic++) {
    result.combinations_.at(ic) = lhs.combinations_.at(ic) - rhs.combinations_.at(ic);
  }

  return result;
}

Correlation Multiply(const Correlation& lhs, const Correlation& rhs) {
  Correlation result = lhs;
  result.average_ = lhs.average_ * rhs.average_;
  for(int ic=0; ic<lhs.combinations_.size(); ic++) {
    result.combinations_.at(ic) = lhs.combinations_.at(ic) * rhs.combinations_.at(ic);
  }

  return result;
}

Correlation Divide(const Correlation& lhs, const Correlation& rhs) {
  Correlation result = lhs;
  result.average_ = lhs.average_ / rhs.average_;
  for(int ic=0; ic<lhs.combinations_.size(); ic++) {
    result.combinations_.at(ic) = lhs.combinations_.at(ic) / rhs.combinations_.at(ic);
  }

  return result;
}

void Correlation::DivideValueByError() {
  if(points_ == nullptr) {
    throw std::runtime_error("Correlation::DivideValueByError() - points_ is nullptr");
  }

  for(int ip=0; ip<points_->GetN(); ip++) {
    const float y = points_->GetPointY(ip);
    const float ey = points_->GetErrorY(ip, 0);
    points_->SetPointY(ip, y/ey);
    points_->SetPointEY(ip, 0, 0., 0.);
  }
}

void Correlation::RenameAxis(const std::string& from, const std::string& to) {
  std::vector<Qn::AxisD>& axes = average_.GetAxes();
  int iaxis{0};
  for(Qn::AxisD& ax : axes) {
    if(ax.Name() == from) {
      ax.SetName(to);
      for(auto& comb : combinations_) {
        comb.GetAxes().at(iaxis).SetName(to);
      }
      return;
    }
    iaxis++;
  }
  throw std::runtime_error("DoubleDifferentialCorrelation::RenameAxis() - axis from is absent");
}

void Correlation::ShiftXaxis( float value ){
  this->RefreshPoints();
  for(int i=0; i<points_->GetN(); i++){
    auto x = points_->GetPointX(i);
    points_->SetPointX(i, x+value);
  }
}

void Correlation::SetDrawErrorAsMean( bool is_draw, bool multiply_by_sqrt_N ) {
  if(is_draw == true) draw_errors_as_mean_.set(0);
  if(multiply_by_sqrt_N == true) draw_errors_as_mean_.set(1);
}

#ifdef DiscriminatorMode
  void Correlation::SetMeanType(Qn::Stat::ErrorType type) { mean_type_ = type; }
#else
  void Correlation::SetMeanType(Qn::Stat::ErrorType type) {
    std::cout << "Warning! Correlation::SetMeanType() is applicable only in QnDiscriminator mode\n";
  }
#endif
