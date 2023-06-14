//
// Created by mikhail on 5/7/21.
//

#ifndef FLOW_DRAWING_TOOLS_DRAW_DOUBLE_DIFFERENTIAL_CORRELATION_H_
#define FLOW_DRAWING_TOOLS_DRAW_DOUBLE_DIFFERENTIAL_CORRELATION_H_

#include <TGraphErrors.h>
#include <TF1.h>
#include <DataContainer.hpp>

#include "correlation.h"
#include "graph.h"
#include "readable_object.h"

class DoubleDifferentialCorrelation : public ReadableObject {
public:
  DoubleDifferentialCorrelation() = default;
  DoubleDifferentialCorrelation(const std::string &file_name,
                                const std::vector<std::string> &objects)
      : ReadableObject(file_name, objects) {
    std::vector<Qn::DataContainerStatMagic> containers;
    for( const auto& name : objects ){
#ifdef DiscriminatorMode
      try {
        containers.emplace_back(
            *(this->ReadObjectFromFile<Qn::DataContainerStatDiscriminator>(name)));
      } catch (std::exception&) {
      std::cout << "Warning: DoubleDifferentialCorrelation::DoubleDifferentialCorrelation(" <<
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
    correlation_ = containers.front();
    for( size_t i=1; i< containers.size(); ++i )
      correlation_ = correlation_+ containers.at(i);
    correlation_ = correlation_/ (double)containers.size();
  }
  ~DoubleDifferentialCorrelation() override = default;
  Qn::DataContainerStatMagic &GetCorrelation() {
    return correlation_;
  }
  void Scale( double scale ){ correlation_ = correlation_*scale; }
  void Rebin( const std::vector<Qn::AxisD>& axes){
    for( const auto& axis : axes)
      correlation_ = correlation_.Rebin(axis);
  }
  void Select( const std::vector<Qn::AxisD>& axes){
    for( const auto& axis : axes)
      correlation_ = correlation_.Select(axis);
  }
  void SetSliceVariable(const std::string &name, const std::string &units ){
    slice_variable_name_ = name;
    slice_variable_units_ = units;
  }
  void Project(std::vector<std::string> axes){
    correlation_ = correlation_.Projection(std::move(axes));
  }
  void SetProjectionAxis(const Qn::AxisD &projection_axis) {
    projection_axis_ = projection_axis;
  }
  [[nodiscard]] const std::vector<Graph *> &GetProjections() const { return projections_; }
  void SetSliceAxis(const Qn::AxisD &slice_axis) { slice_axis_ = slice_axis; }
  void SetMarker(int marker) { marker_ = marker; }
  void SetIsFillLine(bool value=true) { is_fill_line_ = value; }
  void SetPalette(const std::vector<int> &palette) { palette_ = palette; }
  void SetBiasPalette(bool value=true) { bias_palette_ = value; }
  void Calculate();
  void SaveToFile( const std::string& file_name );
  void SetErrorOption(const std::string &error_option);
  void RecalculateProjectionAxis( const std::vector<double>& x_axis );
  
  // now ShiftProjectionAxis() must be called before Calculate()
  void ShiftProjectionAxis( float value ) { shift_projection_axis_ = value; };
  void SlightShiftProjectionAxis( float gap, float moveall=0 ) { slight_shift_projection_axis_ = std::make_pair(gap, moveall); };
  
  // ShiftSliceAxis() must be called after SetSliceAxis() but before Calculate()
  void ShiftSliceAxis( const float value )  { slice_axis_shift_ = value; }
  void SetErrorType(Qn::Stat::ErrorType type) { error_type_ = type; }
  void SetDrawErrorAsMean( bool is_draw, bool multiply_by_sqrt_N=false ) { draw_errors_as_mean_.first = is_draw; draw_errors_as_mean_.second = multiply_by_sqrt_N; }
  void SetMeanType(Qn::Stat::ErrorType type);

  void RenameAxis(const std::string& from, const std::string& to);

  friend DoubleDifferentialCorrelation Plus(const DoubleDifferentialCorrelation& lhs, const DoubleDifferentialCorrelation& rhs);
  friend DoubleDifferentialCorrelation Minus(const DoubleDifferentialCorrelation& lhs, const DoubleDifferentialCorrelation& rhs);
  friend DoubleDifferentialCorrelation Multiply(const DoubleDifferentialCorrelation& lhs, const DoubleDifferentialCorrelation& rhs);
  friend DoubleDifferentialCorrelation Divide(const DoubleDifferentialCorrelation& lhs, const DoubleDifferentialCorrelation& rhs);
  void DivideValueByError();

protected:
  void FillGraphs();
  void ExeShiftProjectionAxis();
  void ExeSlightShiftProjectionAxis();
  Qn::DataContainerStatMagic correlation_;
  Qn::AxisD projection_axis_;
  Qn::AxisD slice_axis_;
  float slice_axis_shift_{0};
  std::vector<TGraphErrors*> projection_points_;
  std::string slice_variable_name_;
  std::string slice_variable_units_;
  std::string error_option_;
  std::vector<Graph*> projections_;
  int marker_{kFullCircle};
  bool is_fill_line_{false};
  float shift_projection_axis_{0.f};
  std::pair<float, float> slight_shift_projection_axis_{0.f, 0.f};
  std::vector<int> palette_{
      kPink,
      kMagenta+1,
      kViolet-8,
      kBlue,
      kAzure-4,
      kCyan+1,
      kGreen+2,
      kSpring-4,
      kYellow-3,
      kOrange-3,
      kRed,
  };
  bool bias_palette_{false};
  std::pair<bool, bool> draw_errors_as_mean_{false, false};  // first: draw error or not; second: if draw - multiply by sqrt(Neff) or not
  Qn::Stat::ErrorType error_type_{Qn::Stat::ErrorType::BOOTSTRAP};
#ifdef DiscriminatorMode
  Qn::Stat::ErrorType mean_type_{Qn::Stat::ErrorType::PROPAGATION};
#endif
};

#endif // FLOW_DRAWING_TOOLS_DRAW_DOUBLE_DIFFERENTIAL_CORRELATION_H_
