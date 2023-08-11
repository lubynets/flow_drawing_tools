#ifndef FLOW_DRAWING_TOOLS_SRC_MULTI_PICTURE_H_
#define FLOW_DRAWING_TOOLS_SRC_MULTI_PICTURE_H_

#include "picture.h"

class MultiPicture : public TObject {

public:

  MultiPicture() = delete;
  MultiPicture(int nx, int ny);
  ~MultiPicture();

  void SetPicture(int i, int j, Picture* pic);
  std::vector<Picture*>& GetPictures() { return pictures_; }
  Picture* GetPicture(int i, int j) const;
  TCanvas* GetCanvas() const { return canvas_; }

  void SetXaxisRanges(int i, float lo, float hi);
  void SetYaxisRanges(int j, float lo, float hi);
  void SetIsEqualizeXaxisRanges(int value=true) { is_equalize_x_ranges_ = value; }
  void SetIsEqualizeYaxisRanges(int value=true) { is_equalize_y_ranges_ = value; }

  void SetAspectRatio(float x, float y) { canvas_x_ = x; canvas_y_ = y; }
  void SetLeftMargin(float value) { lMargin_ = value; }
  void SetRightMargin(float value) { rMargin_ = value; }
  void SetBottomMargin(float value) { bMargin_ = value; }
  void SetTopMargin(float value) { tMargin_ = value; }
  void SetVerticalSpacing(float value) { vSpacing_ = value; }
  void SetHorizontalSpacing(float value) { hSpacing_ = value; }

  void Draw();

protected:

  int nx_;
  int ny_;
  std::vector<Picture*> pictures_{};
  std::vector<bool> picture_is_inited_{};

  std::vector<TPad*> pads_{};

  bool is_equalize_x_ranges_{false};
  bool is_equalize_y_ranges_{false};

  std::vector<std::pair<float, float>> x_axis_ranges_{}; // pads axes ranges
  std::vector<std::pair<float, float>> y_axis_ranges_{};

  TCanvas* canvas_{nullptr};
  float canvas_x_{1000};
  float canvas_y_{1000};
  float lMargin_{0.05};
  float rMargin_{0.01};
  float bMargin_{0.05};
  float tMargin_{0.01};
  float vSpacing_{0};
  float hSpacing_{0};

  int TransformCoordinates(int i, int j) const;
  std::pair<int, int> TransformCoordinates(int k) const;
  void CreatePads();
  void EqualizeXaxisRanges(int i=-1);
  void EqualizeYaxisRanges(int j=-1);
  void CheckAxesRangesCorrectness();

  ClassDefOverride(MultiPicture, 1)
};


#endif // FLOW_DRAWING_TOOLS_SRC_MULTI_PICTURE_H_
