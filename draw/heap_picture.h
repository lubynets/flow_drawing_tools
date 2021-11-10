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
    xmin_ = std::min(xmin_, (float)obj->GetPoints()->GetXaxis()->GetXmin());
    xmax_ = std::max(xmax_, (float)obj->GetPoints()->GetXaxis()->GetXmax());    
    ymin_ = std::min(ymin_, (float)obj->GetPoints()->GetYaxis()->GetXmin());
    ymax_ = std::max(ymax_, (float)obj->GetPoints()->GetYaxis()->GetXmax());    
  }
  void AddDrawables( const std::vector<DrawableObject*>& objects ){
    for( auto obj : objects ){
      this->AddDrawable(obj);
    }
  }
  void SetAxisTitles(const std::vector<std::string> &axis_titles) override;
  void Draw() override;
  TCanvas* GetCanvas() { return canvas_; }
  void CustomizeXRange(const float part=1.);
  void CustomizeYRange(const float part=1.);

protected:
  std::vector<DrawableObject*> drawable_objects_;
  float xmin_ =  999.;
  float xmax_ = -999.;
  float ymin_ =  999.;
  float ymax_ = -999.;
  ClassDefOverride(HeapPicture, 1)
};

#endif // FLOW_DRAWING_TOOLS_DRAW_HEAP_PICTURE_H_
