#include "multi_picture.h"

MultiPicture::MultiPicture(int nx, int ny) {
  nx_ = nx;
  ny_ = ny;
  pictures_.resize(nx_*ny_);
  picture_is_inited_.resize(nx_*ny_);
  for(int k=0; k<nx_*ny_; k++) {
    picture_is_inited_.at(k) = false;
  }
  pads_.resize(nx_*ny_);
  x_axis_ranges_.resize(nx_);
  y_axis_ranges_.resize(ny_);
  for(auto& xar : x_axis_ranges_) {
    xar = std::make_pair(std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
  }
  for(auto& yar : y_axis_ranges_) {
    yar = std::make_pair(std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
  }
}

MultiPicture::~MultiPicture() {
  for(auto& pic : pictures_) {
    if(pic != nullptr) delete pic;
  }
}

void MultiPicture::Draw() {
  canvas_ = new TCanvas("cc", "", canvas_x_, canvas_y_);
  CreatePads();
  if(is_equalize_x_ranges_ == true) EqualizeXaxisRanges();
  if(is_equalize_y_ranges_ == true) EqualizeYaxisRanges();
  CheckAxesRangesCorrectness();

  for(int ix=0; ix<nx_; ix++) {
    for(int jy=0; jy<ny_; jy++) {
      const int kk = TransformCoordinates(ix, jy);
      if(picture_is_inited_.at(kk) == false) continue;
      canvas_->cd();
      pads_.at(kk)->Draw();
      pads_.at(kk)->SetFillStyle(4000);
      pads_.at(kk)->SetFrameFillStyle(4000);
      pads_.at(kk)->cd();
      pictures_.at(kk)->DrawPad(pads_.at(kk));
    }
  }
}

void MultiPicture::SetPicture(int i, int j, Picture* pic) {
  const int k = TransformCoordinates(i, j);
  pictures_.at(k) = pic;
  picture_is_inited_.at(k) = true;
}

Picture* MultiPicture::GetPicture(int i, int j) const {
  const int k = TransformCoordinates(i, j);
  return pictures_.at(k);
}

void MultiPicture::SetXaxisRanges(int i, float lo, float hi) {
  if(i<0 || i>=nx_) {
    std::cout << "Error: MultiPicture::SetXaxisRanges() - i<0 || i>=nx_\n";
    std::cout << "i = " << i << ", nx_ = " << nx_ << "\n";
    throw;
  }
  x_axis_ranges_.at(i) = std::make_pair(lo, hi);
}

void MultiPicture::SetYaxisRanges(int j, float lo, float hi) {
  if(j<0 || j>=ny_) {
    std::cout << "Error: MultiPicture::SetYaxisRanges() - j<0 || j>=ny_\n";
    std::cout << "j = " << j << ", ny_ = " << ny_ << "\n";
    throw;
  }
  y_axis_ranges_.at(j) = std::make_pair(lo, hi);
}

void MultiPicture::EqualizeXaxisRanges(int i) {
 // i - number of column where automatize
 // i==-1 means to automatize in each column
  if(i<0) {
    for(int ii=0; ii<nx_; ii++) {
      EqualizeXaxisRanges(ii);
    }
  }
  for(int j=0; j<ny_; j++) {
    const int k = TransformCoordinates(i, j);
    if(picture_is_inited_.at(k) == false) continue;
    auto picranges = pictures_.at(k)->GetXRange();
    x_axis_ranges_.at(i).first = std::min(x_axis_ranges_.at(i).first, picranges.first);
    x_axis_ranges_.at(i).second = std::max(x_axis_ranges_.at(i).second, picranges.second);
  }
}

void MultiPicture::EqualizeYaxisRanges(int j) {
 // j - number of raw where automatize
 // j==-1 means to automatize in each raw
  if(j<0) {
    for(int jj=0; jj<nx_; jj++) {
      EqualizeXaxisRanges(jj);
    }
  }
  for(int i=0; i<nx_; i++) {
    const int k = TransformCoordinates(i, j);
    if(picture_is_inited_.at(k) == false) continue;
    auto picranges = pictures_.at(k)->GetYRange();
    y_axis_ranges_.at(j).first = std::min(y_axis_ranges_.at(j).first, picranges.first);
    y_axis_ranges_.at(j).second = std::max(y_axis_ranges_.at(j).second, picranges.second);
  }
}

void MultiPicture::CheckAxesRangesCorrectness() {
  for(auto& xar : x_axis_ranges_) {
    if(xar.first >= xar.second) {
      throw std::runtime_error("MultiPicture::CheckAxesRangesCorrectness() failed - lower range exceeds upper one");
    }
  }
  for(auto& yar : y_axis_ranges_) {
    if(yar.first >= yar.second) {
      throw std::runtime_error("MultiPicture::CheckAxesRangesCorrectness() failed - lower range exceeds upper one");
    }
  }
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
    throw;
  }
  const int i = k/nx_;
  const int j = k%nx_;

  return std::make_pair(i, j);
}

void MultiPicture::CreatePads() {
  // borrowed from Root tutorials/graphics/canvas2.C
  float vStep  = (1.- bMargin_ - tMargin_ - (ny_-1) * vSpacing_) / ny_;
  float hStep  = (1.- lMargin_ - rMargin_ - (nx_-1) * hSpacing_) / nx_;
  float vposd,vposu,vmard,vmaru,vfactor;
  float hposl,hposr,hmarl,hmarr,hfactor;
  for (int i=0;i<nx_;i++) {
    if (i==0) {
        hposl = 0.0;
        hposr = lMargin_ + hStep;
        hfactor = hposr-hposl;
        hmarl = lMargin_ / hfactor;
        hmarr = 0.0;
    } else if (i == nx_-1) {
        hposl = hposr + hSpacing_;
        hposr = hposl + hStep + rMargin_;
        hfactor = hposr-hposl;
        hmarl = 0.0;
        hmarr = rMargin_ / hfactor;
    } else {
        hposl = hposr + hSpacing_;
        hposr = hposl + hStep;
        hmarl = 0.0;
        hmarr = 0.0;
    }
    for (int j=0;j<ny_;j++) {
      const int k = TransformCoordinates(i, j);
      if (j==0) {
        vposd = 0.0;
        vposu = bMargin_ + vStep;
        vfactor = vposu-vposd;
        vmard = bMargin_ / vfactor;
        vmaru = 0.0;
      } else if (j == ny_-1) {
        vposd = vposu + vSpacing_;
        vposu = vposd + vStep + tMargin_;
        vfactor = vposu-vposd;
        vmard = 0.0;
        vmaru = tMargin_ / vfactor;
      } else {
        vposd = vposu + vSpacing_;
        vposu = vposd + vStep;
        vmard = 0.0;
        vmaru = 0.0;
      }
      char name[16];
      sprintf(name,"pad_%i_%i",i,j);
      pads_.at(k) = new TPad(name,"",hposl,vposd,hposr,vposu);
      pads_.at(k)->SetLeftMargin(hmarl);
      pads_.at(k)->SetRightMargin(hmarr);
      pads_.at(k)->SetBottomMargin(vmard);
      pads_.at(k)->SetTopMargin(vmaru);
      pads_.at(k)->SetFrameBorderMode(0);
      pads_.at(k)->SetBorderMode(0);
      pads_.at(k)->SetBorderSize(0);
      pads_.at(k)->Draw();
    }
  }
}
