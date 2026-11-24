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

// Verifica se dois vertices sao validos e diferentes entre si
bool verticesValidos(int a, int b)
{
    return a >= 0 && a < quantidadeVertices && b >= 0 && b < quantidadeVertices && a != b;
}

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

void adicionarAresta(int origem, int destino, int capacidade)//sera acionada por cadastrarArestas()
{
    Aresta *novaAresta = new Aresta;
    Aresta *arestaReversa = new Aresta;

    // Configuracao da aresta original
    novaAresta->destino = destino;
    novaAresta->capacidade = capacidade;
    novaAresta->fluxo = 0;
    novaAresta->original = true;

    // Configuracao da aresta reversa, aresta fantasma do grafo residual, usada posteriormente
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
        while (!verticesValidos(origem, destino))
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
    while (!verticesValidos(origem, destino))
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

    Aresta *aresta = grafo[atual]->listaAdjacencia; //percorre lista de adjacencia do vertice atual

    while (aresta != NULL)
    {
        int proximo = aresta->destino;
        int capacidadeResidual = aresta->capacidade - aresta->fluxo; // Calcula a capacidade que ainda pode ser utilizada
        if (capacidadeResidual > 0 && !visitado[proximo])// verifica se existe capacidade residual e se o vertice ainda nao foi visitado 
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

// Mostra o fluxo final de cada conexao e identifica as que atingiram a capacidade maxima
void mostrarFluxosFinais()
{
    cout << "\n===== FLUXO UTILIZADO EM CADA CONEXAO =====\n";
    cout << "Capacidade = quanto a conexao suporta | Fluxo = quanto realmente passa por ela\n\n";

    int quantidadeSaturadas = 0;

    for (int i = 0; i < quantidadeVertices; i++)
    {
        Aresta *aresta = grafo[i]->listaAdjacencia;
        while (aresta != NULL)
        {
            if (aresta->original) // reversas nao sao conexoes reais
            {
                cout << i << " -> " << aresta->destino
                     << " | Capacidade: " << aresta->capacidade
                     << " | Fluxo: " << aresta->fluxo;

                if (aresta->capacidade > 0 && aresta->fluxo == aresta->capacidade)
                {
                    cout << " | [SATURADA]";
                    quantidadeSaturadas++;
                }
                cout << endl;
            }
            aresta = aresta->proxima;
        }
    }
    cout << "\nConexoes que atingiram a capacidade maxima: " << quantidadeSaturadas << "\n";
}

int executarFordFulkerson() {

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

    mostrarFluxosFinais();

    return fluxoMaximo;
}

// Funcao para zerar o fluxo de todas as arestas (originais e reversas)
void zerarFluxo()
{
    for (int i = 0; i < quantidadeVertices; i++)
    {
        Aresta *aresta = grafo[i]->listaAdjacencia;
        while (aresta != NULL)
        {
            aresta->fluxo = 0; // zera originais e reversas
            aresta = aresta->proxima;
        }
    }
}

// Mostra as tubulacoes numeradas e devolve quantas existem
int listarTubulacoes()
{
    int numero = 0;
    for (int i = 0; i < quantidadeVertices; i++)
    {
        Aresta *aresta = grafo[i]->listaAdjacencia;
        while (aresta != NULL)
        {
            if (aresta->original)
            {
                numero++;
                cout << numero << ") " << i << " -> " << aresta->destino
                     << " (capacidade: " << aresta->capacidade << ")\n";
            }
            aresta = aresta->proxima;
        }
    }
    return numero;
}

// Percorre na mesma ordem de listarTubulacoes e devolve a aresta de numero escolhido
Aresta *buscarTubulacao(int numeroEscolhido)
{
    int numero = 0;
    for (int i = 0; i < quantidadeVertices; i++)
    {
        Aresta *aresta = grafo[i]->listaAdjacencia;
        while (aresta != NULL)
        {
            if (aresta->original)
            {
                numero++;
                if (numero == numeroEscolhido)
                {
                    return aresta;
                }
            }
            aresta = aresta->proxima;
        }
    }
    return NULL;
}

// Permite ao usuario escolher uma tubulacao e alterar sua capacidade
void alterarCapacidade()
{
    cout << "\n===== TUBULACOES =====\n";
    int total = listarTubulacoes();
    if (total == 0)
    {
        cout << "Nao ha tubulacoes cadastradas.\n";
        return;
    }

    int escolha;
    cout << "Escolha a tubulacao: ";
    cin >> escolha;
    while (escolha < 1 || escolha > total)
    {
        cout << "Numero invalido. Informe novamente: ";
        cin >> escolha;
    }

    Aresta *aresta = buscarTubulacao(escolha);

    int novaCapacidade;
    cout << "Nova capacidade: ";
    cin >> novaCapacidade;
    while (novaCapacidade < 0)
    {
        cout << "Capacidade nao pode ser negativa. Informe novamente: ";
        cin >> novaCapacidade;
    }
    aresta->capacidade = novaCapacidade;
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

    // Primeira execucao do algoritmo, mostrando cada iteracao.
    int fluxoAnterior = executarFordFulkerson();

    cout << "\n===== RESULTADO FINAL =====\n";
    cout << "Fluxo maximo encontrado: " << fluxoAnterior << "\n";

    int opcao;
    do
    {
        cout << "\n1 - Alterar capacidade e executar novamente\n";
        cout << "0 - Sair\nOpcao: ";
        cin >> opcao;

        if (opcao == 1)
        {
            alterarCapacidade();
            zerarFluxo();
            mostrarGrafo();
            int fluxoNovo = executarFordFulkerson();

            cout << "\n===== COMPARACAO =====\n";
            cout << "Fluxo anterior: " << fluxoAnterior << "\n";
            cout << "Fluxo novo:     " << fluxoNovo << "\n";
            cout << "Diferenca:      " << fluxoNovo - fluxoAnterior << "\n";

            fluxoAnterior = fluxoNovo;
        }
    } while (opcao != 0);

    liberarMemoria();
    return 0;
}