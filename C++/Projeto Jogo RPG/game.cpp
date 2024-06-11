/*

- struct dos ITENS
typedef struct {
    string nome, descricao;
    int quantidade;
} Item;

*/

#include <iostream>
// #include <boost/locale.hpp>
#include <string>
#include <initializer_list>
#include <sstream>
#include <random>
#include <thread>

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <unistd.h>
    #include <cstdlib>
#endif

using namespace std;

// estrutura que define as vari�veis relacionadas ao personagem do jogador
typedef struct {
    string nome, pronome;
    string arquetipo;
    int vida, ataque, defesa;
    int exp, nivel;
    // vetor de habilidades
} Personagem;

// estrutura das habilidades dos inimigos
typedef struct {
    string nomeHabilidade;
    int danoHabilidade;
} HabilidadeInimigo;

typedef struct {
    string nomeContraAtaque, descricaoContraAtaque;
    int danoContraAtaque;
} ContraAtaquesInimigo;

// estrutura que define as vari�veis relacionadas aos inimigos
typedef struct {
    string nome;
    int vida, ataque, defesa;
    HabilidadeInimigo habilidades[3];
    ContraAtaquesInimigo contraAtaques[2];
} Inimigo;

// estrutura dos n�s da FILA DE COMBATE
typedef struct noFilaCombate {
    string nome;
    struct noFilaCombate *proximo;
} noFilaCombate;

// estrutura principal da FILA DE COMBATE
typedef struct {
    noFilaCombate *frente;
    noFilaCombate *tras;
    int tam;
} FilaCombate;

void criarFilaCombate(FilaCombate& filaCombate)
{
    filaCombate.frente = nullptr;
	filaCombate.tras = nullptr;
	filaCombate.tam = 0;
}

void enfileirarCriaturasCombate(FilaCombate& filaCombate, string criatura)
{
    noFilaCombate *novo_no = new noFilaCombate;
    
    if(novo_no)
	{
		novo_no -> nome = criatura;
		novo_no -> proximo = nullptr;
		
		if(filaCombate.frente == nullptr)
			filaCombate.frente = novo_no;
		else
			filaCombate.tras->proximo = novo_no;

        filaCombate.tras = novo_no;
		
		filaCombate.tam++;
	}
	else
		cout << "\n\nAlgo deu errado ao enfileirar as criaturas para o combate\n\n";
}

void imprimirFilaCombate(FilaCombate& filaCombate)
{
	noFilaCombate *aux = filaCombate.frente;
	
	printf("\n------ Queue / Itens: %02i ------\n", filaCombate.tam);
	
	while(aux)
	{
		cout << aux->nome << endl;
		aux = aux->proximo;
	}
	
	printf("\n---------- Queue End ----------\n\n");
}

// função que limpa a tela com o comando correto dependendo do seu SO
void limparTela()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// função que pausa a execução do programa com base no SO
void pausar(int milisegundos)
{
    #ifdef _WIN32
        Sleep(milisegundos);
    #else
        usleep(milisegundos * 1000);
    #endif
}

// função de animação dos 3 pontos ao finalizar o jogo
void imprimirAnimacaoPontos()
{
    for(int i=0; i<3; i++)
    {
        cout << "." << flush;
        pausar(500);
        cout << "." << flush;
        pausar(500);
        cout << "." << flush;
        pausar(500);
        
        cout << "\b\b\b   " << flush;
        cout << "\b\b\b" << flush;
        pausar(500);
    }
}

// função que imprime strings letra a letra, dando a impressão de fala
void imprimirNarracao(string narracao)
{
    for(char letra : narracao)
    {
        cout << letra << flush;
        pausar(50);
    }
}

// função que espera uma tecla qualquer do teclado para continuar a execução
void esperarTeclaParaContinuar()
{
    #ifdef _WIN32
        cout << "Pressione a tecla enter para continuar...\n";
        _getch();
    #else
        cout << "Pressione a tecla enter para continuar...\n";
        system("read -r -p \"\" key");
    #endif
}

void contraAtaquesInimigo(Inimigo& inimigo, int escolhaDoJogador)
{
    string narracao;
    ostringstream valorConvertido;
    int dano;

    ContraAtaquesInimigo contraAtaque = inimigo.contraAtaques[escolhaDoJogador-2];

    dano = contraAtaque.danoContraAtaque;
    dano -= inimigo.defesa;
    inimigo.vida -= dano;

    valorConvertido << dano;

    cout << "> ";
    narracao = contraAtaque.descricaoContraAtaque;
    imprimirNarracao(narracao);
    cout << endl;

    if(dano > 0)
    {
        cout << "> ";
        narracao = "Voc� causou ";
        imprimirNarracao(narracao);
        narracao = valorConvertido.str();
        imprimirNarracao(narracao);
        narracao = " de dano ao ";
        imprimirNarracao(narracao);
        narracao = inimigo.nome;
        imprimirNarracao(narracao);
        narracao = ".";
        imprimirNarracao(narracao);
        cout << endl;
    }
    else
    {
        cout << "> ";
        narracao = "Apesar das suas habilidades, n�o foi suficiente para abalar a criatura.";
        imprimirNarracao(narracao);
        cout << endl;

        cout << "> ";
        narracao = "Todo o dano foi absorvido pela defesa da criatura.";
        imprimirNarracao(narracao);
        cout << endl;
    }
    
    valorConvertido.str("");
    valorConvertido.clear();

    cout << endl;
}

void imprimirAcoesUnicas(Inimigo inimigo, int& contadorDeAcoes)
{
    string narracao;
    ostringstream contadorDeAcoesConvertido;

    for(int i=0; i<2; i++)
    {
        contadorDeAcoesConvertido << contadorDeAcoes;

        cout << "> ";
        narracao = "[";
        imprimirNarracao(narracao);
        narracao = contadorDeAcoesConvertido.str();
        imprimirNarracao(narracao);
        narracao = "] ";
        imprimirNarracao(narracao);
        narracao = inimigo.contraAtaques[i].nomeContraAtaque;
        imprimirNarracao(narracao);
        cout << endl;

        contadorDeAcoesConvertido.str("");
        contadorDeAcoesConvertido.clear();
        contadorDeAcoes++;
    }
}

void executarAcaoJogadorCombate(int escolhaDoJogador, Personagem personagem, Inimigo& inimigo)
{
    string narracao;
    ostringstream valorConvertido;
    int dano;

    switch(escolhaDoJogador)
    {
        case 1:
            dano = personagem.ataque - inimigo.defesa;
            inimigo.vida -= dano;

            valorConvertido << dano;

            cout << "> ";
            narracao = "Voc� causou ";
            imprimirNarracao(narracao);
            narracao = valorConvertido.str();
            imprimirNarracao(narracao);
            narracao = " de dano ao ";
            imprimirNarracao(narracao);
            narracao = inimigo.nome;
            imprimirNarracao(narracao);
            narracao = ".";
            imprimirNarracao(narracao);
            cout << endl;
            cout << endl;

            valorConvertido.str("");
            valorConvertido.clear();

            break;
        case 2:
        case 3:
            contraAtaquesInimigo(inimigo, escolhaDoJogador);
            break;
    }
}

void combate(Personagem personagem, initializer_list<Inimigo*> inimigos)
{
    FilaCombate filaCombate;
    string narracao;
    Inimigo* inimigoMarcado = nullptr;
    int contadorDeAcoes = 1, escolhaDoJogador, contadorDeInimigos = 0, marcador, habilidadeInimigoEscolhida, dano;
    ostringstream contadorDeAcoesConvertido;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 2);
    
    criarFilaCombate(filaCombate);
    enfileirarCriaturasCombate(filaCombate, personagem.nome);
    
    for(auto inimigo : inimigos)
    {
        enfileirarCriaturasCombate(filaCombate, inimigo->nome);
        contadorDeInimigos++;
    }
    
    contadorDeInimigos--;
    noFilaCombate *aux = filaCombate.frente;
    
    while(true)
    {
        // ajustar para imprimir com animacao
        // ajustar para se adequar � quantidade de inimigos
        cout << "> ";
        narracao = "Sua vida: " + to_string(personagem.vida);
        imprimirNarracao(narracao);
        cout << endl;

        for(auto inimigo : inimigos)
        {
            cout << "> ";
            narracao = "Vida do " + inimigo->nome + ": " + to_string(inimigo->vida);
            imprimirNarracao(narracao);
            cout << endl;
        }

        cout << endl;

        if(aux->nome == personagem.nome)
        {
            if(contadorDeInimigos > 0)
            {
                cout << "> ";
                narracao = "� a sua vez! Escolha qual inimigo atacar:";
                imprimirNarracao(narracao);
                cout << endl;
                cout << endl;
                
                for(auto inimigo : inimigos)
                {
                    if(!inimigo->nome.empty())
                    {
                        contadorDeAcoesConvertido << contadorDeAcoes;
                        
                        cout << "> ";
                        narracao = "[";
                        imprimirNarracao(narracao);
                        narracao = contadorDeAcoesConvertido.str();
                        imprimirNarracao(narracao);
                        narracao = "] ";
                        imprimirNarracao(narracao);
                        narracao = inimigo->nome;
                        imprimirNarracao(narracao);
                        cout << endl;
                        
                        contadorDeAcoesConvertido.str("");
                        contadorDeAcoesConvertido.clear();
                        contadorDeAcoes++;
                    }
                }
                
                cin >> marcador;
                marcador--;
                inimigoMarcado = inimigos.begin()[marcador];
                
                contadorDeAcoes = 1;
            }
            else
            {
                cout << "> ";
                narracao = "� a sua vez! Como voc� ir� agir?";
                imprimirNarracao(narracao);
                cout << endl;

                inimigoMarcado = inimigos.begin()[0];
            }

            cout << endl;
            cout << "> ";
            narracao = "[1] Golpear";
            imprimirNarracao(narracao);
            cout << endl;
            contadorDeAcoes++;
            
            imprimirAcoesUnicas(*inimigoMarcado, contadorDeAcoes);
            
            contadorDeAcoesConvertido << contadorDeAcoes;
            
            cout << "> ";
            narracao = "[";
            imprimirNarracao(narracao);
            narracao = contadorDeAcoesConvertido.str();
            imprimirNarracao(narracao);
            narracao = "] Checar Invent�rio";
            imprimirNarracao(narracao);
            cout << endl;
            
            cin >> escolhaDoJogador;

            cout << endl;
            executarAcaoJogadorCombate(escolhaDoJogador, personagem, *inimigoMarcado);
        }
        else
        {
            habilidadeInimigoEscolhida = dis(gen);
            
            for(auto inimigo : inimigos)
            {
                if(aux->nome == inimigo->nome)
                    inimigoMarcado = inimigo;
            }
            
            dano = inimigoMarcado->habilidades[habilidadeInimigoEscolhida].danoHabilidade - personagem.defesa;

            if(dano > 0)
            {
                personagem.vida -= dano;

                cout << endl;
                cout << "> ";
                narracao = inimigoMarcado->nome + " usou a habilidade " + inimigoMarcado->habilidades[habilidadeInimigoEscolhida].nomeHabilidade + " e causou " + dano + " de dano a voc�.";
                imprimirNarracao(narracao);
                cout << endl;
            }
            else
            {
                cout << "> ";
                narracao = "A criatura tenta te atacar de diversas formas, mas voc� tem experi�ncia com isso; e sua armadura � bastante resistente.";
                imprimirNarracao(narracao);
                cout << endl;

                cout << "> ";
                narracao = "Todo o dano foi absorvido pela sua defesa.";
                imprimirNarracao(narracao);
                cout << endl;
                cout << endl;
            }
            
            esperarTeclaParaContinuar();
            cout << endl;
        }
        
        aux = aux->proximo;
        
        if(aux == nullptr)
            aux = filaCombate.frente;
        
        contadorDeAcoesConvertido.str("");
        contadorDeAcoesConvertido.clear();
        contadorDeAcoes = 1;
        
        bool todosInimigosDerrotados = true;

        for(auto inimigo : inimigos)
        {
            if(inimigo->vida > 0)
                todosInimigosDerrotados = false;
        }

        if(todosInimigosDerrotados == true || personagem.vida <= 0)
            break;
    }

    // aqui inserir o destrutor da FILA DE COMBATE
}

// fun��o que imprime o menu e guarda a sele��o do usu�rio
int imprimirMenuPrincipal()
{
    int escolhaMenu;
    
    while(true)
    {
        limparTela();
    
        cout << "=========================================" << endl;
        cout << "             [Nome do jogo]              " << endl;
        cout << "=========================================" << endl;
        cout << endl;
        cout << "[1] Iniciar Jogo" << endl;
        cout << "[2] Sair" << endl;
        
        cin >> escolhaMenu;
        
        if(escolhaMenu != 1 && escolhaMenu != 2)
        {
            cout << "Desculpe! N�o temos a op��o inserida, ainda..." << flush;
            pausar(4000);
        }
        else
            break;
    }
    
    return escolhaMenu;
}

// fun��o que direciona as frases de cria��o de personagem para o feminino
void criarPersonagemPronomeFeminino(Personagem& personagem)
{
    string narracao;
    int escolhaArquetipo;
    
    cout << "> ";
    narracao = "Dentre eles, uma se destacou. O nome dela era: ";
    imprimirNarracao(narracao);
    
    cin >> personagem.nome;
    cout << endl;
    
    cout << "> ";
    narracao = personagem.nome;
    imprimirNarracao(narracao);
    narracao = ", inquieta, se equipava e se preparava para sair em busca de algo maior.";
    imprimirNarracao(narracao);
    cout << endl;
    cout << endl;
    
    while(true)
    {
        cout << "> ";
        narracao = "Ela foi criada sua vida inteira nesta vila, sempre treinando para se tornar a melhor:";
        imprimirNarracao(narracao);
        cout << endl;
        
        narracao = "[1] Guerreira";
        imprimirNarracao(narracao);
        cout << endl;
        narracao = "[2] Bruxa";
        imprimirNarracao(narracao);
        cout << endl;
        narracao = "[3] Assassina";
        imprimirNarracao(narracao);
        cout << endl;
        narracao = "[4] Cavaleira";
        imprimirNarracao(narracao);
        cout << endl;
        cin >> escolhaArquetipo;
        
        switch(escolhaArquetipo)
        {
            case 1:
                personagem.arquetipo = "Guerreira";
                break;
            case 2:
                personagem.arquetipo = "Bruxa";
                break;
            case 3:
                personagem.arquetipo = "Assassina";
                break;
            case 4:
                personagem.arquetipo = "Cavaleira";
                break;
            default:
                cout << "> ";
                narracao = "Ela ainda n�o aprendeu as habilidades necess�rias para se tornar isso. Vamos tentar de novo!";
                imprimirNarracao(narracao);
                cout << endl;
                break;
        }
        
        if(escolhaArquetipo < 5 && escolhaArquetipo > 0)
            break;
    }
}

// fun��o que direciona as frases de cria��o de personagem para o masculino
void criarPersonagemPronomeMasculino(Personagem& personagem)
{
    string narracao;
    int escolhaArquetipo;
    
    cout << "> ";
    narracao = "Dentre eles, um se destacou. O nome dele era: ";
    imprimirNarracao(narracao);
    
    cin >> personagem.nome;
    cout << endl;
    
    cout << "> ";
    narracao = personagem.nome;
    imprimirNarracao(narracao);
    narracao = ", inquieto, se equipava e se preparava para sair em busca de algo maior.";
    imprimirNarracao(narracao);
    cout << endl;
    cout << endl;
    
    while(true)
    {
        cout << "> ";
        narracao = "Ele foi criado sua vida inteira nesta vila, sempre treinando para se tornar o melhor:";
        imprimirNarracao(narracao);
        cout << endl;
        
        narracao = "[1] Guerreiro";
        imprimirNarracao(narracao);
        cout << endl;
        narracao = "[2] Bruxo";
        imprimirNarracao(narracao);
        cout << endl;
        narracao = "[3] Assassino";
        imprimirNarracao(narracao);
        cout << endl;
        narracao = "[4] Cavaleiro";
        imprimirNarracao(narracao);
        cout << endl;
        cin >> escolhaArquetipo;
        
        switch(escolhaArquetipo)
        {
            case 1:
                personagem.arquetipo = "Guerreiro";
                break;
            case 2:
                personagem.arquetipo = "Bruxo";
                break;
            case 3:
                personagem.arquetipo = "Assassino";
                break;
            case 4:
                personagem.arquetipo = "Cavaleiro";
                break;
            default:
                cout << "> ";
                narracao = "Ele ainda n�o aprendeu as habilidades necess�rias para se tornar isso. Vamos tentar de novo!";
                imprimirNarracao(narracao);
                cout << endl;
                break;
        }
        
        if(escolhaArquetipo < 5 && escolhaArquetipo > 0)
            break;
    }
}

// fun��o que conta a hist�ria inicial e cria o personagem
void criarPersonagem(Personagem& personagem)
{
    string narracao;
    int pronomePersonagem;
    
    while(true)
    {
        limparTela();
    
        cout << "> Antes de tudo, escolha o pronome do seu personagem:" << endl;
        cout << "[1] Ela" << endl;
        cout << "[2] Ele" << endl;
        cin >> pronomePersonagem;
        
        if(pronomePersonagem == 1 || pronomePersonagem == 2)
            break;
        else
        {
            cout << endl;
            cout << "Infelizmente n�o temos esse pronome dispon�vel no momento..." << flush;
            pausar(4000);
        }
    }
    
    limparTela();
    
    cout << "> ";
    narracao = "Em um reino distante, guerreiros se erguiam em uma pequena vila denominada [nome da vila].";
    imprimirNarracao(narracao);
    cout << endl;
    
    if(pronomePersonagem == 1)
    {
        personagem.pronome = "Ela";
        criarPersonagemPronomeFeminino(personagem);
    }
    else
    {
        personagem.pronome = "Ele";
        criarPersonagemPronomeMasculino(personagem);
    }
    
    if(personagem.arquetipo == "Guerreira" || personagem.arquetipo == "Guerreiro")
    {
        personagem.vida = 8;
        personagem.ataque = 6;
        personagem.defesa = 6;
    }
    else if(personagem.arquetipo == "Bruxa" || personagem.arquetipo == "Bruxo")
    {
        personagem.vida = 7;
        personagem.ataque = 7;
        personagem.defesa = 6;
    }
    else if(personagem.arquetipo == "Assassina" || personagem.arquetipo == "Assassino")
    {
        personagem.vida = 8;
        personagem.ataque = 8;
        personagem.defesa = 4;
    }
    else
    {
        personagem.vida = 10;
        personagem.ataque = 5;
        personagem.defesa = 5;
    }
    
    personagem.exp = 0;
    personagem.nivel = 1;

    // inicializar a lista de habilidades aqui com base no arquetipo escolhido
}

// fun��o que executa os comandos caso o jogador decida se juntar � T�mis
void respostaCasoAceitarProposta()
{
    string narracao;
    
    limparTela();
    
    cout << "> [T�mis] - ";
    narracao = "Fez a escolha certa, crian�a.";
    imprimirNarracao(narracao);
    cout << endl;
    
    cout << "> [T�mis] - ";
    narracao = "O que eu sei at� ent�o � que muitas criaturas esquisitas t�m aparecido pelos lados de {nome de algum lugar}. Talvez seja interessante come�ar por l�.";
    imprimirNarracao(narracao);
    cout << endl;
    
    cout << "> [T�mis] - ";
    narracao = "Infelizmente n�o posso te acompanhar no momento, mas acredito que isto te servir� para alguma coisa. Boa sorte.";
    imprimirNarracao(narracao);
    cout << endl;
    cout << endl;
    
    cout << "> ";
    narracao = "Voc� recebeu um saco com 3 pequenos frascos vermelhos borbulhantes.";
    imprimirNarracao(narracao);
    cout << endl;
    
    cout << "> ";
    narracao = "3 Frascos de cicatriza��o adicionados ao INVENT�RIO.";
    imprimirNarracao(narracao);
    cout << endl;
    
    // aqui adicionar o item ao inventário via Tabela Hash
    
    esperarTeclaParaContinuar();
}

// fun��o que executa os comandos caso o jogador decida n�o se juntar � T�mis
void respostaCasoRecusarProposta()
{
    string narracao;
    
    limparTela();
    
    cout << "> [T�mis] - ";
    narracao = "Como quiser. Boa sorte na sua jornada.";
    imprimirNarracao(narracao);
    cout << endl;
    
    cout << ">  ";
    narracao = "A pessoa misteriosa se desvanece em meio a uma fuma�a na sua frente, deixando um s�mbolo marcado no ch�o.";
    imprimirNarracao(narracao);
    cout << endl;
    
    cout << ">  ";
    narracao = "Voc� percebe um ac�mulo de um p� cinza no local. Sua curiosidade fala mais alto. Voc� guarda o p� em um frasco para mais tarde.";
    imprimirNarracao(narracao);
    cout << endl;
    cout << endl;
    
    cout << ">  ";
    narracao = "1 Pote de Cinzas Espirituais adicionado ao INVENT�RIO.";
    imprimirNarracao(narracao);
    cout << endl;
    
    // aqui adicionar o item ao inventário via Tabela Hash
    
    esperarTeclaParaContinuar();
}

// fun��o que direciona a conversa do pr�logo para o fluxo 1.1
void respostaCaso1o1()
{
    string narracao;
    int resposta;
    
    limparTela();
    
    cout << "> [T�mis] - ";
    narracao = "Receio que temos um interesse em comum. Parti de minha vila h� um tempo para descobrir o que s�o os acontecimentos recentes.";
    imprimirNarracao(narracao);
    cout << endl;
    
    while(true)
    {
        cout << "> [T�mis] - ";
        narracao = "Imagino que queira se juntar a mim para descobrir o que est� havendo, o que acha? Podemos decifrar juntos o que as pessoas t�m chamado de \"Apocalipse\".";
        imprimirNarracao(narracao);
        cout << endl;
        cout << endl;
        
        narracao = "[1] (Se juntar)";
        imprimirNarracao(narracao);
        cout << endl;
        narracao = "[2] (Continuar de forma independente)";
        imprimirNarracao(narracao);
        cout << endl;
        
        cin >> resposta;
        
        if(resposta < 1 || resposta > 2)
        {
            cout << "Resposta indispon�vel... Vamos tentar de novo!" << flush;
            pausar(5000);
            limparTela();
        }
        else
            break;
    }
    
    switch(resposta)
    {
        case 1:
            respostaCasoAceitarProposta();
            break;
        case 2:
            respostaCasoRecusarProposta();
            break;
    }
    
}

// fun��o que direciona a conversa do pr�logo para o fluxo 1.2
void respostaCaso1o2()
{
    string narracao;
    int resposta;

    while(true)
    {
        cout << "> [T�mis] - ";
        narracao = "Me encontro em um momento de muita descoberta dos mist�rios recentes. Talvez essas armas que carrega podem ser �teis. O que acha de me acompanhar?";
        imprimirNarracao(narracao);
        cout << endl;
        cout << endl;
                
        narracao = "[1] (Se juntar)";
        imprimirNarracao(narracao);
        cout << endl;
        narracao = "[2] (Continuar de forma independente)";
        imprimirNarracao(narracao);
        cout << endl;
        
        cin >> resposta;
        
        if(resposta < 1 || resposta > 2)
        {
            cout << "Resposta indispon�vel... Vamos tentar de novo!" << flush;
            pausar(5000);
            limparTela();
        }
        else
            break;
    }

    switch(resposta)
    {
        case 1:
            respostaCasoAceitarProposta();
            break;
        case 2:
            respostaCasoRecusarProposta();
            break;
    }
}

// fun��o que direciona a conversa do pr�logo para o fluxo 1
void respostaCaso1()
{
    string narracao;
    int resposta;
    
    limparTela();
    
    cout << "> [???] - ";
    narracao = "A aurora cativa essas plan�cies. Acredito que queira voltar sempre que puder.";
    imprimirNarracao(narracao);
    cout << endl;
    
    cout << "> [???] - ";
    narracao = "Eu, a prop�sito, estou aqui em busca de algo. Prazer, me chamo T�mis.";
    imprimirNarracao(narracao);
    cout << endl;
    cout << endl;
    
    cout << "> ";
    narracao = "O ser misterioso retira o capuz de cor marrom, revelando um rosto de uma mulher cansada, como quem j� viu coisas demais.";
    imprimirNarracao(narracao);
    cout << endl;
    cout << endl;
    
    esperarTeclaParaContinuar();
    
    while(true)
    {
        limparTela();
    
        cout << "> [T�mis] - ";
        narracao = "Acredito que tenha percebido os estranhos acontecimentos recentes. Vejo que carrega armas, para que s�o?";
        imprimirNarracao(narracao);
        cout << endl;
        cout << endl;
        
        narracao = "[1] Estou as levando em minha jornada. Quero desvendar o que tem acontecido ao redor destes vilarejos.";
        imprimirNarracao(narracao);
        cout << endl;
        narracao = "[2] S�o apenas por garantia. N�o pretendo us�-las para fins que n�o seja defesa pessoal.";
        imprimirNarracao(narracao);
        cout << endl;
        
        cin >> resposta;
        
        if(resposta < 1 || resposta > 2)
        {
            cout << "Resposta indispon�vel... Vamos tentar de novo!" << flush;
            pausar(5000);
        }
        else
            break;
    }
    
    switch(resposta)
    {
        case 1:
            respostaCaso1o1();
            break;
        case 2:
            respostaCaso1o2();
            break;
    }
}

// fun��o que direciona a conversa do pr�logo para o fluxo 2
void respostaCaso2()
{
    string narracao;
    int resposta;

    limparTela();

    cout << "> [???] - ";
    narracao = "Por sorte encontrou algu�m com o mesmo objetivo que voc�. Esses acontecimentos t�m me intrigado por semanas.";
    imprimirNarracao(narracao);
    cout << endl;

    cout << "> [???] - ";
    narracao = "Meu nome � T�mis, por sinal. Moro em uma vila a n�o muito tempo daqui. As pessoas l� n�o sabem mais o que fazer em rela��o �s coisas estranhas que t�m acontecido.";
    imprimirNarracao(narracao);
    cout << endl;
    cout << endl;

    cout << "> ";
    narracao = "O ser misterioso retira o capuz de cor marrom, revelando um rosto de uma mulher cansada, como quem j� viu coisas demais.";
    imprimirNarracao(narracao);
    cout << endl;
    cout << endl;

    esperarTeclaParaContinuar();
    limparTela();

    cout << "> [T�mis] - ";
    narracao = "Por isso, decidi eu mesma ir atr�s desse mist�rio. J� fiz muito progresso.";
    imprimirNarracao(narracao);
    cout << endl;

    while(true)
    {
        cout << "> [T�mis] - ";
        narracao = u8"Uma ajuda extra seria muito bem-vinda. O que acha de ouvir um pouco sobre o que tenho a dizer sobre essa situa��o?";
        imprimirNarracao(narracao);
        cout << endl;
        cout << endl;
        narracao = "[1] (Se juntar)";
        imprimirNarracao(narracao);
        cout << endl;
        narracao = "[2] (Continuar de forma independente)";
        imprimirNarracao(narracao);
        cout << endl;
        
        cin >> resposta;
        
        if(resposta < 1 || resposta > 2)
        {
            cout << "Resposta indisponível... Vamos tentar de novo!" << flush;
            pausar(5000);
            limparTela();
        }
        else
            break;
    }

    switch(resposta)
    {
        case 1:
            respostaCasoAceitarProposta();
            break;
        case 2:
            respostaCasoRecusarProposta();
            break;
    }
}

// fun��o que direciona a conversa do pr�logo para o fluxo 3
void respostaCaso3()
{
    string narracao;

    limparTela();

    cout << "> [???] - ";
    narracao = "Nesse caso, meu nome tamb�m n�o lhe diz respeito. Trilhe seu caminho sem inc�modos, mas se atente aos perigos dessa estrada. Nem tudo s�o rosas se visto muito de perto.";
    imprimirNarracao(narracao);
    cout << endl;
    cout << endl;

    cout << "> ";
    narracao = "A pessoa misteriosa se desvanece em meio a uma fuma�a na sua frente, deixando um s�mbolo marcado no ch�o.";
    imprimirNarracao(narracao);
    cout << endl;
    cout << endl;

    cout << "> ";
    narracao = "Voc� percebe um ac�mulo de um p� cinza no local. Sua curiosidade fala mais alto. Voc� guarda o p� em um frasco para mais tarde.";
    imprimirNarracao(narracao);
    cout << endl;

    cout << "> ";
    narracao = "1 Pote de Cinzas Espirituais adicionado ao INVENT�RIO.";
    imprimirNarracao(narracao);
    cout << endl;

    // adicionar item ao inventario aqui
    
    esperarTeclaParaContinuar();
}

// fun��o que executa o pr�logo do jogo
void prologo()
{
    string narracao;
    int resposta;
    
    limparTela();
    
    cout << "> ";
    narracao = u8"Voc� se encontra saindo da vila onde viveu sua vida toda, em busca de algo que tem perturbado a paz dos moradores locais.";
    imprimirNarracao(narracao);
    cout << endl;
    
    cout << "> ";
    narracao = u8"Planta��es mortas, desastres naturais, animais doentes por causas desconhecidas, e nada de respostas.";
    imprimirNarracao(narracao);
    cout << endl;
    cout << endl;
    
    esperarTeclaParaContinuar();
    limparTela();
    
    cout << "> ";
    narracao = u8"Durante seu caminhar atrav�s de uma estreita estrada de calc�rio, algo chama sua aten��o: um ser encapuzado.";
    imprimirNarracao(narracao);
    cout << endl;
    
    cout << "> ";
    narracao = u8"Voc� se aproxima, apesar da afli��o.";
    imprimirNarracao(narracao);
    cout << endl;
    cout << endl;
    
    esperarTeclaParaContinuar();
    
    while(true)
    {
        limparTela();
        
        cout << "> [???] - ";
        narracao = u8"O que anda fazendo por essas bandas, crian�a?";
        imprimirNarracao(narracao);
        cout << endl;
        cout << endl;
        
        narracao = "[1] Apenas dando uma passeada, e o senhor?";
        imprimirNarracao(narracao);
        cout << endl;
        narracao = "[2] Estou procurando algo que est� causando desordem em minha vila.";
        imprimirNarracao(narracao);
        cout << endl;
        narracao = "[3] N�o lhe diz respeito. Quem seria voc� por sinal?";
        imprimirNarracao(narracao);
        cout << endl;
        
        cin >> resposta;
        
        if(resposta < 1 || resposta > 3)
        {
            cout << "Resposta indispon�vel... Vamos tentar de novo!" << flush;
            pausar(5000);
        }
        else
            break;
    }
    
    switch(resposta)
    {
        case 1:
            respostaCaso1();
            break;
        case 2:
            respostaCaso2();
            break;
        case 3:
            respostaCaso3();
            break;
    }
}

void addInimigo(Inimigo& inimigo, string nome, int vida, int ataque, int defesa, HabilidadeInimigo habilidades[3], ContraAtaquesInimigo contraAtaques[2])
{
    inimigo.nome = nome;

    inimigo.vida = vida;
    inimigo.ataque = ataque;
    inimigo.defesa = defesa;
    
    for(int i=0; i<3; i++)
    {
        inimigo.habilidades[i] = habilidades[i];
        
        if(i<2)
            inimigo.contraAtaques[i] = contraAtaques[i];
    }
}

void introducaoInimigoUm(Personagem personagem)
{
    Inimigo druidaAssombrado;
    
    HabilidadeInimigo habilidades[] = {
        {"Agarrar", 5},
        {"Envolver em Trevas", 6},
        {"A Maldi��o da Floresta", 8}
    };

    ContraAtaquesInimigo contraAtaques[] = {
        {"Purificar", "Voc� usa as hist�rias que ouvia quando era crian�a para proferir ora��es que livram esta alma maldita de seu estado decadente.", 9},
        {"Usar a for�a da natureza", "Voc� n�o est� apto a controlar a natureza. A criatura sente seus movimentos e absorve quase todos.", 1}
    };

    addInimigo(druidaAssombrado, "Druida Assombrado", 15, 1, 2, habilidades, contraAtaques);
       
    limparTela();
    
    combate(personagem, {&druidaAssombrado});
}

void introducaoEntrarNaFloresta(Personagem personagem)
{
    string narracao;
    
    limparTela();
    
    cout << "> ";
    narracao = "Com apreens�o e entusiasmo, voc� se dirige � trilha banhada de dor, � inevit�vel se assustar com a apar�ncia dessa floresta.";
    imprimirNarracao(narracao);
    cout << endl;
    
    cout << "> ";
    narracao = "As �rvores se assemelham a tent�culos de um enorme polvo emergindo do solo, se contorcendo e entrela�ando nelas mesmas.";
    imprimirNarracao(narracao);
    cout << endl;
    
    cout << "> ";
    narracao = "Um ar pesado toma seu corpo. Seu caminhar se torna mais cuidadoso. H� algo por perto.";
    imprimirNarracao(narracao);
    cout << endl;
    cout << endl;
    
    esperarTeclaParaContinuar();
    limparTela();

    cout << "> ";
    narracao = "De forma bestial, um vulto negro se lan�a em sua dire��o a partir de folhagens escuras.";
    imprimirNarracao(narracao);
    cout << endl;
    cout << endl;
    
    cout << "> ";
    narracao = "UM INIMIGO APARECEU!";
    imprimirNarracao(narracao);
    cout << endl;
    cout << endl;
    
    esperarTeclaParaContinuar();
    
    introducaoInimigoUm(personagem);
}

void introducao(Personagem personagem)
{
    string narracao;
    int escolha;

    limparTela();
    
    cout << "> ";
    narracao = "Voc� observa tra�os decadentes a partir de uma parte da trilha.";
    imprimirNarracao(narracao);
    cout << endl;
    cout << endl;

    while(true)
    {
        cout << "> ";
        narracao = "Fa�a sua escolha:";
        imprimirNarracao(narracao);
        cout << endl;
        
        cout << "> ";
        narracao = "[1] Checar invent�rio";
        imprimirNarracao(narracao);
        cout << endl;

        cout << "> ";
        narracao = "[2] Seguir o caminho da decad�ncia";
        imprimirNarracao(narracao);
        cout << endl;

        cin >> escolha;

        if(escolha < 1 || escolha > 2)
        {
            cout << endl;
            cout << "Fa�a uma das escolhas propostas. N�o fuja disso.";
            pausar(4000);
            limparTela();
        }
        else
            break;
    }
    
    switch(escolha)
    {
        case 1:
            limparTela();
            cout << "Este � a sua bolsa. Aqui voc� pode guardar tudo o que achar em sua jornada." << endl;
            cout << endl;
            // imprimir inventario;
            break;
        case 2:
            introducaoEntrarNaFloresta(personagem);
            break;
    }

}

// fun��o que inicia o jogo e executa todos os comandos para o funcionamento dele
void iniciarJogo()
{
    Personagem personagem;
    
    criarPersonagem(personagem);
    
    // prologo();

    introducao(personagem);
}

// fun��o que finaliza o jogo mostrando uma pequena anima��o
void finalizarJogo()
{
    char confirmarSaida;
    
    limparTela();
    
    cout << "Tem certeza que deseja sair? [s/n]" << endl;
    cin >> confirmarSaida;
    
    switch(confirmarSaida)
    {
        case 's':
            cout << "\nFinalizando o jogo" << flush;
            
            imprimirAnimacaoPontos();
            
            limparTela();
            cout << "Jogo finalizado. Continue explorando por a�. At� mais.";
            
            exit(0);
            break;
        case 'n':
            break;
    }
}

// fun��o principal que executa todos os comandos necess�rios para iniciar e finalizar o jogo
int main()
{
    system("chcp 1252 > nul");

    int escolhaMenu;
       
    while(true)
    {
        escolhaMenu = imprimirMenuPrincipal();
        
        switch(escolhaMenu)
        {
            case 1:
                iniciarJogo();
                break;
            case 2:
                finalizarJogo();
                break;
        }
    }

    return 0;
}