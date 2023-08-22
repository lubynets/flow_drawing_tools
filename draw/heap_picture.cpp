//
// Created by mikhail on 3/6/21.
//

#include "heap_picture.h"
#include "TH1.h"
ClassImp(HeapPicture)

void HeapPicture::Draw() {
  DrawPad();
}

void HeapPicture::DrawPad(TVirtualPad* pad) {
  if(pad == nullptr){
    pad = canvas_->GetPad(0);
  }
  pad->cd();
  if(auto_legend_){
    assert(legends_.empty());
    legends_.emplace_back( new TLegend() );
  }
  FillStackWithDrawableObjects();
  if( is_log_x )
    gPad->SetLogx();
  if( is_log_y )
    gPad->SetLogy();
  stack_->Draw("APL");
  if( drawable_objects_.empty() ) {
    functions_.at(0)->Draw();
    if( x_range_.at(0) < x_range_.at(1) ) {
      functions_.at(0)->GetXaxis()->SetLimits(x_range_.at(0), x_range_.at(1));
      for(auto& hl : horizontal_lines_) {
        hl->SetRange(x_range_.at(0), x_range_.at(1));
      }
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
    for(auto& hl : horizontal_lines_) {
      hl->SetRange(x_range_.at(0), x_range_.at(1));
    }
  }
  if( y_range_.at(0) < y_range_.at(1) ) {
    stack_->GetYaxis()->SetRangeUser(y_range_.at(0), y_range_.at(1));
  }
  if( draw_zero_line )
    horizontal_lines_.at(0)->Draw("same");
  for(int i=1; i<horizontal_lines_.size(); i++) {
    horizontal_lines_.at(i)->Draw("same");
  }
  int i=0;
  for( auto text : texts_ ){
    text->SetNDC();
    text->SetTextSize(text_sizes_.at(i));
    text->SetTextFont(text_fonts_.at(i));
    if(text_intramargin_xy_.at(i).first != -1) {
      const float xNDC = pad->GetLeftMargin() + text_intramargin_xy_.at(i).first * (1. - pad->GetRightMargin() - pad->GetLeftMargin());
      const float yNDC = pad->GetBottomMargin() + text_intramargin_xy_.at(i).second * (1. - pad->GetTopMargin() - pad->GetBottomMargin());
      texts_.at(i)->SetX(xNDC);
      texts_.at(i)->SetY(yNDC);
    }
    text->Draw("same");
    ++i;
  }
  for(auto legend : legends_) {
    assert(legend);
    CustomizeLegend(legend);
    legend->Draw("same");
  }
}
void HeapPicture::SetAxisTitles(const std::vector<std::string> &axis_titles) {
  assert(axis_titles.size()==2);
  axis_titles_ = axis_titles;
  auto title = ";"+axis_titles.at(0)+";"+axis_titles.at(1);
  stack_->SetTitle( title.c_str() );
}

void HeapPicture::FillStackWithDrawableObjects() {
  // clear stack_ if it was by chance already filled
  if(stack_ != nullptr && stack_->GetListOfGraphs() != nullptr) {
    for(int iGr=stack_->GetListOfGraphs()->GetEntries()-1; iGr>=0; iGr--) {
      stack_->RecursiveRemove(stack_->GetListOfGraphs()->At(iGr));
    }
  }

  for( auto obj : drawable_objects_ ){
    if( obj->IsLine() ) {
      std::string opt;
      if(obj->IsFillLine()) {
        opt = "E3" + obj->GetErrorOption();
        if(obj->GetPoints()->GetLineStyle() == 2) obj->GetPoints()->SetFillStyle(3244);
      } else {
        opt = "L+X+" + obj->GetErrorOption();
      }
      stack_->Add(obj->GetPoints(), opt.c_str());
      if( auto_legend_ )
        legends_.back()->AddEntry(obj->GetPoints(), obj->GetTitle().c_str(),"L");
    } else {
      std::string opt{"P+" + obj->GetErrorOption() + "; 2"};
      stack_->Add(obj->GetPoints(), opt.c_str());
      if( auto_legend_ )
        legends_.back()->AddEntry(obj->GetPoints(), obj->GetTitle().c_str(),"P");
    }
  }
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
    std::vector<float> place_user = TransformToUser(pl);
    for(auto& drob : drawable_objects_) {
      TGraphMultiErrors* gr = (TGraphMultiErrors*)drob->GetPoints();
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
    std::vector<float> place_user = TransformToUser(places.at(0));
    leg -> SetX1(place_user.at(kX1));
    leg -> SetY1(place_user.at(kY1));
    leg -> SetX2(place_user.at(kX2));
    leg -> SetY2(place_user.at(kY2));
    leg -> SetOption("br");
}

std::pair<float, float> HeapPicture::GetYLimits(TGraphMultiErrors* gr) const {
  float lo =  std::numeric_limits<float>::max();
  float hi = -std::numeric_limits<float>::max();

  for(int i = 0; i<gr->GetN(); i++) {
    const float y = gr->GetPointY(i);
    const float ey = gr->GetErrorY(i, 0);
    const float err = std::abs(ey/y);
    if(err < relative_error_threshold_ || relative_error_threshold_ < 0.) {
      lo = std::min(lo, y-ey);
      hi = std::max(hi, y+ey);
    }
  }

  return std::make_pair(lo, hi);
}

HeapPicture::HeapPicture(const std::string &name,
                         const std::array<int, 2> &resolution)
    : Picture(name, resolution) {}
HeapPicture::~HeapPicture() {}
HeapPicture::HeapPicture() {}
