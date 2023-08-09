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

protected:

  std::vector<Picture*> pictures_{};
  int nx_;
  int ny_;

  int TransformCoordinates(int i, int j) const;
  std::pair<int, int> TransformCoordinates(int k) const;

};


#endif // FLOW_DRAWING_TOOLS_SRC_MULTI_PICTURE_H_
