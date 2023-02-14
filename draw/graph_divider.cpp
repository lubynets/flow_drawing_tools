#include "graph_divider.h"

void GraphDivider::Calculate() {
  result_ = new Graph();
  result_ -> SetStyle(numerator_->GetColor(), numerator_->GetMarker());

  TGraphErrors* gr_num = numerator_->GetPoints();
  TGraphErrors* gr_den = denominator_->GetPoints();
  TGraphErrors* gr_res = new TGraphErrors();

  for(int ip=0; ip<gr_num->GetN(); ip++) {

    const float x = gr_num->GetPointX(ip);
    const float y_num = gr_num->GetPointY(ip);
    const float y_den = gr_den->GetPointY(ip);

    float err_num, err_den;
    if(is_ignore_numerator_errors_) {
      err_num = 0;
    } else {
      err_num = gr_num->GetErrorY(ip);
    }

    if(is_ignore_denominator_errors_) {
      err_den = 0;
    } else {
      err_den = gr_den->GetErrorY(ip);
    }

    std::pair ratio = CalculateRatio(y_den, err_den, y_num, err_num);

    gr_res->SetPoint(ip, x, ratio.first);
    gr_res->SetPointError(ip, 0, ratio.second);
  }

  result_->SetPoints(gr_res);
}

Graph* GraphDivider::GetResult() const {
  if(result_ == nullptr) {
    throw std::runtime_error("GraphDivider::GetResult() - result_ is nullptr");
  }
  return result_;
}

std::vector<float> GraphDivider::GetPointsValues() const {
  if(result_ == nullptr) {
    throw std::runtime_error("GraphDivider::GetPointsValues() - result_ is nullptr");
  }
  return result_->GetPointsValues();
}

std::vector<float> GraphDivider::GetPointsErrors() const {
  if(result_ == nullptr) {
    throw std::runtime_error("GraphDivider::GetPointsErrors() - result_ is nullptr");
  }
  return result_->GetPointsErrors();
}

std::pair<float, float> GraphDivider::CalculateRatio(float vmc, float sigma_mc, float vr, float sigma_r) const {
  const float vmc2 = vmc*vmc;
  const float vr2 = vr*vr;
  const float sigma_mc2 = sigma_mc*sigma_mc;
  const float sigma_r2 = sigma_r*sigma_r;

  const float rat = vr / vmc;
  float sigma_rat{0.f};
  const float sigma_rat2 = (vmc2*sigma_r2 + vr2*sigma_mc2 - 2*vmc*vr*sigma_mc2)/vmc2/vmc2;
  if(sigma_rat2 < 0) {
    std::cout << "GraphDivider::CalculateRatio() - sigma_rat2 < 0\n";
  } else {
    sigma_rat = std::sqrt(sigma_rat2);
  }

  return std::make_pair(rat, sigma_rat);
}
