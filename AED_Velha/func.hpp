#include <iostream>
#include <vector>
#include <climits>
using namespace std;

// Estrutura para armazenar uma jogada
struct Move {
    int row, col;
};

// Estrutura para armazenar informações sobre o jogo em cada nó da árvore
struct TreeNode {
    Move move;
    int score;
    vector<TreeNode*> children;
    void deleteTree(TreeNode* node);
    TreeNode(Move m) : move(m), score(0) {}
};

// Classe tabuleiro
class Board {
public:
    // Atributos
    char tab[3][3];

    // Construtor
    Board();

    // Métodos
    void deleteTree(TreeNode* node);
    void drawBoard() const;
    bool isFull() const;
    bool isWinner(char player) const;
    bool isValidMove(int row, int col) const;
    void makeMove(Move move, char player);
    int evaluate(char player);
    TreeNode* buildTree(char player);
    void buildSubTree(TreeNode* node, char player);
    int alphaBeta(TreeNode* node, int depth, int alpha, int beta, bool maximizingPlayer);
    Move getBestMove(int depth);
    Move getPlayerMove() const;
};

// Método auxiliar para construir a subárvore a partir de um nó
void Board::buildSubTree(TreeNode* node, char player) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (isValidMove(i, j)) {
                Move move = {i, j};
                TreeNode* childNode = new TreeNode(move);
                makeMove(move, player);
                childNode->score = evaluate(player);
                buildSubTree(childNode, (player == 'O') ? 'X' : 'O');
                node->children.push_back(childNode);
                makeMove(move, ' '); // Desfaz a jogada para explorar outras opções
            }
        }
    }
}

// Método para construir a árvore de decisão
TreeNode* Board::buildTree(char player) {
    TreeNode* rootNode = new TreeNode({-1, -1}); // Nó raiz (jogada inválida)
    buildSubTree(rootNode, player);
    return rootNode;
}

//Método para obter as jogadas do usuário
Move Board::getPlayerMove() const {
    Move move;
    cout << "Digite a linha (0, 1, 2): ";
    cin >> move.row;

    // Verifica se a entrada da linha é válida
    while (move.row < 0 || move.row >= 3) {
        cout << "Entrada inválida. Digite a linha novamente (0, 1, 2): ";
        cin >> move.row;
    }

    cout << "Digite a coluna (0, 1, 2): ";
    cin >> move.col;

    // Verifica se a entrada da coluna é válida
    while (move.col < 0 || move.col >= 3) {
        cout << "Entrada inválida. Digite a coluna novamente (0, 1, 2): ";
        cin >> move.col;
    }

    

    return move;
}

// Método para calcular a melhor jogada usando o algoritmo alpha-beta pruning
int Board::alphaBeta(TreeNode* node, int depth, int alpha, int beta, bool maximizingPlayer) {
    if (depth == 0 || isFull() || isWinner('O') || isWinner('X')) {
        return node->score;
    }

    if (maximizingPlayer) {
        int maxScore = INT_MIN;
        for (TreeNode* child : node->children) {
            int score = alphaBeta(child, depth - 1, alpha, beta, false);
            maxScore = max(maxScore, score);
            alpha = max(alpha, score);
            if (beta <= alpha) {
                break; // Beta cut-off
            }
        }
        return maxScore;
    } else {
        int minScore = INT_MAX;
        for (TreeNode* child : node->children) {
            int score = alphaBeta(child, depth - 1, alpha, beta, true);
            minScore = min(minScore, score);
            beta = min(beta, score);
            if (beta <= alpha) {
                break; // Alpha cut-off
            }
        }
        return minScore;
    }
}

// Método para obter a melhor jogada usando alpha-beta pruning
Move Board::getBestMove(int depth) {
    TreeNode* rootNode = buildTree('X');  // O jogador humano é 'X'
    int bestScore = INT_MIN;
    Move bestMove = {-1, -1};
    for (TreeNode* child : rootNode->children) {
        int score = alphaBeta(child, depth - 1, INT_MIN, INT_MAX, false);
        if (score > bestScore) {
            bestScore = score;
            bestMove = child->move;
        }
    }
    deleteTree(rootNode);  // Limpa a árvore após o uso
    return bestMove;
}


// Método para deletar a árvore
void Board::deleteTree(TreeNode* node) {
        for (TreeNode* child : node->children) {
            deleteTree(child);
        }
        delete node;
    }

// Construtor da classe Board
Board::Board() {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            tab[i][j] = ' ';
}

// Função para desenhar o tabuleiro
void Board::drawBoard() const {
cout << "  0   1   2\n";
    for (int i = 0; i < 3; ++i) {
        cout << i << " ";
        for (int j = 0; j < 3; ++j) {
            cout << tab[i][j];
            if (j < 2) cout << " | ";
        }
        cout << "\n";
        if (i < 2) cout << "  ----------\n";
    }
    cout << "\n";
}


// Função para verificar se o tabuleiro está totalmente preenchido
bool Board::isFull() const {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (tab[i][j] == ' ')
                return false;
    return true;
}

// Função para verificar se um jogador venceu
bool Board::isWinner(char player) const {
    for (int i = 0; i < 3; i++) {
        if ((tab[i][0] == player && tab[i][1] == player && tab[i][2] == player) ||
            (tab[0][i] == player && tab[1][i] == player && tab[2][i] == player) ||
            (tab[0][0] == player && tab[1][1] == player && tab[2][2] == player) ||
            (tab[0][2] == player && tab[1][1] == player && tab[2][0] == player)) {
            return true;
        }
    }
    return false;
}

// Função para verificar se uma jogada é válida
bool Board::isValidMove(int row, int col) const {
    return row >= 0 && row < 3 && col >= 0 && col < 3 && tab[row][col] == ' ';
}

// Função para realizar uma jogada
void Board::makeMove(Move move, char player) {
    tab[move.row][move.col] = player;
}

// Função para avaliar o estado do tabuleiro para um jogador
int Board::evaluate(char player) {
    if (isWinner(player))
        return player == 'O' ? 10 : -10;
    return 0;
}


