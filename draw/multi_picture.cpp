#include "multi_picture.h"

MultiPicture::MultiPicture(int nx, int ny) {
  nx_ = nx;
  ny_ = ny;
  pictures_.resize(nx_*ny_);
}

MultiPicture::~MultiPicture() {
  for(auto& pic : pictures_) {
    if(pic != nullptr) delete pic;
  }
}

void MultiPicture::SetPicture(int i, int j, Picture* pic) {
  const int k = TransformCoordinates(i, j);
  pictures_.at(k) = pic;
}

Picture* MultiPicture::GetPicture(int i, int j) const {
  const int k = TransformCoordinates(i, j);
  return pictures_.at(k);
}

int MultiPicture::TransformCoordinates(int i, int j) const {
  if(i>=nx_ || j>=ny_ || i<0 || j<0) {
    std::cout << "Error: MultiPicture::TransformCoordinates() - i>=nx_ || j>=ny_ || i<0 || j<0\n";
    std::cout << "i = " << i << ", j = " << j << ", nx_ = " << nx_ << ", ny_ = " << ny_ << "\n";
    throw;
  }
  const int k = nx_*j + i;
  return k;
}

std::pair<int, int> MultiPicture::TransformCoordinates(int k) const {
  if(k >= nx_*ny_ || k<0) {
    std::cout << "Error: MultiPicture::TransformCoordinates() - k >= nx_*ny_ || k<0\n";
    std::cout << "k = " << k << ", nx_ = " << nx_ << ", ny_ = " << ny_ << "\n";
  }
  const int i = k/nx_;
  const int j = k%nx_;

  return std::make_pair(i, j);
}
