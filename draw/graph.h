//
// Created by mikhail on 2/28/21.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_GRAPH_H_
#define FLOW_DRAWING_TOOLS_SRC_GRAPH_H_

#include "drawable_object.h"
#include "Helper.h"

class Graph : public DrawableObject {
public:
  Graph() = default;
  Graph(const std::string &file_name, const std::vector<std::string> &objects,
        const std::string &title);
  explicit Graph( DrawableObject* );
  ~Graph() override;
  void RefreshPoints() override;
  void RecalculateXaxis( const std::vector<double>& x_axis );
  void ShiftXaxis( const float value );
  void SetPoints( TGraphMultiErrors* graph ){ points_ = graph; }
  void SetPoints( TGraphErrors* graph ){ points_ = Helper::TGraphErrorsToTGraphMultiErrors(graph); }
  void SetSysErrorPoints( TGraphErrors* graph ){ Helper::AddErrorsToTGraphMultiErrors(points_, graph); }
  ClassDefOverride(Graph, 1)
};

#endif // FLOW_DRAWING_TOOLS_SRC_GRAPH_H_
