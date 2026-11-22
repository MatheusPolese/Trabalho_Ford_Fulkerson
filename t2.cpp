#include <iostream>
using namespace std;

// Estrutura que representa uma aresta do grafo
struct Aresta
{
    int destino;     // Para qual vertice a aresta aponta
    int capacidade;  // Capacidade da conexao
    int fluxo;       // Fluxo atual da conexao
    bool ehReversa;  // true se for a aresta reversa (grafo residual)
    Aresta *reversa; // Ponteiro para a aresta oposta (par residual)
    Aresta *proxima; // Ponteiro para a proxima aresta na lista de adjacencia
};

// Estrutura que representa um vertice do grafo
struct Vertice
{
    int codigo;              // Identificador do vertice
    int grauEntrada;         // Grau de entrada do vertice
    int grauSaida;           // Grau de saida do vertice
    Aresta *listaAdjacencia; // Lista de adjacencia do vertice
};

Vertice **grafo; // Ponteiro para o grafo completo
int quantidadeVertices;
int quantidadeArestas;
int verticeOrigem;
int verticeDestino;
int fluxoMaximo = 0;

void cadastrarVertices() // Funcao para cadastrar os vertices
{
    cout << "Informe a quantidade de vertices: ";
    cin >> quantidadeVertices;
    // Alocacao dinamica do array de ponteiros
    grafo = new Vertice *[quantidadeVertices];
    for (int i = 0; i < quantidadeVertices; i++)
    {
        grafo[i] = new Vertice;

        grafo[i]->codigo = i;
        grafo[i]->grauEntrada = 0;
        grafo[i]->grauSaida = 0;
        grafo[i]->listaAdjacencia = NULL;
    }
    cout << "\nVertices cadastrados com sucesso!\n";
}

// Funcao para adicionar uma aresta na lista de adjacencia.
// Alem da aresta normal, cria a aresta reversa (capacidade 0),
// necessaria para "desfazer" fluxo no grafo residual.
void adicionarAresta(int origem, int destino, int capacidade)
{

    Aresta *novaAresta = new Aresta;
    Aresta *arestaReversa = new Aresta;

    novaAresta->destino = destino;
    novaAresta->capacidade = capacidade;
    novaAresta->fluxo = 0;
    novaAresta->ehReversa = false;
    novaAresta->reversa = arestaReversa;

    arestaReversa->destino = origem;
    arestaReversa->capacidade = 0;
    arestaReversa->fluxo = 0;
    arestaReversa->ehReversa = true;
    arestaReversa->reversa = novaAresta;

    // Insercao no inicio da lista
    novaAresta->proxima = grafo[origem]->listaAdjacencia;
    grafo[origem]->listaAdjacencia = novaAresta;

    arestaReversa->proxima = grafo[destino]->listaAdjacencia;
    grafo[destino]->listaAdjacencia = arestaReversa;

    // Atualizacao dos graus dos vertices (apenas arestas reais)
    grafo[origem]->grauSaida++;
    grafo[destino]->grauEntrada++;
}

// Funcao para cadastrar as arestas
void cadastrarArestas()
{

    int origem, destino, capacidade;

    cout << "\nInforme a quantidade de arestas: ";
    cin >> quantidadeArestas;

    for (int i = 0; i < quantidadeArestas; i++)
    {
        cout << "\nAresta " << i + 1 << endl;
        cout << "Vertice de origem: ";
        cin >> origem;

        cout << "Vertice de destino: ";
        cin >> destino;

        cout << "Capacidade da conexao: ";
        cin >> capacidade;

        if (origem < 0 || origem >= quantidadeVertices ||
            destino < 0 || destino >= quantidadeVertices ||
            capacidade < 0)
        {
            cout << "Dados invalidos! Informe esta aresta novamente.\n";
            i--;
            continue;
        }

        adicionarAresta(origem, destino, capacidade);
    }
    cout << "\nArestas cadastradas com sucesso!\n";
}

// Funcao para escolher o vertice de origem e o de destino do fluxo
void definirOrigemDestino()
{
    do
    {
        cout << "\nInforme o vertice de origem do fluxo: ";
        cin >> verticeOrigem;

        cout << "Informe o vertice de destino do fluxo: ";
        cin >> verticeDestino;

        if (verticeOrigem < 0 || verticeOrigem >= quantidadeVertices ||
            verticeDestino < 0 || verticeDestino >= quantidadeVertices ||
            verticeOrigem == verticeDestino)
        {
            cout << "Vertices invalidos! Devem existir e ser diferentes.\n";
        }

    } while (verticeOrigem < 0 || verticeOrigem >= quantidadeVertices ||
             verticeDestino < 0 || verticeDestino >= quantidadeVertices ||
             verticeOrigem == verticeDestino);
}

// DFS que procura um caminho aumentante no grafo residual.
// Retorna o gargalo (fluxo enviado) se achou caminho, ou 0 se nao achou.
// Ao voltar da recursao, ja atualiza o fluxo das arestas do caminho.
int buscarCaminhoDFS(int atual, int destino, int gargalo, bool *visitado)
{

    if (atual == destino)
    {
        return gargalo;
    }

    visitado[atual] = true;

    Aresta *aresta = grafo[atual]->listaAdjacencia;

    while (aresta != NULL)
    {

        int residual = aresta->capacidade - aresta->fluxo;

        if (!visitado[aresta->destino] && residual > 0)
        {

            int menor = (gargalo < residual) ? gargalo : residual;

            int enviado = buscarCaminhoDFS(aresta->destino, destino, menor, visitado);

            if (enviado > 0)
            {
                // Aumenta o fluxo na aresta e diminui na reversa
                aresta->fluxo += enviado;
                aresta->reversa->fluxo -= enviado;

                return enviado;
            }
        }

        aresta = aresta->proxima;
    }

    return 0;
}

// Funcao que executa o algoritmo de Ford-Fulkerson
void calcularFluxoMaximo()
{

    fluxoMaximo = 0;

    // Zera o fluxo de todas as arestas (permite reexecutar)
    for (int i = 0; i < quantidadeVertices; i++)
    {
        for (Aresta *a = grafo[i]->listaAdjacencia; a != NULL; a = a->proxima)
        {
            a->fluxo = 0;
        }
    }

    bool *visitado = new bool[quantidadeVertices];

    // "Infinito" inicial para o gargalo
    const int INFINITO = 2147483647;

    while (true)
    {

        for (int i = 0; i < quantidadeVertices; i++)
        {
            visitado[i] = false;
        }

        int enviado = buscarCaminhoDFS(verticeOrigem, verticeDestino, INFINITO, visitado);

        if (enviado == 0)
        {
            break; // Nao existem mais caminhos aumentantes
        }

        fluxoMaximo += enviado;
    }

    delete[] visitado;
}

// Funcao para mostrar o grafo
void mostrarGrafo()
{

    cout << "\n===== REPRESENTACAO DO GRAFO =====\n";

    for (int i = 0; i < quantidadeVertices; i++)
    {

        cout << "\nVertice " << grafo[i]->codigo;

        cout << " | Grau de entrada: "
             << grafo[i]->grauEntrada;

        cout << " | Grau de saida: "
             << grafo[i]->grauSaida;

        cout << "\nLista de adjacencia: ";

        Aresta *atual = grafo[i]->listaAdjacencia;

        bool temAresta = false;

        while (atual != NULL)
        {

            // As arestas reversas sao apenas internas (residual)
            if (!atual->ehReversa)
            {
                temAresta = true;

                cout << " -> " << atual->destino;

                cout << " (Capacidade: "
                     << atual->capacidade << ")";
            }

            atual = atual->proxima;
        }

        if (!temAresta)
        {
            cout << "NULL";
        }

        cout << endl;
    }
}

// Funcao para mostrar o fluxo maximo
void mostrarFluxoMaximo()
{

    cout << "\n===== FLUXO MAXIMO =====\n";
    cout << "Origem: " << verticeOrigem
         << " | Destino: " << verticeDestino << endl;
    cout << "Quantidade maxima de agua transportada: "
         << fluxoMaximo << endl;
}

// Funcao para mostrar o fluxo e a capacidade de cada aresta
void mostrarFluxosArestas()
{

    cout << "\n===== FLUXO DAS CONEXOES =====\n";

    for (int i = 0; i < quantidadeVertices; i++)
    {

        Aresta *atual = grafo[i]->listaAdjacencia;

        while (atual != NULL)
        {

            if (!atual->ehReversa)
            {
                cout << i << " -> " << atual->destino
                     << " | Fluxo: " << atual->fluxo
                     << " / Capacidade: " << atual->capacidade << endl;
            }

            atual = atual->proxima;
        }
    }
}

// Funcao para identificar as conexoes saturadas
void mostrarArestasSaturadas()
{

    cout << "\n===== CONEXOES SATURADAS =====\n";

    bool encontrou = false;

    for (int i = 0; i < quantidadeVertices; i++)
    {

        Aresta *atual = grafo[i]->listaAdjacencia;

        while (atual != NULL)
        {

            if (!atual->ehReversa &&
                atual->capacidade > 0 &&
                atual->fluxo == atual->capacidade)
            {
                cout << i << " -> " << atual->destino
                     << " (Fluxo " << atual->fluxo
                     << " = Capacidade " << atual->capacidade << ")\n";

                encontrou = true;
            }

            atual = atual->proxima;
        }
    }

    if (!encontrou)
    {
        cout << "Nenhuma conexao saturada.\n";
    }
}

// Funcao para liberar a memoria utilizada
void liberarMemoria()
{

    for (int i = 0; i < quantidadeVertices; i++)
    {

        Aresta *atual = grafo[i]->listaAdjacencia;

        while (atual != NULL)
        {

            Aresta *temporaria = atual;

            atual = atual->proxima;

            delete temporaria;
        }

        delete grafo[i];
    }

    delete[] grafo;
}

// Funcao principal
int main()
{

    cadastrarVertices();

    cadastrarArestas();

    mostrarGrafo();

    definirOrigemDestino();

    calcularFluxoMaximo();

    mostrarFluxoMaximo();

    mostrarFluxosArestas();

    mostrarArestasSaturadas();

    liberarMemoria();

    return 0;
}

// anotações sobre, reduzir o numero de linhas, e implementar uma maneira que o usuário possa escolher se quer ver o grafo, o fluxo máximo, os fluxos das arestas e as arestas saturadas.
// tambem uma maneira que quando esta cadastrando o vertice ja escolha o vertice de origem e destino e o peso da aresta.