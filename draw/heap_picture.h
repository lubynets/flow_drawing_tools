//
// Created by mikhail on 3/6/21.
//

#ifndef FLOW_DRAWING_TOOLS_DRAW_HEAP_PICTURE_H_
#define FLOW_DRAWING_TOOLS_DRAW_HEAP_PICTURE_H_

#include "picture.h"

class HeapPicture : public Picture {
public:
  HeapPicture();
  HeapPicture(const std::string &name, const std::array<int, 2> &resolution);
  ~HeapPicture() override;
  void AddDrawable( DrawableObject* obj ){
    assert(obj);
    drawable_objects_.push_back(obj);
    auto limits = GetYLimits(obj->GetPoints());
    xmin_ = std::min(xmin_, (float)obj->GetPoints()->GetXaxis()->GetXmin());
    xmax_ = std::max(xmax_, (float)obj->GetPoints()->GetXaxis()->GetXmax());    
    ymin_ = std::min(ymin_, limits.first);
    ymax_ = std::max(ymax_, limits.second);
  }
  void AddDrawables( const std::vector<DrawableObject*>& objects ){
    for( auto obj : objects ){
      this->AddDrawable(obj);
    }
  }
  void SetAxisTitles(const std::vector<std::string> &axis_titles) override;
  void SetRelErrorThreshold(float val) { relative_error_threshold_ = val; }
  void Draw() override;
  void DrawPad(TVirtualPad* pad = nullptr) override;
  void ExeCustomizeLegend(TLegend* leg) override;
  void SetIsPullMarkersOnTop(bool is=true) { is_pull_markers_on_top_ = is; }

protected:
  bool IsGoodPlaceForLegend(std::array<float, 4> place) const;
  void ApplyPlaceForLegend(TLegend* leg, std::array<float, 4> place);
  void GetRidOfNaNs();

  std::vector<DrawableObject*> drawable_objects_;
  float relative_error_threshold_{-1.};
  bool is_pull_markers_on_top_{false};

  std::pair<float, float> GetYLimits(TGraphMultiErrors* gr) const;
  void FillStackWithDrawableObjects();

  ClassDefOverride(HeapPicture, 1)
};

#endif // FLOW_DRAWING_TOOLS_DRAW_HEAP_PICTURE_H_
