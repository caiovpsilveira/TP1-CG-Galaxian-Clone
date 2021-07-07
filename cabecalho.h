#include "biblioteca/GL/glew.h"
#include "biblioteca/GL/freeglut.h"
#include "biblioteca/SOIL/SOIL.h"
#include "biblioteca/SDL/SDL.h"
#include "biblioteca/SDL/SDL_mixer.h"

#define LARG_JANELA 800
#define ALT_JANELA 800

#define LARG_ORTHO 240
#define ALT_ORTHO 240
#define ZNEAR_ORTHO -1
#define ZFAR_ORTHO 1

#define TAM_BORDA_ESQ 20
#define TAM_BORDA_DIR 20
#define TAM_BORDA_INF 20
#define TAM_BORDA_SUP 20

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

#define ALT_TIRO_INIM 5
#define LARG_TIRO_INIM 3
#define VEL_TIRO_INIM 2

#define LARG_VIDA 10
#define ALT_VIDA 10

#define LARG_PAUSE 20
#define ALT_PAUSE 20

#define FRAMES_INTERVALO_TIROS 10

#define TELA_MENU 0
#define TELA_JOGO 1
#define TELA_PERDEU 2
#define TELA_GANHOU 3
#define TELA_CREDITOS 4
#define TELA_CONTROLES 5


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
    GLuint textura;
};

struct jogador{
    struct retangulo box;
    int vidas;
    float vel_jogador;
    GLuint textura;
};

struct tiro{
    struct retangulo box;
    float vel_tiro;
    bool visivel;   //o tiro so sera desenhado e verificara colisoes se estiver true;
    GLuint textura;
};

int tela=0;
int indice_vet_tiros_jog; //sera inicializada junto com o vet_tiros
int indice_vet_tiros_inim;
int frames_desde_ultimo_tiro;

bool ta_pausado = false;    //false - nao ta pausado; true - pausado.

float vel_jogador = 2;
float vel_inimigo = 0.4;

GLuint texturaInimigo[3];
GLuint texturaTiroInimigo;
GLuint texturaJogador;
GLuint texturaTiroJogador;
GLuint texturaBordas;
GLuint texturaVida;
GLuint texturaPause;
GLuint texturaFundo[6];

Mix_Music *efeito_musica = NULL; //- música de fundo
Mix_Chunk *efeito_explosao = NULL;  //- efeito sonoro mixável.
Mix_Chunk *efeito_tiro = NULL;
Mix_Chunk *efeito_ganhou = NULL;
Mix_Chunk *efeito_perdeu = NULL;

struct jogador jogador;

struct tiro vet_tiros_jogador[NUM_TIROS_JOGADOR];
struct tiro vet_tiros_inimigos[NUM_TIROS_INIMIGOS];
struct inimigo vet_inimigos[NUM_LINHAS_INIMIGOS*NUM_COLUNAS_INIMIGOS];


void inicializaSDL();
void carregarEfeitosSonoros();
GLuint carregaTextura(const char* caminho);
void inicializaTexturas();
void inicializaJogador();
void inicializaInimigos();
void inicializaTiros();
void inicializaTudo();
void reiniciarJogo();
bool verificarColisao(struct retangulo rect1, struct retangulo rect2);
void desenhaStructRetangulo(struct retangulo rect);
void desenhaRetanguloSemStruct(float xpos, float ypos, float larg, float alt);
void desenhaJogador();
void desenhaTiros();
void desenhaFundo();
void desenhaBordas();
void desenhaVidas();
void desenhaHUD();
void desenhaPause();
void movimentaInimigos();
void verificarColisaoInimigosJogador();
void gerarTirosInimigos();
void movimentarTiros();
void verificarColisaoTiros();
void jogadorAtira();
void verificarFimJogo();
void desenhaMinhaCena();
void atualizaCena(int valorQualquer);
void redimensionada(int width, int height);
void teclaPressionada(unsigned char key, int x, int y);
