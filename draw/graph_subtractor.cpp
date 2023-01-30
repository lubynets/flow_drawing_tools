#include "graph_subtractor.h"

void GraphSubtractor::Calculate() {
  result_ = new Graph();
  result_ -> SetStyle(minuend_->GetColor(), minuend_->GetMarker());
  
  TGraphErrors* gr_min = minuend_->GetPoints();
  TGraphErrors* gr_sub = subtrahend_->GetPoints();
  TGraphErrors* gr_res = new TGraphErrors();
  
  for(int ip=0; ip<gr_min->GetN(); ip++) {
    
    const float x = gr_min->GetPointX(ip);
    const float y_min = gr_min->GetPointY(ip);
    const float y_sub = gr_sub->GetPointY(ip);
    const float diff = y_min - y_sub;
    
    float err_min, err_sub;
    if(is_ignore_minuend_errors_)
      err_min = 0;
    else
      err_min = gr_min->GetErrorY(ip);
    
    if(is_ignore_subtrahend_errors_)
      err_sub = 0;
    else
      err_sub = gr_sub->GetErrorY(ip);

    const float err_diff = std::sqrt(err_min*err_min - err_sub*err_sub);
    if(err_min < err_sub)
      std::cout << "problems...\n";
    
    if(!is_divide_over_error_) {
      gr_res->SetPoint(ip, x, diff);
      gr_res->SetPointError(ip, 0, err_diff);
    }
    else
      gr_res->SetPoint(ip, x, diff/err_diff); 
  }
  
  result_->SetPoints(gr_res);
}

std::vector<float> GraphSubtractor::GetPointsValues() const {
  if(result_ == nullptr) {
    throw std::runtime_error("GraphSubtractor::GetPointsValues() - result_ is nullptr");
  }
  return result_->GetPointsValues();
}

std::vector<float> GraphSubtractor::GetPointsErrors() const {
  if(result_ == nullptr) {
    throw std::runtime_error("GraphSubtractor::GetPointsErrors() - result_ is nullptr");
  }
  return result_->GetPointsErrors();
}
