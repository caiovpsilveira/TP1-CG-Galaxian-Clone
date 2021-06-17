#include <GL/glew.h>      // glew.h deve vir antes
#include <GL/freeglut.h>  // do freeglut.h
#include <stdio.h>

#define LARG_JANELA 500
#define ALT_JANELA 500

#define LARG_ORTHO 200
#define ALT_ORTHO 200

#define NUM_LINHAS_INIMIGOS 4
#define NUM_COLUNAS_INIMIGOS 7

#define NUM_VIDAS_JOGADOR 3;

#define LARG_JOGADOR 20
#define ALT_JOGADOR 20

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

struct jogador jogador;
struct inimigo vet_inimigos[NUM_LINHAS_INIMIGOS*NUM_COLUNAS_INIMIGOS];

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
        desenhaRetangulo(vet_inimigos[i].box);
    }
}

void desenhaMinhaCena()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0, 1, 0); //deve ser trocado por textura, por enquanto e para teste.
    desenhaJogador();
    desenhaInimigos();

    glutSwapBuffers();
}

void atualizaCena(int valorQualquer) {
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

void inicializaTudo()
{
    inicializaJogador();
    inicializaInimigos();
    glClearColor(1, 1, 1, 1);
}

void redimensionada(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, LARG_ORTHO, 0, ALT_ORTHO, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void teclaPressionada(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 'd':
        jogador.box.xpos+=2;
        break;
    case 'a':
        jogador.box.xpos-=2;
        break;
    case 27:
        exit(0);
        break;
    default:
        break;
    }
}

// função principal
int main(int argc, char** argv)
{
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
