#include <iostream>
#include <string>
using namespace std;

struct Aresta
{
    int destino;
    int capacidade;
    int fluxo;
    bool original;

    Aresta *proxima;
    Aresta *reversa;
};

struct Vertice
{
    int codigo;
    int grauEntrada;
    int grauSaida;
    Aresta *listaAdjacencia;
};

Vertice **grafo;

int quantidadeVertices;
int quantidadeArestas;

int origem;
int destino;

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

    for (int i = 0; i < quantidadeVertices; i++)//cria todos os vertices do grafo, inicialmente nulos
    {
        grafo[i] = new Vertice;
        grafo[i]->codigo = i;
        grafo[i]->grauEntrada = 0;
        grafo[i]->grauSaida = 0;
        grafo[i]->listaAdjacencia = NULL;
    }
    cout << "\nVertices cadastrados com sucesso!\n";
}

void adicionarAresta(int origem, int destino, int capacidade)//será acionada cadastrarArestas()
{
    Aresta *novaAresta = new Aresta;
    Aresta *arestaReversa = new Aresta;

    // Configuracao da aresta original
    novaAresta->destino = destino;
    novaAresta->capacidade = capacidade;
    novaAresta->fluxo = 0;
    novaAresta->original = true;

    // Configuracao da aresta reversa, aresta fantasma do grafo residual, usada posteiormente
    arestaReversa->destino = origem;
    arestaReversa->capacidade = 0;
    arestaReversa->fluxo = 0;
    arestaReversa->original = false;

    // Ligacao entre as arestas (aresta->reversa->fluxo -= gargalo)
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
        while (origem < 0 || origem >= quantidadeVertices || destino < 0 || destino >= quantidadeVertices || origem == destino)
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
    while (origem < 0 || origem >= quantidadeVertices || destino < 0 || destino >= quantidadeVertices || origem == destino)
    {
        cout << "\nVertices invalidos. Informe novamente.\n";
        cout << "Vertice de origem: ";
        cin >> origem;
        cout << "Vertice de destino: ";
        cin >> destino;
    }
}

// Funcao que realiza a busca em profundidade (DFS)
bool buscarCaminhoDFS(int atual, int fim, bool visitado[], Aresta *caminho[])
{
    visitado[atual] = true;

    if (atual == fim)// Verifica se chegou ao destino
    {
        return true;
    }

    Aresta *aresta = grafo[atual]->listaAdjacencia; //percorre lista de ajdjacencia do vertice atual

    while (aresta != NULL)
    {
        int proximo = aresta->destino;
        int capacidadeResidual = aresta->capacidade - aresta->fluxo; // Calcula a capacidade que ainda pode ser utilizada
        if (capacidadeResidual > 0 && !visitado[proximo])// verifica se existe capacidadel residual e se o vertice ainda nao foi visitado 
        {
            caminho[proximo] = aresta;// Armazena a aresta utilizada para chegar ao vertice
            if (buscarCaminhoDFS(proximo, fim, visitado, caminho))// Chamada recursiva para o proximo vertice
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

    bool *visitado = new bool[quantidadeVertices];

    Aresta **caminho = new Aresta *[quantidadeVertices];

    while (true)// Procura caminhos aumentantes enquanto existirem
    {
        for (int i = 0; i < quantidadeVertices; i++) // Inicializa os arrays antes de cada busca
        {
            visitado[i] = false;
            caminho[i] = NULL;
        }
        // Procura um caminho entre origem e destino
        bool encontrouCaminho = buscarCaminhoDFS(origem, destino, visitado, caminho);

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

            int capacidadeResidual = aresta->capacidade - aresta->fluxo;// (fluxo atual) - (capacidade da aresta)

            if (capacidadeResidual < gargalo)
            {
                gargalo = capacidadeResidual;
            }
            atual = aresta->reversa->destino; // Volta para o vertice anterior do caminho
        }

        atual = destino;// Atualiza o fluxo de todas as arestas do caminho

        while (atual != origem)
        {
            Aresta *aresta = caminho[atual];
            
            aresta->fluxo += gargalo; // Aumenta o fluxo da aresta utilizada

            aresta->reversa->fluxo -= gargalo; // Atualiza o fluxo da aresta reversa

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
        cout << " | Grau de entrada: " << grafo[i]->grauEntrada;
        cout << " | Grau de saida: " << grafo[i]->grauSaida;
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

int mostrar_ford_fulkerson() {

    int fluxoMaximo = 0;
    int numeroCaminho = 0;

    bool *visitado = new bool[quantidadeVertices];
    Aresta **caminho = new Aresta*[quantidadeVertices];

    // Guarda os gargalos para montar a soma no final
    string somaGargalos = "";

    cout << "\n========== PASSO A PASSO FORD-FULKERSON ==========\n";

    while (true) {
        // Reinicia os vetores de busca
        for (int i = 0; i < quantidadeVertices; i++) {
            visitado[i] = false;
            caminho[i] = NULL;
        }
        
        bool encontrouCaminho = buscarCaminhoDFS(origem, destino, visitado, caminho); // Procura um caminho aumentante

        if (!encontrouCaminho) {        // Se nao encontrou caminho, o algoritmo terminou
            break;
        }

        numeroCaminho++;

        // Vetores para reconstruir o caminho na ordem correta
        int *verticesCaminho = new int[quantidadeVertices];
        Aresta **arestasCaminho = new Aresta*[quantidadeVertices];

        int tamanhoCaminho = 0;
        int atual = destino;

        verticesCaminho[tamanhoCaminho] = atual;

        // Reconstrucao do caminho, do destino ate a origem
        while (atual != origem) {

            Aresta *aresta = caminho[atual];

            arestasCaminho[tamanhoCaminho] = aresta;

            atual = aresta->reversa->destino;

            tamanhoCaminho++;

            verticesCaminho[tamanhoCaminho] = atual;
        }

        // Encontra o gargalo do caminho
        int gargalo = 2147483647;

        for (int i = 0; i < tamanhoCaminho; i++) {

            Aresta *aresta = arestasCaminho[i];

            int capacidadeResidual =
                aresta->capacidade - aresta->fluxo;

            if (capacidadeResidual < gargalo) {
                gargalo = capacidadeResidual;
            }
        }

        // Atualiza os fluxos das arestas
        for (int i = 0; i < tamanhoCaminho; i++) {

            Aresta *aresta = arestasCaminho[i];

            aresta->fluxo += gargalo;
            aresta->reversa->fluxo -= gargalo;
        }

        // Imprime o caminho encontrado
        cout << "\nCaminho [" << numeroCaminho << "]: ";

        cout << "[" << origem << "]";

        for (int i = tamanhoCaminho - 1; i >= 0; i--) {

            Aresta *aresta = arestasCaminho[i];

            cout << "--(";

            if (aresta->original) {
                cout << aresta->fluxo << "/" << aresta->capacidade;
            }
            else {
                // Caminho utilizou uma aresta reversa
                Aresta *original = aresta->reversa;

                cout << "REVERSA "
                     << original->fluxo << "/" << original->capacidade;
            }

            cout << ")-->";

            cout << "[" << verticesCaminho[i] << "]";
        }

        cout << "\nGargalo [" << numeroCaminho<< "]: " << gargalo << "\n";

        // Guarda o gargalo para a soma final
        if (numeroCaminho > 1) {
            somaGargalos += " + ";
        }

        somaGargalos += to_string(gargalo);

        // Atualiza o fluxo total
        fluxoMaximo += gargalo;

        delete[] verticesCaminho;
        delete[] arestasCaminho;
    }

    cout << "\n==================================================\n";

    cout << "\nFluxo maximo (soma dos gargalos):\n";
    if (numeroCaminho == 0) {
        cout << "Nenhum caminho aumentante encontrado. Fluxo maximo = 0\n";
    } else {
        cout << somaGargalos << " = " << fluxoMaximo << "\n";
    }

    cout << "\n==================================================\n";

    delete[] visitado;
    delete[] caminho;

    return fluxoMaximo;
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

int main()
{
    cadastrarVertices();
    cadastrarArestas();
    definirOrigemDestino();

    mostrarGrafo();

    cout << "\n===== EXECUCAO DO FORD-FULKERSON =====\n";
    cout << "Origem: " << origem << "\n";
    cout << "Destino: " << destino << "\n";

    // Executa o algoritmo uma unica vez e mostra cada iteracao.
    int fluxoMaximo = mostrar_ford_fulkerson();

    cout << "\n===== RESULTADO FINAL =====\n";
    cout << "Fluxo maximo encontrado: " << fluxoMaximo << "\n";

    liberarMemoria();
    return 0;
}
