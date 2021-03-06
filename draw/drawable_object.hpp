//
// Created by mikhail on 2/28/21.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_DRAWABLE_OBJECT_H_
#define FLOW_DRAWING_TOOLS_SRC_DRAWABLE_OBJECT_H_

#include <TGraphErrors.h>
#include <TF1.h>
#include <TFile.h>
#include <TObject.h>

#include <utility>

class DrawableObject : public TObject {
public:
  DrawableObject() = default;
  DrawableObject(std::string file_name,
                 std::vector<std::string> objects,
                 std::string title)
      : file_name_(std::move(file_name)), objects_(std::move(objects)), title_(std::move(title)) {}
  virtual ~DrawableObject() = default;
  void Fit( TF1* function ){
    if(!points_)
      this->RefreshPoints();
    points_->Fit(function);
    fit_ = dynamic_cast<TF1*>( points_->GetListOfFunctions()->At(0));
    fit_->SetLineColor(color_);
  }
  virtual void RefreshPoints() {}
  TGraphErrors* GetPoints() {
    if(!points_)
      this->RefreshPoints();
    return points_;
  }
  TF1 *GetFit() const { return fit_; }
  void SetStyle( int color, int marker ){ color_=color; marker_=marker; }
  bool IsLine(){ return marker_ < 0; }
  void SavePoints(){ points_->Write(); }
  std::string GetTitle() { return title_;}

protected:
  template <typename T>
  T* ReadObjectFromFile(const std::string& obj_name){
    if( !file_ )
      file_ = TFile::Open(file_name_.c_str() );
    if (!file_)
      throw std::runtime_error( "No such file "+ file_name_);
    T* obj;
    file_->GetObject( obj_name.c_str(), obj );
    if( !obj )
      throw std::runtime_error( "No such object "+obj_name+" in file "+
                               file_name_);
    return obj;
  }
  void SetMarkerStyle();
  std::string file_name_;
  std::vector<std::string> objects_;
  std::string title_;
  int color_{kBlack};
  int marker_{kFullCircle};
  TF1* fit_{};
  TFile* file_{};
  TGraphErrors* points_{nullptr};
  ClassDef(DrawableObject, 1)
};

#endif // FLOW_DRAWING_TOOLS_SRC_DRAWABLE_OBJECT_H_