//
// Created by mikhail on 2/28/21.
//

#include "graph.h"
#include <TGraphAsymmErrors.h>
ClassImp(Graph);

Graph::Graph( DrawableObject* other ) : DrawableObject(*other) {
  points_ = (TGraphMultiErrors*)other->GetPoints()->Clone();
};

void Graph::RecalculateXaxis( const std::vector<double>& x_axis ){
  for( int i=0; i<std::max(points_->GetN(), (Int_t)x_axis.size()); i++ ){
    if(i>points_->GetN()-1) {
      points_->RemovePoint(i);
      continue;
    }
    auto x = x_axis.at(i);
    points_->SetPointX(i, x);
  }
}

void Graph::ShiftXaxis( const float value ){
  for(int i=0; i<points_->GetN(); i++){
    auto x = points_->GetPointX(i);
    points_->SetPointX(i, x+value);
  }
}

Graph::Graph(const std::string &file_name,
             const std::vector<std::string> &objects, const std::string &title)
    : DrawableObject(file_name, objects, title) {
  //NOTE This constructor does not accept TGraphMultiErrors as an input. Maybe fix later?
  std::vector<TGraphErrors*> graphs;
  graphs.reserve(objects.size());
  for( const auto& name : objects ){
    try {
      graphs.push_back(this->ReadObjectFromFile<TGraphErrors>(name));
    } catch (std::exception&) {
      graphs.push_back((TGraphErrors*)this->ReadObjectFromFile<TGraphAsymmErrors>(name));
    }
  }
  points_ = new TGraphMultiErrors(graphs.front()->GetN() );
  for( int i=0; i < graphs.front()->GetN(); ++i ){
    double x = graphs.front()->GetPointX(i);
    double y = 0;
    double xerr=0;
    double yerr=0;
    for( auto graph : graphs){
      y+=graph->GetPointY(i);
      yerr+=graph->GetErrorY(i)*graph->GetErrorY(i);
      xerr+=graph->GetErrorX(i)*graph->GetErrorX(i);
    }
    y/= (double) graphs.size();
    yerr/= (double) graphs.size();
    yerr = sqrt(yerr);
    xerr/= (double) graphs.size();
    xerr= sqrt(xerr);
    points_->SetPoint(i, x, y);
    points_->SetPointEX(i, xerr, xerr);
    points_->SetPointEY(i, 0, yerr, yerr);
  }
  this->SetMarkerStyle();
}
void Graph::RefreshPoints() { this->SetMarkerStyle(); }
Graph::~Graph() {}
