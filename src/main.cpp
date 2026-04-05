#include <iostream>
#include <format>
#include <string>

#include "game.hpp"
#include "recorde.hpp"

using namespace std;

void mostrarMenu() {
    cout << endl;
    cout << "==============================" << endl;
    cout << "1. Jogar" << endl;
    cout << "2. Exibir recorde" << endl;
    cout << "3. Fechar jogo" << endl;
    cout << "==============================" << endl;
    cout << "Escolha uma opcao: ";
}

void mostrarMenuDificuldade() {
    cout << endl;
    cout << "--- Selecione a dificuldade ---" << endl;
    cout << "1. Modo Facil   (3 inimigos, movimento em zigue-zague)" << endl;
    cout << "2. Modo Dificil (1 inimigo que te persegue e acelera)" << endl;
    cout << "------------------------------" << endl;
    cout << "Escolha uma opcao: ";
}

int main() {
    Game game;
    bool gameRunning = true;
    while (gameRunning) {
        mostrarMenu();

        int opcao;
        cin >> opcao;
        cin.ignore();

        string pausa;

        switch (opcao) {
        case 1: {
            mostrarMenuDificuldade();
            int dificuldade;
            cin >> dificuldade;
            cin.ignore();
            if (dificuldade == 1) {
                game.start(false);
            } else if (dificuldade == 2) {
                game.start(true);
            } else {
                cout << "Opcao invalida." << endl;
            }
            break;
        }

        case 2:
            cout << endl;
            cout << "Recorde atual: " << std::format("{:.2f}", Recorde::ler()) << "s" << endl;
            cout << "Pressione ENTER para voltar ao menu...";
            getline(cin, pausa);
            break;

        case 3:
            cout << "Fechando jogo..." << endl;
            gameRunning = false;
            break;

        default:
            cout << "Opcao invalida. Tente novamente." << endl;
            break;
        }
    }

    return 0;
}
