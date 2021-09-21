#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_X_TABU 10
#define TAM_Y_TABU 10
#define TAM_CARRIER 5
#define TAM_BATTLESHIP 4
#define TAM_CRUISER 3
#define TAM_SUBMARINE 3
#define TAM_DESTROYER 2
#define NOME_ARQUIVO_1 "tabu_1.txt"
#define NOME_ARQUIVO_2 "tabu_2.txt"
#define NOME_ARQUIVO_SAIDA_INIC "inicializacao.txt"
#define NOME_ARQUIVO_SAIDA_VERIF "validacao_tabuleiros.txt"
#define TAMANHO_NOME_ARQUIVO_ENTRADA_INIC 10
#define TAMANHO_NOME_ARQUIVO_SAIDA_INIC 17

typedef struct {
  char diretorioEntrada[500];
  char diretorioSaida[500];
  char conteudo[50][100];
  int qtdLinhas;
}tArquivoInicializacao;

typedef struct {
  char nome[11]; // no max 10 char + '\0'
  int id;
  int tamanho;
  int orientacao; // 0 vertical e 1 horizontal
  char posicaoYLetra; // (a~j)
  int posicaoY;
  int posicaoX;
  int valido;
}tNavio;

typedef struct {
  char posicoes[TAM_Y_TABU][TAM_X_TABU]; // seguirá o padrao a1, ou seja, YX
  int qtdNaviosCarrier;
  int qtdNaviosBattleship;
  int qtdNaviosCruiser;
  int qtdNaviosSubmarine;
  int qtdNaviosDestroyer;
  int qtdNaviosTotal;
  tNavio navios[100];
  int valido;
  int compativel;
}tTabuleiro;

typedef struct {
  char nome[17];
  int id;
  tTabuleiro tabuleiro;
  tArquivoInicializacao arquivo;
}tJogador;

typedef struct {
  tJogador jogador1;
  tJogador jogador2;
  int idVencedor;
}tJogo;

tJogo InicializarJogo (tJogo jogo);
tTabuleiro ImplementaNavioNoTabuleiro(tTabuleiro tabuleiro, tNavio navio);
int VerificaDisponibilidadeDeCasas (tTabuleiro tabuleiro, tNavio navio);
int NavioEhValido (tNavio navio);
int RetornaTipoDeNavio (tNavio navio);
tNavio LeNavio (char *conteudoArq);
tNavio PadronizaValorYNavio (tNavio navio);
tNavio AtribuiTamanhoNavio (tNavio navio);
tJogador LeNomeJogador(tJogador jogador);
tJogador InicializaJogador (tJogador jogador, int id, tTabuleiro tabuleiro);
int VerificaNaviosColados (tTabuleiro tabuleiro, tNavio navio,int posicaoY, int posicaoX, int qtdCasasCriadas);
tJogo RotinaReiniciarJogo (tJogo jogo);
void ImprimeTabuleiroDecodificado (tTabuleiro tabuleiro);
void ImprimeTabuleiroCodificado (tTabuleiro tabuleiro);
tTabuleiro InvalidaTabuleiro (tTabuleiro tabuleiro);
tTabuleiro ValidaTabuleiro (tTabuleiro tabuleiro);
tTabuleiro AdicionaQtdNavios (tTabuleiro tabuleiro);
tTabuleiro RotinaLerNaviosValidarEGerarTabuleiro();
tArquivoInicializacao PegaArgvERetornaCaminho (char *argv);
tArquivoInicializacao LeituraDeArquivo (tArquivoInicializacao arquivoInic, char *nome_arquivo);
void ResultadoLeituraArquivo (int codigo);
tArquivoInicializacao RotinaLeituraInicializacao (tArquivoInicializacao arquivo, char* nome_arquivo, char **argv, int argc);
int RotinaLeituraNavios (tArquivoInicializacao arquivo, tTabuleiro tabuleiro);
int PegaNumCharTransformaNumInt (char c);
tTabuleiro ResetaTabuleiro (tTabuleiro tabuleiro);
void TransformaLinhaTabuleiroEmStringFormatada (char *linha, tTabuleiro tabuleiro, int indiceLinha);
void CriaArquivoInicializacao (char *diretorioSaida, char *nome_arquivo_saida,tJogador jogador1, tJogador jogador2);
void CriaArquivoSaidaVerificacaoTabu (char *diretorio_saida, char *nomeArquivoTabu1, char *nomeArquivoTabu2, tTabuleiro tabu1, tTabuleiro tabu2);
int VerificaCompatibilidadeTabuleiros (tTabuleiro tabu1, tTabuleiro tabu2);
tTabuleiro TornaTabuCompativel (tTabuleiro tabu);
tTabuleiro TornaTabuIncompativel (tTabuleiro tabu);

int main (int argc, char **argv) {
  tJogo jogo;
  tJogador jogador1, jogador2;
  tTabuleiro tabuleiro1, tabuleiro2;
  tArquivoInicializacao arquivo1, arquivo2;
  
  int rtn=0, qtdNavios=0;
  printf("erros tabuleiro 1:\n");
  arquivo1 = RotinaLeituraInicializacao(arquivo1, NOME_ARQUIVO_1, argv, argc);
  tabuleiro1 = RotinaLerNaviosValidarEGerarTabuleiro(arquivo1);

  printf("erros tabuleiro 2:\n");
  arquivo2 = RotinaLeituraInicializacao(arquivo2, NOME_ARQUIVO_2, argv, argc);
  tabuleiro2 = RotinaLerNaviosValidarEGerarTabuleiro(arquivo2);

  if(VerificaCompatibilidadeTabuleiros(tabuleiro1, tabuleiro2)) {
    tabuleiro1 = TornaTabuCompativel(tabuleiro1);
    tabuleiro2 = TornaTabuCompativel(tabuleiro2);
  }else {
    tabuleiro1 = TornaTabuIncompativel(tabuleiro1);
    tabuleiro2 = TornaTabuIncompativel(tabuleiro2);
  }
  CriaArquivoSaidaVerificacaoTabu(arquivo1.diretorioSaida, NOME_ARQUIVO_1, NOME_ARQUIVO_2, tabuleiro1, tabuleiro2);
  
  if(tabuleiro1.valido && tabuleiro2.valido && tabuleiro1.compativel && tabuleiro2.compativel) {
    jogador1 = InicializaJogador(jogador1, 1, tabuleiro1);
    jogador2 = InicializaJogador(jogador2, 2, tabuleiro2);
    CriaArquivoInicializacao(arquivo1.diretorioSaida, NOME_ARQUIVO_SAIDA_INIC, jogador1, jogador2);
  }
  return 0;
}

tTabuleiro TornaTabuIncompativel (tTabuleiro tabu) {
  tabu.compativel=0;
  return tabu;
}

tTabuleiro TornaTabuCompativel (tTabuleiro tabu) {
  tabu.compativel=1;
  return tabu;
}

int VerificaCompatibilidadeTabuleiros (tTabuleiro tabu1, tTabuleiro tabu2) {

  if(tabu1.qtdNaviosSubmarine == tabu2.qtdNaviosSubmarine
  && tabu1.qtdNaviosCarrier == tabu2.qtdNaviosCarrier
  && tabu1.qtdNaviosBattleship == tabu2.qtdNaviosBattleship
  && tabu1.qtdNaviosCruiser == tabu2.qtdNaviosCruiser
  && tabu1.qtdNaviosDestroyer == tabu2.qtdNaviosDestroyer 
  && tabu1.qtdNaviosTotal == tabu2.qtdNaviosTotal) {
    return 1;
  }else {
    return 0;
  }
}

void CriaArquivoSaidaVerificacaoTabu (char *diretorio_saida, char *nomeArquivoTabu1, char *nomeArquivoTabu2, tTabuleiro tabu1, tTabuleiro tabu2) {
  int y=0, x=0;
  char linha[40];
  FILE * arquivoSaida;
  char caminhoSaida[1000];

  sprintf(caminhoSaida, "%s/%s", diretorio_saida, NOME_ARQUIVO_SAIDA_VERIF);

  arquivoSaida = fopen(caminhoSaida, "w");
  if(!arquivoSaida) {
    printf("ERRO: Ocorreu um erro ao criar o arquivo (%s).", caminhoSaida);
    exit(1);
  }else {
    fprintf(arquivoSaida, "%s;", nomeArquivoTabu1);
    if(tabu1.valido) fprintf(arquivoSaida, "valido\n");
    else fprintf(arquivoSaida, "invalido\n");

    fprintf(arquivoSaida, "%s;", nomeArquivoTabu2);
    if(tabu2.valido) fprintf(arquivoSaida, "valido");
    else fprintf(arquivoSaida, "invalido");

    if(tabu1.valido && tabu2.valido) {
      if(tabu1.compativel && tabu2.compativel) fprintf(arquivoSaida, "\nTabuleiros compativeis entre si");
      else fprintf(arquivoSaida, "\nTabuleiros incompativeis entre si");
    }
  }
  fclose(arquivoSaida);
}

void CriaArquivoInicializacao (char *diretorioSaida, char *nome_arquivo_saida,tJogador jogador1, tJogador jogador2) {
  int y=0, x=0;
  char linha[40];
  FILE * arquivoSaida;
  char caminhoSaida[1000];

  sprintf(caminhoSaida, "%s/%s", diretorioSaida, nome_arquivo_saida);

  arquivoSaida = fopen(caminhoSaida, "w");
  if(!arquivoSaida) {
    printf("ERRO: Ocorreu um erro ao criar o arquivo (%s).", caminhoSaida);
    exit(1);
  }else {
    fprintf(arquivoSaida, "%s\n", jogador1.nome);
    for(y=0; y<TAM_Y_TABU; y++) {
      TransformaLinhaTabuleiroEmStringFormatada(linha, jogador1.tabuleiro, y);
      fprintf(arquivoSaida, "%s\n", linha);
    }
    fprintf(arquivoSaida, "\n");

    fprintf(arquivoSaida, "%s\n", jogador2.nome);
    for(y=0; y<TAM_Y_TABU; y++) {
      TransformaLinhaTabuleiroEmStringFormatada(linha, jogador2.tabuleiro, y);
      if(y+1 == TAM_Y_TABU) fprintf(arquivoSaida, "%s", linha);
      else fprintf(arquivoSaida, "%s\n", linha);
    }
  }              
  fclose(arquivoSaida);
}

tTabuleiro RotinaLerNaviosValidarEGerarTabuleiro(tArquivoInicializacao arquivo) {
  tNavio navio;
  tTabuleiro tabuleiro;
  int i=0, j=0;

  tabuleiro = ResetaTabuleiro(tabuleiro);

  for(j=0; j<arquivo.qtdLinhas; j++) { //qtd linhas OK
    navio = LeNavio(arquivo.conteudo[i]);
    if(NavioEhValido(navio)) {
      navio = PadronizaValorYNavio(navio);
      navio = AtribuiTamanhoNavio(navio); // até aqui td certo
      if(VerificaDisponibilidadeDeCasas(tabuleiro, navio)) {
        tabuleiro = ImplementaNavioNoTabuleiro(tabuleiro, navio);
        ImprimeTabuleiroDecodificado(tabuleiro);
        printf("\n");
        tabuleiro.qtdNaviosTotal++;
        tabuleiro.navios[i] = navio;
      }else {
        printf("ERRO: Disponibilidade do navio %d inválido\n", i+1);
        tabuleiro = InvalidaTabuleiro(tabuleiro);
        return tabuleiro;
      }
    }else {
      printf("ERRO: Navio de linha %d inválido\n", i+1);
      tabuleiro = InvalidaTabuleiro(tabuleiro);
      return tabuleiro;
    }
    i++;
  }
  tabuleiro = ValidaTabuleiro(tabuleiro);

  return tabuleiro;
}

tArquivoInicializacao RotinaLeituraInicializacao (tArquivoInicializacao arquivo, char *nome_arquivo, char **argv, int argc) {
  int rtn=0, i=0;

  if(argc<=1) {
    printf("ERRO: O diretorio de arquivos de configuracao nao foi informado.\n");
    exit(1);
  }
  arquivo = PegaArgvERetornaCaminho(argv[1]); 
  arquivo = LeituraDeArquivo(arquivo, nome_arquivo);
  return arquivo;
}

tArquivoInicializacao LeituraDeArquivo (tArquivoInicializacao arquivoInic, char *nome_arquivo) {
  FILE * arquivo;
  int i=0;
  char caminhoEntrada[1200];

  sprintf(caminhoEntrada, "%s/%s", arquivoInic.diretorioEntrada, nome_arquivo);

  arquivo = fopen(caminhoEntrada, "r");
  if(!arquivo) {
    printf("ERRO: Nao foi possivel abrir o arquivo (%s).\n", caminhoEntrada);
    exit(1);
  }else {
    while(1) {
      if(!feof(arquivo)) {
        if(fscanf(arquivo, "%[^\n]\n", arquivoInic.conteudo[i])==1) {
          i++;
          arquivoInic.qtdLinhas++;
        }else {
          printf("ERRO: O arquivo (%s) foi aberto mas nao foi possivel ler seu conteudo.\n", caminhoEntrada);
          exit(1);
        }
      }else break;
    }
  }
  fclose(arquivo);

  return arquivoInic;
}

tArquivoInicializacao PegaArgvERetornaCaminho (char *argv) {
  tArquivoInicializacao arquivo;
  sprintf(arquivo.diretorioEntrada, "%s", argv);
  sprintf(arquivo.diretorioSaida, "%s/saida", argv);
  
  return arquivo;
}

tTabuleiro ResetaTabuleiro (tTabuleiro tabuleiro) {
  int x=0, y=0;
  for(y=0; y<TAM_Y_TABU; y++) {
    for(x=0; x<TAM_X_TABU; x++) {
      tabuleiro.posicoes[y][x] = 'o';
    }
  }
  tabuleiro.valido=1;
  tabuleiro.compativel=1;
  tabuleiro.qtdNaviosTotal=0;
  tabuleiro.qtdNaviosCarrier=0;
  tabuleiro.qtdNaviosBattleship=0;
  tabuleiro.qtdNaviosCruiser=0;
  tabuleiro.qtdNaviosSubmarine=0;
  tabuleiro.qtdNaviosDestroyer=0;

  return tabuleiro;
}

tNavio LeNavio (char *conteudoArq) {
  tNavio navio;
  int i, numAux;
  i=0;
  numAux=0;
  
  while(1) {
    navio.nome[i] = conteudoArq[i];
    i++;
    if(conteudoArq[i] == ';') {
      navio.nome[i] = '\0';
      i++;
      if(conteudoArq[i+1] >= 48 && conteudoArq[i+1] <= 57) {
        navio.id=PegaNumCharTransformaNumInt(conteudoArq[i])*10 + PegaNumCharTransformaNumInt(conteudoArq[i+1]);
        i+=3;
      }else {
      navio.id = PegaNumCharTransformaNumInt(conteudoArq[i]);
      i+=2;
      }
      navio.orientacao = PegaNumCharTransformaNumInt(conteudoArq[i]);
      i+=2;
      navio.posicaoYLetra = conteudoArq[i];
      i++;
      if(conteudoArq[i+1] >= 48 && conteudoArq[i+1] <= 57) {
        navio.posicaoX = 10;
        break;
      }else {
        navio.posicaoX = PegaNumCharTransformaNumInt(conteudoArq[i]);
        break;
      }
    }
  }
  return navio;
}

int NavioEhValido (tNavio navio) {
  //nome, id, orientacao e posicao
  if(RetornaTipoDeNavio(navio) < 1 || RetornaTipoDeNavio(navio) > 5){
    printf("Nome de navio invalido\n");
    return 0;
  }
  if(navio.orientacao != 0 && navio.orientacao != 1)  {
    printf("Orientacao de navio invalida\n");
    return 0;
  }
  if(navio.posicaoYLetra < 'a' || navio.posicaoYLetra > 'j') {
    printf("Posicao vertical de navio invalida\n");
    return 0;
  }
  if(navio.posicaoX < 1 || navio.posicaoX > 10) {
    printf("Posicao horizontal de navio invalida\n");
    return 0;
  }
  return 1;
}

int VerificaNaviosColados (tTabuleiro tabuleiro, tNavio navio, int posicaoY, int posicaoX, int qtdCasasCriadas) {
  // como os navios sao 'criados' apenas em duas direcoes, devemos verificar apenas os lados da onde não estao sendo criados, quando pelo menos uma casa desse navio já foi criado.
  if(navio.orientacao==1) { // horizontal (esquerda->direita)
    if(qtdCasasCriadas==0) { //verifica todos os lados
      if(tabuleiro.posicoes[posicaoY-1][posicaoX-1] == 'X' && posicaoX > 0 && posicaoY > 0) {
        printf("COD: 101\n");
        return 0;
      }
      else if(tabuleiro.posicoes[posicaoY-1][posicaoX] == 'X' && posicaoY > 0) {
        printf("COD: 102\n");
        return 0;
      }
      else if(tabuleiro.posicoes[posicaoY-1][posicaoX+1] == 'X' && posicaoY > 0 && posicaoX < 9) {
        printf("COD: 103\n");
        return 0;
      }
      else if(tabuleiro.posicoes[posicaoY][posicaoX+1] == 'X' && posicaoX < 9) {
        printf("COD: 104\n");
        return 0;
      }
      else if(tabuleiro.posicoes[posicaoY+1][posicaoX+1] == 'X' && posicaoY < 9 && posicaoX < 9) {
        printf("COD: 105\n");
        return 0;
      }
      else if(tabuleiro.posicoes[posicaoY+1][posicaoX] == 'X' && posicaoY < 9) {
        printf("COD: 106\n");
        return 0;
      }
      else if(tabuleiro.posicoes[posicaoY+1][posicaoX-1] == 'X' && posicaoX > 0 && posicaoY < 9) {
        printf("COD: 107\n");
        return 0;
      }
      else if(tabuleiro.posicoes[posicaoY][posicaoX-1] == 'X' && posicaoX > 0) {
        printf("COD: 108\n");
        return 0;
      }
    }else {
      if(tabuleiro.posicoes[posicaoY-1][posicaoX-1] == 'X' && posicaoY > 0 && posicaoX > 0) {
        printf("COD: 111\n");
        return 0;
      }
      else if(tabuleiro.posicoes[posicaoY-1][posicaoX] == 'X' && posicaoY > 0) {
        printf("COD: 112\n");
        return 0;
      }
      else if(tabuleiro.posicoes[posicaoY-1][posicaoX+1] == 'X' && posicaoY > 0 && posicaoX < 9) {
        printf("COD: 113\n");
        return 0;
      }
      else if(tabuleiro.posicoes[posicaoY][posicaoX+1] == 'X' && posicaoX < 9) {
        printf("COD: 114\n");
        return 0;
      }
      else if(tabuleiro.posicoes[posicaoY+1][posicaoX+1] == 'X' && posicaoY < 9 && posicaoX < 9) {
        printf("COD: 115\n");
        return 0;
      }
      else if(tabuleiro.posicoes[posicaoY+1][posicaoX] == 'X' && posicaoY < 9) {
        printf("COD: 116\n");
        return 0;
      }
      else if(tabuleiro.posicoes[posicaoY+1][posicaoX-1] == 'X' && posicaoX > 0){
        printf("COD: 117\n");
        return 0;
      }
    }
  }else if(navio.orientacao==0) { // vertical (cima -> baixo)
    if(qtdCasasCriadas==0) {
      if(tabuleiro.posicoes[posicaoY-1][posicaoX-1] == 'X' && posicaoY > 0 && posicaoX > 0) {
        printf("COD: 001\n");
        return 0;
      }
      else if(tabuleiro.posicoes[posicaoY-1][posicaoX] == 'X' && posicaoY > 0) {
        printf("COD: 002\n");
        return 0;
      }
      else if(tabuleiro.posicoes[posicaoY-1][posicaoX+1] == 'X' && posicaoY > 0 && posicaoX < 9) {
        printf("COD: 003\n");
        return 0;
      }
      else if(tabuleiro.posicoes[posicaoY][posicaoX+1] == 'X' && posicaoX < 9) {
        printf("COD: 004\n");
        return 0;
      }
      else if(tabuleiro.posicoes[posicaoY+1][posicaoX+1] == 'X' && posicaoY < 9 && posicaoX < 9) {
        printf("COD: 005\n");
        return 0;
      }
      else if(tabuleiro.posicoes[posicaoY+1][posicaoX] == 'X' && posicaoY < 9) {
        printf("COD: 006\n");
        return 0;
      }
      else if(tabuleiro.posicoes[posicaoY+1][posicaoX-1] == 'X' && posicaoX > 0 && posicaoY < 9) {
        printf("COD: 007\n");
        return 0;
      }
      else if(tabuleiro.posicoes[posicaoY][posicaoX-1] == 'X' && posicaoX > 0){
        printf("COD: 008\n");
        return 0;
      }
    }else {
      if(tabuleiro.posicoes[posicaoY-1][posicaoX-1] == 'X' && posicaoY > 0 && posicaoX > 0) {
        printf("COD: 011\n");
        return 0;
      }
      else if(tabuleiro.posicoes[posicaoY-1][posicaoX+1] == 'X' && posicaoY > 0 && posicaoX < 9) {
        printf("COD: 012\n");
        return 0;
      }
      else if(tabuleiro.posicoes[posicaoY][posicaoX+1] == 'X' && posicaoX < 9) {
        printf("COD: 013\n");
        return 0;
      }
      else if(tabuleiro.posicoes[posicaoY+1][posicaoX+1] == 'X' && posicaoY < 9 && posicaoX < 9) {
        printf("COD: 014\n");
        return 0;
      }
      else if(tabuleiro.posicoes[posicaoY+1][posicaoX] == 'X' && posicaoY < 9) {
        printf("COD: 015\n");
        return 0;
      }
      else if(tabuleiro.posicoes[posicaoY+1][posicaoX-1] == 'X' && posicaoX > 0 && posicaoY < 9) {
        printf("COD: 016\n");
        return 0;
      }
      else if(tabuleiro.posicoes[posicaoY][posicaoX-1] == 'X' && posicaoX > 0) {
        printf("COD: 017\n");
        return 0;
      }
    }
  }
  return 1;
}

tNavio PadronizaValorYNavio (tNavio navio) {
  //a = 1, b=2, c=3, d=4, e=5, f=6, g=7, h=8, i=9, j=10;
  if(navio.posicaoYLetra == 'a') navio.posicaoY = 1;
  else if(navio.posicaoYLetra == 'b') navio.posicaoY = 2;
  else if(navio.posicaoYLetra == 'c') navio.posicaoY = 3;
  else if(navio.posicaoYLetra == 'd') navio.posicaoY = 4;
  else if(navio.posicaoYLetra == 'e') navio.posicaoY = 5;
  else if(navio.posicaoYLetra == 'f') navio.posicaoY = 6;
  else if(navio.posicaoYLetra == 'g') navio.posicaoY = 7;
  else if(navio.posicaoYLetra == 'h') navio.posicaoY = 8;
  else if(navio.posicaoYLetra == 'i') navio.posicaoY = 9;
  else if(navio.posicaoYLetra == 'j') navio.posicaoY = 10;
  return navio;
}

int RetornaTipoDeNavio (tNavio navio) {
  if(strcmp(navio.nome, "Carrier")==0){
   return 1;
  }else if(strcmp(navio.nome, "Battleship")==0){
   return 2;
  }else if(strcmp(navio.nome, "Cruiser")==0){
   return 3;
  }else if(strcmp(navio.nome, "Submarine")==0){
   return 4;
  }else if(strcmp(navio.nome, "Destroyer")==0){
   return 5;
  }else return -1;
}

tNavio AtribuiTamanhoNavio (tNavio navio) {
  if(strcmp(navio.nome, "Carrier")==0){
    navio.tamanho = 5; 
  }else if(strcmp(navio.nome, "Battleship")==0){
    navio.tamanho = 4;
  }else if(strcmp(navio.nome, "Cruiser")==0){
    navio.tamanho = 3;
  }else if(strcmp(navio.nome, "Submarine")==0){
    navio.tamanho = 3;
  }else if(strcmp(navio.nome, "Destroyer")==0){
    navio.tamanho = 2;
  }
  return navio;
}

tTabuleiro AdicionaQtdNavios (tTabuleiro tabuleiro) {
  int i=0;
  for(i=0; i<tabuleiro.qtdNaviosTotal; i++) {
    if(RetornaTipoDeNavio(tabuleiro.navios[i])==1) tabuleiro.qtdNaviosCarrier++;
    else if(RetornaTipoDeNavio(tabuleiro.navios[i])==2) tabuleiro.qtdNaviosBattleship++;
    else if(RetornaTipoDeNavio(tabuleiro.navios[i])==3) tabuleiro.qtdNaviosCruiser++;
    else if(RetornaTipoDeNavio(tabuleiro.navios[i])==4) tabuleiro.qtdNaviosSubmarine++;
    else if(RetornaTipoDeNavio(tabuleiro.navios[i])==5) tabuleiro.qtdNaviosDestroyer++;
  }
  return tabuleiro;
}

int VerificaDisponibilidadeDeCasas (tTabuleiro tabuleiro, tNavio navio) {
  int x=0, y=0, k=0;  

  if(navio.orientacao == 1) { // horizontal
    y = navio.posicaoY-1;
    x = navio.posicaoX-1;
    if(RetornaTipoDeNavio(navio) == 1) {
      for(k=0; k<TAM_CARRIER; k++) {
        if(tabuleiro.posicoes[y][x] == 'X') {
          printf("O navio (%s id: %d) está em uma posição que já tem outro navio\n", navio.nome, navio.id);
          return 0;
        }else if(x >= TAM_X_TABU) {
          printf("O navio (%s id: %d) está ultrapassando os limites do tabuleiro\n", navio.nome, navio.id);
          return 0;
        }else if(VerificaNaviosColados(tabuleiro, navio, y, x, k)==0) {
          printf("O navio (%s id: %d) está colado em outro navio na posicao (%d,%d) na %d posição do navio\n", navio.nome,navio.id, y+1, x+1, k+1);
          return 0;
        }
        x++;
      }
    }else if(RetornaTipoDeNavio(navio) == 2) {
      for(k=0; k<TAM_BATTLESHIP; k++) {
        if(tabuleiro.posicoes[y][x] == 'X') {
          printf("O navio (%s id: %d) está em uma posição que já tem outro navio\n", navio.nome, navio.id);
          return 0;
        }else if(x >= TAM_X_TABU) {
          printf("O navio (%s id: %d) está ultrapassando os limites do tabuleiro\n", navio.nome, navio.id);
          return 0;
        }else if(VerificaNaviosColados(tabuleiro, navio, y, x, k)==0) {
          printf("O navio (%s id: %d) está colado em outro navio na posicao (%d,%d) na %d posição do navio\n", navio.nome,navio.id, y+1, x+1, k+1);
          return 0;
        }
        x++;
      }
    }else if(RetornaTipoDeNavio(navio) == 3) {
      for(k=0; k<TAM_CRUISER; k++) {
        if(tabuleiro.posicoes[y][x] == 'X') {
          printf("O navio (%s id: %d) está em uma posição que já tem outro navio\n", navio.nome, navio.id);
          return 0;
        }else if(x >= TAM_X_TABU) {
          printf("O navio (%s id: %d) está ultrapassando os limites do tabuleiro\n", navio.nome, navio.id);
          return 0;
        }else if(VerificaNaviosColados(tabuleiro, navio, y, x, k)==0) {
          printf("O navio (%s id: %d) está colado em outro navio na posicao (%d,%d) na %d posição do navio\n", navio.nome,navio.id, y+1, x+1, k+1);
          return 0;
        }
        x++;
      }
    }else if(RetornaTipoDeNavio(navio) == 4) {
      for(k=0; k<TAM_SUBMARINE; k++) {
        if(tabuleiro.posicoes[y][x] == 'X') {
          printf("O navio (%s id: %d) está em uma posição que já tem outro navio\n", navio.nome, navio.id);
          return 0;
        }else if(x >= TAM_X_TABU) {
          printf("O navio (%s id: %d) está ultrapassando os limites do tabuleiro\n", navio.nome, navio.id);
          return 0;
        }else if(VerificaNaviosColados(tabuleiro, navio, y, x, k)==0) {
          printf("O navio (%s id: %d) está colado em outro navio na posicao (%d,%d) na %d posição do navio\n", navio.nome,navio.id, y+1, x+1, k+1);
          return 0;
        }
        x++;
      }
    }else if(RetornaTipoDeNavio(navio) == 5) {
      for(k=0; k<TAM_DESTROYER; k++) {
        if(tabuleiro.posicoes[y][x] == 'X') {
          printf("O navio (%s id: %d) está em uma posição que já tem outro navio\n", navio.nome, navio.id);
          return 0;
        }else if(x >= TAM_X_TABU) {
          printf("O navio (%s id: %d) está ultrapassando os limites do tabuleiro\n", navio.nome, navio.id);
          return 0;
        }else if(VerificaNaviosColados(tabuleiro, navio, y, x, k)==0) {
          printf("O navio (%s id: %d) está colado em outro navio na posicao (%d,%d) na %d posição do navio\n", navio.nome,navio.id, y+1, x+1, k+1);
          return 0;
        }
        x++;
      }
    }
  }else if(navio.orientacao == 0) { // vertical
  y = navio.posicaoY-1;
  x = navio.posicaoX-1;
    if(RetornaTipoDeNavio(navio) == 1) {
      for(k=0; k<TAM_CARRIER; k++) {
        if(tabuleiro.posicoes[y][x] == 'X') {
          printf("O navio (%s id: %d) está em uma posição que já tem outro navio\n", navio.nome, navio.id);
          return 0;
        }else if(y >= TAM_Y_TABU) {
          printf("O navio (%s id: %d) está ultrapassando os limites do tabuleiro\n", navio.nome, navio.id);
          return 0;
        }else if(VerificaNaviosColados(tabuleiro, navio, y, x, k)==0) {
          printf("O navio (%s id: %d) está colado em outro navio na posicao (%d,%d) na %d posição do navio\n", navio.nome,navio.id, y+1, x+1, k+1);
          return 0;
        }
        y++;
      }
    }else if(RetornaTipoDeNavio(navio) == 2) {
      for(k=0; k<TAM_BATTLESHIP; k++) {
        if(tabuleiro.posicoes[y][x] == 'X') {
          printf("O navio (%s id: %d) está em uma posição que já tem outro navio\n", navio.nome, navio.id);
          return 0;
        }else if(y >= TAM_Y_TABU) {
          printf("O navio (%s id: %d) está ultrapassando os limites do tabuleiro\n", navio.nome, navio.id);
          return 0;
        }else if(VerificaNaviosColados(tabuleiro, navio, y, x, k)==0) {
          printf("O navio (%s id: %d) está colado em outro navio na posicao (%d,%d) na %d posição do navio\n", navio.nome,navio.id, y+1, x+1, k+1);
          return 0;
        }
        y++;
      }
    }else if(RetornaTipoDeNavio(navio) == 3) {
      for(k=0; k<TAM_CRUISER; k++) {
        if(tabuleiro.posicoes[y][x] == 'X') {
          printf("O navio (%s id: %d) está em uma posição que já tem outro navio\n", navio.nome, navio.id);
          return 0;
        }else if(y >= TAM_Y_TABU) {
          printf("O navio (%s id: %d) está ultrapassando os limites do tabuleiro\n", navio.nome, navio.id);
          return 0;
        }else if(VerificaNaviosColados(tabuleiro, navio, y, x, k)==0) {
          printf("O navio (%s id: %d) está colado em outro navio na posicao (%d,%d) na %d posição do navio\n", navio.nome,navio.id, y+1, x+1, k+1);
          return 0;
        }
        y++;
      }
    }else if(RetornaTipoDeNavio(navio) == 4) {
      for(k=0; k<TAM_SUBMARINE; k++) {
        if(tabuleiro.posicoes[y][x] == 'X') {
          printf("O navio (%s id: %d) está em uma posição que já tem outro navio\n", navio.nome, navio.id);
          return 0;
        }else if(y >= TAM_Y_TABU) {
          printf("O navio (%s id: %d) está ultrapassando os limites do tabuleiro\n", navio.nome, navio.id);
          return 0;
        }else if(VerificaNaviosColados(tabuleiro, navio, y, x, k)==0) {
          printf("O navio (%s id: %d) está colado em outro navio na posicao (%d,%d) na %d posição do navio\n", navio.nome,navio.id, y+1, x+1, k+1);
          return 0;
        }
        y++;
      }
    }else if(RetornaTipoDeNavio(navio) == 5) {
      for(k=0; k<TAM_DESTROYER; k++) {
        if(tabuleiro.posicoes[y][x] == 'X') {
          printf("O navio (%s id: %d) está em uma posição que já tem outro navio\n", navio.nome, navio.id);
          return 0;
        }else if(y >= TAM_Y_TABU) {
          printf("O navio (%s id: %d) está ultrapassando os limites do tabuleiro\n", navio.nome, navio.id);
          return 0;
        }else if(VerificaNaviosColados(tabuleiro, navio, y, x, k)==0) {
          printf("O navio (%s id: %d) está colado em outro navio na posicao (%d,%d) na %d posição do navio\n", navio.nome,navio.id, y+1, x+1, k+1);
          return 0;
        }
        y++;
      }
    }
  }
  return 1;
}

tTabuleiro ImplementaNavioNoTabuleiro(tTabuleiro tabuleiro, tNavio navio) {
  int x=0, y=0, k=0;

  y = navio.posicaoY-1;
  x = navio.posicaoX-1;

  if(navio.orientacao == 1) { // horizontal
    if(RetornaTipoDeNavio(navio) == 1) {
      tabuleiro.qtdNaviosCarrier++;
      for(k=0; k<TAM_CARRIER; k++) {
        tabuleiro.posicoes[y][x] = 'X';
        x++;
      }
    }else if(RetornaTipoDeNavio(navio) == 2) {
      tabuleiro.qtdNaviosBattleship++;
      for(k=0; k<TAM_BATTLESHIP; k++) {
        tabuleiro.posicoes[y][x] = 'X';
        x++;
      }
    }else if(RetornaTipoDeNavio(navio) == 3) {
      tabuleiro.qtdNaviosCruiser++;
      for(k=0; k<TAM_CRUISER; k++) {
        tabuleiro.posicoes[y][x] = 'X';
        x++;
      }
    }else if(RetornaTipoDeNavio(navio) == 4) {
      tabuleiro.qtdNaviosSubmarine++;
      for(k=0; k<TAM_SUBMARINE; k++) {
        tabuleiro.posicoes[y][x] = 'X';
        x++;
      }
    }else if(RetornaTipoDeNavio(navio) == 5) {
    tabuleiro.qtdNaviosDestroyer++;
      for(k=0; k<TAM_DESTROYER; k++) {
        tabuleiro.posicoes[y][x] = 'X';
        x++;
      }
    }
  }else if(navio.orientacao == 0) { // vertical
    if(RetornaTipoDeNavio(navio) == 1) {
      tabuleiro.qtdNaviosCarrier++;
      for(k=0; k<TAM_CARRIER; k++) {
        tabuleiro.posicoes[y][x] = 'X';
        y++;
      }
    }else if(RetornaTipoDeNavio(navio) == 2) {
      tabuleiro.qtdNaviosBattleship++;
      for(k=0; k<TAM_BATTLESHIP; k++) {
        tabuleiro.posicoes[y][x] = 'X';
        y++;
      }
    }else if(RetornaTipoDeNavio(navio) == 3) {
      tabuleiro.qtdNaviosCruiser++;
      for(k=0; k<TAM_CRUISER; k++) {
        tabuleiro.posicoes[y][x] = 'X';
        y++;
      }
    }else if(RetornaTipoDeNavio(navio) == 4) {
      tabuleiro.qtdNaviosSubmarine++;
      for(k=0; k<TAM_SUBMARINE; k++) {
        tabuleiro.posicoes[y][x] = 'X';
        y++;
      }
    }else if(RetornaTipoDeNavio(navio) == 5) {
      tabuleiro.qtdNaviosDestroyer++;
      for(k=0; k<TAM_DESTROYER; k++) {
        tabuleiro.posicoes[y][x] = 'X';
        y++;
      }
    }
  }
  return tabuleiro;
}

tJogador InicializaJogador (tJogador jogador, int id, tTabuleiro tabuleiro) {
  jogador.id = id;
  jogador = LeNomeJogador(jogador);
  jogador.tabuleiro = tabuleiro;
  return jogador;
}

tJogador LeNomeJogador(tJogador jogador) {
  printf("Nome do jogador %d:\n", jogador.id);
  scanf("%[^\n]%*c", jogador.nome);
  return jogador;
}

tTabuleiro InvalidaTabuleiro (tTabuleiro tabuleiro) {
  tabuleiro.valido = 0;

  return tabuleiro;
}

tTabuleiro ValidaTabuleiro (tTabuleiro tabuleiro) {
  tabuleiro.valido = 1;

  return tabuleiro;
}

void ImprimeTabuleiroDecodificado (tTabuleiro tabuleiro) {
  int x=0, y=0;

  for(y=0; y<TAM_Y_TABU; y++) {
    for(x=0; x<TAM_X_TABU; x++) {
      if(x==TAM_X_TABU-1) printf("%c\n", tabuleiro.posicoes[y][x]);
      else printf("%c ", tabuleiro.posicoes[y][x]);
    }
  }
}

int PegaNumCharTransformaNumInt (char c) {
  if(c==48) return 0;
  else if(c==49) return 1;
  else if(c==50) return 2;
  else if(c==51) return 3;
  else if(c==52) return 4;
  else if(c==53) return 5;
  else if(c==54) return 6;
  else if(c==55) return 7;
  else if(c==56) return 8;
  else if(c==57) return 9;
  else return -1;
}

void TransformaLinhaTabuleiroEmStringFormatada (char *linha, tTabuleiro tabuleiro, int indiceLinha) {
  int y=0, x=0, i=0;

  for(x=0; x<TAM_X_TABU; x++) {
    if(x+1 == TAM_X_TABU) {
      linha[i] = tabuleiro.posicoes[indiceLinha][x];
      i++;
      linha[i] = '\0';
    }else {
      linha[i] = tabuleiro.posicoes[indiceLinha][x];
      i++;
      linha[i] = ' ';
      i++;
    }
  }
}