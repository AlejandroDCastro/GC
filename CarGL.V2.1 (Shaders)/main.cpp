
/****************************************************************************************************

    CarGL (main.cpp)

    Grado en Ingeniería Multimedia.
    Práctica de Gráficos por Computador.
    OpenGL con Shaders.
  ------------------------------------------------------------------------------------------

    CarsGL (C) 2012-2016 - Juan Antonio Puchol García (puchol@dccia.ua.es)

    Changelog:

    Versión 1.0 (Noviembre 2012)
        - Versión inicial para Code::Blocks 10.05

    Versión 1.1 (Noviembre 2012)
        - Arreglado el bug de selección incorrecta debido al panel inferior de controles

    Versión 1.2 (Octubre 2013 )
        - Adaptación de la librería GLUI a Code::Blocks 12.11 y 13.12

    Versión 1.3 (Octubre 2014 )
        - Adaptación de la librería GLUI a Code::Blocks 13.12

    Versión 2.0 (Octubre 2015 )
        - Adaptación del código empleando Shaders (Modern OpenGL Graphics)

    Versión 2.1 (Septiembre 2016 )
        - Modificación de los scrollbars de las luces para manipular la posisión (compatible con Code::Blocks 16.01)

    NOTA:   Para que esta aplicación compile se necesita tener copiados los
            siguientes ficheros desde la carpeta "Para Copiar":

            glut32.dll  -> C:\Windows\system

            Si es un Windows de 32 bits:
            glew32s.lib -> C:\Program Files\CodeBlocks\MinGW\lib
            glew32.lib  -> C:\Program Files\CodeBlocks\MinGW\lib
            libglui32.a -> C:\Program Files\CodeBlocks\MinGW\lib
            glut32.lib  -> C:\Program Files\CodeBlocks\MinGW\lib

            glui.h      -> C:\Program Files\CodeBlocks\MinGW\include\GL
            glut.h      -> C:\Program Files\CodeBlocks\MinGW\include\GL
            glew.h      -> C:\Program Files\CodeBlocks\MinGW\include\GL

            y la carpeta completa
            glm         -> C:\Program Files\CodeBlocks\MinGW\include

            Si es un Windows de 64 bits:
            glew32s.lib -> C:\Program Files (x86)\CodeBlocks\MinGW\lib
            glew32.lib  -> C:\Program Files (x86)\CodeBlocks\MinGW\lib
            libglui32.a -> C:\Program Files (x86)\CodeBlocks\MinGW\lib
            glut32.lib  -> C:\Program Files (x86)\CodeBlocks\MinGW\lib

            glui.h      -> C:\Program Files (x86)\CodeBlocks\MinGW\include\GL
            glut.h      -> C:\Program Files (x86)\CodeBlocks\MinGW\include\GL
            glew.h      -> C:\Program Files (x86)\CodeBlocks\MinGW\include\GL

            y la carpeta completa
            glm         -> C:\Program Files (x86)\CodeBlocks\MinGW\include


            ACLARACIÓN:
                Hay varias librerías libglui32.a en la carpeta "otros"
                - Renombrar libglui32.10.05.a a libglui32.a para usar Code::Blocks 10.05
                - Renombrar libglui32.12.11.a a libglui32.a para usar Code::Blocks 12.11
                - Renombrar libglui32.13.12.a a libglui32.a para usar Code::Blocks 13.12 o superior

****************************************************************************************************/

#include "Objects.h"
#include <GL\glui.h>

/**************************************** myGlutKeyboard() **********/

void Keyboard(unsigned char Key, int x, int y)
{
    switch(Key)
    {
        case 27:
        case 'q':
            exit(0);
            break;

        case 'O':
        case 'o':
            escena.tipoVista=1;
            gui.Reshape(x,y);

            break;

        case 'P':
        case 'p':
            escena.tipoVista=0;
            gui.Reshape(x,y);
            break;

    }

    glutPostRedisplay();
}

/**************************************** mySpecialKey() *************/

static void SpecialKey(int key, int x, int y)
{
    if (escena.ultimaSelecccion > 0) {
        TPrimitiva *car = escena.GetCar(escena.seleccion);

        // Formula posicion del coche
        float gRotacion = (car->ry+180)*PI/180.0;
        float senoAngulo = sin(gRotacion);
        float cosenoAngulo = cos(gRotacion);

        switch (key)
        {
            case GLUT_KEY_UP:   // El coche avanza
                car->rr -= ROTACION_RUEDA;
                car->tz += VELOCIDAD*cosenoAngulo;
                car->tx += VELOCIDAD*senoAngulo;
                car->ry += 0.03*(car->anguloRuedas);
                break;
            case GLUT_KEY_DOWN:   // El coche retrocede
                car->rr += ROTACION_RUEDA;
                car->tz -= VELOCIDAD*cosenoAngulo;
                car->tx -= VELOCIDAD*senoAngulo;
                car->ry -= 0.03*(car->anguloRuedas);
                break;
            case GLUT_KEY_RIGHT:
                if (car->anguloRuedas > -45) {
                    car->anguloRuedas -= 3;
                }
                break;
            case GLUT_KEY_LEFT:
                if (car->anguloRuedas < 45) {
                    car->anguloRuedas += 3;
                }
                break;

        }
    }
    glutPostRedisplay();
}

/***************************************** myGlutMenu() ***********/

void Menu( int value )
{
  Keyboard( value, 0, 0 );
}

void Mouse(int button, int button_state, int x, int y )
{
    gui.Mouse(button, button_state, x, y);
}

void Render()
{
    escena.Render();
}

void Idle()
{
    gui.Idle();
}

void Reshape(int x, int y){
    gui.Reshape(x, y);
}

void Motion(int x, int y){
    gui.Motion(x, y);
}

/**************************************** main() ********************/

int main(int argc, char* argv[])
{
    // Inicializa GLUT and crea la ventana principal
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE );
    glutInitWindowPosition( 50, 50 );
    glutInitWindowSize( 1400, 800 );

    int main_window = glutCreateWindow( "CarGL V2.0 (2015) con Shaders" );

    // Inicializa los valores de OpenGL para esta Aplicación
    escena.InitGL();
    gui.Init(main_window);

    glutDisplayFunc( Render );
    GLUI_Master.set_glutReshapeFunc( Reshape );
    GLUI_Master.set_glutKeyboardFunc( Keyboard );
    GLUI_Master.set_glutSpecialFunc( SpecialKey );
    GLUI_Master.set_glutMouseFunc( Mouse );
    glutMotionFunc( Motion );

    /**** We register the idle callback with GLUI, *not* with GLUT ****/
    GLUI_Master.set_glutIdleFunc( Idle );


    // Creamos la carretera...
    TPrimitiva *road1 = new TPrimitiva(CARRETERA_ID, CARRETERA_ID);

    /*
            z-
            |
            |
            |
     -x-----|-----x+
            |
            |
            |
            z+

    */


    // Creamos las aceras...
    TPrimitiva *acera1 = new TPrimitiva(ACERAMANZANA_ID, ACERAMANZANA_ID);
    TPrimitiva *acera2 = new TPrimitiva(ACERAMANZANA_ID, ACERAMANZANA_ID);
    TPrimitiva *acera3 = new TPrimitiva(ACERAMANZANA_ID, ACERAMANZANA_ID);
    TPrimitiva *acera4 = new TPrimitiva(ACERAMANZANA_ID, ACERAMANZANA_ID);
    TPrimitiva *acera5 = new TPrimitiva(ACERAMANZANA_ID, ACERAMANZANA_ID);
    TPrimitiva *acera6 = new TPrimitiva(ACERAMANZANA_ID, ACERAMANZANA_ID);
    TPrimitiva *acera7 = new TPrimitiva(ACERAMANZANA_ID, ACERAMANZANA_ID);
    TPrimitiva *acera8 = new TPrimitiva(ACERAMANZANA_ID, ACERAMANZANA_ID);

    acera1->tz = 1.33;
    acera1->tx = -0.49;
    acera2->tz = -1.245;
    acera2->tx = 4;
    acera2->ry = 180;

    acera3->tz = 1.33+2;
    acera3->tx = -0.49;
    acera4->tz = -1.245-2;
    acera4->tx = 4;
    acera4->ry = 180;

    acera5->tz = 1.33;
    acera5->tx = -0.49+4.49;
    acera6->tz = -1.245;
    acera6->tx = 4-4.49;
    acera6->ry = 180;

    acera7->tz = 1.33+2;
    acera7->tx = -0.49+4.49;
    acera8->tz = -1.245-2;
    acera8->tx = 4-4.49;
    acera8->ry = 180;


    // Creamos los edificios
    TPrimitiva *edificio1 = new TPrimitiva(EDIFICIO_ID, EDIFICIO_ID);
    TPrimitiva *edificio2 = new TPrimitiva(EDIFICIO_ID, EDIFICIO_ID);
    TPrimitiva *edificio3 = new TPrimitiva(EDIFICIO_ID, EDIFICIO_ID);
    TPrimitiva *edificio4 = new TPrimitiva(EDIFICIO_ID, EDIFICIO_ID);
    TPrimitiva *edificio5 = new TPrimitiva(EDIFICIO_ID, EDIFICIO_ID);
    TPrimitiva *edificio6 = new TPrimitiva(EDIFICIO_ID, EDIFICIO_ID);
    TPrimitiva *edificio7 = new TPrimitiva(EDIFICIO_ID, EDIFICIO_ID);
    TPrimitiva *edificio8 = new TPrimitiva(EDIFICIO_ID, EDIFICIO_ID);

    edificio1->tz = 1.29;
    edificio1->tx = -1.3;
    edificio2->tz = 1.29;
    edificio2->tx = -1.3-1.88;

    edificio3->tz = 1.29+2;
    edificio3->tx = -1.3;
    edificio4->tz = 1.29+2;
    edificio4->tx = -1.3-1.88;

    edificio5->tz = 1.29;
    edificio5->tx = -1.3+4.49;
    edificio6->tz = 1.29;
    edificio6->tx = -1.3-1.88+4.49;

    edificio7->tz = 1.29+2;
    edificio7->tx = -1.3+4.49;
    edificio8->tz = 1.29+2;
    edificio8->tx = -1.3-1.88+4.49;


    // Creamos las farolas
    TPrimitiva *farola1 = new TPrimitiva(FAROLA_ID, FAROLA_ID);
    TPrimitiva *farola2 = new TPrimitiva(FAROLA_ID, FAROLA_ID);
    TPrimitiva *farola3 = new TPrimitiva(FAROLA_ID, FAROLA_ID);
    TPrimitiva *farola4 = new TPrimitiva(FAROLA_ID, FAROLA_ID);
    TPrimitiva *farola5 = new TPrimitiva(FAROLA_ID, FAROLA_ID);
    TPrimitiva *farola6 = new TPrimitiva(FAROLA_ID, FAROLA_ID);
    TPrimitiva *farola7 = new TPrimitiva(FAROLA_ID, FAROLA_ID);
    TPrimitiva *farola8 = new TPrimitiva(FAROLA_ID, FAROLA_ID);
    TPrimitiva *farola9 = new TPrimitiva(FAROLA_ID, FAROLA_ID);
    TPrimitiva *farola10 = new TPrimitiva(FAROLA_ID, FAROLA_ID);
    TPrimitiva *farola11 = new TPrimitiva(FAROLA_ID, FAROLA_ID);
    TPrimitiva *farola12 = new TPrimitiva(FAROLA_ID, FAROLA_ID);
    TPrimitiva *farola13 = new TPrimitiva(FAROLA_ID, FAROLA_ID);
    TPrimitiva *farola14 = new TPrimitiva(FAROLA_ID, FAROLA_ID);

    farola1->tz = 0.765;
    farola1->tx = -0.75;

    farola2->tz = 0.765+3.07;
    farola2->tx = -0.75;

    farola3->tz = 0.765;
    farola3->tx = -0.75-2.97;

    farola4->tz = 0.765+3.07;
    farola4->tx = -0.75-2.97;

    farola5->tz = 0.765;
    farola5->tx = -0.75+4.5;

    farola6->tz = 0.765+3.07;
    farola6->tx = -0.75+4.5;

    farola7->tz = -2;
    farola7->tx = -1.3;
    farola7->ty = -0.03;

    farola8->tz = -2.6;
    farola8->tx = -1.3;
    farola8->ty = -0.03;

    farola9->tz = -2;
    farola9->tx = 1.3;
    farola9->ty = -0.03;

    farola10->tz = -2.6;
    farola10->tx = 1.3;
    farola10->ty = -0.03;

    farola11->tz = -1-0.3*7.5;
    farola11->tx = -1.73-0.34*6;
    farola11->ty = -0.03;
    farola12->tz = -1-0.3*7.5;
    farola12->tx = 1.6+0.34*6;
    farola12->ty = -0.03;

    farola13->tz = -1.15;
    farola13->tx = -1.73-0.34*6;
    farola13->ty = -0.03;
    farola14->tz = -1.2;
    farola14->tx = 1.6+0.34*6;
    farola14->ty = -0.03;



    // Creamos los semáforos
    TPrimitiva *semaforo1 = new TPrimitiva(SEMAFORO_ID, SEMAFORO_ID);
    TPrimitiva *semaforo2 = new TPrimitiva(SEMAFORO_ID, SEMAFORO_ID);

    semaforo1->tz = 0.765;
    semaforo1->tx = -0.75+1.48;

    semaforo2->tz = 0.765+3.07;
    semaforo2->tx = -0.75+1.48;



    // Creamos las casas
    TPrimitiva *casa1 = new TPrimitiva(CASA_ID, CASA_ID);
    TPrimitiva *casa2 = new TPrimitiva(CASA_ID, CASA_ID);
    TPrimitiva *casa3 = new TPrimitiva(CASA_ID, CASA_ID);
    TPrimitiva *casa4 = new TPrimitiva(CASA_ID, CASA_ID);

    casa1->tz = 2.5;
    casa1->tx = -1.8-0.5;
    casa1->ry = -90;

    casa2->tz = 2.5;
    casa2->tx = 1.8+0.5;
    casa2->ry = 90;



    // Creamos las rocas
    TPrimitiva *roca1 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca2 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca3 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca4 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca5 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca6 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca7 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca8 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca9 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca10 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca11 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca12 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca13 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca14 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca15 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca16 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca17 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca18 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca19 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca20 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca21 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca22 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca23 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca24 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca25 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca26 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca27 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca28 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca29 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca30 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca31 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca32 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca33 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca34 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca35 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca36 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca37 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca38 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca39 = new TPrimitiva(ROCAS_ID, ROCAS_ID);
    TPrimitiva *roca40 = new TPrimitiva(ROCAS_ID, ROCAS_ID);

    roca1->tz = 1.3;
    roca1->tx = -2.2;

    roca2->tz = 1.3;
    roca2->tx = -2.2+4.4;

    roca3->tz = 1.3+2;
    roca3->tx = -2.2;

    roca4->tz = 1.3+2;
    roca4->tx = -2.2+4.4;

    roca5->tz = -1.2;
    roca5->tx = -1.73;
    roca6->tz = -1.2;
    roca6->tx = 1.6;

    roca7->tz = -1.2;
    roca7->tx = -1.73-0.34;
    roca8->tz = -1.2;
    roca8->tx = 1.6+0.34;

    roca9->tz = -1.2;
    roca9->tx = -1.73-0.34*2;
    roca10->tz = -1.2;
    roca10->tx = 1.6+0.34*2;

    roca11->tz = -1.2;
    roca11->tx = -1.73-0.34*3;
    roca12->tz = -1.2;
    roca12->tx = 1.6+0.34*3;

    roca13->tz = -1.2;
    roca13->tx = -1.73-0.34*4;
    roca14->tz = -1.2;
    roca14->tx = 1.6+0.34*4;

    roca15->tz = -1.2;
    roca15->tx = -1.73-0.34*5;
    roca16->tz = -1.2;
    roca16->tx = 1.6+0.34*5;

    roca19->tz = -1.2-0.3;
    roca19->tx = -1.73-0.34*6;
    roca20->tz = -1.2-0.3;
    roca20->tx = 1.6+0.34*6;

    roca17->tz = -1.2-0.3*2;
    roca17->tx = -1.73-0.34*6;
    roca18->tz = -1.2-0.3*2;
    roca18->tx = 1.6+0.34*6;

    roca21->tz = -1.2-0.3*3;
    roca21->tx = -1.73-0.34*6;
    roca22->tz = -1.2-0.3*3;
    roca22->tx = 1.6+0.34*6;

    roca23->tz = -1.2-0.3*4;
    roca23->tx = -1.73-0.34*6;
    roca24->tz = -1.2-0.3*4;
    roca24->tx = 1.6+0.34*6;

    roca25->tz = -1.2-0.3*5;
    roca25->tx = -1.73-0.34*6;
    roca26->tz = -1.2-0.3*5;
    roca26->tx = 1.6+0.34*6;

    roca27->tz = -1.2-0.3*6;
    roca27->tx = -1.73-0.34*6;
    roca28->tz = -1.2-0.3*6;
    roca28->tx = 1.6+0.34*6;

    roca29->tz = -1.2-0.3*7.5;
    roca29->tx = -1.73-0.34*5;
    roca30->tz = -1.2-0.3*7.5;
    roca30->tx = 1.6+0.34*5;

    roca31->tz = -1.2-0.3*7.5;
    roca31->tx = -1.73-0.34*4;
    roca32->tz = -1.2-0.3*7.5;
    roca32->tx = 1.6+0.34*4;

    roca37->tz = -1.2-0.3*7.5;
    roca37->tx = -1.73-0.34*3;
    roca38->tz = -1.2-0.3*7.5;
    roca38->tx = 1.6+0.34*3;

    roca33->tz = -1.2-0.3*7.5;
    roca33->tx = -1.73-0.34*2;
    roca34->tz = -1.2-0.3*7.5;
    roca34->tx = 1.6+0.34*2;

    roca35->tz = -1.2-0.3*7.5;
    roca35->tx = -1.73-0.34;
    roca36->tz = -1.2-0.3*7.5;
    roca36->tx = 1.6+0.34;

    roca39->tz = -1.2-0.3*7.5;
    roca39->tx = -1.73;
    roca40->tz = -1.2-0.3*7.5;
    roca40->tx = 1.6;


    // Creamos el puente
    TPrimitiva *puente1 = new TPrimitiva(PUENTE_ID, PUENTE_ID);
    TPrimitiva *puente2 = new TPrimitiva(PUENTE_ID, PUENTE_ID);
    TPrimitiva *puente3 = new TPrimitiva(PUENTE_ID, PUENTE_ID);

    puente1->tx = 8.5;
    puente2->tx = -8.5;

    puente3->ry = 90;
    puente3->tx = 8.5;



    // Creamos el coche
    TPrimitiva *coche1 = new TPrimitiva(1, COCHE_ID);
    TPrimitiva *coche2 = new TPrimitiva(2, COCHE_ID);

    coche1->tx = -0.38;
    coche1->tz = 5;

    coche2->tx = 0.38;
    coche2->tz = 5;
    coche2->colores[0][0] = 0.3;
    coche2->colores[0][1] = 0.2;
    coche2->colores[0][2] = 0.6;
    coche2->colores[0][3] = 1.0;


    // Añadimos los objetos a la escena
    escena.AddObject(road1);
    escena.AddObject(acera1);
    escena.AddObject(acera2);
    escena.AddObject(acera3);
    escena.AddObject(acera4);
    escena.AddObject(acera5);
    escena.AddObject(acera6);
    escena.AddObject(acera7);
    escena.AddObject(acera8);
    escena.AddObject(edificio1);
    escena.AddObject(edificio2);
    escena.AddObject(edificio3);
    escena.AddObject(edificio4);
    escena.AddObject(edificio5);
    escena.AddObject(edificio6);
    escena.AddObject(edificio7);
    escena.AddObject(edificio8);
    escena.AddObject(farola1);
    escena.AddObject(farola2);
    escena.AddObject(farola3);
    escena.AddObject(farola4);
    escena.AddObject(farola5);
    escena.AddObject(farola6);
    escena.AddObject(farola7);
    escena.AddObject(farola8);
    escena.AddObject(farola9);
    escena.AddObject(farola10);
    escena.AddObject(farola11);
    escena.AddObject(farola12);
    escena.AddObject(farola13);
    escena.AddObject(farola14);
    escena.AddObject(semaforo1);
    escena.AddObject(semaforo2);
    escena.AddObject(casa1);
    escena.AddObject(casa2);
    escena.AddObject(roca1);
    escena.AddObject(roca2);
    escena.AddObject(roca3);
    escena.AddObject(roca4);
    escena.AddObject(roca5);
    escena.AddObject(roca6);
    escena.AddObject(roca7);
    escena.AddObject(roca8);
    escena.AddObject(roca9);
    escena.AddObject(roca10);
    escena.AddObject(roca11);
    escena.AddObject(roca12);
    escena.AddObject(roca13);
    escena.AddObject(roca14);
    escena.AddObject(roca15);
    escena.AddObject(roca16);
    escena.AddObject(roca17);
    escena.AddObject(roca18);
    escena.AddObject(roca19);
    escena.AddObject(roca20);
    escena.AddObject(roca21);
    escena.AddObject(roca22);
    escena.AddObject(roca23);
    escena.AddObject(roca24);
    escena.AddObject(roca25);
    escena.AddObject(roca26);
    escena.AddObject(roca27);
    escena.AddObject(roca28);
    escena.AddObject(roca29);
    escena.AddObject(roca30);
    escena.AddObject(roca31);
    escena.AddObject(roca32);
    escena.AddObject(roca33);
    escena.AddObject(roca34);
    escena.AddObject(roca35);
    escena.AddObject(roca36);
    escena.AddObject(roca37);
    escena.AddObject(roca38);
    escena.AddObject(roca39);
    escena.AddObject(roca40);
    escena.AddObject(puente1);
    escena.AddObject(puente2);
    escena.AddObject(puente3);
    escena.AddCar(coche1);
    escena.AddCar(coche2);



    /**** Regular GLUT main loop ****/
    glutMainLoop();// Creamos los edificios...

    return EXIT_SUCCESS;
}

