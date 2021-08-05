//+------------------------------------------------------------------+
//|                                                      Retorno.mq4 |
//|                        Copyright 2020, MetaQuotes Software Corp. |
//|                                             https://www.mql5.com |
//+------------------------------------------------------------------+
#property copyright "Copyright 2020, MetaQuotes Software Corp."
#property link      "https://www.mql5.com"
#property version   "1.00"
#property strict
#property indicator_chart_window
//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
#define NOME_BOTAO "btn_retorno"

bool MouseClicked = false; 

enum entradaSinal {
   Atual = 0,  
   Proxima = 1
};

//painel inicial
input string s0 = "=========== INDICADOR ==========="; // ==Configuração Do Indicador de Sinal==
input string NomeIndicador = ""; // Nome do Arquivo Indicador de Sinal
input int CallBufferIndicador = 0;      // Buffer de Sinal -> Compra ("Call") 
input int PutBufferIndicador = 1;       // Buffer de Sinal -> Venda ("Put")
input entradaSinal EntradaSinal = Proxima; // Entrada na Vela
input int TempoExpiracao = 0; // Tempo de Expiração da Vela
input string s1 = "=========== RETORNO ==========="; // ==Configuração Do Local do Retorno==
input string LocalArqRetorno = ""; // Local Onde Salvar o Arquivo de Retorno

//variaveis
datetime tempoEnviado;
string terminal_data_path;
string nomearquivo;
string data_patch;
int fileHandle;
int tempo_expiracao;

int OnInit()
  {
//--- indicator buffers mapping

   EventSetTimer(1); 
   tempoEnviado = TimeCurrent();
   terminal_data_path = TerminalInfoString(TERMINAL_DATA_PATH)+"\\MQL4\\Files\\";
   MqlDateTime time;
   datetime tempo = TimeToStruct(TimeLocal(),time);
   string hoje = StringFormat("%d%02d%02d",time.year,time.mon,time.day);
   nomearquivo = hoje+"_retorno.csv";
   data_patch = LocalArqRetorno;
   tempo_expiracao = TempoExpiracao;
   if(tempo_expiracao == 0){
      tempo_expiracao = Period();
   }
   
   ObjectCreate(0,NOME_BOTAO, OBJ_BUTTON,0,0,0);
   ObjectSetInteger(0,NOME_BOTAO,OBJPROP_CORNER,3);
   ObjectSetInteger(0,NOME_BOTAO,OBJPROP_XSIZE,100);
   ObjectSetInteger(0,NOME_BOTAO,OBJPROP_YSIZE,30);
   ObjectSetInteger(0,NOME_BOTAO,OBJPROP_XDISTANCE,110);
   ObjectSetInteger(0,NOME_BOTAO,OBJPROP_YDISTANCE,35);
   ObjectSetInteger(0,NOME_BOTAO,OBJPROP_COLOR,clrAntiqueWhite);
   ObjectSetInteger(0,NOME_BOTAO,OBJPROP_BGCOLOR,clrCrimson);
   
   ObjectSetString(0,NOME_BOTAO,OBJPROP_TEXT,"Retorno Off...");

   if(data_patch == ""){
      data_patch = terminal_data_path;
   }
      
   if(FileIsExist(nomearquivo,0)){
      Print("Local do Arquivo: "+data_patch+nomearquivo);
      fileHandle = FileOpen(nomearquivo,FILE_CSV|FILE_READ|FILE_WRITE);
      string data = "tempo,ativo,acao,expiracao";
      FileWrite(fileHandle,data);
      FileClose(fileHandle);
      
   }
   else{
      Print("Criando Arquivo de Retorno...");
      Print("Local do Arquivo: "+data_patch+nomearquivo);
      fileHandle = FileOpen(nomearquivo,FILE_CSV|FILE_READ|FILE_WRITE);
      string data = "tempo,ativo,acao,expiracao";
      FileWrite(fileHandle,data);
      FileClose(fileHandle);
      
   }   
//---
   return(INIT_SUCCEEDED);
  }
 
 void OnDeinit(const int reason)
{
   EventKillTimer();
   ObjectDelete(NOME_BOTAO);
      
}

//+------------------------------------------------------------------+
//| Custom indicator iteration function                              |
//+------------------------------------------------------------------+
int OnCalculate(const int rates_total,
                const int prev_calculated,
                const datetime &time[],
                const double &open[],
                const double &high[],
                const double &low[],
                const double &close[],
                const long &tick_volume[],
                const long &volume[],
                const int &spread[])
  {
//---
   double call = 0, put = 0;
   ResetLastError();
   
   if (NomeIndicador != "") {
      call = iCustom(NULL, 0, NomeIndicador, CallBufferIndicador,EntradaSinal);
      put = iCustom(NULL, 0, NomeIndicador, PutBufferIndicador,EntradaSinal);
   }
   else {
      Print("ERRO!!! Verifique o Nome do Indicador!");
      
   }
   
   int erro = GetLastError();
   if (erro == 4072) {
      Print("ERRO!!! Indicador Não Encontrado!");
      ResetLastError();
      }
   
   if(MouseClicked == true){
      
              
      if (sinal(call) && Time[0] > tempoEnviado) {
         Print(Symbol(), "," , tempo_expiracao , ",CALL," , Time[0]);
         fileHandle = FileOpen(nomearquivo,FILE_CSV|FILE_READ|FILE_WRITE);
         FileSeek(fileHandle, 0, SEEK_END); 
         string data = IntegerToString((long)TimeGMT())+","+Symbol()+",call,"+IntegerToString(tempo_expiracao);
         FileWrite(fileHandle,data);
         FileClose(fileHandle);
         tempoEnviado = Time[0];       
      }
            
      if (sinal(put) && Time[0] > tempoEnviado) {
         Print(Symbol() , "," , tempo_expiracao ,",PUT," , Time[0]);
         fileHandle = FileOpen(nomearquivo,FILE_CSV|FILE_READ|FILE_WRITE);
         FileSeek(fileHandle, 0, SEEK_END); 
         string data = IntegerToString((long)TimeGMT())+","+Symbol()+",put,"+IntegerToString(tempo_expiracao);
         FileWrite(fileHandle,data);
         FileClose(fileHandle);
         tempoEnviado = Time[0]; 
      }
      
   }
   
   else{
      return false;
   }
   
//--- return value of prev_calculated for next call
   return(rates_total);
   
}

bool sinal (double value) 
{
   if (value != 0 && value != EMPTY_VALUE)
      return true;
   else
      return false;
} 

//+------------------------------------------------------------------+
//| ChartEvent function                                              |
//+------------------------------------------------------------------+
void OnChartEvent(const int id,
                  const long &lparam,
                  const double &dparam,
                  const string &sparam)
  {
//---
   if(id==CHARTEVENT_OBJECT_CLICK && sparam == NOME_BOTAO){
   
      if(MouseClicked == false){
      
         ObjectSetInteger(0,NOME_BOTAO,OBJPROP_COLOR,clrAntiqueWhite);
         ObjectSetInteger(0,NOME_BOTAO,OBJPROP_BGCOLOR,clrDarkGreen);
   
         ObjectSetString(0,NOME_BOTAO,OBJPROP_TEXT,"Retorno On...");
         
         Print("Retorno Ligado!");
         //Alert("Retorno Ligado!");
         
              
         MouseClicked = true; 
      }
      else{
         ObjectSetInteger(0,NOME_BOTAO,OBJPROP_COLOR,clrAntiqueWhite);
         ObjectSetInteger(0,NOME_BOTAO,OBJPROP_BGCOLOR,clrCrimson);
   
         ObjectSetString(0,NOME_BOTAO,OBJPROP_TEXT,"Retorno Off...");
         Print("Retorno Desligado!");
         //Alert("Retorno Desligado!");
         
         MouseClicked = false;
      }
         
      }
   
  }
//+------------------------------------------------------------------+
