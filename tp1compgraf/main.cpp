#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>  //temporario e para teste

#define LARG_JANELA 500
#define ALT_JANELA 500

#define LARG_ORTHO 200
#define ALT_ORTHO 200

#define NUM_LINHAS_INIMIGOS 4
#define NUM_COLUNAS_INIMIGOS 7

#define NUM_VIDAS_JOGADOR 3

#define NUM_TIROS 5

#define LARG_JOGADOR 20
#define ALT_JOGADOR 20

#define ALT_TIRO 5
#define LARG_TIRO 3

struct retangulo{
    float xpos,
    ypos,
    larg,
    alt;
};

struct inimigo{
    struct retangulo box;
    int vida;
};

struct jogador{
    struct retangulo box;
    int vida;
};

struct tiro{
    struct retangulo box;
    float vel_y;
    bool visivel;   //o tiro so sera desenhado e verificara colisoes se estiver true;
};

float vel_inimigo = 0.4;
float vel_jogador = 2;
struct jogador jogador;
struct tiro vet_tiros[NUM_TIROS];
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
        if(vet_inimigos[i].vida>0){
            desenhaRetangulo(vet_inimigos[i].box);
        }
    }
}

void desenhaTiros(){
    int i;
    for(i=0;i<NUM_TIROS;i++){
        if(vet_tiros[i].visivel){
            desenhaRetangulo(vet_tiros[i].box);
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
    movimentaInimigos();
    glutPostRedisplay();
    glutTimerFunc(33, atualizaCena, 0); // por quê 33? 1000/33 = 30fps, 16:60
}

void inicializaJogador(){
    jogador.box.larg=LARG_JOGADOR;
    jogador.box.alt=ALT_JOGADOR;
    jogador.box.xpos=(LARG_ORTHO-LARG_JOGADOR)/2;
    jogador.box.ypos=0;
    jogador.vida = NUM_VIDAS_JOGADOR;
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

            vet_inimigos[indice_vet].vida = 1;
        }
    }
}

void inicializaTiros(){
    int i;
    for(i=0;i<NUM_TIROS;i++){
        vet_tiros[i].box.larg = LARG_TIRO;
        vet_tiros[i].box.alt = ALT_TIRO;
        vet_tiros[i].box.xpos = 0;
        vet_tiros[i].box.ypos = 0;
        vet_tiros[i].visivel = false;
    }
}

void inicializaTudo(){
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
    case 'd':
        if(jogador.box.xpos+LARG_JOGADOR >= LARG_ORTHO){
            jogador.box.xpos = LARG_ORTHO-jogador.box.larg;
        }
        else{
            jogador.box.xpos += vel_jogador;
        }
        break;
    case 'a':
        if(jogador.box.xpos <= 0){
            jogador.box.xpos = 0;
        }
        else{
            jogador.box.xpos += -vel_jogador;
        }
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
