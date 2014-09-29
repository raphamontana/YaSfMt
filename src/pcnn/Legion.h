#ifndef LEGION_H
#define LEGION_H

#include <cmath>

/**
 * Funcao tangente hiperbolica
 */
double tnh( double x );

double bi( double x, double threshold );

/*
// Runge Kutta
double f( double x, double y, double input );
double G( double x, double y, double eta, double gamma, double beta );
void rungeKutta( double x1, double y1, double& x2, double& y2, double input, double h, double eta, double gamma, double beta );

// Funcao sigmoide
double sgm( double x );

double Ls( double x, double input );
*/

#endif //  LEGION_H
