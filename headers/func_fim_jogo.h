/*
FUNCAO FIM DE JOGO
*/
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
