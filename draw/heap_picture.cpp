//
// Created by mikhail on 3/6/21.
//

#include "heap_picture.h"
#include "TH1.h"
#include "TLegendEntry.h"
ClassImp(HeapPicture)

void HeapPicture::Draw() {
  if(auto_legend_){
    assert(legends_.empty());
    legends_.emplace_back( new TLegend() );
  }
  for( auto obj : drawable_objects_ ){
    if( obj->IsLine() ) {
      std::string opt{"L+X+" + obj->GetErrorOption()};
//       std::string opt{"E3" + obj->GetErrorOption()};
      stack_->Add(obj->GetPoints(), opt.c_str());
//       obj->GetPoints()->SetFillStyle(4050);
      if( auto_legend_ )
        legends_.back()->AddEntry(obj->GetPoints(), obj->GetTitle().c_str(),"L");
      if( obj->GetSysErrorPoints() )
        stack_->Add( obj->GetSysErrorPoints(), "L+2" );
    } else {
      std::string opt{"P+" + obj->GetErrorOption()};
      stack_->Add(obj->GetPoints(), opt.c_str());
      if( auto_legend_ )
        legends_.back()->AddEntry(obj->GetPoints(), obj->GetTitle().c_str(),"P");
      if( obj->GetSysErrorPoints() )
        stack_->Add( obj->GetSysErrorPoints(), "P+2" );
    }
  }
  canvas_->cd();
  if( is_log_x )
    gPad->SetLogx();
  if( is_log_y )
    gPad->SetLogy();
  stack_->Draw("APL");
  if( drawable_objects_.empty() ) {
    functions_.at(0)->Draw();
    if( x_range_.at(0) < x_range_.at(1) ) {
      functions_.at(0)->GetXaxis()->SetLimits(x_range_.at(0), x_range_.at(1));
      zero_line_->SetRange(x_range_.at(0), x_range_.at(1));
      functions_.at(0)->Draw();
    }
    if( y_range_.at(0) < y_range_.at(1) ) {
      functions_.at(0)->GetYaxis()->SetRangeUser(y_range_.at(0), y_range_.at(1));
      functions_.at(0)->Draw();
    }
    for( size_t i=1; i<functions_.size(); ++i ){
      functions_.at(i)->Draw("same");
    }
  } else {
    for (auto func : functions_) {
      if (func)
        func->Draw("same");
    }
  }
  if( x_range_.at(0) < x_range_.at(1) ) {
    stack_->GetXaxis()->SetLimits(x_range_.at(0), x_range_.at(1));
    zero_line_->SetRange(x_range_.at(0), x_range_.at(1));
    stack_->Draw();
  }
  if( y_range_.at(0) < y_range_.at(1) ) {
    stack_->GetYaxis()->SetRangeUser(y_range_.at(0), y_range_.at(1));
    stack_->Draw();
  }
  if( draw_zero_line )
    zero_line_->Draw("same");
  int i=0;
  for( auto text : texts_ ){
    text->SetNDC();
    text->SetTextSize(text_sizes_.at(i));
//    text->SetLineWidth(1);
//    text.SetLineColor(kBlack);
    text->Draw("same");
    ++i;
  }
  for(auto legend : legends_) {
    assert(legend);
    legend->Draw("same");
  }
}
void HeapPicture::SetAxisTitles(const std::vector<std::string> &axis_titles) {
  assert(axis_titles.size()==2);
  axis_titles_ = axis_titles;
  auto title = ";"+axis_titles.at(0)+";"+axis_titles.at(1);
  stack_->SetTitle( title.c_str() );
}

void HeapPicture::CustomizeXRange(const float part) {
  const float diff = xmax_ - xmin_;
  const float up = xmax_ + (1-part)/2*diff/part;
  const float down = xmin_ - (1-part)/2*diff/part;
  SetXRange({down, up});
}

void HeapPicture::CustomizeYRange(const float part) {
  const float diff = ymax_ - ymin_;
  const float up = ymax_ + (1-part)/2*diff/part;
  const float down = ymin_ - (1-part)/2*diff/part;
  SetYRange({down, up});
}

void HeapPicture::CustomizeLegend(TLegend* leg) {
  
  const float zero = 0.;
  const float one = 1.;
  const float middle = 0.5;
  
  const float leftspace = 0.04;
  const float rightspace = 0.02;
  const float topspace = 0.02;
  const float bottomspace = 0.04;
  
  const int Nspaces = 3;
  
  const float width = GetOptimalLegendSize(leg).first;
  const float height = GetOptimalLegendSize(leg).second;
  
  std::vector<std::vector<float>> places;
  
  for(int i=1; i<=Nspaces; i++)  // top right
    for(int j=1; j<=Nspaces; j++) {
      const float x_stop = one - i*rightspace;
      const float y_stop = one - j*topspace;
      const float x_start = x_stop - width;
      const float y_start = y_stop - height;
      places.push_back({x_start, y_start, x_stop, y_stop});
    }
  
//   for(int i=1; i<=Nspaces; i++)  // top left
//     for(int j=1; j<=Nspaces; j++) {
//       const float x_start = zero + i*leftspace;
//       const float y_stop = one - j*topspace;
//       const float x_stop = x_start + width;
//       const float y_start = y_stop - height;
//       places.push_back({x_start, y_start, x_stop, y_stop});
//     }
  
  for(int j=1; j<=Nspaces; j++) { // top
    const float x_stop = middle + width/2;
    const float y_stop = one - j*topspace;
    const float x_start = middle - width/2;
    const float y_start = y_stop - height;
    places.push_back({x_start, y_start, x_stop, y_stop});
  }
  
  for(int i=1; i<=Nspaces; i++) { // right
    const float x_stop = one - i*rightspace;
    const float y_stop = middle + height/2;
    const float x_start = x_stop - width;
    const float y_start = middle - height/2;
    places.push_back({x_start, y_start, x_stop, y_stop});
  }
  
  for(int i=1; i<=Nspaces; i++) { // left
    const float x_start = zero + i*leftspace;
    const float y_stop = middle + height/2;
    const float x_stop = x_start + width;
    const float y_start = middle - height/2;
    places.push_back({x_start, y_start, x_stop, y_stop});
  }
  
  for(int i=1; i<=Nspaces; i++)  // bottom right
    for(int j=1; j<=Nspaces; j++) {
      const float x_stop = one - i*rightspace;
      const float y_start = zero + j*bottomspace;
      const float x_start = x_stop - width;
      const float y_stop = y_start + height;
      places.push_back({x_start, y_start, x_stop, y_stop});
    }  
    
  for(int i=1; i<=Nspaces; i++)  // bottom left
    for(int j=1; j<=Nspaces; j++) {
      const float x_start = zero + i*leftspace;
      const float y_start = zero + j*bottomspace;
      const float x_stop = x_start + width;
      const float y_stop = y_start + height;
      places.push_back({x_start, y_start, x_stop, y_stop});
    }  
    
  for(int j=1; j<=Nspaces; j++) { // bottom
    const float x_start = middle - width/2;
    const float y_start = zero + j*bottomspace;
    const float x_stop = middle + width/2;
    const float y_stop = y_start + height;
    places.push_back({x_start, y_start, x_stop, y_stop});
  }    
  
  for(auto& pl : places) {
    bool is_good_place = true;
    std::vector<float> place_user = TransformToUser(canvas_, pl);
    for(auto& drob : drawable_objects_) {
      TGraph* gr = (TGraph*)drob->GetPoints();
      if(OverlapWithGraph(gr, place_user)) {
        is_good_place = false;
        break;
      }
    }
    if(is_good_place) {
      leg -> SetX1(place_user.at(kX1));
      leg -> SetY1(place_user.at(kY1));
      leg -> SetX2(place_user.at(kX2));
      leg -> SetY2(place_user.at(kY2));
      leg -> SetOption("br");
      leg -> SetBorderSize(0);
      return;
    }
  }
    std::vector<float> place_user = TransformToUser(canvas_, places.at(0));
    leg -> SetX1(place_user.at(kX1));
    leg -> SetY1(place_user.at(kY1));
    leg -> SetX2(place_user.at(kX2));
    leg -> SetY2(place_user.at(kY2));
    leg -> SetOption("br");
}

bool HeapPicture::OverlapRectangles(std::vector<float> rect1, std::vector<float> rect2) const {
  if(rect1.at(kX1) > rect2.at(kX2) || rect1.at(kX2) < rect2.at(kX1))
    return false;
  if(rect1.at(kY1) > rect2.at(kY2) || rect1.at(kY2) < rect2.at(kY1))
    return false;
  
  return true;
}

std::vector<float> HeapPicture::TransformToUser(TCanvas* canvas, std::vector<float> x) const {
  // Transforms from Pad coordinates to User coordinates.
  // This can probably be replaced by using the built-in conversion commands.

  const float xstart = x_range_.at(0);
  const float xlength = x_range_.at(1) - xstart;
  float xlow = xlength * x.at(kX1) + xstart;
  float xhigh = xlength * x.at(kX2) + xstart;
//   if (canvas->GetLogx()) {
//     xlow = std::pow(10, xlow);
//     xhigh = std::pow(10, xhigh);
//   }

  const float ystart = y_range_.at(0);
  const float ylength = y_range_.at(1) - ystart;
  float ylow = ylength * x.at(kY1) + ystart;
  float yhigh = ylength * x.at(kY2) + ystart;
//   if (canvas->GetLogy()) {
//     ylow = std::pow(10, ylow);
//     yhigh = std::pow(10, yhigh);
//   }
     
  return {xlow, ylow, xhigh, yhigh};
}

bool HeapPicture::OverlapWithGraph(TGraph* graph, std::vector<float> rect2) const {
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

std::pair<float, float> HeapPicture::GetOptimalLegendSize(TLegend* leg) const {
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

HeapPicture::HeapPicture(const std::string &name,
                         const std::array<int, 2> &resolution)
    : Picture(name, resolution) {}
HeapPicture::~HeapPicture() {}
HeapPicture::HeapPicture() {}
