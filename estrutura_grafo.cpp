#include <iostream>
using namespace std;

// Estrutura que representa uma aresta do grafo
struct Aresta
{
    int destino;     // Para qual vertice a aresta aponta
    int capacidade;  // Capacidade da conexao
    int fluxo;       // Fluxo atual da conexao
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

// Ponteiro para o grafo completo
Vertice **grafo;

int quantidadeVertices;
int quantidadeArestas;

// Funcao para cadastrar os vertices
void cadastrarVertices()
{
    do
    {
        cout << "Informe a quantidade de vertices (maior que 0): ";
        cin >> quantidadeVertices;

        if (quantidadeVertices <= 0)
        {
            cout << "Quantidade de vertices invalida. Tente novamente.\n";
        }

    } while (quantidadeVertices <= 0);

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

// Funcao para adicionar uma aresta na lista de adjacencia
void adicionarAresta(int origem, int destino, int capacidade)
{

    Aresta *novaAresta = new Aresta;

    novaAresta->destino = destino;
    novaAresta->capacidade = capacidade;
    novaAresta->fluxo = 0;

    // Insercao no inicio da lista
    novaAresta->proxima = grafo[origem]->listaAdjacencia;

    grafo[origem]->listaAdjacencia = novaAresta;

    // Atualizacao dos graus dos vertices
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

        do
        {
            cout << "Capacidade da conexao (maior que 0): ";
            cin >> capacidade;

            if (capacidade <= 0)
            {
                cout << "Capacidade invalida. Tente novamente.\n";
            }

        } while (capacidade <= 0);

        cout << "Capacidade da conexao: ";
        cin >> capacidade;

        adicionarAresta(origem, destino, capacidade);
    }

    cout << "\nArestas cadastradas com sucesso!\n";
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

        if (atual == NULL)
        {
            cout << "NULL";
        }

        while (atual != NULL)
        {

            cout << " -> " << atual->destino;

            cout << " (Capacidade: "
                 << atual->capacidade << ")";

            atual = atual->proxima;
        }

        cout << endl;
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

    liberarMemoria();

    return 0;
}
