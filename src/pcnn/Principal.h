#ifndef PRINCIPAL_H
#define PRINCIPAL_H

#include <opencv2/opencv.hpp>

#include "Legion.h"

using namespace std;
using namespace cv;

const int MAXVAL = 32768;
const int MAXIMO = 32767;
const int GRAY = 255;
const int Num = 1;
const int KS = 5;
const int LP = 0;
const int RP = 4;
const int LK = 1;
const int RK = 3;
const int M = 25600;
const int MaxKnl = 500;
const int MaxKnc = 700;

const int nL = 1501;
const int nC = 1501;


struct TRGBDouble {
    double r;
    double g;
    double b;
};


struct TMat {
    double v;
    unsigned char p, d;
    double v_ant;
    double rn;
    double i;
    unsigned char rgb [ 5 ], Seg_rgb [ 5 ];
    unsigned char rgbSColor [ 3 ];
    unsigned char ang [ 5 ];
    unsigned char rgb_Itti [ 3 ];
    double y;
    int l, c;
    bool pulsar;
    double lx [ 2 ];
    double lateral;
    double w [ 8 ];
    int stim, matrix;
    int ly, cycle, trigger;
    int segN;
};


typedef TMat TIeD;
typedef double TVetorAtributos [ 13 ];
typedef TVetorAtributos ArrayNeuronios [ nL ][ nC ];
typedef long double TW [ 151 ][ 151 ];
typedef double TCMap0 [4][MaxKnl][MaxKnc][9];
typedef double TCMapArray [ MaxKnl+1 ][ MaxKnc+1 ];
typedef TCMapArray TCMap [ 9 ];
typedef TCMapArray TCMapCS [ 6 ];

class Principal
{

public:

    void Saliencia( Mat image );

    /**
     *
     */
    Mat convertNeuronToMat( Mat imagem );

private:

    double WeightsConnections( int i, int j );

    void Select();

    double Sim_Cor( int vl, int vc, int hl, int hc );

    void Connections();

    void normalization( TCMapArray& d, int knl_t, int knc_t );

    void subtraction( TCMapArray& O, TCMapArray& d, int knl_t, int knc_t );

    void interpolation( TCMapArray& O, TCMapArray& d, int knl_t, int knc_t, double f );

    void subtractionColor( TCMapArray& r, TCMapArray& O, TCMapArray& d, int knl_t, int knc_t );

    //TW gaussian2d( int n, double sigma);

    void IttiModel();

    /**
     * Captura da imagem e inicialização do neurônio.
     * Thread disparada a cada 10 milisegundos
     * IttiModel vem depois do timer.
     */
    void convertMatToNeuron( Mat image );

};

#endif // PRINCIPAL_H
