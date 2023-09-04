//
// Created by oleksii on 21.08.23.
//

#ifndef FLOW_DRAWING_TOOLS_HELPER_H
#define FLOW_DRAWING_TOOLS_HELPER_H

#include "TGraphMultiErrors.h"

namespace Helper {

  inline TGraphMultiErrors* TGraphErrorsToTGraphMultiErrors(const TGraphErrors* gre) {
    auto* grme = new TGraphMultiErrors(gre->GetN(), 1);
    for (int iP = 0; iP < gre->GetN(); iP++) {
      grme->SetPoint(iP, gre->GetPointX(iP), gre->GetPointY(iP));
      const Double_t ex = gre->GetErrorX(iP);
      Double_t ey = gre->GetErrorY(iP);
      if (std::isnan(ey)) ey = 0.;
      grme->SetPointEX(iP, ex, ex);
      grme->SetPointEY(iP, 0, ey, ey);
    }

    return grme;
  }

  inline void AddErrorsToTGraphMultiErrors(TGraphMultiErrors* grme, const TGraphErrors* gre) {
    if (grme->GetN() != gre->GetN()) {
      throw std::runtime_error("Helper::AddErrorsToTGraphMultiErrors() - graphs have non-equal number of points");
    }

    const int Npoints = gre->GetN();
    auto ey = new Double_t[Npoints];
    for (int iP = 0; iP < Npoints; iP++) {
      const Double_t ex = gre->GetErrorX(iP);
      grme->SetPointEX(iP, ex, ex);
      ey[iP] = gre->GetErrorY(iP);
    }

    grme->AddYError(Npoints, ey, ey);
  }

}

namespace Rectangle {

  enum corners : short {
      kX1 = 0,
      kY1,
      kX2,
      kY2,
      kNumberOfCorners
  };

  inline bool OverlapRectangles(std::array<float, 4> rect1, std::array<float, 4> rect2) {
    if(rect1.at(kX1) > rect2.at(kX2) || rect1.at(kX2) < rect2.at(kX1))
      return false;
    if(rect1.at(kY1) > rect2.at(kY2) || rect1.at(kY2) < rect2.at(kY1))
      return false;

    return true;
  }

}

#endif //FLOW_DRAWING_TOOLS_HELPER_H
