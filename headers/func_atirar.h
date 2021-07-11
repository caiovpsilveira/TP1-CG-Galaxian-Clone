/*
FUNCOES GERAR TIROS
*/

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
