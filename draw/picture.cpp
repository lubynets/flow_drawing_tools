//
// Created by mikhail on 2/28/21.
//

#include "picture.h"
#include "TLegendEntry.h"

Picture::~Picture() {
  for(auto& hl : horizontal_lines_) {
    delete hl;
  }
  for(auto& fu : functions_) {
    delete fu;
  }
}

void Picture::CustomizeXRange(float part) {
  const float diff = xmax_ - xmin_;
  const float up = xmax_ + (1-part)/2*diff/part;
  const float down = xmin_ - (1-part)/2*diff/part;
  SetXRange({down, up});
}

void Picture::CustomizeYRange(float part) {
  const float diff = ymax_ - ymin_;
  const float up = ymax_ + (1-part)/2*diff/part;
  const float down = ymin_ - (1-part)/2*diff/part;
  SetYRange({down, up});
}

void Picture::CustomizeXRangeWithLimits(float lo, float hi, float part) {
  const float diff = xmax_ - xmin_;
  const float up = std::min(hi, xmax_ + (1-part)/2*diff/part);
  const float down = std::max(lo, xmin_ - (1-part)/2*diff/part);
  SetXRange({down, up});
}

void Picture::CustomizeYRangeWithLimits(float lo, float hi, float part) {
  const float diff = ymax_ - ymin_;
  const float up = std::min(hi, ymax_ + (1-part)/2*diff/part);
  const float down = std::max(lo, ymin_ - (1-part)/2*diff/part);
  SetYRange({down, up});
}

bool Picture::OverlapRectangles(std::vector<float> rect1, std::vector<float> rect2) const {
  if(rect1.at(kX1) > rect2.at(kX2) || rect1.at(kX2) < rect2.at(kX1))
    return false;
  if(rect1.at(kY1) > rect2.at(kY2) || rect1.at(kY2) < rect2.at(kY1))
    return false;
  
  return true;
}

std::vector<float> Picture::TransformToUser(std::vector<float> x) const {
  // Transforms from Pad coordinates to User coordinates.
  // This can probably be replaced by using the built-in conversion commands.

  const float xstart = x_range_.at(0);
  const float xlength = x_range_.at(1) - xstart;
  float xlow = xlength * x.at(kX1) + xstart;
  float xhigh = xlength * x.at(kX2) + xstart;

  const float ystart = y_range_.at(0);
  const float ylength = y_range_.at(1) - ystart;
  float ylow = ylength * x.at(kY1) + ystart;
  float yhigh = ylength * x.at(kY2) + ystart;

  return {xlow, ylow, xhigh, yhigh};
}

bool Picture::OverlapWithGraph(TGraph* graph, std::vector<float> rect2) const {
  const float y2x = (y_range_.at(1) - y_range_.at(0))/(x_range_.at(1) - x_range_.at(0));
  const double minex = (graph->GetPointX(1)-graph->GetPointX(0))/4.;
  const double miney = minex*y2x;
  
  for(int i=0; i<graph->GetN(); i++) {
    const float x = graph->GetPointX(i);
    const float y = graph->GetPointY(i);
    const float ex = std::max(graph->GetErrorX(i), minex);
    const float ey = std::max(graph->GetErrorY(i), miney);
    
    if(OverlapRectangles(rect2, {x-ex, y-ey, x+ex, y+ey}))
      return true;
  }
  return false;
}

std::pair<float, float> Picture::GetOptimalLegendSize(TLegend* leg) const {
  int max_length = -1;
  TList* primitives = leg->GetListOfPrimitives();
  for(int iP=0; iP<primitives->GetEntries(); iP++) {
    TLegendEntry* lentry = (TLegendEntry*)primitives->At(iP);
    std::string label = lentry->GetLabel();
    int length = label.length();
    max_length = std::max(max_length, length);
  }
  const int nrows = leg->GetNRows();
  const float width = 0.012 + 0.012 * max_length;
  const float height = 0.03 * nrows;
  return {width, height}; 
}

void Picture::AddHorizontalLine(float value) {
  horizontal_lines_.emplace_back(new TF1( "zero_line", "[0]", -100, 100 ));
  horizontal_lines_.back()->SetParameter(0, value);
}

void Picture::AddText( TLatex text, float size) {
  texts_.push_back(new TLatex(text));
  text_sizes_.push_back(size);
  text_intramargin_xy_.push_back(std::make_pair(-1., -1.));
}

void Picture::AddText( std::string text, float size, std::pair<float, float> intramargin_xy ) {
  texts_.push_back(new TLatex({0., 0., text.c_str()}));
  text_sizes_.push_back(size);
  text_intramargin_xy_.push_back(intramargin_xy);
}

ClassImp(Picture);
