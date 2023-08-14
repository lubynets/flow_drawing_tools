//
// Created by mikhail on 2/28/21.
//

#include "picture.h"

#include "TLegendEntry.h"

#include <numeric>

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

void Picture::AddText( TLatex text, float size, int font) {
  texts_.push_back(new TLatex(text));
  text_sizes_.push_back(size);
  text_fonts_.push_back(font);
  text_intramargin_xy_.push_back(std::make_pair(-1., -1.));
}

void Picture::AddText( std::string text, std::pair<float, float> intramargin_xy, float size, int font ) {
  texts_.push_back(new TLatex({0., 0., text.c_str()}));
  text_sizes_.push_back(size);
  text_fonts_.push_back(font);
  text_intramargin_xy_.push_back(intramargin_xy);
}

void Picture::ManageTexts(float value, const std::string& option, std::vector<int> vec) {
  if(vec.size()==1 && vec.at(0) == -1) {
    vec.resize(texts_.size());
    std::iota(vec.begin(), vec.end(), 0); // fill vec with 0,1,2,3...
  }
  for(auto& vv : vec) {
    auto tx = texts_.at(vv);
    if(option == "size") tx->SetTextSize(value);
    else if(option == "font") tx->SetTextFont(value);
    else {
      throw std::runtime_error("Picture::ManageTexts() - option \"" + option + "\" is not valid one");
    }
  }
}

void Picture::ManageGraphs(float x, const std::string& option, std::vector<int> vec) {
  if(stack_ == nullptr) {
    throw std::runtime_error("Picture::ManageGraphs() - Picture::stack_ is a nullptr");
  }
  auto log = stack_->GetListOfGraphs();
  if(log == nullptr) {
    throw std::runtime_error("Warning: Picture::ManageGraphs() - Picture::stack_->GetListOfGraphs() is a nullptr");
  }
  if(vec.size()==1 && vec.at(0) == -1) {
    vec.resize(stack_->GetListOfGraphs()->GetEntries());
    std::iota(vec.begin(), vec.end(), 0); // fill vec with 0,1,2,3...
  }
  for(auto& vv : vec) {
    TGraph* gr = (TGraph*)log->At(vv);
    if(option == "width") gr->SetLineWidth(x);
    else if(option == "style") gr->SetLineStyle(x);
    else if(option == "markersize") gr->SetMarkerSize(x);
    else {
      throw std::runtime_error("Picture::ManageGraphs() - option \"" + option + "\" is not valid one");
    }
  }
}

void Picture::ManageLegends(float value, const std::string& option, std::vector<int> vec) {
  // Picture::ManageLegends() with option == "markersize" over-writes ManageGraphs with the same option
  if(vec.size()==1 && vec.at(0) == -1) {
    vec.resize(legends_.size());
    std::iota(vec.begin(), vec.end(), 0); // fill vec with 0,1,2,3...
  }
  for(auto& vv : vec) {
    auto leg = legends_.at(vv);
    if(option == "size") leg->SetTextSize(value);
    else if(option == "font") leg->SetTextFont(value);
    else if(option == "x1") leg->SetX1(value);
    else if(option == "x1NDC") leg->SetX1NDC(value);
    else if(option == "y1") leg->SetY1(value);
    else if(option == "y1NDC") leg->SetY1NDC(value);
    else if(option == "x2") leg->SetX2(value);
    else if(option == "x2NDC") leg->SetX2NDC(value);
    else if(option == "y2") leg->SetY2(value);
    else if(option == "y2NDC") leg->SetY2NDC(value);
    else if(option == "markersize") {
      TList* lop = leg->GetListOfPrimitives();
      for(int iP=0; iP<lop->GetEntries(); iP++) {
        TLegendEntry* lentry = (TLegendEntry*)lop->At(iP);
        auto obj = lentry->GetObject();
        if(obj->ClassName() == (TString)"TGraph" || obj->ClassName() == (TString)"TGraphErrors") {
          TGraph* gr = (TGraph*)obj;
          gr->SetMarkerSize(value);
        } else if(obj->ClassName() == (TString)"TLegendEntry") {
          TLegendEntry* le = (TLegendEntry*)obj;
          le->SetMarkerSize(value);
        }
      }
    }
    else {
      throw std::runtime_error("Picture::ManageLegends() - option \"" + option + "\" is not valid one");
    }
  }
}

void Picture::ClearLegends(std::vector<int> vec) {
  ClearVectorOfObjects(legends_, vec);
}

void Picture::ClearTexts(std::vector<int> vec) {
  ClearVectorOfObjects(texts_, vec);
}

template <typename T>
void Picture::ClearVectorOfObjects(std::vector<T*>& voo, std::vector<int> vec) {
  if(vec.size()==1 && vec.at(0) == -1) {
    vec.resize(voo.size());
    std::iota(vec.begin(), vec.end(), 0); // fill vec with 0,1,2,3...
  }
  std::sort(vec.begin(), vec.end(), std::greater<int>()); // sort vector from max to min since erasing is optimal in this way
  for(auto& vv : vec) {
    if(vv>=voo.size()) {
      std::cout << "Warning: Picture::ClearVectorOfObjects() - required element " <<
                   vv << " to be erased is out of vector's size\n";
    }
    delete voo.at(vv);
    voo.erase(voo.begin() + vv);
  }
}

void Picture::ScaleGraphs(float factor, std::vector<int> vec) {
  if(stack_ == nullptr) {
    throw std::runtime_error("Picture::ScaleGraphs() - Picture::stack_ is a nullptr");
  }
  auto log = stack_->GetListOfGraphs();
  if(log == nullptr) {
    throw std::runtime_error("Warning: Picture::ScaleGraphs() - Picture::stack_->GetListOfGraphs() is a nullptr");
  }
  if(vec.size()==1 && vec.at(0) == -1) {
    vec.resize(stack_->GetListOfGraphs()->GetEntries());
    std::iota(vec.begin(), vec.end(), 0); // fill vec with 0,1,2,3...
  }
  for(auto& vv : vec) {
    TGraphErrors* gr = (TGraphErrors*)log->At(vv);
    for(int iP=0; iP<gr->GetN(); iP++) {
      gr->GetY()[iP] *= factor;
      gr->GetEY()[iP] *= factor;
    }
  }
  ymax_ *= factor; //TODO what if scale not all graphs?
  ymin_ *= factor;
  CustomizeYRange();
}

ClassImp(Picture);
