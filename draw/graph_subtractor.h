#ifndef FLOW_DRAWING_TOOLS_DRAW_GRAPH_SUBTRACTOR_DIFFERENCE_H_
#define FLOW_DRAWING_TOOLS_DRAW_GRAPH_SUBTRACTOR_DIFFERENCE_H_

#include "graph.h"

class GraphSubtractor {
public:
  void SetMinuend(Graph* minuend) { minuend_ = minuend; }
  void SetSubtrahend(Graph* subtrahend) { subtrahend_ = subtrahend; }
  
  void DivideOverError(bool is = true) { is_divide_over_error_ = is; }
  void IgnoreMinuendErrors(bool is = true) { is_ignore_minuend_errors_ = is; }
  void IgnoreSubtrahendErrors(bool is = true) { is_ignore_subtrahend_errors_ = is; }
  
  void Calculate();
  Graph* GetResult() { return result_; }
  
protected:
  Graph* minuend_{nullptr};
  Graph* subtrahend_{nullptr};
  Graph* result_{nullptr};
  
  bool is_ignore_minuend_errors_{false};
  bool is_ignore_subtrahend_errors_{false};
  bool is_divide_over_error_{true};
};
#endif // FLOW_DRAWING_TOOLS_DRAW_GRAPH_SUBTRACTOR_DIFFERENCE_H_
