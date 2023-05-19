#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#define N 15

//estruturas para lista
typedef struct Cadastro {
    char nome[50];
    char dataNasc[11];
    char condicaoSaude;
    struct Cadastro *proximo;
    struct Cadastro *anterior;
} Cadastro;

typedef struct Lista {
    Cadastro *inicio;
} Lista;

//estruturas para fila dinamica
typedef struct cliente {
    char nome[50];
    struct cliente* proximo;
} Cliente;

typedef struct {
    Cliente* inicio;
    Cliente* fim;
} FilaDinamica;

//estruturas para fila estaticas
typedef struct pessoa{
    char nome[50];
}Pessoa;

typedef struct filaEstatica{
    Pessoa pessoas[N];
    int inicio;
    int final;
    int qtdElem;
}FilaEstatica;

//estrutura para o restaurante
typedef struct restaurante{
  FilaEstatica filaDiabeticos;
  FilaEstatica filaHipertensos;
  FilaEstatica filaNormal;
  int atendimentosDiabeticos;
  int atendimentosHipertensos;
  int atendimentosNormais;
}Restaurante;

//funções para lista

void inicializaLista(Lista *lista) {
    lista->inicio = NULL;
}

Cadastro *criaCadastro(char* nome, char* dataNasc, char condicaoSaude) {
    Cadastro *novo = (Cadastro *)malloc(sizeof(Cadastro));
    strcpy(novo->nome, nome);
    strcpy(novo->dataNasc, dataNasc);
    novo->condicaoSaude = condicaoSaude;
    novo->proximo = NULL;
    novo->anterior = NULL;
    return novo;
}

bool listaEstaVazia(Lista* lista){
    return(lista->inicio==NULL);
}

bool insereElemento(Lista* lista, char* nome, char* dataNasc, char condicaoSaude) {
    Cadastro *novo = (Cadastro*)malloc(sizeof(Cadastro));
    if (novo == NULL) {
        printf("Erro ao alocar memória.\n");
        return false;
    }

    strcpy(novo->nome, nome);
    strcpy(novo->dataNasc, dataNasc);
    novo->condicaoSaude = condicaoSaude;
    novo->proximo = NULL;
    novo->anterior = NULL;

    Cadastro *atual = lista->inicio;
    Cadastro *anterior = NULL;

    // Percorrer a lista até encontrar a posição correta de inserção
    while (atual != NULL && strcasecmp(atual->nome, nome) < 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    // Inserir o novo elemento na posição correta
    if (anterior == NULL) {
        // Inserir no início da lista
        novo->proximo = lista->inicio;
        if (lista->inicio != NULL) {
            lista->inicio->anterior = novo;
        }
        lista->inicio = novo;
    } else {
        // Inserir no meio ou final da lista
        novo->proximo = atual;
        novo->anterior = anterior;
        anterior->proximo = novo;
        if (atual != NULL) {
            atual->anterior = novo;
        }
    }

    return true;
}

void removeElemento(Lista *lista, char* nome) {
    if (lista->inicio == NULL) {
        printf("Lista vazia. Nenhum elemento para remover.\n");
        return;
    }

    Cadastro *atual = lista->inicio;

    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) {
            if (atual->anterior == NULL) {
                lista->inicio = atual->proximo;
            } else {
                atual->anterior->proximo = atual->proximo;
            }
            if (atual->proximo != NULL) {
                atual->proximo->anterior = atual->anterior;
            }
            free(atual);
            return;
        }
        atual = atual->proximo;
    }

    printf("Elemento não encontrado.\n");
}

Cadastro *buscaElemento(Lista *lista, char* nome) {
    Cadastro *atual = lista->inicio;
    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}

void destroiLista(Lista *lista) {
    Cadastro *atual = lista->inicio;
    while (atual != NULL) {
        Cadastro *prox = atual->proximo;
        free(atual);
        atual = prox;
    }
    lista->inicio = NULL;
}

void preencheLista(Lista *lista, char* nomeArquivo) {
    char nome[50], dataNasc[11], condicaoSaude;
    char linha[100]; // Tamanho suficiente para armazenar uma linha do arquivo
    FILE *arqCadastro = fopen(nomeArquivo, "r");
    if(arqCadastro==NULL){
      printf("Erro ao abrir o arquivo DadosClientes.csv!\n");
      exit(1);
    }
    // Pular a primeira linha (Nome,Nascimento,Saude)
    fgets(linha, sizeof(linha), arqCadastro);

    while (fscanf(arqCadastro, "%[^,],%[^,],%c\n", nome, dataNasc, &condicaoSaude) == 3) {
        insereElemento(lista, nome, dataNasc, condicaoSaude);
    }
    fclose(arqCadastro);
}
void imprimeLista(Lista *lista) {
    Cadastro *atual = lista->inicio;

    if (atual == NULL) {
        printf("A lista está vazia.\n");
        return;
    }

    printf("Elementos da lista:\n");
    while (atual != NULL) {
        printf("Nome: %s\n", atual->nome);
        printf("Data de Nascimento: %s\n", atual->dataNasc);
        printf("Condicao de Saude: %c\n", atual->condicaoSaude);
        printf("---------------------\n");

        atual = atual->proximo;
    }
}

void alteraCadastro(Lista *lista, char* nome) {
    Cadastro *cadastro = buscaElemento(lista, nome);

    if (cadastro == NULL) {
        printf("Cadastro não encontrado.\n");
        return;
    }

    int opcao;
    char novoNome[50];
    char novaData[11];
    char novaCondicao;

    printf("Qual informação deseja alterar:\n");
    printf("1. Nome\n");
    printf("2. Data de Nascimento\n");
    printf("3. Condição de Saúde\n");
    printf("4. Cancelar\n");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            printf("Digite o novo nome: ");
            fgets(novoNome, 50,stdin);
            novoNome[strcspn(novoNome,"\n")]='\0';
            // Remover o cadastro com o nome antigo
            removeElemento(lista, nome);

            // Inserir novamente na posição correta com o novo nome, para manter ordem alfabetica
            insereElemento(lista, novoNome, cadastro->dataNasc, cadastro->condicaoSaude);

            printf("Nome alterado com sucesso.\n");
            break;
        case 2:
            printf("Digite a nova data de nascimento: ");
            fgets(novaData, 11,stdin);
            novaData[strcspn(novaData,"\n")]='\0';

            strcpy(cadastro->dataNasc, novaData);
            printf("Data de nascimento alterada com sucesso.\n");
            break;
        case 3:
            do {
              printf("Digite a nova condição de saúde: ");
              scanf(" %c", &novaCondicao);
              fflush(stdin);
              novaCondicao = toupper(novaCondicao);
            } while(novaCondicao!='H' && novaCondicao!='D' && novaCondicao!='N');
            cadastro->condicaoSaude = novaCondicao;
            printf("Condição de saúde alterada com sucesso.\n");
            break;
        case 4:
            printf("Operação cancelada.\n");
            break;
        default:
            printf("Opção inválida.\n");
            break;
    }
}

//funcao para salvar a lista de cadastro atualizada no arquivo .csv
void salvaLista(Lista *lista, const char* nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo DadosClientes.txt para escrita.\n");
        exit(1);
    }//if 1
    if (listaEstaVazia(lista)) {
        printf("Lista vazia. Nenhum dado para salvar.\n");
        return;
    }//if 2

    Cadastro *atual = lista->inicio;
    fprintf(arquivo, "Nome,Nascimento,Saúde\n");
    while (atual != NULL) {
        fprintf(arquivo, "%s,%s,%c\n", atual->nome, atual->dataNasc, atual->condicaoSaude);
        atual = atual->proximo;
    }//while
    fclose(arquivo);
}

//funções para fila dinamica

void inicializaFilaDinamica(FilaDinamica* fila) {
    fila->inicio = NULL;
    fila->fim = NULL;
}

bool filaDinamicaEstaVazia(FilaDinamica* fila) {
    return fila->inicio == NULL;
}

void insereElementoFilaDinamica(FilaDinamica* fila, char nome[]) {
    Cliente* novo = (Cliente*)malloc(sizeof(Cliente));
    strcpy(novo->nome, nome);
    novo->proximo = NULL;

    if (filaDinamicaEstaVazia(fila)) {
        fila->inicio = novo;
        fila->fim = novo;
    } else {
        fila->fim->proximo = novo;
        fila->fim = novo;
    }
}

void imprimeFilaDinamica(FilaDinamica* fila) {
    Cliente* atual = fila->inicio;
    while (atual != NULL) {
        printf("%s\n", atual->nome);
        atual = atual->proximo;
    }
}

void destroiFilaDinamica(FilaDinamica* fila) {
    Cliente* atual = fila->inicio;
    while (atual != NULL) {
        Cliente* proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    fila->inicio = NULL;
    fila->fim = NULL;
}

void removeElementoFilaDinamica(FilaDinamica* fila) {
    if (filaDinamicaEstaVazia(fila)) {
        printf("Fila dinâmica está vazia.\n");
        return;
    }

    Cliente* removido = fila->inicio;
    fila->inicio = fila->inicio->proximo;
    free(removido);

    if (fila->inicio == NULL) {
        fila->fim = NULL;
    }
}

//função para calcular a Idade
int calcularIdade(const char* dataNascimento) {
    time_t t = time(NULL);
    struct tm* currentTime = localtime(&t);

    int currentYear = currentTime->tm_year + 1900;
    int currentMonth = currentTime->tm_mon + 1;
    int currentDay = currentTime->tm_mday;

    int birthYear, birthMonth, birthDay;
    sscanf(dataNascimento, "%d/%d/%d", &birthDay, &birthMonth, &birthYear);

    int idade = currentYear - birthYear;

    if (currentMonth < birthMonth) {
        idade--;
    } else if (currentMonth == birthMonth && currentDay < birthDay) {
        idade--;
    }

    return idade;
}

void preencheFilaDinamica(FilaDinamica* fila, const char* nomeArquivo, Lista* lista) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    char nome[50];

    while (fgets(nome, sizeof(nome), arquivo) != NULL) {
        // Remover o caractere de nova linha do final da string
        nome[strcspn(nome, "\n")] = '\0';

        //verificar se a pessoa tem cadastro previo
        Cadastro* cadastro = buscaElemento(lista, nome);
        if (cadastro != NULL && calcularIdade(cadastro->dataNasc) > 12) {
            insereElementoFilaDinamica(fila, nome);
        }
    }

    fclose(arquivo);
}

// funções para as filas estaticas
void inicializaFilaEstatica(FilaEstatica* fila) {
    fila->inicio = fila->final = -1;
    fila->qtdElem = 0;
}

bool filaEstaticaEstaVazia(FilaEstatica* fila) {
    return (fila->qtdElem==0);
}

bool filaEstaticaEstaCheia(FilaEstatica* fila) {
    return (fila->qtdElem==N);
}

int tamanhoFilaEstatica(FilaEstatica* fila) {
    return (fila->qtdElem) ;
}

bool insereElementoFilaEstatica(FilaEstatica* fila, char* nome) {
    if (filaEstaticaEstaCheia(fila)) {
        return false;
    }

    if (filaEstaticaEstaVazia(fila)){
        fila->inicio = fila->final = 0;
    }else{
      fila->final=(fila->final+1)%N;
    }

    strcpy(fila->pessoas[fila->final].nome, nome);
    fila->qtdElem++;
    return true;
}

bool removeElementoFilaEstatica(FilaEstatica* fila) {
    if (filaEstaticaEstaVazia(fila)) {
        printf("Erro: A fila está vazia.\n");
        return false;
    }

    if (fila->inicio == fila->final) {
        fila->inicio = -1;
        fila->final = -1;
        fila->qtdElem=0;
    } else {
        fila->qtdElem--;
        fila->inicio = (fila->inicio + 1) % N;
    }

    return true;
}

void imprimeFilaEstatica(FilaEstatica* fila) {
    int indice = fila->inicio;
    if (filaEstaticaEstaVazia(fila)) {
        printf("Fila está vazia.\n");
        return;
    }

    for (int i = 0; i < tamanhoFilaEstatica(fila); i++) {
        printf(" %s\n", fila->pessoas[indice].nome);
        indice = (indice+1)%N;
    }
    printf("\n");
}

//inicializar as filas do buffet
void inicializaRestaurante(Restaurante* restaurante){
  inicializaFilaEstatica(&restaurante->filaDiabeticos);
  restaurante->atendimentosDiabeticos=0;
  inicializaFilaEstatica(&restaurante->filaHipertensos);
  restaurante->atendimentosHipertensos=0;
  inicializaFilaEstatica(&restaurante->filaNormal);
  restaurante->atendimentosNormais=0;
}

//função  que separa os clientes em 3 filas, e retira o cliente da fila de espera
void entrarNoRestaurante(FilaDinamica* fila, Lista* lista, Restaurante* restaurante){
  char* nome = fila->inicio->nome;
  //inserir cliente na fila apropriada
  Cadastro* cadastro = buscaElemento(lista, nome);
  if(cadastro!=NULL){
      if(cadastro->condicaoSaude=='D'){
        insereElementoFilaEstatica(&restaurante->filaDiabeticos, nome);
        restaurante->atendimentosDiabeticos++;
      }else if(cadastro->condicaoSaude=='H'){
        insereElementoFilaEstatica(&restaurante->filaHipertensos, nome);
        restaurante->atendimentosHipertensos++;
      }else if(cadastro->condicaoSaude=='N'){
        insereElementoFilaEstatica(&restaurante->filaNormal, nome);
        restaurante->atendimentosNormais++;
      }
      //remover o primeiro cliente da fila
      removeElementoFilaDinamica(fila);
  }//if
  return;
}

//função sair do restaurante
void sairDoRestaurante(Restaurante* restaurante){
    //remover todos os elementos que estao nas filas estaticas
    while(!filaEstaticaEstaVazia(&restaurante->filaDiabeticos)){
      removeElementoFilaEstatica(&restaurante->filaDiabeticos);
    }
    while(!filaEstaticaEstaVazia(&restaurante->filaHipertensos)){
      removeElementoFilaEstatica(&restaurante->filaHipertensos);
    }
    while(!filaEstaticaEstaVazia(&restaurante->filaNormal)){
      removeElementoFilaEstatica(&restaurante->filaNormal);
    }
    return;
}
//relatorio
void gerarRelatorio(Restaurante* restaurante){
  int totalDeEntrada, totalDespesas, lucro;
  int entradaH, entradaD, entradaN;
  int custosH, custosD, custosN;

  int totalDeAtendimentos;

  totalDeAtendimentos = restaurante->atendimentosHipertensos + restaurante->atendimentosDiabeticos +
                        restaurante->atendimentosNormais;

  //calculando o valor das entradas
  entradaH = 50 * restaurante->atendimentosHipertensos;
  entradaD = 55 * restaurante->atendimentosDiabeticos;
  entradaN = 40 * restaurante->atendimentosNormais;
  totalDeEntrada = entradaH + entradaD + entradaN;

  //calculando despesas
  custosH = 30 * restaurante->atendimentosHipertensos;
  custosD = 35 * restaurante->atendimentosDiabeticos;
  custosN = 25 * restaurante->atendimentosNormais;
  totalDespesas = custosH + custosD + custosN;

  //calculando lucro
  lucro = totalDeEntrada - totalDespesas;

   //gerando nome do arquivo
  time_t t = time(NULL);
    struct tm* dataHora = localtime(&t);

    char nomeArquivo[50];
    snprintf(nomeArquivo, sizeof(nomeArquivo), "Relatorio_%02d_%02d_%04d.txt",
             dataHora->tm_mday, dataHora->tm_mon + 1, dataHora->tm_year + 1900);

    FILE* relatorio = fopen(nomeArquivo, "w");
    if (relatorio == NULL) {
        printf("Erro ao criar o arquivo.\n");
        return;
    }
    //escrevendo relatorio
    fprintf(relatorio, "Relatorio do dia %02d/%02d/%04d\n",
            dataHora->tm_mday, dataHora->tm_mon + 1, dataHora->tm_year + 1900);
    fprintf(relatorio, "Total de Atendimentos: %d\n\n", totalDeAtendimentos);
    fprintf(relatorio, "Hipertensos: %d\n", restaurante->atendimentosHipertensos);
    fprintf(relatorio, "Diabeticos: %d\n", restaurante->atendimentosDiabeticos);
    fprintf(relatorio, "Saudaveis: %d\n\n", restaurante->atendimentosNormais);
    fprintf(relatorio, "Total de entrada (R$): %d (%d + %d + %d)\n", totalDeEntrada,
            entradaH,entradaD, entradaN);
    fprintf(relatorio, "Total de custos de despesas (R$): %d (%d + %d + %d)\n", totalDespesas,
            custosH,custosD, custosN);
    fprintf(relatorio, "Lucro (R$): %d\n\n", lucro);
  fclose(relatorio);
}

//menus
void menuAdministrador(Lista* lista, FilaDinamica* fila, Restaurante* restaurante){
      int opcao;
      do {
        printf("\n\n==========Administrador==========\n\n");
        printf("(1) Montar fila de espera\n");
        printf("(2) Proximo a ir ao buffet\n");
        printf("(3) Sair do restaurante\n");
        printf("(4) Imprimir fila D\n");
        printf("(5) Imprimir fila H\n");
        printf("(6) Imprimir fila N\n");
        printf("(7) Imprimir fila de Espera\n");
        printf("(8) Imprimir todas as filas\n");
        printf("(9) Sair\n\n");
        scanf("%d", &opcao);
        switch (opcao) {
          case 1:
            preencheFilaDinamica(fila, "DadosChegada.txt",lista);
            if(filaDinamicaEstaVazia(fila)==false){
              printf("\nFila de espera montada\n");
            }
          break;
          case 2:
            if(filaDinamicaEstaVazia(fila)){//conferindo se tem clientes na fila de espera
              printf("\nFila de espera vazia\n");
            }else{
              if(filaEstaticaEstaVazia(&restaurante->filaNormal)&&filaEstaticaEstaVazia(&restaurante->filaHipertensos)
                &&filaEstaticaEstaVazia(&restaurante->filaDiabeticos)){//conferindo se os clientes anteriores sairam do restaurante
                  for(int i=0; i<15; i++){
                    if(filaDinamicaEstaVazia(fila)==true){//se tiver <15 clientes na fila de espera
                      printf("\nClientes entraram no restaurante\n");
                      break;
                    }//if
                    entrarNoRestaurante(fila, lista, restaurante);//entrar na fila do buffet certa
                  }//fot
                  printf("\nClientes entraram no restaurante\n");
                }else{
                  printf("\n\nO restaurante esta com alotacao maxima\n\n");
                }//if interno
            }//if externo
          break;
          case 3:
              sairDoRestaurante(restaurante);//tirar todos os clientes das filas de buffet
              printf("\n\nRestaurante vazio\n");
          break;
          case 4:
              printf("\n\n-----Fila de Diabeticos-----\n");
              imprimeFilaEstatica(&restaurante->filaDiabeticos);
          break;
          case 5:
              printf("\n\n-----Fila de Hipertensos-----\n");
              imprimeFilaEstatica(&restaurante->filaHipertensos);
          break;
          case 6:
              printf("\n\n-----Fila Saudavel-----\n");
              imprimeFilaEstatica(&restaurante->filaNormal);
          break;
          case 7:
              printf("\n\n-----Fila de Espera-----\n");
              imprimeFilaDinamica(fila);
          break;
          case 8:
            printf("\n\n-----Fila de Diabeticos-----\n");
            imprimeFilaEstatica(&restaurante->filaDiabeticos);
            printf("\n\n-----Fila de Hipertensos-----\n");
            imprimeFilaEstatica(&restaurante->filaHipertensos);
            printf("\n\n-----Fila Saudavel-----\n");
            imprimeFilaEstatica(&restaurante->filaNormal);
            printf("\n\n-----Fila de Espera-----\n");
            imprimeFilaDinamica(fila);
          break;
          case 9:
            printf("Saindo...\n");
          break;
          default:
            printf("Opcao invalida!\n");
          break;
        }
      } while(opcao!=9);
}

void menuCliente(Lista *l){
    int opcao;
    char nome[50], data[11], saude;
    Cadastro *aux = NULL;
    do{
      printf("\n\n==========Cliente==========\n\n");
      printf("(1) Realizar um novo cadastro\n");
      printf("(2) Buscar cadastro\n");
      printf("(3) Alterar dados do cadastro\n");
      printf("(4) Sair\n");
      printf("Digite a opcao: ");
      scanf("%d", &opcao);
      fflush(stdin);
      switch (opcao) {
        case 1:
        printf("\nDigite seu nome completo: ");
        fgets(nome, 50,stdin);
        nome[strcspn(nome,"\n")]='\0';
        printf("\nDigite sua data de nascimento (dd/mm/aaaa): ");
        fgets(data, 11,stdin);
        data[strcspn(data,"\n")]='\0';
        setbuf(stdin,NULL);
        do {
          printf("\nDigite a condicao de saude: ");
          printf("\nOBS: (H) Hipertenso / (D) Diabetico / (N) Normal\n");
          scanf("%c", &saude);
          fflush(stdin);
          saude=toupper(saude);
        } while(saude!='H' && saude!='D' && saude!='N');
        if(insereElemento(l,nome, data, saude)==true) printf("\nCliente cadastrado\n");
        break;
        case 2:
        printf("\nDigite seu nome completo: ");
        fgets(nome, 50,stdin);
        nome[strcspn(nome,"\n")]='\0';
        printf("\n\nEstamos verificando nosso sistema...\n");
        aux=buscaElemento(l, nome);
        if(aux==NULL){
           printf("\nVoce ainda nao tem cadastro\n");
        }else{
          printf("\n\nNome: %s\n", aux->nome);
          printf("Nasc: %s\n", aux->dataNasc);
          printf("Condicao: %c\n", aux->condicaoSaude);
        }
        break;
        case 3:
        printf("\nDigite seu nome completo: ");
        fgets(nome, 50,stdin);
        nome[strcspn(nome,"\n")]='\0';
        printf("\n\nEstamos verificando nosso sistema...\n");
        aux=buscaElemento(l, nome);
        if(aux==NULL){
           printf("\nVoce ainda nao tem cadastro\n");
        }else{
          printf("\n\nNome: %s\n", aux->nome);
          printf("Nasc: %s\n", aux->dataNasc);
          printf("Condicao: %c\n", aux->condicaoSaude);
          alteraCadastro(l, nome);
        }
        break;
        case 4:
          printf("Saindo...\n");
        break;
        default:
          printf("Opcao inalida!\n");
        break;
      }
    }while(opcao!=4);
}

void menuPrincipal(Lista *l, FilaDinamica* fila, Restaurante* restaurante){
  int opcao;
    do {
      printf("\n\n==========Menu==========\n\n");
      printf("(1) Cliente\n");
      printf("(2) Administrador\n");
      printf("(3) Sair\n");
      printf("Digite uma opcao: \n");
      scanf("%d", &opcao);
      fflush(stdin);
      switch (opcao) {
        case 1:
          menuCliente(l);
        break;
        case 2:
          menuAdministrador(l,fila,restaurante);
        break;
        case 3:
          printf("\nSaindo...\n");
        break;
        default:
          printf("Opcao invalida!\n");
        break;
      }
    } while(opcao!=3);

    return;
}
int main(){
  Lista lista;
  inicializaLista(&lista);
  preencheLista(&lista, "DadosClientes.csv");

  FilaDinamica fila;
  inicializaFilaDinamica(&fila);

  Restaurante restaurante;
  //inicializando  restaurante
  inicializaRestaurante(&restaurante);


  //menu principal
  menuPrincipal(&lista, &fila, &restaurante);

  //escrever arquivo do relatorio
  gerarRelatorio(&restaurante);

  //salvando o arquivo com os dados alterados
  salvaLista(&lista, "DadosClientes.csv");

  //desalocando memoria
  destroiFilaDinamica(&fila);
  destroiLista(&lista);
  return 0;
}
