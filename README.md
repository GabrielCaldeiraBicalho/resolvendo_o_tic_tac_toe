# Jogo da Velha com Programação Concorrente

[![Status](https://img.shields.io/badge/status-concluído-brightgreen)]()

## 📝 Descrição

Este projeto implementa um jogo da velha (Tic‑Tac‑Toe) onde dois jogadores controlados pelo computador jogam de forma **concorrente** usando **threads** em C++. O objetivo é demonstrar o uso de mecanismos de sincronização (`std::mutex` e `std::condition_variable`) para evitar condições de corrida e coordenar a alternância de turnos entre os jogadores.

O código original apresentava erros de concorrência (acesso simultâneo ao tabuleiro, falta de controle de turno, saída embaralhada). A versão corrigida utiliza **exclusão mútua** e **sinalização** para garantir a correta execução do jogo.

## 🧩 Funcionalidades

- **Tabuleiro 3x3** exibido a cada jogada.
- Dois jogadores com estratégias diferentes:
  - **Jogador Sequencial** (`X`) – tenta posições em ordem (linha 0..2, coluna 0..2).
  - **Jogador Aleatório** (`O`) – tenta posições aleatórias até encontrar uma vazia.
- **Threads concorrentes** – cada jogador executa em uma thread separada.
- **Sincronização**:
  - `std::mutex` protege o acesso ao tabuleiro e variáveis de estado.
  - `std::condition_variable` gerencia a espera pelo turno correto.
- **Resultado final** – exibe o vencedor (`X` ou `O`) ou empate (`D`).

## 🚀 Como compilar e executar

### Requisitos
- Compilador C++ com suporte a C++11 (ou superior).
- Biblioteca `pthread` (em sistemas Unix/Linux) – o flag `-pthread` é necessário.

### Compilação (WSL)
```bash
g++ -std=c++11 -pthread tic_tac_toe.cpp -o tic_tac_toe
