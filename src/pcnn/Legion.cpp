#include "Legion.h"

/**
 * Basic Oscillator Model
 * @param I denotes the external stimulation to the oscillator.
 * @param parameter ε is positive and it is chosen so that 0<ε<<1
 */
double f( double x, double y, double input )
{
    return( 3 * x - x * x * x + 2 - y + input );
}

/**
 * Basic Oscillator Model
 * @param parameter ε is positive and it is chosen so that 0<ε<<1
 */
double G( double x, double y, double eta, double gamma, double beta )
{
    return( eta * ( gamma * ( 1 + tnh( x / beta ) ) - y ) );
}

double sgm( double x )
{
    return( 1.0 / ( 1.0 + exp( -x ) ) );
}

double tnh( double x )
{
    return( ( exp( x ) - exp( - x ) ) / ( exp( x ) + exp( - x ) ) );
}

void runge_kutta123123( double x1, double y1, double& x2, double& y2, double input, double h, double eta, double gamma, double beta )
{
    double kx1, kx2, kx3, kx4;
    double ky1, ky2, ky3, ky4;
    kx1 = h * f( x1, y1, input );
    ky1 = h * G( x1, y1, eta, gamma, beta );
    kx2 = h * f( x1 + kx1 / 2.0, y1 + ky1 / 2.0, input );
    ky2 = h * G( x1 + kx1 / 2.0, y1 + ky1 / 2.0, eta, gamma, beta );
    kx3 = h * f( x1 + kx2 / 2.0, y1 + ky2 / 2.0, input );
    ky3 = h * G( x1 + kx2 / 2.0, y1 + ky2 / 2.0, eta, gamma, beta );
    kx4 = h * f( x1 + kx3, y1 + ky3, input );
    ky4 = h * G( x1 + kx3, y1 + ky3, eta, gamma, beta );
    x2 = x1 + ( kx1 + 2.0 * kx2 + 2.0 * kx3 + kx4 ) / 6.0;
    y2 = y1 + ( ky1 + 2.0 * ky2 + 2.0 * ky3 + ky4 ) / 6.0;
}

double bi( double x, double threshold )
{
    if ( x > threshold ) {
        return( 1 );
    }
    else {
        return( 0 );
    }
}

double Ls( double x, double input )
{
    return( 3.0 * ( input - x ) );
}
