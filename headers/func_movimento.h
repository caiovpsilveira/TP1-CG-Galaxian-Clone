/*
FUNCOES MOVIMENTO
*/

void movimentarInimigos(){
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
