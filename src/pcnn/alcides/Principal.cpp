unit Principal;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ColorGrd, ExtCtrls, Menus, ComCtrls, jpeg, Buttons,
  ExtDlgs, TeEngine, Series, BubbleCh, TeeProcs, Chart, Spin, Math;

Const
  n_padrao = 64;
  nL = 8;
  nC = 8;
type

//------------------------------------------------------------------------------
  TMat = Record
            v : real;
            p : byte;
            v_ant : real;
         end;

//------------------------------------------------------------------------------
  TIeD = array[1..nL,1..nC] of TMat;

//------------------------------------------------------------------------------
  TPos = Record
            l,c : integer;
          end;

//------------------------------------------------------------------------------
  TVetorAtributos = array [1..3] of real;

//------------------------------------------------------------------------------
  //--- 769 entradas
  //----  1 para o Neuronio de Entrada
  //--- 768 para o Aprendizado não Supervisionado
  //ArrayNeuronios = array[0..n_padrao] of TVetorAtributos;
  ArrayNeuronios = array[0..nL,1..nC] of TVetorAtributos;


  TFormPrincipal = class(TForm)
    Bevel1: TBevel;
    Memo1: TMemo;
    Label1: TLabel;
    Label2: TLabel;
    Bevel4: TBevel;
    Bevel6: TBevel;
    Panel1: TPanel;
    Barra: TStatusBar;
    RadioGroup1: TRadioGroup;
    OpenDialog1: TOpenDialog;
    SaveDialog1: TSaveDialog;
    Shape1: TShape;
    Image1: TImage;
    Label3: TLabel;
    Label4: TLabel;
    Shape2: TShape;
    Label5: TLabel;
    Shape3: TShape;
    Label6: TLabel;
    Label7: TLabel;
    BitBtn1: TBitBtn;
    Label8: TLabel;
    BitBtn2: TBitBtn;
    BotAprender: TBitBtn;
    Memo2: TMemo;
    Shape4: TShape;
    BotClassificar: TBitBtn;
    Panel2: TPanel;
    MainMenu1: TMainMenu;
    Arquivo1: TMenuItem;
    Abrir1: TMenuItem;
    Salvarcomo1: TMenuItem;
    N1: TMenuItem;
    Sair1: TMenuItem;
    Sair2: TMenuItem;
    OpenPictureDialog1: TOpenPictureDialog;
    BitBtn5: TBitBtn;
    Label9: TLabel;
    SEVizinhos: TSpinEdit;
    procedure FormMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure RadioGroup1Click(Sender: TObject);
    procedure Abrir1Click(Sender: TObject);
    procedure Salvarcomo1Click(Sender: TObject);
    procedure Sair1Click(Sender: TObject);
    procedure Image1MouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure FormDblClick(Sender: TObject);
    procedure BitBtn1Click(Sender: TObject);
    procedure BitBtn2Click(Sender: TObject);
    procedure BotAprenderClick(Sender: TObject);
    procedure Image1DblClick(Sender: TObject);
    procedure BotClassificarClick(Sender: TObject);
    procedure Sair2Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure BitBtn5Click(Sender: TObject);
    procedure mostrarBasedeTreinamento;
    procedure RGB2vNeuronios(R,G,B: real; var RGBs: TVetorAtributos);
    procedure aprendizagem;
    function  distancia(var X,Y:array of real):real;
    function  hci(rc,ri: TPos) : real;
    function  selecionaVencedor(R,G,B: real; var VNeuronios: ArrayNeuronios; modo:boolean) : TPos;
    procedure resetaVetor(var VNeuronios : ArrayNeuronios);
    function  procuraZero(var VNeuronios : ArrayNeuronios;indice:integer):integer;
    procedure salvaArquivo(FileName : String);
    procedure abreArquivo(FileName : String);
    procedure defineStatusBotoes;
  private
    { Private declarations }
  public
    { Public declarations }
    //SNeuronios: array[1..n_padrao] of TShape;
    SNeuronios: array[1..nL,1..nC] of TShape;
  end;

var
  FormPrincipal   : TFormPrincipal;
  n : real; //taxa de aprendizagem
  ncor,ncorSaida,nCorSelect : TColor;
  alfa, d_std, raio : real;
  Neuronios : ArrayNeuronios;
  NeuroniosTrei : array [1..n_padrao] of TVetorAtributos;

implementation

{$R *.dfm}

procedure TFormPrincipal.FormCreate(Sender: TObject);
var
  i,j,l,t : integer;
  rs,gs,bs : string;
  ri,gi,bi : integer;
begin
   // Inicializando Parâmetros
   // Taxa de Aprendizagem e Raio do Centro de Vizinhança
   alfa := 1;
   d_std := 1;

   // Inicializando Neuronios com Pontos Iniciais
   self.resetaVetor(Neuronios);

   // Desenhando Mapa...
   l := 125;
   t := 45;
 for i := 1 to nL do
 begin
    for j := 1 to nC do
    begin
      SNeuronios[i,j] := TShape.Create(Panel2);
      SNeuronios[i,j].Parent := Panel2;
      SNeuronios[i,j].Left := l;
      SNeuronios[i,j].Top := t;
      SNeuronios[i,j].Width := 50;
      SNeuronios[i,j].Height := 50;
      SNeuronios[i,j].Pen.Color := ClSilver;

      rs := floattostr(trunc(Neuronios[i,j][1]));
      ri := strtoint(rs);
      gs := floattostr(trunc(Neuronios[i,j][2]));
      gi := strtoint(gs);
      bs := floattostr(trunc(Neuronios[i,j][3]));
      bi := strtoint(bs);

      //SNeuronios[i].Pen.Color := TColor(RGB(ri,gi,bi));
      SNeuronios[i,j].Brush.Color := TColor(RGB(ri,gi,bi));

      l := l + 49;

    end;
    l := 125;
    t := t + 49;
 end;


end;//fim proc


//Métodos e Funções Criadas <><><><><><><><><><><><><><><><><><><><><><><><><><>

//------------------------------------------------------------------------------
// Método de Aprendizagem
procedure TFormPrincipal.aprendizagem;
var
  r,g,b : real;
  incr,t,e : integer;
  i,j,pos : Integer;
  rs,gs,bs : string;
  ri,gi,bi : integer;
  calc, teste : real;
  atualizado : boolean;
  str_atu : string;
  hci_f : real;
  vnc,antl,postl,antc,postc : TPos;
  antno,postne,antso,postse : TPos;
begin

  // Raio da Vizinhança...
  raio := SEVizinhos.Value;

  for e := 1 to n_padrao do
  begin

     //r:=10; g:=134; b:=255;
     r := NeuroniosTrei[e][1];
     g := NeuroniosTrei[e][2];
     b := NeuroniosTrei[e][3];

     t := 1;
     alfa := 1;

     Self.Memo1.Lines.Add('/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\');
     Self.Memo2.Lines.Add('/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\');
     Memo1.Lines.Append(formatfloat('Entrada 000',e)+'-R('+floattostr(R)+')/G('+floattostr(G)+')/B('+floattostr(B)+')');
     Memo2.Lines.Append(formatfloat('Entrada 000',e)+'-R('+floattostr(R)+')/G('+floattostr(G)+')/B('+floattostr(B)+')');

     // Vencedor = menor distância Euclidiana
     vnc := selecionaVencedor(R,G,B,Neuronios,true);
     Memo2.Lines.Append('Vencedor ['+inttostr(vnc.l)+','+inttostr(vnc.c)+']-R('+floattostr(Neuronios[vnc.l,vnc.c][1])+')/G('+floattostr(Neuronios[vnc.l,vnc.c][2])+')/B('+floattostr(Neuronios[vnc.l,vnc.c][3])+')');
     Self.Memo2.Lines.Add('');

     Memo2.Lines.Append('Atualizando Pesos...');
     Memo2.Lines.Append('ANTES: ['+inttostr(vnc.l)+','+inttostr(vnc.c)+']-R('+floattostr(Neuronios[vnc.l,vnc.c][1])+')/G('+floattostr(Neuronios[vnc.l,vnc.c][2])+')/B('+floattostr(Neuronios[vnc.l,vnc.c][3])+')');

     // Devido ser a mesma posição
     // não faz sentido calcular a vizinhança
     //hci_f := hci(aux_indice,i);
     for j := 1 to 3 do
     begin
        calc := alfa * (Neuronios[0,1][j]-Neuronios[vnc.l,vnc.c][j]);
        if calc > 0 then
        begin
           Neuronios[vnc.l,vnc.c][j] := Neuronios[vnc.l,vnc.c][j] + round(calc);
        end else
        begin
           Neuronios[vnc.l,vnc.c][j] := Neuronios[vnc.l,vnc.c][j] + round(calc);
        end;
     end;
     Memo2.Lines.Append('APÓS : ['+inttostr(vnc.l)+','+inttostr(vnc.c)+']-R('+floattostr(Neuronios[vnc.l,vnc.c][1])+')/G('+floattostr(Neuronios[vnc.l,vnc.c][2])+')/B('+floattostr(Neuronios[vnc.l,vnc.c][3])+')');
     Memo2.Lines.Append('');
     rs := floattostr(Neuronios[vnc.l,vnc.c][1]);
     ri := strtoint(rs);
     gs := floattostr(Neuronios[vnc.l,vnc.c][2]);
     gi := strtoint(gs);
     bs := floattostr(Neuronios[vnc.l,vnc.c][3]);
     bi := strtoint(bs);
     SNeuronios[vnc.l,vnc.c].Pen.Color := clWhite;
     //SNeuronios[aux_indice].Pen.Width := 5;
     SNeuronios[vnc.l,vnc.c].Brush.Color := TColor(RGB(ri,gi,bi));
     SNeuronios[vnc.l,vnc.c].Repaint;


     // Atualizando Vizinhança
     hci_f := 1;

     // Valores Iniciais de Linhas Anteriores e Posteriores
     antl.l := vnc.l - 1;
     antl.c := vnc.c;
     postl.l := vnc.l + 1;
     postl.c := vnc.c;

     // Valores Iniciais de Colunas Anteriores e Posteriores
     antc.l := vnc.l;
     antc.c := vnc.c - 1;
     postc.l := vnc.l;
     postc.c := vnc.c + 1;

     // Valores Iniciais de Linha de Coluna Noroeste e Sudeste
     antno.l := vnc.l - 1;
     antno.c := vnc.c - 1;
     postse.l := vnc.l + 1;
     postse.c := vnc.c + 1;

     // Valores Iniciais de Linha de Coluna Sudoeste e Nordeste
     antso.l := vnc.l + 1;
     antso.c := vnc.c - 1;
     postne.l := vnc.l - 1;
     postne.c := vnc.c + 1;


     while (hci_f > 0.01) do
     begin

        // Anterior Linha Anterior----------------------------------------------
        hci_f := hci(vnc,antl);
        if (antl.l) >= 1 then
        begin

           Memo2.Lines.Append('ANTES: ['+inttostr(antl.l)+','+inttostr(antl.c)+']-R('+floattostr(Neuronios[antl.l,antl.c][1])+')/G('+floattostr(Neuronios[antl.l,antl.c][2])+')/B('+floattostr(Neuronios[antl.l,antl.c][3])+')');

           for j := 1 to 3 do
           begin
              calc := alfa * hci_f * (Neuronios[0,1][j]-Neuronios[antl.l,antl.c][j]);
              if calc > 0 then
              begin
                 Neuronios[antl.l,antl.c][j] := Neuronios[antl.l,antl.c][j] + round(calc);
              end else
              begin
                 Neuronios[antl.l,antl.c][j] := Neuronios[antl.l,antl.c][j] + round(calc);
              end;
           end;

           Memo2.Lines.Append('APÓS : ['+inttostr(antl.l)+','+inttostr(antl.c)+']-R('+floattostr(Neuronios[antl.l,antl.c][1])+')/G('+floattostr(Neuronios[antl.l,antl.c][2])+')/B('+floattostr(Neuronios[antl.l,antl.c][3])+')');
           Memo2.Lines.Append('');
           rs := floattostr(Neuronios[antl.l,antl.c][1]);
           ri := strtoint(rs);
           gs := floattostr(Neuronios[antl.l,antl.c][2]);
           gi := strtoint(gs);
           bs := floattostr(Neuronios[antl.l,antl.c][3]);
           bi := strtoint(bs);
           //SNeuronios[ant].Pen.Color := clWhite;
           //SNeuronios[ant].Pen.Width := 2;
           SNeuronios[antl.l,antl.c].Brush.Color := TColor(RGB(ri,gi,bi));
           SNeuronios[antl.l,antl.c].Repaint;
        end;
        dec(antl.l);

        // Anterior Linha Posterior---------------------------------------------
        hci_f := hci(vnc,postl);
        if (postl.l) <= nl then
        begin

           Memo2.Lines.Append('ANTES: ['+inttostr(postl.l)+','+inttostr(postl.c)+']-R('+floattostr(Neuronios[postl.l,postl.c][1])+')/G('+floattostr(Neuronios[postl.l,postl.c][2])+')/B('+floattostr(Neuronios[postl.l,postl.c][3])+')');

           for j := 1 to 3 do
           begin
              calc := alfa * hci_f * (Neuronios[0,1][j]-Neuronios[postl.l,postl.c][j]);
              if calc > 0 then
              begin
                 Neuronios[postl.l,postl.c][j] := Neuronios[postl.l,postl.c][j] + round(calc);
              end else
              begin
                 Neuronios[postl.l,postl.c][j] := Neuronios[postl.l,postl.c][j] + round(calc);
              end;
           end;

           Memo2.Lines.Append('APÓS : ['+inttostr(postl.l)+','+inttostr(postl.c)+']-R('+floattostr(Neuronios[postl.l,postl.c][1])+')/G('+floattostr(Neuronios[postl.l,postl.c][2])+')/B('+floattostr(Neuronios[postl.l,postl.c][3])+')');
           Memo2.Lines.Append('');
           rs := floattostr(Neuronios[postl.l,postl.c][1]);
           ri := strtoint(rs);
           gs := floattostr(Neuronios[postl.l,postl.c][2]);
           gi := strtoint(gs);
           bs := floattostr(Neuronios[postl.l,postl.c][3]);
           bi := strtoint(bs);
           //SNeuronios[ant].Pen.Color := clWhite;
           //SNeuronios[ant].Pen.Width := 2;
           SNeuronios[postl.l,postl.c].Brush.Color := TColor(RGB(ri,gi,bi));
           SNeuronios[postl.l,postl.c].Repaint;
        end;
        inc(postl.l);


        // Coluna Anterior------------------------------------------------------
        hci_f := hci(vnc,antc);
        if (antc.c) >= 1 then
        begin

           Memo2.Lines.Append('ANTES: ['+inttostr(antc.l)+','+inttostr(antc.c)+']-R('+floattostr(Neuronios[antc.l,antc.c][1])+')/G('+floattostr(Neuronios[antc.l,antc.c][2])+')/B('+floattostr(Neuronios[antc.l,antc.c][3])+')');

           for j := 1 to 3 do
           begin
              calc := alfa * hci_f * (Neuronios[0,1][j]-Neuronios[antc.l,antc.c][j]);
              if calc > 0 then
              begin
                 Neuronios[antc.l,antc.c][j] := Neuronios[antc.l,antc.c][j] + round(calc);
              end else
              begin
                 Neuronios[antc.l,antc.c][j] := Neuronios[antc.l,antc.c][j] + round(calc);
              end;
           end;

           Memo2.Lines.Append('APÓS : ['+inttostr(antc.l)+','+inttostr(antc.c)+']-R('+floattostr(Neuronios[antc.l,antc.c][1])+')/G('+floattostr(Neuronios[antc.l,antc.c][2])+')/B('+floattostr(Neuronios[antc.l,antc.c][3])+')');
           Memo2.Lines.Append('');
           rs := floattostr(Neuronios[antc.l,antc.c][1]);
           ri := strtoint(rs);
           gs := floattostr(Neuronios[antc.l,antc.c][2]);
           gi := strtoint(gs);
           bs := floattostr(Neuronios[antc.l,antc.c][3]);
           bi := strtoint(bs);
           //SNeuronios[ant].Pen.Color := clWhite;
           //SNeuronios[ant].Pen.Width := 2;
           SNeuronios[antc.l,antc.c].Brush.Color := TColor(RGB(ri,gi,bi));
           SNeuronios[antc.l,antc.c].Repaint;
        end;
        dec(antc.c);

        // Coluna Posterior-----------------------------------------------------
        hci_f := hci(vnc,postc);
        if (postc.c) <= nl then
        begin

           Memo2.Lines.Append('ANTES: ['+inttostr(postc.l)+','+inttostr(postc.c)+']-R('+floattostr(Neuronios[postc.l,postc.c][1])+')/G('+floattostr(Neuronios[postc.l,postc.c][2])+')/B('+floattostr(Neuronios[postc.l,postc.c][3])+')');

           for j := 1 to 3 do
           begin
              calc := alfa * hci_f * (Neuronios[0,1][j]-Neuronios[postc.l,postc.c][j]);
              if calc > 0 then
              begin
                 Neuronios[postc.l,postc.c][j] := Neuronios[postc.l,postc.c][j] + round(calc);
              end else
              begin
                 Neuronios[postc.l,postc.c][j] := Neuronios[postc.l,postc.c][j] + round(calc);
              end;
           end;

           Memo2.Lines.Append('APÓS : ['+inttostr(postc.l)+','+inttostr(postc.c)+']-R('+floattostr(Neuronios[postc.l,postc.c][1])+')/G('+floattostr(Neuronios[postc.l,postc.c][2])+')/B('+floattostr(Neuronios[postc.l,postc.c][3])+')');
           Memo2.Lines.Append('');
           rs := floattostr(Neuronios[postc.l,postc.c][1]);
           ri := strtoint(rs);
           gs := floattostr(Neuronios[postc.l,postc.c][2]);
           gi := strtoint(gs);
           bs := floattostr(Neuronios[postc.l,postc.c][3]);
           bi := strtoint(bs);
           //SNeuronios[ant].Pen.Color := clWhite;
           //SNeuronios[ant].Pen.Width := 2;
           SNeuronios[postc.l,postc.c].Brush.Color := TColor(RGB(ri,gi,bi));
           SNeuronios[postc.l,postc.c].Repaint;
        end;
        inc(postc.c);

        // Noroeste-------------------------------------------------------------
        hci_f := hci(vnc,antno);
        if ((antno.c) >= 1) and ((antno.l) >= 1) then
        begin

           Memo2.Lines.Append('ANTES: ['+inttostr(antno.l)+','+inttostr(antno.c)+']-R('+floattostr(Neuronios[antno.l,antno.c][1])+')/G('+floattostr(Neuronios[antno.l,antno.c][2])+')/B('+floattostr(Neuronios[antno.l,antno.c][3])+')');

           for j := 1 to 3 do
           begin
              calc := alfa * hci_f * (Neuronios[0,1][j]-Neuronios[antno.l,antno.c][j]);
              if calc > 0 then
              begin
                 Neuronios[antno.l,antno.c][j] := Neuronios[antno.l,antno.c][j] + round(calc);
              end else
              begin
                 Neuronios[antno.l,antno.c][j] := Neuronios[antno.l,antno.c][j] + round(calc);
              end;
           end;

           Memo2.Lines.Append('APÓS : ['+inttostr(antno.l)+','+inttostr(antno.c)+']-R('+floattostr(Neuronios[antno.l,antno.c][1])+')/G('+floattostr(Neuronios[antno.l,antno.c][2])+')/B('+floattostr(Neuronios[antno.l,antno.c][3])+')');
           Memo2.Lines.Append('');
           rs := floattostr(Neuronios[antno.l,antno.c][1]);
           ri := strtoint(rs);
           gs := floattostr(Neuronios[antno.l,antno.c][2]);
           gi := strtoint(gs);
           bs := floattostr(Neuronios[antno.l,antno.c][3]);
           bi := strtoint(bs);
           //SNeuronios[ant].Pen.Color := clWhite;
           //SNeuronios[ant].Pen.Width := 2;
           SNeuronios[antno.l,antno.c].Brush.Color := TColor(RGB(ri,gi,bi));
           SNeuronios[antno.l,antno.c].Repaint;
        end;
        dec(antno.c);
        dec(antno.l);

        // Sudeste--------------------------------------------------------------
        hci_f := hci(vnc,postse);
        if ((postse.l) <= nl) and ((postse.c) <= nc) then
        begin

           Memo2.Lines.Append('ANTES: ['+inttostr(postse.l)+','+inttostr(postse.c)+']-R('+floattostr(Neuronios[postse.l,postse.c][1])+')/G('+floattostr(Neuronios[postse.l,postse.c][2])+')/B('+floattostr(Neuronios[postse.l,postse.c][3])+')');

           for j := 1 to 3 do
           begin
              calc := alfa * hci_f * (Neuronios[0,1][j]-Neuronios[postse.l,postse.c][j]);
              if calc > 0 then
              begin
                 Neuronios[postse.l,postse.c][j] := Neuronios[postse.l,postse.c][j] + round(calc);
              end else
              begin
                 Neuronios[postse.l,postse.c][j] := Neuronios[postse.l,postse.c][j] + round(calc);
              end;
           end;

           Memo2.Lines.Append('APÓS : ['+inttostr(postse.l)+','+inttostr(postse.c)+']-R('+floattostr(Neuronios[postse.l,postse.c][1])+')/G('+floattostr(Neuronios[postse.l,postse.c][2])+')/B('+floattostr(Neuronios[postse.l,postse.c][3])+')');
           Memo2.Lines.Append('');
           rs := floattostr(Neuronios[postse.l,postse.c][1]);
           ri := strtoint(rs);
           gs := floattostr(Neuronios[postse.l,postse.c][2]);
           gi := strtoint(gs);
           bs := floattostr(Neuronios[postse.l,postse.c][3]);
           bi := strtoint(bs);
           //SNeuronios[ant].Pen.Color := clWhite;
           //SNeuronios[ant].Pen.Width := 2;
           SNeuronios[postse.l,postse.c].Brush.Color := TColor(RGB(ri,gi,bi));
           SNeuronios[postse.l,postse.c].Repaint;
        end;
        inc(postse.l);
        inc(postse.c);

        // Sudoeste-------------------------------------------------------------
        hci_f := hci(vnc,antso);
        if ((antso.c) >= 1) and ((antso.l) <= nl) then
        begin

           Memo2.Lines.Append('ANTES: ['+inttostr(antso.l)+','+inttostr(antso.c)+']-R('+floattostr(Neuronios[antso.l,antso.c][1])+')/G('+floattostr(Neuronios[antso.l,antso.c][2])+')/B('+floattostr(Neuronios[antso.l,antso.c][3])+')');

           for j := 1 to 3 do
           begin
              calc := alfa * hci_f * (Neuronios[0,1][j]-Neuronios[antso.l,antso.c][j]);
              if calc > 0 then
              begin
                 Neuronios[antso.l,antso.c][j] := Neuronios[antso.l,antso.c][j] + round(calc);
              end else
              begin
                 Neuronios[antso.l,antso.c][j] := Neuronios[antso.l,antso.c][j] + round(calc);
              end;
           end;

           Memo2.Lines.Append('APÓS : ['+inttostr(antso.l)+','+inttostr(antso.c)+']-R('+floattostr(Neuronios[antso.l,antso.c][1])+')/G('+floattostr(Neuronios[antso.l,antso.c][2])+')/B('+floattostr(Neuronios[antso.l,antso.c][3])+')');
           Memo2.Lines.Append('');
           rs := floattostr(Neuronios[antso.l,antso.c][1]);
           ri := strtoint(rs);
           gs := floattostr(Neuronios[antso.l,antso.c][2]);
           gi := strtoint(gs);
           bs := floattostr(Neuronios[antso.l,antso.c][3]);
           bi := strtoint(bs);
           //SNeuronios[ant].Pen.Color := clWhite;
           //SNeuronios[ant].Pen.Width := 2;
           SNeuronios[antso.l,antso.c].Brush.Color := TColor(RGB(ri,gi,bi));
           SNeuronios[antso.l,antso.c].Repaint;
        end;
        dec(antso.c);
        inc(antso.l);

        // Nordeste--------------------------------------------------------------
        hci_f := hci(vnc,postne);
        if ((postne.l) >= 1) and ((postne.c) <= nc) then
        begin

           Memo2.Lines.Append('ANTES: ['+inttostr(postne.l)+','+inttostr(postne.c)+']-R('+floattostr(Neuronios[postne.l,postne.c][1])+')/G('+floattostr(Neuronios[postne.l,postne.c][2])+')/B('+floattostr(Neuronios[postne.l,postne.c][3])+')');

           for j := 1 to 3 do
           begin
              calc := alfa * hci_f * (Neuronios[0,1][j]-Neuronios[postne.l,postne.c][j]);
              if calc > 0 then
              begin
                 Neuronios[postne.l,postne.c][j] := Neuronios[postne.l,postne.c][j] + round(calc);
              end else
              begin
                 Neuronios[postne.l,postne.c][j] := Neuronios[postne.l,postne.c][j] + round(calc);
              end;
           end;

           Memo2.Lines.Append('APÓS : ['+inttostr(postne.l)+','+inttostr(postne.c)+']-R('+floattostr(Neuronios[postne.l,postne.c][1])+')/G('+floattostr(Neuronios[postne.l,postne.c][2])+')/B('+floattostr(Neuronios[postne.l,postne.c][3])+')');
           Memo2.Lines.Append('');
           rs := floattostr(Neuronios[postne.l,postne.c][1]);
           ri := strtoint(rs);
           gs := floattostr(Neuronios[postne.l,postne.c][2]);
           gi := strtoint(gs);
           bs := floattostr(Neuronios[postne.l,postne.c][3]);
           bi := strtoint(bs);
           //SNeuronios[ant].Pen.Color := clWhite;
           //SNeuronios[ant].Pen.Width := 2;
           SNeuronios[postne.l,postne.c].Brush.Color := TColor(RGB(ri,gi,bi));
           SNeuronios[postne.l,postne.c].Repaint;
        end;
        dec(postne.l);
        inc(postne.c);

        (*
        // Atualizando Posteriores...
        hci_f := hci(aux_indice,post);
        if post <= n_padrao then
        begin

           Memo2.Lines.Append('ANTES: '+formatfloat('000',post)+'-R('+floattostr(Neuronios[post][1])+')/G('+floattostr(Neuronios[post][2])+')/B('+floattostr(Neuronios[post][3])+')');

           for j := 1 to 3 do
           begin
              calc := alfa * hci_f * (Neuronios[0][j]-Neuronios[post][j]);
              if calc > 0 then
              begin
                 Neuronios[post][j] := Neuronios[post][j] + round(calc);
              end else
              begin
                 Neuronios[post][j] := Neuronios[post][j] + round(calc);
              end;
           end;

           Memo2.Lines.Append('APÓS : '+formatfloat('000',post)+'-R('+floattostr(Neuronios[post][1])+')/G('+floattostr(Neuronios[post][2])+')/B('+floattostr(Neuronios[post][3])+')');
           Memo2.Lines.Append('');
           rs := floattostr(Neuronios[post][1]);
           ri := strtoint(rs);
           gs := floattostr(Neuronios[post][2]);
           gi := strtoint(gs);
           bs := floattostr(Neuronios[post][3]);
           bi := strtoint(bs);
           //SNeuronios[post].Pen.Color := clWhite;
           //SNeuronios[post].Pen.Width := 2;
           SNeuronios[post].Brush.Color := TColor(RGB(ri,gi,bi));
           SNeuronios[post].Repaint;
        end;
        inc(post);

        SNeuronios[aux_indice].Pen.Color := clSilver;
        //SNeuronios[aux_indice].Pen.Width := 2;
        SNeuronios[aux_indice].Repaint;
        *)
     end;
  end;

  (*
  while (alfa >=0) do
  begin

      for i := 1 to n_padrao do
      begin
        atualizado := false;
        str_atu := 'ANTES: '+formatfloat('Neurônio 000',i)+' - R('+floattostr(Neuronios[i][1])+') / G('+floattostr(Neuronios[i][2])+') / B('+floattostr(Neuronios[i][3])+')';
        for j := 1 to 3 do
        begin
          //calc := 0;
          //teste := 0;
          calc := (alfa * hci(aux_indice,i) * (Neuronios[aux_indice][j]-Neuronios[i][j]));
          if calc > 1 then
          begin
             //Neuronios[i][j] := Neuronios[i][j] + round(calc);
             Neuronios[i][j] := Neuronios[i][j] + round(calc);
             //ShowMessage(floattostr(Neuronios[i][j]));
             atualizado := true;
             pos := i;
          end;
          //Neuronios[i][j] := Neuronios[i][j] + (alfa * hci(aux_indice,i) * (Neuronios[aux_indice][j]-Neuronios[i][j]));
        end;

        // Atualizando Taxa de Aprendizado
        alfa := 0.9 * (1 - t/4);

        // Caso peso do Neurônio for atualizado será mostrado
        if atualizado = true then
        begin

           Memo2.Lines.Append(str_atu);
           Memo2.Lines.Append('APÓS : '+formatfloat('Neurônio 000',i)+' - R('+floattostr(Neuronios[pos][1])+') / G('+floattostr(Neuronios[pos][2])+') / B('+floattostr(Neuronios[pos][3])+')');
           rs := floattostr(Neuronios[i][1]);
           ri := strtoint(rs);
           gs := floattostr(Neuronios[i][2]);
           gi := strtoint(gs);
           bs := floattostr(Neuronios[i][3]);
           bi := strtoint(bs);
           SNeuronios[i].Pen.Color := clWhite;
           SNeuronios[i].Pen.Width := 2;
           SNeuronios[i].Brush.Color := TColor(RGB(ri,gi,bi));
           SNeuronios[i].Repaint;
           atualizado := false;
        end;

      end;
      inc(t);
  end;
  Memo2.Lines.Append('Atualizado para '+formatfloat('Neurônio 000',aux_indice)+' - R('+floattostr(Neuronios[aux_indice][1])+') / G('+floattostr(Neuronios[aux_indice][2])+') / B('+floattostr(Neuronios[aux_indice][3])+')');

end;
  *)

//------------------------------------------------------------------------------
(*
  incr := 30;
  count := 1;

  //Taxa de Aprendizado
  //if n>0.1 then n:= n-n*0.1; //cf
  n := 1;

  r:=255; g:=0; b:=0;
  while (g <= 255) do
  begin
      Memo1.Lines.Append(formatfloat('APR 000',count)+' - R('+inttostr(R)+') / G('+inttostr(G)+') / B('+inttostr(B)+')');
      Memo2.Lines.Append(formatfloat('APR 000',count)+' - R('+inttostr(R)+') / G('+inttostr(G)+') / B('+inttostr(B)+')');

      // Vencedor = menor distância Euclidiana
      aux_indice := selecionaVencedor(R,G,B,Neuronios,true);
      for i := 1 to n_padrao do
        for j := 1 to 3 do
        begin
          Neuronios[i][j] := Neuronios[i][j] + (alfa * hci(aux_indice,i) * (Neuronios[aux_indice][j]-Neuronios[0][j]));
        end;

      inc(count);
      g := g + incr;
  end;


    r:=255; g:=255; b:=0;
    while (r >= 0) do
    begin
      Memo1.Lines.Append(formatfloat('APR 000',count)+' - R('+inttostr(R)+') / G('+inttostr(G)+') / B('+inttostr(B)+')');
      Memo2.Lines.Append(formatfloat('APR 000',count)+' - R('+inttostr(R)+') / G('+inttostr(G)+') / B('+inttostr(B)+')');

      // Vencedor = menor distância Euclidiana
      aux_indice := selecionaVencedor(R,G,B,Neuronios,true);
      for i := 1 to n_padrao do
        for j := 1 to 3 do
        begin
          Neuronios[i][j] := Neuronios[i][j] + (alfa * hci(aux_indice,i) * (Neuronios[aux_indice][j]-Neuronios[0][j]));
        end;

      inc(count);
      r := r - incr;
    end;

    r:=0; g:=255; b:=0;
    while (b <= 255) do
    begin
      Memo1.Lines.Append(formatfloat('APR 000',count)+' - R('+inttostr(R)+') / G('+inttostr(G)+') / B('+inttostr(B)+')');
      Memo2.Lines.Append(formatfloat('APR 000',count)+' - R('+inttostr(R)+') / G('+inttostr(G)+') / B('+inttostr(B)+')');

      // Vencedor = menor distância Euclidiana
      aux_indice := selecionaVencedor(R,G,B,Neuronios,true);
      for i := 1 to n_padrao do
        for j := 1 to 3 do
        begin
          Neuronios[i][j] := Neuronios[i][j] + (alfa * hci(aux_indice,i) * (Neuronios[aux_indice][j]-Neuronios[0][j]));
        end;

      inc(count);
      b := b + incr;
    end;

    r:=0; g:=255; b:=255;
    while (g >= 0) do
    begin
      Memo1.Lines.Append(formatfloat('APR 000',count)+' - R('+inttostr(R)+') / G('+inttostr(G)+') / B('+inttostr(B)+')');
      Memo2.Lines.Append(formatfloat('APR 000',count)+' - R('+inttostr(R)+') / G('+inttostr(G)+') / B('+inttostr(B)+')');

      // Vencedor = menor distância Euclidiana
      aux_indice := selecionaVencedor(R,G,B,Neuronios,true);
      for i := 1 to n_padrao do
        for j := 1 to 3 do
        begin
          Neuronios[i][j] := Neuronios[i][j] + (alfa * hci(aux_indice,i) * (Neuronios[aux_indice][j]-Neuronios[0][j]));
        end;

      inc(count);
      g := g - incr;
    end;

    r:=0; g:=0; b:=255;
    while (r <= 255) do
    begin
      Memo1.Lines.Append(formatfloat('APR 000',count)+' - R('+inttostr(R)+') / G('+inttostr(G)+') / B('+inttostr(B)+')');
      Memo2.Lines.Append(formatfloat('APR 000',count)+' - R('+inttostr(R)+') / G('+inttostr(G)+') / B('+inttostr(B)+')');

      // Vencedor = menor distância Euclidiana
      aux_indice := selecionaVencedor(R,G,B,Neuronios,true);
      for i := 1 to n_padrao do
        for j := 1 to 3 do
        begin
          Neuronios[i][j] := Neuronios[i][j] + (alfa * hci(aux_indice,i) * (Neuronios[aux_indice][j]-Neuronios[0][j]));
        end;

      inc(count);
      r := r + incr;
    end;

    r:=255; g:=0; b:=255;
    while (b >= 0) do
    begin
      Memo1.Lines.Append(formatfloat('APR 000',count)+' - R('+inttostr(R)+') / G('+inttostr(G)+') / B('+inttostr(B)+')');
      Memo2.Lines.Append(formatfloat('APR 000',count)+' - R('+inttostr(R)+') / G('+inttostr(G)+') / B('+inttostr(B)+')');

      // Vencedor = menor distância Euclidiana
      aux_indice := selecionaVencedor(R,G,B,Neuronios,true);
      for i := 1 to n_padrao do
        for j := 1 to 3 do
        begin
          Neuronios[i][j] := Neuronios[i][j] + (alfa * hci(aux_indice,i) * (Neuronios[aux_indice][j]-Neuronios[0][j]));
        end;

      inc(count);
      b := b - incr;
    end;

*)

end;


//------------------------------------------------------------------------------
// Habilita e Desabilita Botões
procedure TFormPrincipal.defineStatusBotoes;
begin
        if( RadioGroup1.ItemIndex = 0) //--- Escolhido modo de aprendizagem
           then
                begin
                  FormPrincipal.BotClassificar.Enabled := false;
                  FormPrincipal.BotAprender.Enabled := true ;
                  //FormPrincipal.Width := 418;
                end
           else
                 begin
                  FormPrincipal.BotAprender.Enabled := false;
                  FormPrincipal.BotClassificar.Enabled := true;
                  //FormPrincipal.Width := 597;
                 end;
end;

//------------------------------------------------------------------------------
// Mostrar Base de Treinamento para possíveis Alterações
procedure TFormPrincipal.mostrarBasedeTreinamento;
var
  x,y,incr,count,x_ini : integer;
  i,j,rnd : integer;
  r,g,b : real;
  rs,gs,bs : string;
  ri,gi,bi : integer;
  temp : TVetorAtributos;
begin
  // Opção Randomizando...
  Randomize;
  Self.Memo1.Lines.Add('Randomizando Treinamento...');
  for i := 0 to n_padrao do
     for j := 1 to 3 do
     begin
        NeuroniosTrei[i][j] := RandomRange(0,255);
        //Self.Memo1.Lines.Add(formatfloat('000',i)+' - R/G/B '+floattostr(NeuroniosTrei[i][1])+' '+floattostr(NeuroniosTrei[i][2])+' '+floattostr(NeuroniosTrei[i][3]));
     end;

  // Opção por Amostragem...
  x_ini := 30;
  x := x_ini;
  y := 2;
  incr := 25;
  count := 1;

 repeat

    r:=255; g:=0; b:=0;
    while (g <= 255) do
    begin
        rs := floattostr(r);
        ri := strtoint(rs);
        gs := floattostr(g);
        gi := strtoint(gs);
        bs := floattostr(b);
        bi := strtoint(bs);
        Canvas.Pixels[y,x] := RGB(ri,gi,bi);
        if (x = x_ini) then
        begin
{            NeuroniosTrei[count][1] := r;
            NeuroniosTrei[count][2] := g;
            NeuroniosTrei[count][3] := b;
}            inc(count);
        end;
        inc(Y);
        g := g + incr;
    end;

    r:=255; g:=255; b:=0;
    while (r >= 0) do
    begin
        rs := floattostr(r);
        ri := strtoint(rs);
        gs := floattostr(g);
        gi := strtoint(gs);
        bs := floattostr(b);
        bi := strtoint(bs);
        Canvas.Pixels[y,x] := RGB(ri,gi,bi);
        if (x = x_ini) then
        begin
{            NeuroniosTrei[count][1] := r;
            NeuroniosTrei[count][2] := g;
            NeuroniosTrei[count][3] := b;
}            inc(count);
        end;
        inc(Y);
        r := r - incr;
    end;

    r:=0; g:=255; b:=0;
    while (b <= 255) do
    begin
        rs := floattostr(r);
        ri := strtoint(rs);
        gs := floattostr(g);
        gi := strtoint(gs);
        bs := floattostr(b);
        bi := strtoint(bs);
        Canvas.Pixels[y,x] := RGB(ri,gi,bi);
        if (x = x_ini) then
        begin
{            NeuroniosTrei[count][1] := r;
            NeuroniosTrei[count][2] := g;
            NeuroniosTrei[count][3] := b;
}            inc(count);
        end;
        inc(Y);
        b := b + incr;
    end;

    r:=0; g:=255; b:=255;
    while (g >= 0) do
    begin
        rs := floattostr(r);
        ri := strtoint(rs);
        gs := floattostr(g);
        gi := strtoint(gs);
        bs := floattostr(b);
        bi := strtoint(bs);
        Canvas.Pixels[y,x] := RGB(ri,gi,bi);
        if (x = x_ini) then
        begin
{            NeuroniosTrei[count][1] := r;
            NeuroniosTrei[count][2] := g;
            NeuroniosTrei[count][3] := b;
}            inc(count);
        end;
        inc(Y);
        g := g - incr;
    end;

    r:=0; g:=0; b:=255;
    while (r <= 255) do
    begin
        rs := floattostr(r);
        ri := strtoint(rs);
        gs := floattostr(g);
        gi := strtoint(gs);
        bs := floattostr(b);
        bi := strtoint(bs);
        Canvas.Pixels[y,x] := RGB(ri,gi,bi);
        if (x = x_ini) then
        begin
{            NeuroniosTrei[count][1] := r;
            NeuroniosTrei[count][2] := g;
            NeuroniosTrei[count][3] := b;
}            inc(count);
        end;
        inc(Y);
        r := r + incr;
    end;

    r:=255; g:=0; b:=255;
    while (b >= 0) do
    begin
        rs := floattostr(r);
        ri := strtoint(rs);
        gs := floattostr(g);
        gi := strtoint(gs);
        bs := floattostr(b);
        bi := strtoint(bs);
        Canvas.Pixels[y,x] := RGB(ri,gi,bi);
        if (x = x_ini) then
        begin
{            NeuroniosTrei[count][1] := r;
            NeuroniosTrei[count][2] := g;
            NeuroniosTrei[count][3] := b;
}            inc(count);
        end;
        inc(Y);
        b := b - incr;
    end;

   inc(x);
   y :=2;
   until (x > 645);


  // Embaralhando padrões;
  (*Randomize;
  for i := 1 to n_padrao do
  begin
     rnd := Random(n_padrao);
     temp := NeuroniosTrei[i];
     NeuroniosTrei[i] := NeuroniosTrei[rnd];
     NeuroniosTrei[rnd] := temp;
  end;
  *)

  // Mostrando...
  for i := 1 to n_padrao do
       Self.Memo1.Lines.Add(formatfloat('000',i)+' - R/G/B '+floattostr(NeuroniosTrei[i][1])+' '+floattostr(NeuroniosTrei[i][2])+' '+floattostr(NeuroniosTrei[i][3]));
  Self.Memo1.Lines.Add('------------------------------------');
  Self.Memo1.Lines.Add('');

end;

//------------------------------------------------------------------------------
// Vizinhos mais próximos
function TFormPrincipal.hci(rc, ri: TPos): real;
begin
   //caption:= floattostr(exp(-0.0168089171709006));
   //hci := exp(-1*(sqr(rc-ri)/(2*sqr(d_std))));
   hci := exp(-1*((sqr(rc.l-ri.l)+sqr(rc.c-ri.c))/(raio)));
end;


//------------------------------------------------------------------------------
// Função para o Cálcula da Distância Euclidiana
function TFormPrincipal.Distancia(var X,Y:array of real):real;
var I:longint;
    R:real;
begin
    R:=0;
    for I:=Low(X) to High(X) do
       R:=R+sqr(X[I]-Y[I]);
    Distancia:=sqrt(R);
end;

//------------------------------------------------------------------------------
//
procedure TFormPrincipal.RGB2vNeuronios(R,G,B: real; var RGBs: TVetorAtributos);
begin
    RGBs[1] := R;
    RGBs[2] := G;
    RGBs[3] := B;
end;

//------------------------------------------------------------------------------
// Calcula Distância Euclidiana entre todos os Neurônio e retorna vencedor
function TFormPrincipal.selecionaVencedor(R,G,B: real; var VNeuronios: ArrayNeuronios;
  modo: boolean): TPos;
var
     i,j : Integer;
     aux : real;
     aux_indice : TPos;
begin
       Self.RGB2vNeuronios(R,G,B,VNeuronios[0,1]);

       aux_indice.l := 1;
       aux_indice.c := 1;

       // Calculando Distancia Euclidiana
       aux := Distancia(VNeuronios[0,1],VNeuronios[1,1]); //www.schulers.com/jpss
       Memo1.Lines.Append('');
       Memo1.Lines.Append('Distâncias Euclidianas...');
       Memo1.Lines.Append('E - N['+inttostr(1)+','+inttostr(1)+']='+floattostr(aux));

       for i := 1 to nL do
       for j := 1 to nC do
           begin
               if(aux > Distancia(VNeuronios[0,1],VNeuronios[i,j])) then
                  begin
                      aux_indice.l := i;
                      aux_indice.c := j;
                      aux := Distancia(VNeuronios[0,1],VNeuronios[i,j]);
                  end;
               Memo1.Lines.Append('E - N['+inttostr(i)+','+inttostr(j)+']='+floattostr(Distancia(VNeuronios[0,1],VNeuronios[i,j])));
           end;
       //Memo2.Lines.Append('VENCEDOR Neurônio = ['+inttostr(aux_indice)+']');

       {
       if modo= true then
           begin
                i:= procuraZero(VNeuronios,aux_indice);
                if i<>aux_indice then
                        aux_indice:=i;
           end;
       }
       //FormPrincipal.Memo2.Lines.Append('Indice Procura ZERO Selecionado = ['+inttostr(aux_indice)+']');

       Memo2.Lines.Append('');
       Memo1.Lines.Append('');
       selecionaVencedor := aux_indice;

end;

//------------------------------------------------------------------------------
function TFormPrincipal.procuraZero(var VNeuronios : ArrayNeuronios;indice:integer):integer;
var
     j,i : Integer;
     zero : boolean;
begin
{     i:=indice-1;
     zero:=false;
     while (zero<>true)and(i<n_padrao) do
          begin
               inc(i);
               zero:= true;
               for j:= 1 to 3 do
                   if Neuronios[i][j] <> 0 then
                      zero:= false;
          end;
     procuraZero:= i;
}     
end;

//------------------------------------------------------------------------------
// Resetando Neurônios
procedure TFormPrincipal.resetaVetor(var VNeuronios : ArrayNeuronios);
var
   i,j,c : Integer;
begin
    Randomize;
    for i := 0 to nL do
    for j := 1 to nC do
        for c := 1 to 3 do
              VNeuronios[i,j][c] := random(100);

    (*
    // Inserindo Pontos Iniciais
    // Preto
    VNeuronios[5][1] := 0;
    VNeuronios[5][2] := 0;
    VNeuronios[5][3] := 0;

    // Vermelho
    VNeuronios[14][1] := 255;
    VNeuronios[14][2] := 0;
    VNeuronios[14][3] := 0;

    // Amarelo
    VNeuronios[23][1] := 255;
    VNeuronios[23][2] := 255;
    VNeuronios[23][3] := 0;

    // Verde
    VNeuronios[32][1] := 0;
    VNeuronios[32][2] := 255;
    VNeuronios[32][3] := 0;

    // Azul
    VNeuronios[41][1] := 0;
    VNeuronios[41][2] := 0;
    VNeuronios[41][3] := 255;

    // Rosa
    VNeuronios[50][1] := 255;
    VNeuronios[50][2] := 0;
    VNeuronios[50][3] := 255;

    // Branco
    VNeuronios[59][1] := 255;
    VNeuronios[59][2] := 255;
    VNeuronios[59][3] := 255;
    *)

    Self.Memo1.Lines.Add('Neurônios Randômicos Iniciais...');
    for i := 1 to nL do
    for j := 1 to nC do
        Self.Memo1.Lines.Add(formatfloat('000',i)+' - R/G/B '+floattostr(Neuronios[i,j][1])+' '+floattostr(Neuronios[i,j][2])+' '+floattostr(Neuronios[i,j][3]));
    Self.Memo1.Lines.Add('------------------------------------');

end;

//------------------------------------------------------------------------------

  procedure TFormPrincipal.salvaArquivo(FileName : String);
  var
     arquivo : File of ArrayNeuronios;
  begin
     assignFile(arquivo,FileName);
     rewrite(arquivo);
     write(arquivo,Neuronios);
     closefile(arquivo);
  end;

//------------------------------------------------------------------------------

  procedure TFormPrincipal.abreArquivo(FileName : String);
  var
     arquivo : File of ArrayNeuronios;
     i,j : integer;
  begin
      if(FileExists(FileName)) then
         begin
           assignfile(arquivo,FileName);
           reset(arquivo);
           read(arquivo,Neuronios);
           closefile(arquivo);
         end
      else
         showmessage('Arquivo não encontrado...');

      Self.Memo1.Lines.Add('Carregando Base...');
      for i := 1 to nL do
      for j := 1 to nC do
         //Self.Memo1.Lines.Add(formatfloat('000',i)+' - R/G/B '+floattostr(Neuronios[i][1])+' '+floattostr(Neuronios[i][2])+' '+floattostr(Neuronios[i][3]));

  end;

//------------------------------------------------------------------------------


//Métodos e Funções Dinâmicas <><><><><><><><><><><><><><><><><><><><><><><><><>

//------------------------------------------------------------------------------
// Atualizado na Barra de Status a cor selecionana R,B e G.
procedure TFormPrincipal.FormMouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
begin
//  ncor := ColorToRGB(FormPrincipal.Canvas.Pixels[X,Y]);
//  Barra.SimpleText := 'Pixel Selecionado : ' +  Format('r(%d) / g(%d) / b(%d)',[GetRValue(nCor), GetGValue(nCor), GetBValue(nCor)]);
//  //r := GetRValue(nCor); g := GetGValue(nCor); b := GetBValue(nCor);
  ncor := ColorToRGB(Canvas.Pixels[X,Y]);
  Barra.SimpleText := 'Pixel Selecionado : ' +  Format('r(%d) / g(%d) / b(%d)',[GetRValue(nCor), GetGValue(nCor), GetBValue(nCor)]);
end;

procedure TFormPrincipal.Image1MouseMove(Sender: TObject;
  Shift: TShiftState; X, Y: Integer);
begin
  ncor := ColorToRGB(Image1.Canvas.Pixels[X,Y]);
  Barra.SimpleText := 'Pixel Selecionado : ' +  Format('r(%d) / g(%d) / b(%d)',[GetRValue(nCor), GetGValue(nCor), GetBValue(nCor)]);
end;

procedure TFormPrincipal.FormDblClick(Sender: TObject);
begin
  nCorSelect := nCor;
  Shape1.Brush.Color := nCorSelect;
  Label6.Caption := Format('r(%d)g(%d)b(%d)',[GetRValue(nCorSelect), GetGValue(nCorSelect), GetBValue(nCorSelect)]);
end;


//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
procedure TFormPrincipal.RadioGroup1Click(Sender: TObject);
begin
   FormPrincipal.defineStatusBotoes;
end;

//------------------------------------------------------------------------------
procedure TFormPrincipal.Abrir1Click(Sender: TObject);
var
   NomeArq : String;
begin
      if OpenDialog1.Execute then
         begin
             NomeArq := OpenDialog1.FileName;
             Self.abreArquivo(NomeArq);
         end;
end;

procedure TFormPrincipal.Salvarcomo1Click(Sender: TObject);
var
    NomeArq : String;
begin
    if SaveDialog1.Execute then
        begin
            NomeArq := SaveDialog1.FileName;
            if(length(NomeArq) > 0) then
                begin
                  salvaArquivo(NomeArq);
                end;
        end;
end;

procedure TFormPrincipal.Sair1Click(Sender: TObject);
begin
    Self.Close;
end;

procedure TFormPrincipal.BitBtn1Click(Sender: TObject);
begin
   OpenPictureDialog1.InitialDir := ExtractFilePath(Application.ExeName);
   if OpenPictureDialog1.Execute then
      Image1.Picture.LoadFromFile(OpenPictureDialog1.FileName);
end;

procedure TFormPrincipal.BitBtn2Click(Sender: TObject);
begin
    Self.mostrarBasedeTreinamento;
end;

procedure TFormPrincipal.BotAprenderClick(Sender: TObject);
var I : integer;
begin
    Self.aprendizagem;

    Self.Memo1.Lines.Add('');
    Self.Memo1.Lines.Add('Neurônios Após Aprendizagem...');
    for i := 1 to n_padrao do
        //Self.Memo1.Lines.Add(formatfloat('000',i)+' - R/G/B '+floattostr(Neuronios[i][1])+' '+floattostr(Neuronios[i][2])+' '+floattostr(Neuronios[i][3]));

end;

procedure TFormPrincipal.Image1DblClick(Sender: TObject);
begin
  nCorSelect := nCor;
  Shape1.Brush.Color := nCorSelect;
  Label6.Caption := Format('r(%d)g(%d)b(%d)',[GetRValue(nCorSelect), GetGValue(nCorSelect), GetBValue(nCorSelect)]);
end;

procedure TFormPrincipal.BotClassificarClick(Sender: TObject);
var
  ind : integer;
  rs,gs,bs : string;
  ri,gi,bi,i : integer;
  T_Color : TColor;
begin
(*
    Label4.Caption := 'Saída';
    Memo2.Lines.Append('Entrada - R('+floattostr(GetRValue(nCorSelect))+')/G('+floattostr(GetGValue(nCorSelect))+')/B('+floattostr(GetBValue(nCorSelect))+')');

    ind := selecionaVencedor(GetRValue(nCorSelect),GetGValue(nCorSelect),GetBValue(nCorSelect),Neuronios,false);

    Memo2.Lines.Append('Vencedor '+formatfloat('000',ind)+'-R('+floattostr(Neuronios[ind][1])+')/G('+floattostr(Neuronios[ind][2])+')/B('+floattostr(Neuronios[ind][3])+')');
    Self.Memo2.Lines.Add('');

    T_Color := SNeuronios[ind].Brush.Color;
    SNeuronios[ind].Brush.Color := clWhite;
    SNeuronios[ind].Repaint;
    sleep(150);
    SNeuronios[ind].Brush.Color := T_Color;
    SNeuronios[ind].Repaint;

    Label4.Caption := 'Saída : ' + inttostr(ind);

    rs := floattostr(Neuronios[ind][1]);
    gs := floattostr(Neuronios[ind][2]);
    bs := floattostr(Neuronios[ind][3]);

    ri := strtoint(rs);
    gi := strtoint(gs);
    bi := strtoint(bs);

    ncorSaida := TColor(RGB(ri,gi,bi));
    Shape3.Brush.Color := ncorSaida;
    Label7.Caption := Format('r(%d)g(%d)b(%d)',[GetRValue(nCorSaida), GetGValue(nCorSaida), GetBValue(nCorSaida)]);
*)
end;

procedure TFormPrincipal.Sair2Click(Sender: TObject);
begin
   Close;
end;



procedure TFormPrincipal.BitBtn5Click(Sender: TObject);
var
  r,g,b,incr,count : integer;
  i,j,k,aux_indice : Integer;
  rs,gs,bs : string;
  ri,gi,bi : integer;
  calc : real;
begin
(*
  k := 1;
  count := 1;
  r:=10; g:=134; b:=255;
  while (k <= 50) do
  begin
      Memo1.Lines.Append(formatfloat('APR 000',count)+' - R('+inttostr(R)+') / G('+inttostr(G)+') / B('+inttostr(B)+')');
      // Vencedor = menor distância Euclidiana
      aux_indice := selecionaVencedor(R,G,B,Neuronios,true);
      for i := 1 to n_padrao do
      begin
        for j := 1 to 3 do
        begin
          calc := (alfa * hci(aux_indice,i) * (Neuronios[aux_indice][j]-Neuronios[i][j]));
          if calc > 0 then  Neuronios[i][j] := trunc(Neuronios[i][j] + calc);
          //Neuronios[i][j] := Neuronios[i][j] + (alfa * hci(aux_indice,i) * (Neuronios[aux_indice][j]-Neuronios[i][j]));
        end;


         rs := floattostr(trunc(Neuronios[i][1]));
         ri := strtoint(rs);
         gs := floattostr(trunc(Neuronios[i][2]));
         gi := strtoint(gs);
         bs := floattostr(trunc(Neuronios[i][3]));
         bi := strtoint(bs);

         SNeuronios[i].Brush.Color := TColor(RGB(ri,gi,bi));
         SNeuronios[i].Repaint;
     end;
     
      inc(count);
      inc(k);
  end;



//////////////////

  k := 1;
  count := 1;
  r:=10; g:=255; b:=25;
  while (k <= 50) do
  begin
      Memo1.Lines.Append(formatfloat('APR 000',count)+' - R('+inttostr(R)+') / G('+inttostr(G)+') / B('+inttostr(B)+')');
      // Vencedor = menor distância Euclidiana
      aux_indice := selecionaVencedor(R,G,B,Neuronios,true);
      for i := 1 to n_padrao do
      begin
        for j := 1 to 3 do
        begin
          calc := (alfa * hci(aux_indice,i) * (Neuronios[aux_indice][j]-Neuronios[i][j]));
          if calc > 0 then  Neuronios[i][j] := trunc(Neuronios[i][j] + calc);
          //Neuronios[i][j] := Neuronios[i][j] + (alfa * hci(aux_indice,i) * (Neuronios[aux_indice][j]-Neuronios[i][j]));
        end;


         rs := floattostr(trunc(Neuronios[i][1]));
         ri := strtoint(rs);
         gs := floattostr(trunc(Neuronios[i][2]));
         gi := strtoint(gs);
         bs := floattostr(trunc(Neuronios[i][3]));
         bi := strtoint(bs);

         SNeuronios[i].Brush.Color := TColor(RGB(ri,gi,bi));
         SNeuronios[i].Repaint;
     end;
     
      inc(count);
      inc(k);
  end;

*)
end;


end.
