# Projeto 6 — Algoritmo de Ford-Fulkerson

## 1. Descrição do projeto

Este projeto tem como objetivo implementar o algoritmo de **Ford-Fulkerson** em C++, para calcular o fluxo máximo entre uma origem e um destino em uma rede direcionada.

O problema é contextualizado em uma rede de distribuição de água, na qual existem tubulações que conectam diferentes regiões. Cada tubulação possui uma capacidade máxima de transporte, e o objetivo é descobrir a quantidade máxima de água que pode sair da estação de tratamento (origem) e chegar à região desejada (destino).

## 2. Tecnologias e restrições

- **Linguagem:** C++.
- **Bibliotecas:** utilização de bibliotecas básicas, sem bibliotecas prontas de grafos.
- **Estruturas de dados:** structs, ponteiros e listas encadeadas.
- **Entrada de dados:** realizada pelo teclado.
- **Execução do algoritmo:** implementação própria do Ford-Fulkerson.
- **Visualização:** apresentação textual do grafo diretamente no terminal.

O código será desenvolvido de maneira didática, com nomes significativos para variáveis e funções, facilitando a compreensão e a explicação durante a apresentação.

## 3. Modelagem do grafo

O problema será representado por um **grafo direcionado e ponderado**, no qual:

- **Vértices:** representam as estações ou regiões da rede.
- **Arestas:** representam as tubulações que conectam os vértices.
- **Capacidade:** representa o limite máximo de transporte de cada tubulação.
- **Fluxo:** representa a quantidade efetivamente transportada por uma tubulação.
- **Origem:** vértice de onde parte o fluxo.
- **Destino:** vértice que deve receber o fluxo.

Os vértices serão identificados por números inteiros, começando em `0`.

## 4. Estrutura de dados escolhida

Será utilizada uma **lista de adjacência com alocação dinâmica de memória**.

A estrutura será composta por:

### Struct Aresta

Armazena as informações de cada conexão:

- `destino`: vértice para o qual a conexão aponta.
- `capacidade`: capacidade máxima da conexão.
- `fluxo`: fluxo atual da conexão.
- `original`: identifica se a aresta pertence ao grafo original ou se é uma aresta reversa.
- `proxima`: ponteiro para a próxima aresta da lista.
- `reversa`: ponteiro para a aresta reversa correspondente.

### Struct Vertice

Armazena as informações de cada vértice:

- `codigo`: identificador do vértice.
- `grauEntrada`: quantidade de conexões originais que chegam ao vértice.
- `grauSaida`: quantidade de conexões originais que partem do vértice.
- `listaAdjacencia`: ponteiro para a lista de arestas do vértice.

### Representação geral

Será utilizado um vetor dinâmico de ponteiros para vértices:

`Vertice **grafo`

Cada posição do vetor representa um vértice e aponta para sua respectiva lista encadeada de conexões.

Essa estrutura permite cadastrar uma quantidade variável de vértices e arestas, sem estabelecer um limite fixo no código.

## 5. Arestas reversas e capacidade residual

Para implementar corretamente o Ford-Fulkerson, cada conexão original terá uma conexão reversa associada.

A conexão reversa começa com capacidade zero e fluxo zero.

Durante a execução do algoritmo, ela permite que o fluxo anteriormente enviado por uma conexão seja parcialmente cancelado e redirecionado por outro caminho, quando necessário.

A capacidade residual será calculada pela expressão:

`capacidade residual = capacidade - fluxo`

O algoritmo poderá percorrer uma aresta quando sua capacidade residual for maior que zero.

## 6. Funcionamento do algoritmo

Será utilizado o método **Ford-Fulkerson com busca em profundidade (DFS)** para encontrar caminhos aumentantes.

O funcionamento será dividido nas seguintes etapas:

1. Inicializar o fluxo de todas as conexões com zero.
2. Procurar um caminho entre a origem e o destino utilizando DFS.
3. Considerar somente conexões com capacidade residual maior que zero.
4. Identificar o gargalo do caminho, ou seja, a menor capacidade residual entre suas conexões.
5. Atualizar o fluxo das conexões do caminho encontrado.
6. Atualizar também os fluxos das respectivas arestas reversas.
7. Somar o valor do gargalo ao fluxo máximo acumulado.
8. Repetir o processo enquanto existir um caminho aumentante.
9. Quando não houver mais caminhos possíveis, apresentar o fluxo máximo encontrado.

## 7. Funções previstas

| Função                      | Responsabilidade                                                          |
| --------------------------- | ------------------------------------------------------------------------- |
| `cadastrarVertices()`       | Solicitar e cadastrar a quantidade de vértices.                           |
| `adicionarAresta()`         | Criar uma conexão original e sua respectiva aresta reversa.               |
| `cadastrarArestas()`        | Solicitar as conexões e suas capacidades.                                 |
| `definirOrigemDestino()`    | Definir os vértices de origem e destino.                                  |
| `buscarCaminhoDFS()`        | Procurar um caminho aumentante utilizando busca em profundidade.          |
| `calcularFluxoMaximo()`     | Executar o Ford-Fulkerson e calcular o fluxo máximo.                      |
| `mostrarGrafo()`            | Apresentar a representação do grafo e suas conexões.                      |
| `mostrarFluxosArestas()`    | Mostrar o fluxo utilizado e a capacidade de cada conexão.                 |
| `visualizarGrafoTerminal()` | Apresentar uma visualização textual das conexões diretamente no terminal. |
| `liberarMemoria()`          | Liberar a memória alocada dinamicamente.                                  |

## 8. Entrada de dados

O programa solicitará os dados nesta ordem:

1. Quantidade de vértices.
2. Quantidade de arestas.
3. Vértice de origem de cada conexão.
4. Vértice de destino de cada conexão.
5. Capacidade de cada conexão.
6. Vértice de origem do fluxo.
7. Vértice de destino do fluxo.

Serão realizadas verificações para evitar entradas inválidas, como vértices fora do intervalo cadastrado, origem igual ao destino e capacidades negativas.

Será permitido cadastrar conexões com capacidade zero.

## 9. Resultados apresentados

Ao finalizar a execução, o programa deverá apresentar:

- Vértice de origem.
- Vértice de destino.
- Fluxo máximo encontrado.
- Fluxo utilizado em cada conexão.
- Capacidade máxima de cada conexão.
- Identificação das conexões saturadas.
- Capacidade residual disponível em cada conexão.

A diferença entre capacidade e fluxo será destacada:

- **Capacidade:** limite máximo que uma tubulação pode transportar.
- **Fluxo:** quantidade que está efetivamente passando pela tubulação.

Uma conexão será considerada saturada quando seu fluxo atingir sua capacidade máxima.

## 10. Visualização no terminal

A visualização será feita diretamente no terminal, sem utilização de ferramentas externas.

O programa apresentará os vértices e suas conexões em uma estrutura textual.

Exemplo ilustrativo:

```text
========== REDE DE FLUXO ==========

VERTICE 0 [ORIGEM]
   |----> 2  [5/5]  SATURADA
   |----> 1  [10/10] SATURADA

VERTICE 1
   |----> 3  [8/8]  SATURADA
   |----> 2  [2/4]

VERTICE 2
   |----> 3  [7/10]

VERTICE 3 [DESTINO]
   (sem conexoes de saida)

===================================
```

A representação `5/8` significa que a conexão está transportando 5 unidades de fluxo e possui capacidade máxima de 8 unidades.

## 11. Testes

Os testes deverão demonstrar o funcionamento do algoritmo em diferentes situações.

Conforme as orientações do trabalho, serão utilizados grafos com pelo menos 8 vértices.

Os testes previstos são:

- **Teste 1 — Caso normal:** rede com várias conexões, diferentes capacidades e caminhos possíveis entre origem e destino.
- **Teste 2 — Caso alternativo:** alteração das capacidades ou das conexões para verificar a mudança no fluxo máximo.
- **Teste 3 — Caso especial:** situação em que não exista caminho entre origem e destino, ou em que existam conexões com capacidade zero.

Em cada teste, deverão ser apresentados os dados de entrada, a representação do grafo, a execução do algoritmo, o resultado e sua interpretação.

## 12. Organização e apresentação

O projeto será desenvolvido para que todos os integrantes consigam explicar:

- A modelagem do problema por meio de grafos.
- A estrutura de lista de adjacência.
- A utilização das arestas reversas.
- A busca em profundidade.
- O cálculo do gargalo.
- A atualização dos fluxos.
- A identificação do fluxo máximo.
- A interpretação dos resultados.

A apresentação deverá demonstrar pelo menos um teste completo e explicar o resultado obtido pelo programa.

**Observação:** a funcionalidade de alterar a capacidade de uma tubulação e executar novamente o algoritmo será confirmada antes de ser considerada parte da implementação final.
