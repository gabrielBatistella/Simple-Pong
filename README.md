# Simple-Pong
Trabalho final da disciplina SSC0140 - Sistemas Operacionais I - Pong simples implementado com threads e semáforos

Eduardo Neves Gomes da Silva - 13822710

Gabriel Takeshi Miyake Batistella - 11232198

Rogério Lopes Lübe - 10770113
# Manual de instalação

Para o funcionamento correto do jogo é necessário fazer o download da biblioteca SDL2 disponível em https://www.libsdl.org/

Instruções para a instalação para diversos sistemas operacionais estão disponíveis em https://wiki.libsdl.org/SDL2/Installation

Uma vez instalada a biblioteca, basta rodar o comando **make all** para compilar o código e o comando **make run** para executá-lo

# Utilização de Threads e Semáforos

O código é feito em 2 threads, sendo que a thread principal gerencia o loop de gameplay e uma thread auxiliar gerencia os inputs dos jogadores. Os semáforos são utilizados para 2 regiões críticas, a principal delas é a direção do movimento das raquetes, que é alterada pela thread auxiliar e lida pela thread principal, que processa o movimento das raquetes. A outra é o sinal de encerramento do jogo. Quando a thread auxiliar detecta um input de saída, ela acessa a região crítica e altera a flag de encerramento, comunicando à thread principal do encerramento da partida, concorrentemente, a thread principal processa o jogo, e se detecta a vitória de algum dos jogadores, acessa a região crítica e altera a flag de encerramento, fazendo com que a thread auxiliar encerre sua execução.

# Manual de Jogo

O projeto consiste de uma versão simples do clássico jogo Pong. 

## Comandos

**w** - Movimenta a raquete esquerda para cima

**s** - Movimenta a raquete esquerda para baixo

**&uarr;** - Movimenta a raquete direita para cima

**&darr;** - Movimenta a raquete direita para baixo

Uma bola se move pelo campo de jogo e o trabalho de cada jogador é rebater a bola com a sua raquete, evitando que ela passe das laterais da tela. O jogador que deixar a bola passar perde.

Boa diversão!
