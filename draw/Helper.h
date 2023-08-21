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
      if(std::isnan(ey)) ey = 0.;
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
      ey[iP] = gre->GetErrorY(iP);
    }

    grme->AddYError(Npoints, ey, ey);
  }
}

#endif //FLOW_DRAWING_TOOLS_HELPER_H
