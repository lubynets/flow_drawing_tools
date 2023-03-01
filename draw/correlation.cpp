//
// Created by mikhail on 2/28/21.
//

#include "correlation.h"
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
    points_ = Qn::ToTGraph(average_);
  }
  for( int i=0; i<points_->GetN(); ++i ){
    auto y_err = points_->GetErrorY(i);
    if( isnan( y_err ) )
      points_->SetPointError(i, 0., 0.);
  }
  if( fit_ )
    points_->Fit(fit_);
  if( calculate_systematics_from_variation_ ){
    std::vector<Qn::DataContainerStatMagic> variations;
    for( const auto& combination : combinations_ ){
      variations.emplace_back( average_ - combination );
    }
    if(sys_error_points_ == nullptr) {
      sys_error_points_ = Qn::ToTGraph( average_ );
    }
    for( int i=0; i<sys_error_points_->GetN(); ++i ){
      auto x_hi = average_.GetAxes().front().GetUpperBinEdge(i);
      auto x_lo = average_.GetAxes().front().GetLowerBinEdge(i);
      auto x_err = (x_hi - x_lo) / 4;
      double y_err=0;
      for( auto var : variations ){
        auto err = fabs(var.At(i).Mean());
        if( y_err < err )
          y_err=err;
//        y_err += var.At(i).Mean()*var.At(i).Mean();
      }
//      y_err= sqrt(y_err);
      sys_error_points_->SetPointError(i, x_err, y_err);
    }
  }
  this->SetMarkerStyle();
}
