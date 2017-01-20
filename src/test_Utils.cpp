

#include <iostream>
#include <ctime>
#include "Utils.h"
#include "pnl/pnl_mathtools.h"

int main()
{
   PnlMat* M;
   M =  pnl_mat_create_from_list(3,3,2.0,4.0,6.0,4.0,0.0,10.0,6.0,10.0,12.0);
   std::cout << "Matrice M symétrique non positive :"<< std::endl;
   pnl_mat_print(M);
   std::cout << std::endl;
   PnlMat* A = pnl_mat_create(3,3) ;
   A = Utils::higham(M);
   std::cout << "Matrice symétrique positive la plus proche de M"<< std::endl;
   pnl_mat_print(A);
   pnl_mat_free(&A);
   pnl_mat_free(&M);
   exit(0);
}
