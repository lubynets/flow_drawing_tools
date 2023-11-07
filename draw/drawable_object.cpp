//
// Created by mikhail on 2/28/21.
//

#include "drawable_object.h"
ClassImp(DrawableObject);

void DrawableObject::SetMarkerStyle() {
  points_->SetTitle(title_.c_str());
  if( marker_ > 0 )
    points_->SetMarkerStyle(marker_);
  else
    points_->SetLineStyle(-marker_);
  points_->SetMarkerColor(color_);
  points_->SetLineColor(color_);
  points_->SetFillColorAlpha(color_, fill_line_alpha_);
  if( fit_ )
    fit_->SetLineColor(color_);
}

std::vector<float> DrawableObject::GetPointsValues() const {
  std::vector<float> v;
  const int N = points_->GetN();
  for(int i=0; i<N; i++) {
    v.emplace_back(points_->GetPointY(i));
  }
  return v;
}

std::vector<float> DrawableObject::GetPointsErrors() const {
  std::vector<float> v;
  const int N = points_->GetN();
  for(int i=0; i<N; i++) {
    v.emplace_back(points_->GetErrorY(i));
  }
  return v;
}
