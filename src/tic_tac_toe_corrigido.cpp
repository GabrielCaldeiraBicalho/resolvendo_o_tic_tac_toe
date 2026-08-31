#include <iostream>
#include <random>
#include <thread>
#include <array>
#include <mutex>
#include <condition_variable>

class TicTacToe {
private:
    std::array<std::array<char, 3>, 3> board;
    char current_player;
    bool game_over;
    char winner;
    std::mutex mtx;
    std::condition_variable cv;
public:
    TicTacToe() {
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                board[i][j] = ' ';
            }
        }
        winner = '-';
        game_over = false;
        static std::mt19937 sorteiaJogador(static_cast<unsigned int>(time(0)));
        static std::uniform_int_distribution<int> distr(0, 1);
        current_player = distr(sorteiaJogador) == 0 ? 'X' : 'O';
    }
    void display_board() {
        std::system("clear");
        for(int i = 0; i < 3; i++){
            std::cout << board[i][0] << "|" << board[i][1] << "|" << board[i][2] << std::endl;
            if(i != 2){
                std::cout << "-----" << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
//==============================(A CORREÇÃO DO CÓDIGO AQUI)==============================
    // Método que bloqueia até ser a vez do jogador 'player'
    // Retorna false se o jogo já tiver terminado, true caso contrário
    bool wait_for_turn(char player) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this, player] {
            return (current_player == player) || game_over;
        });
        return !game_over;   // se game_over for true, sai do loop
    }
    // Realiza uma jogada (supõe que já é a vez do jogador)
    // Retorna true se a jogada foi efetuada, false se a célula estiver ocupada
    bool make_move(char player, int row, int col) {
        std::lock_guard<std::mutex> lock(mtx);
        // Verifica se a posição está livre e o jogo não acabou
        if (game_over || row < 0 || row > 2 || col < 0 || col > 2)
            return false;
        if (board[row][col] != ' ')
            return false;
        // Executa a jogada
        board[row][col] = player;
        display_board();   // exibe o tabuleiro atualizado
        // Verifica fim de jogo
        if (check_win(player)) {
            winner = player;
            game_over = true;
        } else if (check_draw()) {
            winner = 'D';
            game_over = true;
        }
        // Alterna o turno (se o jogo não acabou)
        if (!game_over) {
            current_player = (player == 'X') ? 'O' : 'X';
        }
        // Notifica a outra thread (que pode estar esperando seu turno)
        cv.notify_all();
        return true;
    }
//========================================================================================
    bool check_win(char player) {
        // linhas
        for(int i = 0; i < 3; i++){
            if(player == board[i][0] && player == board[i][1] && player == board[i][2]){
                return true;
            }
        }
        // colunas
        for(int i = 0; i < 3; i++){
            if(player == board[0][i] && player == board[1][i] && player == board[2][i]){
                return true;
            }
        }
        // diagonal
        if(player == board[0][0] && player == board[1][1] && player == board[2][2])
            return true;
        if(player == board[0][2] && player == board[1][1] && player == board[2][0])
            return true;
        return false;
    }
    bool check_draw() {
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                if(board[i][j] == ' ')
                    return false;
            }
        }
        return true;
    }
    char get_winner() {
        std::lock_guard<std::mutex> lock(mtx);
        return winner;
    }
};
class Player {
private:
    TicTacToe& game;
    char symbol;
    std::string strategy;
public:
    Player(TicTacToe& g, char s, std::string strat)
        : game(g), symbol(s), strategy(strat) {}

    void play() {
        while (game.wait_for_turn(symbol)) {
            // É a vez deste jogador
            if (strategy == "sequential")
                play_sequential();
            else
                play_random();
        }
    }
private:
    void play_sequential() {
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                if(game.make_move(symbol, i, j))
                    return;   // jogada bem‑sucedida
            }
        }
    }
    void play_random() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> distr(0, 2);
        while (true) {
            int l = distr(gen);
            int c = distr(gen);
            if (game.make_move(symbol, l, c))
                return;
        }
    }
};
int main() {
    TicTacToe tabuleiro;
    tabuleiro.display_board();   //tabuleiro vazio
    Player X(tabuleiro, 'X', "sequential");
    Player O(tabuleiro, 'O', "random");
    std::thread Jogador1(&Player::play, &X);
    std::thread Jogador2(&Player::play, &O);
    Jogador1.join();
    Jogador2.join();
    char vencedor = tabuleiro.get_winner();
    if(vencedor == 'D')
        std::cout << "Empate!\n";
    else
        std::cout << "Vencedor: " << vencedor << "\n";
    return 0;
}
