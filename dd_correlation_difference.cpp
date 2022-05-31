#include "dd_correlation_difference.h"

void DDCorrelationDifference::Calculate() {
  const int n_of_graphs = minuend_.GetProjections().size();
  if(n_of_graphs != subtrahend_.GetProjections().size())
    std::cout << "# of minuend_ and subtrahend_ graphs do not correspond!\n";
    
  for(int igr=0; igr<n_of_graphs; igr++) {
    TGraphErrors* gr_minuend = minuend_.GetProjections().at(igr)->GetPoints();
    TGraphErrors* gr_subtrahend = subtrahend_.GetProjections().at(igr)->GetPoints();
    TGraphErrors* gr_diff = CalculateDifference(gr_minuend, gr_subtrahend);
    
    
  }
}

TGraphErrors* DDCorrelationDifference::CalculateDifference(TGraphErrors* minuend, TGraphErrors* subtrahend) {
  const int n_points = minuend->GetN();
  if(n_points != subtrahend->GetN())
    std::cout << "# of points in minuend_ and subtrahend_ graphs do not correspond!\n";
  
  TGraphErrors* diff = new TGraphErrors();
  
  for(int ip=0; ip<n_points; ip++) {
    const float x = minuend->GetPointX(ip);
    const float y_min = minuend->GetPointY(ip);
    const float y_sub = subtrahend->GetPointY(ip);
    
    const float y_diff = y_min - y_sub;
    diff->SetPoint(ip, x, y_diff);
  }
  
  return diff;
}