/*
STRUCTS
*/
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
