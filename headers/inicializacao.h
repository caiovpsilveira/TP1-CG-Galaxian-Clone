/*
FUNCOES INICIALIZACAO
*/
void inicializaSDL(){
     SDL_Init(SDL_INIT_AUDIO);
     atexit(SDL_Quit);
     //screen = SDL_SetVideoMode( LARG_ORTHO, ALT_ORTHO, 32, SDL_SWSURFACE);
     Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 1024 ); //- inicializa SDL_Mixer
     atexit(Mix_CloseAudio);
}

void carregarEfeitosSonoros(){
     efeito_musica = Mix_LoadMUS("audios/SuperMarioBros.ogg");
     efeito_explosao = Mix_LoadWAV("audios/explode1.wav");
     efeito_tiro = Mix_LoadWAV("audios/fire.wav");
     efeito_perdeu = Mix_LoadWAV("audios/game_over.wav");
     efeito_ganhou = Mix_LoadWAV("audios/victory.wav");
}


GLuint carregaTextura(const char* caminho){
    GLuint idTextura = SOIL_load_OGL_texture(
                           caminho,
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );

    if (idTextura == 0){
        printf("Erro do SOIL ao carregar %s: '%s'\n", caminho, SOIL_last_result());
    }

    return idTextura;
}

void inicializaTexturas(){
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    texturaInimigo[0] = carregaTextura("texturas/inimigo1.png");
    texturaInimigo[1] = carregaTextura("texturas/inimigo2.png");
    texturaInimigo[2] = carregaTextura("texturas/inimigo3.png");

    texturaJogador = carregaTextura("texturas/jogador.png");
    texturaTiroInimigo = carregaTextura("texturas/tiro_inimigo.png");
    texturaTiroJogador = carregaTextura("texturas/tiro_jogador.png");

    texturaBordas = carregaTextura("texturas/bordas.png");
    texturaVida = carregaTextura("texturas/vida.png");
	texturaPause = carregaTextura("texturas/pause.png");

    texturaFundo[TELA_MENU] = carregaTextura("texturas/fundo_menu.png"); //0
    texturaFundo[TELA_JOGO] = carregaTextura("texturas/fundo_jogo.png"); //1
    texturaFundo[TELA_PERDEU] = carregaTextura("texturas/fundo_perdeu.png"); //2
    texturaFundo[TELA_GANHOU] = carregaTextura("texturas/fundo_ganhou.png"); //3
    texturaFundo[TELA_CREDITOS] = carregaTextura("texturas/fundo_creditos.png"); //4
	texturaFundo[TELA_CONTROLES] = carregaTextura("texturas/fundo_controles.png"); //5
}

void inicializaJogador(){
    jogador.box.larg=LARG_JOGADOR;
    jogador.box.alt=ALT_JOGADOR;
    jogador.box.xpos=TAM_BORDA_ESQ+(LARG_ORTHO-(TAM_BORDA_DIR+TAM_BORDA_ESQ)-jogador.box.larg)/2;
    jogador.box.ypos=TAM_BORDA_INF;
    jogador.vidas = NUM_VIDAS_JOGADOR;
    jogador.vel_jogador = vel_jogador;
    jogador.textura = texturaJogador;
}

void inicializaInimigos(){
    int i, j, indice_vet;
    int espacamento_vertical=2;

    int largura_inimigo = 10;
    int altura_inimigo = largura_inimigo; //ser um quadrado.

    int espacamento_horizontal= (LARG_ORTHO -(TAM_BORDA_ESQ+TAM_BORDA_DIR) - NUM_COLUNAS_INIMIGOS*largura_inimigo)/(NUM_COLUNAS_INIMIGOS);

    for(i=0; i<NUM_LINHAS_INIMIGOS; i++){
        for(j=0; j<NUM_COLUNAS_INIMIGOS; j++){
            indice_vet = i*NUM_COLUNAS_INIMIGOS + j;

            vet_inimigos[indice_vet].box.larg = largura_inimigo;
            vet_inimigos[indice_vet].box.alt = altura_inimigo;

            vet_inimigos[indice_vet].box.xpos = j*(largura_inimigo + espacamento_horizontal)+TAM_BORDA_ESQ;
            vet_inimigos[indice_vet].box.ypos = (ALT_ORTHO-TAM_BORDA_SUP-altura_inimigo) - i*(altura_inimigo+espacamento_vertical);

            vet_inimigos[indice_vet].vidas = 1;
            vet_inimigos[indice_vet].vel_inimigo = vel_inimigo;
        }
    }

    //atribuir uma textura aleatoria para cada inimigo
    for(i=0;i<NUM_COLUNAS_INIMIGOS*NUM_LINHAS_INIMIGOS;i++){
        vet_inimigos[i].textura = texturaInimigo[rand()%3]; //resto pelo numero de texturas inimigas
    }
}

void inicializaTiros(){
    frames_desde_ultimo_tiro = FRAMES_INTERVALO_TIROS; //garantir que o jogador possa atirar imediatamente apos comecar
    indice_vet_tiros_jog = 0;
    indice_vet_tiros_inim = 0;

    //parte dos tiros do jogador
    int i;
    for(i=0; i<NUM_TIROS_JOGADOR; i++){
        vet_tiros_jogador[i].box.larg = LARG_TIRO_JOG;
        vet_tiros_jogador[i].box.alt = ALT_TIRO_JOG;
        vet_tiros_jogador[i].box.xpos = 0;
        vet_tiros_jogador[i].box.ypos = 0;
        vet_tiros_jogador[i].visivel = false;
        vet_tiros_jogador[i].vel_tiro = VEL_TIRO_JOG;
        vet_tiros_jogador[i].textura = texturaTiroJogador;
    }

    //parte do tiro dos inimigos
    for(i=0; i<NUM_TIROS_INIMIGOS; i++){
        vet_tiros_inimigos[i].box.larg = LARG_TIRO_INIM;
        vet_tiros_inimigos[i].box.alt = ALT_TIRO_INIM;
        vet_tiros_inimigos[i].box.xpos = 0;
        vet_tiros_inimigos[i].box.ypos = 0;
        vet_tiros_inimigos[i].visivel = false;
        vet_tiros_inimigos[i].vel_tiro = VEL_TIRO_INIM;
        vet_tiros_inimigos[i].textura = texturaTiroInimigo;
    }
}

void inicializaTudo(){
    tela=TELA_MENU;

    inicializaSDL();
    carregarEfeitosSonoros();
    //Mix_PlayMusic(efeito_musica, -1 );//inicializar o som do jogo

    inicializaTexturas();
    ta_pausado = true;

    inicializaJogador();
    inicializaInimigos();
    inicializaTiros();

    glClearColor(1, 1, 1, 1);
}

void reiniciarJogo(){
    Mix_PlayMusic(efeito_musica, -1 );

    tela=TELA_JOGO;
    ta_pausado = false;

    inicializaJogador();
    inicializaInimigos();
    inicializaTiros();

    glClearColor(1, 1, 1, 1);
}
