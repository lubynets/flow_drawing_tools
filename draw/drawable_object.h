//
// Created by mikhail on 2/28/21.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_DRAWABLE_OBJECT_H_
#define FLOW_DRAWING_TOOLS_SRC_DRAWABLE_OBJECT_H_

#include "readable_object.h"
#include <TF1.h>
#include <TFile.h>
#include <TGraphErrors.h>
#include <TGraphMultiErrors.h>
#include <TObject.h>

#include <utility>

class DrawableObject : public ReadableObject {
public:
  DrawableObject() = default;
  DrawableObject(const DrawableObject& ) = default;
  DrawableObject(const std::string &file_name,
                 const std::vector<std::string> &objects,
                 std::string title)
      : ReadableObject(file_name, objects), title_(std::move(title)) {}
  ~DrawableObject() override = default;

  int GetColor() const { return color_; }
  int GetMarker() const { return marker_; }
  void Fit( TF1* function ){
    this->RefreshPoints();
    points_->Fit(function);
    fit_ = dynamic_cast<TF1*>( points_->GetListOfFunctions()->Last());
    if(fit_)
      fit_->SetLineColor(color_);
  }
  virtual void RefreshPoints() {}
  TGraphMultiErrors* GetPoints() {
    this->RefreshPoints();
    return points_;
  }
  std::vector<float> GetPointsValues() const;
  std::vector<float> GetPointsErrors() const;
  TF1 *GetFit() const { return fit_; }
  void SetStyle( int color, int marker ){ color_=color; marker_=marker; }
  bool IsLine(){ return marker_ < 0; }
  bool IsFillLine() const { return fill_line_alpha_ > 0; }
  void SavePoints(){ points_->Write(); }
  std::string GetTitle() { return title_;}
  void SetTitle(const std::string &title) { title_ = title; }
  const std::string &GetErrorOption() const { return error_option_; }
  void SetErrorOption(const std::string &error_option) {
    error_option_ = error_option;
  }
  void SetIsFillLine(float alpha=0.3) { fill_line_alpha_ = alpha; }

protected:
  void SetMarkerStyle();
  int color_{kBlack};
  int marker_{kFullCircle};
  float fill_line_alpha_{-1};
  TF1* fit_{};
  TGraphMultiErrors* points_{nullptr};
  std::string error_option_{"Z"};
  std::string title_;
  ClassDefOverride(DrawableObject, 1)
};

#endif // FLOW_DRAWING_TOOLS_SRC_DRAWABLE_OBJECT_H_
