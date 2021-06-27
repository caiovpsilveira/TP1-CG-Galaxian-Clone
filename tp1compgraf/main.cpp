#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <stdio.h>  //temporario e para teste
#include <stdlib.h>
#include <time.h>

#define LARG_JANELA 500
#define ALT_JANELA 500

#define LARG_ORTHO 200
#define ALT_ORTHO 200

#define NUM_LINHAS_INIMIGOS 4
#define NUM_COLUNAS_INIMIGOS 7

#define NUM_VIDAS_JOGADOR 3

#define NUM_TIROS_JOGADOR 30
#define NUM_TIROS_INIMIGOS 10

#define LARG_JOGADOR 20
#define ALT_JOGADOR 20

#define ALT_TIRO_JOG 5
#define LARG_TIRO_JOG 3
#define VEL_TIRO_JOG 5

#define ALT_TIRO_INIM 3
#define LARG_TIRO_INIM 2
#define VEL_TIRO_INIM 2

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

int indice_vet_tiros_jog; //sera inicializada junto com o vet_tiros
int indice_vet_tiros_inim;
int frames_desde_ultimo_tiro;

bool ta_pausado;    //false - nao ta pausado; true - pausado.

float vel_jogador = 2;
float vel_inimigo = 0.4;

struct jogador jogador;

struct tiro vet_tiros_jogador[NUM_TIROS_JOGADOR];
struct tiro vet_tiros_inimigos[NUM_TIROS_INIMIGOS];
struct inimigo vet_inimigos[NUM_LINHAS_INIMIGOS*NUM_COLUNAS_INIMIGOS];

struct Textura {
    GLuint id;
    char arquivo[50];
};
GLubyte texturaAtual = 0;
GLubyte qtdeTexturas = 9;
//definir texturar
static struct Textura texturas[] = { {0, "galaxia1.png"}, {0, "galaxia2.png"}, {0, "galaxia3.png"},{0, "nave1.png"}, {0, "nave2.png"}, {0, "nave2.png"},{0, "tiro1.png"}, {0, "tiro2.png"}, {0, "tiro3.png"}};


GLuint carregaTextura(const char* arquivo) {
    GLuint idTextura = SOIL_load_OGL_texture(
                           arquivo,
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );

    if (idTextura == 0) {
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }

    return idTextura;
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
    frames_desde_ultimo_tiro = FRAMES_INTERVALO_TIROS; //garantir que o jogador possa atirar imediatamente apos comecar
    indice_vet_tiros_jog = 0;
    indice_vet_tiros_inim = 0;

    //parte dos tiros do jogador
    int i;
    for(i=0;i<NUM_TIROS_JOGADOR;i++){
        vet_tiros_jogador[i].box.larg = LARG_TIRO_JOG;
        vet_tiros_jogador[i].box.alt = ALT_TIRO_JOG;
        vet_tiros_jogador[i].box.xpos = 0;
        vet_tiros_jogador[i].box.ypos = 0;
        vet_tiros_jogador[i].visivel = false;
        vet_tiros_jogador[i].vel_tiro = VEL_TIRO_JOG;
    }

    //parte do tiro dos inimigos
    for(i=0;i<NUM_TIROS_INIMIGOS;i++){
        vet_tiros_inimigos[i].box.larg = LARG_TIRO_INIM;
        vet_tiros_inimigos[i].box.alt = ALT_TIRO_INIM;
        vet_tiros_inimigos[i].box.xpos = 0;
        vet_tiros_inimigos[i].box.ypos = 0;
        vet_tiros_inimigos[i].visivel = false;
        vet_tiros_inimigos[i].vel_tiro = VEL_TIRO_INIM;
    }
}

void inicializaTudo(){
    ta_pausado = false;
    inicializaJogador();
    inicializaInimigos();
    inicializaTiros();
      // define qual é a cor do fundo
    glClearColor(1, 1, 1, 1); // branco
      // habilita mesclagem de cores, para termos suporte a texturas
    // com transparência
    glEnable(GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    int i;
    for(i=0;i<qtdeTexturas;i++){
    texturas[i].id=carregaTextura(texturas[i].arquivo);
    }

  //  idTexturaDaNave = carregaTextura("galaxy.png");
}

bool verificarColisao(struct retangulo rect1, struct retangulo rect2){
    bool retorno = false;
    if((rect1.xpos < rect2.xpos + rect2.larg) && (rect1.xpos + rect1.larg > rect2.xpos)){
        if((rect1.ypos < rect2.ypos + rect2.alt) && (rect1.ypos + rect1.alt > rect2.ypos)){
            retorno = true;
        }
    }
    return retorno;
}

void desenhaRetangulo(struct retangulo rect,GLuint id){
// Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);

    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, id);
    glBegin(GL_POLYGON);
    glTexCoord2f(0, 0);
        glVertex3f(rect.xpos, rect.ypos, 0);
    glTexCoord2f(1, 0);
        glVertex3f(rect.xpos+rect.larg, rect.ypos, 0);
    glTexCoord2f(1, 1);
        glVertex3f(rect.xpos+rect.larg, rect.ypos+rect.alt, 0);
    glTexCoord2f(0, 1);
        glVertex3f(rect.xpos, rect.ypos+rect.alt, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
void desenhaGalaxia(){
// Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);

    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, texturas[0].id);
    glBegin(GL_POLYGON);
    glTexCoord2f(0, 0);
        glVertex3f(0, 0, 0);
    glTexCoord2f(1, 0);
        glVertex3f(LARG_ORTHO, 0, 0);
    glTexCoord2f(1, 1);
        glVertex3f(LARG_ORTHO, ALT_ORTHO, 0);
    glTexCoord2f(0, 1);
        glVertex3f(0, ALT_ORTHO, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

}

void desenhaJogador(){
    desenhaRetangulo(jogador.box,texturas[3].id);
}

void desenhaInimigos(){
    int i;
    for(i=0;i<NUM_LINHAS_INIMIGOS*NUM_COLUNAS_INIMIGOS;i++){
        if(vet_inimigos[i].vidas>0){
            desenhaRetangulo(vet_inimigos[i].box,texturas[4].id);
        }
    }
}

void desenhaTiros(){
    int i;
    for(i=0;i<NUM_TIROS_JOGADOR;i++){
        if(vet_tiros_jogador[i].visivel){
            desenhaRetangulo(vet_tiros_jogador[i].box, texturas[8].id);
        }
    }

    for(i=0;i<NUM_TIROS_INIMIGOS;i++){
        if(vet_tiros_inimigos[i].visivel){
            desenhaRetangulo(vet_tiros_inimigos[i].box,texturas[6].id);
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

            if(vet_inimigos[i].vidas>0){ //para que um inimigo morto nao atire
                if(rand()%2000<=1){
                    vet_tiros_inimigos[indice_vet_tiros_inim].box.xpos = vet_inimigos[i].box.xpos + vet_inimigos[i].box.larg/2;
                    vet_tiros_inimigos[indice_vet_tiros_inim].box.ypos = vet_inimigos[i].box.ypos;
                    vet_tiros_inimigos[indice_vet_tiros_inim].visivel = true;
                    indice_vet_tiros_inim++;
                    if(indice_vet_tiros_inim==NUM_TIROS_INIMIGOS){
                        indice_vet_tiros_inim = 0;
                    }
                }
            }
        }
    }
}

void movimentarTirosEVerificarColisao(){
    int i,j;
    for(i=0;i<NUM_TIROS_JOGADOR;i++){
        if(vet_tiros_jogador[i].visivel){
            vet_tiros_jogador[i].box.ypos += vet_tiros_jogador[i].vel_tiro;
            //verificando colisao com algum inimigo
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

    //parte dos tiros inimigos
    for(i=0;i<NUM_TIROS_INIMIGOS;i++){
        if(vet_tiros_inimigos[i].visivel){
            vet_tiros_inimigos[i].box.ypos -= vet_tiros_inimigos[i].vel_tiro; //coemntar para poder testar colisoes com os tiros parados.
            //verificando colisao com o jogador
            if(verificarColisao(vet_tiros_inimigos[i].box,jogador.box)){
                jogador.vidas--;
                vet_tiros_inimigos[i].visivel = false;
                //printf("%d\n",jogador.vidas);   //teste
                //nao pode haver um break aqui, visto que deve verificar todos os tiros.
            }
        }
    }
}

void verificarFimJogo(){
    int i, inim_vivos = 0;
    bool perdeu = false; //obs: nao perder é diferente de ganhar.
    if(jogador.vidas>0){
        for(i=0;i<NUM_COLUNAS_INIMIGOS*NUM_LINHAS_INIMIGOS;i++){
            if(vet_inimigos[i].vidas>0){ //o inimigo esta vivo.
                inim_vivos++;
                if(vet_inimigos[i].box.ypos<=0){
                    perdeu = true;
                }
            }
        }
    }
    else{ //num vidas jogador = 0;
        perdeu = true;
    }

    if(perdeu){
        printf("perdeu"); //temporario teste;
        inicializaTudo();
    }
    else if(inim_vivos==0){
        printf("ganhou"); //temporario teste;
        inicializaTudo();
    }
}

void desenhaMinhaCena(){
    glClear(GL_COLOR_BUFFER_BIT);

    //glColor3f(1, 1, 1); //deve ser trocado por textura, por enquanto e para teste.
    desenhaGalaxia();
    desenhaJogador();
    desenhaInimigos();
    desenhaTiros();

    glutSwapBuffers();
}

void atualizaCena(int valorQualquer){  //FUNCAO DE UPDATE DO JOGO
    if(!ta_pausado){ //se nao esta pausado
        movimentaInimigos();
        movimentarTirosEVerificarColisao();
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

                vet_tiros_jogador[indice_vet_tiros_jog].box.xpos = jogador.box.xpos + jogador.box.larg/2 - vet_tiros_jogador[indice_vet_tiros_jog].box.larg/2; //comecar no meio do jogador
                vet_tiros_jogador[indice_vet_tiros_jog].box.ypos = jogador.box.ypos; //comeca em baixo do quadrado do jogador, dentro dele.
                vet_tiros_jogador[indice_vet_tiros_jog].visivel = true;

                indice_vet_tiros_jog++;
                if(indice_vet_tiros_jog == NUM_TIROS_JOGADOR){  //cuidado: se apertar muito rapido ele pode teleportar um tiro que ja esta na tela. adicionar limitacao ou aumentar muito o numero de tiros.
                    indice_vet_tiros_jog = 0;
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
    srand(time(NULL));
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
