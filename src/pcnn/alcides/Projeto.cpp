unit UnitProjeto;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ComCtrls, Menus, ExtCtrls, StdCtrls, Buttons, ExtDlgs, jpeg,
  Grids, ColorGrd, Mask, Spin, TeEngine,
  Series, TeeProcs, Chart,UNIT_LISTA_ARRANJO, UNIT_FILA_APONTADOR, UnitZoom,Math,
  FileCtrl, MLP, DB, DBTables, DBCtrls, DBGrids, UnitLegion, TeeSurfa,
  TeeURL, TeeSeriesTextEd, TeeTriSurface, MyPoint, Camera, PiconeBarreTache, MMSystem,
  MPlayer ;

const
  MAXVAL = 32768;  // 2147483648.0, or 32768.0, machine dependable
  MAXIMO = 32767; //* 2147483647 or 32767, machine dependable */
  GRAY = 255;     //* Maximum value of a pixel */
  Num  = 1;
  KS   = 5;       //* the diameter of a kernel */
  LP   = 0;       //* value of the left point in the limit cycle */
  RP   = 4;       //* value of the right point in the limit cycle */
  LK   = 1;       //* value of the left knee in the limit cycle */
  RK   = 3;       //* value of the right knee in the limit cycle */
  M	   = 25600;   //* number of displayed oscillators = N1xN2 */
  MaxKnl = 500;
  MaxKnc = 700;

type

  TRGBDouble = record
     b : double;
     g : double;
     r : double;
  end;

  TRGB = record
     b : byte;
     g : byte;
     r : byte;
  end;

  PRGB = ^VRGB;
  VRGB = array [0..32767] of TRGB;

  TPos = Record
            l,c : integer;
            nt : real;
          end;

  TMat = Record
            v : real;
            p,d : byte; //p,d : byte;
            v_ant : real;
            rn : real;
            i : real;
            rgb,Seg_rgb : array[0..4] of byte;
            rgbSColor: array [0..2] of byte;
            ang : array[0..4] of byte;
            rgb_Itti: array[0..2] of byte;
            y: real;
            l,c : integer;
            pulsar : boolean;
            // LEGION
            lx : array[0..1] of double;
            lateral : double;               //* lateral: the sum of input from lateral connections */
            w : array[0..7] of double;      //* strength of the interactions for 8 directions */
            stim, matrix : integer;
            ly, cycle, trigger : integer;
            //* y: integer of x; cycle: test for 1st cycle; trigger: jumping signal */
            segN : integer;
         end;

  TCMapArray = array[0..MaxKnl,0..MaxKnc] of double;
  TCMap0 = array[0..8] of array[0..MaxKnl,0..MaxKnc] of array[0..3] of double;
  TCMap = array[0..8] of TCMapArray;
  TCMapCS = array[0..5] of TCMapArray;
  TCMap_Norm = array[0..8] of double;
  TIeD = array of array of TMat;

  TVetorAtributos = array [0..12] of integer;
  TVetorAtributos_Norma = array [0..12] of double;

  ArrayNeuronios = array[0..1500,0..1500] of TVetorAtributos;

  TVetorAtributos_ReEscalar = record
                                 v : array [0..12] of double;
                                 max,min : array [0..12] of integer;
                              end;


  TSEG = record
             Sknl,Sknc  : integer;
             imin,imax,jmin,jmax : integer;
             SegNeuronios : TIeD;
             //SegMedias : TVetorAtributos;
             SegMedias : TVetorAtributos_Norma;
             IeD : TMat;
             v_Max : real;
             Classify : real;
             segValido : boolean;
         end;

  TSeg_Classify = record
             imin,imax,jmin,jmax : integer;
             Classify : real;
         end;

  // gaborkernel2D
  TW = array[0..150,0..150] of extended;

  TFormProjeto = class(TForm)
    MainMenu1: TMainMenu;
    Sair1: TMenuItem;
    OpenPictureDialog1: TOpenPictureDialog;
    PageControl1: TPageControl;
    OpenDialog1: TOpenDialog;
    StatusBar: TStatusBar;
    TabSheet74: TTabSheet;
    PageControl13: TPageControl;
    TabSheet75: TTabSheet;
    mod_ImgExp4: TImage;
    GroupBox42: TGroupBox;
    TabSheet78: TTabSheet;
    TabSheet81: TTabSheet;
    Timer: TTimer;
    GroupBox47: TGroupBox;
    cbExp4_Cinza: TCheckBox;
    Timer2: TTimer;
    Timer1: TTimer;
    TabSheet84: TTabSheet;
    Timer3: TTimer;
    Timer4: TTimer;
    Timer5: TTimer;
    BitBtn36: TBitBtn;
    TabSheet87: TTabSheet;
    cbExp4_Color: TCheckBox;
    Bevel4: TBevel;
    Bevel8: TBevel;
    cbExp4_MP: TCheckBox;
    Label86: TLabel;
    Bevel9: TBevel;
    tbpulso: TTrackBar;
    GroupBox44: TGroupBox;
    sgExp4_CNA: TStringGrid;
    sgExp4_CNB: TStringGrid;
    sgExp4_CN45P: TStringGrid;
    sgExp4_CN45N: TStringGrid;
    Label64: TLabel;
    mod_ImgExp4_GCanny: TImage;
    mod_ImgExp4_BCanny: TImage;
    Label63: TLabel;
    mod_ImgExp4_RCanny: TImage;
    mod_ImgExp4_ICanny: TImage;
    Label71: TLabel;
    Label62: TLabel;
    cbSeg: TCheckBox;
    Timer6: TTimer;
    Bevel5: TBevel;
    cbExp4_Cores: TCheckBox;
    Label5: TLabel;
    Label6: TLabel;
    Label7: TLabel;
    Label8: TLabel;
    mod_ImgExp4B: TImage;
    Label24: TLabel;
    Label25: TLabel;
    Label27: TLabel;
    Label28: TLabel;
    Label29: TLabel;
    Label30: TLabel;
    Label35: TLabel;
    Label36: TLabel;
    Label37: TLabel;
    Label38: TLabel;
    Label39: TLabel;
    Label40: TLabel;
    Label41: TLabel;
    Label42: TLabel;
    FileListBox1: TFileListBox;
    Label46: TLabel;
    Shape4: TShape;
    BitBtn32: TBitBtn;
    rbExp4_8: TRadioButton;
    RadioButton13: TRadioButton;
    Bevel10: TBevel;
    TabSheet1: TTabSheet;
    PageControl2: TPageControl;
    TabSheet2: TTabSheet;
    GroupBox2: TGroupBox;
    Label47: TLabel;
    seNEpocas: TSpinEdit;
    seNeuronios: TSpinEdit;
    Label48: TLabel;
    Label49: TLabel;
    seAprendizagem: TSpinEdit;
    seInercia: TSpinEdit;
    Label50: TLabel;
    Label51: TLabel;
    edArquivo: TEdit;
    PageControl3: TPageControl;
    TabSheet3: TTabSheet;
    StatusBar1: TStatusBar;
    Label58: TLabel;
    snClasses: TSpinEdit;
    TabSheet4: TTabSheet;
    GroupBox3: TGroupBox;
    DBNavigator1: TDBNavigator;
    DBGrid1: TDBGrid;
    DBImage1: TDBImage;
    Shape5: TShape;
    Label52: TLabel;
    SpeedButton1: TSpeedButton;
    SpeedButton2: TSpeedButton;
    Label54: TLabel;
    Label55: TLabel;
    Label56: TLabel;
    Label57: TLabel;
    ed3: TEdit;
    ed2: TEdit;
    ed1: TEdit;
    ed0: TEdit;
    Label59: TLabel;
    DirectoryListBox2: TDirectoryListBox;
    Label60: TLabel;
    FileListBox2: TFileListBox;
    imgTeste: TImage;
    Shape6: TShape;
    SpeedButton4: TSpeedButton;
    SpeedButton3: TSpeedButton;
    Image1: TImage;
    Shape7: TShape;
    Image2: TImage;
    Image3: TImage;
    Image4: TImage;
    sp0: TShape;
    sp1: TShape;
    sp2: TShape;
    sp3: TShape;
    Label61: TLabel;
    TabSheet5: TTabSheet;
    img_OBJ_Pulsante_02: TImage;
    Label72: TLabel;
    Label73: TLabel;
    Label74: TLabel;
    Label82: TLabel;
    Label83: TLabel;
    Label87: TLabel;
    tbI: TTrackBar;
    tbRG: TTrackBar;
    tbBY: TTrackBar;
    tbXY: TTrackBar;
    tbRN: TTrackBar;
    tbSize: TTrackBar;
    Label101: TLabel;
    BitBtn2: TBitBtn;
    TabSheet7: TTabSheet;
    PopupMenu1: TPopupMenu;
    VisualizarAtividadedesteNeurnio1: TMenuItem;
    SelecionarNeurnio011: TMenuItem;
    SelecionarNeurnio021: TMenuItem;
    SelecionarNeurnio031: TMenuItem;
    SelecionarNeurnio041: TMenuItem;
    N1: TMenuItem;
    N2: TMenuItem;
    LimparaSeleo1: TMenuItem;
    N3: TMenuItem;
    PageControl4: TPageControl;
    TabSheet8: TTabSheet;
    GroupBox5: TGroupBox;
    Label106: TLabel;
    Label110: TLabel;
    Label112: TLabel;
    Label121: TLabel;
    TrackBarTetax: TTrackBar;
    TrackBarthetap: TTrackBar;
    Editthetap: TEdit;
    EditTetax: TEdit;
    GroupBox6: TGroupBox;
    Label113: TLabel;
    Label114: TLabel;
    Label115: TLabel;
    Label116: TLabel;
    TrackBarWz: TTrackBar;
    TrackBarTetaxz: TTrackBar;
    EditWz: TEdit;
    EditTetaxz: TEdit;
    GroupBox7: TGroupBox;
    Label107: TLabel;
    TrackBarLimiarFundo: TTrackBar;
    EditLimiarFundo: TEdit;
    Panel1: TPanel;
    Image5: TImage;
    cbIni: TCheckBox;
    TabSheet12: TTabSheet;
    cbExp4_LO: TCheckBox;
    SEditLO: TSpinEdit;
    Bevel1: TBevel;
    TabSheet9: TTabSheet;
    GroupBox9: TGroupBox;
    ColorGridSeg: TColorGrid;
    Label9: TLabel;
    CBBorda: TCheckBox;
    Label120: TLabel;
    Shape9: TShape;
    cbVer_Sincro: TCheckBox;
    Img_Segmentos: TImage;
    cb_Strech: TCheckBox;
    SpeedButton5: TSpeedButton;
    Label122: TLabel;
    Shape8: TShape;
    Label123: TLabel;
    TabSheet13: TTabSheet;
    GroupBox10: TGroupBox;
    Label125: TLabel;
    mskDt_Sal: TMaskEdit;
    Label126: TLabel;
    Label127: TLabel;
    Panel3: TPanel;
    Image11: TImage;
    mod_ImgExp4_IeF_OR: TImage;
    cbVer_Just_S: TCheckBox;
    Label128: TLabel;
    tbLimiarFundoSeg: TTrackBar;
    EditLimiarFundoSeg: TEdit;
    TabSheet14: TTabSheet;
    Memo1: TMemo;
    mskWInib: TMaskEdit;
    Label75: TLabel;
    SeriesTextSource1: TSeriesTextSource;
    Timer7: TTimer;
    PageControl5: TPageControl;
    TabSheet15: TTabSheet;
    TabSheet16: TTabSheet;
    TabSheet17: TTabSheet;
    mod_ImgExp4_SOM_SAL: TImage;
    rgIE: TRadioButton;
    rbIS: TRadioButton;
    mod_ImgExp4_SAL: TImage;
    cbV3D: TCheckBox;
    CSal: TChart;
    Series1: TSurfaceSeries;
    tb3dDim: TTrackBar;
    Label10: TLabel;
    tbOrtho: TTrackBar;
    Label11: TLabel;
    Label12: TLabel;
    TabSheet10: TTabSheet;
    CO: TChart;
    Series2: TBarSeries;
    Label13: TLabel;
    Label14: TLabel;
    Label15: TLabel;
    mskThetas: TMaskEdit;
    Label16: TLabel;
    TabSheet11: TTabSheet;
    COII: TChart;
    Series3: TFastLineSeries;
    Series4: TFastLineSeries;
    Series5: TFastLineSeries;
    Series6: TFastLineSeries;
    Series7: TFastLineSeries;
    Series8: TFastLineSeries;
    Series9: TFastLineSeries;
    Series10: TFastLineSeries;
    Series11: TFastLineSeries;
    Series12: TFastLineSeries;
    Series13: TFastLineSeries;
    Series14: TFastLineSeries;
    Series15: TFastLineSeries;
    Series16: TFastLineSeries;
    Series17: TFastLineSeries;
    Series18: TFastLineSeries;
    Series19: TFastLineSeries;
    Series20: TFastLineSeries;
    Series21: TFastLineSeries;
    Series22: TFastLineSeries;
    Series23: TFastLineSeries;
    Series24: TFastLineSeries;
    Series25: TFastLineSeries;
    Series26: TFastLineSeries;
    Series27: TFastLineSeries;
    Series28: TFastLineSeries;
    Series29: TFastLineSeries;
    Series30: TFastLineSeries;
    Series31: TFastLineSeries;
    Series32: TFastLineSeries;
    BitBtn1: TBitBtn;
    mskVIT: TMaskEdit;
    GroupBox8: TGroupBox;
    Label20: TLabel;
    Label17: TLabel;
    mskWc: TMaskEdit;
    Label18: TLabel;
    Label19: TLabel;
    mskWi: TMaskEdit;
    Label21: TLabel;
    Label22: TLabel;
    mskWo: TMaskEdit;
    Label23: TLabel;
    Label26: TLabel;
    mskWsum: TMaskEdit;
    cbWCor: TCheckBox;
    cbWInt: TCheckBox;
    cbWOri: TCheckBox;
    TabSheet18: TTabSheet;
    ChartOBF: TChart;
    FastLineSeries2: TFastLineSeries;
    ChartCP: TChart;
    FastLineSeries1: TFastLineSeries;
    Series33: TBarSeries;
    CBCam: TCheckBox;
    TabSheet19: TTabSheet;
    PageControl6: TPageControl;
    TabSheet20: TTabSheet;
    GroupBox27: TGroupBox;
    Label43: TLabel;
    Label44: TLabel;
    seNb: TSpinEdit;
    seDPb: TMaskEdit;
    GroupBox11: TGroupBox;
    Label31: TLabel;
    Label32: TLabel;
    seMask_GF: TSpinEdit;
    meDP_GF: TMaskEdit;
    meWL: TMaskEdit;
    Label33: TLabel;
    mePO: TMaskEdit;
    Label34: TLabel;
    meAR: TMaskEdit;
    Label69: TLabel;
    TabSheet21: TTabSheet;
    CP: TChart;
    Label70: TLabel;
    Label76: TLabel;
    TrackBar1: TTrackBar;
    TrackBar2: TTrackBar;
    SurfaceSeries1: TSurfaceSeries;
    Label80: TLabel;
    Label81: TLabel;
    Label84: TLabel;
    Memo2: TMemo;
    TabSheet22: TTabSheet;
    Image_GB: TImage;
    CBCP3D: TCheckBox;
    TrackBar4: TTrackBar;
    Label77: TLabel;
    Label78: TLabel;
    TrackBar3: TTrackBar;
    Label79: TLabel;
    BitBtn4: TBitBtn;
    RGTheta: TRadioGroup;
    BitBtn3: TBitBtn;
    Label85: TLabel;
    BitBtn5: TBitBtn;
    BitBtn6: TBitBtn;
    SpeedButton6: TSpeedButton;
    SpeedButton7: TSpeedButton;
    Camera1: TCamera;
    TimerCam: TTimer;
    mod_ImgExp4_Itti_Cm_c: TImage;
    mod_ImgExp4_IeF: TImage;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    CBAttention: TCheckBox;
    BitBtn7: TBitBtn;
    Timer8: TTimer;
    Label68: TLabel;
    MLP: TMLP;
    mod_sp0: TShape;
    Image7: TImage;
    mod_ed0: TEdit;
    Label53: TLabel;
    mod_sp1: TShape;
    Image6: TImage;
    mod_ed1: TEdit;
    Label65: TLabel;
    mod_sp3: TShape;
    Image8: TImage;
    mod_ed3: TEdit;
    Label66: TLabel;
    mod_sp2: TShape;
    Image9: TImage;
    mod_ed2: TEdit;
    Label67: TLabel;
    CBCamTC: TCheckBox;
    CBSegTC: TCheckBox;
    mod_sp4: TShape;
    Image10: TImage;
    Label4: TLabel;
    mod_ed4: TEdit;
    Image12: TImage;
    mod_sp5: TShape;
    Label45: TLabel;
    mod_ed5: TEdit;
    Image13: TImage;
    mod_sp6: TShape;
    Label88: TLabel;
    mod_ed6: TEdit;
    Image14: TImage;
    mod_sp7: TShape;
    Label89: TLabel;
    mod_ed7: TEdit;
    Image15: TImage;
    mod_sp8: TShape;
    Label90: TLabel;
    mod_ed8: TEdit;
    Image16: TImage;
    mod_sp9: TShape;
    Label91: TLabel;
    mod_ed9: TEdit;
    procedure Sair1Click(Sender: TObject);
    procedure IniciarContagem;
    procedure FinalizarContagem;
    procedure mod_ImgExp4DblClick(Sender: TObject);
    procedure BitBtn32Click(Sender: TObject);
    procedure Zoom(Sender: TObject);
    //procedure ShowSincro;
    function Similaridade_CorMedia(i,j,c:integer) : double;
    function Sim_Cinza(I,Ilin: byte):real;
    function Sim_Cinza_Media(i,j:integer) : real;
    procedure Kohonem;
    function selecionaVencedor(RGBCor: TVetorAtributos): TPos;
    function DistanciaKohonem(X,Y: TVetorAtributos_Norma):real;
    function hci(rc, ri: TPos; raio: real): real;
    procedure ShowKohonem;
    procedure Timer2Timer(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure Timer3Timer(Sender: TObject);
    procedure ShowAtributoSaliente;
    procedure Timer4Timer(Sender: TObject);
    procedure Timer5Timer(Sender: TObject);
    procedure mod_ImgExp4_SALMouseMove(Sender: TObject; Shift: TShiftState;
      X, Y: Integer);
    procedure cbIniClick(Sender: TObject);
    procedure cbSegClick(Sender: TObject);
    procedure Timer6Timer(Sender: TObject);
    procedure BitBtn36Click(Sender: TObject);
    function Similaridade_CorMediaColor(i,j,c:integer) : double;
    function Sim_Cor(vl,vc,hl,hc: integer):real;
    procedure FileListBox1Click(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure SpeedButton1Click(Sender: TObject);
    procedure FileListBox2Click(Sender: TObject);
    procedure mod_ImgExp4_IeFMouseMove(Sender: TObject; Shift: TShiftState;
      X, Y: Integer);
    procedure tbIChange(Sender: TObject);
    procedure tbRGChange(Sender: TObject);
    procedure tbBYChange(Sender: TObject);
    procedure tbXYChange(Sender: TObject);
    procedure tbRNChange(Sender: TObject);
    procedure tbSizeChange(Sender: TObject);
    procedure BitBtn2Click(Sender: TObject);
    procedure Connections;
    procedure Select;
    procedure ShowSincro_LEGION;
    procedure FormCreate(Sender: TObject);
    procedure TrackBarthetapChange(Sender: TObject);
    procedure TrackBarTetaxChange(Sender: TObject);
    procedure TrackBarTetaxzChange(Sender: TObject);
    procedure TrackBarWzChange(Sender: TObject);
    procedure VisualizarAtividadedesteNeurnio1Click(Sender: TObject);
    procedure SelecionarNeurnio011Click(Sender: TObject);
    procedure SelecionarNeurnio021Click(Sender: TObject);
    procedure SelecionarNeurnio031Click(Sender: TObject);
    procedure SelecionarNeurnio041Click(Sender: TObject);
    procedure LimparaSeleo1Click(Sender: TObject);
    function Sim_Cinza_LEG(I,Ilin: double):double;
    procedure TrackBarLimiarFundoChange(Sender: TObject);
    procedure AtualizarParametros(Sender: TObject);
    procedure ShowSincro_LEGION_no_Preview;
    procedure cb_StrechClick(Sender: TObject);
    procedure SpeedButton5Click(Sender: TObject);
    procedure tbLimiarFundoSegChange(Sender: TObject);
    procedure Atualiza_para_SOM;
    procedure mod_ImgExp4_SOM_SALMouseMove(Sender: TObject;
      Shift: TShiftState; X, Y: Integer);
    function Normalizar(v,min,max:integer) : double;
    procedure Timer7Timer(Sender: TObject);
    procedure cbV3DClick(Sender: TObject);
    procedure tb3dDimChange(Sender: TObject);
    procedure tbOrthoChange(Sender: TObject);
    procedure BitBtn1Click(Sender: TObject);
    function WeightsConnections(i,j:integer): real;
    procedure CBCamClick(Sender: TObject);
    procedure TimerCamTimer(Sender: TObject);
    procedure CBCP3DClick(Sender: TObject);
    procedure TrackBar3Change(Sender: TObject);
    procedure TrackBar4Change(Sender: TObject);
    function gaborkernel2d(var n:integer; lambda,sigma,theta,phi,gamma,bandwidth:double):TW;
    procedure BitBtn4Click(Sender: TObject);
    function gaussian2d(n:integer;sigma:double):TW;
    procedure BitBtn3Click(Sender: TObject);
    procedure interpolation(var O:TCMapArray;var D:TCMapArray;knl_t,knc_t:integer;f:double);
    procedure redimensionar(var O:TCMapArray;var D:TCMapArray;nl,nc,new_nl,new_nc:integer);
    procedure redimensionar_Image(var O:TImage;var D:TBitmap;nl,nc,new_nl,new_nc:integer);
    procedure subtraction(var O:TCMapArray;var D:TCMapArray;knl_t,knc_t:integer);
    procedure normalization(var D:TCMapArray;knl_t,knc_t:integer);
    procedure normalization_cm(var D:TCMapArray;knl_t,knc_t:integer);
    procedure subtractionColor(var R,O,D:TCMapArray;knl_t,knc_t:integer);
    procedure BitBtn5Click(Sender: TObject);
    procedure BitBtn6Click(Sender: TObject);
    procedure SpeedButton6Click(Sender: TObject);
    procedure SpeedButton7Click(Sender: TObject);
    procedure IttiModel(Sender: TObject);
    procedure Segmentation(Sender: TObject);
    procedure SegmentationLEGION(Sender: TObject);
    procedure SegmentationAlone(Sender: TObject);
    procedure CBAttentionClick(Sender: TObject);
    procedure Timer8Timer(Sender: TObject);
    procedure BitBtn7Click(Sender: TObject);
    procedure CarregaMLP;


  private
    { Private declarations }
  public
    { Public declarations }

  end;

var
  FormProjeto: TFormProjeto;
  knL : integer;
  knC : integer;
  TempoInicial,Intervalo,TempoFinal : integer;

  tmp : integer;

  sel_neuron_x, sel_neuron_y : array[0..3] of integer;

  Neuronios : TIeD; //Neuronios,SNeuronios : TIeD;
  BitmapS : TBitmap;
  iBMPsS : PRGB;

  // Visualização do Mapa SOM
     KNeuronios : ArrayNeuronios;
     BitmapK : TBitmap;
     iBMPsK : PRGB;
     som_i, som_j : integer;

  // Mapa de Atributo Saliente Atributo Saliencia
     BitmapAS : TBitmap;
     iBMPsAS : PRGB;
     rgbtmp_ant,rgbtmp : real;
     Sal,SalK : TPOs;
     dt_SAL,Winib : double;
     maxl, maxc, max_seg : integer;
     saliency, saliency_max : double;

  // Variaveis Locais para o SOM
     RGBCor : TVetorAtributos;
     neu : TPos;
     hci_f,calc : double;
     t_geral,nit: integer;
     fat_dec: double;
     raio, raio_ini,alfa, alfa_ini : double;
     vnc, centro : TPos;

  // Variáveis da Rede Neural
     BitmapRN : TBitmap;
     rna_sim : real;
     treina_som : boolean;

  // Variáveis LEGION
     thetae : double;
     thetaz : double;
     thetap : double;
     Wz : double;
     limiarF : double;
     LimiarFundoSeg : double;
     first_pulse : boolean;

  // Enviesamento Top-down do LEGION
     Wc,Wi,Wo,WSum : double;

     xinput : array[0..1000,0..1000] of double;
     s_neuron_x,s_neuron_y : integer;

  // Variáveis LEGION 1997
     jump : array[0..1] of integer;
     z : integer;

  // Parametros
    parametros : TMemo;
    nome_arq : string;
    semaforo : boolean;
    imin,imax,jmin,jmax : integer;

    SeG_Imagem : array of TImage;

  // MLP
     nl,nc : integer;

  // Modelo
     Segmentos : array of TSeg;
     Seg_Classify : array of TSeg_Classify;
     Seg_UP : integer;

  // Parâmetros
     vtbSize, vtbI, vtbRG, vtbBY, vtbO1, vtbO2, vtbO3, vtbO4,
     vtbXY, vtbRN  : integer;

  // Contador de Iterações
     cont_it : longint;

  // Array de Cores
     //VC : array[0..255] of array[0..2] of byte;
     //vc_cor : integer;
     c_r,c_g,c_b : byte;

   // Itti
    w : TW;
    wg : array[0..3] of TW;
    IM : TCMap0;
    Mi,Mr,Mg,Mb,My,M0,M1,M2,M3 : TCMap;
    Mrg,Mgr,Myb,Mby,Cm_i,Cm_c,Cm_o : TCMapArray;
    MCSi,MCSrg,MCSby,MCS0,MCS1,MCS2,MCS3 : TCMapCS;
    MCSrg_t,MCSby_t : TCMapCS;
    fmin_i,fmin_r,fmin_g,fmin_b,fmin_y,fmin_0,fmin_1,fmin_2,fmin_3: TCMap_Norm;
    fmax_i,fmax_r,fmax_g,fmax_b,fmax_y,fmax_0,fmax_1,fmax_2,fmax_3: TCMap_Norm;

   //Itti
   desv_P,total,wl,po,ar,i_lin,j_lin,band : double;
   Cor : TRGBDouble;
   theta : array[0..3] of double;
    
  Col,Row : integer;

implementation

uses UnitAtividade, UnitCam, UnitSeg;

{$R *.dfm}

procedure TFormProjeto.IniciarContagem;
begin
  StatusBar.Color := clGray;
  StatusBar.Panels[0].Text := 'Aguarde o processamento...';
  StatusBar.Refresh;
  TempoInicial := GetTickCount;
end;

procedure TFormProjeto.FinalizarContagem;
begin
  TempoFinal := GetTickCount;
  Intervalo := TempoFinal - TempoInicial;
  StatusBar.Color := clSilver;
  StatusBar.Panels[0].Text := 'Tempo de execução do último processo: '+ (inttostr(intervalo)) + ' milissegundos.';
end;

procedure TFormProjeto.Sair1Click(Sender: TObject);
begin
  Close;
end;

procedure TFormProjeto.mod_ImgExp4DblClick(Sender: TObject);
var
   img : string;
begin
   img := OpenPictureDialog1.FileName;
   OpenPictureDialog1.FileName := '';
   if OpenPictureDialog1.Execute then
   begin
      mod_ImgEXP4.Picture.LoadFromFile(OpenPictureDialog1.FileName);
   end
   else OpenPictureDialog1.FileName := img;
end;

procedure TFormProjeto.BitBtn32Click(Sender: TObject);
var
   caminho : string;
   s,b,knl_t,knc_t,i,j,RGCM_i : integer;
   Imagem : TImage;
   Bitmap : TBitmap;
   iBMPs : PRGB;
begin
   // Parando processo
   CBCam.Checked := false;

   if not(DirectoryExists(ExtractFilePath(Application.ExeName)+'\Experimento 7')) then
   begin
      CreateDir(ExtractFilePath(Application.ExeName)+'\Experimento 7');
   end;
   caminho :=  ExtractFilePath(Application.ExeName)+'\Experimento 7\'+formatdatetime('yyyymmdd hhmmss',now);
   CreateDir(caminho);

   // Dados Gerais
   mod_ImgExp4.Picture.SaveToFile(caminho+'\'+'sim_00_ENT.bmp');
   mod_ImgExp4_IeF.Picture.SaveToFile(caminho+  '\'+'sim_05_ImagSeg.bmp');
   mod_ImgExp4_SOM_SAL.Picture.SaveToFile(caminho+  '\'+'sim_06_MOS.bmp');
   mod_ImgExp4_SAL.Picture.SaveToFile(caminho+  '\'+'sim_07_LocObjSal.bmp');
   for s := 0 to length(Seg_Imagem)-1 do
   begin
     Seg_Imagem[s].Picture.SaveToFile(caminho+  '\'+'sim_08_Segmento'+inttostr(s)+'.bmp');
   end;
   CSal.SaveToBitmapFile(caminho+  '\'+'sim_09_GrafSal.bmp');
   CO.SaveToBitmapFile(caminho+  '\'+'sim_10_GrafSal.bmp');
   COII.SaveToBitmapFile(caminho+  '\'+'sim_11_GrafSal.bmp');
   ChartCP.SaveToBitmapFile(caminho+  '\'+'sim_12_ConspMapsWeights.bmp');
   ChartOBF.SaveToBitmapFile(caminho+  '\'+'sim_13_ObjectBasedWeigths.bmp');

  // ITTI
  // Apresentação da Interpolação ----------------------------------------------
  CreateDir(Caminho+'\Centro_Surround');
  try
  Bitmap := TBitmap.Create;
  RGCM_i := 1;
  repeat
    b:=0;
    knl_t := trunc((knl/2));
    knc_t := trunc((knc/2));
    repeat
     Bitmap.PixelFormat := pf24bit;
     Bitmap.Width := knc_t;
     Bitmap.Height := knl_t;
     for i := 0 to knl_t-1 do
     begin
       iBMPs := Bitmap.ScanLine[i];
       for j := 0 to knc_t-1 do
       begin
          case RGCM_i of
          0 : begin
                iBMPs[j].r := trunc(MCSi[b][i,j]);
                iBMPs[j].g := trunc(MCSi[b][i,j]);
                iBMPs[j].b := trunc(MCSi[b][i,j]);
             end;
          1 : begin
                iBMPs[j].r := trunc(MCSrg[b][i,j]);
                iBMPs[j].g := trunc(MCSrg[b][i,j]);
                iBMPs[j].b := trunc(MCSrg[b][i,j]);
             end;
          2 : begin
                iBMPs[j].r := trunc(MCSby[b][i,j]);
                iBMPs[j].g := trunc(MCSby[b][i,j]);
                iBMPs[j].b := trunc(MCSby[b][i,j]);
             end;
          3 : begin
                iBMPs[j].r := trunc(MCS0[b][i,j]);
                iBMPs[j].g := trunc(MCS0[b][i,j]);
                iBMPs[j].b := trunc(MCS0[b][i,j]);
             end;
          4 : begin
                iBMPs[j].r := trunc(MCS1[b][i,j]);
                iBMPs[j].g := trunc(MCS1[b][i,j]);
                iBMPs[j].b := trunc(MCS1[b][i,j]);
             end;
          5 : begin
                iBMPs[j].r := trunc(MCS2[b][i,j]);
                iBMPs[j].g := trunc(MCS2[b][i,j]);
                iBMPs[j].b := trunc(MCS2[b][i,j]);
             end;
          6 : begin
                iBMPs[j].r := trunc(MCS3[b][i,j]);
                iBMPs[j].g := trunc(MCS3[b][i,j]);
                iBMPs[j].b := trunc(MCS3[b][i,j]);
             end;
          end;// case
       end;
     end;

     // Salvando Imagem
     case RGCM_i of
        1 : Bitmap.SaveToFile(Caminho+'\Centro_Surround'+'\'+'CS_RG'+inttostr(b)+'.bmp');
        2 : Bitmap.SaveToFile(Caminho+'\Centro_Surround'+'\'+'CS_BY'+inttostr(b)+'.bmp');
     end;
     inc(b);
     if (b mod 2 = 0) then
     begin
        knl_t := trunc(knl_t/2);
        knc_t := trunc(knc_t/2);
     end;
    until b = 6;
    inc(RGCM_i);
  until RGCM_i = 3
  finally
     Bitmap.Free;
  end;

  // Apresentação dos Mapas de Conspicuidades-----------------------------------
  CreateDir(Caminho+'\ConspMpaFinal');
  Bitmap := TBitmap.Create;
  try
    b:=1;
    repeat
     Bitmap.PixelFormat := pf24bit;
     Bitmap.Width := knc;
     Bitmap.Height := knl;
     for i := 0 to knl-1 do
     begin
       iBMPs := Bitmap.ScanLine[i];
       for j := 0 to knc-1 do
       begin
          case b of
          0 : begin
                iBMPs[j].r := trunc(Cm_i[i,j]);
                iBMPs[j].g := trunc(Cm_i[i,j]);
                iBMPs[j].b := trunc(Cm_i[i,j]);
             end;
          1 : begin
                iBMPs[j].r := trunc(Cm_c[i,j]);
                iBMPs[j].g := trunc(Cm_c[i,j]);
                iBMPs[j].b := trunc(Cm_c[i,j]);
             end;
          2 : begin
                iBMPs[j].r := trunc(Cm_o[i,j]);
                iBMPs[j].g := trunc(Cm_o[i,j]);
                iBMPs[j].b := trunc(Cm_o[i,j]);
             end;
          end;// case
       end;
     end;

     // Mostrando Imagem
     case b of
       0 : Bitmap.SaveToFile(Caminho+'\ConspMpaFinal'+'\'+'CM_Intensity'+inttostr(b)+'.bmp');
       1 : Bitmap.SaveToFile(Caminho+'\ConspMpaFinal'+'\'+'CM_Color'+inttostr(b)+'.bmp');
       2 : Bitmap.SaveToFile(Caminho+'\ConspMpaFinal'+'\'+'CM_Orientation'+inttostr(b)+'.bmp');
     end;
     inc(b);
    until b = 2;
  finally
     Bitmap.Free;
  end;

  //=========================================

  // Apresentação da Pirâmide --------------------------------------------------
  b := 0;
  knl_t := trunc(knl);
  knc_t := trunc(knc);
  repeat
     normalization(Mr[b],knl_t,knc_t);
     normalization(Mg[b],knl_t,knc_t);
     normalization(Mb[b],knl_t,knc_t);
     normalization(My[b],knl_t,knc_t);
    inc(b);
     knl_t := trunc(knl_t/2);
     knc_t := trunc(knc_t/2);
  until b = 9;
  

  CreateDir(Caminho+'\Piramide');
  Bitmap := TBitmap.Create;
  try
  RGCM_i := 1;
  repeat
    b:=0;
    knl_t := trunc(knl);
    knc_t := trunc(knc);
    repeat
     Bitmap.PixelFormat := pf24bit;
     Bitmap.Width := knc_t;
     Bitmap.Height := knl_t;
     for i := 0 to knl_t-1 do
     begin
       iBMPs := Bitmap.ScanLine[i];
       for j := 0 to knc_t-1 do
       begin
          case RGCM_i of
          0 : begin
                iBMPs[j].r := trunc(Mi[b][i,j]);
                iBMPs[j].g := trunc(Mi[b][i,j]);
                iBMPs[j].b := trunc(Mi[b][i,j]);
             end;
          1 : begin
                iBMPs[j].r := trunc(Mr[b][i,j]);
                iBMPs[j].g := trunc(Mr[b][i,j]);
                iBMPs[j].b := trunc(Mr[b][i,j]);
             end;
          2: begin
                iBMPs[j].r := trunc(Mg[b][i,j]);
                iBMPs[j].g := trunc(Mg[b][i,j]);
                iBMPs[j].b := trunc(Mg[b][i,j]);
             end;
          3 : begin
                iBMPs[j].r := trunc(Mb[b][i,j]);
                iBMPs[j].g := trunc(Mb[b][i,j]);
                iBMPs[j].b := trunc(Mb[b][i,j]);
             end;
          4 : begin
                iBMPs[j].r := trunc(My[b][i,j]);
                iBMPs[j].g := trunc(My[b][i,j]);
                iBMPs[j].b := trunc(My[b][i,j]);
             end;
          5 : begin
                iBMPs[j].r := trunc(M0[b][i,j]);
                iBMPs[j].g := trunc(M0[b][i,j]);
                iBMPs[j].b := trunc(M0[b][i,j]);
             end;
          6 : begin
                iBMPs[j].r := trunc(M1[b][i,j]);
                iBMPs[j].g := trunc(M1[b][i,j]);
                iBMPs[j].b := trunc(M1[b][i,j]);
             end;
          7 : begin
                iBMPs[j].r := trunc(M2[b][i,j]);
                iBMPs[j].g := trunc(M2[b][i,j]);
                iBMPs[j].b := trunc(M2[b][i,j]);
             end;
          8 : begin
                iBMPs[j].r := trunc(M3[b][i,j]);
                iBMPs[j].g := trunc(M3[b][i,j]);
                iBMPs[j].b := trunc(M3[b][i,j]);
             end;
          end;// case
       end;
     end;

     // Mostrando Imagem
     case RGCM_i of
       1 : Bitmap.SaveToFile(Caminho+'\Piramide'+'\'+'Py01_R'+inttostr(b)+'.bmp');
       2 : Bitmap.SaveToFile(Caminho+'\Piramide'+'\'+'Py02_G'+inttostr(b)+'.bmp');
       3 : Bitmap.SaveToFile(Caminho+'\Piramide'+'\'+'Py03_B'+inttostr(b)+'.bmp');
       4 : Bitmap.SaveToFile(Caminho+'\Piramide'+'\'+'Py04_Y'+inttostr(b)+'.bmp');
     end;
     inc(b);
     knl_t := trunc(knl_t/2);
     knc_t := trunc(knc_t/2);
    until b = 9;
    inc(RGCM_i);
  until RGCM_i = 5;
  finally
     Bitmap.Free;
  end;


end;



procedure TFormProjeto.Zoom(Sender: TObject);
begin
   if Sender is TImage then
      with Sender as TImage do
      begin
         FormZoom := TFormZoom.create ( application );
         try
             FormZoom.Height := TImage(Sender).Picture.Bitmap.Height;
             FormZoom.Width := TImage(Sender).Picture.Bitmap.Width;
             FormZoom.mod_ImgZoom.Picture.Graphic := TImage(Sender).Picture.Graphic;
             FormZoom.Show;
         except
             //FormZoom.Release;
             //FormZoom := nil;
         end;
      end;
end;

procedure TFormProjeto.ShowSincro_LEGION_no_Preview;
var
  i,j : integer;
  corR,corG,corB : integer;
begin
  // Apresentação dos Neuronios Inicializados-----------------------------------
  try

     // capturan objeto pulsante
     imin := knl; imax := -1; jmin := knc; jmax := -1;

     if (cbExp4_Cores.Checked) then //-----------Segmentação em Cores Aleatórias
     begin
        corR := randomrange(0,255);
        corG := randomrange(0,255);
        corB := randomrange(0,255);

        for i := 0 to knl-1 do
          for j := 0 to knc-1 do
             if (Neuronios[i,j].Lx[1] >= thetae) and (Neuronios[i,j].d = 1) then
             begin
                corR := Neuronios[i,j].Seg_rgb[0];
                corG := Neuronios[i,j].Seg_rgb[1];
                corB := Neuronios[i,j].Seg_rgb[2];
                continue;
             end;

        for i := 0 to knl-1 do
          for j := 0 to knc-1 do
             if (Neuronios[i,j].Lx[1] >= thetae) and (Neuronios[i,j].d = 0) then
             begin
                Neuronios[i,j].Seg_rgb[0] := CorR;
                Neuronios[i,j].Seg_rgb[1] := CorG;
                Neuronios[i,j].Seg_rgb[2] := CorB;
                Neuronios[i,j].d := 1;
             end;
     end
     else begin
        for i := 0 to knl-1 do
          for j := 0 to knc-1 do
             if (Neuronios[i,j].Lx[1] >= thetae) and (Neuronios[i,j].d = 0) then
                Neuronios[i,j].d := 1;

     end;

     for i := 0 to knl-1 do
       for j := 0 to knc-1 do
         if (Neuronios[i,j].Lx[1] >= thetae) then
         begin
            // Criando Imagem Segmentada Individual
            if imin > i then imin := i;
            if jmin > j then jmin := j;
            if imax < i then imax := i;
            if jmax < j then jmax := j;
         end;

     // Ajustando Bordas...
     if imin > 0 then dec(imin);
     if jmin > 0 then dec(jmin);
     if imax < knl-1 then inc(imax);
     if jmax < knc-1 then inc(jmax);

  finally
  end;
end;


procedure TFormProjeto.ShowSincro_LEGION;
var
  i,j : integer;
  pixelI : byte;

  k, n: integer;
  Valor, Maior: double;
  IMaior: integer;

  corR,corG,corB : integer;

  BitmapSeg : TBitmap;
  iBMPsSeg : PRGB;

begin

  // Apresentação dos Neuronios Inicializados-----------------------------------
  try
     // capturan objeto pulsante
     imin := knl; imax := -1; jmin := knc; jmax := -1;

     if (cbExp4_Cores.Checked) then //-----------Segmentação em Cores Aleatórias
     begin
        corR := randomrange(0,255);
        corG := randomrange(0,255);
        corB := randomrange(0,255);

        for i := 0 to knl-1 do
          for j := 0 to knc-1 do
             if (Neuronios[i,j].Lx[1] >= thetae) and (Neuronios[i,j].d = 1) then
             begin
                corR := Neuronios[i,j].Seg_rgb[0];
                corG := Neuronios[i,j].Seg_rgb[1];
                corB := Neuronios[i,j].Seg_rgb[2];
                continue;
             end;

        for i := 0 to knl-1 do
          for j := 0 to knc-1 do
             if (Neuronios[i,j].Lx[1] >= thetae) and (Neuronios[i,j].d = 0) then
             begin
                Neuronios[i,j].Seg_rgb[0] := CorR;
                Neuronios[i,j].Seg_rgb[1] := CorG;
                Neuronios[i,j].Seg_rgb[2] := CorB;
                Neuronios[i,j].d := 1;
             end;
     end
     else begin
        for i := 0 to knl-1 do
          for j := 0 to knc-1 do
             if (Neuronios[i,j].Lx[1] >= thetae) and (Neuronios[i,j].d = 0) then
                 Neuronios[i,j].d := 1;

     end;

     for i := 0 to knl-1 do
     begin
       iBMPsS :=  BitmapS.ScanLine[i];
       for j := 0 to knc-1 do
       begin

         if (Neuronios[i,j].Lx[1] >= thetae)  then
         begin

            if cbExp4_Cinza.Checked then
            begin
               iBMPsS[j].r := neuronios[i,j].rgb[4];
               iBMPsS[j].g := neuronios[i,j].rgb[4];
               iBMPsS[j].b := neuronios[i,j].rgb[4];
            end
            else
               if cbExp4_Cores.Checked then
               begin
                  if Neuronios[i,j].d = 0 then
                  begin
                     Neuronios[i,j].Seg_rgb[0] := corR;
                     Neuronios[i,j].Seg_rgb[1] := corG;
                     Neuronios[i,j].Seg_rgb[2] := corB;
                     Neuronios[i,j].d := 1;
                  end;
                  iBMPsS[j].r := trunc(Neuronios[i,j].Seg_rgb[0]);
                  iBMPsS[j].g := trunc(Neuronios[i,j].Seg_rgb[1]);
                  iBMPsS[j].b := trunc(Neuronios[i,j].Seg_rgb[2]);
               end
               else
               begin
                  iBMPsS[j].r := trunc(Neuronios[i,j].lx[1]*255);
                  iBMPsS[j].g := trunc(Neuronios[i,j].lx[1]*255);
                  iBMPsS[j].b := trunc(Neuronios[i,j].lx[1]*255);
               end;

            // Criando Imagem Segmentada Individual
            if imin > i then imin := i;
            if jmin > j then jmin := j;
            if imax < i then imax := i;
            if jmax < j then jmax := j;

         end
         else begin
            if (cbExp4_MP.Checked) then
            begin
               iBMPsS[j].r := 0;
               iBMPsS[j].g := 0;
               iBMPsS[j].b := 0;
            end;
         end;

       end;
     end;

     // Ajustando Bordas...
     if imin > 0 then dec(imin);
     if jmin > 0 then dec(jmin);
     if imax < knl-1 then inc(imax);
     if jmax < knc-1 then inc(jmax);

     //Mostrando Cantos
     if (CBBorda.Checked) then
     begin

       if (imin <> -1) and (imax <> -1) then
       begin
          for i:=imin to imax do
          begin
            iBMPsS :=  BitmapS.ScanLine[i];
            iBMPsS[jmin].r := GetRValue(ColorGridSeg.ForegroundColor);    //TColor(RGB(KNeuronios[i,j][1],KNeuronios[i,j][2],KNeuronios[i,j][3]))
            iBMPsS[jmin].g := GetGValue(ColorGridSeg.ForegroundColor);
            iBMPsS[jmin].b := GetBValue(ColorGridSeg.ForegroundColor);

            iBMPsS[jmax].r := GetRValue(ColorGridSeg.ForegroundColor);
            iBMPsS[jmax].g := GetGValue(ColorGridSeg.ForegroundColor);
            iBMPsS[jmax].b := GetBValue(ColorGridSeg.ForegroundColor);
          end;

          for j:=jmin to jmax do
          begin
            iBMPsS :=  BitmapS.ScanLine[imin];
            iBMPsS[j].r := GetRValue(ColorGridSeg.ForegroundColor);;
            iBMPsS[j].g := GetGValue(ColorGridSeg.ForegroundColor);;
            iBMPsS[j].b := GetBValue(ColorGridSeg.ForegroundColor);

            iBMPsS :=  BitmapS.ScanLine[imax];
            iBMPsS[j].r := GetRValue(ColorGridSeg.ForegroundColor);
            iBMPsS[j].g := GetGValue(ColorGridSeg.ForegroundColor);
            iBMPsS[j].b := GetBValue(ColorGridSeg.ForegroundColor);
          end;

       end;

     end;

     // Mostrando Imagem
     mod_ImgExp4_IeF.Picture.Graphic := BitmapS;

     // Mostrando Segmento
     if (cbExp4_MP.Checked) then
     begin

      BitmapSEG := TBitmap.Create;
      try
         BitmapSEG.PixelFormat := pf24bit;
         BitmapSEG.Width := jmax-jmin+1;
         BitmapSEG.Height := imax-imin+1;
         for i := imin to imax do
         begin
           iBMPsSEG := BitmapSEG.ScanLine[i-imin];
           for j := jmin to jmax do
           begin
              if Neuronios[i,j].Lx[1] >= thetae then
                if cbExp4_Cinza.Checked then
                begin
                  iBMPsSEG[j-jmin].r := neuronios[i,j].rgb[4];
                  iBMPsSEG[j-jmin].g := neuronios[i,j].rgb[4];
                  iBMPsSEG[j-jmin].b := neuronios[i,j].rgb[4];
                end
                else
                   if cbExp4_Cores.Checked then
                   begin
                      iBMPsSEG[j-jmin].r := trunc(Neuronios[i,j].Seg_rgb[0]);
                      iBMPsSEG[j-jmin].g := trunc(Neuronios[i,j].Seg_rgb[1]);
                      iBMPsSEG[j-jmin].b := trunc(Neuronios[i,j].Seg_rgb[2]);
                   end
                   else
                   begin
                      iBMPsSEG[j-jmin].r := trunc(Neuronios[i,j].lx[1]*255);
                      iBMPsSEG[j-jmin].g := trunc(Neuronios[i,j].lx[1]*255);
                      iBMPsSEG[j-jmin].b := trunc(Neuronios[i,j].lx[1]*255);
                   end
               else begin
                  iBMPsSEG[j-jmin].r := 0;
                  iBMPsSEG[j-jmin].g := 0;
                  iBMPsSEG[j-jmin].b := 0;
               end;
           end;
         end;
         // Mostrando Imagem
         img_Segmentos.Picture.Graphic := BitmapSEG;
         img_OBJ_Pulsante_02.Picture.Graphic := BitmapSEG;
       finally
          BitmapSEG.Destroy;
       end;

     end;

  finally
  end;
end;

(*
procedure TFormProjeto.ShowSincro;
var
  i,j : integer;
  pixelI : byte;
  dist_i, dist_j : integer;

  k, n: integer;
  Valor, Maior: double;
  IMaior: integer;
begin
  // Apresentação dos Neuronios Inicializados-----------------------------------
  try
     dist_i := -1;
     dist_j := -1;
     // Limpando Imagem a ser avaliada na RN
     BitmapRN := TBitmap.Create;
     BitmapRN.PixelFormat := pf24bit;
     BitmapRN.Width := 16;
     BitmapRN.Height := 16;


     for i := 0 to knl-1 do
     begin
       iBMPsS :=  BitmapS.ScanLine[i];
       for j := 0 to knc-1 do
       begin
         if (Neuronios[i,j].p = 1) then
         begin

            if cbExp4_Cinza.Checked then
            begin
               pixelI := (trunc(Neuronios[i,j].Seg_rgb[0])+trunc(Neuronios[i,j].Seg_rgb[1])+trunc(Neuronios[i,j].Seg_rgb[2])) div 3;
               iBMPsS[j].r := pixelI;
               iBMPsS[j].g := pixelI;
               iBMPsS[j].b := pixelI;
            end else
            begin
               iBMPsS[j].r := trunc(Neuronios[i,j].Seg_rgb[0]);
               iBMPsS[j].g := trunc(Neuronios[i,j].Seg_rgb[1]);
               iBMPsS[j].b := trunc(Neuronios[i,j].Seg_rgb[2]);
            end;

            // Mostrando Objeto Reconhecido em Cor Diferente
            if Neuronios[i,j].rn = 255 then
            begin
               iBMPsS[j].r := 255;
               iBMPsS[j].g := 255;
               iBMPsS[j].b := 255;
            end;

            // Atualizando distancia da borda para gerar figura 16x16
            if (dist_i = -1) and (dist_j = -1) then
            begin
               dist_i := i;
               dist_j := j;
            end;

            // Objeto a ser avaliado pela Rede Neural
            bitmapRN.Canvas.Pixels[j-dist_j+2,i-dist_i+2] := clBlack;

         end else
         begin
            if cbExp4_MP.Checked then
            begin
               iBMPsS[j].r := 0;
               iBMPsS[j].g := 0;
               iBMPsS[j].b := 0;
            end;
         end;


       end;
     end;
     // Mostrando Imagem
     mod_ImgExp4_IeF.Picture.Graphic := BitmapS;

     // Verificando a qual classe o objeto pertence
     //img_OBJ_Pulsante_01.Picture.Graphic := BitmapRN;
     //img_OBJ_Pulsante_02.Picture.Graphic := BitmapRN;

     { Alimenta a rede com os valores dos Ocelos de visão }
     for i := 0 to 15 do
        for j := 0 to 15 do
        begin
           n := i*16+j+1; // índice do neurônio de entrada
           MLP.SetInput(n,img_OBJ_Pulsante_02.Picture.Bitmap.Canvas.Pixels[i,j] shr 23); // 0 ou 1
        end;

     MLP.Test;

     mod_ed0.Text := Format('%2.4f',[MLP.GetOutput(1)]);
     mod_ed1.Text := Format('%2.4f',[MLP.GetOutput(2)]);
     mod_ed2.Text := Format('%2.4f',[MLP.GetOutput(3)]);
     mod_ed3.Text := Format('%2.4f',[MLP.GetOutput(4)]);

     { Verifica a maior saída para indicação visual }
     Maior := -10;
     for i := 1 to snClasses.Value do
     begin
        Valor := MLP.GetOutput(i);
        if Valor > Maior then
        begin
           Maior := Valor;
           IMaior := i;
        end;
     end;

     { Todas com fundo branco }
     mod_sp0.Brush.Color := clWhite;
     mod_sp1.Brush.Color := clWhite;
     mod_sp2.Brush.Color := clWhite;
     mod_sp3.Brush.Color := clWhite;

     { Se semelhança for maior que 0.95}
     if Maior > 0.95 then
     begin
        { Exceto a maior }
        case IMaior of
           1:  mod_sp0.Brush.Color := clMoneyGreen;
           2:  mod_sp1.Brush.Color := clMoneyGreen;
           3:  mod_sp2.Brush.Color := clMoneyGreen;
           4:  mod_sp3.Brush.Color := clMoneyGreen;
        end;

        { Alimentando neurônios reconhecidos pela MLP}
        for i := 0 to 15 do
           for j := 0 to 15 do
           begin
               if (Neuronios[i+dist_i-2,j+dist_j-2].p = 1) then
                   Neuronios[i+dist_i-2,j+dist_j-2].rn := 255;
           end;

     end;

     BitMapRN.Destroy;
  finally
  end;
end;
*)

// Método Alterado - Retorna o Valor do último Vizinho...
function TFormProjeto.Similaridade_CorMediaColor(i,j,c:integer) : double;
var
  s,t : integer;
  media : double;
begin
  media := 0;
    for s := -1 to 1 do
       for t := -1 to 1 do
          if ((i+s >=0) and (i+s <= knL-1)) and ((j+t >= 0) and (j+t <= knC-1)) then
              media := media + neuronios[i+s,j+t].rgb[c];

    //media := media/9;
    Similaridade_CorMediaColor := media;
end;

// Método Alterado - Retorna o Valor do último Vizinho...
function TFormProjeto.Similaridade_CorMedia(i,j,c:integer) : double;
var
  s,t : integer;
  media : double;
begin
  media := 0;
    for s := -1 to 1 do
       for t := -1 to 1 do
          if ((i+s >=0) and (i+s <= knL-1)) and ((j+t >= 0) and (j+t <= knC-1)) then
              media := neuronios[i+s,j+t].rgb[c];
              //media := media + neuronios[i+s,j+t].rgb[c];

    //media := media/9;
    Similaridade_CorMedia := media;
end;


function TFormProjeto.Sim_Cinza_Media(i,j:integer) : real;
var
  s,t : integer;
  media : double;
begin
  media := 0;
    for s := -1 to 1 do
       for t := -1 to 1 do
          if ((i+s >=0) and (i+s <= knL-1)) and ((j+t >= 0) and (j+t <= knC-1)) then
              media := media + (255/(1+abs(neuronios[i,j].rgb[4]-neuronios[i+s,j+t].rgb[4])));;

    media := media/9;
    Sim_Cinza_Media := media;
end;

//------------------------------------------------------------------------------
// Função de Similaridade para Tons de Cinza - LEGION
function TFormProjeto.Sim_Cinza_LEG(I,Ilin: double):double;
var
  R:double;
begin
    R:=0;
    R := 1/(1+abs(I-Ilin));
    Sim_Cinza_LEG := R;
end;
//------------------------------------------------------------------------------
// Função de Similaridade para Tons de Cinza
function TFormProjeto.Sim_Cinza(I,Ilin: byte):real;
var
  R:real;
begin
    R:=0;
    R := GRAY/(1+abs(I-Ilin));
    Sim_Cinza := R;
end;

//------------------------------------------------------------------------------
// Função de Similaridade para Cores
function TFormProjeto.Sim_Cor(vl,vc,hl,hc: integer):real;
var
  R:real;
  i : integer;
begin
    R:=0;
    for i := 0 to 2 do
        R := R + abs(neuronios[vl,vc].RGBSColor[i]-neuronios[hl,hc].RGBSColor[i]);
    R := 255/(1+R);
    Sim_Cor := R;
end;


//------------------------------------------------------------------------------
// Função para o Cálcula da Distância Euclidiana
{function TFormProjeto.DistanciaKohonem(X,Y: TVetorAtributos):real;
var
  r,max_Dist:double;
  dI,dRG,dBY,dXY,dO1,dO2,dO3,dO4,dRN,dRGB : double;
begin
    r := 0;

//    if treina_som = false then
    begin
    // Cor I 0 menos simular e 1 mais similar
      dI := sim_Cinza(x[0],y[0]);
      dI := dI/255;

    // RG
      dRG := sim_Cinza(X[1],Y[1]);
      dRG := dRG/255;

    // BY
      dBY := sim_Cinza(X[2],Y[2]);
      dBY := dBY/255;

    // Posiçao XY 0 menos simular e 1 mais similar
      max_Dist := sqrt(sqr(0-knl-1)+sqr(0-knc-1));
      dXY := sqrt(sqr(x[3]-y[3])+sqr(x[4]-y[4]));
      dXY := dXY/max_Dist;
      dXY := 1-dXY;

    //Angulo
      dO1:=sim_Cinza(X[5],Y[5])/255;
      dO2:=sim_Cinza(X[6],Y[6])/255;
      dO3:=sim_Cinza(X[7],Y[7])/255;
      dO4:=sim_Cinza(X[8],Y[8])/255;

    //Simularidade
      dRN:=sim_Cinza(X[9],Y[9])/255;

    //R,G,B
      dRGB := abs(x[10]-y[10]) + abs(x[11]-y[11]) + abs(x[12]-y[12]) ;
      dRGB := 255/(1+dRGB);
      dRGB := dRGB/255;


      R := ((dI+dRG+dBY+dXY+dO1+dO2+dO3+dO4+dRN+dRGB)/10)*255;
//    end else
//    begin
    end;

    if r<0 then r:=0;
    DistanciaKohonem:=(R);
end;
}



function TFormProjeto.DistanciaKohonem(X,Y: TVetorAtributos_Norma):real;
var
  R:real;
begin
    R:=0;

    // Cor I, Color, Orientação
      R:=R+(sqr(X[0]-Y[0])*vtbI/10);
      R:=R+(sqr(X[1]-Y[1])*vtbRG/10);
      R:=R+(sqr(X[2]-Y[2])*vtbBY/10);
    // Posiçao XY
      R:=R+(sqr(X[3]-Y[3])*vtbXY/10);
      R:=R+(sqr(X[4]-Y[4])*vtbXY/10);
    //Simularidade
      R:=R+(sqr(X[9]-Y[9])*vtbRN/10);
    //Size
      R:=R+(sqr(X[10]-Y[10])*vtbSize/10);

    if r<0 then r:=0;
    DistanciaKohonem:=sqrt(R);
end;



// Calcula Distância Euclidiana entre todos os Neurônio e retorna vencedor
function TFormProjeto.selecionaVencedor(RGBCor: TVetorAtributos): TPos;
begin
end;

// Vizinhos mais próximos
function TFormProjeto.hci(rc, ri: TPos; raio: real): real;
begin
   hci := exp(-1*((sqr(rc.l-ri.l)+sqr(rc.c-ri.c))/(2*(raio))));
end;

procedure TFormProjeto.ShowKohonem;
begin

end;

//procedure TFormProjeto.Kohonen(som_i,som_j:integer);
procedure TFormProjeto.Kohonem;
var
  RGBCor : TVetorAtributos_Norma;
  i,j : integer;
  Seg_valido : boolean;
  dist : double;
begin
    // Atribuindo as Caracteríscticas
    RGBCor := Segmentos[seg_UP].SegMedias;
    {
    Seg_valido := false;
    for i := 0 to 2 do
    begin
       if Segmentos[seg_UP].SegMedias[i] > 0 then Seg_valido := true;
    end;
    if Seg_valido = false then exit;
    }

    // Pulsando o Neuronio no Mapa de Atributo Saliente
    for i := 0 to length(Segmentos)-1 do
    begin

       if (Segmentos[i].segValido) then //and (i <> seg_UP)then
       begin
                  // conforme aumenta, aumenta o exp
             dist := exp(-DistanciaKohonem(RGBCor,Segmentos[i].SegMedias));
             
          if (Segmentos[i].IeD.i < dist) then
          begin
             Segmentos[i].IeD.i := dist;
          end;
          Segmentos[i].IeD.y := dist;


       end;{
       else if (Segmentos[i].segValido) and (i = seg_UP)then
       begin
          Segmentos[i].IeD.i := 1;
          Segmentos[i].IeD.y := 0;
       end;}

    end;

end;

procedure TFormProjeto.Timer2Timer(Sender: TObject);
begin
   //ShowKohonem;
end;

procedure TFormProjeto.Timer1Timer(Sender: TObject);
begin
  Kohonem;
end;

procedure TFormProjeto.ShowAtributoSaliente;
var
  s,i,j : integer;

begin
  // LOCAL: Limpando a Imagem em que será Destacado o Objeto Saliente
  try
     for i := 0 to knl-1 do
     begin
       iBMPsAS :=  BitmapAS.ScanLine[i];
       for j := 0 to knc-1 do
       begin
         iBMPsAS[j].r := 0;
         iBMPsAS[j].g := 0;
         iBMPsAS[j].b := 0;

         // Atualizando Gráfico
         //CSal.Series[0].
          //CSal.Series[0].YValue[(j*knc)+i] := 0;

       end;
     end;
     // Mostrando Imagem
     //mod_ImgExp4_SAL.Picture.Graphic := Bitmap;
     mod_ImgExp4_SOM_SAL.Picture.Graphic := BitmapAS;
  finally
  end;

  // Limpando Vetor de Segmentos para Classificação

  // Apresentação dos Neuronios Inicializados-----------------------------------
  try
     saliency := 0; maxl := -1; maxc := -1;
     for s := 0 to length(Segmentos)-1 do
         for i := Segmentos[s].imin to Segmentos[s].imax do
         begin
             iBMPsAS :=  BitmapAS.ScanLine[i];
             for j := Segmentos[s].jmin to Segmentos[s].jmax do
             begin
                  if (Segmentos[s].segValido = true) and 
                     (Segmentos[s].SegNeuronios[i-Segmentos[s].imin,j-Segmentos[s].jmin].Lx[1] >= thetae) and
                     (Segmentos[s].SegNeuronios[i-Segmentos[s].imin,j-Segmentos[s].jmin].d = 1) then
                  begin

                    iBMPsAS[j].r := trunc(Segmentos[s].IeD.v*255);
                    iBMPsAS[j].g := trunc(Segmentos[s].IeD.v*255);
                    iBMPsAS[j].b := trunc(Segmentos[s].IeD.v*255);

                    if saliency < Segmentos[s].IeD.v*255 then
                    begin
                       saliency := Segmentos[s].IeD.v*255;
                       maxl := i; maxc := j;
                       max_seg := s;

                       if saliency_max < saliency then
                       begin
                          saliency_max := saliency;
                       end;

                       if Segmentos[s].IeD.v > Segmentos[s].v_Max then
                       begin
                          Segmentos[s].v_Max := Segmentos[s].IeD.v;
                       end;
                    end;

                  end;{ else
                  begin
                    iBMPsAS[j].r := 0;
                    iBMPsAS[j].g := 0;
                    iBMPsAS[j].b := 0;
                  end;} // if
             end; // j
         end; // i

      // Atribuindo Classificacao
      if max_seg >= 0 then Segmentos[max_seg].Classify := Segmentos[max_seg].Classify + 1;

     (*
     // Mostrando Imagem
     if (maxl <> -1) and (maxc <> -1) then
     begin

        // Vizualização de Informação
        {
           label25.Caption := '['+inttostr(maxl)+','+inttostr(maxc)+']';
           label27.Caption := 'I  = [' + inttostr(KNeuronios[maxl,maxc][0]) + ']';
           label28.Caption := 'RB = [' + inttostr(KNeuronios[maxl,maxc][1]) + ']';
           label29.Caption := 'BY = [' + inttostr(KNeuronios[maxl,maxc][2]) + ']';

           label36.Caption := 'X = [' + inttostr(KNeuronios[maxl,maxc][3]) + ']';
           label37.Caption := 'Y = [' + inttostr(KNeuronios[maxl,maxc][4]) + ']';

           label31.Caption := '0   = [' + inttostr(KNeuronios[maxl,maxc][5]) + ']';
           label32.Caption := '90  = [' + inttostr(KNeuronios[maxl,maxc][6]) + ']';
           label33.Caption := '45 = [' + inttostr(KNeuronios[maxl,maxc][7]) + ']';
           label34.Caption := '135 = [' + inttostr(KNeuronios[maxl,maxc][8]) + ']';

           label40.Caption := 'i = [' + formatfloat('0.00',SNeuronios[maxl,maxc].i) + ']';
           label41.Caption := 'v = [' + formatfloat('0.00',SNeuronios[maxl,maxc].v) + ']';
           label42.Caption := 'y = [' + formatfloat('0.00',SNeuronios[maxl,maxc].y) + ']';
          }

        //BitmapAS.Canvas.Ellipse(salk.c-9,salk.l-9,salk.c+9,salk.l+9);
        //BitmapAS.Canvas.Pen.Color := clgreen;
        //BitmapAS.Canvas.LineTo(salk.c-6,salk.c+10);
        //BitmapAS.Canvas.Arc(salk.c-6,salk.l-6,salk.c+10,salk.l+10,0,0,0,0);

        iBMPsAS := BitmapAS.ScanLine[maxl];
        iBMPsAS[maxc].r := 0;
        iBMPsAS[maxc].g := 255;
        iBMPsAS[maxc].b := 0;
      end;

     // desenhando mira
     if (maxl-3 >= 0) then
     begin
         iBMPsAS := BitmapAS.ScanLine[maxl-3];
         iBMPsAS[maxc].r := 255;
         iBMPsAS[maxc].g := 255;
         iBMPsAS[maxc].b := 255;

         iBMPsAS := BitmapAS.ScanLine[maxl-2];
         iBMPsAS[maxc].r := 255;
         iBMPsAS[maxc].g := 255;
         iBMPsAS[maxc].b := 255;
     end;

     if (maxl+3 < knl-1) then
     begin
         iBMPsAS := BitmapAS.ScanLine[maxl+3];
         iBMPsAS[maxc].r := 255;
         iBMPsAS[maxc].g := 255;
         iBMPsAS[maxc].b := 255;

         iBMPsAS := BitmapAS.ScanLine[maxl+2];
         iBMPsAS[maxc].r := 255;
         iBMPsAS[maxc].g := 255;
         iBMPsAS[maxc].b := 255;
     end;

     if (maxc-3 >= 0) and (maxl >= 0)then
     begin
         iBMPsAS := BitmapAS.ScanLine[maxl];
         iBMPsAS[maxc-3].r := 255;
         iBMPsAS[maxc-3].g := 255;
         iBMPsAS[maxc-3].b := 255;

         iBMPsAS := BitmapAS.ScanLine[maxl];
         iBMPsAS[maxc-2].r := 255;
         iBMPsAS[maxc-2].g := 255;
         iBMPsAS[maxc-2].b := 255;
     end;


     if (maxc+3 < knc-1) and (maxl >= 0) then
     begin
         iBMPsAS := BitmapAS.ScanLine[maxl];
         iBMPsAS[maxc+3].r := 255;
         iBMPsAS[maxc+3].g := 255;
         iBMPsAS[maxc+3].b := 255;

         iBMPsAS := BitmapAS.ScanLine[maxl];
         iBMPsAS[maxc+2].r := 255;
         iBMPsAS[maxc+2].g := 255;
         iBMPsAS[maxc+2].b := 255;
     end;
     *)
         
     // Mostrando Imagem
     mod_ImgExp4_SOM_SAL.Picture.Graphic := BitmapAS;
  finally
  end;

  // Mostrando Gráfico
 // for i := 0 to 100*100 do
  //   CSal.Series[0].YValue[i] := randomrange(0,10);

end;


procedure TFormProjeto.Timer3Timer(Sender: TObject);
begin
   ShowAtributoSaliente;
end;

procedure TFormProjeto.Timer4Timer(Sender: TObject);
var
    i,j : integer;
begin
    for i := 0 to length(Segmentos)-1 do
    begin
       if Segmentos[i].segValido = true then
       begin
          if CBIni.Checked = True then Segmentos[i].IeD.y := 0;  //influencia da inibição

          Segmentos[i].IeD.v := Segmentos[i].IeD.v + (( (Segmentos[i].SegMedias[0]*(vtbI/10))  +
                                                        (Segmentos[i].SegMedias[1]*(vtbRG/10)) +
                                                        (Segmentos[i].SegMedias[2]*(vtbBY/10)) +
                                                        (Segmentos[i].SegMedias[3]*(vtbXY/10)) +
                                                        (Segmentos[i].SegMedias[4]*(vtbXY/10)) +
                                                        (Segmentos[i].SegMedias[9]*(vtbRN/10)) + //Segmentos[i].IeD.i - Segmentos[i].IeD.v  - (Segmentos[i].IeD.y * Winib)) * dt_SAL); //dt
                                                        (Segmentos[i].SegMedias[10]*(vtbSize/10)) + Segmentos[i].IeD.i - Segmentos[i].IeD.v  - (Segmentos[i].IeD.y * Winib)) * dt_SAL); //dt

          if (Segmentos[i].IeD.v < 0) then                            // o valor i só cresce                                                           //discretização do tempo
              Segmentos[i].IeD.v := 0;

          if (Segmentos[i].IeD.v > 1) then                            // o valor i só cresce                                                           //discretização do tempo
              Segmentos[i].IeD.v := 1;

          Segmentos[i].IeD.y := 0;
       end;
    end;

end;

procedure TFormProjeto.Timer5Timer(Sender: TObject);
var
  Bitmap : TBitmap;
  iBMPs : PRGB;
  i,j,s,ml,mc : integer;
  SEG_TMP : TSeg_Classify;
begin

  rgbtmp := 0; rgbtmp_ant := 0;

  Bitmap := TBitmap.Create;
  try
     Bitmap.PixelFormat := pf24bit;
     Bitmap.Width := knc;
     Bitmap.Height := knl;
     for i := 0 to knl-1 do
     begin
       iBMPs := Bitmap.ScanLine[i];
       for j := 0 to knc-1 do
       begin


         //iBMPs[j].r := GetRValue(mod_ImgExp4_IeF.Canvas.Pixels[j,i]); //rgb[0]
         //iBMPs[j].g := GetGValue(mod_ImgExp4_IeF.Canvas.Pixels[j,i]);
         //iBMPs[j].b := GetBValue(mod_ImgExp4_IeF.Canvas.Pixels[j,i]);
         if rbIS.Checked then
         begin
            iBMPs[j].r := trunc(Neuronios[i,j].rgb[0]); //rgb[0]
            iBMPs[j].g := trunc(Neuronios[i,j].rgb[1]);
            iBMPs[j].b := trunc(Neuronios[i,j].rgb[2]);
         end   
         else begin
            iBMPs[j].r := trunc(Neuronios[i,j].rgbSColor[0]); //rgb[0]
            iBMPs[j].g := trunc(Neuronios[i,j].rgbSColor[1]);
            iBMPs[j].b := trunc(Neuronios[i,j].rgbSColor[2]);
         end;

         //rgbtmp := SNeuronios[i,j].v * 255;
         //if (maxl <> -1) and (maxc <> -1) then
         //rgbtmp := saliency;

         if saliency > rgbtmp_ant then
         begin
            // Ativando ainda mais caso seja o vencedor...
            //SNeuronios[i,j].v := SNeuronios[i,j].v * 1.001;

            //sal.l := Neuronios[maxl,maxc][3]; //sal.l := KNeuronios[i,j][3];
            //sal.c := Neuronios[maxl,maxc][4];//sal.c := KNeuronios[i,j][4];

            sal.l := (Segmentos[max_seg].imin + Segmentos[max_seg].imax) div 2;
            sal.c := (Segmentos[max_seg].jmin + Segmentos[max_seg].jmax) div 2;

            //sal.l := maxl;
            //sal.c := maxc;

            //sal.l := SNeuronios[i,j].l;
            //sal.c := SNeuronios[i,j].c;
            rgbtmp_ant := saliency;
            //salk.l := KNeuronios[i,j][3];//i;
            //salk.c := KNeuronios[i,j][4];//j;
         end;
       end;
     end;

     if (sal.l <> -1) and (sal.c <> -1) then
     begin

     //label25.Caption := 'X = [' + inttostr(sal.l) + ',' + inttostr(sal.c) + ']';

     iBMPs := Bitmap.ScanLine[sal.l];
     iBMPs[sal.c].r := 0;
     iBMPs[sal.c].g := 255;
     iBMPs[sal.c].b := 0;

     //BitMap.Canvas.Pen.Color := clgreen;
     //BitMap.Canvas.Arc(sal.c-7,sal.l-7,sal.c+7,sal.l+7,0,0,0,0);

     // desenhando mira
     if (sal.l-3 > 0) then
     begin
         iBMPs := Bitmap.ScanLine[sal.l-3];
         iBMPs[sal.c].r := 255;
         iBMPs[sal.c].g := 255;
         iBMPs[sal.c].b := 255;

         iBMPs := Bitmap.ScanLine[sal.l-2];
         iBMPs[sal.c].r := 255;
         iBMPs[sal.c].g := 255;
         iBMPs[sal.c].b := 255;
     end;
     if (sal.l+3 < knl-1) then
     begin
         iBMPs := Bitmap.ScanLine[sal.l+3];
         iBMPs[sal.c].r := 255;
         iBMPs[sal.c].g := 255;
         iBMPs[sal.c].b := 255;

         iBMPs := Bitmap.ScanLine[sal.l+2];
         iBMPs[sal.c].r := 255;
         iBMPs[sal.c].g := 255;
         iBMPs[sal.c].b := 255;
     end;
     if (sal.c-3 > 0) then
     begin
         iBMPs := Bitmap.ScanLine[sal.l];
         iBMPs[sal.c-3].r := 255;
         iBMPs[sal.c-3].g := 255;
         iBMPs[sal.c-3].b := 255;

         iBMPs := Bitmap.ScanLine[sal.l];
         iBMPs[sal.c-2].r := 255;
         iBMPs[sal.c-2].g := 255;
         iBMPs[sal.c-2].b := 255;
     end;
     if (sal.c+3 < knc-1) then
     begin
         iBMPs := Bitmap.ScanLine[sal.l];
         iBMPs[sal.c+3].r := 255;
         iBMPs[sal.c+3].g := 255;
         iBMPs[sal.c+3].b := 255;

         iBMPs := Bitmap.ScanLine[sal.l];
         iBMPs[sal.c+2].r := 255;
         iBMPs[sal.c+2].g := 255;
         iBMPs[sal.c+2].b := 255;
     end;

     end;

     // Desenhando as Linhas
     // Ordena Vetos de V

     {
     for s := 0 to length(Segmentos)-1 do
     begin
         Seg_Classify[s].imin := Segmentos[s].imin;
         Seg_Classify[s].imax := Segmentos[s].imax;
         Seg_Classify[s].jmin := Segmentos[s].jmin;
         Seg_Classify[s].jmax := Segmentos[s].jmax;
         Seg_Classify[s].Classify := Segmentos[s].IED.v;
     end;

    for s := 0 to length(Seg_Classify)-2 do
        for i := s+1 to length(Seg_Classify)-1 do
            if Seg_Classify[s].Classify > Seg_Classify[i].Classify then
            begin
               SEG_TMP := Seg_Classify[s];
               Seg_Classify[s] := Seg_Classify[i];
               Seg_Classify[i] := SEG_TMP;
            end;

     Bitmap.Canvas.Pen.Color:=clred;
     mc := (Seg_Classify[0].imin + Seg_Classify[0].imax) div 2;
     ml := (Seg_Classify[0].jmin + Seg_Classify[0].jmax) div 2;
     BitMap.Canvas.MoveTo(0,0);
     Bitmap.Canvas.LineTo(ml,mc);
     Bitmap.Canvas.Pen.Color:=clgreen;
     for s := 1 to (length(Seg_Classify) div 2) do
     begin
        mc := (Seg_Classify[s].imin + Seg_Classify[s].imax) div 2;
        ml := (Seg_Classify[s].jmin + Seg_Classify[s].jmax) div 2;
        Bitmap.Canvas.LineTo(ml,mc);
        BitMap.Canvas.MoveTo(ml,mc);
     end;
     }

     // Mostrando Imagem
     mod_ImgExp4_SAL.Picture.Graphic := Bitmap;
  finally
     Bitmap.Free;
  end;

end;

procedure TFormProjeto.mod_ImgExp4_SALMouseMove(Sender: TObject;
  Shift: TShiftState; X, Y: Integer);
var
  l,c : integer;
begin
//    c := (x div 7) ;
//    l := (y div 5) ;
    c := round(x / (mod_ImgExp4_SAL.Width  /(knc-1)));//(x div 7) ;
    l := round(y / (mod_ImgExp4_SAL.Height /(knl-1)));//(y div 5) ;

    label25.Caption := 'X = [' + inttostr(l) + ',' + inttostr(c) + ']';
end;

procedure TFormProjeto.cbIniClick(Sender: TObject);
begin
     rgbtmp_ant := 0;
{   if cbini.Checked then
   begin
     rgbtmp_ant := 0;
     rgbtmp := 0;
     sal.l := -1; sal.c := -1;
     salk.l := -1; salk.c := -1;
   end else
   begin
     rgbtmp_ant := 0;
     rgbtmp := 0;
     sal.l := -1; sal.c := -1;
     salk.l := -1; salk.c := -1;
   end;
   }
end;

procedure TFormProjeto.cbSegClick(Sender: TObject);
begin
   Timer.Enabled := false;
   timer6.Enabled := false;
   timer2.Enabled := false;
   timer4.Enabled := false;
   timer1.Enabled := false;
   timer3.Enabled := false; timer7.Enabled := false; 
   timer5.Enabled := false;
   //cbSeg.Checked := true;
   cbSeg.Caption := 'Simulação Parada';
end;

procedure TFormProjeto.Timer6Timer(Sender: TObject);
begin
  //Kohonen;
end;

procedure TFormProjeto.BitBtn36Click(Sender: TObject);
begin
  //Treina_SOM := false;
  //PageControl13.TabIndex := 10;
  // SOM Saliente-----------------------------------
  timer4.Enabled := true;
  timer1.Enabled := true;
  timer3.Enabled := true;
  // timer7.Enabled := true;

  // SOM Saliente-----------------------------------
  timer5.Enabled := true;

end;

Procedure TFormProjeto.AtualizarParametros(Sender: TObject);
var
  arq : TextFile;
  linha : string;
  i : integer;
begin
   if semaforo = true then exit;

   parametros.Clear;

   // LATERAL
   // 0 Imagem Colorida 8bits
   if rbExp4_8.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');
   // 1 Imagem Colorida 24bits
   if RadioButton13.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');
   // 2 Imagem Colorida
   if cbExp4_Color.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');
   // 3 Mostrar Pulso
   if cbExp4_MP.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');
   // 4 Segmentação em Tons de Cinza
   if cbExp4_Cinza.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');
   // 5 Segmentação em Cores Aleatórias
   if cbExp4_Cores.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');
   // 6 Ativar/Desativar Inibidor Global
   if cbIni.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');

   // 7 Tempo de Pulso   
   parametros.Lines.Add(inttostr(tbpulso.Position));

   // 8 9 10 11 12 LEGION
   parametros.Lines.Add(inttostr(TrackBarthetap.Position));
   parametros.Lines.Add(inttostr(TrackBarTetax.Position));
   parametros.Lines.Add(inttostr(TrackBarWz.Position));
   parametros.Lines.Add(inttostr(TrackBarTetaxz.Position));
   parametros.Lines.Add(inttostr(TrackBarLimiarFundo.Position));

   // 13 14 15 16 SALIÊNCIA
   parametros.Lines.Add(mskdt_SAL.Text);
   parametros.Lines.Add(mskWInib.Text);
   parametros.Lines.Add(inttostr(tbLimiarFundoSeg.Position));
   parametros.Lines.Add(mskThetas.Text);

   // 17 18 SUAVIZAR IMAGEM
   parametros.Lines.Add('0');
   parametros.Lines.Add('0');

   // 19 20 ORIENTAÇÃO
   if cbExp4_LO.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');
   parametros.Lines.Add(inttostr(SEditLO.Value));

   // 21 22 COR BORDA SEGMENTO
   if cbBorda.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');
   parametros.Lines.Add(inttostr(ColorGridSeg.ForegroundIndex));

   // 23 24 SEGMENTADO
   if cb_Strech.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');
   if cbVer_Sincro.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');

   // 25 26 27 MAPA DE SALIÊNCIA 3D
   if cbV3D.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');
   parametros.Lines.Add(inttostr(tb3dDim.Position));
   parametros.Lines.Add(inttostr(tbOrtho.Position));

   // 28 29 REGIÃO SALIENTE
   if rgIE.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');
   if rbIS.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');

   // 30 31 32 33 34 35 PESOS SALIENTES
   // TAM
   parametros.Lines.Add(inttostr(tbSize.Position));
   // Intensidade
   parametros.Lines.Add(inttostr(tbI.Position));
   // Cor RG/BY
   parametros.Lines.Add(inttostr(tbRG.Position));
   // Orientação
   parametros.Lines.Add(inttostr(tbBY.Position));
   // Posição LIN/COL
   parametros.Lines.Add(inttostr(tbXY.Position));
   // Reconhecimento
   parametros.Lines.Add(inttostr(tbRN.Position));

   // 36 37 38 39 40 41 42 PESOS ENVIESAMENTO LEGION
   if cbWCor.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');
   parametros.Lines.Add(mskWc.Text);
   if cbWInt.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');
   parametros.Lines.Add(mskWi.Text);
   if cbWOri.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');
   parametros.Lines.Add(mskWo.Text);
   parametros.Lines.Add(mskWsum.Text);


   AssignFile(arq,nome_arq);
   rewrite(arq);

   for i := 0 to 42 do
   begin
      writeln(arq,parametros.Lines.Strings[i]);
   end;
   closefile(arq);

   // Atualizando gráficos de pesos

   ChartCP.Series[0].Clear;
   ChartCP.Series[1].Clear;
//   ChartCP.Series[1].AddXY(0,0,'',clRed);

   if cbWCor.Checked and (mskWc.Text <> '') then
      ChartCP.Series[1].AddXY(1,strtofloat(mskWc.Text),'Color',clRed)
   else
      ChartCP.Series[1].AddXY(1,0,'Color',clRed);

   if cbWInt.Checked and (mskWi.Text <> '') then
      ChartCP.Series[1].AddXY(2,strtofloat(mskWi.Text),'Intensity',clRed)
   else
      ChartCP.Series[1].AddXY(2,0,'Intensity',clRed);

   if cbWOri.Checked and (mskWo.Text <> '') then
      ChartCP.Series[1].AddXY(3,strtofloat(mskWo.Text),'Orientation',clRed)
   else
      ChartCP.Series[1].AddXY(3,0,'Orientation',clRed);


   ChartCP.Series[0].AddXY(0,strtofloat(mskWsum.Text),'Threshold of Biasing',clRed);
   ChartCP.Series[0].AddXY(1,-1,'Color',clRed);
   ChartCP.Series[0].AddXY(2,-1,'Intensity',clRed);
   ChartCP.Series[0].AddXY(3,-1,'Orientation',clRed);


   ChartOBF.Series[0].Clear;
   ChartOBF.Series[0].AddXY(0,(tbRG.Position/10),'Color',clRed);
   ChartOBF.Series[0].AddXY(1,(tbI.Position/10),'Intensity',clRed);
   ChartOBF.Series[0].AddXY(2,(tbBY.Position/10),'Orientation',clRed);
   ChartOBF.Series[0].AddXY(3,(tbXY.Position/10),'Location',clRed);
   ChartOBF.Series[0].AddXY(4,(tbSize.Position/10),'Size',clRed);
   ChartOBF.Series[0].AddXY(5,(tbRN.Position/10),'Recognition',clRed);

end;


procedure TFormProjeto.FileListBox1Click(Sender: TObject);
var
  linha,n_arq : string;
  i : integer;
  arq_in : TextFile;
begin
  try

   if (BitBtn2.Enabled = false) then
   begin
      Application.MessageBox('Existe uma simulação em execução. Para continuar deve interromper clicando em [PARAR SIMULAÇÃO]','Atenção',mb_iconExclamation);
      exit;
   end;

   n_arq := FileListBox1.FileName;
   setlength(n_arq,length(n_arq)-4);

   semaforo:= true;
   nome_arq := FileListBox1.FileName;
   parametros.Clear;

   If not(fileexists(nome_arq)) then
   begin

      // 0 Imagem Colorida 8bits
      if rbExp4_8.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');

      // 1 Imagem Colorida 24bits
      if RadioButton13.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');

      // 2 Imagem Colorida
      if cbExp4_Color.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');

      // 3 Mostrar Pulso
      if cbExp4_MP.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');

      // 4 Segmentação em Tons de Cinza
      if cbExp4_Cinza.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');

      // 5 Segmentação em Cores Aleatórias
      if cbExp4_Cores.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');

      // 6 Ativar/Desativar Inibidor Global
      if cbIni.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');

      // 7 Tempo de Pulso
      parametros.Lines.Add(inttostr(tbpulso.Position));

      // 8 9 10 11 12 LEGION
      parametros.Lines.Add(inttostr(TrackBarthetap.Position));
      parametros.Lines.Add(inttostr(TrackBarTetax.Position));
      parametros.Lines.Add(inttostr(TrackBarWz.Position));
      parametros.Lines.Add(inttostr(TrackBarTetaxz.Position));
      parametros.Lines.Add(inttostr(TrackBarLimiarFundo.Position));

      // 13 14 15 16 SALIÊNCIA
      parametros.Lines.Add(mskdt_SAL.Text);
      parametros.Lines.Add(mskWInib.Text);
      parametros.Lines.Add(inttostr(tbLimiarFundoSeg.Position));
      parametros.Lines.Add(mskThetas.Text);

      // 17 18 SUAVIZAR IMAGEM
      parametros.Lines.Add('0');
      parametros.Lines.Add('0');

      // 19 20 ORIENTAÇÃO
      if cbExp4_LO.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');
      parametros.Lines.Add(inttostr(SEditLO.Value));

      // 21 22 COR BORDA SEGMENTO
      if cbBorda.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');
      parametros.Lines.Add(inttostr(ColorGridSeg.ForegroundIndex));

      // 23 24 SEGMENTADO
      if cb_Strech.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');
      if cbVer_Sincro.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');

      // 25 26 27 MAPA DE SALIÊNCIA 3D
      if cbV3D.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');
      parametros.Lines.Add(inttostr(tb3dDim.Position));
      parametros.Lines.Add(inttostr(tbOrtho.Position));

      // 28 29 REGIÃO SALIENTE
      if rgIE.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');
      if rbIS.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');

      // 30 31 32 33 34 35 PESOS SALIENTES
      // TAM
      parametros.Lines.Add(inttostr(tbSize.Position));
      // Intensidade
      parametros.Lines.Add(inttostr(tbI.Position));
      // Cor RG/BY
      parametros.Lines.Add(inttostr(tbRG.Position));
      // Orientação
      parametros.Lines.Add(inttostr(tbBY.Position));
      // Posição LIN/COL
      parametros.Lines.Add(inttostr(tbXY.Position));
      // Reconhecimento
      parametros.Lines.Add(inttostr(tbRN.Position));

      // 36 37 38 39 40 41 42 PESOS ENVIESAMENTO LEGION
      if cbWCor.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');
      parametros.Lines.Add(mskWc.Text);
      if cbWInt.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');
      parametros.Lines.Add(mskWi.Text);
      if cbWOri.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');
      parametros.Lines.Add(mskWo.Text);
      parametros.Lines.Add(mskWsum.Text);

      AssignFile(arq_in,nome_arq);
      rewrite(arq_in);
      for i := 0 to 42 do
      begin
         writeln(arq_in,parametros.Lines.Strings[i]);
      end;
      closefile(arq_in);
   end else
   begin
      AssignFile(arq_in,nome_arq);
      reset(arq_in);
      for i := 0 to 42 do
      begin
         readln(arq_in,linha);
         case i of
           // LATERAL
           0 : if linha = '1' then rbExp4_8.Checked := true      else  rbExp4_8.Checked := false;
           1 : if linha = '1' then RadioButton13.Checked := true else  RadioButton13.Checked := false;
           2 : if linha = '1' then cbExp4_Color.Checked := true  else  cbExp4_Color.Checked := false;
           3 : if linha = '1' then cbExp4_MP.Checked := true     else  cbExp4_MP.Checked := false;
           4 : if linha = '1' then cbExp4_Cinza.Checked := true  else  cbExp4_Cinza.Checked := false;
           5 : if linha = '1' then cbExp4_Cores.Checked := true  else  cbExp4_Cores.Checked := false;
           6 : if linha = '1' then cbIni.Checked := true         else  cbIni.Checked := false;
           7 : tbpulso.Position := strtoint(linha);

           //LEGION
            8 : TrackBarthetap.Position := strtoint(linha);
            9 : TrackBarTetax.Position := strtoint(linha);
           10 : TrackBarWz.Position := strtoint(linha);
           11 : TrackBarTetaxz.Position := strtoint(linha);
           12 : TrackBarLimiarFundo.Position := strtoint(linha);

           // SALIENCIA
           13 : mskdt_SAL.Text := linha;
           14 : if linha <> '' then mskWInib.Text := linha;
           15 : if linha <> '' then tbLimiarFundoSeg.Position := strtoint(linha);
           16 : if linha <> '' then mskThetas.Text := linha;

           // SUAVIZAR IMAGEM
           //17 : if linha = '1' then cbExp4_S.Checked := true else cbExp4_S.Checked := false;
           //18 : SEditLadoExp4_M.Value := strtoint(linha);

           // 19 20 ORIENTAÇÃO
           19 : if linha = '1' then cbExp4_LO.Checked := true else cbExp4_LO.Checked := false;
           20 : SEditLO.Value := strtoint(linha);

           // 21 22 COR BORDA SEGMENTO
           21 : if linha = '1' then cbBorda.Checked := true else cbBorda.Checked := false;
           22 : ColorGridSeg.ForegroundIndex := strtoint(linha);

           // 23 24 SEGMENTADO
           23 : if linha = '1' then cb_Strech.Checked := true else cb_Strech.Checked := false;
           24 : if linha = '1' then cbVer_Sincro.Checked := true else cbVer_Sincro.Checked := false;

           // 25 26 27 MAPA DE SALIÊNCIA 3D
           25 : if linha = '1' then cbV3D.Checked := true else cbV3D.Checked := false;
           26 : tb3dDim.Position := strtoint(linha);
           27 : tbOrtho.Position := strtoint(linha);

           // 28 29 REGIÃO SALIENTE
           28 : if linha = '1' then rgIE.Checked := true else rgIE.Checked := false;
           29 : if linha = '1' then rbIS.Checked := true else rbIS.Checked := false;

           // 30 31 32 33 34 35 PESOS SALIENTES
           30 : begin
                  tbSize.Position := strtoint(linha);
                  tbSize.OnChange(Sender);
                end;
           31 : begin
                  tbI.Position := strtoint(linha);
                  tbI.OnChange(Sender);
                end;
           32 : begin
                  tbRG.Position := strtoint(linha);
                  tbRG.OnChange(Sender);
                end;
           33 : begin
                  tbBY.Position := strtoint(linha);
                  tbBY.OnChange(Sender);
                end;
           34 : begin
                  tbXY.Position := strtoint(linha);
                  tbXY.OnChange(Sender);
                end;
           35 : begin
                  tbRN.Position := strtoint(linha);
                  tbRN.OnChange(Sender);
                end;

           36 : if linha = '1' then cbWcor.Checked := true else cbWcor.Checked := false;
           37 : if linha <> '' then mskWc.Text := linha;
           38 : if linha = '1' then cbWint.Checked := true else cbWint.Checked := false;
           39 : if linha <> '' then mskWi.Text := linha;
           40 : if linha = '1' then cbWori.Checked := true else cbWori.Checked := false;
           41 : if linha <> '' then mskWo.Text := linha;
           42 : if linha <> '' then mskWsum.Text := linha;

         end;
      end;
      closefile(arq_in);
   end;
   semaforo := false;


  except
      Application.MessageBox('Imagem Não Suportada ou Mapa de Saliência Inexistente.','Atenção',mb_iconExclamation);
  end;

end;

procedure TFormProjeto.FormClose(Sender: TObject;
  var Action: TCloseAction);
begin
   CBSeg.OnClick(Sender);
end;

procedure TFormProjeto.SpeedButton1Click(Sender: TObject);
var i: integer;
begin
end;

procedure TFormProjeto.FileListBox2Click(Sender: TObject);
begin
   ImgTeste.Picture.LoadFromFile(FileListBox2.FileName);
end;

procedure TFormProjeto.mod_ImgExp4_IeFMouseMove(Sender: TObject;
  Shift: TShiftState; X, Y: Integer);
var
  l,c : integer;
begin
    if (knc = 0) or (knl = 0) then exit;

    if (mod_ImgExp4_IeF.Width div knc = 0) or (mod_ImgExp4_IeF.Height div knl = 0) then exit;

    c := round(x / (mod_ImgExp4_IeF.Width  /(knc-1)));//(x div 7) ;
    l := round(y / (mod_ImgExp4_IeF.Height /(knl-1)));//(y div 5) ;

   label82.Caption := '['+inttostr(l)+','+inttostr(c)+']';

   label73.Caption := 'MLP = [' + formatfloat('0.00',Neuronios[l,c].rn) + ']';
end;

procedure TFormProjeto.tbIChange(Sender: TObject);
begin
   vtbI := tbI.Position;
   AtualizarParametros(Sender);   
end;

procedure TFormProjeto.tbRGChange(Sender: TObject);
begin
    vtbRG := tbRG.Position;
    AtualizarParametros(Sender);
end;

procedure TFormProjeto.tbBYChange(Sender: TObject);
begin
    vtbBY := tbBY.Position;
    AtualizarParametros(Sender);    
end;

procedure TFormProjeto.tbXYChange(Sender: TObject);
begin
    vtbXY := tbXY.Position;
    AtualizarParametros(Sender);    
end;

procedure TFormProjeto.tbRNChange(Sender: TObject);
begin
   vtbRN := tbRN.Position;
   AtualizarParametros(Sender);   
end;

procedure TFormProjeto.tbSizeChange(Sender: TObject);
begin
   vtbSize := tbSize.Position;
   AtualizarParametros(Sender);
end;

(*procedure TFormProjeto.Connections;
var
   i,j,k,a,b,s,t,cont : integer;
begin
   //limiarF := 0.05;
   //WTotal := 6.0;

   //* for eight nearest neighbors */
   for i := 0 to knl-1 do
     for j := 0 to knc-1 do
     begin
       neuronios[i,j].w[0] := 0.0;
       neuronios[i,j].w[1] := 0.0;
       neuronios[i,j].w[2] := 0.0;
       neuronios[i,j].w[3] := 0.0;
       neuronios[i,j].w[4] := 0.0;
       neuronios[i,j].w[5] := 0.0;
       neuronios[i,j].w[6] := 0.0;
       neuronios[i,j].w[7] := 0.0;

       neuronios[i,j].lateral := 0;
     end;


   a := 1;
   b := a;
   for i := 0 to knl-1 do
   begin
     for j := 0 to knc-1 do
     begin
       if neuronios[i,j].stim >= limiarF then  // elimando fundo
       begin

         cont := 0;
         for s := -a to a do
            for t := -b to b do
            begin
               if not((s=0) and (t=0)) then
               begin
                  if ((i+s >=0) and (i+s <= knL-1)) and ((j+t >= 0) and (j+t <= knC-1)) then
              	      if cbExp4_Color.Checked = false then
                         Neuronios[i,j].w[cont] := Sim_Cinza(trunc(neuronios[i,j].stim),trunc(neuronios[i+s,j+t].stim))
                      else
                         Neuronios[i,j].w[cont] := Sim_Cor(i,j,i+s,j+t);
                  inc(cont);
               end;
            end;


         {* se todos os vizinhos estiverem realmente acoplados,
            o valor do lateral será 8;
         *}
         for k := 0 to 8 do
            neuronios[i,j].lateral := neuronios[i,j].lateral + neuronios[i,j].w[k];
       end;

     end; // j
   end; // i


end;
*)

//Salvo Rotina Connections
(*
procedure TFormProjeto.Connections;
var
   i,j,k : integer;
begin
   //limiarF := 0.05;
   //WTotal := 6.0;

   //* for eight nearest neighbors */
   for i := 0 to knl-1 do
     for j := 0 to knc-1 do
     begin
       neuronios[i,j].w[0] := 0.0;
       neuronios[i,j].w[1] := 0.0;
       neuronios[i,j].w[2] := 0.0;
       neuronios[i,j].w[3] := 0.0;
       neuronios[i,j].w[4] := 0.0;
       neuronios[i,j].w[5] := 0.0;
       neuronios[i,j].w[6] := 0.0;
       neuronios[i,j].w[7] := 0.0;

       neuronios[i,j].lateral := 0;
     end;


   for i := 0 to knl-1 do
     for j := 0 to knc-1 do
     begin

       if neuronios[i,j].stim >= limiarF then  // elimando fundo
       begin

         if cbExp4_Color.Checked = false then
         begin

//                    dist := Sim_Cor(vnc.l,vnc.c,vnc.l+s,vnc.c+t);


         if (j-1 >= 0) then
    	      neuronios[i,j].w[0] := Sim_Cinza(trunc(neuronios[i,j].stim),trunc(neuronios[i,j-1].stim));

         if (i-1 >= 0) and (j-1 >= 0) then
	          neuronios[i,j].w[1] := Sim_Cinza(trunc(neuronios[i,j].stim),trunc(neuronios[i-1,j-1].stim));

         if (i-1 >= 0) then
	          neuronios[i,j].w[2] := Sim_Cinza(trunc(neuronios[i,j].stim),trunc(neuronios[i-1,j].stim));

         if (i-1 >= 0) and (j+1 < knc) then
      	    neuronios[i,j].w[3] := Sim_Cinza(trunc(neuronios[i,j].stim),trunc(neuronios[i-1,j+1].stim));

         if (j+1 < knc) then
      	    neuronios[i,j].w[4] := Sim_Cinza(trunc(neuronios[i,j].stim),trunc(neuronios[i,j+1].stim));

         if (i+1 < knl) and (j+1 < knc) then
      	    neuronios[i,j].w[5] := Sim_Cinza(trunc(neuronios[i,j].stim),trunc(neuronios[i+1,j+1].stim));

         if (i+1 < knl) then
      	    neuronios[i,j].w[6] := Sim_Cinza(trunc(neuronios[i,j].stim),trunc(neuronios[i+1,j].stim));

         if (i+1 < knl) and (j-1 >= 0) then
      	    neuronios[i,j].w[7] := Sim_Cinza(trunc(neuronios[i,j].stim),trunc(neuronios[i+1,j-1].stim));

         end else
         begin

         if (j-1 >= 0) then
    	      neuronios[i,j].w[0] := Sim_Cor(i,j,i,j-1);

         if (i-1 >= 0) and (j-1 >= 0) then
	          neuronios[i,j].w[1] := Sim_Cor(i,j,i-1,j-1);

         if (i-1 >= 0) then
	          neuronios[i,j].w[2] := Sim_Cor(i,j,i-1,j);

         if (i-1 >= 0) and (j+1 < knc) then
      	    neuronios[i,j].w[3] := Sim_Cor(i,j,i-1,j+1);

         if (j+1 < knc) then
      	    neuronios[i,j].w[4] := Sim_Cor(i,j,i,j+1);

         if (i+1 < knl) and (j+1 < knc) then
      	    neuronios[i,j].w[5] := Sim_Cor(i,j,i+1,j+1);

         if (i+1 < knl) then
      	    neuronios[i,j].w[6] := Sim_Cor(i,j,i+1,j);

         if (i+1 < knl) and (j-1 >= 0) then
      	    neuronios[i,j].w[7] := Sim_Cor(i,j,i+1,j-1);

         end;
         {* se todos os vizinhos estiverem realmente acoplados,
            o valor do lateral será 8;
         *}
         for k := 0 to 8 do
            neuronios[i,j].lateral := neuronios[i,j].lateral + neuronios[i,j].w[k];
       end;

     end;

end;

*)

function TFormProjeto.WeightsConnections(i,j:integer): real;
var Res : real;
    n:integer;
begin
   n:=0;
   if cbWint.Checked then inc(n);
   if cbWcor.Checked then inc(n);
   if cbWori.Checked then inc(n);

   Res := 0;
   with Neuronios[i,j] do
   begin
      if cbWint.Checked then Res := Res + (rgb_Itti[0]/255*wi);
      if cbWcor.Checked then Res := Res + (rgb_Itti[1]/255*wc);
      if cbWori.Checked then Res := Res + (rgb_Itti[2]/255*wo);
      WeightsConnections := Res/n;
   end;
end;


procedure TFormProjeto.Connections;
var
   i,j,k : integer;
begin
   //* for eight nearest neighbors */
   for i := 0 to knl-1 do
     for j := 0 to knc-1 do
     begin
       neuronios[i,j].w[0] := 0.0;
       neuronios[i,j].w[1] := 0.0;
       neuronios[i,j].w[2] := 0.0;
       neuronios[i,j].w[3] := 0.0;
       neuronios[i,j].w[4] := 0.0;
       neuronios[i,j].w[5] := 0.0;
       neuronios[i,j].w[6] := 0.0;
       neuronios[i,j].w[7] := 0.0;
       neuronios[i,j].lateral := 0;
     end;

   for i := 0 to knl-1 do
     for j := 0 to knc-1 do
     begin

       if neuronios[i,j].stim >= limiarF then  // elimando fundo
       begin

         if (j-1 >= 0) then
    	      neuronios[i,j].w[0] := Sim_Cor(i,j,i,j-1);

         if (i-1 >= 0) and (j-1 >= 0) then
	          neuronios[i,j].w[1] := Sim_Cor(i,j,i-1,j-1);

         if (i-1 >= 0) then
	          neuronios[i,j].w[2] := Sim_Cor(i,j,i-1,j);

         if (i-1 >= 0) and (j+1 < knc) then
      	    neuronios[i,j].w[3] := Sim_Cor(i,j,i-1,j+1);

         if (j+1 < knc) then
      	    neuronios[i,j].w[4] := Sim_Cor(i,j,i,j+1);

         if (i+1 < knl) and (j+1 < knc) then
      	    neuronios[i,j].w[5] := Sim_Cor(i,j,i+1,j+1);

         if (i+1 < knl) then
      	    neuronios[i,j].w[6] := Sim_Cor(i,j,i+1,j);

         if (i+1 < knl) and (j-1 >= 0) then
      	    neuronios[i,j].w[7] := Sim_Cor(i,j,i+1,j-1);

           {* se todos os vizinhos estiverem realmente acoplados,
            o valor do lateral será 8;
         *}
         for k := 0 to 8 do
            neuronios[i,j].lateral := neuronios[i,j].lateral + neuronios[i,j].w[k];
       end;

     end;

end;


function rnd : integer;
begin     //(1 - 2 * (rand()                 /(MAXVAL - 1.0)));
   rnd := randomrange(0,MAXVAL-1);
end;

procedure TFormProjeto.Select;
var
	i, j, max, imax, jmax : integer;
	temp : double;
begin

  imax := 0; jmax := 0;
  //baseline := 1200.0;
	//* baseline 1200 -> 17 regions; 1000 -> 23 regions. Both reasonable */

	max := -1;
  for i:=0 to knl-1 do
    for j:=0 to knc-1 do
    begin
			neuronios[i,j].trigger := 0;
			if (neuronios[i,j].lateral > thetap) and (neuronios[i,j].ly <> MAXIMO) and (neuronios[i,j].ly > max) then
         if WeightsConnections(i,j) >= wsum then
         begin
	    			max := neuronios[i,j].ly;
		    		imax := i;
    				jmax := j;
    			end;
		end;

  // Se imax = 1 e jmax = -1 significa que nao foi encontrado
  if (imax = 0) and (jmax = 0) and (wsum > 0) then
  begin
    for i:=0 to knl-1 do
      for j:=0 to knc-1 do
      begin
	   		neuronios[i,j].d := 1;
    		neuronios[i,j].lx[1] := LP;
    		neuronios[i,j].ly := -1;
    		neuronios[i,j].cycle := 1;
        z := 1;
      end;
  end;

	if (neuronios[imax,jmax].cycle > 0) then
  begin                          //* find out jumpers */
     for i:=0 to knl-1 do
       for j:=0 to knc-1 do
  				if (neuronios[i,j].ly = max) and (neuronios[i,j].cycle > 0) then
  					  neuronios[i,j].trigger := 1;
	end
  else
 	   neuronios[imax,jmax].trigger := 1;


  for i:=0 to knl-1 do    //* advance oscillators on the left branch */
    for j:=0 to knc-1 do
 			if (neuronios[i,j].ly <> MAXIMO) and ((neuronios[i,j].cycle > 0) or (neuronios[i,j].lateral > thetap)) then
      begin
				neuronios[i,j].ly := neuronios[i,j].ly + (MAXIMO - max);
				neuronios[i,j].lx[0] := neuronios[i,j].ly/(MAXVAL + 1.0);
			end;

   //inc(vc_cor);
   c_r := neuronios[imax,jmax].rgbSColor[0];
   c_g := neuronios[imax,jmax].rgbSColor[1];
   c_b := neuronios[imax,jmax].rgbSColor[2];
end;


// Atualiza para SOM
Procedure TFormProjeto.Atualiza_para_SOM;
var
  i,j,n_v,s,t,n,a,b : integer;
  iBMPe8 : PByteArray;
  pixelI : double;
  iBMPe24 : PRGB;
  Bitmap : TBitmap;
  iBMPs : PRGB;

  // Vetor de Pesos para Angulos de Direções
    wa : array[0..2,0..2] of integer;
    maxi : double;
begin
  // Inicializando neurônios, Leitura da Imagem de Entrada
  if rbExp4_8.Checked then
  begin
     for i := 0 to knl-1 do
     begin
        iBMPe8 := mod_ImgExp4_IeF.Picture.Bitmap.ScanLine[i];
        for j := 0 to knc-1 do
        begin
            // Imagem de Entrada Original RGB e I
               pixelI := (iBMPe8[j]);
               Neuronios[i,j].ang[0] := trunc(pixelI);
               Neuronios[i,j].ang[1] := 0; // Gradiente na Direção Horizontal
               Neuronios[i,j].ang[2] := 0; // Gradiente na Direção Vertical
               Neuronios[i,j].ang[3] := 0; // Gradiente na Direção +45
               Neuronios[i,j].ang[4] := 0; // Gradiente na Direção -45
        end; // fim da inicialização 8bits
      end;
  end
  else begin
     for i := 0 to knl-1 do
     begin
        iBMPe24 := mod_ImgExp4_IeF.Picture.Bitmap.ScanLine[i];
        for j := 0 to knc-1 do
        begin
            // Imagem de Entrada Original RGB e I
               pixelI := (iBMPe24[j].r+iBMPe24[j].g+iBMPe24[j].b) div 3;
               Neuronios[i,j].ang[0] := trunc(pixelI);
               Neuronios[i,j].ang[1] := 0; // Gradiente na Direção Horizontal
               Neuronios[i,j].ang[2] := 0; // Gradiente na Direção Vertical
               Neuronios[i,j].ang[3] := 0; // Gradiente na Direção +45
               Neuronios[i,j].ang[4] := 0; // Gradiente na Direção -45
        end; // fim da inicialização 24bits
      end;
  end; //  if


 //alalalalalalalala
  (*
  Low Pass
  *)
  // Carregando Máscara...
(*  for i := 0 to 2 do
     for j := 0 to 2 do
        wa[i,j] := strtoint(sgExp4_CNA.Cells[j,i]);

  a := 1;
  b := a;
  maxi := 0;
  for i := 0 to knL-1 do
     for j := 0 to knC-1 do
        for s := -a to a do
           for t := -b to b do
              if ((i+s >=0) and (i+s <= knL-1)) and ((j+t >= 0) and (j+t <= knC-1)) then
                 Neuronios[i,j].ang[1] := Neuronios[i,j].ang[1] + (wa[s+1,t+1] * Neuronios[i+s,j+t].rgb[4]);

  Bitmap := TBitmap.Create;
  try
     Bitmap.PixelFormat := pf24bit;
     Bitmap.Width := knc;
     Bitmap.Height := knl;
     for i := 0 to knl-1 do
     begin
       iBMPs := Bitmap.ScanLine[i];
       for j := 0 to knc-1 do
       begin
         iBMPs[j].r := (Neuronios[i,j].ang[1]);
         iBMPs[j].g := (Neuronios[i,j].ang[1]);
         iBMPs[j].b := (Neuronios[i,j].ang[1]);
       end;
     end;
     // Mostrando Imagem
     mod_ImgExp4_ICanny.Picture.Graphic := Bitmap;
  finally
     Bitmap.Free;
  end;


 //alalalalalalalala
 *)

  (*
  Componente do Gradiente na Direção X
  *)
  // Carregando Máscara...
  for i := 0 to 2 do
     for j := 0 to 2 do
        wa[i,j] := strtoint(sgExp4_CNA.Cells[j,i]);

  a := 1;
  b := a;
  maxi := 0;
  for i := 0 to knL-1 do
     for j := 0 to knC-1 do
     begin
        for s := -a to a do
           for t := -b to b do
              if ((i+s >=0) and (i+s <= knL-1)) and ((j+t >= 0) and (j+t <= knC-1)) then
                 Neuronios[i,j].ang[1] := Neuronios[i,j].ang[1] + (wa[s+1,t+1] * Neuronios[i+s,j+t].ang[0]);

        if (Neuronios[i,j].ang[1] < 0) then Neuronios[i,j].ang[1] := 0;
        if maxi < abs(Neuronios[i,j].ang[1]) then maxi := abs(Neuronios[i,j].ang[1]);
     end;
  // Apresentando Valor Absoluto do Filtro Laplaciano
  maxi := maxi*(SEditLO.Value/100);
  Bitmap := TBitmap.Create;
  try
     Bitmap.PixelFormat := pf24bit;
     Bitmap.Width := knc;
     Bitmap.Height := knl;
     for i := 0 to knl-1 do
     begin
       iBMPs := Bitmap.ScanLine[i];
       for j := 0 to knc-1 do
       begin
          if (cbExp4_LO.Checked) then
             if abs(trunc(Neuronios[i,j].ang[1])) >= trunc(maxi) then
             begin
                //Neuronios[i,j].ang[1] := 255;
             end
             else
             begin
                //Neuronios[i,j].ang[1] := 0;
             end;
          iBMPs[j].r := (Neuronios[i,j].ang[1]);
          iBMPs[j].g := (Neuronios[i,j].ang[1]);
          iBMPs[j].b := (Neuronios[i,j].ang[1]);

       end;
     end;
     // Mostrando Imagem
     mod_ImgExp4_ICanny.Picture.Graphic := Bitmap;
  finally
     Bitmap.Free;
  end;


  //----------------------------------------------------------------------------
  (*
  Componente do Gradiente na Direção Y
  *)

  // Carregando Máscara...
  for i := 0 to 2 do
     for j := 0 to 2 do
        wa[i,j] := strtoint(sgExp4_CNB.Cells[j,i]);

  a := 1;
  b := a;
  maxi := 0;
  for i := 0 to knL-1 do
     for j := 0 to knC-1 do
     begin
        for s := -a to a do
           for t := -b to b do
              if ((i+s >=0) and (i+s <= knL-1)) and ((j+t >= 0) and (j+t <= knC-1)) then
                 Neuronios[i,j].ang[2] := Neuronios[i,j].ang[2] + (wa[s+1,t+1] * Neuronios[i+s,j+t].ang[0]);

        if (Neuronios[i,j].ang[2] < 0) then Neuronios[i,j].ang[2] := 0;
        if maxi < abs(Neuronios[i,j].ang[2]) then maxi := abs(Neuronios[i,j].ang[2]);
     end;
  // Apresentando Valor Absoluto do Filtro Laplaciano
  maxi := maxi*(SEditLO.Value/100);
  Bitmap := TBitmap.Create;
  try
     Bitmap.PixelFormat := pf24bit;
     Bitmap.Width := knc;
     Bitmap.Height := knl;
     for i := 0 to knl-1 do
     begin
       iBMPs := Bitmap.ScanLine[i];
       for j := 0 to knc-1 do
       begin
          if (cbExp4_LO.Checked) then
             if abs(trunc(Neuronios[i,j].ang[2])) >= trunc(maxi) then
             begin
                //Neuronios[i,j].ang[2] := 255;
             end
             else
             begin
                //Neuronios[i,j].ang[2] := 0;
             end;
          iBMPs[j].r := trunc(Neuronios[i,j].ang[2]);
          iBMPs[j].g := trunc(Neuronios[i,j].ang[2]);
          iBMPs[j].b := trunc(Neuronios[i,j].ang[2]);
       end;
     end;
     // Mostrando Imagem
     mod_ImgExp4_RCanny.Picture.Graphic := Bitmap;
  finally
     Bitmap.Free;
  end;


  //----------------------------------------------------------------------------
  (*
  Componente do Gradiente na Direção +45
  *)
  // Carregando Máscara...
  for i := 0 to 2 do
     for j := 0 to 2 do
        wa[i,j] := strtoint(sgExp4_CN45P.Cells[j,i]);

  a := 1;
  b := a;
  maxi := 0;
  for i := 0 to knL-1 do
     for j := 0 to knC-1 do
     begin
        for s := -a to a do
           for t := -b to b do
              if ((i+s >=0) and (i+s <= knL-1)) and ((j+t >= 0) and (j+t <= knC-1)) then
                 Neuronios[i,j].ang[3] := Neuronios[i,j].ang[3] + (wa[s+1,t+1] * Neuronios[i+s,j+t].ang[0]);

        if (Neuronios[i,j].ang[3] < 0) then Neuronios[i,j].ang[3] := 0;
        if maxi < abs(Neuronios[i,j].ang[3]) then maxi := abs(Neuronios[i,j].ang[3]);
     end;
  // Apresentando Valor Absoluto do Filtro Laplaciano
  maxi := maxi*(SEditLO.Value/100);
  Bitmap := TBitmap.Create;
  try
     Bitmap.PixelFormat := pf24bit;
     Bitmap.Width := knc;
     Bitmap.Height := knl;
     for i := 0 to knl-1 do
     begin
       iBMPs := Bitmap.ScanLine[i];
       for j := 0 to knc-1 do
       begin
          if (cbExp4_LO.Checked) then
             if abs(trunc(Neuronios[i,j].ang[3])) >= trunc(maxi) then
             begin
                //Neuronios[i,j].ang[3] := 255;
             end
             else
             begin
                //Neuronios[i,j].ang[3] := 0;
             end;
          iBMPs[j].r := trunc(Neuronios[i,j].ang[3]);
          iBMPs[j].g := trunc(Neuronios[i,j].ang[3]);
          iBMPs[j].b := trunc(Neuronios[i,j].ang[3]);
       end;
     end;
     // Mostrando Imagem
     mod_ImgExp4_GCanny.Picture.Graphic := Bitmap;
  finally
     Bitmap.Free;
  end;

  //----------------------------------------------------------------------------
  (*
  Componente do Gradiente na Direção -45
  *)
  // Carregando Máscara...
  for i := 0 to 2 do
     for j := 0 to 2 do
        wa[i,j] := strtoint(sgExp4_CN45N.Cells[j,i]);

  a := 1;
  b := a;
  maxi := 0;
  for i := 0 to knL-1 do
     for j := 0 to knC-1 do
     begin
        for s := -a to a do
           for t := -b to b do
              if ((i+s >=0) and (i+s <= knL-1)) and ((j+t >= 0) and (j+t <= knC-1)) then
                 Neuronios[i,j].ang[4] := Neuronios[i,j].ang[4] + (wa[s+1,t+1] * Neuronios[i+s,j+t].ang[0]);

        if (Neuronios[i,j].ang[4] < 0) then Neuronios[i,j].ang[4] := 0;
        if maxi < abs(Neuronios[i,j].ang[4]) then maxi := abs(Neuronios[i,j].ang[4]);
     end;
  // Apresentando Valor Absoluto do Filtro Laplaciano
  maxi := maxi*(SEditLO.Value/100);
  Bitmap := TBitmap.Create;
  try
     Bitmap.PixelFormat := pf24bit;
     Bitmap.Width := knc;
     Bitmap.Height := knl;
     for i := 0 to knl-1 do
     begin
       iBMPs := Bitmap.ScanLine[i];
       for j := 0 to knc-1 do
       begin
          if (cbExp4_LO.Checked) then
             if abs(trunc(Neuronios[i,j].ang[4])) >= trunc(maxi) then
             begin
                //Neuronios[i,j].ang[4] := 255;
             end
             else
             begin
               // Neuronios[i,j].ang[4] := 0;
             end;
          iBMPs[j].r := trunc(Neuronios[i,j].ang[4]);
          iBMPs[j].g := trunc(Neuronios[i,j].ang[4]);
          iBMPs[j].b := trunc(Neuronios[i,j].ang[4]);
       end;
     end;
     // Mostrando Imagem
     mod_ImgExp4_BCanny.Picture.Graphic := Bitmap;
  finally
     Bitmap.Free;
  end;

end;

function TFormProjeto.Normalizar(v,min,max:integer) : double;
var tmp : double;
begin
  tmp := 0;
  if max-min > 0 then tmp := (v-min)/(max-min);
  if (v=max) and (v=min) and (v <> 1) then
     tmp := 1;
  Normalizar := tmp;
end;

function TFormProjeto.gaussian2d(n:integer;sigma:double):TW;
var
   sigmatmp, sigmatmp2 : double;
   s,t,x,y : integer;
begin
   // use constants to speed up processing
   sigmatmp := 2*sigma*sigma;
   sigmatmp2 := 2*pi*sigma*sigma;

   // calculate the gaussian
   s := (n-1) div 2;
   t := s;
   for x := -s to s do  //x
      for y := -t to t do //y
          result[y+t,x+s] := exp(-(sqr(x) + sqr(y)) / (sigmatmp)) / (sigmatmp2);
end;


function TFormProjeto.gaborkernel2d(var n:integer;lambda,sigma,theta,phi,gamma,bandwidth:double):TW;
var
   slratio,xp,yp,a,b,f,gamma2 : double;
   x,y,s,t : integer;
   pos,neg,meansum : double;
begin
   gamma2 := gamma*gamma;
   b := 1 / (2*sigma*sigma);
   a := b / pi;
   f := 2*pi/lambda;

   pos := 0; // sum of the positive elements of result
   neg := 0; // abs value of sum of the negative elements of result
   s := (n-1) div 2;
   t := s;
   for x := -s to s do  //x
      for y := -t to t do //y
      begin
          xp :=  x * cos(theta) + (-y) * sin(theta);
          yp := -x * sin(theta) + (-y) * cos(theta);
          result[y+t,x+s] := a*exp(-b*(xp*xp + gamma2*(yp*yp))) * cos(f*xp + phi);
          if result[y+t,x+s] > 0 then pos := pos + result[y+t,x+s];
          if result[y+t,x+s] < 0 then neg := neg + abs(result[y+t,x+s]);
      end;

   // NORMALIZATION of positive and negative values to ensure that the integral of the kernel is 0.
   // This is needed when phi is different from pi/2.
   meansum := (pos+neg)/2;
   if (meansum > 0) then
   begin
       pos := pos / meansum; // normalization coefficient for negative values of result
       neg := neg / meansum; // normalization coefficient for psoitive values of result
   end;

   for x := -s to s do  //x
      for y := -t to t do //y
      begin
          if result[y+t,x+s] < 0 then result[y+t,x+s] := pos * result[y+t,x+s];
          if result[y+t,x+s] > 0 then result[y+t,x+s] := neg * result[y+t,x+s];
      end;
end;

(*
function TFormProjeto.gaborkernel2d(var n:integer;lambda,sigma,theta,phi,gamma,bandwidth:double):TW;
var
   slratio,xp,yp,a,b,f,gamma2 : double;
   x,y,s,t : integer;
   pos,neg,meansum : double;
begin
// VERSION 2004-04-05
// CREATED BY: N. Petkov and M.B. Wieling, Groningen University
//
// GABORKERNEL2D(LAMBDA, SIGMA, THETA, PHI, GAMMA, BANDWIDTH)
//   fills a (2N+1)*(2N+1) matrix with the values of a 2D Gabor function. N is computed from SIGMA.
//     LAMBDA - preferred wavelength (period of the cosine factor) [in pixels]
//     SIGMA - standard deviation of the Gaussian factor [in pixels]
//     THETA - preferred orientation [in radians]; can be a 1D array
//     PHI   - phase offset [in radians] of the cosine factor; can be a 1D array
//     GAMMA - spatial aspect ratio (of the x- and y-axis of the Gaussian elipse)
//     BANDWIDTH - spatial frequency bandwidth at half response,
//       BANDWIDTH, SIGMA and LAMBDA are interdependent. To use BANDWIDTH,
//       the input value of one of SIGMA or LAMBDA must be 0. Otherwise BANDWIDTH is ignored.
//       The actual value of the parameter whose input value is 0 is computed inside the
//       function from the input vallues of BANDWIDTH and the other parameter.

// calculation of the ratio sigma/lambda from BANDWIDTH
// according to Kruizinga and Petkov, 2002 IEEE Trans on Image Processing 8 (10) p.1396
// note that in Matlab log means ln
   slratio := (1/pi) * sqrt( (ln(2)/2) ) * ( (Power(2,bandwidth) + 1) / (Power(2,bandwidth) - 1) );
// e.g. BANDWITH = 1 will result in the slratio = 0.56

// test if the sigma/lambda ratio is to be used and set sigma or lambda to the correct value
   if (sigma = 0) then
      sigma := slratio * lambda
   else
      if (lambda = 0) then
         lambda := sigma / slratio;

// compute the size of the 2n+1 x 2n+1 matrix to be filled with the values of a Gabor function
// this size depends on sigma and gamma
   if (gamma <= 1) and (gamma > 0) then
    	n := ceil(2.5*sigma/gamma)
   else
    	n := ceil(2.5*sigma);

   n := 2*n+1;
// change direction of y-axis (In Matlab the vertical axis corresponds to the row index
// of a matrix. If the y-coordinates run from -n to n, the lowest value (-n) comes
// in the top row of the matrix ycoords and the highest value (n) in the
// lowest row. This is oposite to the customary rendering of values on the y-axis: lowest value
// in the bottom, highest on the top. Therefore the y-axis is inverted:
   //y = -y;

// xp and yp are the coordinates of a point in a coordinate system rotated by theta.
// They are the main axes of the elipse of the Gaussian factor of the Gabor function.
// The wave vector of the Gabor function is along the xp axis.


// precompute coefficients gamma2=gamma*gamma, b=1/(2*sigma*sigma) and spacial frequency
// f = 2*pi/lambda to prevent multiple evaluations
   gamma2 := gamma*gamma;
   b := 1 / (2*sigma*sigma);
   a := b / pi;
   f := 2*pi/lambda;

   pos := 0; // sum of the positive elements of result
   neg := 0; // abs value of sum of the negative elements of result
   s := (n-1) div 2;
   t := s;
   for x := -s to s do  //x
      for y := -t to t do //y
      begin
          xp :=  x * cos(theta) + (y) * sin(theta);
          yp := -x * sin(theta) + (y) * cos(theta);
          // filling (2n+1) x (2n+1) matrix result with the values of a 2D Gabor function
          //result[x+s,y+t] := a*exp(-b*(xp*xp + gamma2*(yp*yp))) * cos(f*xp + phi);
          result[y+t,x+s] := a*exp(-b*(xp*xp + gamma2*(yp*yp))) * cos(f*xp + phi);

          if result[y+t,x+s] > 0 then pos := pos + result[y+t,x+s];
          if result[y+t,x+s] < 0 then neg := neg + abs(result[y+t,x+s]);
      end;

   // NORMALIZATION of positive and negative values to ensure that the integral of the kernel is 0.
   // This is needed when phi is different from pi/2.
   meansum := (pos+neg)/2;
   if (meansum > 0) then
   begin
       pos := pos / meansum; // normalization coefficient for negative values of result
       neg := neg / meansum; // normalization coefficient for psoitive values of result
   end;

   for x := -s to s do  //x
      for y := -t to t do //y
      begin
          if result[y+t,x+s] < 0 then result[y+t,x+s] := pos * result[y+t,x+s];
          if result[y+t,x+s] > 0 then result[y+t,x+s] := neg * result[y+t,x+s];
      end;

   // Meus testes
   for x := -s to s do  //x
      for y := -t to t do //y
      begin
          if result[y+t,x+s] > 0 then pos := pos + result[y+t,x+s];
          if result[y+t,x+s] < 0 then neg := neg + abs(result[y+t,x+s]);
      end;
   meansum := (pos+neg)/2;
   LABEL88.Caption := formatfloat('0.00000',meansum);

end;
*)


procedure TFormProjeto.normalization(var D:TCMapArray;knl_t,knc_t:integer);
var
   i,j : integer;
   fmin,fmax : double;
begin

  // Normalização - MIN -------------------------------------------------------
     fmin := 255;
     for i := 0 to knl_t do
        for j := 0 to knc_t do
           if fmin > D[i,j] then fmin := D[i,j];

  // Normalizando - MAX --------------------------------------------------------
     fmax := 0; // valor máximo
     for i := 0 to knl_t do
        for j := 0 to knc_t do
        begin
           D[i,j] := D[i,j] - fmin;
           if fmax < D[i,j] then fmax := D[i,j];
        end; // j

  // Normalizando - FIM --------------------------------------------------------
     for i := 0 to knl_t do
        for j := 0 to knc_t do
          if fmax <> 0 then D[i,j] := (trunc(255 * (D[i,j]/fmax)));

end;


{procedure TFormProjeto.normalization(var D:TCMapArray;knl_t,knc_t:integer;vmin,vmax:double);
var
   i,j : integer;
   tmp : double;
begin
     for i := 0 to knl_t-1 do
        for j := 0 to knc_t-1 do
        begin
           try
           tmp := trunc(255 * (D[i,j] - vmin) / (vmax - vmin));
           D[i,j]  := tmp;
           except
              exit;
           end;
        end;   
end;
}

procedure TFormProjeto.normalization_cm(var D:TCMapArray;knl_t,knc_t:integer);
var
   i,j : integer;
   fmin,fmax,Mmax,Mmed,mult : double;

begin

  //(a) Normalize the map activity to a fixedrange [0..M].
     // Normalização - MIN -------------------------------------------------------
     fmin := 255;
     for i := 0 to knl_t do
        for j := 0 to knc_t do
           if fmin > D[i,j] then fmin := D[i,j];

     // Normalizando - MAX --------------------------------------------------------
     fmax := 0; // valor máximo
     for i := 0 to knl_t do
        for j := 0 to knc_t do
        begin
           D[i,j] := D[i,j] - fmin;
           if fmax < D[i,j] then fmax := D[i,j];
        end; // j

     // Normalizando - FIM --------------------------------------------------------
     Mmax := 0;
     Mmed := 0;
     for i := 0 to knl_t do
        for j := 0 to knc_t do
        begin
          if fmax <> 0 then D[i,j] := (trunc(255 * (D[i,j]/fmax)));

          // (b) Find the Maps Global Maximum M
          if Mmax < D[i,j] then Mmax := D[i,j];

          // (c) Compute the average m of all local maximas of the map.
          Mmed := Mmed + D[i,j];
        end;
     Mmed := Mmed / (knl_t*knc_t);

     // (d) Multiply the map with (M-m)^2.
     mult := sqr(Mmax - Mmed);
     for i := 0 to knl_t do
        for j := 0 to knc_t do
             D[i,j] := D[i,j] * mult;

     //normalization(D,knl_t,knc_t);
end;


procedure TFormProjeto.interpolation(var O:TCMapArray;var D:TCMapArray;knl_t,knc_t:integer;f:double);
var i,j,a,s: integer;
begin
   for i := 0 to knl_t-1 do
      for j := 0 to knc_t-1 do
      begin
         if f = 0 then
         begin
           a := i;
           s := j;
         end
         else
            if f < 0 then
            begin
               a := abs(trunc(i/(power(2,abs(f)))));
               s := abs(trunc(j/(power(2,abs(f)))));
            end
            else
            begin
               a := trunc(i*power(2,f));
               s := trunc(j*power(2,f));
            end;
         D[i,j] := O[a,s];
      end;
end;

procedure TFormProjeto.redimensionar(var O:TCMapArray;var D:TCMapArray;nl,nc,new_nl,new_nc:integer);
var i,j,a,s: integer;
    f_l,f_c:double;
begin
   f_l := nl/new_nl;
   f_c := nc/new_nc;
   for i := 0 to new_nl-1 do
      for j := 0 to new_nc-1 do
      begin
         a := trunc(i * f_l);
         s := trunc(j * f_c);
         D[i,j] := O[a,s];
      end;
end;

procedure TFormProjeto.redimensionar_Image(var O:TImage;var D:TBitmap;nl,nc,new_nl,new_nc:integer);
var i,j,a,s: integer;
    f_l,f_c:double;
    //iBMPs : PRGB;
begin
   f_l := nl/new_nl;
   f_c := nc/new_nc;
   for i := 0 to new_nl-1 do
   begin
      //iBMPs := D.ScanLine[i];
      for j := 0 to new_nc-1 do
      begin
         a := trunc(i * f_l);
         s := trunc(j * f_c);
         //iBMPs[j].r := 255;
         //iBMPs[j].g := 0;
         //iBMPs[j].b := 0;
         D.Canvas.Pixels[j,i] := O.Canvas.Pixels[s,a];
      end;
   end;
end;

procedure TFormProjeto.subtraction(var O:TCMapArray;var D:TCMapArray;knl_t,knc_t:integer);
var i,j: integer;
begin
   for i := 0 to knl_t-1 do
      for j := 0 to knc_t-1 do
         D[i,j] := abs(O[i,j]-D[i,j]);

   // Normalizando
   normalization(D,knl_t,knc_t);

end;

procedure TFormProjeto.subtractionColor(var R,O,D:TCMapArray;knl_t,knc_t:integer);
var i,j: integer;
begin
   for i := 0 to knl_t-1 do
      for j := 0 to knc_t-1 do
         r[i,j] := abs(O[i,j] - D[i,j]);

   // Normalizando
   //normalization(r,knl_t,knc_t);
end;


procedure TFormProjeto.IttiModel(Sender: TObject);
var
   n,a,b,i,j,s,t,c,knl_t,knc_t,i_,j_ : integer;
   MCStmp : TCMapArray;
   maxloc_rg,maxloc_by  : array [0..5] of double;
   medloc_rg,medloc_by  : array [0..5] of double;
   medmaxglobal : double;
   Bitmap : TBitmap;
   iBMPs : PRGB;
begin
  // Criação do template para o filtro Gaussiano com tamanho e desvio padrão----------------------
  if seNb.Value mod 2 = 0 then seNb.Value := seNb.Value + 1;
  n := seNb.Value;
  desv_P := strtofloat(seDPb.Text);  //sigma
  w := gaussian2d(n,desv_P);

  // 1 - Suavização da imagem de entrada com o filtro gaussiano ---
  // Nível 0 da Pirâmide
  a := (n-1) div 2;
  b := a;
  for i := 0 to knL-1 do
     for j := 0 to knC-1 do
     begin
        IM[0][i,j][0] := 0; //R
        IM[0][i,j][1] := 0; //G
        IM[0][i,j][2] := 0; //B
        tmp := 0;
        for s := -a to a do
           for t := -b to b do
              if ((i+s >=0) and (i+s <= knL-1)) and ((j+t >= 0) and (j+t <= knC-1)) then
              begin
                  IM[0][i,j][0] := IM[0][i,j][0] + trunc(w[s+a,t+b] * Neuronios[i+s,j+t].rgb[0]);
                  IM[0][i,j][1] := IM[0][i,j][1] + trunc(w[s+a,t+b] * Neuronios[i+s,j+t].rgb[1]);
                  IM[0][i,j][2] := IM[0][i,j][2] + trunc(w[s+a,t+b] * Neuronios[i+s,j+t].rgb[2]);
               end; //if

        // Canais R/G/B/Y
           cor.r := IM[0][i,j][0]; //R
           cor.g := IM[0][i,j][1]; //G
           cor.b := IM[0][i,j][2]; //B

           Mr[0][i,j] := trunc((cor.r-(cor.g+cor.b))/2);
           Mg[0][i,j] := trunc((cor.g-(cor.r+cor.b))/2);
           Mb[0][i,j] := trunc((cor.b-(cor.r+cor.g))/2);
           My[0][i,j] := trunc((cor.r+cor.g)-(2*(abs(cor.r-cor.g)+cor.b)));

     end; //j

  // Passo 01 - Criando a PirâIMde Gaussiana
  // Níveis 1 ao 8
  c := 1;
  knl_t := trunc(knl/2);
  knc_t := trunc(knc/2);
  repeat
     for i := 0 to knl_t-1 do
     begin
        for j := 0 to knc_t-1 do
        begin

             // Suavizando com Gaussiana
             i_ := trunc(i * 2); // a := trunc(i * 2);
             j_ := trunc(j * 2); // s := trunc(j * 2);
             IM[c][i,j][0]:=0;
             IM[c][i,j][1]:=0;
             IM[c][i,j][2]:=0;
             for s := -a to a do
                for t := -b to b do
                   if ((i_+s >=0) and (i_+s <= (knL_t*2)-1)) and ((j_+t >= 0) and (j_+t <= (knC_t*2)-1)) then
                   begin
                       IM[c][i,j][0] := IM[c][i,j][0] + trunc(w[s+a,t+b] * IM[c-1][i_+s,j_+t][0]);
                       IM[c][i,j][1] := IM[c][i,j][1] + trunc(w[s+a,t+b] * IM[c-1][i_+s,j_+t][1]);
                       IM[c][i,j][2] := IM[c][i,j][2] + trunc(w[s+a,t+b] * IM[c-1][i_+s,j_+t][2]);
                    end; //if

             //----------------
               cor.r := IM[c][i,j][0]; //R
               cor.g := IM[c][i,j][1]; //G
               cor.b := IM[c][i,j][2]; //B

               Mr[c][i,j] := trunc((cor.r-(cor.g+cor.b))/2);
               Mg[c][i,j] := trunc((cor.g-(cor.r+cor.b))/2);
               Mb[c][i,j] := trunc((cor.b-(cor.r+cor.g))/2);
               My[c][i,j] := trunc((cor.r+cor.g)-(2*(abs(cor.r-cor.g)+cor.b)));

        end; // j
     end; // i
     inc(c);
     knl_t := trunc(knl_t/2);
     knc_t := trunc(knc_t/2);
  until c = 9;

  (*
  // Normalização
  b := 0;
  knl_t := trunc(knl);
  knc_t := trunc(knc);
  repeat
     normalization(Mr[b],knl_t,knc_t);
     normalization(Mg[b],knl_t,knc_t);
     normalization(Mb[b],knl_t,knc_t);
     normalization(My[b],knl_t,knc_t);
    inc(b);
     knl_t := trunc(knl_t/2);
     knc_t := trunc(knc_t/2);
  until b = 9;
  *)

  //---------------------------------------------------------------------------
  // Interpolação dos Canais c{1,2,3} s{2,3}
  b := 0;
  knl_t := trunc((knl/2));
  knc_t := trunc((knc/2));
  repeat

     if b = 0 then
     begin
        // Cores
           // Color center-surround difference maps (r-g) - (g-r)
           subtractionColor(Mrg,Mr[1],Mg[1],knl_t,knc_t);
           subtractionColor(Mgr,Mg[3],Mr[3],knl_t,knc_t);
           interpolation(Mgr,MCSrg[b],knl_t,knc_t,-2); // 1 <-- 3
           subtraction(Mrg,MCSrg[b],knl_t,knc_t); // 1 - 3;

           // Color center-surround difference maps (y-b) - (b-y)
           subtractionColor(Myb,My[1],Mb[1],knl_t,knc_t);
           subtractionColor(Mby,Mb[3],My[3],knl_t,knc_t);
           interpolation(Mby,MCSby[b],knl_t,knc_t,-2); // 1 <-- 3
           subtraction(Myb,MCSby[b],knl_t,knc_t); // 1 - 3;

        inc(b);

        // Cores
           // Color center-surround difference maps (r-g) - (g-r)
           subtractionColor(Mrg,Mr[1],Mg[1],knl_t,knc_t);
           subtractionColor(Mgr,Mg[4],Mr[4],knl_t,knc_t);
           interpolation(Mgr,MCSrg[b],knl_t,knc_t,-3); // 1 <-- 4
           subtraction(Mrg,MCSrg[b],knl_t,knc_t); // 1 - 4;

           // Color center-surround difference maps (y-b) - (b-y)
           subtractionColor(Myb,My[1],Mb[1],knl_t,knc_t);
           subtractionColor(Mby,Mb[4],My[4],knl_t,knc_t);
           interpolation(Mby,MCSby[b],knl_t,knc_t,-3); // 1 <-- 4
           subtraction(Myb,MCSby[b],knl_t,knc_t); // 1 - 4;

     end;

     if b = 2 then
     begin

        // Cores
           // Color center-surround difference maps (r-g) - (g-r)
           subtractionColor(Mrg,Mr[2],Mg[2],knl_t,knc_t);
           subtractionColor(Mgr,Mg[4],Mr[4],knl_t,knc_t);
           interpolation(Mgr,MCSrg[b],knl_t,knc_t,-2); // 2 <-- 4
           subtraction(Mrg,MCSrg[b],knl_t,knc_t); // 2 - 4;

           // Color center-surround difference maps (y-b) - (b-y)
           subtractionColor(Myb,My[2],Mb[2],knl_t,knc_t);
           subtractionColor(Mby,Mb[4],My[4],knl_t,knc_t);
           interpolation(Mby,MCSby[b],knl_t,knc_t,-2); // 2 <-- 4
           subtraction(Myb,MCSby[b],knl_t,knc_t); // 2 - 4;

        inc(b);


        // Cores
           // Color center-surround difference maps (r-g) - (g-r)
           subtractionColor(Mrg,Mr[2],Mg[2],knl_t,knc_t);
           subtractionColor(Mgr,Mg[5],Mr[5],knl_t,knc_t);
           interpolation(Mgr,MCSrg[b],knl_t,knc_t,-3); // 2 <-- 5
           subtraction(Mrg,MCSrg[b],knl_t,knc_t); // 2 - 5;

           // Color center-surround difference maps (y-b) - (b-y)
           subtractionColor(Myb,My[2],Mb[2],knl_t,knc_t);
           subtractionColor(Mby,Mb[5],My[5],knl_t,knc_t);
           interpolation(Mby,MCSby[b],knl_t,knc_t,-3); // 2 <-- 5
           subtraction(Myb,MCSby[b],knl_t,knc_t); // 2 - 5;

     end;

     if b = 4 then
     begin

        // Cores
           // Color center-surround difference maps (r-g) - (g-r)
           subtractionColor(Mrg,Mr[3],Mg[3],knl_t,knc_t);
           subtractionColor(Mgr,Mg[5],Mr[5],knl_t,knc_t);
           interpolation(Mgr,MCSrg[b],knl_t,knc_t,-2); // 3 <-- 5
           subtraction(Mrg,MCSrg[b],knl_t,knc_t); // 3 - 5;

           // Color center-surround difference maps (y-b) - (b-y)
           subtractionColor(Myb,My[3],Mb[3],knl_t,knc_t);
           subtractionColor(Mby,Mb[5],My[5],knl_t,knc_t);
           interpolation(Mby,MCSby[b],knl_t,knc_t,-2); // 3 <-- 5
           subtraction(Myb,MCSby[b],knl_t,knc_t); // 3 - 5;

        inc(b);


        // Cores
           // Color center-surround difference maps (r-g) - (g-r)
           subtractionColor(Mrg,Mr[3],Mg[3],knl_t,knc_t);
           subtractionColor(Mgr,Mg[6],Mr[6],knl_t,knc_t);
           interpolation(Mgr,MCSrg[b],knl_t,knc_t,-3); // 3 <-- 6
           subtraction(Mrg,MCSrg[b],knl_t,knc_t); // 3 - 6;

           // Color center-surround difference maps (y-b) - (b-y)
           subtractionColor(Myb,My[3],Mb[3],knl_t,knc_t);
           subtractionColor(Mby,Mb[6],My[6],knl_t,knc_t);
           interpolation(Mby,MCSby[b],knl_t,knc_t,-3); // 3 <-- 6
           subtraction(Myb,MCSby[b],knl_t,knc_t); // 3 - 6;

     end;

     inc(b);
     knl_t := trunc(knl_t/2);
     knc_t := trunc(knc_t/2);
  until b >= 5;

  // Geração dos Mapas de Conspicuidades ---------------------------------------
  // Canais c{1,2,3} s{2,3}
  // Interpolação de todos para o nível 3
     b := 0;
     knl_t := trunc((knl/power(2,3)));
     knc_t := trunc((knc/power(2,3)));
     repeat
         case b of
            0..1 : begin
                     MCStmp := MCSrg[b];
                     interpolation(MCStmp,MCSrg_t[b],knl_t,knc_t,2); //3 <-- 1
                     MCStmp := MCSby[b];
                     interpolation(MCStmp,MCSby_t[b],knl_t,knc_t,2); //3 <-- 1
                   end;
            2..3 : begin
                     MCStmp := MCSrg[b];
                     interpolation(MCStmp,MCSrg_t[b],knl_t,knc_t,1); //3 <-- 2
                     MCStmp := MCSby[b];
                     interpolation(MCStmp,MCSby_t[b],knl_t,knc_t,1); //3 <-- 2
                   end;
            4..5 : begin
                     MCStmp := MCSrg[b];
                     interpolation(MCStmp,MCSrg_t[b],knl_t,knc_t,0); //3 <-- 3
                     MCStmp := MCSby[b];
                     interpolation(MCStmp,MCSby_t[b],knl_t,knc_t,0); //3 <-- 3
                   end;
         end;
        inc(b);
     until b = 6;


  // Across-scale addition
  knl_t := trunc((knl/power(2,3)));
  knc_t := trunc((knc/power(2,3)));
  b:=0;
  medmaxglobal := 0;
  repeat
     maxloc_rg[b] := 0;
     maxloc_by[b] := 0;
     medloc_rg[b] := 0;
     medloc_by[b] := 0;
     for i := 0 to knl_t-1 do
        for j := 0 to knc_t-1 do
        begin
           // Maps global maximum
           if maxloc_rg[b] < MCSrg_t[b][i,j] then maxloc_rg[b] := MCSrg_t[b][i,j];
           if maxloc_by[b] < MCSby_t[b][i,j] then maxloc_by[b] := MCSby_t[b][i,j];
        end; //j

     medmaxglobal := medmaxglobal + (maxloc_rg[b] + maxloc_by[b]);
     medloc_rg[b] := medloc_rg[i] + maxloc_rg[b];
     medloc_by[b] := medloc_by[i] + maxloc_by[b];

     inc(b);
  until b = 6;
  medmaxglobal := medmaxglobal / 12;

  // Multiply the map with (M-m)^2.
  b:=0;
  repeat
     for i := 0 to knl_t-1 do
        for j := 0 to knc_t-1 do
        begin
           MCSrg_t[b][i,j] := MCSrg_t[b][i,j] * sqr(maxloc_rg[b]-(medloc_rg[b]/6));
           MCSby_t[b][i,j] := MCSby_t[b][i,j] * sqr(maxloc_by[b]-(medloc_by[b]/6));

           // Simplesmente zerando
           if b = 0 then
           begin
              Cm_c[i,j] := 0;
           end;

        end; //j
     inc(b);
  until b = 6;


  // Normalização
  b:=0;
  repeat
     normalization(MCSrg_t[b],knl_t,knc_t);
     normalization(MCSby_t[b],knl_t,knc_t);
     inc(b);
  until b = 6;


  b:=0;
  repeat

        for i := 0 to knl_t-1 do
           for j := 0 to knc_t-1 do
           begin
              Cm_c[i,j] := Cm_c[i,j] + (MCSrg_t[b][i,j] + MCSby_t[b][i,j]);
           end;

     inc(b);
  until b = 6;
  normalization(Cm_c,knl_t,knc_t);

  // Interpolação para o Tamanho da Imagem de Entrada
  MCStmp := Cm_c;
  interpolation(MCStmp,Cm_c,knl,knc,-3); //3 <-- 3

  // Apresentação de C-------------------------------------------------

  Bitmap := TBitmap.Create;
  try
     Bitmap.PixelFormat := pf24bit;
     Bitmap.Width := knc;
     Bitmap.Height := knl;
     for i := 0 to knl-1 do
     begin
       iBMPs := Bitmap.ScanLine[i];
       for j := 0 to knc-1 do
       begin
          Neuronios[i,j].rgb_Itti[1] := trunc(Cm_c[i,j]);
          iBMPs[j].r := (Neuronios[i,j].rgb_Itti[1]);
          iBMPs[j].g := (Neuronios[i,j].rgb_Itti[1]);
          iBMPs[j].b := (Neuronios[i,j].rgb_Itti[1]);
       end;
     end;
     // Mostrando Imagem
     mod_ImgExp4_Itti_Cm_c.Picture.Graphic := Bitmap;
  finally
     Bitmap.Free;
  end;

   //IF cbATTENTION.Checked THEN
   //SegmentationLEGION(Sender);


end;

procedure TFormProjeto.BitBtn2Click(Sender: TObject);
var
   //Variáveis Locais Gerais
     Bitmap : TBitmap;
     iBMPs, iBMPe24 : PRGB;
     iBMPe8 : PByteArray;
     pixelI : double;
     i,j,a,b,s,t,c,i_,j_ : integer;
     knl_t, knc_t : integer;

   // Variáveis Locais para a Suavização Mediana
     LR,LG,LB : TLista;

   // Vetor de Pesos para Angulos de Direções
     wa : array[0..2,0..2] of integer;
     maxi : double;

   // Variáveis Locais para o LEGION
     //first_pulse,
     sincronizado, seg_ordem, segmentado : boolean;
     checksum, checksum_temp : double;
     n_seg, count, sleft, stop, seg_Validos : integer;
     temp, temp2, temp3  : double;
     neuronios_pulsantes : integer;

     //Segmentos : array of TSeg;
     Seg_TMP : TSeg;
     Segmentos_Pulso : array of record
                        l,c : integer;
                       end;

     //SeG_Imagem : array of TImage;
     SeG_TMP_Imagem : TImage;
     ScrollBox : TScrollBox;

   // MLP
     IMaior, n: integer;
     Valor, Maior: double;

     ttt : double;

     Seg_Medias : TVetorAtributos_ReEscalar;

   // Grafico
   aSurfaceSeries : TSurfaceSeries;

   MCStmp : TCMapArray;
   maxloc_i,maxloc_rg,maxloc_by,maxloc_0,maxloc_1,maxloc_2,maxloc_3  : array [0..5] of double;
   medloc_i,medloc_rg,medloc_by,medloc_0,medloc_1,medloc_2,medloc_3  : array [0..5] of double;

   medmaxglobal : double;


begin
   // Verificando numero de linhas e colunas
   if (knl > Maxknl) or (knc > MaxKnc) then
   begin
      Application.MessageBox('Imagem com dimensões não suportadas. ','Atenção',mb_iconExclamation);
      exit;
   end;

   // Mudando caption do button
   if BitBtn2.Caption = 'Iniciar Processo LEGION' then
      BitBtn2.Caption := 'Para SIMULAÇÃO LEGION'
   else begin
      BitBtn2.Caption := 'Iniciar Processo LEGION';
      CBSeg.OnClick(Sender);
      //PageControl13.TabIndex := 0;
      exit;
   end;

   // Limpando o Gráfico
   CSal.BottomAxis.Minimum := 0;
   CSal.BottomAxis.Maximum := knc;

   // Criando dinamicamente
   aSurfaceSeries := TSurfaceSeries.Create(Self);
   with aSurfaceSeries do
   begin

      //Marks.Visible := False;
      //MidColor := clwhite;//33023;
      NumXValues := knc;
      NumZValues := knl;
      //SideBrush.Color := clWhite;
      //SideBrush.Style := bsClear;
      Pen.Visible := true;
      Pen.Color := clblack;
      //Pen.Style := psdot;
      //SmoothPalette := true;
      //StartColor := clgray;
      //endcolor := clsilver;
      TimesZOrder := trunc(knl*0.11);//20;

      for i := 1 to knl do
         for j := 1 to knc do
             AddXYZ(j,0,i,'',clwhite);
   end;
   CSal.Series[0].Destroy;
   CSal.AddSeries(TSurfaceSeries(aSurfaceSeries));

   CO.Series[0].Clear;

   for i := 0 to 29 do
      COII.Series[i].Clear;

   cont_it := 0;

  //Copiando figura de entrada na Segmentando...
    mod_ImgExp4_IeF_OR.Picture := mod_ImgExp4.Picture;
    mod_ImgExp4_IeF_OR.Visible := false;
    //PageControl13.TabIndex := 5;
    if cbExp4_MP.Checked = false then CBBorda.Checked := false;

  //LEGION----------------------------------------------------------------------
    CBSeg.OnClick(Sender);
    CBSeg.Checked := false;
    Img_Segmentos.Picture := nil;

  //Enviesamento top-down LEGION
    Wc := strtofloat(mskWc.text);
    Wi := strtofloat(mskWi.text);
    Wo := strtofloat(mskWo.text);
    Wsum := strtofloat(mskWsum.text);
    //vc_cor := -1;

  //Copiando Imagem de Entrada somente para vizualização
    mod_ImgExp4B.Picture.Graphic  := mod_ImgExp4.Picture.Graphic;

   // Definindo tamanho do array dinamico
   SetLength(Neuronios,knl,knc);

  //LOCAL
     //raio := strtofloat(mskrini.text);
     //alfa_ini := strtofloat(mskt.Text);
     //alfa := strtofloat(mskt.text);
     //raio_ini := strtofloat(mskrini.Text);
     t_geral := 0;
     fat_dec := 0.9;
     NIt := (knl-1)*(knc-1);
     dt_Sal := strtofloat(mskDt_Sal.Text);
     wInib := strtofloat(mskWInib.Text);

  // Visualização do Mapa SOM
     BitmapK := TBitmap.Create;
     BitmapK.PixelFormat := pf24bit;
     BitmapK.Width := knc;
     BitmapK.Height := knl;

  // Mapa de Atributo Saliente Atributo Saliencia
     BitmapAS := TBitmap.Create;
     BitmapAS.PixelFormat := pf24bit;
     BitmapAS.Width := knc;
     BitmapAS.Height := knl;
     maxl := -1; maxc := -1; max_seg := -1;
     saliency := 0;
     saliency_max :=0;

  // GLOBAL: Visualização da Imagem Original Segmentada
     BitmapS := TBitmap.Create;
     BitmapS.PixelFormat := pf24bit;
     BitmapS.Width := knc;
     BitmapS.Height := knl;
     Segmentos := nil;
     Seg_Imagem := nil;

  // GLOBAL: Carregando base de treinamento da MLP
     SpeedButton1.Click();
     SpeedButton4.Click();
     Treina_SOM := false;

  // LOCAL: Limpando a Imagem em que será Destacado o Objeto Saliente
  Bitmap := TBitmap.Create;
  try
     Bitmap.PixelFormat := pf24bit;
     Bitmap.Width := knc;
     Bitmap.Height := knl;
     for i := 0 to knl-1 do
     begin
       iBMPs := Bitmap.ScanLine[i];
       for j := 0 to knc-1 do
       begin
         iBMPs[j].r := 0;
         iBMPs[j].g := 0;
         iBMPs[j].b := 0;
       end;
     end;
     // Mostrando Imagem
     mod_ImgExp4_IeF.Picture.Graphic := Bitmap;
  finally
     Bitmap.Free;
  end;

  // LOCAL: Limpando a Imagem SOM e SOM SAL
  Bitmap := TBitmap.Create;
  try
     Bitmap.PixelFormat := pf24bit;
     Bitmap.Width := knc;
     Bitmap.Height := knl;
     for i := 0 to knl-1 do
     begin
       iBMPs := Bitmap.ScanLine[i];
       for j := 0 to knc-1 do
       begin
         iBMPs[j].r := 0;
         iBMPs[j].g := 0;
         iBMPs[j].b := 0;
       end;
     end;
     // Mostrando Imagem
     mod_ImgExp4_SOM_SAL.Picture.Graphic := Bitmap;
  finally
     Bitmap.Free;
  end;

  // Inicializando neuronios do SOM
  randomize;
  for i := 0 to knl-1 do
  begin
     for j := 0 to knc-1 do
     begin
         //SOM Intensidades, RG e BY
            if cbExp4_Color.Checked then
            begin
               KNeuronios[i,j][0] := randomrange(0,255);
               KNeuronios[i,j][1] := randomrange(0,255);
               KNeuronios[i,j][2] := randomrange(0,255);
            end else begin
               KNeuronios[i,j][0] := randomrange(0,255);
               KNeuronios[i,j][1] := KNeuronios[i,j][0];
               KNeuronios[i,j][2] := KNeuronios[i,j][0];
            end;
         //SOM Posições Iniciais do Mapa
            KNeuronios[i,j][3] := randomrange(0,knl-1);
            KNeuronios[i,j][4] := randomrange(0,knc-1);
         //SOM Angulos Iniciais do Mapa
            KNeuronios[i,j][5] := randomrange(0,255);
            KNeuronios[i,j][6] := randomrange(0,255);
            KNeuronios[i,j][7] := randomrange(0,255);
            KNeuronios[i,j][8] := randomrange(0,255);

          //SOM Reconhecimento
            KNeuronios[i,j][9] := 0;//randomrange(0,255);

         //SOM R,G,B
            KNeuronios[i,j][10] := randomrange(0,255);
            KNeuronios[i,j][11] := randomrange(0,255);
            KNeuronios[i,j][12] := randomrange(0,255);

         //Mapa de Atributo Saliencia Neuronios
            //SNeuronios[i,j].i := 0;
            //SNeuronios[i,j].v := 0;
            //SNeuronios[i,j].y := 0;
            //SNeuronios[i,j].rn := 0;

        end;
   end;

  // Atualizando os Valores de Orientação, I, RG, BY
  for i := 0 to knl-1 do
  begin
     for j := 0 to knc-1 do
     begin

         // Orientações
         Neuronios[i,j].ang[0] := Neuronios[i,j].rgb[4];
         Neuronios[i,j].ang[1] := 0; // Gradiente na Direção Horizontal
         Neuronios[i,j].ang[2] := 0; // Gradiente na Direção Vertical
         Neuronios[i,j].ang[3] := 0; // Gradiente na Direção +45
         Neuronios[i,j].ang[4] := 0; // Gradiente na Direção -45

         // I, RG, BY
         Neuronios[i,j].rgb_Itti[0] := Neuronios[i,j].rgb[4]; // Itti Intensidades
         PixelI := 0;
         maxi := max(Neuronios[i,j].rgbSColor[0],max(Neuronios[i,j].rgbSColor[1],Neuronios[i,j].rgbSColor[2]));
         if maxi > 0 then PixelI := ((abs(Neuronios[i,j].rgbSColor[0] - Neuronios[i,j].rgbSColor[1]))/maxi);
         Neuronios[i,j].rgb_Itti[1] := trunc(PixelI*255); // Itti RG
         PixelI := 0;
         if maxi > 0 then PixelI := ((abs(Neuronios[i,j].rgbSColor[2] - min(Neuronios[i,j].rgbSColor[0],Neuronios[i,j].rgbSColor[1])))/maxi);
         Neuronios[i,j].rgb_Itti[2] := trunc(PixelI*255); // Itti BY

     end;
  end;

  // É aqui que a porca torce o raboooooooooo

  // Criação do template para o filtro Gaussiano com tamanho e desvio padrão----------------------
  if seNb.Value mod 2 = 0 then seNb.Value := seNb.Value + 1;
  n := seNb.Value;
  desv_P := strtofloat(seDPb.Text);  //sigma
  w := gaussian2d(n,desv_P);

  // 1 - Suavização da imagem de entrada com o filtro gaussiano ---
  // Nível 0 da Pirâmide
  a := (n-1) div 2;
  b := a;
  for i := 0 to knL-1 do
     for j := 0 to knC-1 do
     begin
        IM[0][i,j][0] := 0; //R
        IM[0][i,j][1] := 0; //G
        IM[0][i,j][2] := 0; //B
        tmp := 0;
        for s := -a to a do
           for t := -b to b do
              if ((i+s >=0) and (i+s <= knL-1)) and ((j+t >= 0) and (j+t <= knC-1)) then
              begin
                  IM[0][i,j][0] := IM[0][i,j][0] + trunc(w[s+a,t+b] * Neuronios[i+s,j+t].rgb[0]);
                  IM[0][i,j][1] := IM[0][i,j][1] + trunc(w[s+a,t+b] * Neuronios[i+s,j+t].rgb[1]);
                  IM[0][i,j][2] := IM[0][i,j][2] + trunc(w[s+a,t+b] * Neuronios[i+s,j+t].rgb[2]);
               end; //if

        // Canais R/G/B/Y
           cor.r := IM[0][i,j][0]; //R
           cor.g := IM[0][i,j][1]; //G
           cor.b := IM[0][i,j][2]; //B

           Mr[0][i,j] := trunc((cor.r-(cor.g+cor.b))/2);
           Mg[0][i,j] := trunc((cor.g-(cor.r+cor.b))/2);
           Mb[0][i,j] := trunc((cor.b-(cor.r+cor.g))/2);
           My[0][i,j] := trunc((cor.r+cor.g)-(2*(abs(cor.r-cor.g)+cor.b)));

     end; //j

  // Passo 01 - Criando a PirâIMde Gaussiana
  // Níveis 1 ao 8
  c := 1;
  knl_t := trunc(knl/2);
  knc_t := trunc(knc/2);
  repeat
     for i := 0 to knl_t-1 do
     begin
        for j := 0 to knc_t-1 do
        begin

             // Suavizando com Gaussiana
             i_ := trunc(i * 2); // a := trunc(i * 2);
             j_ := trunc(j * 2); // s := trunc(j * 2);
             IM[c][i,j][0]:=0;
             IM[c][i,j][1]:=0;
             IM[c][i,j][2]:=0;
             for s := -a to a do
                for t := -b to b do
                   if ((i_+s >=0) and (i_+s <= (knL_t*2)-1)) and ((j_+t >= 0) and (j_+t <= (knC_t*2)-1)) then
                   begin
                       IM[c][i,j][0] := IM[c][i,j][0] + trunc(w[s+a,t+b] * IM[c-1][i_+s,j_+t][0]);
                       IM[c][i,j][1] := IM[c][i,j][1] + trunc(w[s+a,t+b] * IM[c-1][i_+s,j_+t][1]);
                       IM[c][i,j][2] := IM[c][i,j][2] + trunc(w[s+a,t+b] * IM[c-1][i_+s,j_+t][2]);
                    end; //if

             //----------------
               cor.r := IM[c][i,j][0]; //R
               cor.g := IM[c][i,j][1]; //G
               cor.b := IM[c][i,j][2]; //B

               Mr[c][i,j] := trunc((cor.r-(cor.g+cor.b))/2);
               Mg[c][i,j] := trunc((cor.g-(cor.r+cor.b))/2);
               Mb[c][i,j] := trunc((cor.b-(cor.r+cor.g))/2);
               My[c][i,j] := trunc((cor.r+cor.g)-(2*(abs(cor.r-cor.g)+cor.b)));

        end; // j
     end; // i
     inc(c);
     knl_t := trunc(knl_t/2);
     knc_t := trunc(knc_t/2);
  until c = 9;

  (*
  // Normalização
  b := 0;
  knl_t := trunc(knl);
  knc_t := trunc(knc);
  repeat
     normalization(Mr[b],knl_t,knc_t);
     normalization(Mg[b],knl_t,knc_t);
     normalization(Mb[b],knl_t,knc_t);
     normalization(My[b],knl_t,knc_t);
    inc(b);
     knl_t := trunc(knl_t/2);
     knc_t := trunc(knc_t/2);
  until b = 9;
  *)

  //---------------------------------------------------------------------------
  // Interpolação dos Canais c{1,2,3} s{2,3}
  b := 0;
  knl_t := trunc((knl/2));
  knc_t := trunc((knc/2));
  repeat

     if b = 0 then
     begin
        // Cores
           // Color center-surround difference maps (r-g) - (g-r)
           subtractionColor(Mrg,Mr[1],Mg[1],knl_t,knc_t);
           subtractionColor(Mgr,Mg[3],Mr[3],knl_t,knc_t);
           interpolation(Mgr,MCSrg[b],knl_t,knc_t,-2); // 1 <-- 3
           subtraction(Mrg,MCSrg[b],knl_t,knc_t); // 1 - 3;

           // Color center-surround difference maps (y-b) - (b-y)
           subtractionColor(Myb,My[1],Mb[1],knl_t,knc_t);
           subtractionColor(Mby,Mb[3],My[3],knl_t,knc_t);
           interpolation(Mby,MCSby[b],knl_t,knc_t,-2); // 1 <-- 3
           subtraction(Myb,MCSby[b],knl_t,knc_t); // 1 - 3;

        inc(b);

        // Cores
           // Color center-surround difference maps (r-g) - (g-r)
           subtractionColor(Mrg,Mr[1],Mg[1],knl_t,knc_t);
           subtractionColor(Mgr,Mg[4],Mr[4],knl_t,knc_t);
           interpolation(Mgr,MCSrg[b],knl_t,knc_t,-3); // 1 <-- 4
           subtraction(Mrg,MCSrg[b],knl_t,knc_t); // 1 - 4;

           // Color center-surround difference maps (y-b) - (b-y)
           subtractionColor(Myb,My[1],Mb[1],knl_t,knc_t);
           subtractionColor(Mby,Mb[4],My[4],knl_t,knc_t);
           interpolation(Mby,MCSby[b],knl_t,knc_t,-3); // 1 <-- 4
           subtraction(Myb,MCSby[b],knl_t,knc_t); // 1 - 4;

     end;

     if b = 2 then
     begin

        // Cores
           // Color center-surround difference maps (r-g) - (g-r)
           subtractionColor(Mrg,Mr[2],Mg[2],knl_t,knc_t);
           subtractionColor(Mgr,Mg[4],Mr[4],knl_t,knc_t);
           interpolation(Mgr,MCSrg[b],knl_t,knc_t,-2); // 2 <-- 4
           subtraction(Mrg,MCSrg[b],knl_t,knc_t); // 2 - 4;

           // Color center-surround difference maps (y-b) - (b-y)
           subtractionColor(Myb,My[2],Mb[2],knl_t,knc_t);
           subtractionColor(Mby,Mb[4],My[4],knl_t,knc_t);
           interpolation(Mby,MCSby[b],knl_t,knc_t,-2); // 2 <-- 4
           subtraction(Myb,MCSby[b],knl_t,knc_t); // 2 - 4;

        inc(b);


        // Cores
           // Color center-surround difference maps (r-g) - (g-r)
           subtractionColor(Mrg,Mr[2],Mg[2],knl_t,knc_t);
           subtractionColor(Mgr,Mg[5],Mr[5],knl_t,knc_t);
           interpolation(Mgr,MCSrg[b],knl_t,knc_t,-3); // 2 <-- 5
           subtraction(Mrg,MCSrg[b],knl_t,knc_t); // 2 - 5;

           // Color center-surround difference maps (y-b) - (b-y)
           subtractionColor(Myb,My[2],Mb[2],knl_t,knc_t);
           subtractionColor(Mby,Mb[5],My[5],knl_t,knc_t);
           interpolation(Mby,MCSby[b],knl_t,knc_t,-3); // 2 <-- 5
           subtraction(Myb,MCSby[b],knl_t,knc_t); // 2 - 5;

     end;

     if b = 4 then
     begin

        // Cores
           // Color center-surround difference maps (r-g) - (g-r)
           subtractionColor(Mrg,Mr[3],Mg[3],knl_t,knc_t);
           subtractionColor(Mgr,Mg[5],Mr[5],knl_t,knc_t);
           interpolation(Mgr,MCSrg[b],knl_t,knc_t,-2); // 3 <-- 5
           subtraction(Mrg,MCSrg[b],knl_t,knc_t); // 3 - 5;

           // Color center-surround difference maps (y-b) - (b-y)
           subtractionColor(Myb,My[3],Mb[3],knl_t,knc_t);
           subtractionColor(Mby,Mb[5],My[5],knl_t,knc_t);
           interpolation(Mby,MCSby[b],knl_t,knc_t,-2); // 3 <-- 5
           subtraction(Myb,MCSby[b],knl_t,knc_t); // 3 - 5;

        inc(b);


        // Cores
           // Color center-surround difference maps (r-g) - (g-r)
           subtractionColor(Mrg,Mr[3],Mg[3],knl_t,knc_t);
           subtractionColor(Mgr,Mg[6],Mr[6],knl_t,knc_t);
           interpolation(Mgr,MCSrg[b],knl_t,knc_t,-3); // 3 <-- 6
           subtraction(Mrg,MCSrg[b],knl_t,knc_t); // 3 - 6;

           // Color center-surround difference maps (y-b) - (b-y)
           subtractionColor(Myb,My[3],Mb[3],knl_t,knc_t);
           subtractionColor(Mby,Mb[6],My[6],knl_t,knc_t);
           interpolation(Mby,MCSby[b],knl_t,knc_t,-3); // 3 <-- 6
           subtraction(Myb,MCSby[b],knl_t,knc_t); // 3 - 6;

     end;

     inc(b);
     knl_t := trunc(knl_t/2);
     knc_t := trunc(knc_t/2);
  until b >= 5;



  // Geração dos Mapas de Conspicuidades ---------------------------------------
  // Canais c{1,2,3} s{2,3}
  // Interpolação de todos para o nível 3
     b := 0;
     knl_t := trunc((knl/power(2,3)));
     knc_t := trunc((knc/power(2,3)));
     repeat
         case b of
            0..1 : begin
                     MCStmp := MCSrg[b];
                     interpolation(MCStmp,MCSrg_t[b],knl_t,knc_t,2); //3 <-- 1
                     MCStmp := MCSby[b];
                     interpolation(MCStmp,MCSby_t[b],knl_t,knc_t,2); //3 <-- 1
                   end;
            2..3 : begin
                     MCStmp := MCSrg[b];
                     interpolation(MCStmp,MCSrg_t[b],knl_t,knc_t,1); //3 <-- 2
                     MCStmp := MCSby[b];
                     interpolation(MCStmp,MCSby_t[b],knl_t,knc_t,1); //3 <-- 2
                   end;
            4..5 : begin
                     MCStmp := MCSrg[b];
                     interpolation(MCStmp,MCSrg_t[b],knl_t,knc_t,0); //3 <-- 3
                     MCStmp := MCSby[b];
                     interpolation(MCStmp,MCSby_t[b],knl_t,knc_t,0); //3 <-- 3
                   end;
         end;
        inc(b);
     until b = 6;


  // Across-scale addition
  knl_t := trunc((knl/power(2,3)));
  knc_t := trunc((knc/power(2,3)));
  b:=0;
  medmaxglobal := 0;
  repeat
     maxloc_rg[b] := 0;
     maxloc_by[b] := 0;

     medloc_rg[b] := 0;
     medloc_by[b] := 0;
     for i := 0 to knl_t-1 do
        for j := 0 to knc_t-1 do
        begin
           // Maps global maximum
           if maxloc_rg[b] < MCSrg_t[b][i,j] then maxloc_rg[b] := MCSrg_t[b][i,j];
           if maxloc_by[b] < MCSby_t[b][i,j] then maxloc_by[b] := MCSby_t[b][i,j];
        end; //j

     medmaxglobal := medmaxglobal + (maxloc_rg[b] + maxloc_by[b]);

     medloc_rg[b] := medloc_rg[i] + maxloc_rg[b];
     medloc_by[b] := medloc_by[i] + maxloc_by[b];

     inc(b);
  until b = 6;
  medmaxglobal := medmaxglobal / 12;

  // Multiply the map with (M-m)^2.
  b:=0;
  repeat
     for i := 0 to knl_t-1 do
        for j := 0 to knc_t-1 do
        begin
           MCSrg_t[b][i,j] := MCSrg_t[b][i,j] * sqr(maxloc_rg[b]-(medloc_rg[b]/6));
           MCSby_t[b][i,j] := MCSby_t[b][i,j] * sqr(maxloc_by[b]-(medloc_by[b]/6));

           // Simplesmente zerando
           if b = 0 then
           begin
              Cm_i[i,j] := 0;
              Cm_c[i,j] := 0;
              Cm_o[i,j] := 0;
           end;

        end; //j
     inc(b);
  until b = 6;


  // Normalização
  b:=0;
  repeat
     normalization(MCSrg_t[b],knl_t,knc_t);
     normalization(MCSby_t[b],knl_t,knc_t);
     inc(b);
  until b = 6;


  b:=0;
  repeat

        for i := 0 to knl_t-1 do
           for j := 0 to knc_t-1 do
           begin
              Cm_c[i,j] := Cm_c[i,j] + (MCSrg_t[b][i,j] + MCSby_t[b][i,j]);
           end;

     inc(b);
  until b = 6;
  normalization(Cm_c,knl_t,knc_t);

  // Interpolação para o Tamanho da Imagem de Entrada
  MCStmp := Cm_c;
  interpolation(MCStmp,Cm_c,knl,knc,-3); //3 <-- 3


  // Apresentação de C-------------------------------------------------

  Bitmap := TBitmap.Create;
  try
     Bitmap.PixelFormat := pf24bit;
     Bitmap.Width := knc;
     Bitmap.Height := knl;
     for i := 0 to knl-1 do
     begin
       iBMPs := Bitmap.ScanLine[i];
       for j := 0 to knc-1 do
       begin
          Neuronios[i,j].rgb_Itti[1] := trunc(Cm_c[i,j]);
          iBMPs[j].r := (Neuronios[i,j].rgb_Itti[1]);
          iBMPs[j].g := (Neuronios[i,j].rgb_Itti[1]);
          iBMPs[j].b := (Neuronios[i,j].rgb_Itti[1]);
       end;
     end;
     // Mostrando Imagem
     mod_ImgExp4_Itti_Cm_c.Picture.Graphic := Bitmap;
  finally
     Bitmap.Free;
  end;

  //exit;

  // Parâmetros do LEGION
  // Randomizando os valores iniciais...
  tmp := 0;

  // Inicializando neuronios LEGION
  for i:=0 to knl-1 do
    for j:=0 to knc-1 do
    begin

      neuronios[i,j].matrix := 0;
      neuronios[i,j].ly := rnd();
      neuronios[i,j].lx[0] := neuronios[i,j].ly / (MAXVAL+1);
      neuronios[i,j].ly := neuronios[i,j].ly - 2;
      if (neuronios[i,j].ly < 0) then neuronios[i,j].ly := 0;
      neuronios[i,j].trigger := 0;
      neuronios[i,j].cycle := 0;

      neuronios[i,j].d := 0;
      Neuronios[i,j].stim := neuronios[i,j].rgb[4];
    end;
  z := 0;

  // Verificando Conexões
  connections;

  // evaluate...
  jump[0] := 0;
  select();

  first_pulse := false;
  sincronizado := false;
  checksum := 0;
  n_seg := 0;
  seg_Ordem := false;
  temp :=0; temp2:=0; temp3:=0.0;
  count := 0;
  n := 0;

  timer.Enabled := true;
  while (timer.Enabled = true) do
  begin
     Application.ProcessMessages;
     inc(tmp);
     count := 0;

     if (timer6.Enabled = false) then
     begin

     // calculando entradas da rede input
     for i:=0 to knl-1 do
       for j:=0 to knc-1 do
       begin

          //Application.ProcessMessages;

				  if (neuronios[i,j].lx[0] > thetae) and (jump[0] = 0) then
          begin  //* jumping down */
    					neuronios[i,j].lx[1] := LP;
    					neuronios[i,j].ly := -1;
    					neuronios[i,j].cycle := 1;
    					inc(count);
				  end else
              if (neuronios[i,j].lx[0] < thetae) then
              begin     //* summing up overall input */
                 temp :=  -Wz * bi(z,thetaz);  //* used to be 50 */

        				 //* the following is based on traversing through strongest link */
      					 temp2 := 0.0;
       			  	 if (j-1 >= 0) then
                 begin
						        temp3 := neuronios[i,j].W[0] * bi(neuronios[i,j-1].lx[0], thetae);
      					  	if (temp2 < temp3) then temp2 := temp3;
                 end;

				         if (i-1 >= 0) and (j-1 >= 0) then
                 begin
        						temp3 := neuronios[i,j].W[1] * bi(neuronios[i-1,j-1].lx[0], thetae);
      					    if (temp2 < temp3) then temp2 := temp3;
                 end;

        				 if (i-1 >= 0) then
                 begin
        						temp3 := neuronios[i,j].W[2] * bi(neuronios[i-1,j].lx[0], thetae);
        						if (temp2 < temp3) then temp2 := temp3;
                 end;

        				 if (i-1 >= 0) and (j+1 < knc) then
                 begin
        						temp3 := neuronios[i,j].W[3] * bi(neuronios[i-1,j+1].lx[0], thetae);
        						if (temp2 < temp3) then temp2 := temp3;
                 end;

        				 if (j+1 < knc) then
                 begin
        						temp3 := neuronios[i,j].W[4] * bi(neuronios[i,j+1].lx[0], thetae);
        						if (temp2 < temp3) then temp2 := temp3;
                 end;

        				 if (i+1 < knl) and (j+1 < knc) then
                 begin
        						temp3 := neuronios[i,j].W[5] * bi(neuronios[i+1,j+1].lx[0], thetae);
        						if (temp2 < temp3) then temp2 := temp3;
                 end;

        				 if (i+1 < knl) then
                 begin
        						temp3 := neuronios[i,j].W[6] * bi(neuronios[i+1,j].lx[0], thetae);
        						if (temp2 < temp3) then temp2 := temp3;
                 end;

        				 if (i+1 < knl) and (j-1 >= 0) then
                 begin
        						temp3 := neuronios[i,j].W[7] * bi(neuronios[i+1,j-1].lx[0], thetae);
        						if (temp2 < temp3) then temp2 := temp3;
                 end;

        					temp := temp + temp2;

        					if (neuronios[i,j].trigger = 1) or (temp > 0.25) then
                  begin               //* jumping up */
          						neuronios[i,j].lx[1] := RP;
             					neuronios[i,j].ly := MAXIMO;
            					jump[1] := 1;
            					inc(z);
                  end else
        					  	neuronios[i,j].Lx[1] := neuronios[i,j].Lx[0];
              end else
              begin
   					  	 neuronios[i,j].Lx[1] := neuronios[i,j].Lx[0];
              end;

    	 end;

     z := z - count;

  	 if (jump[1] = 0) and (z = 0) then
      	select();

     // Apresentando...

     // Mostra neuronios sincronizados...
     if (z > 0) then
     begin
        segmentado := true;
        for i := 0 to knl-1 do
           for j := 0 to knc-1 do
               if (Neuronios[i,j].d = 0) and (Neuronios[i,j].Lx[1] >= thetae) then
               begin
                   segmentado := false;
                   continue;
               end;

        if cbVer_Sincro.Checked then
           ShowSincro_LEGION
        else
        begin
           ShowSincro_LEGION_no_Preview;
           if segmentado = true then
              ShowSincro_LEGION;
        end;

        // Gravando Segmento
        checksum_temp := 0;

        if (segmentado = true) and (n_seg = 1) then
           checksum_temp := checksum;

        if segmentado = true then
        begin

           for i := imin+1 to imax-1 do
              for j := jmin+1 to jmax-1 do           //( testando       )
                  if (Neuronios[i,j].lx[1] >= thetae) and (Neuronios[i,j].d = 1) then
                     if first_pulse = false then
                     begin
                        checksum := checksum + (-checksum + (i+j)*0.1);
                        n_seg := 0;
                     end
                     else
                        checksum_temp := checksum_temp + (-checksum_temp + (i+j)*0.1);

           first_pulse := true;

           // Verificando se Sincronizou
           if (checksum = checksum_temp) then
           begin
              checksum := 0; first_pulse := false; n_seg := 0;
              sincronizado := true;

              Sleft := 3; Stop := 3;

              setlength(Seg_Imagem,length(Segmentos));

              Scrollbox := TScrollBox.Create(FormProjeto);
              Scrollbox.Parent := Tabsheet13;
              Scrollbox.Align := alClient;

              for s := 0 to length(Segmentos)-1 do
              begin

                 Seg_Imagem[s] := TImage.Create(FormProjeto);
                 Seg_Imagem[s].Parent := ScrollBox;
                 Seg_Imagem[s].Height := 180;
                 Seg_Imagem[s].Width := 180;
                 Seg_Imagem[s].AutoSize := false;
                 Seg_Imagem[s].Center := true;
                 Seg_Imagem[s].Proportional := true;
                 Seg_Imagem[s].Left := sleft;
                 Seg_Imagem[s].Top := stop;
                 Seg_Imagem[s].OnClick := Zoom;
                 sleft := sleft + 6 + Seg_Imagem[s].Width;
                 if sleft > 700 then
                 begin
                    sleft := 3;
                    stop := stop + Seg_Imagem[s].Height + 6;
                 end;

                 Bitmap := TBitmap.Create;
                 try

                    Bitmap.PixelFormat := pf24bit;
                    Bitmap.Width := Segmentos[s].Sknc;
                    Bitmap.Height := Segmentos[s].Sknl;
                    for i := 0 to Segmentos[s].Sknl-1 do
                    begin
                       iBMPs := Bitmap.ScanLine[i];
                       for j := 0 to Segmentos[s].Sknc-1 do
                       begin

                          //----------------------------------------------------
                          if (Segmentos[s].SegNeuronios[i,j].Lx[1] >= thetae) and
                             (Segmentos[s].SegNeuronios[i,j].d = 1) then
                          begin

                             if cbExp4_Cinza.Checked then
                             begin
                                iBMPs[j].r := Segmentos[s].SegNeuronios[i,j].rgb[4];
                                iBMPs[j].g := Segmentos[s].SegNeuronios[i,j].rgb[4];
                                iBMPs[j].b := Segmentos[s].SegNeuronios[i,j].rgb[4];
                             end
                             else
                                if cbExp4_Cores.Checked then
                                begin
                                    iBMPs[j].r := trunc(Segmentos[s].SegNeuronios[i,j].Seg_rgb[0]);
                                    iBMPs[j].g := trunc(Segmentos[s].SegNeuronios[i,j].Seg_rgb[1]);
                                    iBMPs[j].b := trunc(Segmentos[s].SegNeuronios[i,j].Seg_rgb[2]);
                                end
                                else
                                begin
                                    iBMPs[j].r := trunc(Segmentos[s].SegNeuronios[i,j].lx[1]*255);
                                    iBMPs[j].g := trunc(Segmentos[s].SegNeuronios[i,j].lx[1]*255);
                                    iBMPs[j].b := trunc(Segmentos[s].SegNeuronios[i,j].lx[1]*255);
                                end;
                          end
                          else begin
                             //if (cbExp4_MP.Checked) then
                             begin
                                iBMPs[j].r := 0;
                                iBMPs[j].g := 0;
                                iBMPs[j].b := 0;
                             end;
                          end;
                          //------------------------------------------------------------------------------

                          //MLP.SetInput(i*13+j+1,Bitmap.Canvas.Pixels[i,j] shr 23); // 0 ou 1

                       end;
                    end;
                    // Mostrando Imagem
                    Seg_Imagem[s].Picture.Graphic := Bitmap;
                    //sleep(2000);
                 finally
                    Bitmap.Free;
                 end;


                 //if (Segmentos[s].Sknl = 13) and (Segmentos[s].Sknc = 13) then
                 begin
                 // Verificando Classificação MLP
                 for i := 0 to nc do
                    for j := 0 to nl do
                  //for i := 0 to Segmentos[s].Sknl-1 do
                    //for j := 0 to Segmentos[s].Sknc-1 do
                    begin
                       n := i*nc+j+1; // índice do neurônio de entrada
                       //if (Segmentos[s].SegNeuronios[i,j].Lx[1] >= thetae) and
                          //(Segmentos[s].SegNeuronios[i,j].d = 1) then
                          //ttt := Seg_Imagem[s].Picture.Bitmap.Canvas.Pixels[j,i] shr 23;
                          //MLP.SetInput(n,Seg_Imagem[s].Picture.Bitmap.Canvas.Pixels[j,i] shr 23); // 0 ou 1
                          //MLP.SetInput(n,0)
                       //else MLP.SetInput(n,1);    // 0 ou 1
                    end;
                 //MLP.Test;
                 {
                 mod_ed0.Text := Format('%2.4f',[MLP.GetOutput(1)]);
                 mod_ed1.Text := Format('%2.4f',[MLP.GetOutput(2)]);
                 mod_ed2.Text := Format('%2.4f',[MLP.GetOutput(3)]);
                 mod_ed3.Text := Format('%2.4f',[MLP.GetOutput(4)]);
                 }
                 { Verifica a maior saída para indicação visual }
                 Maior := -10;
                 for i := 1 to snClasses.Value do
                 begin
                     //Valor := MLP.GetOutput(i);
                     if Valor > Maior then
                     begin
                        Maior := Valor;
                        IMaior := i;
                     end;
                 end;

                 { Todas com fundo branco }
                 mod_sp0.Brush.Color := clWhite;
                 mod_sp1.Brush.Color := clWhite;
                 mod_sp2.Brush.Color := clWhite;
                 mod_sp3.Brush.Color := clWhite;

                 { Se semelhança for maior que 0.95}
                 if (Maior > 0.95) and (Maior < 1.00) then
                 begin
                     { Exceto a maior }
                      case IMaior of
                          1:  mod_sp0.Brush.Color := clMoneyGreen;
                          2:  mod_sp1.Brush.Color := clMoneyGreen;
                          3:  mod_sp2.Brush.Color := clMoneyGreen;
                          4:  mod_sp3.Brush.Color := clMoneyGreen;
                      end;


                     { Alimentando neurônios reconhecidos pela MLP}
                      for i := 0 to Segmentos[s].Sknl-1 do
                           for j := 0 to Segmentos[s].Sknc-1 do
                               if (Segmentos[s].SegNeuronios[i,j].Lx[1] >= thetae) and
                                  (Segmentos[s].SegNeuronios[i,j].d = 1) then
                               begin
                                   //Neuronios[i+Segmentos[s].imin,j+Segmentos[s].jmin].rn := 255;
                                   //Segmentos[s].SegNeuronios[i,j].rn := 255;
                               end;


                 end; // se for de tamanho 13
                 end;

                 { Alimentando neurônios reconhecidos pela MLP}
                   for i := 0 to Segmentos[s].Sknl-1 do
                        for j := 0 to Segmentos[s].Sknc-1 do
                            if (Segmentos[s].SegNeuronios[i,j].Lx[1] >= thetae) and
                               (Segmentos[s].SegNeuronios[i,j].d = 1) then
                            begin
                                Segmentos[s].SegNeuronios[i,j].rn :=
                                    Neuronios[i+Segmentos[s].imin,j+Segmentos[s].jmin].rn;
                            end;

              end;//-------------


              ///////////////////
              //if cbExp4_MP.Checked = false then
                 for i := 0 to knl-1 do
                    for j := 0 to knc-1 do
                    begin
                       iBMPs := mod_ImgExp4_IeF.Picture.Bitmap.ScanLine[i];
                       if (iBMPs[j].r = 255) and (iBMPs[j].g = 255) and
                          (iBMPs[j].b = 255) then
                       begin
                          iBMPs[j].r := 0; iBMPs[j].g := 0; iBMPs[j].b := 0;
                       end;
                       Neuronios[i,j].rgb[0] := iBMPs[j].r;
                       Neuronios[i,j].rgb[1] := iBMPs[j].g;
                       Neuronios[i,j].rgb[2] := iBMPs[j].b;


                    end;

              ///////////////////

              if cbVer_Just_S.checked = true then
              begin
                 //Atualiza_para_SOM;

                 BitBtn2.OnClick(Sender);
              end
              else begin
              // Saindo da Sincronização ... e habilitando o Kohonem

                 //Atualiza_para_SOM;

                 //PageControl13.TabIndex := 8;
                 //BitBtn35.OnClick(Sender); // exit;
                 timer6.Enabled := true;
                 //PageControl13.TabIndex := 8;

                 //BitBtn36.OnClick(Sender);
                 //PageControl13.TabIndex := 10;
                         // exit;

              end;

           end
           else // grava o segmento
           begin

              setlength(Segmentos,n_seg+1); setlength(Seg_Classify,n_seg+1);
              setlength(Segmentos[n_seg].SegNeuronios,imax-imin+1,jmax-jmin+1);
              Segmentos[n_seg].Sknl := imax-imin+1;
              Segmentos[n_seg].Sknc := jmax-jmin+1;
              Segmentos[n_seg].imin := imin;
              Segmentos[n_seg].imax := imax;
              Segmentos[n_seg].jmin := jmin;
              Segmentos[n_seg].jmax := jmax;

              for i := imin to imax do
                 for j := jmin to jmax do
                 begin
                    if (Neuronios[i,j].Lx[1] >= thetae) and
                       (Neuronios[i,j].d = 1) then
                    begin
                        Segmentos[n_seg].SegNeuronios[i-imin,j-jmin] := Neuronios[i,j];
                        Neuronios[i,j].segN := n_seg;
                        for s := 0 to 12 do Segmentos[n_seg].SegMedias[s] := 0;
                    end;
                 end;

              inc(n_seg);
              Label68.caption := inttostr(n_seg);
           end;

        end;

     end;  // z > 0

     //if (cbExp4_MP.Checked) then
     sleep(trunc((tbpulso.Position/10)*1000));

     for i:=0 to knl-1 do   //* replace OLD by NEW */
       for j:=0 to knc-1 do
  				neuronios[i,j].lx[0] := neuronios[i,j].lx[1];

		 jump[0] := jump[1];
		 jump[1] := 0;

     end // if timer6 false
     else begin

         //Showmessage ('Chegueiiiii');
         //exit;

         // Ordenação dos segmentos pelo menor...
         //Seg_TMP_Imagem := TImage.Create(FormProjeto);

         if seg_Ordem = false then
         begin
            {for s := 0 to length(Segmentos)-2 do
                for i := s+1 to length(Segmentos)-1 do
                    if (Segmentos[s].Sknl*Segmentos[s].Sknc) >
                       (Segmentos[i].Sknl*Segmentos[i].Sknc) then
                    begin
                        SEG_TMP := Segmentos[s];
                        Segmentos[s] := Segmentos[i];
                        Segmentos[i] := SEG_TMP;

                        //SEG_TMP_Imagem := Seg_Imagem[s];
                        //Seg_Imagem[s] := Seg_Imagem[i];
                        //Seg_Imagem[i] := Seg_TMP_Imagem;
                    end;
            }
            seg_ordem := true;

            // Veficando o número de segmentos válidos pelo tamanho
            seg_validos := 0;
            for s := 0 to length(Segmentos)-1 do
            begin
                Memo1.Lines.Append(formatfloat('0.0',Segmentos[s].Sknl*Segmentos[s].Sknc));
                if (Segmentos[s].Sknl*Segmentos[s].Sknc) < (knl*knc*LimiarFundoSeg) then
                begin
                   inc(seg_validos);
                   Memo1.Lines.Append('OK');
                end else
                begin
                   Memo1.Lines.Append('No');
                   //Seg_Imagem[s].Picture.Bitmap.Canvas.Pixels[3,3] := clred;
                end;
            end;

            // Alteração
            //seg_validos := length(Segmentos);

            // Tamanho do Objeto
            Seg_Medias.max[10] := 0;
            Seg_Medias.min[10] := 0;

            // Pegando neuronios pulsantes
            setlength(Segmentos_Pulso,(knl*knc));

               // Zerando Medias
                           Seg_Medias.v[0] := 0;
                           Seg_Medias.max[0] := 0;//Segmentos[s].SegNeuronios[i,j].rgb_Itti[0];
                           Seg_Medias.min[0] := 255;//Segmentos[s].SegNeuronios[i,j].rgb_Itti[0];

                           Seg_Medias.v[1] := 0;
                           Seg_Medias.max[1] := 0;//Segmentos[s].SegNeuronios[i,j].rgb_Itti[1];
                           Seg_Medias.min[1] := 255;//Segmentos[s].SegNeuronios[i,j].rgb_Itti[1];

                           Seg_Medias.v[2] := 0;
                           Seg_Medias.max[2] := 0;//Segmentos[s].SegNeuronios[i,j].rgb_Itti[2];
                           Seg_Medias.min[2] := 255;//Segmentos[s].SegNeuronios[i,j].rgb_Itti[2];

                           Seg_Medias.v[3] := 0;
                           Seg_Medias.max[3] := 0;//Segmentos[s].SegNeuronios[i,j].l;
                           Seg_Medias.min[3] := knl;//Segmentos[s].SegNeuronios[i,j].l;

                           Seg_Medias.v[4] := 0;
                           Seg_Medias.max[4] := 0;//Segmentos[s].SegNeuronios[i,j].c;
                           Seg_Medias.min[4] := knc;//Segmentos[s].SegNeuronios[i,j].c;

                           Seg_Medias.v[5] := 0;
                           Seg_Medias.max[5] := 0;//Segmentos[s].SegNeuronios[i,j].ang[1];
                           Seg_Medias.min[5] := 255;//Segmentos[s].SegNeuronios[i,j].ang[1];

                           Seg_Medias.v[6] := 0;
                           Seg_Medias.max[6] := 0;//Segmentos[s].SegNeuronios[i,j].ang[2];
                           Seg_Medias.min[6] := 255;//Segmentos[s].SegNeuronios[i,j].ang[2];

                           Seg_Medias.v[7] := 0;
                           Seg_Medias.max[7] := 0;//Segmentos[s].SegNeuronios[i,j].ang[3];
                           Seg_Medias.min[7] := 255;//Segmentos[s].SegNeuronios[i,j].ang[3];

                           Seg_Medias.v[8] := 0;
                           Seg_Medias.max[8] := 0;//Segmentos[s].SegNeuronios[i,j].ang[4];
                           Seg_Medias.min[8] := 255;//Segmentos[s].SegNeuronios[i,j].ang[4];

                           Seg_Medias.v[9] := 0;
                           Seg_Medias.max[9] := 0;//trunc(Segmentos[s].SegNeuronios[i,j].rn);
                           Seg_Medias.min[9] := 255;//trunc(Segmentos[s].SegNeuronios[i,j].rn);

                           //Seg_Medias[10].max := 0;
                           //Seg_Medias[10].min := 0;

            for s := 0 to seg_validos-1 do
            begin


               // Normalização dos Valores [0,1]
               for i := 0 to Segmentos[s].Sknl-1 do
                  for j := 0 to Segmentos[s].Sknc-1 do
                       if (Segmentos[s].SegNeuronios[i,j].Lx[1] >= thetae) and
                          (Segmentos[s].SegNeuronios[i,j].d = 1) then
                       begin
                           // Atribuindo as Características
                           // Re-Escalar [0]
                              if Segmentos[s].SegNeuronios[i,j].rgb_Itti[0] > Seg_Medias.max[0] then
                                 Seg_Medias.max[0] := Segmentos[s].SegNeuronios[i,j].rgb_Itti[0];
                              if Segmentos[s].SegNeuronios[i,j].rgb_Itti[0] < Seg_Medias.min[0] then
                                 Seg_Medias.min[0] := Segmentos[s].SegNeuronios[i,j].rgb_Itti[0];
                           // Re-Escalar [1]
                              if Segmentos[s].SegNeuronios[i,j].rgb_Itti[1] > Seg_Medias.max[1] then
                                 Seg_Medias.max[1] := Segmentos[s].SegNeuronios[i,j].rgb_Itti[1];
                              if Segmentos[s].SegNeuronios[i,j].rgb_Itti[1] < Seg_Medias.min[1] then
                                 Seg_Medias.min[1] := Segmentos[s].SegNeuronios[i,j].rgb_Itti[1];
                           // Re-Escalar [2]
                              if Segmentos[s].SegNeuronios[i,j].rgb_Itti[2] > Seg_Medias.max[2] then
                                 Seg_Medias.max[2] := Segmentos[s].SegNeuronios[i,j].rgb_Itti[2];
                              if Segmentos[s].SegNeuronios[i,j].rgb_Itti[2] < Seg_Medias.min[2] then
                                 Seg_Medias.min[2] := Segmentos[s].SegNeuronios[i,j].rgb_Itti[2];

                           // X Y
                           // Re-Escalar [3]
                              if Segmentos[s].SegNeuronios[i,j].l > Seg_Medias.max[3] then
                                 Seg_Medias.max[3] := Segmentos[s].SegNeuronios[i,j].l;
                              if Segmentos[s].SegNeuronios[i,j].l < Seg_Medias.min[3] then
                                 Seg_Medias.min[3] := Segmentos[s].SegNeuronios[i,j].l;
                           // Re-Escalar [4]
                              if Segmentos[s].SegNeuronios[i,j].c > Seg_Medias.max[4] then
                                 Seg_Medias.max[4] := Segmentos[s].SegNeuronios[i,j].c;
                              if Segmentos[s].SegNeuronios[i,j].c < Seg_Medias.min[4] then
                                 Seg_Medias.min[4] := Segmentos[s].SegNeuronios[i,j].c;


                           //Angulos do Mapa
                           // Re-Escalar [5]
                              if Segmentos[s].SegNeuronios[i,j].ang[1] > Seg_Medias.max[5] then
                                 Seg_Medias.max[5] := Segmentos[s].SegNeuronios[i,j].ang[1];
                              if Segmentos[s].SegNeuronios[i,j].ang[1] < Seg_Medias.min[5] then
                                 Seg_Medias.min[5] := Segmentos[s].SegNeuronios[i,j].ang[1];
                           // Re-Escalar [6]
                              if Segmentos[s].SegNeuronios[i,j].ang[2] > Seg_Medias.max[6] then
                                 Seg_Medias.max[6] := Segmentos[s].SegNeuronios[i,j].ang[2];
                              if Segmentos[s].SegNeuronios[i,j].ang[2] < Seg_Medias.min[6] then
                                 Seg_Medias.min[6] := Segmentos[s].SegNeuronios[i,j].ang[2];
                           // Re-Escalar [7]
                              if Segmentos[s].SegNeuronios[i,j].ang[3] > Seg_Medias.max[7] then
                                 Seg_Medias.max[7] := Segmentos[s].SegNeuronios[i,j].ang[3];
                              if Segmentos[s].SegNeuronios[i,j].ang[3] < Seg_Medias.min[7] then
                                 Seg_Medias.min[7] := Segmentos[s].SegNeuronios[i,j].ang[3];
                           // Re-Escalar [8]
                              if Segmentos[s].SegNeuronios[i,j].ang[4] > Seg_Medias.max[8] then
                                 Seg_Medias.max[8] := Segmentos[s].SegNeuronios[i,j].ang[4];
                              if Segmentos[s].SegNeuronios[i,j].ang[4] < Seg_Medias.min[8] then
                                 Seg_Medias.min[8] := Segmentos[s].SegNeuronios[i,j].ang[4];

                           //Rede Neural Objeto ConhecidoConhecido 0 ou 1
                           // Re-Escalar [5]
                              if trunc(Segmentos[s].SegNeuronios[i,j].rn) > Seg_Medias.max[9] then
                                 Seg_Medias.max[9] := trunc(Segmentos[s].SegNeuronios[i,j].rn);
                              if trunc(Segmentos[s].SegNeuronios[i,j].rn) < Seg_Medias.min[9] then
                                 Seg_Medias.min[9] := trunc(Segmentos[s].SegNeuronios[i,j].rn);

                       end; // if

            end; // for s


            for s := 0 to seg_validos-1 do
            begin
               neuronios_pulsantes := 0;

               Seg_Medias.v[0] := 0;
               Seg_Medias.v[1] := 0;
               Seg_Medias.v[2] := 0;
               Seg_Medias.v[3] := 0;
               Seg_Medias.v[4] := 0;
               Seg_Medias.v[5] := 0;
               Seg_Medias.v[6] := 0;
               Seg_Medias.v[7] := 0;
               Seg_Medias.v[8] := 0;
               Seg_Medias.v[9] := 0;
               Seg_Medias.v[10] := 0;

               for i := 0 to Segmentos[s].Sknl-1 do
                  for j := 0 to Segmentos[s].Sknc-1 do
                       if (Segmentos[s].SegNeuronios[i,j].Lx[1] >= thetae) and
                          (Segmentos[s].SegNeuronios[i,j].d = 1) then
                       begin
                           Segmentos_Pulso[neuronios_pulsantes].l := Segmentos[s].SegNeuronios[i,j].l;
                           Segmentos_Pulso[neuronios_pulsantes].c := Segmentos[s].SegNeuronios[i,j].c;
                           neuronios_pulsantes := neuronios_pulsantes + 1;

                           // Média de Cadas Segmentos...
                           // Atribuindo as Características
                           Seg_Medias.v[0] := Seg_Medias.v[0] + Normalizar(Segmentos[s].SegNeuronios[i,j].rgb_Itti[0],Seg_Medias.min[0],Seg_Medias.max[0]);
                           Seg_Medias.v[1] := Seg_Medias.v[1] + Normalizar(Segmentos[s].SegNeuronios[i,j].rgb_Itti[1],Seg_Medias.min[1],Seg_Medias.max[1]);
                           Seg_Medias.v[2] := Seg_Medias.v[2] + Normalizar(Segmentos[s].SegNeuronios[i,j].rgb_Itti[2],Seg_Medias.min[2],Seg_Medias.max[2]);

                           // X Y
                           Seg_Medias.v[3] := Seg_Medias.v[3] + Normalizar(Segmentos[s].SegNeuronios[i,j].l,Seg_Medias.min[3],Seg_Medias.max[3]);
                           Seg_Medias.v[4] := Seg_Medias.v[4] + Normalizar(Segmentos[s].SegNeuronios[i,j].c,Seg_Medias.min[4],Seg_Medias.max[4]);

                           //Angulos do Mapa
                           Seg_Medias.v[5] := Seg_Medias.v[5] + Normalizar(Segmentos[s].SegNeuronios[i,j].ang[1],Seg_Medias.min[5],Seg_Medias.max[5]);
                           Seg_Medias.v[6] := Seg_Medias.v[6] + Normalizar(Segmentos[s].SegNeuronios[i,j].ang[2],Seg_Medias.min[6],Seg_Medias.max[6]);
                           Seg_Medias.v[7] := Seg_Medias.v[7] + Normalizar(Segmentos[s].SegNeuronios[i,j].ang[3],Seg_Medias.min[7],Seg_Medias.max[7]);
                           Seg_Medias.v[8] := Seg_Medias.v[8] + Normalizar(Segmentos[s].SegNeuronios[i,j].ang[4],Seg_Medias.min[8],Seg_Medias.max[8]);

                           //Rede Neural Objeto ConhecidoConhecido 0 ou 1
                           Seg_Medias.v[9] := Seg_Medias.v[9] + Normalizar(trunc(Segmentos[s].SegNeuronios[i,j].rn),Seg_Medias.min[9],Seg_Medias.max[9]);

                       end; // if

               // Tamanho do Segmento
               if s = 0 then
               begin
                  Seg_Medias.max[10] := neuronios_pulsantes;
                  Seg_Medias.min[10] := neuronios_pulsantes;
               end else
               begin
                  if neuronios_pulsantes > Seg_Medias.max[10] then
                     Seg_Medias.max[10] := neuronios_pulsantes;
                  if neuronios_pulsantes < Seg_Medias.min[10] then
                     Seg_Medias.min[10] := neuronios_pulsantes;
               end;

               // Calculo das Médias de Saliencia de cada Atributo
               // Intensidade
               Segmentos[s].SegMedias[0] := (Seg_Medias.v[0]/neuronios_pulsantes);

               // Cor RG e BY
               Segmentos[s].SegMedias[1] := (Seg_Medias.v[1]/neuronios_pulsantes);

               // Orientação
               Segmentos[s].SegMedias[2] := (Seg_Medias.v[2]/neuronios_pulsantes);

               // Calculo X e Y
               Segmentos[s].SegMedias[3] := (Seg_Medias.v[3]/neuronios_pulsantes);
               Segmentos[s].SegMedias[4] := (Seg_Medias.v[4]/neuronios_pulsantes);
               //.. colocando Normalizando
                    //Segmentos[s].SegMedias[3] := trunc((Segmentos[s].SegMedias[3]*255)/knl);
                    //Segmentos[s].SegMedias[4] := trunc((Segmentos[s].SegMedias[4]*255)/knc);

               //Rede Neural Objeto ConhecidoConhecido
               Segmentos[s].SegMedias[9] := (Seg_Medias.v[9]/neuronios_pulsantes);

               // Size
               Segmentos[s].SegMedias[10] := (neuronios_pulsantes);

               (*
               // Teste pegando os máximos pontos de saliência-------
               // Calculo das Médias de Saliencia de cada Atributo
               // Intensidade
               Segmentos[s].SegMedias[0] := Normalizar(Seg_Medias.max[0],Seg_Medias.min[0],Seg_Medias.max[0]);

               // Cor RG e BY
               Segmentos[s].SegMedias[1] := Normalizar(Seg_Medias.max[1],Seg_Medias.min[1],Seg_Medias.max[1]);

               // Orientação
               Segmentos[s].SegMedias[2] := Normalizar(Seg_Medias.max[2],Seg_Medias.min[2],Seg_Medias.max[2]);

               // Calculo X e Y
               Segmentos[s].SegMedias[3] := Normalizar(Seg_Medias.max[3],Seg_Medias.min[3],Seg_Medias.max[3]);
               Segmentos[s].SegMedias[4] := Normalizar(Seg_Medias.max[4],Seg_Medias.min[4],Seg_Medias.max[4]);
               //.. colocando Normalizando
                    //Segmentos[s].SegMedias[3] := trunc((Segmentos[s].SegMedias[3]*255)/knl);
                    //Segmentos[s].SegMedias[4] := trunc((Segmentos[s].SegMedias[4]*255)/knc);

               //Rede Neural Objeto ConhecidoConhecido
               Segmentos[s].SegMedias[9] := Normalizar(Seg_Medias.max[9],Seg_Medias.min[9],Seg_Medias.max[9]);

               // Size
               Segmentos[s].SegMedias[10] := (neuronios_pulsantes);
               //--------------------------------------------------
               *)

               // Tornando Segmento Válido de acordo com Média
               Segmentos[s].segValido := false;
               if ((Segmentos[s].SegMedias[0] + Segmentos[s].SegMedias[1] +
                  Segmentos[s].SegMedias[2] + Segmentos[s].SegMedias[9])/4) >= strtofloat(mskThetas.Text) then //> 0.03 then // INSERIR PARÂMETRO...!?!?!?!
                  //if (Segmentos[s].Sknl*Segmentos[s].Sknc) < (knl*knc*LimiarFundoSeg) then
                      Segmentos[s].segValido := true;

               // Zerando Valors
               Segmentos[s].IeD.i := 0;
               Segmentos[s].IeD.v := 0;
               Segmentos[s].IeD.y := 0;
               Segmentos[s].IeD.rn := 0;
               Segmentos[s].v_Max := 0;
               Segmentos[s].Classify := 0;

               //exit;
               //BitBtn36.OnClick(Sender);

            end;

            // Normalizando o Tamanho do Segmento
            for s := 0 to seg_validos-1 do
            begin
               Segmentos[s].SegMedias[10] := Normalizar(trunc(Segmentos[s].SegMedias[10]),Seg_Medias.min[10],Seg_Medias.max[10]);
            end;

            BitBtn36.OnClick(Sender);

            setlength(Segmentos_Pulso,neuronios_pulsantes);

         end;

         //Memo1.Lines.Append(inttostr(neuronios_pulsantes));
         // Treinando pelos segmentos...----------------------------------------
         //for s := 0 to neuronios_pulsantes-1 do


         begin
             Application.ProcessMessages;
             if timer6.Enabled = false then exit;

             //repeat
             //randomize;
             if n_seg > 1 then
             begin
                repeat
                   s := randomrange(0,length(Segmentos));
                until (Segmentos[s].segValido = true) and (s <> seg_UP);
             end else
             begin
                repeat
                   s := randomrange(0,length(Segmentos));
                until (Segmentos[s].segValido = true);
             end;
             seg_UP := s;
             //som_i := Segmentos_Pulso[s].l;
             //som_j := Segmentos_Pulso[s].c;
             //Kohonen;
            //until Segmentos[s].segValido = true;

         end;


         timer7.Enabled := true;

         //for s := 0 to length(Segmentos)-1 do
         (*
         for s := 0 to seg_validos-1 do
            for i := 0 to Segmentos[s].Sknl-1 do
               for j := 0 to Segmentos[s].Sknc-1 do
               begin
                  Application.ProcessMessages;
                  if timer6.Enabled = false then exit;
                  if (Segmentos[s].SegNeuronios[i,j].Lx[1] >= thetae) and
                     (Segmentos[s].SegNeuronios[i,j].d = 1) then
                  begin  // Neuronio a ser treinado no SOM
                     som_i := Segmentos[s].SegNeuronios[i,j].l;
                     som_j := Segmentos[s].SegNeuronios[i,j].c;
                     Kohonen;
                  end;
               end;
         *)

           //BitBtn2.OnClick(Sender);


            {
            s := randomrange(0,length(Segmentos));
            i := randomrange(0,Segmentos[s].Sknl-1);
            j := randomrange(0,Segmentos[s].Sknc-1);

            if (Segmentos[s].SegNeuronios[i,j].Lx[1] >= thetae) and
               (Segmentos[s].SegNeuronios[i,j].d = 1) then
            begin  // Neuronio a ser treinado no SOM
               som_i := Segmentos[s].SegNeuronios[i,j].l;
               som_j := Segmentos[s].SegNeuronios[i,j].c;
               Kohonen;
            end;
            }

         //BitBtn36.OnClick(Sender);
         //PageControl13.TabIndex := 9;
         // fim dos segmentos-------------------------------------------------

     end;

  end; // enquanto true

  FinalizarContagem;

end;

procedure TFormProjeto.FormCreate(Sender: TObject);
var
  i: integer;
begin
   nl := DBImage1.Picture.Bitmap.Height;
   nc := DBImage1.Picture.Bitmap.Width;


   OpenPictureDialog1.InitialDir := ExtractFilePath(Application.ExeName);//+'\Imagens\';
   OpenDialog1.InitialDir := ExtractFilePath(Application.ExeName);//+'\Simulação\';

   sgExp4_CNA.Cells[0,0] := '-1';  sgExp4_CNA.Cells[1,0] := '-1';  sgExp4_CNA.Cells[2,0] := '-1';
   sgExp4_CNA.Cells[0,1] :=  '2';  sgExp4_CNA.Cells[1,1] :=  '2';  sgExp4_CNA.Cells[2,1] :=  '2';
   sgExp4_CNA.Cells[0,2] :=  '-1';  sgExp4_CNA.Cells[1,2] :=  '-1';  sgExp4_CNA.Cells[2,2] :=  '-1';

   sgExp4_CNB.Cells[0,0] := '-1';  sgExp4_CNB.Cells[1,0] := '2';  sgExp4_CNB.Cells[2,0] := '-1';
   sgExp4_CNB.Cells[0,1] := '-1';  sgExp4_CNB.Cells[1,1] := '2';  sgExp4_CNB.Cells[2,1] := '-1';
   sgExp4_CNB.Cells[0,2] := '-1';  sgExp4_CNB.Cells[1,2] := '2';  sgExp4_CNB.Cells[2,2] := '-1';

   sgExp4_CN45P.Cells[0,0] := '2';  sgExp4_CN45P.Cells[1,0] := '-1';  sgExp4_CN45P.Cells[2,0] := '-1';
   sgExp4_CN45P.Cells[0,1] :=  '-1';  sgExp4_CN45P.Cells[1,1] :=  '2';  sgExp4_CN45P.Cells[2,1] :=  '-1';
   sgExp4_CN45P.Cells[0,2] :=  '-1';  sgExp4_CN45P.Cells[1,2] :=  '-1';  sgExp4_CN45P.Cells[2,2] :=  '2';

   sgExp4_CN45N.Cells[0,0] := '-1';  sgExp4_CN45N.Cells[1,0] := '-1';  sgExp4_CN45N.Cells[2,0] := '2';
   sgExp4_CN45N.Cells[0,1] := '-1';  sgExp4_CN45N.Cells[1,1] := '2';  sgExp4_CN45N.Cells[2,1] := '-1';
   sgExp4_CN45N.Cells[0,2] := '2';  sgExp4_CN45N.Cells[1,2] := '-1';  sgExp4_CN45N.Cells[2,2] := '-1';

  semaforo:= true;
  nome_arq := FileListBox1.FileName;
  delete(nome_arq,length(nome_arq)-2,3);
  nome_arq := nome_arq + 'txt';

  thetap := 1200;
  Editthetap.Text:=floattostr(thetap);
  TrackBarthetap.Position := round(thetap);

  thetae := 2.0;
  EditTetax.Text:=floattostr(thetae);
  TrackBarTetax.Position:=round(10*thetae);

  Wz := 20;//2.0;
  EditWz.Text:=floattostr(Wz);
  TrackBarWz.Position:=round(Wz);

  thetaz := 0.5;
  EditTetaxz.Text:=floattostr(thetaz);
  TrackBarTetaxz.Position:=round(10*thetaz);

  limiarF := 0.05;
  EditlimiarFundo.Text:=floattostr(limiarF);
  TrackBarLimiarFundo.Position := round(100*limiarF);

  raio := 13;
  raio_ini := 13;
  //mskrini.Text := floattostr(raio);

  alfa_ini := 0.2;
  alfa := 0.2;
  //mskt.text := floattostr(alfa);

  dt_Sal := 0.01;
  mskDt_Sal.Text := floattostr(dt_Sal);

  wInib := 10;
  mskWInib.Text := floattostr(wInib);

  LimiarFundoSeg := 0.2;
  EditLimiarFundoSeg.Text:=floattostr(LimiarFundoSeg);
  tbLimiarFundoSeg.Position := round(10*LimiarFundoSeg);

  for i := 0 to 3 do
  begin
    sel_neuron_x[i] := -1;
    sel_neuron_y[i] := -1;
    PopupMenu1.Items[i+4].Caption := 'Selecionar Neurônio...';
  end;

  parametros := TMemo.Create(FormProjeto);
  parametros.Parent := FormProjeto;
  parametros.Visible := false;

  // Definindo tamanho do array dinamico
  knL := mod_ImgExp4.Picture.Bitmap.Height;
  knC := mod_ImgExp4.Picture.Bitmap.Width;
  label120.Caption := 'Dimensões ['+inttostr(knl)+']x['+inttostr(knc)+']';
  //SetLength(Neuronios,knl,knc);
  //SetLength(SNeuronios,knl,knc);

  // Inicializando Pesos
  vtbSize := 0;
  vtbI := vtbSize; vtbRG := vtbSize; vtbBY := vtbSize;
  vtbXY := vtbSize; vtbRN := vtbSize;

  cbSeg.Checked := true;
  cbSeg.Caption := 'Simulação Parada';
end;

procedure TFormProjeto.TrackBarthetapChange(Sender: TObject);
begin
  thetap:=TrackBarthetap.Position;
  Editthetap.Text:=floattostr(thetap);
  AtualizarParametros(Sender);
end;

procedure TFormProjeto.TrackBarTetaxChange(Sender: TObject);
begin
  thetae:=TrackBarTetax.Position/10;
  EditTetax.Text:=floattostr(thetae);
  AtualizarParametros(Sender);
end;

procedure TFormProjeto.TrackBarTetaxzChange(Sender: TObject);
begin
  thetaz:=TrackBarTetaxz.Position/10;
  EditTetaxz.Text:=floattostr(thetaz);
  AtualizarParametros(Sender);
end;

procedure TFormProjeto.TrackBarWzChange(Sender: TObject);
begin
  Wz:=TrackBarWz.Position;
  EditWz.Text:=floattostr(Wz);
  AtualizarParametros(Sender);
end;

procedure TFormProjeto.VisualizarAtividadedesteNeurnio1Click(
  Sender: TObject);
var
   resul : string;
   i : integer;
begin
         FormAtividade := TFormAtividade.create ( application );
         try

             resul := '';
             for i := 0 to 3 do begin
             end;
                 if sel_neuron_x[0] <> - 1 then
                    resul := resul + ' 1 [' + inttostr(sel_neuron_x[0])+','+inttostr(sel_neuron_y[0])+']';

                 if sel_neuron_x[1] <> - 1 then
                    resul := resul + ' 2 [' + inttostr(sel_neuron_x[1])+','+inttostr(sel_neuron_y[1])+']';

                 if sel_neuron_x[2] <> - 1 then
                    resul := resul + ' 3 [' + inttostr(sel_neuron_x[2])+','+inttostr(sel_neuron_y[2])+']';

                 if sel_neuron_x[3] <> - 1 then
                    resul := resul + ' 4 [' + inttostr(sel_neuron_x[3])+','+inttostr(sel_neuron_y[3])+']';

             FormAtividade.Caption := 'Atividade do Neurônio' + resul;

             FormAtividade.Show;
         except
             //FormZoom.Release;
             //FormZoom := nil;
         end;

end;

procedure TFormProjeto.SelecionarNeurnio011Click(Sender: TObject);
begin
    sel_neuron_x[0] := s_neuron_x;
    sel_neuron_y[0] := s_neuron_y;
    PopupMenu1.Items[4].Caption := 'Neurônio Selecionado: ['+inttostr(s_neuron_x)+','
                                                            +inttostr(s_neuron_y)+']';
end;


procedure TFormProjeto.SelecionarNeurnio021Click(Sender: TObject);
begin
    sel_neuron_x[1] := s_neuron_x;
    sel_neuron_y[1] := s_neuron_y;
    PopupMenu1.Items[5].Caption := 'Neurônio Selecionado: ['+inttostr(s_neuron_x)+','
                                                            +inttostr(s_neuron_y)+']';
end;

procedure TFormProjeto.SelecionarNeurnio031Click(Sender: TObject);
begin
    sel_neuron_x[2] := s_neuron_x;
    sel_neuron_y[2] := s_neuron_y;
    PopupMenu1.Items[6].Caption := 'Neurônio Selecionado: ['+inttostr(s_neuron_x)+','
                                                            +inttostr(s_neuron_y)+']';
end;

procedure TFormProjeto.SelecionarNeurnio041Click(Sender: TObject);
begin
    sel_neuron_x[3] := s_neuron_x;
    sel_neuron_y[3] := s_neuron_y;
    PopupMenu1.Items[7].Caption := 'Neurônio Selecionado: ['+inttostr(s_neuron_x)+','
                                                            +inttostr(s_neuron_y)+']';
end;

procedure TFormProjeto.LimparaSeleo1Click(Sender: TObject);
var
  i : integer;
begin
  for i := 0 to 3 do
  begin
    sel_neuron_x[i] := -1;
    sel_neuron_y[i] := -1;
    PopupMenu1.Items[i+4].Caption := 'Selecionar Neurônio...';
  end;
end;

procedure TFormProjeto.TrackBarLimiarFundoChange(Sender: TObject);
begin
  LimiarF:=TrackBarLimiarFundo.Position/100;
  EditLimiarFundo.Text:=floattostr(LimiarF);
  AtualizarParametros(Sender);
end;

procedure TFormProjeto.cb_StrechClick(Sender: TObject);
begin
   if cb_Strech.Checked then img_Segmentos.Stretch := true else
   img_Segmentos.Stretch := false;
   AtualizarParametros(Sender);
end;

procedure TFormProjeto.SpeedButton5Click(Sender: TObject);
begin

    if SpeedButton5.Caption = 'Ver Imagem de Entrada' then
    begin
       mod_ImgExp4_IeF_OR.Visible := true;
       SpeedButton5.Caption := 'Ver Imagem Segmentada'
    end else
    begin
       mod_ImgExp4_IeF_OR.Visible := false;
       SpeedButton5.Caption := 'Ver Imagem de Entrada'
    end;
end;

procedure TFormProjeto.tbLimiarFundoSegChange(Sender: TObject);
begin
  LimiarFundoSeg:=tbLimiarFundoSeg.Position/10;
  EditLimiarFundoSeg.Text:=floattostr(LimiarFundoSeg);
  AtualizarParametros(Sender);
end;

procedure TFormProjeto.mod_ImgExp4_SOM_SALMouseMove(Sender: TObject;
  Shift: TShiftState; X, Y: Integer);
var
  l,c,s : integer;
begin
   c := round(x / (mod_ImgExp4_SOM_SAL.Width  /(knc-1)));//(x div 7) ;
   l := round(y / (mod_ImgExp4_SOM_SAL.Height /(knl-1)));//(y div 5) ;

   s := Neuronios[l,c].segN;

   if s <> -1 then
   begin
     label25.Caption := 'SEG:' + inttostr(s)+ ' ['+inttostr(l)+','+inttostr(c)+']';
     label75.Caption := 'TAM  = [' + floattostr(Segmentos[s].SegMedias[10]) + ']';
     label27.Caption := 'INT  = [' + floattostr(Segmentos[s].SegMedias[0]) + ']';
     label28.Caption := 'COR = [' + floattostr(Segmentos[s].SegMedias[1]) + ']';
     label29.Caption := 'ORI = [' + floattostr(Segmentos[s].SegMedias[2]) + ']';
     label36.Caption := 'LIN = [' + floattostr(Segmentos[s].SegMedias[3]) + ']';
     label37.Caption := 'COL = [' + floattostr(Segmentos[s].SegMedias[4]) + ']';
     label87.Caption := 'REC = [' + formatfloat('0.00',Segmentos[s].SegMedias[9]) + ']';
     label40.Caption := 'i = [' + formatfloat('0.00',Segmentos[s].IeD.i) + ']';
     label41.Caption := 'v = [' + formatfloat('0.00',Segmentos[s].IeD.v) + ']';
     label42.Caption := 'y = [' + formatfloat('0.00',Segmentos[s].IeD.y) + ']';
   end; {else
   begin
     label25.Caption := '';
     label27.Caption := '';
     label28.Caption := '';
     label29.Caption := '';
     label36.Caption := '';
     label37.Caption := '';
     label31.Caption := '';
     label32.Caption := '';
     label33.Caption := '';
     label34.Caption := '';
     label87.Caption := '';
     label40.Caption := '';
     label41.Caption := '';
     label42.Caption := '';
   end; }


{
    c := round(x / (mod_ImgExp4_SOM_SAL.Width  /(koknc-1)));//(x div 7) ;
    l := round(y / (mod_ImgExp4_SOM_SAL.Height /(koknl-1)));//(y div 5) ;

   label25.Caption := '['+inttostr(l)+','+inttostr(c)+']';

   label27.Caption := 'I  = [' + inttostr(KNeuronios[l,c][0]) + ']';
   label28.Caption := 'RB = [' + inttostr(KNeuronios[l,c][1]) + ']';
   label29.Caption := 'BY = [' + inttostr(KNeuronios[l,c][2]) + ']';

   label36.Caption := 'X = [' + inttostr(KNeuronios[l,c][3]) + ']';
   label37.Caption := 'Y = [' + inttostr(KNeuronios[l,c][4]) + ']';

   label31.Caption := '0   = [' + inttostr(KNeuronios[l,c][5]) + ']';
   label32.Caption := '90  = [' + inttostr(KNeuronios[l,c][6]) + ']';
   label33.Caption := '45 = [' + inttostr(KNeuronios[l,c][7]) + ']';
   label34.Caption := '135 = [' + inttostr(KNeuronios[l,c][8]) + ']';

   label40.Caption := 'i = [' + formatfloat('0.00',SNeuronios[l,c].i) + ']';
   label41.Caption := 'v = [' + formatfloat('0.00',SNeuronios[l,c].v) + ']';
   label42.Caption := 'y = [' + formatfloat('0.00',SNeuronios[l,c].y) + ']';

   label87.Caption := 'MLP = [' + formatfloat('0.00',KNeuronios[l,c][9]) + ']';
}

end;

procedure TFormProjeto.Timer7Timer(Sender: TObject);
var s,i,j : integer;
    cor : Tcolor;
begin
     CO.Series[0].Clear;

     for s := 0 to length(Segmentos)-1 do
     begin
         cor := clblack;
         for i := Segmentos[s].imin to Segmentos[s].imax do
            for j := Segmentos[s].jmin to Segmentos[s].jmax do
                  if (Segmentos[s].segValido = true) and
                     (Segmentos[s].SegNeuronios[i-Segmentos[s].imin,j-Segmentos[s].jmin].Lx[1] >= thetae) and
                     (Segmentos[s].SegNeuronios[i-Segmentos[s].imin,j-Segmentos[s].jmin].d = 1) then
                  begin
                     CSal.Series[0].YValue[(((knl)-i)*(knc))+j] := Segmentos[s].IeD.v;

                     if cor = clblack then
                        cor := TColor(RGB(Segmentos[s].SegNeuronios[i-Segmentos[s].imin,j-Segmentos[s].jmin].Seg_rgb[0],
                               Segmentos[s].SegNeuronios[i-Segmentos[s].imin,j-Segmentos[s].jmin].Seg_rgb[1],
                               Segmentos[s].SegNeuronios[i-Segmentos[s].imin,j-Segmentos[s].jmin].Seg_rgb[2]))
                  end;

         if (Segmentos[s].segValido = true) then
            CO.Series[0].AddXY(s,Segmentos[s].IeD.v,'',Cor);

         // Atribuindo Valores a cada gráfico
         if (s < 30) and (cont_it <= strtoint(mskVit.text))then
         begin
            COII.Series[s].Color := Cor;
            COII.Series[s].AddXY(cont_it,Segmentos[s].IeD.v,'',Cor);
         end;


     end;

     // Incrementando o número de iterações
     inc(cont_it);

     Timer7.Enabled := false;
end;


procedure TFormProjeto.cbV3DClick(Sender: TObject);
begin
   CSal.View3D := CBV3D.checked;
   AtualizarParametros(Sender);   
end;

procedure TFormProjeto.tb3dDimChange(Sender: TObject);
begin
    CSal.Chart3DPercent := tb3dDim.Position;
    AtualizarParametros(Sender);
end;

procedure TFormProjeto.tbOrthoChange(Sender: TObject);
begin
   CSal.View3DOptions.OrthoAngle := tbOrtho.Position;
   AtualizarParametros(Sender);   
end;

procedure TFormProjeto.BitBtn1Click(Sender: TObject);
var
    i : integer;
begin
    for i := 0 to 29 do COII.Series[i].Clear;

    for i := 0 to length(Segmentos)-1 do
    begin
       if Segmentos[i].segValido = true then
       begin
          Segmentos[i].IeD.y := 0;
          Segmentos[i].IeD.v := 0;
       end;
    end;
    cont_it := 0;
end;

procedure TFormProjeto.CBCamClick(Sender: TObject);
begin
  if CBCam.Checked then
  begin
     try
        Camera1.Actif:=True;
        TimerCam.Enabled := true;
        if FileExists('F.bmp') then
           DeleteFile('F.bmp');
        Camera1.CaptureImageDisque;
        mod_ImgExp4.Picture.LoadFromFile(Camera1.FichierImage);

        // Definindo tamanho do array dinamico
        knL := mod_ImgExp4.Picture.Bitmap.Height;
        knC := mod_ImgExp4.Picture.Bitmap.Width;
        label120.Caption := 'Dimensões ['+inttostr(knl)+']x['+inttostr(knc)+']';
        SetLength(Neuronios,knl,knc);
     except
       begin
         MessageDlg('Erro ao conectar a WebCam.',mtWarning,[mbOK],0);
         Abort;
       end;
     end;
  end else
  begin
     TimerCam.Enabled := false;
     Camera1.Actif := False;
  end;
end;

procedure TFormProjeto.TimerCamTimer(Sender: TObject);
var
   i,j : integer;
   pixelI : double;
   iBMPe24 : PRGB;
begin
   //Application.ProcessMessages;
   try

   if FileExists('F.bmp') then
       DeleteFile('F.bmp');
   Camera1.CaptureImageDisque;
   mod_ImgExp4.Picture.LoadFromFile(Camera1.FichierImage);
   if CBCamTC.Checked then
   begin
      if FormCam.Showing = false then FormCam.Show;
      FormCam.mod_ImgExp4.Picture.LoadFromFile(Camera1.FichierImage);
   end;

      for i := 0 to knl-1 do
      begin
         iBMPe24 := mod_ImgExp4.Picture.Bitmap.ScanLine[i];
         for j := 0 to knc-1 do
         begin
             // Imagem de Entrada Original RGB e I
                Neuronios[i,j].rgb[0] := (iBMPe24[j].r); // R
                Neuronios[i,j].rgb[1] := (iBMPe24[j].g); // G
                Neuronios[i,j].rgb[2] := (iBMPe24[j].b); // B

                pixelI := (iBMPe24[j].r+iBMPe24[j].g+iBMPe24[j].b) div 3;
                Neuronios[i,j].rgb[3] := trunc(pixelI);
                Neuronios[i,j].rgb[4] := trunc(pixelI); // Suavização Mediana Cinza / Cores

             // Imagem de Entrada Suavizada Colorida
                Neuronios[i,j].RGBSColor[0] := iBMPe24[j].r;
                Neuronios[i,j].RGBSColor[1] := iBMPe24[j].g;
                Neuronios[i,j].RGBSColor[2] := iBMPe24[j].b;

             // Utilizado para Sincronismo
                Neuronios[i,j].v := Trunc(Random * 1000) / 1000;
                Neuronios[i,j].p := 0;
                Neuronios[i,j].d := 0;
                if (iBMPe24[j].r=0) and (iBMPe24[j].g=0) and (iBMPe24[j].b=0) then
                   Neuronios[i,j].pulsar := false
                else
                   Neuronios[i,j].pulsar := true;
                Neuronios[i,j].l := i;
                Neuronios[i,j].c := j;
                Neuronios[i,j].rn := 0;
                Neuronios[i,j].segN := -1;

         end; // fim da inicialização 24bits
      end; // i

   //BitBtn2.Click;
   IttiModel(Sender);
   except
   end;
   
end;

procedure TFormProjeto.CBCP3DClick(Sender: TObject);
begin
   CP.View3D := CBCP3D.checked;
end;

procedure TFormProjeto.TrackBar3Change(Sender: TObject);
begin
    CP.Chart3DPercent := TrackBar3.Position;
end;

procedure TFormProjeto.TrackBar4Change(Sender: TObject);
begin
   CP.View3DOptions.OrthoAngle := TrackBar4.Position;
end;

procedure TFormProjeto.BitBtn4Click(Sender: TObject);
var
   // Grafico
   aSurfaceSeries : TSurfaceSeries;
   n,i,j,a,b,ind_theta : integer;
   linha : String;

   Bitmap : TBitmap;
   iBMPs: PRGB;
begin
   n := seMask_GF.Value;
   ind_theta := RGTheta.ItemIndex;
   // Limpando o Gráfico
   CP.BottomAxis.Minimum := 0;
   CP.BottomAxis.Maximum := n;

   // Criando dinamicamente
   aSurfaceSeries := TSurfaceSeries.Create(Self);
   with aSurfaceSeries do
   begin
      NumXValues := n;
      NumZValues := n;
      Pen.Visible := true;
      Pen.Color := clblack;
      TimesZOrder := 20;//trunc(n*0.11);//20;

      for i := 1 to n do
         for j := 1 to n do
             AddXYZ(j,0,i,'',clwhite);
   end;
   CP.Series[0].Destroy;
   CP.AddSeries(TSurfaceSeries(aSurfaceSeries));

   Memo2.Clear;
   a := (n-1) div 2;
   b := a;
   for i := -a to a do
   begin
      linha := '';
      for j := -b to b do
      begin
          CP.Series[0].YValue[(((-i)+(n div 2))*n)+(j+b)] := wg[ind_theta][i+a,j+b];
          linha := linha + ' ' + formatfloat('#0.0000',wg[ind_theta][i+a,j+b]);
      end;
      Memo2.Lines.Append(linha);
   end; // i

   // LOCAL: Limpando a Imagem em que será Destacado o Objeto Saliente
   Bitmap := TBitmap.Create;
   try
     Bitmap.PixelFormat := pf24bit;
     Bitmap.Width := n;
     Bitmap.Height := n;
     for i := 0 to n-1 do
     begin
       iBMPs := Bitmap.ScanLine[i];
       for j := 0 to n-1 do
       begin
         iBMPs[j].r := trunc(wg[ind_theta][i,j]*255);
         iBMPs[j].g := trunc(wg[ind_theta][i,j]*255);
         iBMPs[j].b := trunc(wg[ind_theta][i,j]*255);
       end;
     end;
     // Mostrando Imagem
     Image_GB.Picture.Graphic := Bitmap;
   finally
     Bitmap.Free;
   end;


end;

procedure TFormProjeto.BitBtn3Click(Sender: TObject);
var
   // Grafico
   aSurfaceSeries : TSurfaceSeries;
   n,i,j,a,b : integer;
   linha : String;

   Bitmap : TBitmap;
   iBMPs: PRGB;
begin
   n := seNb.Value;
   // Limpando o Gráfico
   CP.BottomAxis.Minimum := 0;
   CP.BottomAxis.Maximum := n;

   // Criando dinamicamente
   aSurfaceSeries := TSurfaceSeries.Create(Self);
   with aSurfaceSeries do
   begin
      NumXValues := n;
      NumZValues := n;
      Pen.Visible := true;
      Pen.Color := clblack;
      TimesZOrder := 20;//trunc(n*0.11);//20;

      for i := 1 to n do
         for j := 1 to n do
             AddXYZ(j,0,i,'',clwhite);
   end;
   CP.Series[0].Destroy;
   CP.AddSeries(TSurfaceSeries(aSurfaceSeries));

   Memo2.Clear;
   a := (n-1) div 2;
   b := a;
   for i := -a to a do
   begin
      linha := '';
      for j := -b to b do
      begin
          CP.Series[0].YValue[(((-i)+(n div 2))*n)+(j+b)] := w[i+a,j+b];
          linha := linha + ' ' + formatfloat('#0.0000',w[i+a,j+b]);
      end;
      Memo2.Lines.Append(linha);
   end; // i

   // LOCAL: Limpando a Imagem em que será Destacado o Objeto Saliente
   Bitmap := TBitmap.Create;
   try
     Bitmap.PixelFormat := pf24bit;
     Bitmap.Width := n;
     Bitmap.Height := n;
     for i := 0 to n-1 do
     begin
       iBMPs := Bitmap.ScanLine[i];
       for j := 0 to n-1 do
       begin
         iBMPs[j].r := trunc(w[i,j]*255);
         iBMPs[j].g := trunc(w[i,j]*255);
         iBMPs[j].b := trunc(w[i,j]*255);
       end;
     end;
     // Mostrando Imagem
     Image_GB.Picture.Graphic := Bitmap;
   finally
     Bitmap.Free;
   end;

end;

procedure TFormProjeto.BitBtn5Click(Sender: TObject);
begin
   Camera1.Actif := false;
   Camera1.Actif := true;
   Camera1.SelectConfig;
end;

procedure TFormProjeto.BitBtn6Click(Sender: TObject);
begin
   Camera1.Actif := false;
   Camera1.Actif := true;
   Camera1.SelectFormat;
end;

procedure TFormProjeto.SpeedButton6Click(Sender: TObject);
var
  linha: string;
  i : integer;
  arq_in : TextFile;
begin
  try

   if (BitBtn2.Enabled = false) then
   begin
      Application.MessageBox('Existe uma simulação em execução. Para continuar deve interromper clicando em [PARAR SIMULAÇÃO]','Atenção',mb_iconExclamation);
      exit;
   end;

   semaforo:= true;
   nome_arq := 'set_' + formatdatetime('yyyymmdd',date()) + '_' +
                        formatdatetime('hhmmss',time());
   nome_arq := nome_arq + '.txt';

   parametros.Clear;

   If not(fileexists(nome_arq)) then
   begin

      // 0 Imagem Colorida 8bits
      if rbExp4_8.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');

      // 1 Imagem Colorida 24bits
      if RadioButton13.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');

      // 2 Imagem Colorida
      if cbExp4_Color.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');

      // 3 Mostrar Pulso
      if cbExp4_MP.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');

      // 4 Segmentação em Tons de Cinza
      if cbExp4_Cinza.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');

      // 5 Segmentação em Cores Aleatórias
      if cbExp4_Cores.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');

      // 6 Ativar/Desativar Inibidor Global
      if cbIni.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');

      // 7 Tempo de Pulso
      parametros.Lines.Add(inttostr(tbpulso.Position));

      // 8 9 10 11 12 LEGION
      parametros.Lines.Add(inttostr(TrackBarthetap.Position));
      parametros.Lines.Add(inttostr(TrackBarTetax.Position));
      parametros.Lines.Add(inttostr(TrackBarWz.Position));
      parametros.Lines.Add(inttostr(TrackBarTetaxz.Position));
      parametros.Lines.Add(inttostr(TrackBarLimiarFundo.Position));

      // 13 14 15 16 SALIÊNCIA
      parametros.Lines.Add(mskdt_SAL.Text);
      parametros.Lines.Add(mskWInib.Text);
      parametros.Lines.Add(inttostr(tbLimiarFundoSeg.Position));
      parametros.Lines.Add(mskThetas.Text);

      // 17 18 SUAVIZAR IMAGEM
      parametros.Lines.Add('0');
      parametros.Lines.Add('0');

      // 19 20 ORIENTAÇÃO
      if cbExp4_LO.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');
      parametros.Lines.Add(inttostr(SEditLO.Value));

      // 21 22 COR BORDA SEGMENTO
      if cbBorda.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');
      parametros.Lines.Add(inttostr(ColorGridSeg.ForegroundIndex));

      // 23 24 SEGMENTADO
      if cb_Strech.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');
      if cbVer_Sincro.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');

      // 25 26 27 MAPA DE SALIÊNCIA 3D
      if cbV3D.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');
      parametros.Lines.Add(inttostr(tb3dDim.Position));
      parametros.Lines.Add(inttostr(tbOrtho.Position));

      // 28 29 REGIÃO SALIENTE
      if rgIE.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');
      if rbIS.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');

      // 30 31 32 33 34 35 PESOS SALIENTES
      // TAM
      parametros.Lines.Add(inttostr(tbSize.Position));
      // Intensidade
      parametros.Lines.Add(inttostr(tbI.Position));
      // Cor RG/BY
      parametros.Lines.Add(inttostr(tbRG.Position));
      // Orientação
      parametros.Lines.Add(inttostr(tbBY.Position));
      // Posição LIN/COL
      parametros.Lines.Add(inttostr(tbXY.Position));
      // Reconhecimento
      parametros.Lines.Add(inttostr(tbRN.Position));

      // 36 37 38 39 40 41 42 PESOS ENVIESAMENTO LEGION
      if cbWCor.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');
      parametros.Lines.Add(mskWc.Text);
      if cbWInt.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');
      parametros.Lines.Add(mskWi.Text);
      if cbWOri.Checked then parametros.Lines.Add('1') else parametros.Lines.Add('0');
      parametros.Lines.Add(mskWo.Text);
      parametros.Lines.Add(mskWsum.Text);

      AssignFile(arq_in,nome_arq);
      rewrite(arq_in);
      for i := 0 to 42 do
      begin
         writeln(arq_in,parametros.Lines.Strings[i]);
      end;
      closefile(arq_in);
   end else
   begin
      AssignFile(arq_in,nome_arq);
      reset(arq_in);
      for i := 0 to 42 do
      begin
         readln(arq_in,linha);
         case i of
           // LATERAL
           0 : if linha = '1' then rbExp4_8.Checked := true      else  rbExp4_8.Checked := false;
           1 : if linha = '1' then RadioButton13.Checked := true else  RadioButton13.Checked := false;
           2 : if linha = '1' then cbExp4_Color.Checked := true  else  cbExp4_Color.Checked := false;
           3 : if linha = '1' then cbExp4_MP.Checked := true     else  cbExp4_MP.Checked := false;
           4 : if linha = '1' then cbExp4_Cinza.Checked := true  else  cbExp4_Cinza.Checked := false;
           5 : if linha = '1' then cbExp4_Cores.Checked := true  else  cbExp4_Cores.Checked := false;
           6 : if linha = '1' then cbIni.Checked := true         else  cbIni.Checked := false;
           7 : tbpulso.Position := strtoint(linha);

           //LEGION
            8 : TrackBarthetap.Position := strtoint(linha);
            9 : TrackBarTetax.Position := strtoint(linha);
           10 : TrackBarWz.Position := strtoint(linha);
           11 : TrackBarTetaxz.Position := strtoint(linha);
           12 : TrackBarLimiarFundo.Position := strtoint(linha);

           // SALIENCIA
           13 : mskdt_SAL.Text := linha;
           14 : if linha <> '' then mskWInib.Text := linha;
           15 : if linha <> '' then tbLimiarFundoSeg.Position := strtoint(linha);
           16 : if linha <> '' then mskThetas.Text := linha;

           // SUAVIZAR IMAGEM
           //17 : if linha = '1' then cbExp4_S.Checked := true else cbExp4_S.Checked := false;
           //18 : SEditLadoExp4_M.Value := strtoint(linha);

           // 19 20 ORIENTAÇÃO
           19 : if linha = '1' then cbExp4_LO.Checked := true else cbExp4_LO.Checked := false;
           20 : SEditLO.Value := strtoint(linha);

           // 21 22 COR BORDA SEGMENTO
           21 : if linha = '1' then cbBorda.Checked := true else cbBorda.Checked := false;
           22 : ColorGridSeg.ForegroundIndex := strtoint(linha);

           // 23 24 SEGMENTADO
           23 : if linha = '1' then cb_Strech.Checked := true else cb_Strech.Checked := false;
           24 : if linha = '1' then cbVer_Sincro.Checked := true else cbVer_Sincro.Checked := false;

           // 25 26 27 MAPA DE SALIÊNCIA 3D
           25 : if linha = '1' then cbV3D.Checked := true else cbV3D.Checked := false;
           26 : tb3dDim.Position := strtoint(linha);
           27 : tbOrtho.Position := strtoint(linha);

           // 28 29 REGIÃO SALIENTE
           28 : if linha = '1' then rgIE.Checked := true else rgIE.Checked := false;
           29 : if linha = '1' then rbIS.Checked := true else rbIS.Checked := false;

           // 30 31 32 33 34 35 PESOS SALIENTES
           30 : begin
                  tbSize.Position := strtoint(linha);
                  tbSize.OnChange(Sender);
                end;
           31 : begin
                  tbI.Position := strtoint(linha);
                  tbI.OnChange(Sender);
                end;
           32 : begin
                  tbRG.Position := strtoint(linha);
                  tbRG.OnChange(Sender);
                end;
           33 : begin
                  tbBY.Position := strtoint(linha);
                  tbBY.OnChange(Sender);
                end;
           34 : begin
                  tbXY.Position := strtoint(linha);
                  tbXY.OnChange(Sender);
                end;
           35 : begin
                  tbRN.Position := strtoint(linha);
                  tbRN.OnChange(Sender);
                end;

           36 : if linha = '1' then cbWcor.Checked := true else cbWcor.Checked := false;
           37 : if linha <> '' then mskWc.Text := linha;
           38 : if linha = '1' then cbWint.Checked := true else cbWint.Checked := false;
           39 : if linha <> '' then mskWi.Text := linha;
           40 : if linha = '1' then cbWori.Checked := true else cbWori.Checked := false;
           41 : if linha <> '' then mskWo.Text := linha;
           42 : if linha <> '' then mskWsum.Text := linha;

         end;
      end;
      closefile(arq_in);
   end;
   semaforo := false;
   Filelistbox1.Update;

  except
      Application.MessageBox('Imagem Não Suportada ou Mapa de Saliência Inexistente.','Atenção',mb_iconExclamation);
  end;
end;

procedure TFormProjeto.SpeedButton7Click(Sender: TObject);
var
   n_arq : string;
   i : integer;
begin
   i := FileListBox1.ItemIndex;
   n_arq := FileListBox1.FileName;
   if FileExists(n_arq) then
      DeleteFile(n_arq);
   FileListBox1.Update;
   if i-1 >= 0 then FileListBox1.Selected[i-1] := true;
end;


procedure TFormProjeto.SegmentationAlone(Sender: TObject);
var
   //Variáveis Locais Gerais
     Bitmap : TBitmap;
     iBMPs, iBMPe24 : PRGB;
     iBMPe8 : PByteArray;
     pixelI : double;
     i,j,a,b,s,t,c,i_,j_ : integer;
     knl_t, knc_t : integer;

   // Variáveis Locais para a Suavização Mediana
     LR,LG,LB : TLista;

   // Vetor de Pesos para Angulos de Direções
     wa : array[0..2,0..2] of integer;
     maxi : double;

   // Variáveis Locais para o LEGION
     //first_pulse,
     sincronizado, seg_ordem, segmentado : boolean;
     checksum, checksum_temp : double;
     n_seg, count, sleft, stop, seg_Validos : integer;
     temp, temp2, temp3  : double;
     neuronios_pulsantes : integer;

     //Segmentos : array of TSeg;
     Seg_TMP : TSeg;
     Segmentos_Pulso : array of record
                        l,c : integer;
                       end;

     //SeG_Imagem : array of TImage;
     SeG_TMP_Imagem : TImage;
     ScrollBox : TScrollBox;

   // MLP
     IMaior, n: integer;
     Valor, Maior: double;

     ttt : double;

     Seg_Medias : TVetorAtributos_ReEscalar;


begin

  //Enviesamento top-down LEGION
    Wc := strtofloat(mskWc.text);
    Wi := strtofloat(mskWi.text);
    Wo := strtofloat(mskWo.text);
    Wsum := strtofloat(mskWsum.text);

  //LOCAL
     t_geral := 0;
     fat_dec := 0.9;
     NIt := (knl-1)*(knc-1);
     dt_Sal := strtofloat(mskDt_Sal.Text);
     wInib := strtofloat(mskWInib.Text);

  // Parâmetros do LEGION
  // Randomizando os valores iniciais...
  tmp := 0;

  // Inicializando neuronios LEGION
  for i:=0 to knl-1 do
    for j:=0 to knc-1 do
    begin
      neuronios[i,j].matrix := 0;
      neuronios[i,j].ly := rnd();
      neuronios[i,j].lx[0] := neuronios[i,j].ly / (MAXVAL+1);
      neuronios[i,j].ly := neuronios[i,j].ly - 2;
      if (neuronios[i,j].ly < 0) then neuronios[i,j].ly := 0;
      neuronios[i,j].trigger := 0;
      neuronios[i,j].cycle := 0;
      neuronios[i,j].d := 0;
      Neuronios[i,j].stim := neuronios[i,j].rgb[4];
    end;
  z := 0;

  // Verificando Conexões
  connections;

  // evaluate...
  jump[0] := 0;
  select();

  first_pulse := false;
  sincronizado := false;
  checksum := 0;
  n_seg := 0;
  seg_Ordem := false;
  temp :=0; temp2:=0; temp3:=0.0;
  count := 0;
  n := 0;

  timer.Enabled := true;
  while (timer.Enabled = true) do
  begin
     //Application.ProcessMessages;
     inc(tmp);
     count := 0;

     if (timer6.Enabled = false) then
     begin

     // calculando entradas da rede input
     for i:=0 to knl-1 do
       for j:=0 to knc-1 do
       begin

          //Application.ProcessMessages;

				  if (neuronios[i,j].lx[0] > thetae) and (jump[0] = 0) then
          begin  //* jumping down */
    					neuronios[i,j].lx[1] := LP;
    					neuronios[i,j].ly := -1;
    					neuronios[i,j].cycle := 1;
    					inc(count);
				  end else
              if (neuronios[i,j].lx[0] < thetae) then
              begin     //* summing up overall input */
                 temp :=  -Wz * bi(z,thetaz);  //* used to be 50 */

        				 //* the following is based on traversing through strongest link */
      					 temp2 := 0.0;
       			  	 if (j-1 >= 0) then
                 begin
						        temp3 := neuronios[i,j].W[0] * bi(neuronios[i,j-1].lx[0], thetae);
      					  	if (temp2 < temp3) then temp2 := temp3;
                 end;

				         if (i-1 >= 0) and (j-1 >= 0) then
                 begin
        						temp3 := neuronios[i,j].W[1] * bi(neuronios[i-1,j-1].lx[0], thetae);
      					    if (temp2 < temp3) then temp2 := temp3;
                 end;

        				 if (i-1 >= 0) then
                 begin
        						temp3 := neuronios[i,j].W[2] * bi(neuronios[i-1,j].lx[0], thetae);
        						if (temp2 < temp3) then temp2 := temp3;
                 end;

        				 if (i-1 >= 0) and (j+1 < knc) then
                 begin
        						temp3 := neuronios[i,j].W[3] * bi(neuronios[i-1,j+1].lx[0], thetae);
        						if (temp2 < temp3) then temp2 := temp3;
                 end;

        				 if (j+1 < knc) then
                 begin
        						temp3 := neuronios[i,j].W[4] * bi(neuronios[i,j+1].lx[0], thetae);
        						if (temp2 < temp3) then temp2 := temp3;
                 end;

        				 if (i+1 < knl) and (j+1 < knc) then
                 begin
        						temp3 := neuronios[i,j].W[5] * bi(neuronios[i+1,j+1].lx[0], thetae);
        						if (temp2 < temp3) then temp2 := temp3;
                 end;

        				 if (i+1 < knl) then
                 begin
        						temp3 := neuronios[i,j].W[6] * bi(neuronios[i+1,j].lx[0], thetae);
        						if (temp2 < temp3) then temp2 := temp3;
                 end;

        				 if (i+1 < knl) and (j-1 >= 0) then
                 begin
        						temp3 := neuronios[i,j].W[7] * bi(neuronios[i+1,j-1].lx[0], thetae);
        						if (temp2 < temp3) then temp2 := temp3;
                 end;

        					temp := temp + temp2;

        					if (neuronios[i,j].trigger = 1) or (temp > 0.25) then
                  begin               //* jumping up */
          						neuronios[i,j].lx[1] := RP;
             					neuronios[i,j].ly := MAXIMO;
            					jump[1] := 1;
            					inc(z);
                  end else
        					  	neuronios[i,j].Lx[1] := neuronios[i,j].Lx[0];
              end else
              begin
   					  	 neuronios[i,j].Lx[1] := neuronios[i,j].Lx[0];
              end;

    	 end;

     z := z - count;

  	 if (jump[1] = 0) and (z = 0) then
      	select();

     // Mostra neuronios sincronizados...
     if (z > 0) then
     begin
        segmentado := true;
        for i := 0 to knl-1 do
           for j := 0 to knc-1 do
               if (Neuronios[i,j].d = 0) and (Neuronios[i,j].Lx[1] >= thetae) then
               begin
                   segmentado := false;
                   continue;
               end;


        // Gravando Segmento
        checksum_temp := 0;

        if (segmentado = true) and (n_seg = 1) then
           checksum_temp := checksum;

        if segmentado = true then
        begin

           for i := imin+1 to imax-1 do
              for j := jmin+1 to jmax-1 do           //( testando       )
                  if (Neuronios[i,j].lx[1] >= thetae) and (Neuronios[i,j].d = 1) then
                     if first_pulse = false then
                     begin
                        checksum := checksum + (-checksum + (i+j)*0.1);
                        n_seg := 0;
                     end
                     else
                        checksum_temp := checksum_temp + (-checksum_temp + (i+j)*0.1);

           first_pulse := true;

           // Verificando se Sincronizou
           if (checksum = checksum_temp) then
           begin
              checksum := 0; first_pulse := false; n_seg := 0;
              sincronizado := true;

              if cbVer_Just_S.checked = true then
              begin
                 BitBtn2.OnClick(Sender);
              end
              else begin
                  timer6.Enabled := true;
              end;

           end
           else // grava o segmento
           begin

              setlength(Segmentos,n_seg+1); setlength(Seg_Classify,n_seg+1);
              setlength(Segmentos[n_seg].SegNeuronios,imax-imin+1,jmax-jmin+1);
              Segmentos[n_seg].Sknl := imax-imin+1;
              Segmentos[n_seg].Sknc := jmax-jmin+1;
              Segmentos[n_seg].imin := imin;
              Segmentos[n_seg].imax := imax;
              Segmentos[n_seg].jmin := jmin;
              Segmentos[n_seg].jmax := jmax;

              for i := imin to imax do
                 for j := jmin to jmax do
                 begin
                    if (Neuronios[i,j].Lx[1] >= thetae) and
                       (Neuronios[i,j].d = 1) then
                    begin
                        Segmentos[n_seg].SegNeuronios[i-imin,j-jmin] := Neuronios[i,j];
                        Neuronios[i,j].segN := n_seg;
                        for s := 0 to 12 do Segmentos[n_seg].SegMedias[s] := 0;
                    end;
                 end;

              inc(n_seg);
              Label68.caption := inttostr(n_seg);
           end;

        end;

     end;  // z > 0

     //if (cbExp4_MP.Checked) then
     sleep(trunc((tbpulso.Position/10)*1000));

     for i:=0 to knl-1 do   //* replace OLD by NEW */
       for j:=0 to knc-1 do
  				neuronios[i,j].lx[0] := neuronios[i,j].lx[1];

		 jump[0] := jump[1];
		 jump[1] := 0;

     end // if timer6 false
     else begin


         if seg_Ordem = false then
         begin
            seg_ordem := true;

            // Veficando o número de segmentos válidos pelo tamanho
            seg_validos := 0;
            for s := 0 to length(Segmentos)-1 do
                if (Segmentos[s].Sknl*Segmentos[s].Sknc) < (knl*knc*LimiarFundoSeg) then
                    inc(seg_validos);

            // Tamanho do Objeto
            Seg_Medias.max[10] := 0;
            Seg_Medias.min[10] := 0;

            // Pegando neuronios pulsantes
            setlength(Segmentos_Pulso,(knl*knc));

               // Zerando Medias
                           Seg_Medias.v[0] := 0;
                           Seg_Medias.max[0] := 0;//Segmentos[s].SegNeuronios[i,j].rgb_Itti[0];
                           Seg_Medias.min[0] := 255;//Segmentos[s].SegNeuronios[i,j].rgb_Itti[0];

                           Seg_Medias.v[1] := 0;
                           Seg_Medias.max[1] := 0;//Segmentos[s].SegNeuronios[i,j].rgb_Itti[1];
                           Seg_Medias.min[1] := 255;//Segmentos[s].SegNeuronios[i,j].rgb_Itti[1];

                           Seg_Medias.v[2] := 0;
                           Seg_Medias.max[2] := 0;//Segmentos[s].SegNeuronios[i,j].rgb_Itti[2];
                           Seg_Medias.min[2] := 255;//Segmentos[s].SegNeuronios[i,j].rgb_Itti[2];

                           Seg_Medias.v[3] := 0;
                           Seg_Medias.max[3] := 0;//Segmentos[s].SegNeuronios[i,j].l;
                           Seg_Medias.min[3] := knl;//Segmentos[s].SegNeuronios[i,j].l;

                           Seg_Medias.v[4] := 0;
                           Seg_Medias.max[4] := 0;//Segmentos[s].SegNeuronios[i,j].c;
                           Seg_Medias.min[4] := knc;//Segmentos[s].SegNeuronios[i,j].c;

                           Seg_Medias.v[5] := 0;
                           Seg_Medias.max[5] := 0;//Segmentos[s].SegNeuronios[i,j].ang[1];
                           Seg_Medias.min[5] := 255;//Segmentos[s].SegNeuronios[i,j].ang[1];

                           Seg_Medias.v[6] := 0;
                           Seg_Medias.max[6] := 0;//Segmentos[s].SegNeuronios[i,j].ang[2];
                           Seg_Medias.min[6] := 255;//Segmentos[s].SegNeuronios[i,j].ang[2];

                           Seg_Medias.v[7] := 0;
                           Seg_Medias.max[7] := 0;//Segmentos[s].SegNeuronios[i,j].ang[3];
                           Seg_Medias.min[7] := 255;//Segmentos[s].SegNeuronios[i,j].ang[3];

                           Seg_Medias.v[8] := 0;
                           Seg_Medias.max[8] := 0;//Segmentos[s].SegNeuronios[i,j].ang[4];
                           Seg_Medias.min[8] := 255;//Segmentos[s].SegNeuronios[i,j].ang[4];

                           Seg_Medias.v[9] := 0;
                           Seg_Medias.max[9] := 0;//trunc(Segmentos[s].SegNeuronios[i,j].rn);
                           Seg_Medias.min[9] := 255;//trunc(Segmentos[s].SegNeuronios[i,j].rn);

                           //Seg_Medias[10].max := 0;
                           //Seg_Medias[10].min := 0;

            for s := 0 to seg_validos-1 do
            begin

               // Normalização dos Valores [0,1]
               for i := 0 to Segmentos[s].Sknl-1 do
                  for j := 0 to Segmentos[s].Sknc-1 do
                       if (Segmentos[s].SegNeuronios[i,j].Lx[1] >= thetae) and
                          (Segmentos[s].SegNeuronios[i,j].d = 1) then
                       begin
                           // Atribuindo as Características
                           // Re-Escalar [0]
                              if Segmentos[s].SegNeuronios[i,j].rgb_Itti[0] > Seg_Medias.max[0] then
                                 Seg_Medias.max[0] := Segmentos[s].SegNeuronios[i,j].rgb_Itti[0];
                              if Segmentos[s].SegNeuronios[i,j].rgb_Itti[0] < Seg_Medias.min[0] then
                                 Seg_Medias.min[0] := Segmentos[s].SegNeuronios[i,j].rgb_Itti[0];
                           // Re-Escalar [1]
                              if Segmentos[s].SegNeuronios[i,j].rgb_Itti[1] > Seg_Medias.max[1] then
                                 Seg_Medias.max[1] := Segmentos[s].SegNeuronios[i,j].rgb_Itti[1];
                              if Segmentos[s].SegNeuronios[i,j].rgb_Itti[1] < Seg_Medias.min[1] then
                                 Seg_Medias.min[1] := Segmentos[s].SegNeuronios[i,j].rgb_Itti[1];
                           // Re-Escalar [2]
                              if Segmentos[s].SegNeuronios[i,j].rgb_Itti[2] > Seg_Medias.max[2] then
                                 Seg_Medias.max[2] := Segmentos[s].SegNeuronios[i,j].rgb_Itti[2];
                              if Segmentos[s].SegNeuronios[i,j].rgb_Itti[2] < Seg_Medias.min[2] then
                                 Seg_Medias.min[2] := Segmentos[s].SegNeuronios[i,j].rgb_Itti[2];

                           // X Y
                           // Re-Escalar [3]
                              if Segmentos[s].SegNeuronios[i,j].l > Seg_Medias.max[3] then
                                 Seg_Medias.max[3] := Segmentos[s].SegNeuronios[i,j].l;
                              if Segmentos[s].SegNeuronios[i,j].l < Seg_Medias.min[3] then
                                 Seg_Medias.min[3] := Segmentos[s].SegNeuronios[i,j].l;
                           // Re-Escalar [4]
                              if Segmentos[s].SegNeuronios[i,j].c > Seg_Medias.max[4] then
                                 Seg_Medias.max[4] := Segmentos[s].SegNeuronios[i,j].c;
                              if Segmentos[s].SegNeuronios[i,j].c < Seg_Medias.min[4] then
                                 Seg_Medias.min[4] := Segmentos[s].SegNeuronios[i,j].c;

                           //Angulos do Mapa
                           // Re-Escalar [5]
                              if Segmentos[s].SegNeuronios[i,j].ang[1] > Seg_Medias.max[5] then
                                 Seg_Medias.max[5] := Segmentos[s].SegNeuronios[i,j].ang[1];
                              if Segmentos[s].SegNeuronios[i,j].ang[1] < Seg_Medias.min[5] then
                                 Seg_Medias.min[5] := Segmentos[s].SegNeuronios[i,j].ang[1];
                           // Re-Escalar [6]
                              if Segmentos[s].SegNeuronios[i,j].ang[2] > Seg_Medias.max[6] then
                                 Seg_Medias.max[6] := Segmentos[s].SegNeuronios[i,j].ang[2];
                              if Segmentos[s].SegNeuronios[i,j].ang[2] < Seg_Medias.min[6] then
                                 Seg_Medias.min[6] := Segmentos[s].SegNeuronios[i,j].ang[2];
                           // Re-Escalar [7]
                              if Segmentos[s].SegNeuronios[i,j].ang[3] > Seg_Medias.max[7] then
                                 Seg_Medias.max[7] := Segmentos[s].SegNeuronios[i,j].ang[3];
                              if Segmentos[s].SegNeuronios[i,j].ang[3] < Seg_Medias.min[7] then
                                 Seg_Medias.min[7] := Segmentos[s].SegNeuronios[i,j].ang[3];
                           // Re-Escalar [8]
                              if Segmentos[s].SegNeuronios[i,j].ang[4] > Seg_Medias.max[8] then
                                 Seg_Medias.max[8] := Segmentos[s].SegNeuronios[i,j].ang[4];
                              if Segmentos[s].SegNeuronios[i,j].ang[4] < Seg_Medias.min[8] then
                                 Seg_Medias.min[8] := Segmentos[s].SegNeuronios[i,j].ang[4];

                           //Rede Neural Objeto ConhecidoConhecido 0 ou 1
                           // Re-Escalar [5]
                              if trunc(Segmentos[s].SegNeuronios[i,j].rn) > Seg_Medias.max[9] then
                                 Seg_Medias.max[9] := trunc(Segmentos[s].SegNeuronios[i,j].rn);
                              if trunc(Segmentos[s].SegNeuronios[i,j].rn) < Seg_Medias.min[9] then
                                 Seg_Medias.min[9] := trunc(Segmentos[s].SegNeuronios[i,j].rn);

                       end; // if

            end; // for s


            for s := 0 to seg_validos-1 do
            begin
               neuronios_pulsantes := 0;

               Seg_Medias.v[0] := 0;
               Seg_Medias.v[1] := 0;
               Seg_Medias.v[2] := 0;
               Seg_Medias.v[3] := 0;
               Seg_Medias.v[4] := 0;
               Seg_Medias.v[5] := 0;
               Seg_Medias.v[6] := 0;
               Seg_Medias.v[7] := 0;
               Seg_Medias.v[8] := 0;
               Seg_Medias.v[9] := 0;
               Seg_Medias.v[10] := 0;

               for i := 0 to Segmentos[s].Sknl-1 do
                  for j := 0 to Segmentos[s].Sknc-1 do
                       if (Segmentos[s].SegNeuronios[i,j].Lx[1] >= thetae) and
                          (Segmentos[s].SegNeuronios[i,j].d = 1) then
                       begin
                           Segmentos_Pulso[neuronios_pulsantes].l := Segmentos[s].SegNeuronios[i,j].l;
                           Segmentos_Pulso[neuronios_pulsantes].c := Segmentos[s].SegNeuronios[i,j].c;
                           neuronios_pulsantes := neuronios_pulsantes + 1;

                           // Média de Cadas Segmentos...
                           // Atribuindo as Características
                           Seg_Medias.v[0] := Seg_Medias.v[0] + Normalizar(Segmentos[s].SegNeuronios[i,j].rgb_Itti[0],Seg_Medias.min[0],Seg_Medias.max[0]);
                           Seg_Medias.v[1] := Seg_Medias.v[1] + Normalizar(Segmentos[s].SegNeuronios[i,j].rgb_Itti[1],Seg_Medias.min[1],Seg_Medias.max[1]);
                           Seg_Medias.v[2] := Seg_Medias.v[2] + Normalizar(Segmentos[s].SegNeuronios[i,j].rgb_Itti[2],Seg_Medias.min[2],Seg_Medias.max[2]);

                           // X Y
                           Seg_Medias.v[3] := Seg_Medias.v[3] + Normalizar(Segmentos[s].SegNeuronios[i,j].l,Seg_Medias.min[3],Seg_Medias.max[3]);
                           Seg_Medias.v[4] := Seg_Medias.v[4] + Normalizar(Segmentos[s].SegNeuronios[i,j].c,Seg_Medias.min[4],Seg_Medias.max[4]);

                           //Angulos do Mapa
                           Seg_Medias.v[5] := Seg_Medias.v[5] + Normalizar(Segmentos[s].SegNeuronios[i,j].ang[1],Seg_Medias.min[5],Seg_Medias.max[5]);
                           Seg_Medias.v[6] := Seg_Medias.v[6] + Normalizar(Segmentos[s].SegNeuronios[i,j].ang[2],Seg_Medias.min[6],Seg_Medias.max[6]);
                           Seg_Medias.v[7] := Seg_Medias.v[7] + Normalizar(Segmentos[s].SegNeuronios[i,j].ang[3],Seg_Medias.min[7],Seg_Medias.max[7]);
                           Seg_Medias.v[8] := Seg_Medias.v[8] + Normalizar(Segmentos[s].SegNeuronios[i,j].ang[4],Seg_Medias.min[8],Seg_Medias.max[8]);

                           //Rede Neural Objeto ConhecidoConhecido 0 ou 1
                           Seg_Medias.v[9] := Seg_Medias.v[9] + Normalizar(trunc(Segmentos[s].SegNeuronios[i,j].rn),Seg_Medias.min[9],Seg_Medias.max[9]);

                       end; // if

               // Tamanho do Segmento
               if s = 0 then
               begin
                  Seg_Medias.max[10] := neuronios_pulsantes;
                  Seg_Medias.min[10] := neuronios_pulsantes;
               end else
               begin
                  if neuronios_pulsantes > Seg_Medias.max[10] then
                     Seg_Medias.max[10] := neuronios_pulsantes;
                  if neuronios_pulsantes < Seg_Medias.min[10] then
                     Seg_Medias.min[10] := neuronios_pulsantes;
               end;

               // Calculo das Médias de Saliencia de cada Atributo
               // Intensidade
               Segmentos[s].SegMedias[0] := (Seg_Medias.v[0]/neuronios_pulsantes);

               // Cor RG e BY
               Segmentos[s].SegMedias[1] := (Seg_Medias.v[1]/neuronios_pulsantes);

               // Orientação
               Segmentos[s].SegMedias[2] := (Seg_Medias.v[2]/neuronios_pulsantes);

               // Calculo X e Y
               Segmentos[s].SegMedias[3] := (Seg_Medias.v[3]/neuronios_pulsantes);
               Segmentos[s].SegMedias[4] := (Seg_Medias.v[4]/neuronios_pulsantes);

               //Rede Neural Objeto ConhecidoConhecido
               Segmentos[s].SegMedias[9] := (Seg_Medias.v[9]/neuronios_pulsantes);

               // Size
               Segmentos[s].SegMedias[10] := (neuronios_pulsantes);

               // Tornando Segmento Válido de acordo com Média
               Segmentos[s].segValido := false;
               if ((Segmentos[s].SegMedias[0] + Segmentos[s].SegMedias[1] +
                  Segmentos[s].SegMedias[2] + Segmentos[s].SegMedias[9])/4) >= strtofloat(mskThetas.Text) then //> 0.03 then // INSERIR PARÂMETRO...!?!?!?!
                  //if (Segmentos[s].Sknl*Segmentos[s].Sknc) < (knl*knc*LimiarFundoSeg) then
                      Segmentos[s].segValido := true;

               // Zerando Valors
               Segmentos[s].IeD.i := 0;
               Segmentos[s].IeD.v := 0;
               Segmentos[s].IeD.y := 0;
               Segmentos[s].IeD.rn := 0;
               Segmentos[s].v_Max := 0;
               Segmentos[s].Classify := 0;
            end;

            // Normalizando o Tamanho do Segmento
            for s := 0 to seg_validos-1 do
            begin
               Segmentos[s].SegMedias[10] := Normalizar(trunc(Segmentos[s].SegMedias[10]),Seg_Medias.min[10],Seg_Medias.max[10]);
            end;

            BitBtn36.OnClick(Sender);

            setlength(Segmentos_Pulso,neuronios_pulsantes);

         end;

         //Memo1.Lines.Append(inttostr(neuronios_pulsantes));
         // Treinando pelos segmentos...----------------------------------------
         //for s := 0 to neuronios_pulsantes-1 do


             Application.ProcessMessages;
             if timer6.Enabled = false then exit;

             //repeat
             //randomize;
             if n_seg > 1 then
             begin
                repeat
                   s := randomrange(0,length(Segmentos));
                until (Segmentos[s].segValido = true) and (s <> seg_UP);
             end else
             begin
                repeat
                   s := randomrange(0,length(Segmentos));
                until (Segmentos[s].segValido = true);
             end;
             seg_UP := s;

         timer7.Enabled := true;
     end;

  end; // enquanto true

  FinalizarContagem;

end;


procedure TFormProjeto.Segmentation(Sender: TObject);
var
   //Variáveis Locais Gerais
     Bitmap : TBitmap;
     iBMPs, iBMPe24 : PRGB;
     iBMPe8 : PByteArray;
     pixelI : double;
     i,j,a,b,s,t,c,i_,j_ : integer;
     knl_t, knc_t : integer;

   // Variáveis Locais para a Suavização Mediana
     LR,LG,LB : TLista;

   // Vetor de Pesos para Angulos de Direções
     wa : array[0..2,0..2] of integer;
     maxi : double;

   // Variáveis Locais para o LEGION
     //first_pulse,
     sincronizado, seg_ordem, segmentado : boolean;
     checksum, checksum_temp : double;
     n_seg, count, sleft, stop, seg_Validos : integer;
     temp, temp2, temp3  : double;
     neuronios_pulsantes : integer;

     //Segmentos : array of TSeg;
     Seg_TMP : TSeg;
     Segmentos_Pulso : array of record
                        l,c : integer;
                       end;

     //SeG_Imagem : array of TImage;
     SeG_TMP_Imagem : TImage;
     ScrollBox : TScrollBox;

   // MLP
     IMaior, n: integer;
     Valor, Maior: double;

     ttt : double;

     Seg_Medias : TVetorAtributos_ReEscalar;

   // Grafico
   aSurfaceSeries : TSurfaceSeries;


begin

   // Limpando o Gráfico
   CSal.BottomAxis.Minimum := 0;
   CSal.BottomAxis.Maximum := knc;

   // Criando dinamicamente
   aSurfaceSeries := TSurfaceSeries.Create(Self);
   with aSurfaceSeries do
   begin
      NumXValues := knc;
      NumZValues := knl;
      Pen.Visible := true;
      Pen.Color := clblack;
      TimesZOrder := trunc(knl*0.11);//20;
      for i := 1 to knl do
         for j := 1 to knc do
             AddXYZ(j,0,i,'',clwhite);
   end;
   CSal.Series[0].Destroy;
   CSal.AddSeries(TSurfaceSeries(aSurfaceSeries));

   CO.Series[0].Clear;

   for i := 0 to 29 do
      COII.Series[i].Clear;

   cont_it := 0;

   //PageControl13.TabIndex := 5;
   if cbExp4_MP.Checked = false then CBBorda.Checked := false;

  //LEGION----------------------------------------------------------------------
    CBSeg.OnClick(Sender);
    CBSeg.Checked := false;
    Img_Segmentos.Picture := nil;

  //Enviesamento top-down LEGION
    Wc := strtofloat(mskWc.text);
    Wi := strtofloat(mskWi.text);
    Wo := strtofloat(mskWo.text);
    Wsum := strtofloat(mskWsum.text);

  //LOCAL
     t_geral := 0;
     fat_dec := 0.9;
     NIt := (knl-1)*(knc-1);
     dt_Sal := strtofloat(mskDt_Sal.Text);
     wInib := strtofloat(mskWInib.Text);

  // Visualização do Mapa SOM
     BitmapK := TBitmap.Create;
     BitmapK.PixelFormat := pf24bit;
     BitmapK.Width := knc;
     BitmapK.Height := knl;

  // Mapa de Atributo Saliente Atributo Saliencia
     BitmapAS := TBitmap.Create;
     BitmapAS.PixelFormat := pf24bit;
     BitmapAS.Width := knc;
     BitmapAS.Height := knl;
     maxl := -1; maxc := -1; max_seg := -1;
     saliency := 0;
     saliency_max :=0;

  // GLOBAL: Visualização da Imagem Original Segmentada
     BitmapS := TBitmap.Create;
     BitmapS.PixelFormat := pf24bit;
     BitmapS.Width := knc;
     BitmapS.Height := knl;
     Segmentos := nil;
     Seg_Imagem := nil;

  // GLOBAL: Carregando base de treinamento da MLP
     SpeedButton1.Click();
     SpeedButton4.Click();
     Treina_SOM := false;

  // LOCAL: Limpando a Imagem em que será Destacado o Objeto Saliente
  Bitmap := TBitmap.Create;
  try
     Bitmap.PixelFormat := pf24bit;
     Bitmap.Width := knc;
     Bitmap.Height := knl;
     for i := 0 to knl-1 do
     begin
       iBMPs := Bitmap.ScanLine[i];
       for j := 0 to knc-1 do
       begin
         iBMPs[j].r := 0;
         iBMPs[j].g := 0;
         iBMPs[j].b := 0;
       end;
     end;
     // Mostrando Imagem
     mod_ImgExp4_IeF.Picture.Graphic := Bitmap;
  finally
     Bitmap.Free;
  end;

  // LOCAL: Limpando a Imagem SOM e SOM SAL
  Bitmap := TBitmap.Create;
  try
     Bitmap.PixelFormat := pf24bit;
     Bitmap.Width := knc;
     Bitmap.Height := knl;
     for i := 0 to knl-1 do
     begin
       iBMPs := Bitmap.ScanLine[i];
       for j := 0 to knc-1 do
       begin
         iBMPs[j].r := 0;
         iBMPs[j].g := 0;
         iBMPs[j].b := 0;
       end;
     end;
     // Mostrando Imagem
     mod_ImgExp4_SOM_SAL.Picture.Graphic := Bitmap;
  finally
     Bitmap.Free;
  end;

  // Inicializando neuronios do SOM
  randomize;
  for i := 0 to knl-1 do
  begin
     for j := 0 to knc-1 do
     begin
         //SOM Intensidades, RG e BY
            if cbExp4_Color.Checked then
            begin
               KNeuronios[i,j][0] := randomrange(0,255);
               KNeuronios[i,j][1] := randomrange(0,255);
               KNeuronios[i,j][2] := randomrange(0,255);
            end else begin
               KNeuronios[i,j][0] := randomrange(0,255);
               KNeuronios[i,j][1] := KNeuronios[i,j][0];
               KNeuronios[i,j][2] := KNeuronios[i,j][0];
            end;
         //SOM Posições Iniciais do Mapa
            KNeuronios[i,j][3] := randomrange(0,knl-1);
            KNeuronios[i,j][4] := randomrange(0,knc-1);
         //SOM Angulos Iniciais do Mapa
            KNeuronios[i,j][5] := randomrange(0,255);
            KNeuronios[i,j][6] := randomrange(0,255);
            KNeuronios[i,j][7] := randomrange(0,255);
            KNeuronios[i,j][8] := randomrange(0,255);

          //SOM Reconhecimento
            KNeuronios[i,j][9] := 0;//randomrange(0,255);

         //SOM R,G,B
            KNeuronios[i,j][10] := randomrange(0,255);
            KNeuronios[i,j][11] := randomrange(0,255);
            KNeuronios[i,j][12] := randomrange(0,255);

         //Mapa de Atributo Saliencia Neuronios
            //SNeuronios[i,j].i := 0;
            //SNeuronios[i,j].v := 0;
            //SNeuronios[i,j].y := 0;
            //SNeuronios[i,j].rn := 0;

        end;
   end;

  // Parâmetros do LEGION
  // Randomizando os valores iniciais...
  tmp := 0;

  // Inicializando neuronios LEGION
  for i:=0 to knl-1 do
    for j:=0 to knc-1 do
    begin

      neuronios[i,j].matrix := 0;
      neuronios[i,j].ly := rnd();
      neuronios[i,j].lx[0] := neuronios[i,j].ly / (MAXVAL+1);
      neuronios[i,j].ly := neuronios[i,j].ly - 2;
      if (neuronios[i,j].ly < 0) then neuronios[i,j].ly := 0;
      neuronios[i,j].trigger := 0;
      neuronios[i,j].cycle := 0;

      neuronios[i,j].d := 0;
      Neuronios[i,j].stim := neuronios[i,j].rgb[4];
    end;
  z := 0;

  // Verificando Conexões
  connections;

  // evaluate...
  jump[0] := 0;
  select();

  first_pulse := false;
  sincronizado := false;
  checksum := 0;
  n_seg := 0;
  seg_Ordem := false;
  temp :=0; temp2:=0; temp3:=0.0;
  count := 0;
  n := 0;

  timer.Enabled := true;
  while (timer.Enabled = true) do
  begin
     //Application.ProcessMessages;
     inc(tmp);
     count := 0;

     if (timer6.Enabled = false) then
     begin

     // calculando entradas da rede input
     for i:=0 to knl-1 do
       for j:=0 to knc-1 do
       begin

          //Application.ProcessMessages;

				  if (neuronios[i,j].lx[0] > thetae) and (jump[0] = 0) then
          begin  //* jumping down */
    					neuronios[i,j].lx[1] := LP;
    					neuronios[i,j].ly := -1;
    					neuronios[i,j].cycle := 1;
    					inc(count);
				  end else
              if (neuronios[i,j].lx[0] < thetae) then
              begin     //* summing up overall input */
                 temp :=  -Wz * bi(z,thetaz);  //* used to be 50 */

        				 //* the following is based on traversing through strongest link */
      					 temp2 := 0.0;
       			  	 if (j-1 >= 0) then
                 begin
						        temp3 := neuronios[i,j].W[0] * bi(neuronios[i,j-1].lx[0], thetae);
      					  	if (temp2 < temp3) then temp2 := temp3;
                 end;

				         if (i-1 >= 0) and (j-1 >= 0) then
                 begin
        						temp3 := neuronios[i,j].W[1] * bi(neuronios[i-1,j-1].lx[0], thetae);
      					    if (temp2 < temp3) then temp2 := temp3;
                 end;

        				 if (i-1 >= 0) then
                 begin
        						temp3 := neuronios[i,j].W[2] * bi(neuronios[i-1,j].lx[0], thetae);
        						if (temp2 < temp3) then temp2 := temp3;
                 end;

        				 if (i-1 >= 0) and (j+1 < knc) then
                 begin
        						temp3 := neuronios[i,j].W[3] * bi(neuronios[i-1,j+1].lx[0], thetae);
        						if (temp2 < temp3) then temp2 := temp3;
                 end;

        				 if (j+1 < knc) then
                 begin
        						temp3 := neuronios[i,j].W[4] * bi(neuronios[i,j+1].lx[0], thetae);
        						if (temp2 < temp3) then temp2 := temp3;
                 end;

        				 if (i+1 < knl) and (j+1 < knc) then
                 begin
        						temp3 := neuronios[i,j].W[5] * bi(neuronios[i+1,j+1].lx[0], thetae);
        						if (temp2 < temp3) then temp2 := temp3;
                 end;

        				 if (i+1 < knl) then
                 begin
        						temp3 := neuronios[i,j].W[6] * bi(neuronios[i+1,j].lx[0], thetae);
        						if (temp2 < temp3) then temp2 := temp3;
                 end;

        				 if (i+1 < knl) and (j-1 >= 0) then
                 begin
        						temp3 := neuronios[i,j].W[7] * bi(neuronios[i+1,j-1].lx[0], thetae);
        						if (temp2 < temp3) then temp2 := temp3;
                 end;

        					temp := temp + temp2;

        					if (neuronios[i,j].trigger = 1) or (temp > 0.25) then
                  begin               //* jumping up */
          						neuronios[i,j].lx[1] := RP;
             					neuronios[i,j].ly := MAXIMO;
            					jump[1] := 1;
            					inc(z);
                  end else
        					  	neuronios[i,j].Lx[1] := neuronios[i,j].Lx[0];
              end else
              begin
   					  	 neuronios[i,j].Lx[1] := neuronios[i,j].Lx[0];
              end;

    	 end;

     z := z - count;

  	 if (jump[1] = 0) and (z = 0) then
      	select();

     // Apresentando...

     // Mostra neuronios sincronizados...
     if (z > 0) then
     begin
        segmentado := true;
        for i := 0 to knl-1 do
           for j := 0 to knc-1 do
               if (Neuronios[i,j].d = 0) and (Neuronios[i,j].Lx[1] >= thetae) then
               begin
                   segmentado := false;
                   continue;
               end;

        if cbVer_Sincro.Checked then
           ShowSincro_LEGION
        else
        begin
           ShowSincro_LEGION_no_Preview;
           if segmentado = true then
              ShowSincro_LEGION;
        end;

        // Gravando Segmento
        checksum_temp := 0;

        if (segmentado = true) and (n_seg = 1) then
           checksum_temp := checksum;

        if segmentado = true then
        begin

           for i := imin+1 to imax-1 do
              for j := jmin+1 to jmax-1 do           //( testando       )
                  if (Neuronios[i,j].lx[1] >= thetae) and (Neuronios[i,j].d = 1) then
                     if first_pulse = false then
                     begin
                        checksum := checksum + (-checksum + (i+j)*0.1);
                        n_seg := 0;
                     end
                     else
                        checksum_temp := checksum_temp + (-checksum_temp + (i+j)*0.1);

           first_pulse := true;

           // Verificando se Sincronizou
           if (checksum = checksum_temp) then
           begin
              checksum := 0; first_pulse := false; n_seg := 0;
              sincronizado := true;

              Sleft := 3; Stop := 3;

              setlength(Seg_Imagem,length(Segmentos));

              Scrollbox := TScrollBox.Create(FormProjeto);
              Scrollbox.Parent := Tabsheet13;
              Scrollbox.Align := alClient;

              for s := 0 to length(Segmentos)-1 do
              begin

                 Seg_Imagem[s] := TImage.Create(FormProjeto);
                 Seg_Imagem[s].Parent := ScrollBox;
                 Seg_Imagem[s].Height := 180;
                 Seg_Imagem[s].Width := 180;
                 Seg_Imagem[s].AutoSize := false;
                 Seg_Imagem[s].Center := true;
                 Seg_Imagem[s].Proportional := true;
                 Seg_Imagem[s].Left := sleft;
                 Seg_Imagem[s].Top := stop;
                 Seg_Imagem[s].OnClick := Zoom;
                 sleft := sleft + 6 + Seg_Imagem[s].Width;
                 if sleft > 700 then
                 begin
                    sleft := 3;
                    stop := stop + Seg_Imagem[s].Height + 6;
                 end;

                 Bitmap := TBitmap.Create;
                 try

                    Bitmap.PixelFormat := pf24bit;
                    Bitmap.Width := Segmentos[s].Sknc;
                    Bitmap.Height := Segmentos[s].Sknl;
                    for i := 0 to Segmentos[s].Sknl-1 do
                    begin
                       iBMPs := Bitmap.ScanLine[i];
                       for j := 0 to Segmentos[s].Sknc-1 do
                       begin

                          //----------------------------------------------------
                          if (Segmentos[s].SegNeuronios[i,j].Lx[1] >= thetae) and
                             (Segmentos[s].SegNeuronios[i,j].d = 1) then
                          begin

                             if cbExp4_Cinza.Checked then
                             begin
                                iBMPs[j].r := Segmentos[s].SegNeuronios[i,j].rgb[4];
                                iBMPs[j].g := Segmentos[s].SegNeuronios[i,j].rgb[4];
                                iBMPs[j].b := Segmentos[s].SegNeuronios[i,j].rgb[4];
                             end
                             else
                                if cbExp4_Cores.Checked then
                                begin
                                    iBMPs[j].r := trunc(Segmentos[s].SegNeuronios[i,j].Seg_rgb[0]);
                                    iBMPs[j].g := trunc(Segmentos[s].SegNeuronios[i,j].Seg_rgb[1]);
                                    iBMPs[j].b := trunc(Segmentos[s].SegNeuronios[i,j].Seg_rgb[2]);
                                end
                                else
                                begin
                                    iBMPs[j].r := trunc(Segmentos[s].SegNeuronios[i,j].lx[1]*255);
                                    iBMPs[j].g := trunc(Segmentos[s].SegNeuronios[i,j].lx[1]*255);
                                    iBMPs[j].b := trunc(Segmentos[s].SegNeuronios[i,j].lx[1]*255);
                                end;
                          end
                          else begin
                             //if (cbExp4_MP.Checked) then
                             begin
                                iBMPs[j].r := 0;
                                iBMPs[j].g := 0;
                                iBMPs[j].b := 0;
                             end;
                          end;
                          //------------------------------------------------------------------------------

                          //MLP.SetInput(i*13+j+1,Bitmap.Canvas.Pixels[i,j] shr 23); // 0 ou 1

                       end;
                    end;
                    // Mostrando Imagem
                    Seg_Imagem[s].Picture.Graphic := Bitmap;
                    //sleep(2000);
                 finally
                    Bitmap.Free;
                 end;


                 //if (Segmentos[s].Sknl = 13) and (Segmentos[s].Sknc = 13) then
                 begin
                 // Verificando Classificação MLP
                 for i := 0 to nc do
                    for j := 0 to nl do
                  //for i := 0 to Segmentos[s].Sknl-1 do
                    //for j := 0 to Segmentos[s].Sknc-1 do
                    begin
                       n := i*nc+j+1; // índice do neurônio de entrada
                       //if (Segmentos[s].SegNeuronios[i,j].Lx[1] >= thetae) and
                          //(Segmentos[s].SegNeuronios[i,j].d = 1) then
                          //ttt := Seg_Imagem[s].Picture.Bitmap.Canvas.Pixels[j,i] shr 23;
                          //MLP.SetInput(n,Seg_Imagem[s].Picture.Bitmap.Canvas.Pixels[j,i] shr 23); // 0 ou 1
                          //MLP.SetInput(n,0)
                       //else MLP.SetInput(n,1);    // 0 ou 1
                    end;
                 //MLP.Test;
                 {
                 mod_ed0.Text := Format('%2.4f',[MLP.GetOutput(1)]);
                 mod_ed1.Text := Format('%2.4f',[MLP.GetOutput(2)]);
                 mod_ed2.Text := Format('%2.4f',[MLP.GetOutput(3)]);
                 mod_ed3.Text := Format('%2.4f',[MLP.GetOutput(4)]);
                 }
                 { Verifica a maior saída para indicação visual }
                 Maior := -10;
                 for i := 1 to snClasses.Value do
                 begin
                     //Valor := MLP.GetOutput(i);
                     if Valor > Maior then
                     begin
                        Maior := Valor;
                        IMaior := i;
                     end;
                 end;

                 { Todas com fundo branco }
                 mod_sp0.Brush.Color := clWhite;
                 mod_sp1.Brush.Color := clWhite;
                 mod_sp2.Brush.Color := clWhite;
                 mod_sp3.Brush.Color := clWhite;

                 { Se semelhança for maior que 0.95}
                 if (Maior > 0.95) and (Maior < 1.00) then
                 begin
                     { Exceto a maior }
                      case IMaior of
                          1:  mod_sp0.Brush.Color := clMoneyGreen;
                          2:  mod_sp1.Brush.Color := clMoneyGreen;
                          3:  mod_sp2.Brush.Color := clMoneyGreen;
                          4:  mod_sp3.Brush.Color := clMoneyGreen;
                      end;


                     { Alimentando neurônios reconhecidos pela MLP}
                      for i := 0 to Segmentos[s].Sknl-1 do
                           for j := 0 to Segmentos[s].Sknc-1 do
                               if (Segmentos[s].SegNeuronios[i,j].Lx[1] >= thetae) and
                                  (Segmentos[s].SegNeuronios[i,j].d = 1) then
                               begin
                                   //Neuronios[i+Segmentos[s].imin,j+Segmentos[s].jmin].rn := 255;
                                   //Segmentos[s].SegNeuronios[i,j].rn := 255;
                               end;


                 end; // se for de tamanho 13
                 end;

                 { Alimentando neurônios reconhecidos pela MLP}
                   for i := 0 to Segmentos[s].Sknl-1 do
                        for j := 0 to Segmentos[s].Sknc-1 do
                            if (Segmentos[s].SegNeuronios[i,j].Lx[1] >= thetae) and
                               (Segmentos[s].SegNeuronios[i,j].d = 1) then
                            begin
                                Segmentos[s].SegNeuronios[i,j].rn :=
                                    Neuronios[i+Segmentos[s].imin,j+Segmentos[s].jmin].rn;
                            end;

              end;//-------------


              ///////////////////
              //if cbExp4_MP.Checked = false then
                 for i := 0 to knl-1 do
                    for j := 0 to knc-1 do
                    begin
                       iBMPs := mod_ImgExp4_IeF.Picture.Bitmap.ScanLine[i];
                       if (iBMPs[j].r = 255) and (iBMPs[j].g = 255) and
                          (iBMPs[j].b = 255) then
                       begin
                          iBMPs[j].r := 0; iBMPs[j].g := 0; iBMPs[j].b := 0;
                       end;
                       Neuronios[i,j].rgb[0] := iBMPs[j].r;
                       Neuronios[i,j].rgb[1] := iBMPs[j].g;
                       Neuronios[i,j].rgb[2] := iBMPs[j].b;


                    end;

              ///////////////////

              if cbVer_Just_S.checked = true then
              begin
                 //Atualiza_para_SOM;

                 BitBtn2.OnClick(Sender);
              end
              else begin
              // Saindo da Sincronização ... e habilitando o Kohonem

                 //Atualiza_para_SOM;

                 //PageControl13.TabIndex := 8;
                 //BitBtn35.OnClick(Sender); // exit;
                 timer6.Enabled := true;
                 //PageControl13.TabIndex := 8;

                 //BitBtn36.OnClick(Sender);
                 //PageControl13.TabIndex := 10;
                         // exit;

              end;

           end
           else // grava o segmento
           begin

              setlength(Segmentos,n_seg+1); setlength(Seg_Classify,n_seg+1);
              setlength(Segmentos[n_seg].SegNeuronios,imax-imin+1,jmax-jmin+1);
              Segmentos[n_seg].Sknl := imax-imin+1;
              Segmentos[n_seg].Sknc := jmax-jmin+1;
              Segmentos[n_seg].imin := imin;
              Segmentos[n_seg].imax := imax;
              Segmentos[n_seg].jmin := jmin;
              Segmentos[n_seg].jmax := jmax;

              for i := imin to imax do
                 for j := jmin to jmax do
                 begin
                    if (Neuronios[i,j].Lx[1] >= thetae) and
                       (Neuronios[i,j].d = 1) then
                    begin
                        Segmentos[n_seg].SegNeuronios[i-imin,j-jmin] := Neuronios[i,j];
                        Neuronios[i,j].segN := n_seg;
                        for s := 0 to 12 do Segmentos[n_seg].SegMedias[s] := 0;
                    end;
                 end;

              inc(n_seg);
              Label68.caption := inttostr(n_seg);
           end;

        end;

     end;  // z > 0

     //if (cbExp4_MP.Checked) then
     sleep(trunc((tbpulso.Position/10)*1000));

     for i:=0 to knl-1 do   //* replace OLD by NEW */
       for j:=0 to knc-1 do
  				neuronios[i,j].lx[0] := neuronios[i,j].lx[1];

		 jump[0] := jump[1];
		 jump[1] := 0;

     end // if timer6 false
     else begin


         if seg_Ordem = false then
         begin
            seg_ordem := true;

            // Veficando o número de segmentos válidos pelo tamanho
            seg_validos := 0;
            for s := 0 to length(Segmentos)-1 do
            begin
                Memo1.Lines.Append(formatfloat('0.0',Segmentos[s].Sknl*Segmentos[s].Sknc));
                if (Segmentos[s].Sknl*Segmentos[s].Sknc) < (knl*knc*LimiarFundoSeg) then
                begin
                   inc(seg_validos);
                   Memo1.Lines.Append('OK');
                end else
                begin
                   Memo1.Lines.Append('No');
                end;
            end;

            // Tamanho do Objeto
            Seg_Medias.max[10] := 0;
            Seg_Medias.min[10] := 0;

            // Pegando neuronios pulsantes
            setlength(Segmentos_Pulso,(knl*knc));

               // Zerando Medias
                           Seg_Medias.v[0] := 0;
                           Seg_Medias.max[0] := 0;//Segmentos[s].SegNeuronios[i,j].rgb_Itti[0];
                           Seg_Medias.min[0] := 255;//Segmentos[s].SegNeuronios[i,j].rgb_Itti[0];

                           Seg_Medias.v[1] := 0;
                           Seg_Medias.max[1] := 0;//Segmentos[s].SegNeuronios[i,j].rgb_Itti[1];
                           Seg_Medias.min[1] := 255;//Segmentos[s].SegNeuronios[i,j].rgb_Itti[1];

                           Seg_Medias.v[2] := 0;
                           Seg_Medias.max[2] := 0;//Segmentos[s].SegNeuronios[i,j].rgb_Itti[2];
                           Seg_Medias.min[2] := 255;//Segmentos[s].SegNeuronios[i,j].rgb_Itti[2];

                           Seg_Medias.v[3] := 0;
                           Seg_Medias.max[3] := 0;//Segmentos[s].SegNeuronios[i,j].l;
                           Seg_Medias.min[3] := knl;//Segmentos[s].SegNeuronios[i,j].l;

                           Seg_Medias.v[4] := 0;
                           Seg_Medias.max[4] := 0;//Segmentos[s].SegNeuronios[i,j].c;
                           Seg_Medias.min[4] := knc;//Segmentos[s].SegNeuronios[i,j].c;

                           Seg_Medias.v[5] := 0;
                           Seg_Medias.max[5] := 0;//Segmentos[s].SegNeuronios[i,j].ang[1];
                           Seg_Medias.min[5] := 255;//Segmentos[s].SegNeuronios[i,j].ang[1];

                           Seg_Medias.v[6] := 0;
                           Seg_Medias.max[6] := 0;//Segmentos[s].SegNeuronios[i,j].ang[2];
                           Seg_Medias.min[6] := 255;//Segmentos[s].SegNeuronios[i,j].ang[2];

                           Seg_Medias.v[7] := 0;
                           Seg_Medias.max[7] := 0;//Segmentos[s].SegNeuronios[i,j].ang[3];
                           Seg_Medias.min[7] := 255;//Segmentos[s].SegNeuronios[i,j].ang[3];

                           Seg_Medias.v[8] := 0;
                           Seg_Medias.max[8] := 0;//Segmentos[s].SegNeuronios[i,j].ang[4];
                           Seg_Medias.min[8] := 255;//Segmentos[s].SegNeuronios[i,j].ang[4];

                           Seg_Medias.v[9] := 0;
                           Seg_Medias.max[9] := 0;//trunc(Segmentos[s].SegNeuronios[i,j].rn);
                           Seg_Medias.min[9] := 255;//trunc(Segmentos[s].SegNeuronios[i,j].rn);

                           //Seg_Medias[10].max := 0;
                           //Seg_Medias[10].min := 0;

            for s := 0 to seg_validos-1 do
            begin

               // Normalização dos Valores [0,1]
               for i := 0 to Segmentos[s].Sknl-1 do
                  for j := 0 to Segmentos[s].Sknc-1 do
                       if (Segmentos[s].SegNeuronios[i,j].Lx[1] >= thetae) and
                          (Segmentos[s].SegNeuronios[i,j].d = 1) then
                       begin
                           // Atribuindo as Características
                           // Re-Escalar [0]
                              if Segmentos[s].SegNeuronios[i,j].rgb_Itti[0] > Seg_Medias.max[0] then
                                 Seg_Medias.max[0] := Segmentos[s].SegNeuronios[i,j].rgb_Itti[0];
                              if Segmentos[s].SegNeuronios[i,j].rgb_Itti[0] < Seg_Medias.min[0] then
                                 Seg_Medias.min[0] := Segmentos[s].SegNeuronios[i,j].rgb_Itti[0];
                           // Re-Escalar [1]
                              if Segmentos[s].SegNeuronios[i,j].rgb_Itti[1] > Seg_Medias.max[1] then
                                 Seg_Medias.max[1] := Segmentos[s].SegNeuronios[i,j].rgb_Itti[1];
                              if Segmentos[s].SegNeuronios[i,j].rgb_Itti[1] < Seg_Medias.min[1] then
                                 Seg_Medias.min[1] := Segmentos[s].SegNeuronios[i,j].rgb_Itti[1];
                           // Re-Escalar [2]
                              if Segmentos[s].SegNeuronios[i,j].rgb_Itti[2] > Seg_Medias.max[2] then
                                 Seg_Medias.max[2] := Segmentos[s].SegNeuronios[i,j].rgb_Itti[2];
                              if Segmentos[s].SegNeuronios[i,j].rgb_Itti[2] < Seg_Medias.min[2] then
                                 Seg_Medias.min[2] := Segmentos[s].SegNeuronios[i,j].rgb_Itti[2];

                           // X Y
                           // Re-Escalar [3]
                              if Segmentos[s].SegNeuronios[i,j].l > Seg_Medias.max[3] then
                                 Seg_Medias.max[3] := Segmentos[s].SegNeuronios[i,j].l;
                              if Segmentos[s].SegNeuronios[i,j].l < Seg_Medias.min[3] then
                                 Seg_Medias.min[3] := Segmentos[s].SegNeuronios[i,j].l;
                           // Re-Escalar [4]
                              if Segmentos[s].SegNeuronios[i,j].c > Seg_Medias.max[4] then
                                 Seg_Medias.max[4] := Segmentos[s].SegNeuronios[i,j].c;
                              if Segmentos[s].SegNeuronios[i,j].c < Seg_Medias.min[4] then
                                 Seg_Medias.min[4] := Segmentos[s].SegNeuronios[i,j].c;

                           //Angulos do Mapa
                           // Re-Escalar [5]
                              if Segmentos[s].SegNeuronios[i,j].ang[1] > Seg_Medias.max[5] then
                                 Seg_Medias.max[5] := Segmentos[s].SegNeuronios[i,j].ang[1];
                              if Segmentos[s].SegNeuronios[i,j].ang[1] < Seg_Medias.min[5] then
                                 Seg_Medias.min[5] := Segmentos[s].SegNeuronios[i,j].ang[1];
                           // Re-Escalar [6]
                              if Segmentos[s].SegNeuronios[i,j].ang[2] > Seg_Medias.max[6] then
                                 Seg_Medias.max[6] := Segmentos[s].SegNeuronios[i,j].ang[2];
                              if Segmentos[s].SegNeuronios[i,j].ang[2] < Seg_Medias.min[6] then
                                 Seg_Medias.min[6] := Segmentos[s].SegNeuronios[i,j].ang[2];
                           // Re-Escalar [7]
                              if Segmentos[s].SegNeuronios[i,j].ang[3] > Seg_Medias.max[7] then
                                 Seg_Medias.max[7] := Segmentos[s].SegNeuronios[i,j].ang[3];
                              if Segmentos[s].SegNeuronios[i,j].ang[3] < Seg_Medias.min[7] then
                                 Seg_Medias.min[7] := Segmentos[s].SegNeuronios[i,j].ang[3];
                           // Re-Escalar [8]
                              if Segmentos[s].SegNeuronios[i,j].ang[4] > Seg_Medias.max[8] then
                                 Seg_Medias.max[8] := Segmentos[s].SegNeuronios[i,j].ang[4];
                              if Segmentos[s].SegNeuronios[i,j].ang[4] < Seg_Medias.min[8] then
                                 Seg_Medias.min[8] := Segmentos[s].SegNeuronios[i,j].ang[4];

                           //Rede Neural Objeto ConhecidoConhecido 0 ou 1
                           // Re-Escalar [5]
                              if trunc(Segmentos[s].SegNeuronios[i,j].rn) > Seg_Medias.max[9] then
                                 Seg_Medias.max[9] := trunc(Segmentos[s].SegNeuronios[i,j].rn);
                              if trunc(Segmentos[s].SegNeuronios[i,j].rn) < Seg_Medias.min[9] then
                                 Seg_Medias.min[9] := trunc(Segmentos[s].SegNeuronios[i,j].rn);

                       end; // if

            end; // for s


            for s := 0 to seg_validos-1 do
            begin
               neuronios_pulsantes := 0;

               Seg_Medias.v[0] := 0;
               Seg_Medias.v[1] := 0;
               Seg_Medias.v[2] := 0;
               Seg_Medias.v[3] := 0;
               Seg_Medias.v[4] := 0;
               Seg_Medias.v[5] := 0;
               Seg_Medias.v[6] := 0;
               Seg_Medias.v[7] := 0;
               Seg_Medias.v[8] := 0;
               Seg_Medias.v[9] := 0;
               Seg_Medias.v[10] := 0;

               for i := 0 to Segmentos[s].Sknl-1 do
                  for j := 0 to Segmentos[s].Sknc-1 do
                       if (Segmentos[s].SegNeuronios[i,j].Lx[1] >= thetae) and
                          (Segmentos[s].SegNeuronios[i,j].d = 1) then
                       begin
                           Segmentos_Pulso[neuronios_pulsantes].l := Segmentos[s].SegNeuronios[i,j].l;
                           Segmentos_Pulso[neuronios_pulsantes].c := Segmentos[s].SegNeuronios[i,j].c;
                           neuronios_pulsantes := neuronios_pulsantes + 1;

                           // Média de Cadas Segmentos...
                           // Atribuindo as Características
                           Seg_Medias.v[0] := Seg_Medias.v[0] + Normalizar(Segmentos[s].SegNeuronios[i,j].rgb_Itti[0],Seg_Medias.min[0],Seg_Medias.max[0]);
                           Seg_Medias.v[1] := Seg_Medias.v[1] + Normalizar(Segmentos[s].SegNeuronios[i,j].rgb_Itti[1],Seg_Medias.min[1],Seg_Medias.max[1]);
                           Seg_Medias.v[2] := Seg_Medias.v[2] + Normalizar(Segmentos[s].SegNeuronios[i,j].rgb_Itti[2],Seg_Medias.min[2],Seg_Medias.max[2]);

                           // X Y
                           Seg_Medias.v[3] := Seg_Medias.v[3] + Normalizar(Segmentos[s].SegNeuronios[i,j].l,Seg_Medias.min[3],Seg_Medias.max[3]);
                           Seg_Medias.v[4] := Seg_Medias.v[4] + Normalizar(Segmentos[s].SegNeuronios[i,j].c,Seg_Medias.min[4],Seg_Medias.max[4]);

                           //Angulos do Mapa
                           Seg_Medias.v[5] := Seg_Medias.v[5] + Normalizar(Segmentos[s].SegNeuronios[i,j].ang[1],Seg_Medias.min[5],Seg_Medias.max[5]);
                           Seg_Medias.v[6] := Seg_Medias.v[6] + Normalizar(Segmentos[s].SegNeuronios[i,j].ang[2],Seg_Medias.min[6],Seg_Medias.max[6]);
                           Seg_Medias.v[7] := Seg_Medias.v[7] + Normalizar(Segmentos[s].SegNeuronios[i,j].ang[3],Seg_Medias.min[7],Seg_Medias.max[7]);
                           Seg_Medias.v[8] := Seg_Medias.v[8] + Normalizar(Segmentos[s].SegNeuronios[i,j].ang[4],Seg_Medias.min[8],Seg_Medias.max[8]);

                           //Rede Neural Objeto ConhecidoConhecido 0 ou 1
                           Seg_Medias.v[9] := Seg_Medias.v[9] + Normalizar(trunc(Segmentos[s].SegNeuronios[i,j].rn),Seg_Medias.min[9],Seg_Medias.max[9]);

                       end; // if

               // Tamanho do Segmento
               if s = 0 then
               begin
                  Seg_Medias.max[10] := neuronios_pulsantes;
                  Seg_Medias.min[10] := neuronios_pulsantes;
               end else
               begin
                  if neuronios_pulsantes > Seg_Medias.max[10] then
                     Seg_Medias.max[10] := neuronios_pulsantes;
                  if neuronios_pulsantes < Seg_Medias.min[10] then
                     Seg_Medias.min[10] := neuronios_pulsantes;
               end;

               // Calculo das Médias de Saliencia de cada Atributo
               // Intensidade
               Segmentos[s].SegMedias[0] := (Seg_Medias.v[0]/neuronios_pulsantes);

               // Cor RG e BY
               Segmentos[s].SegMedias[1] := (Seg_Medias.v[1]/neuronios_pulsantes);

               // Orientação
               Segmentos[s].SegMedias[2] := (Seg_Medias.v[2]/neuronios_pulsantes);

               // Calculo X e Y
               Segmentos[s].SegMedias[3] := (Seg_Medias.v[3]/neuronios_pulsantes);
               Segmentos[s].SegMedias[4] := (Seg_Medias.v[4]/neuronios_pulsantes);
               //.. colocando Normalizando
                    //Segmentos[s].SegMedias[3] := trunc((Segmentos[s].SegMedias[3]*255)/knl);
                    //Segmentos[s].SegMedias[4] := trunc((Segmentos[s].SegMedias[4]*255)/knc);

               //Rede Neural Objeto ConhecidoConhecido
               Segmentos[s].SegMedias[9] := (Seg_Medias.v[9]/neuronios_pulsantes);

               // Size
               Segmentos[s].SegMedias[10] := (neuronios_pulsantes);

               (*
               // Teste pegando os máximos pontos de saliência-------
               // Calculo das Médias de Saliencia de cada Atributo
               // Intensidade
               Segmentos[s].SegMedias[0] := Normalizar(Seg_Medias.max[0],Seg_Medias.min[0],Seg_Medias.max[0]);

               // Cor RG e BY
               Segmentos[s].SegMedias[1] := Normalizar(Seg_Medias.max[1],Seg_Medias.min[1],Seg_Medias.max[1]);

               // Orientação
               Segmentos[s].SegMedias[2] := Normalizar(Seg_Medias.max[2],Seg_Medias.min[2],Seg_Medias.max[2]);

               // Calculo X e Y
               Segmentos[s].SegMedias[3] := Normalizar(Seg_Medias.max[3],Seg_Medias.min[3],Seg_Medias.max[3]);
               Segmentos[s].SegMedias[4] := Normalizar(Seg_Medias.max[4],Seg_Medias.min[4],Seg_Medias.max[4]);
               //.. colocando Normalizando
                    //Segmentos[s].SegMedias[3] := trunc((Segmentos[s].SegMedias[3]*255)/knl);
                    //Segmentos[s].SegMedias[4] := trunc((Segmentos[s].SegMedias[4]*255)/knc);

               //Rede Neural Objeto ConhecidoConhecido
               Segmentos[s].SegMedias[9] := Normalizar(Seg_Medias.max[9],Seg_Medias.min[9],Seg_Medias.max[9]);

               // Size
               Segmentos[s].SegMedias[10] := (neuronios_pulsantes);
               //--------------------------------------------------
               *)

               // Tornando Segmento Válido de acordo com Média
               Segmentos[s].segValido := false;
               if ((Segmentos[s].SegMedias[0] + Segmentos[s].SegMedias[1] +
                  Segmentos[s].SegMedias[2] + Segmentos[s].SegMedias[9])/4) >= strtofloat(mskThetas.Text) then //> 0.03 then // INSERIR PARÂMETRO...!?!?!?!
                  //if (Segmentos[s].Sknl*Segmentos[s].Sknc) < (knl*knc*LimiarFundoSeg) then
                      Segmentos[s].segValido := true;

               // Zerando Valors
               Segmentos[s].IeD.i := 0;
               Segmentos[s].IeD.v := 0;
               Segmentos[s].IeD.y := 0;
               Segmentos[s].IeD.rn := 0;
               Segmentos[s].v_Max := 0;
               Segmentos[s].Classify := 0;

               //exit;
               //BitBtn36.OnClick(Sender);

            end;

            // Normalizando o Tamanho do Segmento
            for s := 0 to seg_validos-1 do
            begin
               Segmentos[s].SegMedias[10] := Normalizar(trunc(Segmentos[s].SegMedias[10]),Seg_Medias.min[10],Seg_Medias.max[10]);
            end;

            BitBtn36.OnClick(Sender);

            setlength(Segmentos_Pulso,neuronios_pulsantes);

         end;

         //Memo1.Lines.Append(inttostr(neuronios_pulsantes));
         // Treinando pelos segmentos...----------------------------------------
         //for s := 0 to neuronios_pulsantes-1 do

         CBSeg.OnClick(Sender);
         Exit;// alterado 2013_16_04


         begin
             Application.ProcessMessages;
             if timer6.Enabled = false then exit;

             //repeat
             //randomize;
             if n_seg > 1 then
             begin
                repeat
                   s := randomrange(0,length(Segmentos));
                until (Segmentos[s].segValido = true) and (s <> seg_UP);
             end else
             begin
                repeat
                   s := randomrange(0,length(Segmentos));
                until (Segmentos[s].segValido = true);
             end;
             seg_UP := s;
             //som_i := Segmentos_Pulso[s].l;
             //som_j := Segmentos_Pulso[s].c;
             //Kohonen;
            //until Segmentos[s].segValido = true;

         end;


         timer7.Enabled := true;

         //for s := 0 to length(Segmentos)-1 do
         (*
         for s := 0 to seg_validos-1 do
            for i := 0 to Segmentos[s].Sknl-1 do
               for j := 0 to Segmentos[s].Sknc-1 do
               begin
                  Application.ProcessMessages;
                  if timer6.Enabled = false then exit;
                  if (Segmentos[s].SegNeuronios[i,j].Lx[1] >= thetae) and
                     (Segmentos[s].SegNeuronios[i,j].d = 1) then
                  begin  // Neuronio a ser treinado no SOM
                     som_i := Segmentos[s].SegNeuronios[i,j].l;
                     som_j := Segmentos[s].SegNeuronios[i,j].c;
                     Kohonen;
                  end;
               end;
         *)

           //BitBtn2.OnClick(Sender);


            {
            s := randomrange(0,length(Segmentos));
            i := randomrange(0,Segmentos[s].Sknl-1);
            j := randomrange(0,Segmentos[s].Sknc-1);

            if (Segmentos[s].SegNeuronios[i,j].Lx[1] >= thetae) and
               (Segmentos[s].SegNeuronios[i,j].d = 1) then
            begin  // Neuronio a ser treinado no SOM
               som_i := Segmentos[s].SegNeuronios[i,j].l;
               som_j := Segmentos[s].SegNeuronios[i,j].c;
               Kohonen;
            end;
            }

         //BitBtn36.OnClick(Sender);
         //PageControl13.TabIndex := 9;
         // fim dos segmentos-------------------------------------------------

     end;

  end; // enquanto true

  FinalizarContagem;

end;

procedure TFormProjeto.CarregaMLP;
var i: integer;
begin
   try

   MLP.SetPrepared(false);

   nl := 30;//DBImage1.Picture.Bitmap.Height;
   nc := 30;//DBImage1.Picture.Bitmap.Width;

   { Arquitetura da rede MLP }
   MLP.Struct.Clear;
   MLP.Struct.Add(inttostr(nl*nc));//169
   MLP.Struct.Add(inttostr(snClasses.value));

   { Parâmetros de Treinamento }
   MLP.Learning := seAprendizagem.Value / 100;
   MLP.Momentum := seInercia.Value / 100;

   MLP.Knowledge := ExtractFilePath(Application.ExeName)+'Imagens Treinamento\'+ edArquivo.text;

   { Constrói a rede }
   MLP.Build;

   { Ajusta intervalo das entradas }
   for i := 1 to (nl*nc) do
   begin
      with MLP do
      begin
         SetInputMin(i,0);
         SetInputMax(i,1);
      end;
   end;

   { Ajusta intervalo das saídas }
   for i := 1 to snClasses.Value do
   begin
      with MLP do
      begin
         SetOutputMin(i,0);
         SetOutputMax(i,1);
      end;
   end;
      MLP.Load;
   except
      Application.MessageBox('Clique em [Construir Rede MLP]','Atenção',mb_iconExclamation);
   end;
end;


procedure TFormProjeto.SegmentationLEGION(Sender: TObject);
var
   //Variáveis Locais Gerais
     Bitmap : TBitmap;
     iBMPs, iBMPe24 : PRGB;
     iBMPe8 : PByteArray;
     pixelI : double;
     i,j,a,b,s,t,c,i_,j_ : integer;
     knl_t, knc_t : integer;

   // Variáveis Locais para a Suavização Mediana
     LR,LG,LB : TLista;

   // Vetor de Pesos para Angulos de Direções
     wa : array[0..2,0..2] of integer;
     maxi : double;

   // Variáveis Locais para o LEGION
     //first_pulse,
     sincronizado, seg_ordem, segmentado : boolean;
     checksum, checksum_temp : double;
     n_seg, count, sleft, stop, seg_Validos : integer;
     temp, temp2, temp3  : double;
     neuronios_pulsantes : integer;

     //Segmentos : array of TSeg;
     Seg_TMP : TSeg;
     Segmentos_Pulso : array of record
                        l,c : integer;
                       end;

     //SeG_Imagem : array of TImage;
     //SeG_TMP_Imagem : TImage;
     ScrollBox : TScrollBox;


   // MLP
     IMaior, n: integer;
     Valor, Maior: double;

     ttt : double;

     Seg_Medias : TVetorAtributos_ReEscalar;

   // Grafico
   aSurfaceSeries : TSurfaceSeries;


begin

   // Limpando o Gráfico
   CSal.BottomAxis.Minimum := 0;
   CSal.BottomAxis.Maximum := knc;

   // Criando dinamicamente
   aSurfaceSeries := TSurfaceSeries.Create(Self);
   with aSurfaceSeries do
   begin
      NumXValues := knc;
      NumZValues := knl;
      Pen.Visible := true;
      Pen.Color := clblack;
      TimesZOrder := trunc(knl*0.11);//20;
      for i := 1 to knl do
         for j := 1 to knc do
             AddXYZ(j,0,i,'',clwhite);
   end;
   CSal.Series[0].Destroy;
   CSal.AddSeries(TSurfaceSeries(aSurfaceSeries));

   CO.Series[0].Clear;

   for i := 0 to 29 do
      COII.Series[i].Clear;

   cont_it := 0;

   //PageControl13.TabIndex := 5;
   if cbExp4_MP.Checked = false then CBBorda.Checked := false;

  //LEGION----------------------------------------------------------------------
    CBSeg.OnClick(Sender);
    CBSeg.Checked := false;
    Img_Segmentos.Picture := nil;

  //Enviesamento top-down LEGION
    Wc := strtofloat(mskWc.text);
    Wi := strtofloat(mskWi.text);
    Wo := strtofloat(mskWo.text);
    Wsum := strtofloat(mskWsum.text);

  //LOCAL
     t_geral := 0;
     fat_dec := 0.9;
     NIt := (knl-1)*(knc-1);
     dt_Sal := strtofloat(mskDt_Sal.Text);
     wInib := strtofloat(mskWInib.Text);

  // Visualização do Mapa SOM
     BitmapK := TBitmap.Create;
     BitmapK.PixelFormat := pf24bit;
     BitmapK.Width := knc;
     BitmapK.Height := knl;

  // Mapa de Atributo Saliente Atributo Saliencia
     BitmapAS := TBitmap.Create;
     BitmapAS.PixelFormat := pf24bit;
     BitmapAS.Width := knc;
     BitmapAS.Height := knl;
     maxl := -1; maxc := -1; max_seg := -1;
     saliency := 0;
     saliency_max :=0;

  // GLOBAL: Visualização da Imagem Original Segmentada
     BitmapS := TBitmap.Create;
     BitmapS.PixelFormat := pf24bit;
     BitmapS.Width := knc;
     BitmapS.Height := knl;
     Segmentos := nil;
     Seg_Imagem := nil;

  // GLOBAL: Carregando base de treinamento da MLP
     CarregaMLP;
     Treina_SOM := false;

  // LOCAL: Limpando a Imagem em que será Destacado o Objeto Saliente
  Bitmap := TBitmap.Create;
  try
     Bitmap.PixelFormat := pf24bit;
     Bitmap.Width := knc;
     Bitmap.Height := knl;
     for i := 0 to knl-1 do
     begin
       iBMPs := Bitmap.ScanLine[i];
       for j := 0 to knc-1 do
       begin
         iBMPs[j].r := 0;
         iBMPs[j].g := 0;
         iBMPs[j].b := 0;
       end;
     end;
     // Mostrando Imagem
     mod_ImgExp4_IeF.Picture.Graphic := Bitmap;
  finally
     Bitmap.Free;
  end;

  // LOCAL: Limpando a Imagem SOM e SOM SAL
  Bitmap := TBitmap.Create;
  try
     Bitmap.PixelFormat := pf24bit;
     Bitmap.Width := knc;
     Bitmap.Height := knl;
     for i := 0 to knl-1 do
     begin
       iBMPs := Bitmap.ScanLine[i];
       for j := 0 to knc-1 do
       begin
         iBMPs[j].r := 0;
         iBMPs[j].g := 0;
         iBMPs[j].b := 0;
       end;
     end;
     // Mostrando Imagem
     mod_ImgExp4_SOM_SAL.Picture.Graphic := Bitmap;
  finally
     Bitmap.Free;
  end;

  // Inicializando neuronios do SOM
  randomize;
  for i := 0 to knl-1 do
  begin
     for j := 0 to knc-1 do
     begin
         //SOM Intensidades, RG e BY
            if cbExp4_Color.Checked then
            begin
               KNeuronios[i,j][0] := randomrange(0,255);
               KNeuronios[i,j][1] := randomrange(0,255);
               KNeuronios[i,j][2] := randomrange(0,255);
            end else begin
               KNeuronios[i,j][0] := randomrange(0,255);
               KNeuronios[i,j][1] := KNeuronios[i,j][0];
               KNeuronios[i,j][2] := KNeuronios[i,j][0];
            end;
         //SOM Posições Iniciais do Mapa
            KNeuronios[i,j][3] := randomrange(0,knl-1);
            KNeuronios[i,j][4] := randomrange(0,knc-1);
         //SOM Angulos Iniciais do Mapa
            KNeuronios[i,j][5] := randomrange(0,255);
            KNeuronios[i,j][6] := randomrange(0,255);
            KNeuronios[i,j][7] := randomrange(0,255);
            KNeuronios[i,j][8] := randomrange(0,255);

          //SOM Reconhecimento
            KNeuronios[i,j][9] := 0;//randomrange(0,255);

         //SOM R,G,B
            KNeuronios[i,j][10] := randomrange(0,255);
            KNeuronios[i,j][11] := randomrange(0,255);
            KNeuronios[i,j][12] := randomrange(0,255);

         //Mapa de Atributo Saliencia Neuronios
            //SNeuronios[i,j].i := 0;
            //SNeuronios[i,j].v := 0;
            //SNeuronios[i,j].y := 0;
            //SNeuronios[i,j].rn := 0;

        end;
   end;

  // Parâmetros do LEGION
  // Randomizando os valores iniciais...
  tmp := 0;

  // Inicializando neuronios LEGION
  for i:=0 to knl-1 do
    for j:=0 to knc-1 do
    begin

      neuronios[i,j].matrix := 0;
      neuronios[i,j].ly := rnd();
      neuronios[i,j].lx[0] := neuronios[i,j].ly / (MAXVAL+1);
      neuronios[i,j].ly := neuronios[i,j].ly - 2;
      if (neuronios[i,j].ly < 0) then neuronios[i,j].ly := 0;
      neuronios[i,j].trigger := 0;
      neuronios[i,j].cycle := 0;

      neuronios[i,j].d := 0;
      Neuronios[i,j].stim := neuronios[i,j].rgb[4];
    end;
  z := 0;

  // Verificando Conexões
  connections;

  // evaluate...
  jump[0] := 0;
  select();

  first_pulse := false;
  sincronizado := false;
  checksum := 0;
  n_seg := 0;
  seg_Ordem := false;
  temp :=0; temp2:=0; temp3:=0.0;
  count := 0;
  n := 0;

  timer.Enabled := true;
  while (timer.Enabled = true) do
  begin
     //Application.ProcessMessages;
     inc(tmp);
     count := 0;

     if (timer6.Enabled = false) then
     begin

     // calculando entradas da rede input
     for i:=0 to knl-1 do
       for j:=0 to knc-1 do
       begin

				  if (neuronios[i,j].lx[0] > thetae) and (jump[0] = 0) then
          begin  //* jumping down */
    					neuronios[i,j].lx[1] := LP;
    					neuronios[i,j].ly := -1;
    					neuronios[i,j].cycle := 1;
    					inc(count);
				  end else
              if (neuronios[i,j].lx[0] < thetae) then
              begin     //* summing up overall input */
                 temp :=  -Wz * bi(z,thetaz);  //* used to be 50 */

        				 //* the following is based on traversing through strongest link */
      					 temp2 := 0.0;
       			  	 if (j-1 >= 0) then
                 begin
						        temp3 := neuronios[i,j].W[0] * bi(neuronios[i,j-1].lx[0], thetae);
      					  	if (temp2 < temp3) then temp2 := temp3;
                 end;

				         if (i-1 >= 0) and (j-1 >= 0) then
                 begin
        						temp3 := neuronios[i,j].W[1] * bi(neuronios[i-1,j-1].lx[0], thetae);
      					    if (temp2 < temp3) then temp2 := temp3;
                 end;

        				 if (i-1 >= 0) then
                 begin
        						temp3 := neuronios[i,j].W[2] * bi(neuronios[i-1,j].lx[0], thetae);
        						if (temp2 < temp3) then temp2 := temp3;
                 end;

        				 if (i-1 >= 0) and (j+1 < knc) then
                 begin
        						temp3 := neuronios[i,j].W[3] * bi(neuronios[i-1,j+1].lx[0], thetae);
        						if (temp2 < temp3) then temp2 := temp3;
                 end;

        				 if (j+1 < knc) then
                 begin
        						temp3 := neuronios[i,j].W[4] * bi(neuronios[i,j+1].lx[0], thetae);
        						if (temp2 < temp3) then temp2 := temp3;
                 end;

        				 if (i+1 < knl) and (j+1 < knc) then
                 begin
        						temp3 := neuronios[i,j].W[5] * bi(neuronios[i+1,j+1].lx[0], thetae);
        						if (temp2 < temp3) then temp2 := temp3;
                 end;

        				 if (i+1 < knl) then
                 begin
        						temp3 := neuronios[i,j].W[6] * bi(neuronios[i+1,j].lx[0], thetae);
        						if (temp2 < temp3) then temp2 := temp3;
                 end;

        				 if (i+1 < knl) and (j-1 >= 0) then
                 begin
        						temp3 := neuronios[i,j].W[7] * bi(neuronios[i+1,j-1].lx[0], thetae);
        						if (temp2 < temp3) then temp2 := temp3;
                 end;

        					temp := temp + temp2;

        					if (neuronios[i,j].trigger = 1) or (temp > 0.25) then
                  begin               //* jumping up */
          						neuronios[i,j].lx[1] := RP;
             					neuronios[i,j].ly := MAXIMO;
            					jump[1] := 1;
            					inc(z);
                  end else
        					  	neuronios[i,j].Lx[1] := neuronios[i,j].Lx[0];
              end else
              begin
   					  	 neuronios[i,j].Lx[1] := neuronios[i,j].Lx[0];
              end;

    	 end; // for

     z := z - count;

  	 if (jump[1] = 0) and (z = 0) then
      	select();

     // Apresentando...

     // Mostra neuronios sincronizados...
     if (z > 0) then
     begin
        segmentado := true;
        for i := 0 to knl-1 do
           for j := 0 to knc-1 do
               if (Neuronios[i,j].d = 0) and (Neuronios[i,j].Lx[1] >= thetae) then
               begin
                   segmentado := false;
                   continue;
               end;

        //if cbVer_Sincro.Checked then
        //   ShowSincro_LEGION
        //else
        //begin
           ShowSincro_LEGION_no_Preview;
           if segmentado = true then
              ShowSincro_LEGION;
        //end;

        // Gravando Segmento
        checksum_temp := 0;

        if (segmentado = true) and (n_seg = 1) then
           checksum_temp := checksum;

        if segmentado = true then
        begin

           for i := imin+1 to imax-1 do
              for j := jmin+1 to jmax-1 do           //( testando       )
                  if (Neuronios[i,j].lx[1] >= thetae) and (Neuronios[i,j].d = 1) then
                     if first_pulse = false then
                     begin
                        checksum := checksum + (-checksum + (i+j)*0.1);
                        n_seg := 0;
                     end
                     else
                        checksum_temp := checksum_temp + (-checksum_temp + (i+j)*0.1);

           first_pulse := true;

           // Verificando se Sincronizou
           if (checksum = checksum_temp) then
           begin
              checksum := 0; first_pulse := false; n_seg := 0;
              sincronizado := true;

              Sleft := 3; Stop := 3;

              setlength(Seg_Imagem,length(Segmentos));

              Scrollbox := TScrollBox.Create(FormProjeto);
              Scrollbox.Parent := Tabsheet13;
              Scrollbox.Align := alClient;

              for s := 0 to length(Segmentos)-1 do
              begin

                 Seg_Imagem[s] := TImage.Create(FormProjeto);
                 Seg_Imagem[s].Parent := ScrollBox;
                 Seg_Imagem[s].Height := 180;
                 Seg_Imagem[s].Width := 180;
                 Seg_Imagem[s].AutoSize := false;
                 Seg_Imagem[s].Center := true;
                 Seg_Imagem[s].Proportional := true;
                 Seg_Imagem[s].Left := sleft;
                 Seg_Imagem[s].Top := stop;
                 Seg_Imagem[s].OnClick := Zoom;
                 sleft := sleft + 6 + Seg_Imagem[s].Width;
                 if sleft > 700 then
                 begin
                    sleft := 3;
                    stop := stop + Seg_Imagem[s].Height + 6;
                 end;

                 Bitmap := TBitmap.Create;
                 try

                    Bitmap.PixelFormat := pf24bit;
                    Bitmap.Width := Segmentos[s].Sknc;
                    Bitmap.Height := Segmentos[s].Sknl;
                    for i := 0 to Segmentos[s].Sknl-1 do
                    begin
                       iBMPs := Bitmap.ScanLine[i];
                       for j := 0 to Segmentos[s].Sknc-1 do
                       begin

                          //----------------------------------------------------
                          if (Segmentos[s].SegNeuronios[i,j].Lx[1] >= thetae) and
                             (Segmentos[s].SegNeuronios[i,j].d = 1) then
                          begin

                             if cbExp4_Cinza.Checked then
                             begin
                                iBMPs[j].r := Segmentos[s].SegNeuronios[i,j].rgb[4];
                                iBMPs[j].g := Segmentos[s].SegNeuronios[i,j].rgb[4];
                                iBMPs[j].b := Segmentos[s].SegNeuronios[i,j].rgb[4];
                             end
                             else
                                if cbExp4_Cores.Checked then
                                begin
                                    iBMPs[j].r := trunc(Segmentos[s].SegNeuronios[i,j].Seg_rgb[0]);
                                    iBMPs[j].g := trunc(Segmentos[s].SegNeuronios[i,j].Seg_rgb[1]);
                                    iBMPs[j].b := trunc(Segmentos[s].SegNeuronios[i,j].Seg_rgb[2]);
                                end
                                else
                                begin
                                    iBMPs[j].r := trunc(Segmentos[s].SegNeuronios[i,j].lx[1]*255);
                                    iBMPs[j].g := trunc(Segmentos[s].SegNeuronios[i,j].lx[1]*255);
                                    iBMPs[j].b := trunc(Segmentos[s].SegNeuronios[i,j].lx[1]*255);
                                end;
                          end
                          else begin
                             //if (cbExp4_MP.Checked) then
                             begin
                                iBMPs[j].r := 0;
                                iBMPs[j].g := 0;
                                iBMPs[j].b := 0;
                             end;
                          end;
                          //------------------------------------------------------------------------------

                          //MLP.SetInput(i*13+j+1,Bitmap.Canvas.Pixels[i,j] shr 23); // 0 ou 1

                       end;
                    end;
                    // Mostrando Imagem
                    Seg_Imagem[s].Picture.Graphic := Bitmap;
                    //sleep(2000);
                 finally
                    Bitmap.Free;
                 end;

                 // Gerando segmentos redimensionados para a classificação
                 //nc := 30;
                 //nl := 30;
                 Bitmap := TBitmap.Create;
                 try
                    Bitmap.PixelFormat := pf24bit;
                    Bitmap.Width := nc;
                    Bitmap.Height := nl;
                    redimensionar_Image(Seg_Imagem[s],Bitmap,Segmentos[s].Sknl,Segmentos[s].Sknc,nl,nc);
                    Bitmap.SaveToFile('SEGMENTO_RED_'+inttostr(s)+'.bmp');
                    for i := 0 to nc do
                       for j := 0 to nl do
                       begin
                          n := i*nc+j+1; // índice do neurônio de entrada
                          MLP.SetInput(n,Bitmap.Canvas.Pixels[j,i] shr 23); // 0 ou 1
                       end;
                    MLP.Test;
                 finally
                    Bitmap.Free;
                 end;

                 begin

                 mod_ed0.Text := Format('%2.4f',[MLP.GetOutput(1)]);
                 mod_ed1.Text := Format('%2.4f',[MLP.GetOutput(2)]);
                 mod_ed2.Text := Format('%2.4f',[MLP.GetOutput(3)]);
                 mod_ed3.Text := Format('%2.4f',[MLP.GetOutput(4)]);
                 mod_ed4.Text := Format('%2.4f',[MLP.GetOutput(5)]);
                 mod_ed5.Text := Format('%2.4f',[MLP.GetOutput(6)]);
                 mod_ed6.Text := Format('%2.4f',[MLP.GetOutput(7)]);
                 mod_ed7.Text := Format('%2.4f',[MLP.GetOutput(8)]);
                 mod_ed8.Text := Format('%2.4f',[MLP.GetOutput(9)]);
                 mod_ed9.Text := Format('%2.4f',[MLP.GetOutput(10)]);

                 { Verifica a maior saída para indicação visual }
                 Maior := -10;
                 for i := 1 to snClasses.Value do
                 begin
                     Valor := MLP.GetOutput(i);
                     if Valor > Maior then
                     begin
                        Maior := Valor;
                        IMaior := i;
                     end;
                 end;

                 { Todas com fundo branco }
                 mod_sp0.Brush.Color := clGray;
                 mod_sp1.Brush.Color := clGray;
                 mod_sp2.Brush.Color := clGray;
                 mod_sp3.Brush.Color := clGray;
                 mod_sp4.Brush.Color := clGray;
                 mod_sp5.Brush.Color := clGray;
                 mod_sp6.Brush.Color := clGray;
                 mod_sp7.Brush.Color := clGray;
                 mod_sp8.Brush.Color := clGray;
                 mod_sp9.Brush.Color := clGray;

                 { Se semelhança for maior que 0.95}
                 //if (Maior > 0.10) and (Maior < 1.00) then
                 begin
                     { Exceto a maior }
                      case IMaior of
                          1: begin
                               mod_sp0.Brush.Color := $0080FF80;
                               PlaySound(Pchar(ExtractFilePath(Application.ExeName)+'\Sons\estrela.wav'), 1, SND_ASYNC);
                             end;
                          2: begin
                               mod_sp1.Brush.Color := $0080FF80;
                               PlaySound(Pchar(ExtractFilePath(Application.ExeName)+'\Sons\quadrado.wav'), 1, SND_ASYNC);
                             end;
                          3: begin
                               mod_sp2.Brush.Color := $0080FF80;
                               PlaySound(Pchar(ExtractFilePath(Application.ExeName)+'\Sons\triangulo.wav'), 1, SND_ASYNC);
                             end;
                          4: begin
                               mod_sp3.Brush.Color := $0080FF80;
                               PlaySound(Pchar(ExtractFilePath(Application.ExeName)+'\Sons\circulo.wav'), 1, SND_ASYNC);
                             end;
                          5: begin
                               mod_sp4.Brush.Color := $0080FF80;
                               PlaySound(Pchar(ExtractFilePath(Application.ExeName)+'\Sons\um.wav'), 1, SND_ASYNC);
                             end;
                          6: begin
                               mod_sp5.Brush.Color := $0080FF80;
                               PlaySound(Pchar(ExtractFilePath(Application.ExeName)+'\Sons\dois.wav'), 1, SND_ASYNC);
                             end;
                          7: begin
                               mod_sp6.Brush.Color := $0080FF80;
                               PlaySound(Pchar(ExtractFilePath(Application.ExeName)+'\Sons\tres.wav'), 1, SND_ASYNC);
                             end;
                          8: begin
                               mod_sp7.Brush.Color := $0080FF80;
                               PlaySound(Pchar(ExtractFilePath(Application.ExeName)+'\Sons\a.wav'), 1, SND_ASYNC);
                             end;
                          9: begin
                               mod_sp8.Brush.Color := $0080FF80;
                               PlaySound(Pchar(ExtractFilePath(Application.ExeName)+'\Sons\cruz.wav'), 1, SND_ASYNC);
                             end;
                         10: begin
                               mod_sp9.Brush.Color := $0080FF80;
                               PlaySound(Pchar(ExtractFilePath(Application.ExeName)+'\Sons\c.wav'), 1, SND_ASYNC);
                             end;
                      end;

                     { Alimentando neurônios reconhecidos pela MLP}
                      for i := 0 to Segmentos[s].Sknl-1 do
                           for j := 0 to Segmentos[s].Sknc-1 do
                               if (Segmentos[s].SegNeuronios[i,j].Lx[1] >= thetae) and
                                  (Segmentos[s].SegNeuronios[i,j].d = 1) then
                               begin
                                   Neuronios[i+Segmentos[s].imin,j+Segmentos[s].jmin].rn := 255;
                                   Segmentos[s].SegNeuronios[i,j].rn := 255;
                               end;


                 end; // se for de tamanho 13
                 end;

                 { Alimentando neurônios reconhecidos pela MLP}
                   for i := 0 to Segmentos[s].Sknl-1 do
                        for j := 0 to Segmentos[s].Sknc-1 do
                            if (Segmentos[s].SegNeuronios[i,j].Lx[1] >= thetae) and
                               (Segmentos[s].SegNeuronios[i,j].d = 1) then
                            begin
                                Segmentos[s].SegNeuronios[i,j].rn :=
                                    Neuronios[i+Segmentos[s].imin,j+Segmentos[s].jmin].rn;
                            end;

              end;//-------------


              ///////////////////
              //if cbExp4_MP.Checked = false then
                 for i := 0 to knl-1 do
                    for j := 0 to knc-1 do
                    begin
                       iBMPs := mod_ImgExp4_IeF.Picture.Bitmap.ScanLine[i];
                       if (iBMPs[j].r = 255) and (iBMPs[j].g = 255) and
                          (iBMPs[j].b = 255) then
                       begin
                          iBMPs[j].r := 0; iBMPs[j].g := 0; iBMPs[j].b := 0;
                       end;
                       Neuronios[i,j].rgb[0] := iBMPs[j].r;
                       Neuronios[i,j].rgb[1] := iBMPs[j].g;
                       Neuronios[i,j].rgb[2] := iBMPs[j].b;
                    end;

                 if CBSegTC.Checked then
                 begin
                    FormSeg.mod_ImgExp4.Picture.Graphic := mod_ImgExp4_IeF.Picture.Graphic;
                    if FormSeg.Showing = false then FormSeg.Show;
                 end;

              ///////////////////

              if cbVer_Just_S.checked = true then
              begin
                 //Atualiza_para_SOM;

                 BitBtn2.OnClick(Sender);
              end
              else begin
              // Saindo da Sincronização ... e habilitando o Kohonem

                 //Atualiza_para_SOM;

                 //PageControl13.TabIndex := 8;
                 //BitBtn35.OnClick(Sender); // exit;
                 timer6.Enabled := true;
                 //PageControl13.TabIndex := 8;

                 //BitBtn36.OnClick(Sender);
                 //PageControl13.TabIndex := 10;
                         // exit;

              end;

           end
           else // grava o segmento
           begin

              setlength(Segmentos,n_seg+1); setlength(Seg_Classify,n_seg+1);
              setlength(Segmentos[n_seg].SegNeuronios,imax-imin+1,jmax-jmin+1);
              Segmentos[n_seg].Sknl := imax-imin+1;
              Segmentos[n_seg].Sknc := jmax-jmin+1;
              Segmentos[n_seg].imin := imin;
              Segmentos[n_seg].imax := imax;
              Segmentos[n_seg].jmin := jmin;
              Segmentos[n_seg].jmax := jmax;

              for i := imin to imax do
                 for j := jmin to jmax do
                 begin
                    if (Neuronios[i,j].Lx[1] >= thetae) and
                       (Neuronios[i,j].d = 1) then
                    begin
                        Segmentos[n_seg].SegNeuronios[i-imin,j-jmin] := Neuronios[i,j];
                        Neuronios[i,j].segN := n_seg;
                        for s := 0 to 12 do Segmentos[n_seg].SegMedias[s] := 0;
                    end;
                 end;

              inc(n_seg);
              Label68.caption := inttostr(n_seg);
           end;

        end;

     end;  // z > 0

     //if (cbExp4_MP.Checked) then
     //sleep(trunc((tbpulso.Position/10)*1000));

     for i:=0 to knl-1 do   //* replace OLD by NEW */
       for j:=0 to knc-1 do
  				neuronios[i,j].lx[0] := neuronios[i,j].lx[1];

		 jump[0] := jump[1];
		 jump[1] := 0;

     end // if timer6 false
//nmnmnmnmnmnmnmnmnmnmnmnmnmnmnmnmnmnmnmnmnmnmnmnmnmnmnmnmnmnmnmnmnmnmnmnmnmnmnm
     else begin


         if seg_Ordem = false then
         begin
            seg_ordem := true;

            // Veficando o número de segmentos válidos pelo tamanho
            seg_validos := 0;
            for s := 0 to length(Segmentos)-1 do
            begin
                Memo1.Lines.Append(formatfloat('0.0',Segmentos[s].Sknl*Segmentos[s].Sknc));
                if (Segmentos[s].Sknl*Segmentos[s].Sknc) < (knl*knc*LimiarFundoSeg) then
                begin
                   inc(seg_validos);
                   Memo1.Lines.Append('OK');
                end else
                begin
                   Memo1.Lines.Append('No');
                end;
            end;

            // Tamanho do Objeto
            Seg_Medias.max[10] := 0;
            Seg_Medias.min[10] := 0;

            // Pegando neuronios pulsantes
            setlength(Segmentos_Pulso,(knl*knc));

               // Zerando Medias
                           Seg_Medias.v[0] := 0;
                           Seg_Medias.max[0] := 0;//Segmentos[s].SegNeuronios[i,j].rgb_Itti[0];
                           Seg_Medias.min[0] := 255;//Segmentos[s].SegNeuronios[i,j].rgb_Itti[0];

                           Seg_Medias.v[1] := 0;
                           Seg_Medias.max[1] := 0;//Segmentos[s].SegNeuronios[i,j].rgb_Itti[1];
                           Seg_Medias.min[1] := 255;//Segmentos[s].SegNeuronios[i,j].rgb_Itti[1];

                           Seg_Medias.v[2] := 0;
                           Seg_Medias.max[2] := 0;//Segmentos[s].SegNeuronios[i,j].rgb_Itti[2];
                           Seg_Medias.min[2] := 255;//Segmentos[s].SegNeuronios[i,j].rgb_Itti[2];

                           Seg_Medias.v[3] := 0;
                           Seg_Medias.max[3] := 0;//Segmentos[s].SegNeuronios[i,j].l;
                           Seg_Medias.min[3] := knl;//Segmentos[s].SegNeuronios[i,j].l;

                           Seg_Medias.v[4] := 0;
                           Seg_Medias.max[4] := 0;//Segmentos[s].SegNeuronios[i,j].c;
                           Seg_Medias.min[4] := knc;//Segmentos[s].SegNeuronios[i,j].c;

                           Seg_Medias.v[5] := 0;
                           Seg_Medias.max[5] := 0;//Segmentos[s].SegNeuronios[i,j].ang[1];
                           Seg_Medias.min[5] := 255;//Segmentos[s].SegNeuronios[i,j].ang[1];

                           Seg_Medias.v[6] := 0;
                           Seg_Medias.max[6] := 0;//Segmentos[s].SegNeuronios[i,j].ang[2];
                           Seg_Medias.min[6] := 255;//Segmentos[s].SegNeuronios[i,j].ang[2];

                           Seg_Medias.v[7] := 0;
                           Seg_Medias.max[7] := 0;//Segmentos[s].SegNeuronios[i,j].ang[3];
                           Seg_Medias.min[7] := 255;//Segmentos[s].SegNeuronios[i,j].ang[3];

                           Seg_Medias.v[8] := 0;
                           Seg_Medias.max[8] := 0;//Segmentos[s].SegNeuronios[i,j].ang[4];
                           Seg_Medias.min[8] := 255;//Segmentos[s].SegNeuronios[i,j].ang[4];

                           Seg_Medias.v[9] := 0;
                           Seg_Medias.max[9] := 0;//trunc(Segmentos[s].SegNeuronios[i,j].rn);
                           Seg_Medias.min[9] := 255;//trunc(Segmentos[s].SegNeuronios[i,j].rn);

                           //Seg_Medias[10].max := 0;
                           //Seg_Medias[10].min := 0;

            for s := 0 to seg_validos-1 do
            begin

               // Normalização dos Valores [0,1]
               for i := 0 to Segmentos[s].Sknl-1 do
                  for j := 0 to Segmentos[s].Sknc-1 do
                       if (Segmentos[s].SegNeuronios[i,j].Lx[1] >= thetae) and
                          (Segmentos[s].SegNeuronios[i,j].d = 1) then
                       begin
                           // Atribuindo as Características
                           // Re-Escalar [0]
                              if Segmentos[s].SegNeuronios[i,j].rgb_Itti[0] > Seg_Medias.max[0] then
                                 Seg_Medias.max[0] := Segmentos[s].SegNeuronios[i,j].rgb_Itti[0];
                              if Segmentos[s].SegNeuronios[i,j].rgb_Itti[0] < Seg_Medias.min[0] then
                                 Seg_Medias.min[0] := Segmentos[s].SegNeuronios[i,j].rgb_Itti[0];
                           // Re-Escalar [1]
                              if Segmentos[s].SegNeuronios[i,j].rgb_Itti[1] > Seg_Medias.max[1] then
                                 Seg_Medias.max[1] := Segmentos[s].SegNeuronios[i,j].rgb_Itti[1];
                              if Segmentos[s].SegNeuronios[i,j].rgb_Itti[1] < Seg_Medias.min[1] then
                                 Seg_Medias.min[1] := Segmentos[s].SegNeuronios[i,j].rgb_Itti[1];
                           // Re-Escalar [2]
                              if Segmentos[s].SegNeuronios[i,j].rgb_Itti[2] > Seg_Medias.max[2] then
                                 Seg_Medias.max[2] := Segmentos[s].SegNeuronios[i,j].rgb_Itti[2];
                              if Segmentos[s].SegNeuronios[i,j].rgb_Itti[2] < Seg_Medias.min[2] then
                                 Seg_Medias.min[2] := Segmentos[s].SegNeuronios[i,j].rgb_Itti[2];

                           // X Y
                           // Re-Escalar [3]
                              if Segmentos[s].SegNeuronios[i,j].l > Seg_Medias.max[3] then
                                 Seg_Medias.max[3] := Segmentos[s].SegNeuronios[i,j].l;
                              if Segmentos[s].SegNeuronios[i,j].l < Seg_Medias.min[3] then
                                 Seg_Medias.min[3] := Segmentos[s].SegNeuronios[i,j].l;
                           // Re-Escalar [4]
                              if Segmentos[s].SegNeuronios[i,j].c > Seg_Medias.max[4] then
                                 Seg_Medias.max[4] := Segmentos[s].SegNeuronios[i,j].c;
                              if Segmentos[s].SegNeuronios[i,j].c < Seg_Medias.min[4] then
                                 Seg_Medias.min[4] := Segmentos[s].SegNeuronios[i,j].c;

                           //Angulos do Mapa
                           // Re-Escalar [5]
                              if Segmentos[s].SegNeuronios[i,j].ang[1] > Seg_Medias.max[5] then
                                 Seg_Medias.max[5] := Segmentos[s].SegNeuronios[i,j].ang[1];
                              if Segmentos[s].SegNeuronios[i,j].ang[1] < Seg_Medias.min[5] then
                                 Seg_Medias.min[5] := Segmentos[s].SegNeuronios[i,j].ang[1];
                           // Re-Escalar [6]
                              if Segmentos[s].SegNeuronios[i,j].ang[2] > Seg_Medias.max[6] then
                                 Seg_Medias.max[6] := Segmentos[s].SegNeuronios[i,j].ang[2];
                              if Segmentos[s].SegNeuronios[i,j].ang[2] < Seg_Medias.min[6] then
                                 Seg_Medias.min[6] := Segmentos[s].SegNeuronios[i,j].ang[2];
                           // Re-Escalar [7]
                              if Segmentos[s].SegNeuronios[i,j].ang[3] > Seg_Medias.max[7] then
                                 Seg_Medias.max[7] := Segmentos[s].SegNeuronios[i,j].ang[3];
                              if Segmentos[s].SegNeuronios[i,j].ang[3] < Seg_Medias.min[7] then
                                 Seg_Medias.min[7] := Segmentos[s].SegNeuronios[i,j].ang[3];
                           // Re-Escalar [8]
                              if Segmentos[s].SegNeuronios[i,j].ang[4] > Seg_Medias.max[8] then
                                 Seg_Medias.max[8] := Segmentos[s].SegNeuronios[i,j].ang[4];
                              if Segmentos[s].SegNeuronios[i,j].ang[4] < Seg_Medias.min[8] then
                                 Seg_Medias.min[8] := Segmentos[s].SegNeuronios[i,j].ang[4];

                           //Rede Neural Objeto ConhecidoConhecido 0 ou 1
                           // Re-Escalar [5]
                              if trunc(Segmentos[s].SegNeuronios[i,j].rn) > Seg_Medias.max[9] then
                                 Seg_Medias.max[9] := trunc(Segmentos[s].SegNeuronios[i,j].rn);
                              if trunc(Segmentos[s].SegNeuronios[i,j].rn) < Seg_Medias.min[9] then
                                 Seg_Medias.min[9] := trunc(Segmentos[s].SegNeuronios[i,j].rn);

                       end; // if

            end; // for s


            for s := 0 to seg_validos-1 do
            begin
               neuronios_pulsantes := 0;

               Seg_Medias.v[0] := 0;
               Seg_Medias.v[1] := 0;
               Seg_Medias.v[2] := 0;
               Seg_Medias.v[3] := 0;
               Seg_Medias.v[4] := 0;
               Seg_Medias.v[5] := 0;
               Seg_Medias.v[6] := 0;
               Seg_Medias.v[7] := 0;
               Seg_Medias.v[8] := 0;
               Seg_Medias.v[9] := 0;
               Seg_Medias.v[10] := 0;

               for i := 0 to Segmentos[s].Sknl-1 do
                  for j := 0 to Segmentos[s].Sknc-1 do
                       if (Segmentos[s].SegNeuronios[i,j].Lx[1] >= thetae) and
                          (Segmentos[s].SegNeuronios[i,j].d = 1) then
                       begin
                           Segmentos_Pulso[neuronios_pulsantes].l := Segmentos[s].SegNeuronios[i,j].l;
                           Segmentos_Pulso[neuronios_pulsantes].c := Segmentos[s].SegNeuronios[i,j].c;
                           neuronios_pulsantes := neuronios_pulsantes + 1;

                           // Média de Cadas Segmentos...
                           // Atribuindo as Características
                           Seg_Medias.v[0] := Seg_Medias.v[0] + Normalizar(Segmentos[s].SegNeuronios[i,j].rgb_Itti[0],Seg_Medias.min[0],Seg_Medias.max[0]);
                           Seg_Medias.v[1] := Seg_Medias.v[1] + Normalizar(Segmentos[s].SegNeuronios[i,j].rgb_Itti[1],Seg_Medias.min[1],Seg_Medias.max[1]);
                           Seg_Medias.v[2] := Seg_Medias.v[2] + Normalizar(Segmentos[s].SegNeuronios[i,j].rgb_Itti[2],Seg_Medias.min[2],Seg_Medias.max[2]);

                           // X Y
                           Seg_Medias.v[3] := Seg_Medias.v[3] + Normalizar(Segmentos[s].SegNeuronios[i,j].l,Seg_Medias.min[3],Seg_Medias.max[3]);
                           Seg_Medias.v[4] := Seg_Medias.v[4] + Normalizar(Segmentos[s].SegNeuronios[i,j].c,Seg_Medias.min[4],Seg_Medias.max[4]);

                           //Angulos do Mapa
                           Seg_Medias.v[5] := Seg_Medias.v[5] + Normalizar(Segmentos[s].SegNeuronios[i,j].ang[1],Seg_Medias.min[5],Seg_Medias.max[5]);
                           Seg_Medias.v[6] := Seg_Medias.v[6] + Normalizar(Segmentos[s].SegNeuronios[i,j].ang[2],Seg_Medias.min[6],Seg_Medias.max[6]);
                           Seg_Medias.v[7] := Seg_Medias.v[7] + Normalizar(Segmentos[s].SegNeuronios[i,j].ang[3],Seg_Medias.min[7],Seg_Medias.max[7]);
                           Seg_Medias.v[8] := Seg_Medias.v[8] + Normalizar(Segmentos[s].SegNeuronios[i,j].ang[4],Seg_Medias.min[8],Seg_Medias.max[8]);

                           //Rede Neural Objeto ConhecidoConhecido 0 ou 1
                           Seg_Medias.v[9] := Seg_Medias.v[9] + Normalizar(trunc(Segmentos[s].SegNeuronios[i,j].rn),Seg_Medias.min[9],Seg_Medias.max[9]);

                       end; // if

               // Tamanho do Segmento
               if s = 0 then
               begin
                  Seg_Medias.max[10] := neuronios_pulsantes;
                  Seg_Medias.min[10] := neuronios_pulsantes;
               end else
               begin
                  if neuronios_pulsantes > Seg_Medias.max[10] then
                     Seg_Medias.max[10] := neuronios_pulsantes;
                  if neuronios_pulsantes < Seg_Medias.min[10] then
                     Seg_Medias.min[10] := neuronios_pulsantes;
               end;

               // Calculo das Médias de Saliencia de cada Atributo
               // Intensidade
               Segmentos[s].SegMedias[0] := (Seg_Medias.v[0]/neuronios_pulsantes);

               // Cor RG e BY
               Segmentos[s].SegMedias[1] := (Seg_Medias.v[1]/neuronios_pulsantes);

               // Orientação
               Segmentos[s].SegMedias[2] := (Seg_Medias.v[2]/neuronios_pulsantes);

               // Calculo X e Y
               Segmentos[s].SegMedias[3] := (Seg_Medias.v[3]/neuronios_pulsantes);
               Segmentos[s].SegMedias[4] := (Seg_Medias.v[4]/neuronios_pulsantes);
               //.. colocando Normalizando
                    //Segmentos[s].SegMedias[3] := trunc((Segmentos[s].SegMedias[3]*255)/knl);
                    //Segmentos[s].SegMedias[4] := trunc((Segmentos[s].SegMedias[4]*255)/knc);

               //Rede Neural Objeto ConhecidoConhecido
               Segmentos[s].SegMedias[9] := (Seg_Medias.v[9]/neuronios_pulsantes);

               // Size
               Segmentos[s].SegMedias[10] := (neuronios_pulsantes);

               (*
               // Teste pegando os máximos pontos de saliência-------
               // Calculo das Médias de Saliencia de cada Atributo
               // Intensidade
               Segmentos[s].SegMedias[0] := Normalizar(Seg_Medias.max[0],Seg_Medias.min[0],Seg_Medias.max[0]);

               // Cor RG e BY
               Segmentos[s].SegMedias[1] := Normalizar(Seg_Medias.max[1],Seg_Medias.min[1],Seg_Medias.max[1]);

               // Orientação
               Segmentos[s].SegMedias[2] := Normalizar(Seg_Medias.max[2],Seg_Medias.min[2],Seg_Medias.max[2]);

               // Calculo X e Y
               Segmentos[s].SegMedias[3] := Normalizar(Seg_Medias.max[3],Seg_Medias.min[3],Seg_Medias.max[3]);
               Segmentos[s].SegMedias[4] := Normalizar(Seg_Medias.max[4],Seg_Medias.min[4],Seg_Medias.max[4]);
               //.. colocando Normalizando
                    //Segmentos[s].SegMedias[3] := trunc((Segmentos[s].SegMedias[3]*255)/knl);
                    //Segmentos[s].SegMedias[4] := trunc((Segmentos[s].SegMedias[4]*255)/knc);

               //Rede Neural Objeto ConhecidoConhecido
               Segmentos[s].SegMedias[9] := Normalizar(Seg_Medias.max[9],Seg_Medias.min[9],Seg_Medias.max[9]);

               // Size
               Segmentos[s].SegMedias[10] := (neuronios_pulsantes);
               //--------------------------------------------------
               *)

               // Tornando Segmento Válido de acordo com Média
               Segmentos[s].segValido := false;
               if ((Segmentos[s].SegMedias[0] + Segmentos[s].SegMedias[1] +
                  Segmentos[s].SegMedias[2] + Segmentos[s].SegMedias[9])/4) >= strtofloat(mskThetas.Text) then //> 0.03 then // INSERIR PARÂMETRO...!?!?!?!
                  //if (Segmentos[s].Sknl*Segmentos[s].Sknc) < (knl*knc*LimiarFundoSeg) then
                      Segmentos[s].segValido := true;

               // Zerando Valors
               Segmentos[s].IeD.i := 0;
               Segmentos[s].IeD.v := 0;
               Segmentos[s].IeD.y := 0;
               Segmentos[s].IeD.rn := 0;
               Segmentos[s].v_Max := 0;
               Segmentos[s].Classify := 0;

               //exit;
               //BitBtn36.OnClick(Sender);

            end;

            // Normalizando o Tamanho do Segmento
            for s := 0 to seg_validos-1 do
            begin
               Segmentos[s].SegMedias[10] := Normalizar(trunc(Segmentos[s].SegMedias[10]),Seg_Medias.min[10],Seg_Medias.max[10]);
            end;

            BitBtn36.OnClick(Sender);

            setlength(Segmentos_Pulso,neuronios_pulsantes);

         end;

         //Memo1.Lines.Append(inttostr(neuronios_pulsantes));
         // Treinando pelos segmentos...----------------------------------------
         //for s := 0 to neuronios_pulsantes-1 do

         CBSeg.OnClick(Sender);
         Exit;// alterado 2013_16_04


         begin
             Application.ProcessMessages;
             if timer6.Enabled = false then exit;

             //repeat
             //randomize;
             if n_seg > 1 then
             begin
                repeat
                   s := randomrange(0,length(Segmentos));
                until (Segmentos[s].segValido = true) and (s <> seg_UP);
             end else
             begin
                repeat
                   s := randomrange(0,length(Segmentos));
                until (Segmentos[s].segValido = true);
             end;
             seg_UP := s;
             //som_i := Segmentos_Pulso[s].l;
             //som_j := Segmentos_Pulso[s].c;
             //Kohonen;
            //until Segmentos[s].segValido = true;

         end;


         timer7.Enabled := true;

         //for s := 0 to length(Segmentos)-1 do
         (*
         for s := 0 to seg_validos-1 do
            for i := 0 to Segmentos[s].Sknl-1 do
               for j := 0 to Segmentos[s].Sknc-1 do
               begin
                  Application.ProcessMessages;
                  if timer6.Enabled = false then exit;
                  if (Segmentos[s].SegNeuronios[i,j].Lx[1] >= thetae) and
                     (Segmentos[s].SegNeuronios[i,j].d = 1) then
                  begin  // Neuronio a ser treinado no SOM
                     som_i := Segmentos[s].SegNeuronios[i,j].l;
                     som_j := Segmentos[s].SegNeuronios[i,j].c;
                     Kohonen;
                  end;
               end;
         *)

           //BitBtn2.OnClick(Sender);


            {
            s := randomrange(0,length(Segmentos));
            i := randomrange(0,Segmentos[s].Sknl-1);
            j := randomrange(0,Segmentos[s].Sknc-1);

            if (Segmentos[s].SegNeuronios[i,j].Lx[1] >= thetae) and
               (Segmentos[s].SegNeuronios[i,j].d = 1) then
            begin  // Neuronio a ser treinado no SOM
               som_i := Segmentos[s].SegNeuronios[i,j].l;
               som_j := Segmentos[s].SegNeuronios[i,j].c;
               Kohonen;
            end;
            }

         //BitBtn36.OnClick(Sender);
         //PageControl13.TabIndex := 9;
         // fim dos segmentos-------------------------------------------------

     end;

  end; // enquanto true

  FinalizarContagem;

end;


procedure TFormProjeto.CBAttentionClick(Sender: TObject);
begin
   if CBAttention.Checked then
   begin
      //TimerCam.Enabled := false;
      Timer8.Enabled := true;
   end
   else
      Timer8.Enabled := false; 
end;

procedure TFormProjeto.Timer8Timer(Sender: TObject);
begin
    try
    SegmentationLEGION(Sender);
    except
    end;
end;

procedure TFormProjeto.BitBtn7Click(Sender: TObject);
begin
    SegmentationLEGION(sender);
end;

end.

