#include "cabecalho.h"

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

bool verificarColisao(struct retangulo rect1, struct retangulo rect2){
    bool retorno = false;
    if((rect1.xpos < rect2.xpos + rect2.larg) && (rect1.xpos + rect1.larg > rect2.xpos)){
        if((rect1.ypos < rect2.ypos + rect2.alt) && (rect1.ypos + rect1.alt > rect2.ypos)){
            retorno = true;
            Mix_PlayChannel( -1, efeito_explosao, 0 );//efeito de explosão
        }
    }
    return retorno;
}

void desenhaStructRetangulo(struct retangulo rect){
    glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(rect.xpos, rect.ypos, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(rect.xpos+rect.larg, rect.ypos, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(rect.xpos+rect.larg, rect.ypos+rect.alt, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(rect.xpos, rect.ypos+rect.alt, 0.0f);
    glEnd();
}

void desenhaRetanguloSemStruct(float xpos, float ypos, float larg, float alt){
    glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(xpos, ypos, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(xpos+larg, ypos, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(xpos+larg, ypos+alt, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(xpos, ypos+alt, 0.0f);
    glEnd();
}

void desenhaJogador(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, jogador.textura);
    desenhaStructRetangulo(jogador.box);
    glDisable(GL_TEXTURE_2D);
}

void desenhaInimigos(){
    int i;
    glEnable(GL_TEXTURE_2D);
    for(i=0; i<NUM_LINHAS_INIMIGOS*NUM_COLUNAS_INIMIGOS; i++){
        if(vet_inimigos[i].vidas>0){
            glBindTexture(GL_TEXTURE_2D, vet_inimigos[i].textura);
            desenhaStructRetangulo(vet_inimigos[i].box);
        }
    }
    glDisable(GL_TEXTURE_2D);
}

void desenhaTiros(){
    int i;
    glEnable(GL_TEXTURE_2D);

    for(i=0; i<NUM_TIROS_JOGADOR; i++){
        if(vet_tiros_jogador[i].visivel){
            glBindTexture(GL_TEXTURE_2D, vet_tiros_jogador[i].textura);
            desenhaStructRetangulo(vet_tiros_jogador[i].box);
        }
    }

    for(i=0; i<NUM_TIROS_INIMIGOS; i++){
        if(vet_tiros_inimigos[i].visivel){
            glBindTexture(GL_TEXTURE_2D, vet_tiros_inimigos[i].textura);
            desenhaStructRetangulo(vet_tiros_inimigos[i].box);
        }
    }
    glDisable(GL_TEXTURE_2D);
}

void desenhaFundo(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturaFundo[tela]);
    //A TELA_JOGO POSSUI HUD (ha bordas)
    if(tela==TELA_JOGO){
        desenhaRetanguloSemStruct(TAM_BORDA_ESQ,TAM_BORDA_INF, LARG_ORTHO-(TAM_BORDA_DIR+TAM_BORDA_ESQ), ALT_ORTHO-(TAM_BORDA_SUP+TAM_BORDA_INF));
    }
    else{
        desenhaRetanguloSemStruct(0,0,LARG_ORTHO,ALT_ORTHO);
    }
    glDisable(GL_TEXTURE_2D);
}

void desenhaBordas(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturaBordas);
    desenhaRetanguloSemStruct(0,0,LARG_ORTHO,ALT_ORTHO);
    glDisable(GL_TEXTURE_2D);
}

void desenhaVidas(){
    int i;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturaVida);
    for(i=0;i<jogador.vidas;i++){
        desenhaRetanguloSemStruct(TAM_BORDA_ESQ+(2+LARG_VIDA)*i,(TAM_BORDA_INF-ALT_VIDA)/2, LARG_VIDA, ALT_VIDA); //2+LARG_VIDA: espaco horizontal entre xpos dos retangulos
    }
    glDisable(GL_TEXTURE_2D);
}

void desenhaHUD(){
    desenhaBordas();
    desenhaVidas();
    //escrevePontos();
}

void desenhaPause(){
	//centralizar o pause dentro das bordas do jogo
	float xpos = TAM_BORDA_ESQ+(LARG_ORTHO-(TAM_BORDA_DIR+TAM_BORDA_ESQ)-LARG_PAUSE)/2;
	float ypos = TAM_BORDA_INF+(ALT_ORTHO-(TAM_BORDA_INF+TAM_BORDA_SUP)-ALT_PAUSE)/2;
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturaPause);
	desenhaRetanguloSemStruct(xpos,ypos,LARG_PAUSE, ALT_PAUSE);
	glDisable(GL_TEXTURE_2D);
}

void movimentaInimigos(){
    int i=0;

    //se a soma da posicao do ultimo inimigo da linha + incremento e maior do que a largura ou menor do que 0
    if((vet_inimigos[NUM_COLUNAS_INIMIGOS-1].box.xpos + vet_inimigos[NUM_COLUNAS_INIMIGOS-1].box.larg + vel_inimigo >= LARG_ORTHO-TAM_BORDA_DIR) || (vet_inimigos[0].box.xpos + vel_inimigo <= TAM_BORDA_ESQ)){
        vel_inimigo *= -1;  //inverter direcao movimento
        for(i=0; i<NUM_COLUNAS_INIMIGOS*NUM_LINHAS_INIMIGOS; i++){
            vet_inimigos[i].box.ypos -= vet_inimigos[i].box.alt/2;  //desce na vertical
        }
    }
    else{
        for(i=0; i<NUM_COLUNAS_INIMIGOS*NUM_LINHAS_INIMIGOS; i++){
            vet_inimigos[i].box.xpos += vel_inimigo;
        }
    }
}

void verificarColisaoInimigosJogador(){
    int i;
    for(i=0;i<NUM_COLUNAS_INIMIGOS*NUM_LINHAS_INIMIGOS;i++){
        if(vet_inimigos[i].vidas>0){
            if(verificarColisao(vet_inimigos[i].box,jogador.box)){
                jogador.vidas--;
                vet_inimigos[i].vidas--;
            }
        }
    }
}

void gerarTirosInimigos(){
    int i;
    for(i=0;i<NUM_COLUNAS_INIMIGOS*NUM_LINHAS_INIMIGOS;i++){
        if(vet_inimigos[i].vidas>0){  //para que um inimigo morto nao atire
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

void movimentarTiros(){
    int i;
    //parte dos tiros do jogador
    for(i=0; i<NUM_TIROS_JOGADOR; i++){
        if(vet_tiros_jogador[i].visivel){
            vet_tiros_jogador[i].box.ypos += vet_tiros_jogador[i].vel_tiro;
            //verificar se saiu da tela
            if(vet_tiros_jogador[i].box.ypos >= ALT_ORTHO-TAM_BORDA_SUP-vet_tiros_jogador[i].box.alt){
                vet_tiros_jogador[i].visivel = false;
            }
        }
    }

    //parte dos tiros inimigos
    for(i=0; i<NUM_TIROS_INIMIGOS; i++){
        if(vet_tiros_inimigos[i].visivel){
            vet_tiros_inimigos[i].box.ypos -= vet_tiros_inimigos[i].vel_tiro;
            //verificar se saiu da tela
            if(vet_tiros_inimigos[i].box.ypos <= TAM_BORDA_INF){
                vet_tiros_inimigos[i].visivel = false;
            }
        }
    }
}

void verificarColisaoTiros(){
    int i,j;

    //TIROS DO JOGADOR
    for(i=0;i<NUM_TIROS_JOGADOR;i++){
        for(j=0; j<NUM_LINHAS_INIMIGOS*NUM_COLUNAS_INIMIGOS; j++){
            if(vet_tiros_jogador[i].visivel){
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

    //TIROS DOS INIMIGOS
    for(i=0; i<NUM_TIROS_INIMIGOS; i++){
        if(vet_tiros_inimigos[i].visivel){
            if(verificarColisao(vet_tiros_inimigos[i].box,jogador.box)){
                jogador.vidas--;
                vet_tiros_inimigos[i].visivel = false;
                //printf("%d\n",jogador.vidas);
                //nao pode haver um break aqui, visto que deve verificar todos os tiros.
            }
        }
    }
}

void jogadorAtira(){
    if(!ta_pausado && tela==TELA_JOGO){
        if(frames_desde_ultimo_tiro>=FRAMES_INTERVALO_TIROS){ //evitar spam de tiros
            Mix_PlayChannel( -1, efeito_tiro, 0 );//efeito do tiro jogador
            frames_desde_ultimo_tiro = 0;

            vet_tiros_jogador[indice_vet_tiros_jog].box.xpos = jogador.box.xpos + jogador.box.larg/2 - vet_tiros_jogador[indice_vet_tiros_jog].box.larg/2; //comecar no meio do jogador
            vet_tiros_jogador[indice_vet_tiros_jog].box.ypos = jogador.box.ypos; //comeca em baixo do quadrado do jogador, dentro dele.
            vet_tiros_jogador[indice_vet_tiros_jog].visivel = true;

            indice_vet_tiros_jog++;
            if(indice_vet_tiros_jog == NUM_TIROS_JOGADOR){
                indice_vet_tiros_jog = 0;
            }
        }
    }
}

void verificarFimJogo(){
    int i, inim_vivos = 0;
    bool perdeu = false; //obs: nao perder é diferente de ganhar.

    //verificacao de perder
    if(jogador.vidas>0){

        //se algum inimigo chegou ao solo
        for(i=0; i<NUM_COLUNAS_INIMIGOS*NUM_LINHAS_INIMIGOS; i++){
            if(vet_inimigos[i].vidas>0){  //o inimigo esta vivo.
                inim_vivos++;
                if(vet_inimigos[i].box.ypos<=TAM_BORDA_INF){
                    perdeu = true;
                }
            }
        }
    }
    else{   //num vidas jogador = 0;
        perdeu = true;
    }

    if(tela==TELA_JOGO){    //possibilitar alternar telas apos fim
        if(perdeu){
            Mix_PauseMusic();
            Mix_PlayChannel(-1, efeito_perdeu, 0 );
            tela=TELA_PERDEU;
            ta_pausado = true;
        }
        else if(inim_vivos==0){ //ganhou
            Mix_PauseMusic();
            Mix_PlayChannel(-1, efeito_ganhou, 0 );
            tela=TELA_GANHOU;
            ta_pausado = true;
        }
    }
}

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
        movimentaInimigos();
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
    //A ORDEM DO SWITCH SERA A PRIORIDADE DE SIMULTANIEDADE
    switch(key){
    case 'P':
    case 'p':
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

    case ' ':

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
    case 'a':
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
    case 'd':
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
    case 'c':
        if(tela==TELA_MENU){
            tela = TELA_CREDITOS;
        }
        break;
    case 'M':
    case 'm':
        if(tela==TELA_GANHOU || tela==TELA_PERDEU || tela==TELA_CREDITOS || tela==TELA_CONTROLES){
            tela=TELA_MENU;
        }
        break;
    case 'Z':
	case 'z':
		if(tela==TELA_MENU){
			tela=TELA_CONTROLES;
		}
		break;

    default:
        break;
    }
}
