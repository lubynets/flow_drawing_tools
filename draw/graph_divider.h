#ifndef FLOW_DRAWING_TOOLS_DRAW_GRAPH_DIVIDER_H_
#define FLOW_DRAWING_TOOLS_DRAW_GRAPH_DIVIDER_H_

#include "graph.h"

class GraphDivider {
public:
  void SetNumerator(Graph* numerator) { numerator_ = numerator; }
  void SetDenominator(Graph* denominator) { denominator_ = denominator; }

  void IgnoreNumeratorErrors(bool is = true) { is_ignore_numerator_errors_ = is; }
  void IgnoreDenominatorErrors(bool is = true) { is_ignore_denominator_errors_ = is; }

  void Calculate();
  Graph* GetResult() const;
  std::vector<float> GetPointsValues() const;
  std::vector<float> GetPointsErrors() const;

protected:
  Graph* numerator_{nullptr};
  Graph* denominator_{nullptr};
  Graph* result_{nullptr};

  std::pair<float, float> CalculateRatio(float vmc, float errmc, float vr, float errr) const;

  bool is_ignore_numerator_errors_{false};
  bool is_ignore_denominator_errors_{false};
};

#endif // FLOW_DRAWING_TOOLS_DRAW_GRAPH_DIVIDER_H_
