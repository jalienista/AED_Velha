//Bibliotecas
#include <iostream>
#include <vector>
#include <climits>
#include <C:\Users\jucgr\Downloads\AED_Project2-master\func.hpp>
using namespace std;

//Função main
int main() {
    Board board;
    bool playerTurn = true;

    // Computador faz a primeira jogada
    Move computerMove = board.getBestMove(9);
    board.makeMove(computerMove, 'O');
    cout << endl << "Jogada do Computador:" << endl << endl;

    // Loop do jogo até o jogador ou computador vencer
    while (!board.isWinner('X') && !board.isWinner('O') && !board.isFull()) {
        // Desenha o tabuleiro
        board.drawBoard();

        if (playerTurn) {  // Se for o turno do jogador
            Move playerMove;
            do {
                playerMove = board.getPlayerMove();
            } while (!board.isValidMove(playerMove.row, playerMove.col));

            board.makeMove(playerMove, 'X');  // Realiza a jogada
        } else {  // Se não for o turno do jogador
            computerMove = board.getBestMove(9);  // Obtém a jogada do computador
            board.makeMove(computerMove, 'O');    // Realiza a jogada
            cout << endl << "Jogada do Computador:" << endl << endl;
        }

        // Alterna entre a jogada do computador e do jogador
        playerTurn = !playerTurn;
    }

    // Exibe o resultado final
    board.drawBoard();
    if (board.isWinner('X')) {
        cout << "Voce venceu!\n";
    } else if (board.isWinner('O')) {
        cout << "O computador venceu!\n";
    } else {
        cout << "Empate!\n";
    }

    // Finalização do programa
    return 0;
}
