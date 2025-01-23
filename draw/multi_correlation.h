//
// Created by mikhail on 8/11/21.
//

#ifndef FLOW_DRAWING_TOOLS_DRAW_MULTI_CORRELATION_H_
#define FLOW_DRAWING_TOOLS_DRAW_MULTI_CORRELATION_H_

#include "correlation.h"
#include "graph.h"
#include "readable_object.h"
#include <QnTools/DataContainer.hpp>

class MultiCorrelation : public TObject {
public:
  MultiCorrelation() = default;
  MultiCorrelation(MultiCorrelation const &other) :marker_(other.marker_),
                                                    palette_(other.palette_){
      for( auto corr : other.correlations_ )
        correlations_.push_back( new Correlation(*corr) );
  };
  ~MultiCorrelation() override = default;
  void AddCorrelation( const std::string& file, const std::vector<std::string>& objects, const std::string& title );
  void AddCorrelation( Correlation* correlation ) { correlations_.push_back(new Correlation(*correlation)); }
  void Scale( double scale );
  void Rebin( const std::vector<Qn::AxisD>& axes);
  void Select( const std::vector<Qn::AxisD>& axes);
  void Project(const std::vector<std::string>& axes);
  void RebinAndSelect( const std::vector<Qn::AxisD>& axes);
  void SetMarkers(const std::vector<int> &markers) { markers_ = markers; }
  [[nodiscard]] const std::vector<Correlation*> &GetCorrelations() {
    this->ApplyStyle();
    return correlations_;
  }
  Correlation* GetCorrelation(int i) { return correlations_.at(i); }
  void SetMarker(int marker) { marker_ = marker; }
  void SetPalette(const std::vector<int> &palette) { palette_ = palette; }
  void SetIsFillLines(float alpha=0.3) { fill_line_alpha_ = {alpha}; }
  void SetIsFillLines(const std::vector<float>& alphas) { fill_line_alpha_ = alphas; }
  void SetErrorOption(const std::string &error_option);
  void ShiftXaxis( float value );
  void SlightShiftXAxis( float value );
  void SetIsFillSysErrors(bool value=true) { is_sys_errors_ = {value}; }
  void SetIsFillSysErrors(const std::vector<bool>& value) { is_sys_errors_ = value; }
  void RenameAxis(const std::string& from, const std::string& to);

protected:
  void ApplyStyle();
  std::vector<bool> is_sys_errors_{false};
  std::vector<Correlation*> correlations_;
  std::vector<int> markers_;
  std::vector<float> fill_line_alpha_{-1};
  int marker_{kFullCircle};
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
  ClassDefOverride( MultiCorrelation, 1 );
};

#endif // FLOW_DRAWING_TOOLS_DRAW_MULTI_CORRELATION_H_
