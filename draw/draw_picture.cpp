//
// Created by mikhail on 2/28/21.
//

#include <TROOT.h>
#include <stdexcept>
#include <iostream>
int main(int n_args, char** args){
  if( n_args < 2 )
    throw std::runtime_error( "No argumets provided" );
//  auto interpreter = gROOT->GetInterpreter();

#ifdef DiscriminatorMode
  std::cout << "\nINFO: FlowDrawingTools operates in Discriminator mode\n\n";
#else
  std::cout << "\nINFO: FlowDrawingTools operates in non-Discriminator (usual) mode\n\n";
#endif

  gROOT->Macro(args[1]);
  return 0;
}
