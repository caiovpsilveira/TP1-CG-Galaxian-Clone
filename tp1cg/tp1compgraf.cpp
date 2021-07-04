
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <time.h>
#include "funcoes.h"


// função principal
int main(int argc, char** argv)
{
    srand(time(NULL));
    glutInit(&argc, argv);

    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(LARG_JANELA, ALT_JANELA);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("INVASORES");

    inicializaTudo();
    glutReshapeFunc(redimensionada);
    glutDisplayFunc(desenhaMinhaCena);
    glutKeyboardFunc(teclaPressionada);
    glutTimerFunc(0, atualizaCena, 33);

    glutMainLoop();
    return 0;
}
