/*
VARIAVEIS GLOBAIS
*/
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
