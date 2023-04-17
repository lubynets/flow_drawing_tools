#ifndef FLOW_DRAWING_TOOLS_DRAW_QNTOOLS_HELPER_H
#define FLOW_DRAWING_TOOLS_DRAW_QNTOOLS_HELPER_H

#include "DataContainer.hpp"

using namespace Qn;

inline TGraphErrors* ErrorsToTGraph(const DataContainer<StatCalculate, AxisD> &data, bool take_statistics_into_account=true) {
  if (data.GetAxes().size() > 1) {
    std::cout << "Data container has more than one dimension. " << std::endl;
    std::cout << "Cannot draw as Graph. Use Projection() to make it one dimensional." << std::endl;
    return nullptr;
  }
  auto graph = new TGraphErrors();
  unsigned int ibin = 0;
  for (const auto &bin : data) {
    if (bin.SumWeights() <= 0.) {
      ibin++;
      continue;
    }
    auto error = bin.StandardErrorOfMean();
    auto nentries = 1./bin.NEffectiveInverse();

    double y;
    if(take_statistics_into_account) {
      y = error*std::sqrt(nentries);
    } else {
      y = error;
    }
    auto xhi = data.GetAxes().front().GetUpperBinEdge(ibin);
    auto xlo = data.GetAxes().front().GetLowerBinEdge(ibin);
    auto xhalfwidth = (xhi - xlo)/2.;
    auto x = xlo + xhalfwidth;
    double ex = 0;
    double ey = 0;
    graph->SetPoint(graph->GetN(), x, y);
    graph->SetPointError(graph->GetN() - 1, ex, ey);
    graph->SetMarkerStyle(kFullCircle);
    ibin++;
  }
  return graph;
}

inline TGraphErrors* ErrorsToTGraph(const DataContainer<StatDiscriminator, AxisD> &data, bool take_statistics_into_account=true) {
  if (data.GetAxes().size() > 1) {
    std::cout << "Data container has more than one dimension. " << std::endl;
    std::cout << "Cannot draw as Graph. Use Projection() to make it one dimensional." << std::endl;
    return nullptr;
  }
  auto graph = new TGraphErrors();
  unsigned int ibin = 0;
  for (const auto &bin : data) {
    if (bin.SumWeights() <= 0.) {
      ibin++;
      continue;
    }
    auto error = bin.StandardErrorOfMean();
    auto nentries = bin.Neff();

    double y;
    if(take_statistics_into_account) {
      y = error*std::sqrt(nentries);
    } else {
      y = error;
    }
    auto xhi = data.GetAxes().front().GetUpperBinEdge(ibin);
    auto xlo = data.GetAxes().front().GetLowerBinEdge(ibin);
    auto xhalfwidth = (xhi - xlo)/2.;
    auto x = xlo + xhalfwidth;
    double ex = 0;
    double ey = 0;
    graph->SetPoint(graph->GetN(), x, y);
    graph->SetPointError(graph->GetN() - 1, ex, ey);
    graph->SetMarkerStyle(kFullCircle);
    ibin++;
  }
  return graph;
}

inline TGraphErrors* ErrorsToTGraph(const DataContainer<StatCollect, AxisD> &data, bool take_statistics_into_account=true) {
  if (data.GetAxes().size() > 1) {
    std::cout << "Data container has more than one dimension. " << std::endl;
    std::cout
        << "Cannot draw as Graph. Use Projection() to make it one dimensional."
        << std::endl;
    return nullptr;
  }
  auto graph = new TGraphErrors();
  unsigned int ibin = 0;
  for (const auto &bin : data) {
    const auto &stats = bin.GetStatistics();
    if (stats.SumWeights() <= 0) {
      ibin++;
      continue;
    }
    auto error = stats.StandardErrorOfMean();
    auto nentries = stats.Neff();

    double y;
    if(take_statistics_into_account) {
      y = error*std::sqrt(nentries);
    } else {
      y = error;
    }
    auto xhi = data.GetAxes().front().GetUpperBinEdge(ibin);
    auto xlo = data.GetAxes().front().GetLowerBinEdge(ibin);
    auto xhalfwidth = (xhi - xlo)/2.;
    auto x = xlo + xhalfwidth;
    double ex = 0;
    double ey = 0;
    graph->SetPoint(graph->GetN(), x, y);
    graph->SetPointError(graph->GetN() - 1, ex, ey);
    graph->SetMarkerStyle(kFullCircle);
    ibin++;
  }
  return graph;
}

#endif // FLOW_DRAWING_TOOLS_DRAW_QNTOOLS_HELPER_H
