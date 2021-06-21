#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>  //temporario e para teste
#include <math.h>

#define LARG_JANELA 500
#define ALT_JANELA 500

#define LARG_ORTHO 200
#define ALT_ORTHO 200

#define NUM_LINHAS_INIMIGOS 4
#define NUM_COLUNAS_INIMIGOS 7

#define NUM_VIDAS_JOGADOR 3

#define NUM_TIROS_JOGADOR 30

#define LARG_JOGADOR 20
#define ALT_JOGADOR 20

#define ALT_TIRO 5
#define LARG_TIRO 3

#define FRAMES_INTERVALO_TIROS 10

struct retangulo{
    float xpos,
    ypos,
    larg,
    alt;
};

struct inimigo{
    struct retangulo box;
    int vidas;
    float vel_inimigo;
};

struct jogador{
    struct retangulo box;
    int vidas;
    float vel_jogador;
};

struct tiro{
    struct retangulo box;
    float vel_tiro;
    bool visivel;   //o tiro so sera desenhado e verificara colisoes se estiver true;
};

int indice_vet_tiros; //sera inicializada junto com o vet_tiros
int frames_desde_ultimo_tiro;

bool ta_pausado;

float vel_jogador = 2;
float vel_inimigo = 0.4;
float vel_tiro = 5;

struct jogador jogador;
struct tiro vet_tiros_jogador[NUM_TIROS_JOGADOR];
struct inimigo vet_inimigos[NUM_LINHAS_INIMIGOS*NUM_COLUNAS_INIMIGOS];

bool verificarColisao(struct retangulo rect1, struct retangulo rect2){
    bool retorno = false;
    if((rect1.xpos < rect2.xpos + rect2.larg) && (rect1.xpos + rect1.larg > rect2.xpos)){
        if((rect1.ypos < rect2.ypos + rect2.alt) && (rect1.ypos + rect1.alt > rect2.ypos)){
            retorno = true;
        }
    }
    return retorno;
}

void desenhaRetangulo(struct retangulo rect){
    glBegin(GL_POLYGON);
        glVertex3f(rect.xpos, rect.ypos, 0);
        glVertex3f(rect.xpos+rect.larg, rect.ypos, 0);
        glVertex3f(rect.xpos+rect.larg, rect.ypos+rect.alt, 0);
        glVertex3f(rect.xpos, rect.ypos+rect.alt, 0);
    glEnd();
}

void desenhaJogador(){
    desenhaRetangulo(jogador.box);
}

void desenhaInimigos(){
    int i;
    for(i=0;i<NUM_LINHAS_INIMIGOS*NUM_COLUNAS_INIMIGOS;i++){
        if(vet_inimigos[i].vidas>0){
            desenhaRetangulo(vet_inimigos[i].box);
        }
    }
}

void desenhaTiros(){
    int i;
    for(i=0;i<NUM_TIROS_JOGADOR;i++){
        if(vet_tiros_jogador[i].visivel){
            desenhaRetangulo(vet_tiros_jogador[i].box);
        }
    }
}

void movimentaInimigos(){
    int i=0;

    if((vet_inimigos[NUM_COLUNAS_INIMIGOS-1].box.xpos + vet_inimigos[NUM_COLUNAS_INIMIGOS-1].box.larg + vel_inimigo >= LARG_ORTHO) || (vet_inimigos[0].box.xpos + vel_inimigo <= 0)){
        vel_inimigo *= -1;  //inverter direcao movimento
        for(i=0;i<NUM_COLUNAS_INIMIGOS*NUM_LINHAS_INIMIGOS;i++){
            vet_inimigos[i].box.ypos -= vet_inimigos[i].box.alt/2;  //desce na vertical
        }
    }
    else{
        for(i=0;i<NUM_COLUNAS_INIMIGOS*NUM_LINHAS_INIMIGOS;i++){
            vet_inimigos[i].box.xpos += vel_inimigo;
        }
    }
}

void movimentarTirosEVerificarColisao(){
    int i,j;
    for(i=0;i<NUM_TIROS_JOGADOR;i++){
        if(vet_tiros_jogador[i].visivel){
            vet_tiros_jogador[i].box.ypos += vet_tiros_jogador[i].vel_tiro;
            for(j=0;j<NUM_LINHAS_INIMIGOS*NUM_COLUNAS_INIMIGOS;j++){
                if(vet_inimigos[j].vidas > 0){
                    if(verificarColisao(vet_tiros_jogador[i].box,vet_inimigos[j].box)){
                        vet_tiros_jogador[i].visivel = false;
                        vet_inimigos[j].vidas -=1;
                        break;
                    }
                }
            }
            //se chegou aqui entao nao colidiu com nenhum inimigo. verificar se saiu da tela.
            if(vet_tiros_jogador[i].box.ypos >= LARG_ORTHO){
                vet_tiros_jogador[i].visivel = false;
            }
        }
    }
}

void desenhaMinhaCena()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0, 1, 0); //deve ser trocado por textura, por enquanto e para teste.
    desenhaJogador();
    desenhaInimigos();
    desenhaTiros();

    glutSwapBuffers();
}

void atualizaCena(int valorQualquer) {  //FUNCAO DE UPDATE DO JOGO
    if(!ta_pausado){ //se nao esta pausado
        movimentaInimigos();
        movimentarTirosEVerificarColisao();
        frames_desde_ultimo_tiro++;
    }
    glutPostRedisplay();
    glutTimerFunc(33, atualizaCena, 0); // por quê 33? 1000/33 = 30fps, 16:60
}

void inicializaJogador(){
    jogador.box.larg=LARG_JOGADOR;
    jogador.box.alt=ALT_JOGADOR;
    jogador.box.xpos=(LARG_ORTHO-LARG_JOGADOR)/2;
    jogador.box.ypos=0;
    jogador.vidas = NUM_VIDAS_JOGADOR;
    jogador.vel_jogador = vel_jogador;
}

void inicializaInimigos(){
    int i, j, indice_vet;
    int espacamento_vertical=2;

    int largura_inimigo = 10;
    int altura_inimigo = largura_inimigo; //ser um quadrado.

    int espacamento_horizontal= (LARG_ORTHO - NUM_COLUNAS_INIMIGOS*largura_inimigo)/(NUM_COLUNAS_INIMIGOS);

    for(i=0;i<NUM_LINHAS_INIMIGOS;i++){
        for(j=0;j<NUM_COLUNAS_INIMIGOS;j++){
            indice_vet = i*NUM_COLUNAS_INIMIGOS + j;

            vet_inimigos[indice_vet].box.larg = largura_inimigo;
            vet_inimigos[indice_vet].box.alt = altura_inimigo;

            vet_inimigos[indice_vet].box.xpos = j*(largura_inimigo + espacamento_horizontal);
            vet_inimigos[indice_vet].box.ypos = (ALT_ORTHO-altura_inimigo) - i*(altura_inimigo+espacamento_vertical);

            vet_inimigos[indice_vet].vidas = 1;
            vet_inimigos[indice_vet].vel_inimigo = vel_inimigo;
        }
    }
}

void inicializaTiros(){
    //parte dos tiros do jogador
    frames_desde_ultimo_tiro = FRAMES_INTERVALO_TIROS;
    indice_vet_tiros = 0;
    int i;
    for(i=0;i<NUM_TIROS_JOGADOR;i++){
        vet_tiros_jogador[i].box.larg = LARG_TIRO;
        vet_tiros_jogador[i].box.alt = ALT_TIRO;
        vet_tiros_jogador[i].box.xpos = 0;
        vet_tiros_jogador[i].box.ypos = 0;
        vet_tiros_jogador[i].visivel = false;
        vet_tiros_jogador[i].vel_tiro = vel_tiro;
    }
}

void inicializaTudo(){
    ta_pausado = false;
    inicializaJogador();
    inicializaInimigos();
    inicializaTiros();
    glClearColor(1, 1, 1, 1);
}

void redimensionada(int width, int height){
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, LARG_ORTHO, 0, ALT_ORTHO, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void teclaPressionada(unsigned char key, int x, int y){
    switch(key)
    {
    case 'p':
        if(ta_pausado){
            ta_pausado=false;
        }
        else{
            ta_pausado=true;
        }
        break;
    case 'd':
        if(!ta_pausado){
            if(jogador.box.xpos+LARG_JOGADOR >= LARG_ORTHO){
                jogador.box.xpos = LARG_ORTHO-jogador.box.larg;
            }
            else{
                jogador.box.xpos += vel_jogador;
            }
        }
        break;
    case 'a':
        if(!ta_pausado){
            if(jogador.box.xpos <= 0){
                jogador.box.xpos = 0;
            }
            else{
                jogador.box.xpos -= vel_jogador;
            }
        }
        break;
    case ' ':
        if(!ta_pausado){
            if(frames_desde_ultimo_tiro>=FRAMES_INTERVALO_TIROS){
                frames_desde_ultimo_tiro = 0;

                vet_tiros_jogador[indice_vet_tiros].box.xpos = jogador.box.xpos + jogador.box.larg/2 - vet_tiros_jogador[indice_vet_tiros].box.larg/2; //comecar no meio do jogador
                vet_tiros_jogador[indice_vet_tiros].box.ypos = jogador.box.ypos; //comeca em baixo do quadrado do jogador, dentro dele.
                vet_tiros_jogador[indice_vet_tiros].visivel = true;

                indice_vet_tiros++;
                if(indice_vet_tiros == NUM_TIROS_JOGADOR){  //cuidado: se apertar muito rapido ele pode teleportar um tiro que ja esta na tela. adicionar limitacao ou aumentar muito o numero de tiros.
                    indice_vet_tiros = 0;
                }
            }
        }
        break;
    case 'r':
        inicializaTudo();
        break;
    case 27:
        exit(0);
        break;
    default:
        break;
    }
}

// função principal
int main(int argc, char** argv){
    glutInit(&argc, argv);

    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(LARG_JANELA, ALT_JANELA);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("Teste");

    inicializaTudo();

    glutDisplayFunc(desenhaMinhaCena);
    glutReshapeFunc(redimensionada);
    glutKeyboardFunc(teclaPressionada);
    glutTimerFunc(0, atualizaCena, 33);

    glutMainLoop();
    return 0;
}
