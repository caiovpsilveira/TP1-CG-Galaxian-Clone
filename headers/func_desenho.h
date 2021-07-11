/*
FUNCOES DESENHAR CENA
*/
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
