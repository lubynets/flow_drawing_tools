//
// Created by mikhail on 2/28/21.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_CORRELATION_H_
#define FLOW_DRAWING_TOOLS_SRC_CORRELATION_H_

#include <utility>

#include <DataContainer.hpp>

#include "drawable_object.h"

//***************************************************************
// To be uncommented if use with StatDiscriminator and to leave commented if not:

#define DiscriminatorMode

// Very bad practice, sorry. To be fixed ASAP
//***************************************************************


#ifdef DiscriminatorMode
#define DataContainerStatMagic DataContainerStatDiscriminator
#else
#define DataContainerStatMagic DataContainerStatCalculate
#endif

class Correlation : public DrawableObject {
public:
  Correlation() = default;
  Correlation(Correlation const&) = default;
  Correlation(const std::string &file_name,
              const std::vector<std::string> &objects,
              const std::string &title);
  ~Correlation() override;
  void RefreshPoints() override;
  Qn::DataContainerStatMagic &GetCorrelation() {
    return average_;
  }
  void Rebin( const std::vector<Qn::AxisD>& axes){
    for( const auto& axis : axes) {
      average_ = average_.Rebin(axis);
      for( auto& container : combinations_ )
        container = container.Rebin(axis);
    }
  }
  void Select( const std::vector<Qn::AxisD>& axes){
    for( const auto& axis : axes) {
      average_ = average_.Select(axis);
      for( auto& container : combinations_ )
        container = container.Select(axis);
    }
  }
  void Project(const std::vector<std::string>& axes){
    average_ = average_.Projection(axes);
    for( auto& container : combinations_ )
      container = container.Projection(axes);
  }
  void Scale(double num){
    average_ = average_ *num;
    for( auto& container : combinations_ )
      container = container*num;
  }
  friend Correlation operator/( const Correlation& num, const Correlation& den);
  void SetCalculateSystematicsFromVariation(
      bool calculate_systematics_from_variation = true) {
    calculate_systematics_from_variation_ =
        calculate_systematics_from_variation;
  }

  friend Correlation Plus(const Correlation& lhs, const Correlation& rhs);
  friend Correlation Minus(const Correlation& lhs, const Correlation& rhs);
  friend Correlation Multiply(const Correlation& lhs, const Correlation& rhs);
  friend Correlation Divide(const Correlation& lhs, const Correlation& rhs);
  void DivideValueByError();

  void SetErrorType(Qn::Stat::ErrorType type) { error_type_ = type; }
#ifdef DiscriminatorMode
  void SetMeanType(Qn::Stat::ErrorType type) { mean_type_ = type; }
#endif

protected:
  Qn::DataContainerStatMagic average_;
  std::vector<Qn::DataContainerStatMagic> combinations_;
  bool calculate_systematics_from_variation_{false};
  Qn::Stat::ErrorType error_type_{Qn::Stat::ErrorType::BOOTSTRAP};
#ifdef DiscriminatorMode
  Qn::Stat::ErrorType mean_type_{Qn::Stat::ErrorType::PROPAGATION};
#endif
  ClassDefOverride(Correlation, 1)
};


#endif // FLOW_DRAWING_TOOLS_SRC_CORRELATION_H_
