/*
FUNCOES COLISAO
*/
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
