
#include <iostream>
using namespace std;

// Estrutura que representa uma aresta
struct Aresta
{
    int destino;
    int capacidade;
    int fluxo;
    bool original;

    Aresta *proxima;
    Aresta *reversa;
};

// Estrutura que representa um vertice
struct Vertice
{
    int codigo;
    int grauEntrada;
    int grauSaida;
    Aresta *listaAdjacencia;
};

// Variaveis globais
Vertice **grafo;

int quantidadeVertices;
int quantidadeArestas;

int origem;
int destino;

// Funcao para cadastrar os vertices
void cadastrarVertices()
{

    cout << "Informe a quantidade de vertices: ";
    cin >> quantidadeVertices;

    while (quantidadeVertices < 2)
    {
        cout << "Informe pelo menos 2 vertices: ";
        cin >> quantidadeVertices;
    }

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

// Funcao para adicionar uma aresta e sua reversa
void adicionarAresta(int origem, int destino, int capacidade)
{

    Aresta *novaAresta = new Aresta;
    Aresta *arestaReversa = new Aresta;

    // Configuracao da aresta original
    novaAresta->destino = destino;
    novaAresta->capacidade = capacidade;
    novaAresta->fluxo = 0;
    novaAresta->original = true;

    // Configuracao da aresta reversa
    arestaReversa->destino = origem;
    arestaReversa->capacidade = 0;
    arestaReversa->fluxo = 0;
    arestaReversa->original = false;

    // Ligacao entre as arestas
    novaAresta->reversa = arestaReversa;
    arestaReversa->reversa = novaAresta;

    // Insercao da aresta original na lista
    novaAresta->proxima = grafo[origem]->listaAdjacencia;
    grafo[origem]->listaAdjacencia = novaAresta;

    // Insercao da aresta reversa na lista
    arestaReversa->proxima = grafo[destino]->listaAdjacencia;
    grafo[destino]->listaAdjacencia = arestaReversa;

    // Atualizacao dos graus do grafo original
    grafo[origem]->grauSaida++;
    grafo[destino]->grauEntrada++;
}

// Funcao para cadastrar as arestas
void cadastrarArestas()
{

    int origem, destino, capacidade;

    cout << "\nInforme a quantidade de arestas: ";
    cin >> quantidadeArestas;

    while (quantidadeArestas < 0)
    {
        cout << "Quantidade invalida. Informe novamente: ";
        cin >> quantidadeArestas;
    }

    for (int i = 0; i < quantidadeArestas; i++)
    {

        cout << "\nAresta " << i + 1 << endl;

        cout << "Vertice de origem: ";
        cin >> origem;

        cout << "Vertice de destino: ";
        cin >> destino;

        while (origem < 0 || origem >= quantidadeVertices ||
               destino < 0 || destino >= quantidadeVertices ||
               origem == destino)
        {

            cout << "Vertices invalidos. Informe novamente.\n";

            cout << "Vertice de origem: ";
            cin >> origem;

            cout << "Vertice de destino: ";
            cin >> destino;
        }

        cout << "Capacidade da conexao: ";
        cin >> capacidade;

        while (capacidade < 0)
        {
            cout << "Capacidade nao pode ser negativa. Informe novamente: ";
            cin >> capacidade;
        }

        adicionarAresta(origem, destino, capacidade);
    }

    cout << "\nArestas cadastradas com sucesso!\n";
}

// Funcao para definir origem e destino do fluxo
void definirOrigemDestino()
{

    cout << "\nInforme o vertice de origem: ";
    cin >> origem;

    cout << "Informe o vertice de destino: ";
    cin >> destino;

    while (origem < 0 || origem >= quantidadeVertices ||
           destino < 0 || destino >= quantidadeVertices ||
           origem == destino)
    {

        cout << "\nVertices invalidos. Informe novamente.\n";

        cout << "Vertice de origem: ";
        cin >> origem;

        cout << "Vertice de destino: ";
        cin >> destino;
    }
}

// Funcao que realiza a busca em profundidade (DFS)
bool buscarCaminhoDFS(int atual, int fim,
                      bool visitado[], Aresta *caminho[])
{

    visitado[atual] = true;

    // Verifica se chegou ao destino
    if (atual == fim)
    {
        return true;
    }

    // Percorre a lista de adjacencia do vertice atual
    Aresta *aresta = grafo[atual]->listaAdjacencia;

    while (aresta != NULL)
    {

        int proximo = aresta->destino;

        // Calcula a capacidade que ainda pode ser utilizada
        int capacidadeResidual =
            aresta->capacidade - aresta->fluxo;

        // Verifica se existe capacidade residual e se o vertice
        // ainda nao foi visitado
        if (capacidadeResidual > 0 && !visitado[proximo])
        {

            // Armazena a aresta utilizada para chegar ao vertice
            caminho[proximo] = aresta;

            if (buscarCaminhoDFS(proximo, fim, visitado, caminho))
            {
                return true;
            }
        }

        aresta = aresta->proxima;
    }

    return false;
}

// Funcao principal do algoritmo Ford-Fulkerson
int calcularFluxoMaximo()
{

    int fluxoMaximo = 0;

    // Arrays auxiliares para a DFS
    bool *visitado = new bool[quantidadeVertices];

    Aresta **caminho = new Aresta *[quantidadeVertices];

    // Procura caminhos aumentantes enquanto existirem
    while (true)
    {

        // Inicializa os arrays antes de cada busca
        for (int i = 0; i < quantidadeVertices; i++)
        {

            visitado[i] = false;
            caminho[i] = NULL;
        }

        // Procura um caminho entre origem e destino
        bool encontrouCaminho =
            buscarCaminhoDFS(origem, destino, visitado, caminho);

        // Se nao encontrou caminho, o algoritmo terminou
        if (!encontrouCaminho)
        {
            break;
        }

        // Inicialmente, o gargalo recebe um valor muito grande
        int gargalo = 2147483647;

        // Percorre o caminho para encontrar a menor capacidade residual
        int atual = destino;

        while (atual != origem)
        {

            Aresta *aresta = caminho[atual];

            int capacidadeResidual =
                aresta->capacidade - aresta->fluxo;

            if (capacidadeResidual < gargalo)
            {
                gargalo = capacidadeResidual;
            }

            // Volta para o vertice anterior do caminho
            atual = aresta->reversa->destino;
        }

        // Atualiza o fluxo de todas as arestas do caminho
        atual = destino;

        while (atual != origem)
        {

            Aresta *aresta = caminho[atual];

            // Aumenta o fluxo da aresta utilizada
            aresta->fluxo += gargalo;

            // Atualiza o fluxo da aresta reversa
            aresta->reversa->fluxo -= gargalo;

            atual = aresta->reversa->destino;
        }

        // Soma o gargalo ao fluxo maximo encontrado
        fluxoMaximo += gargalo;
    }

    // Libera os arrays auxiliares
    delete[] visitado;
    delete[] caminho;

    return fluxoMaximo;
}

// Funcao para mostrar o grafo original
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

        bool encontrou = false;

        while (atual != NULL)
        {

            // Mostra somente as arestas originais
            if (atual->original)
            {

                cout << " -> " << atual->destino;
                cout << " (Capacidade: " << atual->capacidade << ")";

                encontrou = true;
            }

            atual = atual->proxima;
        }

        if (!encontrou)
        {
            cout << "NULL";
        }

        cout << endl;
    }
}

// Funcao para mostrar os fluxos utilizados nas arestas
void mostrarFluxosArestas()
{

    cout << "\n===== FLUXOS DAS CONEXOES =====\n";

    for (int i = 0; i < quantidadeVertices; i++)
    {

        Aresta *atual = grafo[i]->listaAdjacencia;

        while (atual != NULL)
        {

            if (atual->original)
            {

                cout << "\n"
                     << i << " -> " << atual->destino;

                cout << " | Fluxo: " << atual->fluxo;

                cout << " | Capacidade: " << atual->capacidade;

                if (atual->fluxo == atual->capacidade)
                {
                    cout << " | CONEXAO SATURADA";
                }
                else
                {
                    cout << " | Capacidade disponivel: "
                         << atual->capacidade - atual->fluxo;
                }
            }

            atual = atual->proxima;
        }
    }

    cout << endl;
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

void visualizarGrafoTerminal()
{
    cout << "\n";
    cout << "========== REDE DE FLUXO ==========\n\n";

    for (int i = 0; i < quantidadeVertices; i++)
    {

        cout << "VERTICE " << i;

        if (i == origem)
            cout << " [ORIGEM]";

        if (i == destino)
            cout << " [DESTINO]";

        cout << "\n";

        Aresta *atual = grafo[i]->listaAdjacencia;

        bool encontrou = false;

        while (atual != NULL)
        {

            if (atual->original)
            {
                encontrou = true;

                cout << "   |";

                if (atual->fluxo == atual->capacidade)
                    cout << "----> ";
                else
                    cout << "----> ";

                cout << atual->destino;

                cout << "  ["
                     << atual->fluxo << "/"
                     << atual->capacidade << "]";

                if (atual->fluxo == atual->capacidade)
                    cout << "  SATURADA";

                cout << "\n";
            }

            atual = atual->proxima;
        }

        if (!encontrou)
            cout << "   (sem conexoes de saida)\n";

        cout << "\n";
    }

    cout << "===================================\n";
}

// Funcao principal
int main()
{

    cadastrarVertices();

    cadastrarArestas();

    definirOrigemDestino();

    mostrarGrafo();

    int fluxoMaximo = calcularFluxoMaximo();

    cout << "\n===== RESULTADO DO FORD-FULKERSON =====\n";

    cout << "\nOrigem: " << origem;
    cout << "\nDestino: " << destino;

    cout << "\nFluxo maximo encontrado: " << fluxoMaximo << endl;

    mostrarFluxosArestas();

    visualizarGrafoTerminal();

    mostrarFluxosArestas();

    liberarMemoria();

    return 0;
}
