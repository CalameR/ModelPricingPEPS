
#include <iostream>
#include <cmath>
#include "Utils.h"
#include <math.h>       /* pow */
#include "pnl/pnl_mathtools.h"
#include <algorithm>

PnlMat* Utils::higham(const PnlMat* M )
{
   int size = M->m;
   // Calcul des valeurs propres de M dans v et des vecteurs propres dans P
   PnlVect* v = pnl_vect_create(size);
   PnlMat* P = pnl_mat_create(size,size);
   pnl_mat_eigen(v, P, M, 1);

   // Calcul de la matrice diagonale modifiée 
   PnlMat* D = pnl_mat_create_from_zero(size,size);
   for(int i = 0 ; i < size ; i++){
   MLET(D,i,i) = std::max(GET(v,i),0.0); 
   }
   // On recompose P*D*P'
   PnlMat* A = pnl_mat_create(size,size);
   A = pnl_mat_mult_mat(P,D);
   A = pnl_mat_mult_mat(A,pnl_mat_transpose(P));
   return A;
}


