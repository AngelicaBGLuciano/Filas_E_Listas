# Filas_E_Listas
Atividade proposta na disciplina Estrutura de Dados 1, na Universidade Tecnologica Federal do Paraná campus Apucarana.
O código implementa um programa que realiza operações em estruturas de dados como lista encadeada, fila dinâmica e fila estática. Ele contém as seguintes funcionalidades:

1. Lista Encadeada:
   - `inicializaLista`: Inicializa uma lista encadeada.
   - `insereElemento`: Insere um elemento ordenado na lista.
   - `removeElemento`: Remove um elemento da lista pelo nome.
   - `buscaElemento`: Busca um elemento na lista pelo nome.
   - `destroiLista`: Libera a memória alocada para a lista.
   - `preencheLista`: Lê um arquivo CSV com dados de cadastro e preenche a lista.
   - `imprimeLista`: Imprime os elementos da lista.

2. Fila Dinâmica:
   - `inicializaFilaDinamica`: Inicializa uma fila dinâmica.
   - `filaDinamicaEstaVazia`: Verifica se a fila dinâmica está vazia.
   - `insereElementoFilaDinamica`: Insere um elemento no final da fila dinâmica.
   - `imprimeFilaDinamica`: Imprime os elementos da fila dinâmica.
   - `destroiFilaDinamica`: Libera a memória alocada para a fila dinâmica.
   - `removeElementoFilaDinamica`: Remove o primeiro elemento da fila dinâmica.

3. Fila Estática:
   - `inicializaFilaEstatica`: Inicializa uma fila estática.
   - `filaEstaticaEstaVazia`: Verifica se a fila estática está vazia.
   - `filaEstaticaEstaCheia`: Verifica se a fila estática está cheia.
   - `tamanhoFilaEstatica`: Retorna o número de elementos na fila estática.
   - `insereElementoFilaEstatica`: Insere um elemento no final da fila estática.
   - `removeElementoFilaEstatica`: Remove o primeiro elemento da fila estática.

O código também possui funções auxiliares, como `calcularIdade`, que calcula a idade com base em uma data de nascimento, e `salvaLista`, que salva os dados da lista em um arquivo CSV.

O programa utiliza as estruturas de dados para realizar operações de cadastro, busca, remoção e alteração de informações, além de preenchimento e impressão das filas dinâmicas e estáticas.

O objetivo principal do programa é simular o atendimento em um restaurante, com filas separadas para pessoas com diferentes condições de saúde. O programa lê arquivos com dados de clientes, insere esses clientes em uma lista encadeada ordenada por nome e, em seguida, preenche as filas dinâmicas e estáticas de acordo com as condições de saúde dos clientes e sua idade.
