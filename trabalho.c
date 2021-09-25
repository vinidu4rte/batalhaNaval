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
  int qtdTamAtacado;
  int foiDestruido;
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
  int resultadoJogadasAtaque[100]; // id navio ou 0 = agua
  int resultadoJogadasDefesa[100]; // id navio ou 0 = agua
  int valido;
  int compativel;
  int qtdNaviosDestruidos;
  int perdeu;
}tTabuleiro;

typedef struct {
  char nome[17];
  int id;
  tTabuleiro tabuleiro;
  tArquivoInicializacao arquivo;
  char jogadasY[100];
  int jogadasYNum[100];
  int jogadasX[100];
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
void ImprimeTabuleiroDecodificado (tJogador jogador, tTabuleiro tabuleiro);
void ImprimeTabuleiroCodificado (tJogador jogador, tTabuleiro tabuleiro, int qtdJogadas);
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
tNavio RetornaNavioDestruido (tNavio navio);
int VerificarTabuleirosValidosECompartiveis(tTabuleiro tabuleiro1, tTabuleiro tabuleiro2);
tNavio RetornaNavioParteDestruida (tNavio navio);
int PegaJogadaLetraETransfEmNum (char jogadaY);
tJogador RecebeJogada (tJogador jogador, int qtdJogadasFeitas);
int VerificaSeNavioFoiDestruido (tNavio navio);
int RetornaTamanhoNavio (tNavio navio);
int RetornaOrientacaoNavio (tNavio navio);
int RetornaPosicaoYNavio (tNavio navio);
int RetornaPosicaoXNavio (tNavio navio);
int VerificaPosicaoAtacada (tTabuleiro tabuleiro, int posicaoY, int posicaoX);
tTabuleiro RetornaTabuleiroAposAtaque (tTabuleiro tabuleiro, char posicaoY, int posicaoYAtaque, int posicaoXAtaque, int numJogada);
void RealizaJogo (tJogo jogo, tJogador jogador1, tJogador jogador2);
tJogo RealizaJogada (tJogo jogo ,tJogador jogadorAtacante, tJogador jogadorAtacado, int numJogada);
int VerificaJogadorPerdedor (tTabuleiro jogador1, tTabuleiro jogador2);
int VerificaSeAmbosJogadoresPerderam (tTabuleiro jogador1, tTabuleiro jogador2);
int VerificaSeAlgumJogadorPerdeu (tTabuleiro jogador1, tTabuleiro jogador2);
tTabuleiro AtribuiResultadoJogadaAtaque (tTabuleiro tabuleiro, int id, int numJogada);
tTabuleiro RealizaJogadaDefesa (tJogador jogadorAtacante, tJogador jogadorAtacado, int numJogada);
tTabuleiro RealizaJogadaAtaque (tJogador jogadorAtacante, tJogador jogadorAtacado, int numJogada);
int RetornaIdNavio (tNavio navio);
int RetornaResultadoJogada (tTabuleiro tabuleiro, int numJogada);
int JogadaEhValida (char jogadaY, int jogadaX, tJogador jogador, int qtdJogadasFeitas);
tTabuleiro IncrementaQtdNaviosDestruidos (tTabuleiro tabuleiro);
tTabuleiro DeclaraTabuleiroPerdedor (tTabuleiro tabuleiro);
void CriaArquivoResumoJogo (char *diretorio_saida, tJogador jogador1, tJogador jogador2, tTabuleiro tabuleiro1, tTabuleiro tabuleiro2);


int main (int argc, char **argv) {
  tJogo jogo;
  tJogador jogador1, jogador2;
  tTabuleiro tabuleiro1, tabuleiro2;
  tArquivoInicializacao arquivo1, arquivo2;
  
  int rtn=0, qtdNavios=0;
  arquivo1 = RotinaLeituraInicializacao(arquivo1, NOME_ARQUIVO_1, argv, argc);
  tabuleiro1 = RotinaLerNaviosValidarEGerarTabuleiro(arquivo1);

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
  
  if(VerificarTabuleirosValidosECompartiveis(tabuleiro1, tabuleiro2)) {
    jogador1 = InicializaJogador(jogador1, 1, tabuleiro1);
    jogador2 = InicializaJogador(jogador2, 2, tabuleiro2);
    printf("\n");
    CriaArquivoInicializacao(arquivo1.diretorioSaida, NOME_ARQUIVO_SAIDA_INIC, jogador1, jogador2);
    RealizaJogo(jogo, jogador1, jogador2);
  }
  return 0;
}

void RealizaJogo (tJogo jogo, tJogador jogador1, tJogador jogador2) {
  int qtdJogadasFeitas1=0, qtdJogadasFeitas2=0, perdedor=0;
  while(VerificaSeAlgumJogadorPerdeu(jogador1.tabuleiro, jogador2.tabuleiro)==0) {
    jogador1 = RecebeJogada(jogador1, qtdJogadasFeitas1);
    jogador2.tabuleiro = RealizaJogadaDefesa(jogador1, jogador2, qtdJogadasFeitas1);
    jogador1.tabuleiro = RealizaJogadaAtaque(jogador1, jogador2, qtdJogadasFeitas1);
    qtdJogadasFeitas1++;
    printf("Tabuleiro atual de %s apos a jogada de %s\n",jogador2.nome, jogador1.nome);
    ImprimeTabuleiroCodificado(jogador1, jogador2.tabuleiro, qtdJogadasFeitas1);

    jogador2 = RecebeJogada(jogador2, qtdJogadasFeitas2);
    jogador1.tabuleiro = RealizaJogadaDefesa(jogador2, jogador1, qtdJogadasFeitas2);
    jogador2.tabuleiro = RealizaJogadaAtaque(jogador2, jogador1, qtdJogadasFeitas2);
    qtdJogadasFeitas2++;
    printf("Tabuleiro atual de %s apos a jogada de %s\n",jogador1.nome, jogador2.nome);
    ImprimeTabuleiroCodificado(jogador2, jogador1.tabuleiro, qtdJogadasFeitas2);
  }
 
  if(VerificaSeAmbosJogadoresPerderam(jogador1.tabuleiro, jogador2.tabuleiro)) {
    printf("Empate\n");
  }else {
    perdedor = VerificaJogadorPerdedor(jogador1.tabuleiro, jogador2.tabuleiro);
    if(perdedor==1) printf("Vencedor: %s\n", jogador2.nome);
    else if(perdedor==2) printf("Vencedor: %s\n", jogador1.nome);
  }
  //agora deve gerar o arquivo de saida, resultado e estatística
  
}

void ImprimeTabuleiroCodificado (tJogador jogador, tTabuleiro tabuleiro, int qtdJogadas) {
   int i=0, j=0, k=0, printou=0;
   int x=0, y=0;

  for(i=0; i<TAM_Y_TABU; i++) {
    for(j=0; j<TAM_X_TABU; j++) {
      for(k=0; k<qtdJogadas; k++) {
        if(jogador.jogadasYNum[k]-1 == i && jogador.jogadasX[k]-1 == j) {
          if(j+1==TAM_X_TABU) {
            printf("%c\n", tabuleiro.posicoes[i][j]);
            printou=1;
            break;
          }else printf("%c ", tabuleiro.posicoes[i][j]);
          printou=1;
          break;
        }
      }
      if(printou) printou=0;
      else {
        if(j+1==TAM_X_TABU) printf(".\n");
        else printf(". ");
      }
    }
  }
  printf("\n");
}


tJogador RecebeJogada (tJogador jogador, int qtdJogadasFeitas) {
  int rtn=0, jogadaX=0, i=0, j=0, temErro=0;
  char jogadaY;
  char jogada[10];
  printf("Jogada de %s:\n\n", jogador.nome);
  while(1) {
    scanf("%[^\n]%*c", jogada);
    rtn = sscanf(jogada, "%c%d", &jogadaY, &jogadaX);

    if(JogadaEhValida(jogadaY, jogadaX, jogador, qtdJogadasFeitas) && rtn == 2) {
      if(jogadaX<10) {
        if(jogada[2] == '\0') break;
        else {
        printf("%s:Jogada invalida! Jogue novamente %s:\n\n", jogada, jogador.nome);
        continue;
        }
      }else if(jogadaX >= 10) {
        if(jogada[3] == '\0') break;
        else {
          printf("%s:Jogada invalida! Jogue novamente %s:\n\n", jogada, jogador.nome);
          continue;
        }
      }
    }else {
      printf("%s:Jogada invalida! Jogue novamente %s:\n\n", jogada, jogador.nome);
      continue;
    }
  }
  jogador.jogadasX[qtdJogadasFeitas] = jogadaX;
  jogador.jogadasY[qtdJogadasFeitas] = jogadaY;
  jogador.jogadasYNum[qtdJogadasFeitas] = PegaJogadaLetraETransfEmNum(jogadaY);
  return jogador;
}

tTabuleiro RealizaJogadaDefesa (tJogador jogadorAtacante, tJogador jogadorAtacado, int numJogada) {
  int rtn=0;
  rtn = VerificaPosicaoAtacada(jogadorAtacado.tabuleiro, jogadorAtacante.jogadasYNum[numJogada], jogadorAtacante.jogadasX[numJogada]);
  if(rtn==1) {
    jogadorAtacado.tabuleiro = RetornaTabuleiroAposAtaque(jogadorAtacado.tabuleiro, jogadorAtacante.jogadasY[numJogada], jogadorAtacante.jogadasYNum[numJogada], jogadorAtacante.jogadasX[numJogada], numJogada);
  }
  return jogadorAtacado.tabuleiro;
}

tTabuleiro RealizaJogadaAtaque (tJogador jogadorAtacante, tJogador jogadorAtacado, int numJogada) {
  int rtn=0;
  rtn = VerificaPosicaoAtacada(jogadorAtacado.tabuleiro, jogadorAtacante.jogadasYNum[numJogada], jogadorAtacante.jogadasX[numJogada]);
  if(rtn==1){
    jogadorAtacante.tabuleiro = AtribuiResultadoJogadaAtaque(jogadorAtacante.tabuleiro, RetornaResultadoJogada(jogadorAtacado.tabuleiro, numJogada), numJogada);
  }else if(rtn==0) {
    jogadorAtacante.tabuleiro = AtribuiResultadoJogadaAtaque(jogadorAtacante.tabuleiro, 0, numJogada);
    printf("%c%d:Agua\n\n", jogadorAtacante.jogadasY[numJogada], jogadorAtacante.jogadasX[numJogada]);
  }
  return jogadorAtacante.tabuleiro;
}

tTabuleiro AtribuiResultadoJogadaDefesa (tTabuleiro tabuleiro, int id, int numJogada) {
  tabuleiro.resultadoJogadasDefesa[numJogada] = id;
  return tabuleiro;
}

tTabuleiro IncrementaQtdNaviosDestruidos (tTabuleiro tabuleiro) {
  tabuleiro.qtdNaviosDestruidos++;
  return tabuleiro;
}

tTabuleiro DeclaraTabuleiroPerdedor (tTabuleiro tabuleiro) {
  tabuleiro.perdeu=1;
  return tabuleiro;
}

tTabuleiro RetornaTabuleiroAposAtaque (tTabuleiro tabuleiro, char posicaoY, int posicaoYAtaque, int posicaoXAtaque, int numJogada) {
int i=0, j=0, m=0, n=0, x=0, y=0;
int navioX=0, navioY=0;

y = posicaoYAtaque - 1;
x = posicaoXAtaque - 1;
for(i=0; i<tabuleiro.qtdNaviosTotal; i++) {
  n=0;
  navioX = RetornaPosicaoXNavio(tabuleiro.navios[i])-1;
  navioY = RetornaPosicaoYNavio(tabuleiro.navios[i])-1;
  if(RetornaOrientacaoNavio(tabuleiro.navios[i])) { //horizontal
    if(navioY == y) {
    for(j=0; j<RetornaTamanhoNavio(tabuleiro.navios[i]); j++) {
      if(navioX+n == x) {
        tabuleiro.navios[i] = RetornaNavioParteDestruida(tabuleiro.navios[i]);
        tabuleiro = AtribuiResultadoJogadaDefesa(tabuleiro, RetornaIdNavio(tabuleiro.navios[i]), numJogada);
        if(VerificaSeNavioFoiDestruido(tabuleiro.navios[i])) {
          tabuleiro.navios[i] = RetornaNavioDestruido(tabuleiro.navios[i]);
          printf("%c%d:Afundou %s\n\n", posicaoY, posicaoXAtaque, tabuleiro.navios[i].nome);
          tabuleiro = IncrementaQtdNaviosDestruidos(tabuleiro);
          if(tabuleiro.qtdNaviosDestruidos == tabuleiro.qtdNaviosTotal) {
            tabuleiro = DeclaraTabuleiroPerdedor(tabuleiro);
          }
        }else {
          printf("%c%d:Tiro!\n\n", posicaoY, posicaoXAtaque);
        }
      }
      n++; 
    }
    }else continue;
  }else if(RetornaOrientacaoNavio(tabuleiro.navios[i]) == 0) { // vertical
    if(navioX == x) {
      for(j=0; j<RetornaTamanhoNavio(tabuleiro.navios[i]); j++) {
        if(navioY+n == y) {
          tabuleiro.navios[i] = RetornaNavioParteDestruida(tabuleiro.navios[i]);
          tabuleiro = AtribuiResultadoJogadaDefesa(tabuleiro, RetornaIdNavio(tabuleiro.navios[i]), numJogada);
          if(VerificaSeNavioFoiDestruido(tabuleiro.navios[i])) {
            tabuleiro.navios[i] = RetornaNavioDestruido(tabuleiro.navios[i]);
            printf("%c%d:Afundou %s\n\n", posicaoY, posicaoXAtaque, tabuleiro.navios[i].nome);
            tabuleiro = IncrementaQtdNaviosDestruidos(tabuleiro);
            if(tabuleiro.qtdNaviosDestruidos == tabuleiro.qtdNaviosTotal) {
              tabuleiro = DeclaraTabuleiroPerdedor(tabuleiro);
            }
          }else {
            printf("%c%d:Tiro!\n\n", posicaoY, posicaoXAtaque);
          }
        }
        n++; 
      }
    }else continue;
  }
}
  return tabuleiro;
}

int RetornaResultadoJogada (tTabuleiro tabuleiro, int numJogada) {
  int resultado=0;
  resultado = tabuleiro.resultadoJogadasDefesa[numJogada];
  return resultado;
}

tTabuleiro AtribuiResultadoJogadaAtaque (tTabuleiro tabuleiro, int id, int numJogada) {
  tabuleiro.resultadoJogadasAtaque[numJogada] = id;
  return tabuleiro;
}

int RetornaIdNavio (tNavio navio) {
  return navio.id;
}

int VerificaSeAmbosJogadoresPerderam (tTabuleiro jogador1, tTabuleiro jogador2) {
  if(jogador1.perdeu && jogador2.perdeu) return 1;
  else return 0;
}

int VerificaSeAlgumJogadorPerdeu (tTabuleiro jogador1, tTabuleiro jogador2) {
  if(jogador1.perdeu || jogador2.perdeu) return 1;
  else return 0;
}

int VerificaJogadorPerdedor (tTabuleiro jogador1, tTabuleiro jogador2) {
  if(jogador1.perdeu==1) return 1;
  else if (jogador2.perdeu==1) return 2;
  else return -1;
}

int VerificaPosicaoAtacada (tTabuleiro tabuleiro, int posicaoY, int posicaoX) {
  int x=0, y=0, i=0, j=0;
  y = posicaoY - 1;
  x = posicaoX - 1;
  if(tabuleiro.posicoes[y][x] == 'X') return 1;
  else if(tabuleiro.posicoes[y][x] == 'o') return 0;  // agua
  else return -1;
}

tNavio RetornaNavioDestruido (tNavio navio) {
  navio.foiDestruido=1;
  return navio;
}

int VerificaSeNavioFoiDestruido (tNavio navio) {
  if(navio.qtdTamAtacado == navio.tamanho) return 1;
  else return 0;
}

tNavio RetornaNavioParteDestruida (tNavio navio) {
  navio.qtdTamAtacado++;
  return navio;
}

int RetornaTamanhoNavio (tNavio navio) {
  return navio.tamanho;
}

int RetornaPosicaoYNavio (tNavio navio) {
  return navio.posicaoY;
}

int RetornaPosicaoXNavio (tNavio navio) {
  return navio.posicaoX;
}

int RetornaOrientacaoNavio (tNavio navio) {
  if(navio.orientacao==1) return 1;
  else return 0;
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
        if(fscanf(arquivo, "%[^\n]%*c", arquivoInic.conteudo[i])==1) {
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
  tabuleiro.qtdNaviosDestruidos=0;
  tabuleiro.perdeu=0;

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

int PegaJogadaLetraETransfEmNum (char jogadaY) {
  //a = 1, b=2, c=3, d=4, e=5, f=6, g=7, h=8, i=9, j=10;
  int jogadaYNum=0;
  if(jogadaY == 'a') jogadaYNum = 1;
  else if(jogadaY == 'b') jogadaYNum = 2;
  else if(jogadaY == 'c') jogadaYNum = 3;
  else if(jogadaY == 'd') jogadaYNum = 4;
  else if(jogadaY == 'e') jogadaYNum = 5;
  else if(jogadaY == 'f') jogadaYNum = 6;
  else if(jogadaY == 'g') jogadaYNum = 7;
  else if(jogadaY == 'h') jogadaYNum = 8;
  else if(jogadaY == 'i') jogadaYNum = 9;
  else if(jogadaY == 'j') jogadaYNum = 10;
  return jogadaYNum;
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
  printf("Nome do Jogador %d:\n", jogador.id);
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

void ImprimeTabuleiroDecodificado (tJogador jogador, tTabuleiro tabuleiro) {
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

int VerificarTabuleirosValidosECompartiveis(tTabuleiro tabuleiro1, tTabuleiro tabuleiro2) {
  if(tabuleiro1.valido && tabuleiro2.valido && tabuleiro1.compativel && tabuleiro2.compativel) return 1;
  else return 0;
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
  && tabu1.qtdNaviosTotal == tabu2.qtdNaviosTotal 
  && tabu1.qtdNaviosTotal>0 && tabu2.qtdNaviosTotal>0) {
    return 1;
  }else {
    return 0;
  }
}

int JogadaEhValida (char jogadaY, int jogadaX, tJogador jogador, int qtdJogadasFeitas) {
  int i=0, j=0;

  if(jogadaY >= 'a' && jogadaY <= 'j'  && jogadaX >= 1 && jogadaX <= 10) {
    for(i=0; i<qtdJogadasFeitas; i++) {
      if(jogador.jogadasY[i] == jogadaY && jogador.jogadasX[i] == jogadaX) {
        //printf("O jogador %d está repetindo a jogada atual com a jogada %d.\n", jogador.id, i+1);
        return 0;
      }
    }
  }else return 0;
  return 1;
}