#ifndef FLOW_DRAWING_TOOLS_DRAW_DD_CORRELATION_DIFFERENCE_H_
#define FLOW_DRAWING_TOOLS_DRAW_DD_CORRELATION_DIFFERENCE_H_

#include "double_differential_correlation.h"

class DDCorrelationDifference {
public:
  DDCorrelationDifference() = default;
  void SetMinuend(DoubleDifferentialCorrelation& minuend) { minuend_ = minuend; }
  void SetSubtrahend(DoubleDifferentialCorrelation& subtrahend) { subtrahend_ = subtrahend; }
  void Calculate();
  
  void IgnoreMinuendErrors(bool is = true) { is_ignore_minuend_errors_ = is; }
  void IgnoreSubtrahendErrors(bool is = true) { is_ignore_subtrahend_errors_ = is; }
  
protected:
  TGraphErrors* CalculateDifference(TGraphErrors* minuend, TGraphErrors* subtrahend);
  TGraphErrors* CalculateDifferenceOverError(TGraphErrors* minuend, TGraphErrors* subtrahend);
  
  DoubleDifferentialCorrelation minuend_;
  DoubleDifferentialCorrelation subtrahend_;
  DoubleDifferentialCorrelation difference_;
  DoubleDifferentialCorrelation difference_over_error_;
   
  bool is_ignore_minuend_errors_{false};
  bool is_ignore_subtrahend_errors_{false};  
};
#endif // FLOW_DRAWING_TOOLS_DRAW_DD_CORRELATION_DIFFERENCE_H_
