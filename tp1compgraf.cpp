/*  Trabalho Pratico 1 - Computacao Grafica - CEFETMG
*   Autores: Abdul Kevin Klexis e Caio Vinicius Pereira Silveira
*   Julho 2021
*/

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <time.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include "headers/definicoes.h"
#include "headers/structs.h"
#include "headers/var_global.h"
#include "headers/inicializacao.h"
#include "headers/func_desenho.h"
#include "headers/func_movimento.h"
#include "headers/func_colisao.h"
#include "headers/func_atirar.h"
#include "headers/func_fim_jogo.h"

/*
CALLBACKS GLUT
*/
void desenhaMinhaCena(){
    glClear(GL_COLOR_BUFFER_BIT);

    //Lembrando do algoritmo do pintor: o que e desenhado por ultimo fica na frente
    //apesar de por enquanto TELA_PERDEU, TELA_GANHOU e TELA_CREDITOS fazerem as
    //mesmas coisas, deixar desse jeito facilita adicoes futuras
    switch(tela){
        case TELA_MENU:
            desenhaFundo();
            break;
        case TELA_JOGO:
            desenhaHUD();
            desenhaFundo();
            desenhaTiros();
            desenhaInimigos();
            desenhaJogador();
            if(ta_pausado){
                desenhaPause();
            }
            break;
        case TELA_PERDEU:
            desenhaFundo();
            break;
        case TELA_GANHOU:
            desenhaFundo();
            break;
        case TELA_CREDITOS:
            desenhaFundo();
            break;
		case TELA_CONTROLES:
			desenhaFundo();
			break;
        default:
            //nunca deveria entrar aqui
            break;
    }

    glutSwapBuffers();
}

void atualizaCena(int valorQualquer){   //GAME UPDATE

    if(!ta_pausado && tela==TELA_JOGO){  //se nao esta pausado
        movimentarInimigos();
        verificarColisaoInimigosJogador();

        gerarTirosInimigos();
        movimentarTiros();
        verificarColisaoTiros();

        verificarFimJogo();
        frames_desde_ultimo_tiro++;
    }
    glutPostRedisplay();
    glutTimerFunc(33, atualizaCena, 0); // por quê 33? 1000/33 = 30fps, 16:60
}

void redimensionada(int width, int height){
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, LARG_ORTHO, 0, ALT_ORTHO, ZNEAR_ORTHO, ZFAR_ORTHO);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void teclaPressionada(unsigned char key, int x, int y){

    switch(key){

        case 'P':
        case 'p':   //pause
            if(tela==TELA_JOGO){
                if(ta_pausado){
                    ta_pausado=false;
                    Mix_ResumeMusic();
                }
                else{
                    ta_pausado=true;
                    Mix_PauseMusic();
                }
            }
            break;

        case ' ': //espaco
            jogadorAtira();
            if(tela == TELA_MENU){ //esse if deve vir abaixo de jogador Atira para que quando comecar o jogo nao atirar "sozinho"
                reiniciarJogo();
                tela = TELA_JOGO;
            }
            break;
        case 'R':
        case 'r':
            if(tela==TELA_JOGO || tela==TELA_GANHOU || tela==TELA_PERDEU){
                reiniciarJogo();
            }
            break;

        case 27://esc
            if(tela==TELA_MENU){
                exit(0);
            }
            else if(tela==TELA_JOGO || tela==TELA_GANHOU || tela==TELA_PERDEU || tela==TELA_CREDITOS || tela==TELA_CONTROLES){
                ta_pausado=true; //necessario pro caso de TELA_JOGO
                Mix_PauseMusic();
                tela = TELA_MENU;
            }
            break;

        case 'A':
        case 'a':   //movimento esquerda
            if(!ta_pausado){
                if(jogador.box.xpos <= TAM_BORDA_ESQ){
                    jogador.box.xpos = TAM_BORDA_ESQ;
                }
                else{
                    jogador.box.xpos -= vel_jogador;
                }
            }
            break;

        case 'D':
        case 'd':   //movimento direita
            if(!ta_pausado){
                if(jogador.box.xpos+LARG_JOGADOR >= LARG_ORTHO-TAM_BORDA_DIR){
                    jogador.box.xpos = LARG_ORTHO-TAM_BORDA_DIR-jogador.box.larg;
                }
                else{
                    jogador.box.xpos += vel_jogador;
                }
            }
            break;

        case 'C':
        case 'c':   //creditos
            if(tela==TELA_MENU){
                tela = TELA_CREDITOS;
            }
            break;

        case 'M':
        case 'm':   //menu
            if(tela==TELA_GANHOU || tela==TELA_PERDEU || tela==TELA_CREDITOS || tela==TELA_CONTROLES){
                tela=TELA_MENU;
            }
            break;

        case 'Z':
        case 'z':   //instrucoes
            if(tela==TELA_MENU){
                tela=TELA_CONTROLES;
            }
            break;

        default:
            break;
    }
}

// função principal
int main(int argc, char** argv)
{
    srand(time(NULL));
    glutInit(&argc, argv);

    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(LARG_JANELA, ALT_JANELA);
    glutInitWindowPosition(600, 100);

    glutCreateWindow("INVASORES");

    inicializaTudo();
    glutReshapeFunc(redimensionada);
    glutDisplayFunc(desenhaMinhaCena);
    glutKeyboardFunc(teclaPressionada);
    glutTimerFunc(0, atualizaCena, 33);

    glutMainLoop();
    return 0;
}
