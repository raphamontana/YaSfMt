#include "principal.h"
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "UnitLegion.h"
#include <time.h>
#include <unistd.h>

using namespace std;
using namespace cv;


//Itti
TW w;
TCMap0 IM;
TRGBDouble Cor;
TCMapCS MCSi, MCSrg, MCSby, MCS0, MCS1, MCS2, MCS3, MCSrg_t, MCSby_t;
TCMapArray Mrg, Mgr, Myb, Mby, Cm_i, Cm_c, Cm_o;
TCMap Mi, Mr, Mg, Mb, My, M0, M1, M2, M3;


TSEG Segmentos[100];
ArrayNeuronios KNeuronios;

int tmp = 0;
int cont_it = 0;
int t_geral =0;
int nit = 0;
float fat_dec = 0.0;
int a = 10;
int b = 10;
float *p;
int maxl = 0;
int maxc = 0;
int max_seg = 0;
double saliency = 0.0, saliency_max = 0.0;
bool treina_som = false;
int num[10];
double desv_P = 0.0;
int z = 0;
double limiarF = 0.0;
double Wz = 0.0;
int jump[ 2 ];
double Wc = 1; //W Cor
double Wi = 0; //W Intencidade
double Wo = 0; //W Orientação
double WSum = 1.0;
bool first_pulse = false;
int nl = 0, nc = 0;
unsigned char c_r = '\0', c_g = '\0', c_b = '\0';
int imin = 0, imax = 0, jmin = 0, jmax = 0;
bool cbWInt = false; //Fazendo a busca por : intensidade
bool cbWCor = true; //Cor
bool cbWOri = false; //Orientação
int seNb = 3; //Valor empírico
double seDPb = 1.0; //Valor empírico



double WeightsConnections( int i, int j )
{
  double result = 0.0;
  double Res = 0.0;
  int n;

  n = 0;

  if ( cbWInt )
    n++;
  if ( cbWCor )
    n++;
  if ( cbWOri )
    n++;

  if ( cbWInt )
    Res = Res + ( double( Neuronios[i][j].rgb_Itti[0] ) / 255 * Wi );

  if ( cbWCor ){
      Res = Res + ( (double(Neuronios[i][j].rgb_Itti[1])/255) * Wc );
  }

  if ( cbWOri )
    Res = Res + ( double( Neuronios[i][j].rgb_Itti[2] ) / 255 * Wo );

  result = Res / n;

  return result;

}

void Select(int knL, int knC ){
  int i = 0, j = 0, max = 0, imax = 0, jmax = 0;
  max = - 1;
  for ( i = 0; i < knC; i++)
    for ( j = 0; j < knL; j++){
      Neuronios[i][j].trigger = 0;

      if ( ( Neuronios[i][ j].lateral > thetap ) && ( Neuronios[i][ j].ly != MAXIMO ) && ( Neuronios[i][ j].ly > max ) )
        if ( WeightsConnections( i, j ) >= WSum )
        {
          max = Neuronios[i][ j].ly;
          imax = i;
          jmax = j;
        }
      }
  if ( ( imax == 0 ) && ( jmax == 0 ) && ( WSum > 0 ) )
  {
    for ( i = 0; i < knC; i++)
      for ( j = 0; j < knL; j++)
      {
        Neuronios[i][ j].d = 1;
        Neuronios[i][ j].lx[1] = LP;
        Neuronios[i][ j].ly = - 1;
        Neuronios[i][ j].cycle = 1;
        z = 1;
      }
  }

  if ( Neuronios[imax][jmax].cycle > 0 ){
    for ( i = 0; i <= knC; i++)
      for ( j = 0; j <= knL; j++)
        if ( ( Neuronios[i][ j].ly == max ) && ( Neuronios[i][ j].cycle > 0 ) )
          Neuronios[i][ j].trigger = 1;
  }
  else{
    Neuronios[imax][jmax].trigger = 1;
  }
  for ( i = 0; i < knC; i++)
    for ( j = 0; j < knL; j++)
      if ( ( Neuronios[i][ j].ly != MAXIMO ) && ( ( Neuronios[i][ j].cycle > 0 ) || ( Neuronios[i][ j].lateral > thetap ) ) )
      {
        Neuronios[i][ j].ly = Neuronios[i][ j].ly + ( MAXIMO - max );
        Neuronios[i][ j].lx[0] = double( Neuronios[i][ j].ly ) / ( MAXVAL + 1.0 );
      }
  c_r = Neuronios[imax][ jmax].rgbSColor[0];
  c_g = Neuronios[imax][ jmax].rgbSColor[1];
  c_b = Neuronios[imax][ jmax].rgbSColor[2];
}

double Sim_Cor( int vl, int vc, int hl, int hc )
{
  double result = 0.0;
  double r = 0.0;
  int i = 0;
  r = 0;
  for ( i = 0; i <= 2; i++)
    r = r + fabs( Neuronios[vl][ vc].rgbSColor[i] - Neuronios[hl][ hc].rgbSColor[i] );
  r = double( 255 ) / ( 1 + r );
  result = r;
  return result;
}

void Connections(int knL, int knC )
{
  int i = 0, j = 0, k = 0;
  for ( i = 0; i < knC; i++)
    for ( j = 0; j < knL; j++)
    {
        Neuronios[i][ j].w[0] = 0.0;
        Neuronios[i][ j].w[1] = 0.0;
        Neuronios[i][ j].w[2] = 0.0;
        Neuronios[i][ j].w[3] = 0.0;
        Neuronios[i][ j].w[4] = 0.0;
        Neuronios[i][ j].w[5] = 0.0;
        Neuronios[i][ j].w[6] = 0.0;
        Neuronios[i][ j].w[7] = 0.0;
        Neuronios[i][ j].lateral = 0;
    }
  for ( i = 0; i < knC; i++)
    for ( j = 0; j < knL; j++)
    {
      if ( Neuronios[i][ j].stim >= limiarF )
      {
        if ( j - 1 >= 0 )
          Neuronios[i][ j].w[0] = Sim_Cor( i, j, i, j - 1 );
        if ( ( i - 1 >= 0 ) && ( j - 1 >= 0 ) )
          Neuronios[i][ j].w[1] = Sim_Cor( i, j, i - 1, j - 1 );
        if ( i - 1 >= 0 )
          Neuronios[i][ j].w[2] = Sim_Cor( i, j, i - 1, j );
        if ( ( i - 1 >= 0 ) && ( j + 1 < knC ) )
          Neuronios[i][ j].w[3] = Sim_Cor( i, j, i - 1, j + 1 );
        if ( j + 1 < knC )
          Neuronios[i][ j].w[4] = Sim_Cor( i, j, i, j + 1 );
        if ( ( i + 1 < knL ) && ( j + 1 < knC ) )
          Neuronios[i][ j].w[5] = Sim_Cor( i, j, i + 1, j + 1 );
        if ( i + 1 < knL )
          Neuronios[i][ j].w[6] = Sim_Cor( i, j, i + 1, j );
        if ( ( i + 1 < knL ) && ( j - 1 >= 0 ) )
          Neuronios[i][ j].w[7] = Sim_Cor( i, j, i + 1, j - 1 );
        for ( int stop = 8, k = 0; k <= stop; k++)
          Neuronios[i][ j].lateral = Neuronios[i][ j].lateral + Neuronios[i][ j].w[k];
      }
    }
}

int rnd( )
{
  int result = 0;
  result = rand() % MAXVAL;
  return result;
}

void normalization( TCMapArray& d, int knl_t, int knc_t )
{
  int i = 0, j = 0;
  double fmin = 0.0, fmax = 0.0;
  //Valor MIN
  fmin = 255;
  for ( i = 0; i <= knc_t; i++)
    for ( j = 0; j <= knl_t; j++)
      if ( fmin > d[i][ j] )
        fmin = d[i][ j];
  //Valor MAX
  fmax = 0;
  for ( i = 0; i <= knc_t; i++)
    for ( j = 0; j <= knl_t; j++)
    {
      d[i][ j] = d[i][ j] - fmin;
      if ( fmax < d[i][ j] )
        fmax = d[i][ j];
    }
  //Normalização - FIM
  for ( i = 0; i <= knc_t; i++)
    for ( j = 0; j <= knl_t; j++)
      if ( fmax != 0 )
        d[i][ j] = ( trunc( 255 * ( d[i][ j] / fmax ) ) );
}


void subtraction( TCMapArray& O, TCMapArray& d, int knl_t, int knc_t )
{
  int i = 0, j = 0;
  for ( i = 0; i < knc_t; i++)
    for ( j = 0; j < knl_t; j++)
      d[i][ j] = fabs( O[i][ j] - d[i][ j] );
  normalization( d, knl_t, knc_t );
}

void interpolation( TCMapArray& O, TCMapArray& d, int knl_t, int knc_t, double f )
{
  int i = 0, j = 0, a = 0, s = 0;
  for ( i = 0; i < knc_t; i++)
    for ( j = 0; j < knl_t; j++)
    {
      if ( f == 0 )
      {
        a = i;
        s = j;
      }
      else
        if ( f < 0 )
        {
          a = fabs( trunc( double( i ) / ( pow( 2, fabs( f ) ) ) ) );
          s = fabs( trunc( double( j ) / ( pow( 2, fabs( f ) ) ) ) );
        }
        else
        {
          a = trunc( i * pow( 2, f ) );
          s = trunc( j * pow( 2, f ) );
        }
      d[i][ j] = O[a][ s];
    }
}

void subtractionColor( TCMapArray& r, TCMapArray& O, TCMapArray& d, int knl_t, int knc_t )
{
  int i = 0, j = 0;
  for ( i = 0; i < knc_t; i++){
    for ( j = 0; j < knl_t; j++){
      r[i][ j] = fabs( O[i][ j] - d[i][ j] );
    }
  }

}


void convertMatToNeuron( Mat image, int knL, int knC )
{
    if ( image.rows > knL || image.cols > knC ) {
        cerr << "Image is too large." << endl;
        return;
    }

    if ((image.rows == 0) || (image.cols == 0)){
        cerr << "No captured image." << endl;
        return;
    }

    //Popular a matriz Neuronio com as informaçoes da imagem.
    for ( int i = 0; i < image.cols; i++ ) {
        for ( int j = 0; j < image.rows; j++ ) {
            Vec3b pixel = image.at<Vec3b>( j, i );
            unsigned char r = pixel[2];
            unsigned char g = pixel[1];
            unsigned char b = pixel[0];
            Neuronios[i][j].rgb[0] = (int)r;
            Neuronios[i][j].rgb[1] = (int)g;
            Neuronios[i][j].rgb[2] = (int)b;
            int pixelI = ( (int)r + (int)g + (int)b ) / 3;
            Neuronios[i][j].rgb[3] = pixelI;
            Neuronios[i][j].rgb[4] = pixelI;
            Neuronios[i][j].rgbSColor[0] = (int)r;
            Neuronios[i][j].rgbSColor[1] = (int)g;
            Neuronios[i][j].rgbSColor[2] = (int)b;
            Neuronios[i][j].v = rand()%1000;
            Neuronios[i][j].p = 0;
            Neuronios[i][j].d = 0;
            Neuronios[i][j].pulsar = !( r == 0 && g == 0 && b == 0 );
            Neuronios[i][j].l = i;
            Neuronios[i][j].c = j;
            Neuronios[i][j].rn = 0.0;
            Neuronios[i][j].segN = -1;

        }
    }

}

Mat convertNeuronToMat( Mat imagem )
{
    //Popular a matriz Neuronio com as informaçoes da imagem.
    for ( int i = 0; i < imagem.cols; i++ ) {
        for ( int j = 0; j < imagem.rows; j++ ) {
            Vec3b pixel;
            pixel[2]= Neuronios[i][j].rgb[0];
            pixel[1]= Neuronios[i][j].rgb[1];
            pixel[0]= Neuronios[i][j].rgb[2];
            imagem.at<Vec3b>( j, i ) = pixel;
        }
    }
    return( imagem );
}



principal::principal()
{

}
Mat Saliencia( Mat image ){

    int i = 0, j = 0, a = 0, b = 0, s = 0, t = 0, c = 0, i_ = 0, j_ = 0;
    int knl_t = 0, knc_t = 0;
    int n = 0;

    TCMapArray MCStmp;
    double maxloc_rg[ 6 ], maxloc_by[ 6 ];
    double medloc_rg[ 6 ], medloc_by[ 6 ];
    double medmaxglobal = 0.0;


                                                            /*INICIO DA FUNÇÃO */


    int knL = image.rows;
    int knC = image.cols;

    //Verificando numero de linhas e colunas
    if ( ( knL > MaxKnl ) || ( knC > MaxKnc ) ){
      cout << "Imagem com dimensões não suportadas";
      //return 0;
    }

    convertMatToNeuron(image, knL, knC);

    //Criacao do template para o filtro Gaussiano com tamanho e desvio padrao ---
    if ( seNb % 2 == 0 ){
      seNb = seNb + 1;
    }
    n = seNb;
    desv_P = seDPb; //sigma


    double sigmatmp = 0.0, sigmatmp2 = 0.0;
    int sa = 0, ta = 0, X = 0, y = 0;

    sigmatmp = 2 * desv_P * desv_P;
    sigmatmp2 = 2 * M_PI * desv_P * desv_P;

    //Calculando a gaussiana
    sa = ( n - 1 ) / 2;
    ta = sa;

    for ( X = - sa; X <= sa; X++){
      for ( y = - ta; y <= ta; y++){
        w[y + ta][ X + sa] = exp( - (X*X)  +  (y*y)  / ( sigmatmp ) ) / ( sigmatmp2 );
      }
    }

    //1 - suavizacao da imagem de entrada com filtro gaussiano
    //nivel 0 da piramede
    a = ( n - 1 ) / 2;
    b = a;

    for ( i = 0; i < knC; i++){
      for ( j = 0; j < knL; j++){
        IM[0][i][j][0] = 0; //R
        IM[0][i][j][1] = 0; //G
        IM[0][i][j][2] = 0; //B
        tmp = 0;

        for ( s = - a; s <= a; s++){
          for ( t = - b; t <= b; t++){

            if ( ( ( i + s >= 0 ) && ( i + s < knC ) ) && ( ( j + t >= 0 ) && ( j + t < knL ) ) )
            {
              IM[0][i][j][0] = IM[0][i][j][0] + trunc( w[s + a][ t + b] * Neuronios[i + s][ j + t].rgb[0] );
              IM[0][i][j][1] = IM[0][i][j][1] + trunc( w[s + a][ t + b] * Neuronios[i + s][ j + t].rgb[1] );
              IM[0][i][j][2] = IM[0][i][j][2] + trunc( w[s + a][ t + b] * Neuronios[i + s][ j + t].rgb[2] );

            }
          }
        }

        Cor.r = IM[0][i][j][0];
        Cor.g = IM[0][i][j][1];
        Cor.b = IM[0][i][j][2];


        //Canais de Cores.
        Mr[0][i][j] = trunc( ( ( Cor.r - ( Cor.g + Cor.b ) ) ) / 2 );
        Mg[0][i][j] = trunc( ( ( Cor.g - ( Cor.r + Cor.b ) ) ) / 2 );
        Mb[0][i][j] = trunc( ( ( Cor.b - ( Cor.r + Cor.g ) ) ) / 2 );
        My[0][i][j] = trunc( ( Cor.r + Cor.g ) - ( 2 * ( fabs( Cor.r - Cor.g ) + Cor.b ) ) );

      }
    }

    //Passo 01 - criando a piramede gaussiana
    //niveis 1 a 8
    //Criando os mapas de cores Rn , Gn , Bn , Yn
    c = 1;
    knl_t = trunc( ( knL ) / 2 );
    knc_t = trunc( ( knC ) / 2 );
    do
    {
      for ( i = 0; i < knc_t; i++){
        for ( j = 0; j <= knl_t; j++){
          //suavizando a gaussiana
          i_ = trunc( i * 2 );
          j_ = trunc( j * 2 );
          IM[c][i][j][0] = 0;
          IM[c][i][j][1] = 0;
          IM[c][i][j][2] = 0;

          for ( s = - a; s <= a; s++){
            for ( t = - b; t <= b; t++){
              if ( ( ( i_ + s >= 0 ) && ( i_ + s < ( knc_t * 2 ) ) ) && ( ( j_ + t >= 0 ) && ( j_ + t < ( knl_t * 2 ) ) ) ){

                IM[c][i][j][0] = IM[c][i][j][0] + trunc( w[s + a][ t + b] * IM[c - 1][i_ + s][ j_ + t][0] );
                IM[c][i][j][1] = IM[c][i][j][1] + trunc( w[s + a][ t + b] * IM[c - 1][i_ + s][ j_ + t][1] );
                IM[c][i][j][2] = IM[c][i][j][2] + trunc( w[s + a][ t + b] * IM[c - 1][i_ + s][ j_ + t][2] );
              }
            }
          }

          Cor.r = IM[c][i][j][0]; //R
          Cor.g = IM[c][i][j][1]; //G
          Cor.b = IM[c][i][j][2]; //B

          //Canais de cores
          Mr[c][i][j] = trunc( ( ( Cor.r - ( Cor.g + Cor.b ) ) ) / 2 );
          Mg[c][i][j] = trunc( ( ( Cor.g - ( Cor.r + Cor.b ) ) ) / 2 );
          Mb[c][i][j] = trunc( ( ( Cor.b - ( Cor.r + Cor.g ) ) ) / 2 );
          My[c][i][j] = trunc( ( Cor.r + Cor.g ) - ( 2 * ( fabs( Cor.r - Cor.g ) + Cor.b ) ) );

        }
      }

      c++;
      knl_t = trunc( ( knl_t ) / 2 );
      knc_t = trunc( ( knc_t ) / 2 );
    }while ( ! ( c == 9 ) );

    // Interpolação dos Canais c{1,2,3} s{2,3}
    b = 0;
    knl_t = trunc( ( ( knL ) / 2 ) );
    knc_t = trunc( ( ( knC ) / 2 ) );

    do{
      if ( b == 0 ){
        //CORES
        //Color center-surround difference maps (r-g) (g - r) => Cores Oponentes
        //Subtraindo r de g no nivel 1, mandando para Mrg
        subtractionColor( Mrg, Mr[1], Mg[1], knl_t, knc_t );
        //Subtraindo g do r no nivel 3, mandando para Mgr
        subtractionColor( Mgr, Mg[3], Mr[3], knl_t, knc_t );
        interpolation( Mgr, MCSrg[b], knl_t, knc_t, - 2 ); // 1 <-- 3
        subtraction( Mrg, MCSrg[b], knl_t, knc_t ); // 1 - 3


        // Color center-surround difference maps (y-b) - (b-y)
        subtractionColor( Myb, My[1], Mb[1], knl_t, knc_t );
        subtractionColor( Mby, Mb[3], My[3], knl_t, knc_t ); //1 <-- 3
        interpolation( Mby, MCSby[b], knl_t, knc_t, - 2 ); // 1 - 3
        subtraction( Myb, MCSby[b], knl_t, knc_t );


        b++;

        //Cores
         //Color center-surround difference maps (r-g) - (g-r)
        subtractionColor( Mrg, Mr[1], Mg[1], knl_t, knc_t );
        subtractionColor( Mgr, Mg[4], Mr[4], knl_t, knc_t );
        interpolation( Mgr, MCSrg[b], knl_t, knc_t, - 3 ); //1 <-- 4
        subtraction( Mrg, MCSrg[b], knl_t, knc_t ); //1 - 4

        // Color center-surround difference maps (y-b) - (b-y)
        subtractionColor( Myb, My[1], Mb[1], knl_t, knc_t );
        subtractionColor( Mby, Mb[4], My[4], knl_t, knc_t );
        interpolation( Mby, MCSby[b], knl_t, knc_t, - 3 ); //1 <-- 4
        subtraction( Myb, MCSby[b], knl_t, knc_t ); //1 - 4

      }
      if ( b == 2 ){
        //CORES
         //Color center-surround difference maps (r-g) - (g-r)
        subtractionColor( Mrg, Mr[2], Mg[2], knl_t, knc_t );
        subtractionColor( Mgr, Mg[4], Mr[4], knl_t, knc_t );
        interpolation( Mgr, MCSrg[b], knl_t, knc_t, - 2 ); // 2 <-- 4
        subtraction( Mrg, MCSrg[b], knl_t, knc_t ); //2 - 4


        //Color center-surround difference maps (y-b) - (b-y)
        subtractionColor( Myb, My[2], Mb[2], knl_t, knc_t );
        subtractionColor( Mby, Mb[4], My[4], knl_t, knc_t );
        interpolation( Mby, MCSby[b], knl_t, knc_t, - 2 ); // 2 <--4
        subtraction( Myb, MCSby[b], knl_t, knc_t ); //2 - 4

        b++;

        //CORES
        // Color center-surround difference maps (r-g) - (g-r)
        subtractionColor( Mrg, Mr[2], Mg[2], knl_t, knc_t );
        subtractionColor( Mgr, Mg[5], Mr[5], knl_t, knc_t );
        interpolation( Mgr, MCSrg[b], knl_t, knc_t, - 3 ); // 2 <--5
        subtraction( Mrg, MCSrg[b], knl_t, knc_t ); // 2 - 5

        //Color center-surround difference maps (y-b) - (b-y)
        subtractionColor( Myb, My[2], Mb[2], knl_t, knc_t );
        subtractionColor( Mby, Mb[5], My[5], knl_t, knc_t );
        interpolation( Mby, MCSby[b], knl_t, knc_t, - 3 ); // 2 <-- 5
        subtraction( Myb, MCSby[b], knl_t, knc_t ); // 2 - 5

      }
      if ( b == 4 ){

        // Color center-surround difference maps (r-g) - (g-r)
        subtractionColor( Mrg, Mr[3], Mg[3], knl_t, knc_t );
        subtractionColor( Mgr, Mg[5], Mr[5], knl_t, knc_t );
        interpolation( Mgr, MCSrg[b], knl_t, knc_t, - 2 ); // 3 <-- 5
        subtraction( Mrg, MCSrg[b], knl_t, knc_t ); // 3 - 5

        //Color center-surround difference maps (y-b) - (b-y)
        subtractionColor( Myb, My[3], Mb[3], knl_t, knc_t );
        subtractionColor( Mby, Mb[5], My[5], knl_t, knc_t );
        interpolation( Mby, MCSby[b], knl_t, knc_t, - 2 );// 3 <-- 5
        subtraction( Myb, MCSby[b], knl_t, knc_t ); // 3 - 5

        b++;

        //CORES
        // Color center-surround difference maps (r-g) - (g-r)
        subtractionColor( Mrg, Mr[3], Mg[3], knl_t, knc_t );
        subtractionColor( Mgr, Mg[6], Mr[6], knl_t, knc_t );
        interpolation( Mgr, MCSrg[b], knl_t, knc_t, - 3 );//3 <-- 6
        subtraction( Mrg, MCSrg[b], knl_t, knc_t ); //3 - 6

        //Color center-surround difference maps (y-b) - (b-y)
        subtractionColor( Myb, My[3], Mb[3], knl_t, knc_t );
        subtractionColor( Mby, Mb[6], My[6], knl_t, knc_t );
        interpolation( Mby, MCSby[b], knl_t, knc_t, - 3 ); //3 <--6
        subtraction( Myb, MCSby[b], knl_t, knc_t ); // 3 - 6
      }

      b++;
      knl_t = trunc( ( knl_t ) / 2 );
      knc_t = trunc( ( knc_t ) / 2 );
    }while ( ! ( b >= 5 ) );

    // Geração dos Mapas de Conspicuidades ---------------------------------------
    // Canais c{1,2,3} s{2,3}
    // Interpolação de todos para o nível 3
    b = 0;
    knl_t = trunc( ( ( knL ) / pow( 2, 3 ) ) );
    knc_t = trunc( ( ( knC ) / pow( 2, 3 ) ) );

    do{
      switch ( b ){
        case  0 :
          for(int testi = 0; testi < knc_t; testi++){
              for(int testj = 0; testj < knl_t; testj++){
                 MCStmp[testi][testj] = MCSrg[b][testi][testj];
              }
          }

          interpolation( MCStmp, MCSrg_t[b], knl_t, knc_t, 2 ); // 3 <-- 1

          for(int testi = 0; testi < knc_t; testi++){
              for(int testj = 0; testj < knl_t; testj++){
                 MCStmp[testi][testj] = MCSby[b][testi][testj];
              }
          }

          interpolation( MCStmp, MCSby_t[b], knl_t, knc_t, 2 ); // 3 <--  1
        break;
      case 1:
          for(int testi = 0; testi < knc_t; testi++){
              for(int testj = 0; testj < knl_t; testj++){
                 MCStmp[testi][testj] = MCSrg[b][testi][testj];
              }
          }

          interpolation( MCStmp, MCSrg_t[b], knl_t, knc_t, 2 ); // 3 <-- 1

          for(int testi = 0; testi < knc_t; testi++){
              for(int testj = 0; testj < knl_t; testj++){
                 MCStmp[testi][testj] = MCSby[b][testi][testj];
              }
          }

          interpolation( MCStmp, MCSby_t[b], knl_t, knc_t, 2 ); // 3 <--  1
        break;

        case 2 :
          for(int testi = 0; testi < knc_t; testi++){
              for(int testj = 0; testj < knl_t; testj++){
                 MCStmp[testi][testj] = MCSrg[b][testi][testj];
              }
          }
          interpolation( MCStmp, MCSrg_t[b], knl_t, knc_t, 1 ); // 3 <-- 2

          for(int testi = 0; testi < knc_t; testi++){
              for(int testj = 0; testj < knl_t; testj++){
                 MCStmp[testi][testj] = MCSrg[b][testi][testj];
              }
          }

          interpolation( MCStmp, MCSby_t[b], knl_t, knc_t, 1 ); // 3 <-- 2
        break;

      case 3:

          for(int testi = 0; testi < knc_t; testi++){
              for(int testj = 0; testj < knl_t; testj++){
                 MCStmp[testi][testj] = MCSrg[b][testi][testj];
              }
          }
          interpolation( MCStmp, MCSrg_t[b], knl_t, knc_t, 1 ); // 3 <-- 2

          for(int testi = 0; testi < knc_t; testi++){
              for(int testj = 0; testj < knl_t; testj++){
                 MCStmp[testi][testj] = MCSrg[b][testi][testj];
              }
          }

          interpolation( MCStmp, MCSby_t[b], knl_t, knc_t, 1 ); // 3 <-- 2
        break;

        case 4 :

          for(int testi = 0; testi < knc_t; testi++){
              for(int testj = 0; testj < knl_t; testj++){
                 MCStmp[testi][testj] = MCSrg[b][testi][testj];
              }
          }
          interpolation( MCStmp, MCSrg_t[b], knl_t, knc_t, 0 ); // 3 <-- 3

          for(int testi = 0; testi < knc_t; testi++){
              for(int testj = 0; testj < knl_t; testj++){
                 MCStmp[testi][testj] = MCSrg[b][testi][testj];
              }
          }

          interpolation( MCStmp, MCSby_t[b], knl_t, knc_t, 0 ); // 3 <-- 3
        break;
      case 5:

          for(int testi = 0; testi < knc_t; testi++){
              for(int testj = 0; testj < knl_t; testj++){
                 MCStmp[testi][testj] = MCSrg[b][testi][testj];
              }
          }
          interpolation( MCStmp, MCSrg_t[b], knl_t, knc_t, 0 ); // 3 <-- 3

          for(int testi = 0; testi < knc_t; testi++){
              for(int testj = 0; testj < knl_t; testj++){
                 MCStmp[testi][testj] = MCSrg[b][testi][testj];
              }
          }

          interpolation( MCStmp, MCSby_t[b], knl_t, knc_t, 0 ); // 3 <-- 3
          break;
      }
      b++;
    }while ( ! ( b == 6 ) );

    //Across-scale addition => Adição atraves das escalas
    knl_t = trunc( ( ( knL ) / pow( 2, 3 ) ) );
    knc_t = trunc( ( ( knC ) / pow( 2, 3 ) ) );
    b = 0;
    medmaxglobal = 0;

    do{
      maxloc_rg[b] = 0;
      maxloc_by[b] = 0;
      medloc_rg[b] = 0;
      medloc_by[b] = 0;
      for ( i = 0; i < knc_t; i++){
        for ( j = 0; j < knl_t; j++){
          //Maps global maximum
          if ( maxloc_rg[b] < MCSrg_t[b][i][j] ){
            maxloc_rg[b] = MCSrg_t[b][i][j];
          }
          if ( maxloc_by[b] < MCSby_t[b][i][j] ){
            maxloc_by[b] = MCSby_t[b][i][j];
          }
        }
      }

      medmaxglobal = medmaxglobal + ( maxloc_rg[b] + maxloc_by[b] );
      medloc_rg[b] = medloc_rg[b] + maxloc_rg[b];
      medloc_by[b] = medloc_by[b] + maxloc_by[b];

      b++;
    }while ( ! ( b == 6 ) );

    medmaxglobal =  medmaxglobal/12;


    // Multiply the map with (M-m)^2.
    b = 0;
    do{
      for ( i = 0; i < knl_t; i++){
        for ( j = 0; j < knc_t; j++){
          MCSrg_t[b][i][j] = MCSrg_t[b][i][j] * (( maxloc_rg[b] - ( ( medloc_rg[b] ) / 6 ) ) * ( maxloc_rg[b] - ( ( medloc_rg[b] ) / 6 ) ));
          MCSby_t[b][i][j] = MCSby_t[b][i][j] * (( maxloc_by[b] - ( ( medloc_by[b] ) / 6 ) ) * ( maxloc_by[b] - ( ( medloc_by[b] ) / 6 ) ));

          //simplesmente zerando
          if ( b == 0 ){
            Cm_i[i][ j] = 0;
            Cm_c[i][ j] = 0;
            Cm_o[i][ j] = 0;
          }
        }
      }
      b++;
    }while ( ! ( b == 6 ) );

    //Normalização
    b = 0;
    do{
      normalization( MCSrg_t[b], knl_t, knc_t );
      normalization( MCSby_t[b], knl_t, knc_t );
      b++;
    }while ( ! ( b == 6 ) );

    b = 0;
    do{
      for ( i = 0; i < knc_t; i++){
        for ( j = 0; j < knl_t; j++){
          Cm_c[i][ j] = Cm_c[i][ j] + (  MCSrg_t[b][i][j] + MCSby_t[b][i][j] );
        }
     }
     b++;
    }while ( ! ( b == 6 ) );
    normalization( Cm_c, knl_t, knc_t );

    //Interpolaçao para o tamanho da imagem de entrada
    for(int testi = 0; testi < knC; testi++){
        for(int testj = 0; testj < knL; testj++){
           MCStmp[testi][testj] = Cm_c[testi][testj];
        }
    }
    interpolation( MCStmp, Cm_c, knL, knC, - 3 );//0 <-- 3

    for(int i = 0; i < knC; i++){
        for(int j = 0; j < knL; j++){
            Neuronios[i][j].rgb[0] = Neuronios[i][j].rgb[1] = Neuronios[i][j].rgb[2] = trunc(Cm_c[i][j]);
        }
    }



  /*============================================================ Fim do Itti ============================================================================*/

    convertNeuronToMat(image);
    return(image);

}

int main(int argc, char *argv[]){

    srand( (unsigned)time(NULL) );

    Mat imagem;

    imagem = imread("testeBlur.jpg");
    imshow( "Original", imagem );

    Mat salientada  = imagem.clone();
    salientada = Saliencia(salientada);

    imshow( "Salientada", salientada );

    waitKey( 0 );

    return 0;
}

