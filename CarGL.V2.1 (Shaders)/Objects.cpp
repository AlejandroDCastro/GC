
/****************************************************************************************

    CarGL (Objects.cpp)

    Grado en Ingeniería Multimedia.
    Práctica de Gráficos por Computador.
    OpenGL con Shaders.
  ---------------------------------------------------------------------------------------

    Noviembre 2012 - Septiembre 2016 (C) Juan Antonio Puchol García (puchol@dccia.ua.es)
    Septiembre 2012 - Enero 2020 (C) Alejandro Castro Valero (acv52@ arroba alu.ua.es)


*****************************************************************************************/

#include "Objects.h"
#include <GL/glui.h>
#include <iostream>

#include "load3ds.c"

// Variable para inicializar los vectores correpondientes con los valores iniciales
GLfloat light0_ambient_c[4]  = {   0.2f,   0.2f,  0.2f, 1.0f };
GLfloat light0_diffuse_c[4]  = {   0.8f,   0.8f,  0.8f, 1.0f };
GLfloat light0_specular_c[4] = {   1.0f,   1.0f,  1.0f, 1.0f };
GLfloat light0_position_c[4] = {-10.0f, -10.0f, -30.0f, 1.0f };

GLfloat light1_ambient_c[4]  = {   0.2f,   0.2f,  0.2f, 1.0f };
GLfloat light1_diffuse_c[4]  = {   0.8f,   0.8f,  0.8f, 1.0f };
GLfloat light1_specular_c[4] = {   1.0f,   1.0f,  1.0f, 1.0f };
GLfloat light1_position_c[4] = {   0.0f, 100.0f,  0.0f, 1.0f };

GLfloat light2_ambient_c[4]  = {   0.2f,   0.2f,  0.2f, 1.0f };
GLfloat light2_diffuse_c[4]  = {   0.8f,   0.8f,  0.8f, 1.0f };
GLfloat light2_specular_c[4] = {   1.0f,   1.0f,  1.0f, 1.0f };
GLfloat light2_position_c[4] = {   80.0f, 100.0f,  -20.0f, 1.0f };

GLfloat mat_ambient_c[4]    = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat mat_diffuse_c[4]    = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat mat_specular_c[4]   = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat mat_shininess_c[1] = { 100.0f };

// Matriz de 4x4 = (I)
float view_rotate_c[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float view_position_c[3] = { 0.0, -2.0, -9.0 };

float coloresc_c[2][4] = { {1, 0.5, 0.0, 1.0}, {0.5, 0.5, 0.5, 1.0}}; // Color del coche
float coloresr_c[2][4] = { {0.4, 0.3, 0.3, 1.0}, {1.0, 1.0, 1.0, 1.0}}; // Color de la carretera
float coloresAcera_c[2][4] = { {0.93, 0.28, 0.1, 1.0}, {1.0, 1.0, 1.0, 1.0}}; // Color de las aceras
float coloresEdificio_c[2][4] = { {0.48, 0.86, 0.4, 1.0}, {1.0, 1.0, 1.0, 1.0}}; // Color de los edificios
float coloresFarola_c[2][4] = { {0.84, 0.8, 0.54, 1.0}, {1.0, 1.0, 1.0, 1.0}}; // Color de las farolas
float coloresSemaforo_c[2][4] = { {0.78, 0.78, 0.78, 1.0}, {1.0, 1.0, 1.0, 1.0}}; // Color de los semaforos
float coloresRocas_c[2][4] = { {0.44, 0.4, 0.14, 1.0}, {1.0, 1.0, 1.0, 1.0}}; // Color de los rocas
float coloresPuente_c[2][4] = { {0.13, 0.51, 0.32, 1.0}, {1.0, 1.0, 1.0, 1.0}}; // Color de las puentes
float coloresCasa_c[2][4] = { {0.83, 0.83, 0.83, 1.0}, {1.0, 1.0, 1.0, 1.0}}; // Color de las casa

//************************************************************** Variables de clase

TEscena escena;
TGui    gui;

//************************************************************** Clase TPrimitiva

TPrimitiva::TPrimitiva(int DL, int t)
{

    ID   = DL;
    tipo = t;
    anguloRuedas = 0;
    sx = sy = sz = 1;
    rx = ry = rz = 0;
	switch (tipo) {
		case CARRETERA_ID: {  // Creación de la carretera
            tx = ty = tz = 0;

            memcpy(colores, coloresr_c, 8*sizeof(float));

            //************************ Cargar modelos 3ds ***********************************
            // formato 8 floats por vértice (x, y, z, A, B, C, u, v)
            modelo0 = Load3DS("../../Modelos/carretera.3ds", &num_vertices0);

            break;
		}
		case COCHE_ID: { // Creación del coche

		    ty =  0.047;
		    rr =  0.0;

		    memcpy(colores, coloresc_c, 8*sizeof(float));

            //************************ Cargar modelos 3ds ***********************************
            // formato 8 floats por vértice (x, y, z, A, B, C, u, v)
            modelo0 = Load3DS("../../Modelos/coche.3ds", &num_vertices0);
            modelo1 = Load3DS("../../Modelos/rueda.3ds", &num_vertices1);
            break;
		}
		case ACERAMANZANA_ID: {
		    tx = tz = 0;
		    ty = -0.145;    // Altura adecuada para los objetos desde el inicio
		    memcpy(colores, coloresAcera_c, 8*sizeof(float));
            modelo0 = Load3DS("../../Modelos/aceramanzana.3ds", &num_vertices0);
            break;
		}
		case EDIFICIO_ID: {
		    tx = tz = tz = 0;    // Altura adecuada para los objetos desde el inicio
		    memcpy(colores, coloresEdificio_c, 8*sizeof(float));
            modelo0 = Load3DS("../../Modelos/edificio.3ds", &num_vertices0);
            break;
		}
		case FAROLA_ID: {
		    tx = tz = 0;
		    ty = 0.04;    // Altura adecuada para los objetos desde el inicio
		    memcpy(colores, coloresFarola_c, 8*sizeof(float));
            modelo0 = Load3DS("../../Modelos/farola.3ds", &num_vertices0);
            break;
		}
		case SEMAFORO_ID: {
		    tx = tz = 0;
		    ty = 0.04;    // Altura adecuada para los objetos desde el inicio
		    memcpy(colores, coloresSemaforo_c, 8*sizeof(float));
            modelo0 = Load3DS("../../Modelos/semaforo.3ds", &num_vertices0);
            break;
		}
		case ROCAS_ID: {
		    tx = tz = 0;
		    ty = -0.05;    // Altura adecuada para los objetos desde el inicio
		    memcpy(colores, coloresRocas_c, 8*sizeof(float));
            modelo0 = Load3DS("../../Modelos/rocas.3ds", &num_vertices0);
            break;
		}
		case PUENTE_ID: {
		    tx = tz = 0;
		    ty = -0.17+0.065;    // Altura adecuada para los objetos desde el inicio
		    memcpy(colores, coloresPuente_c, 8*sizeof(float));
            modelo0 = Load3DS("../../Modelos/puente.3ds", &num_vertices0);
            break;
		}
		case CASA_ID: {
		    tx = tz = 0;
		    ty = 0;  // Altura adecuada para los objetos desde el inicio
		    memcpy(colores, coloresCasa_c, 8*sizeof(float));
            modelo0 = Load3DS("../../Modelos/casa.3ds", &num_vertices0);
            break;
		}
	} // switch
}

void __fastcall TPrimitiva::Render(int seleccion, bool reflejo)
{
    glm::mat4   modelMatrix;
    glm::mat4   modelViewMatrix;
    switch (tipo) {

        case CARRETERA_ID: {
            if (escena.show_road) {
                // Calculo de la ModelView
                modelMatrix     = glm::mat4(1.0f); // matriz identidad

                modelViewMatrix = escena.viewMatrix * modelMatrix;
                // Env�a nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);



                // Pintar la carretera
                glUniform4fv(escena.uColorLocation, 1, colores[0]);
                //Asociamos los vertices y sus normales
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);

                glDrawArrays(GL_TRIANGLES, 0, num_vertices0);


            }
            break;
        }
        case COCHE_ID: {
            if (escena.show_car) {
                glUniform4fv(escena.uColorLocation, 1, (const GLfloat *) colores[0]);
                // Asociamos los vértices y sus normales
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);

                // Cálculo de la matriz modelo
                modelMatrix     = glm::mat4(1.0f); // matriz identidad
                modelMatrix     = glm::translate(modelMatrix,glm::vec3(tx, ty, tz));
                //Escalado
                modelMatrix     = glm::scale(modelMatrix,glm::vec3(sx,sy,sz));
                //rotacion
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(ry), glm::vec3(0,1,0));      // en radianes

                modelViewMatrix = escena.viewMatrix * modelMatrix;

                // Envía nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);

                glDrawArrays(GL_TRIANGLES, 0, num_vertices0);
            }


            if (escena.show_wheels)
            {
                glUniform4fv(escena.uColorLocation, 1, (const GLfloat *) colores[1]);
                // Asociamos los vertices y sus normales
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo1);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo1+3);

                // RUEDA Delantera Derecha: Calculo de la matriz modelo
                modelMatrix     = glm::mat4(1.0f); // matriz identidad

                modelMatrix     = glm::translate(modelMatrix, glm::vec3(tx+0.08, ty-0.017, tz-0.133));
                modelMatrix     = glm::translate(modelMatrix, glm::vec3(-0.08, 0, +0.133));
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(ry), glm::vec3(0,1,0));   // en radianes
                modelMatrix     = glm::translate(modelMatrix, glm::vec3(+0.08, 0, -0.133));
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(anguloRuedas), glm::vec3(0,1,0));   // en radianes
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(rr), glm::vec3(1,0,0));      // en radianes


                modelViewMatrix = escena.viewMatrix * modelMatrix;

                // Envia nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);

                glDrawArrays(GL_TRIANGLES, 0, num_vertices1);

                // RUEDA Trasera Derecha : Calculo de la matriz modelo
                modelMatrix     = glm::mat4(1.0f); // matriz identidad

                modelMatrix     = glm::translate(modelMatrix, glm::vec3(tx+0.08, ty-0.017, tz+0.115));
                modelMatrix     = glm::translate(modelMatrix, glm::vec3(-0.08, 0, -0.115));
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(ry), glm::vec3(0,1,0));   // en radianes
                modelMatrix     = glm::translate(modelMatrix, glm::vec3(+0.08, 0, +0.115));
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(rr), glm::vec3(1,0,0));


                modelViewMatrix = escena.viewMatrix * modelMatrix;

                // Envia nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);

                glDrawArrays(GL_TRIANGLES, 0, num_vertices1);

                // RUEDA Delantera Izquierda : Calculo de la matriz modelo
                modelMatrix     = glm::mat4(1.0f); // matriz identidad

                modelMatrix     = glm::translate(modelMatrix, glm::vec3(tx-0.087, ty-0.02, tz-0.131));
                modelMatrix     = glm::translate(modelMatrix, glm::vec3(+0.087, 0, +0.131));
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(ry), glm::vec3(0,1,0));   // en radianes
                modelMatrix     = glm::translate(modelMatrix, glm::vec3(-0.087, 0, -0.131));
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(anguloRuedas), glm::vec3(0,1,0));   // en radianes
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(rr), glm::vec3(1,0,0));
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(180.0), glm::vec3(0,0,1));   // en radianes

                modelViewMatrix = escena.viewMatrix * modelMatrix;

                // Envia nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);

                glDrawArrays(GL_TRIANGLES, 0, num_vertices1);

                // RUEDA Trasera Izquierda : Calculo de la matriz modelo
                modelMatrix     = glm::mat4(1.0f); // matriz identidad
                modelMatrix     = glm::translate(modelMatrix, glm::vec3(tx-0.087, ty-0.02, tz+0.117));
                modelMatrix     = glm::translate(modelMatrix, glm::vec3(+0.087, 0, -0.117));
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(ry), glm::vec3(0,1,0));   // en radianes
                modelMatrix     = glm::translate(modelMatrix, glm::vec3(-0.087, 0, +0.117));
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(rr), glm::vec3(1,0,0));
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(180.0), glm::vec3(0,0,1));

                modelViewMatrix = escena.viewMatrix * modelMatrix;

                // Envia nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);

                glDrawArrays(GL_TRIANGLES, 0, num_vertices1);
                break;
            }

        }
        case ACERAMANZANA_ID: {
            if (escena.show_aceramanzana) {
                glUniform4fv(escena.uColorLocation, 1, (const GLfloat *) colores[0]);

                // Asociamos los vértices y sus normales
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);

                // Cálculo de la matriz modelo
                modelMatrix     = glm::mat4(1.0f); // matriz identidad
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(ry), glm::vec3(0,1,0));
                modelMatrix     = glm::translate(modelMatrix,glm::vec3(tx, ty, tz));

                modelViewMatrix = escena.viewMatrix * modelMatrix;

                // Envía nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);

                glDrawArrays(GL_TRIANGLES, 0, num_vertices0);
            }
            break;
        }
        case EDIFICIO_ID: {
            if (escena.show_aceramanzana) {
                glUniform4fv(escena.uColorLocation, 1, (const GLfloat *) colores[0]);

                // Asociamos los vértices y sus normales
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);

                // Cálculo de la matriz modelo
                modelMatrix     = glm::mat4(1.0f); // matriz identidad
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(ry), glm::vec3(0,1,0));
                modelMatrix     = glm::translate(modelMatrix,glm::vec3(tx, ty, tz));

                modelViewMatrix = escena.viewMatrix * modelMatrix;

                // Envía nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);

                glDrawArrays(GL_TRIANGLES, 0, num_vertices0);
            }
            break;
        }
        case FAROLA_ID: {
            if (escena.show_aceramanzana) {
                glUniform4fv(escena.uColorLocation, 1, (const GLfloat *) colores[0]);

                // Asociamos los vértices y sus normales
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);

                // Cálculo de la matriz modelo
                modelMatrix     = glm::mat4(1.0f); // matriz identidad
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(ry), glm::vec3(0,1,0));
                modelMatrix     = glm::translate(modelMatrix,glm::vec3(tx, ty, tz));

                modelViewMatrix = escena.viewMatrix * modelMatrix;

                // Envía nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);

                glDrawArrays(GL_TRIANGLES, 0, num_vertices0);
            }
            break;
        }
        case SEMAFORO_ID: {
            if (escena.show_aceramanzana) {
                glUniform4fv(escena.uColorLocation, 1, (const GLfloat *) colores[0]);

                // Asociamos los vértices y sus normales
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);

                // Cálculo de la matriz modelo
                modelMatrix     = glm::mat4(1.0f); // matriz identidad
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(ry), glm::vec3(0,1,0));
                modelMatrix     = glm::translate(modelMatrix,glm::vec3(tx, ty, tz));

                modelViewMatrix = escena.viewMatrix * modelMatrix;

                // Envía nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);

                glDrawArrays(GL_TRIANGLES, 0, num_vertices0);
            }
            break;
        }
        case ROCAS_ID: {
            if (escena.show_aceramanzana) {
                glUniform4fv(escena.uColorLocation, 1, (const GLfloat *) colores[0]);

                // Asociamos los vértices y sus normales
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);

                // Cálculo de la matriz modelo
                modelMatrix     = glm::mat4(1.0f); // matriz identidad
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(ry), glm::vec3(0,1,0));
                modelMatrix     = glm::translate(modelMatrix,glm::vec3(tx, ty, tz));

                modelViewMatrix = escena.viewMatrix * modelMatrix;

                // Envía nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);

                glDrawArrays(GL_TRIANGLES, 0, num_vertices0);
            }
            break;
        }
        case PUENTE_ID: {
            if (escena.show_aceramanzana) {
                glUniform4fv(escena.uColorLocation, 1, (const GLfloat *) colores[0]);

                // Asociamos los vértices y sus normales
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);

                // Cálculo de la matriz modelo
                modelMatrix     = glm::mat4(1.0f); // matriz identidad
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(ry), glm::vec3(0,1,0));
                modelMatrix     = glm::translate(modelMatrix,glm::vec3(tx, ty, tz));

                modelViewMatrix = escena.viewMatrix * modelMatrix;

                // Envía nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);

                glDrawArrays(GL_TRIANGLES, 0, num_vertices0);
            }
            break;
        }
        case CASA_ID: {
            if (escena.show_aceramanzana) {
                glUniform4fv(escena.uColorLocation, 1, (const GLfloat *) colores[0]);

                // Asociamos los vértices y sus normales
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);

                // Cálculo de la matriz modelo
                modelMatrix     = glm::mat4(1.0f); // matriz identidad
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(ry), glm::vec3(0,1,0));
                modelMatrix     = glm::translate(modelMatrix,glm::vec3(tx, ty, tz));

                modelViewMatrix = escena.viewMatrix * modelMatrix;

                // Envía nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);

                glDrawArrays(GL_TRIANGLES, 0, num_vertices0);
            }
            break;
        }
    } // switch

}

//************************************************************** Clase TEscena

TEscena::TEscena() {

    seleccion = 1;
    ultimaSelecccion = seleccion;
    num_objects = 0;
    num_cars = 0;

    show_car = 1;
    show_wheels = 1;
    show_road = 1;
    show_aceramanzana = 1;
    show_farola = 1;
    show_semaforo = 1;
    show_rocas = 1;
    show_puente = 1;
    show_edificio = 1;
    show_casa = 1;

    camara_modo = 0;
    camara = 0;
    tipoVista = 0;
    luzEspecular = 1;

    // live variables usadas por GLUI en TGui
    wireframe = 0;
    z_buffer = 1;
    culling = 0;

    scale = 1.0;
    xy_aspect = 1;
    last_x = 0;
    last_y = 0;

    memcpy(view_position, view_position_c, 3*sizeof(float));
    memcpy(view_rotate, view_rotate_c, 16*sizeof(float));

    memcpy(light0_ambient, light0_ambient_c, 4*sizeof(float));

    memcpy(light0_ambient, light0_ambient_c, 4*sizeof(float));
    memcpy(light0_diffuse, light0_diffuse_c, 4*sizeof(float));
    memcpy(light0_specular, light0_specular_c, 4*sizeof(float));
    memcpy(light0_position, light0_position_c, 4*sizeof(float));

    memcpy(light1_ambient, light1_ambient_c, 4*sizeof(float));
    memcpy(light1_diffuse, light1_diffuse_c, 4*sizeof(float));
    memcpy(light1_specular, light1_specular_c, 4*sizeof(float));
    memcpy(light1_position, light1_position_c, 4*sizeof(float));

    memcpy(light2_ambient, light2_ambient_c, 4*sizeof(float));
    memcpy(light2_diffuse, light2_diffuse_c, 4*sizeof(float));
    memcpy(light2_specular, light2_specular_c, 4*sizeof(float));
    memcpy(light2_position, light2_position_c, 4*sizeof(float));

    memcpy(mat_ambient, mat_ambient_c, 4*sizeof(float));
    memcpy(mat_diffuse, mat_diffuse_c, 4*sizeof(float));
    memcpy(mat_specular, mat_specular_c, 4*sizeof(float));
    memcpy(mat_shininess, mat_shininess_c, 1*sizeof(float));
}

void __fastcall TEscena::InitGL()
{
    int tx, ty, tw, th;

    // Habilita el z_buffer
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SHADE_MODEL);
    glEnable(GL_FRONT_FACE);
    glDepthFunc(GL_LESS);
/*
    // Activamos el Stencil Buffer
    glEnable(GL_STENCIL_TEST); // sólo una vez
*/

    // Inicialización de GLEW
    std::cout << "Inicializando GLEW" << std::endl << std::endl;
    if(glewInit() != GLEW_OK)
        throw std::runtime_error("glewInit failed");

    // print out some info about the graphics drivers
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendedor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    // Carga de los Shaders
    std::cout << std::endl << "Cargando Shaders" << std::endl;

    Shader shader;

    std::vector<GLuint> shaders;
    shaders.push_back(shader.LoadShader("../../Shaders/VertexShader.glsl", GL_VERTEX_SHADER));
    //std::cout << "Vertex Shader: " << shader.ReturnShaderID() << std::endl;
    shaders.push_back(shader.LoadShader("../../Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER));
    //std::cout << "Fragment Shader: " << shader.ReturnShaderID() << std::endl;
    shaderProgram = new Program(shaders);

    //std::cout << "Shader Program: " << shaderProgram->ReturnProgramID() << std::endl;

    glUseProgram(shaderProgram->ReturnProgramID());
    //glValidateProgram(shaderProgram->ReturnProgramID());

    aPositionLocation=shaderProgram->attrib(A_POSITION);
    aNormalLocation=shaderProgram->attrib(A_NORMAL);

    uProjectionMatrixLocation=shaderProgram->uniform(U_PROJECTIONMATRIX);
    uMVMatrixLocation=shaderProgram->uniform(U_MVMATRIX);
    uVMatrixLocation=shaderProgram->uniform(U_VMATRIX);
    uColorLocation=shaderProgram->uniform(U_COLOR);

    //habilitamos el paso del clickado
    seleccionarLocation=shaderProgram->uniform(CLICKADO);

    //Habilitamos Luz0
    uLuz0Location=shaderProgram->uniform(U_LUZ0);
    uLight0_xLocation=shaderProgram->uniform(U_LIGHT0_X);
    uLight0_yLocation=shaderProgram->uniform(U_LIGHT0_Y);
    uLight0_zLocation=shaderProgram->uniform(U_LIGHT0_Z);
    uLight0_iLocation=shaderProgram->uniform(U_LIGHT0_I);


    //Habilitamos cosas de Luz1
    uLuz1Location=shaderProgram->uniform(U_LUZ1);
    uLight1_xLocation=shaderProgram->uniform(U_LIGHT1_X);
    uLight1_yLocation=shaderProgram->uniform(U_LIGHT1_Y);
    uLight1_zLocation=shaderProgram->uniform(U_LIGHT1_Z);
    uLight1_iLocation=shaderProgram->uniform(U_LIGHT1_I);

    //Habilitamos cosas de Luz1
    uLuz2Location=shaderProgram->uniform(U_LUZ2);
    uLight2_xLocation=shaderProgram->uniform(U_LIGHT2_X);
    uLight2_yLocation=shaderProgram->uniform(U_LIGHT2_Y);
    uLight2_zLocation=shaderProgram->uniform(U_LIGHT2_Z);
    uLight2_iLocation=shaderProgram->uniform(U_LIGHT2_I);

    //uLuzEspecularLocation=shaderProgram->uniform(U_LUZ_ESPECULAR);


    /*
    std::cout << "a_Position Location: " << aPositionLocation << std::endl;
    std::cout << "a_Normal Location: " << aNormalLocation << std::endl;

    std::cout << "u_ProjectionMatrix Location: " << uProjectionMatrixLocation << std::endl;
    std::cout << "u_MVMatrix Location: " << uMVMatrixLocation << std::endl;
    std::cout << "u_VMatrix Location: " << uVMatrixLocation << std::endl;
    std::cout << "u_Color Location: " << uColorLocation << std::endl;
    std::cout << "u_Luz0 Location: " << uLuz0Location << std::endl;
    */

    // Habilitamos el paso de attributes
    glEnableVertexAttribArray(aPositionLocation);
    glEnableVertexAttribArray(aNormalLocation);

    // Estableciendo la matriz de proyección perspectiva
    GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
    xy_aspect = (float)tw / (float)th;
    projectionMatrix = glm::perspective(45.0f, xy_aspect, 0.1f, 1000.0f);
    glUniformMatrix4fv(uProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}


/************************** TEscena::AddCar(TPrimitiva *car) *****************/

void __fastcall TEscena::AddCar(TPrimitiva *car)
{
    cars[num_cars] = car;
    num_cars++;
}

/******************** TEscena::AddObject(TPrimitiva *object) *****************/

void __fastcall TEscena::AddObject(TPrimitiva *object)
{
    objects[num_objects] = object;
    num_objects++;
}

/******************** TPrimitiva *TEscena::GetCar(int id) ********************/

TPrimitiva __fastcall *TEscena::GetCar(int id)
{
    TPrimitiva *p=NULL;

    for (int i=0; i<num_cars; i++)
    {
        if (cars[i]->ID==id)
        {
            p = cars[i];
        }

    }
    return(p);
}

/******************** TEscena::RenderCars() **********************************/

void __fastcall TEscena::RenderCars(bool reflejo) {

    for (int i=0; i<num_cars; i++)
    {
        cars[i]->Render(seleccion, reflejo);
    }
}

/******************** TEscena::RenderCars() **********************************/

void __fastcall TEscena::RenderObjects(bool reflejo) {

    for (int i=0; i<num_objects; i++)
    {
        objects[i]->Render(seleccion, reflejo);
    }
}

/***************************************** TEscena::Render() *****************/

void __fastcall TEscena::Render()
{
    glm::mat4 rotateMatrix;

    glClearColor(0.0, 0.7, 0.9, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    // CAMARAS
    switch(escena.camara_modo){


        //CAMARA GENERAL
        case 0: {
            viewMatrix = glm::mat4(1.0f);
            rotateMatrix = glm::make_mat4(view_rotate);
            viewMatrix = glm::translate(viewMatrix,glm::vec3(view_position[0],view_position[1],view_position[2]));
            viewMatrix = viewMatrix*rotateMatrix;
            viewMatrix = glm::scale(viewMatrix,glm::vec3(scale,scale,scale));
            break;
        }


        /*
        ///////////// glm::lookAt FUNCTION ////////////
            detail::tmat4x4<T> glm::gtc::matrix_transform::lookAt
            (
                detail::tvec3< T > const &  eye,
                detail::tvec3< T > const &  center,
                detail::tvec3< T > const &  up
            )
        ///////////////////////////////////////////////
        */


        //CAMARA SEGUIMIENTO
        case 1: {
            TPrimitiva *coche = GetCar(seleccion);
            if(coche){
                float angulo = (coche->ry*PI)/180.0;
                float cX = coche->tx+0.7*sin(angulo);
                float cZ = coche->tz+0.7*cos(angulo);

                viewMatrix = glm::lookAt(glm::vec3(cX,0.4,cZ), glm::vec3(coche->tx,coche->ty,coche->tz),glm::vec3(0,1,0));
            }
            break;
        }


        //CAMARA AEREA
        case 2: {
            TPrimitiva *coche = GetCar(seleccion);
            if(coche){

                viewMatrix = glm::lookAt(glm::vec3(coche->tx,20,coche->tz),glm::vec3(coche->tx,0,coche->tz),glm::vec3(0,0,1));

            }
            break;
        }
    }


    //PICK 3D
    glUniform1i(seleccionarLocation,0);

    glUniform1i(uLuz0Location, gui.light0_enabled);
    glUniform1i(uLuz1Location, gui.light1_enabled);
    glUniform1i(uLuz2Location, gui.light2_enabled);
    glUniformMatrix4fv(uVMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix)); // Para la luz matrix view pero sin escalado!

    // Dibujar carretera y objetos
    RenderObjects(seleccion);

    // Dibujar coches
    RenderCars(seleccion);

    //Usamos dos paginas de video. Dibujamos usando dos buffer
    /** Muestra los renders **/
    glutSwapBuffers();
}

// Selecciona un objeto a través del ratón
void __fastcall TEscena::Pick3D(int mouse_x, int mouse_y)
{
    unsigned char res[4];
    GLint viewport[4];
    int tx,ty,tw,th;
 //   int index;

    renderSeleccion();


    GLUI_Master.get_viewport_area(&tx,&ty,&tw,&th);
    glGetIntegerv(GL_VIEWPORT,viewport);
    glReadPixels(mouse_x,th-mouse_y+80,1,1,GL_RGBA,GL_UNSIGNED_BYTE,&res);

 //   glReadPixels(mouse_x, th-mouse_y-81, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);
 //   seleccion = index;

    switch(res[0]){

        //Coche 1
        case 255:
            std::cout << "Seleccionado coche 1 " << res[0] << std::endl;
            seleccion=1;
            gui.radio->set_int_val(1);
            break;

        //Coche 2
        case 76:
            std::cout << "Seleccionado coche 2 " << res[0] << std::endl;
            seleccion=2;
            gui.radio->set_int_val(2);
            break;

        //Resto de objetos
        default:
            std::cout << "Resto: (" << mouse_x << "," << mouse_y << ") " << res[0] << std::endl;
            seleccion=0;
            gui.radio->set_int_val(0);
            break;
    }
    ultimaSelecccion = seleccion;

}

void __fastcall TEscena::renderSeleccion(){

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

/*
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
*/

    //set matrices to identity
    //glm::mat4 rotateMatrix;

    // set camera as in the regular rendering function


    //viewMatrix = glm::mat4(1.0f);
    //rotateMatrix = glm::make_mat4(view_rotate);
    //viewMatrix = glm::translate(viewMatrix,glm::vec3(view_position[0],view_position[1],view_position[2]));
    //viewMatrix = viewMatrix*rotateMatrix;

    if(scale < 0.01){
        scale=0.01;
    }
    //viewMatrix = glm::scale(viewMatrix,glm::vec3(scale/100.0,scale/100.0,scale/100.0));

    glUniform1i(seleccionarLocation,1);


    // restore clear color if needed
    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    RenderObjects();
    RenderCars(seleccion);
}

//************************************************************** Clase TGui

TGui::TGui()
{
    sel = 1;
    enable_panel2 = 1;
    light0_enabled = 1;
    light1_enabled = 1;
    light2_enabled = 1;
    light0_intensity = 0.8;
    light1_intensity = 0.8;
    light2_intensity = 0.8;
    memcpy(light0_position, light0_position_c, 4*sizeof(float));
    memcpy(light1_position, light1_position_c, 4*sizeof(float));
    memcpy(light2_position, light2_position_c, 4*sizeof(float));
}

void controlCallback(int control)
{
    gui.ControlCallback(control);
}

void __fastcall TGui::Init(int main_window) {

    /****************************************************/
    /*        Código con el interfaz Gráfico GLUI       */
    /****************************************************/
    printf( "GLUI version: %3.2f\n", GLUI_Master.get_version() );

    window_id = main_window;

    /*** Crea a ventana lateral ***/
    glui = GLUI_Master.create_glui_subwindow( window_id, GLUI_SUBWINDOW_RIGHT );

    // Añade una separación
    new GLUI_StaticText( glui, "" );

    // Añade un panel con texto con el valor de la selección
    GLUI_Panel *panel0 = new GLUI_Panel(glui, "Seleccion");
    radio = new GLUI_RadioGroup(panel0, &sel, SEL_ID, controlCallback);
    glui->add_radiobutton_to_group(radio, "NINGUNO");


    glui->add_radiobutton_to_group(radio, "COCHE 1");
    glui->add_radiobutton_to_group(radio, "COCHE 2");


    // Añade una separación
    new GLUI_StaticText( glui, "" );

    obj_panel = new GLUI_Rollout(glui, "Propiedades", true );

    /***** Control para las propiedades de escena *****/

    new GLUI_Checkbox( obj_panel, "Modo Alambrico", &escena.wireframe, 1, controlCallback );
    glui->add_column_to_panel(obj_panel, true);
    new GLUI_Checkbox( obj_panel, "Culling", &escena.culling, 1, controlCallback );
    new GLUI_Checkbox( obj_panel, "Z Buffer", &escena.z_buffer, 1, controlCallback );



    /******** Añade controles para las camaras ********/

    // Añade una separación
    new GLUI_StaticText( glui, "" );

    GLUI_Panel *panel1 = new GLUI_Panel(glui,"Camaras");
    GLUI_RadioGroup *radioGroup1 = new GLUI_RadioGroup(panel1,&escena.camara,CAM_ID,controlCallback);
    glui->add_radiobutton_to_group(radioGroup1,"General");
    glui->add_radiobutton_to_group(radioGroup1,"Seguimiento");
    glui->add_radiobutton_to_group(radioGroup1,"Aerea");




    /******** Añade controles para las luces ********/

    // Añade una separación
    new GLUI_StaticText( glui, "" );

    GLUI_Rollout *roll_lights = new GLUI_Rollout(glui, "Luces", false );

    GLUI_Panel *light0 = new GLUI_Panel( roll_lights, "Luz 1" );
    GLUI_Panel *light1 = new GLUI_Panel( roll_lights, "Luz 2" );
    GLUI_Panel *light2 = new GLUI_Panel( roll_lights, "Luz 3" );

    new GLUI_Checkbox( light0, "Encendida", &light0_enabled, LIGHT0_ENABLED_ID, controlCallback );
    light0_spinner = new GLUI_Spinner( light0, "Intensidad:", &light0_intensity,
                            LIGHT0_INTENSITY_ID, controlCallback );
    light0_spinner->set_float_limits( 0.0, 1.0 );
    GLUI_Scrollbar *sb;
    sb = new GLUI_Scrollbar( light0, "X",GLUI_SCROLL_HORIZONTAL,
                            &escena.light0_position[0],LIGHT0_POSITION_ID,controlCallback);
    sb->set_float_limits(-100,100);
    sb = new GLUI_Scrollbar( light0, "Y",GLUI_SCROLL_HORIZONTAL,
                            &escena.light0_position[1],LIGHT0_POSITION_ID,controlCallback);
    sb->set_float_limits(-100,100);
    sb = new GLUI_Scrollbar( light0, "Z",GLUI_SCROLL_HORIZONTAL,
                            &escena.light0_position[2],LIGHT0_POSITION_ID,controlCallback);
    sb->set_float_limits(-100,100);

    new GLUI_Checkbox( light1, "Encendida", &light1_enabled, LIGHT1_ENABLED_ID, controlCallback );
    light1_spinner = new GLUI_Spinner( light1, "Intensidad:", &light1_intensity,
                            LIGHT1_INTENSITY_ID, controlCallback );
    light1_spinner->set_float_limits( 0.0, 1.0 );
    sb = new GLUI_Scrollbar( light1, "X",GLUI_SCROLL_HORIZONTAL,
                            &escena.light1_position[0],LIGHT1_POSITION_ID,controlCallback);
    sb->set_float_limits(-100,100);
    sb = new GLUI_Scrollbar( light1, "Y",GLUI_SCROLL_HORIZONTAL,
                            &escena.light1_position[1],LIGHT1_POSITION_ID,controlCallback);
    sb->set_float_limits(-100,100);
    sb = new GLUI_Scrollbar( light1, "Z",GLUI_SCROLL_HORIZONTAL,
                            &escena.light1_position[2],LIGHT1_POSITION_ID,controlCallback);
    sb->set_float_limits(-100,100);


    new GLUI_Checkbox( light2, "Encendida", &light2_enabled, LIGHT2_ENABLED_ID, controlCallback );
    light2_spinner = new GLUI_Spinner( light2, "Intensidad:", &light2_intensity,
                            LIGHT2_INTENSITY_ID, controlCallback );
    light2_spinner->set_float_limits( 0.0, 1.0 );
    sb = new GLUI_Scrollbar( light2, "X",GLUI_SCROLL_HORIZONTAL,
                            &escena.light2_position[0],LIGHT2_POSITION_ID,controlCallback);
    sb->set_float_limits(-100,100);
    sb = new GLUI_Scrollbar( light2, "Y",GLUI_SCROLL_HORIZONTAL,
                            &escena.light2_position[1],LIGHT2_POSITION_ID,controlCallback);
    sb->set_float_limits(-100,100);
    sb = new GLUI_Scrollbar( light2, "Z",GLUI_SCROLL_HORIZONTAL,
                            &escena.light2_position[2],LIGHT2_POSITION_ID,controlCallback);
    sb->set_float_limits(-100,100);


    // Añade una separación
    new GLUI_StaticText( glui, "" );

    /***  Rollout de Opciones ***/
    GLUI_Rollout *options = new GLUI_Rollout(glui, "Opciones", true );
    new GLUI_Checkbox( options, "Dibujar Coche", &escena.show_car );
    new GLUI_Checkbox( options, "Dibujar Ruedas", &escena.show_wheels );
    new GLUI_Checkbox( options, "Dibujar Carretera", &escena.show_road );
    new GLUI_Checkbox( options, "Dibujar Acera", &escena.show_aceramanzana );
    new GLUI_Checkbox( options, "Dibujar Edificio", &escena.show_edificio );
    new GLUI_Checkbox( options, "Dibujar Casa", &escena.show_casa );
    new GLUI_Checkbox( options, "Dibujar Farola", &escena.show_farola );
    new GLUI_Checkbox( options, "Dibujar Semaforo", &escena.show_semaforo );
    new GLUI_Checkbox( options, "Dibujar Puente", &escena.show_puente );
    new GLUI_Checkbox( options, "Dibujar Rocas", &escena.show_rocas );


    /*** Disable/Enable botones ***/
    // Añade una separación
    new GLUI_StaticText( glui, "" );
    new GLUI_Checkbox( glui, "Bloquear Movimiento", &enable_panel2 );
    // Añade una separación
    new GLUI_StaticText( glui, "" );
    new GLUI_Button( glui, "Resetear Posicion", RESET_ID, controlCallback );

    // Añade una separación
    new GLUI_StaticText( glui, "" );

    new GLUI_Separator( glui );

     // Añade una separación
    new GLUI_StaticText( glui, "" );

    new GLUI_StaticText( glui, "  Autor:" );
    new GLUI_StaticText( glui, "  2019-2020 (C) Alejandro Castro Valero  " );

    // Añade una separación
    new GLUI_StaticText( glui, "" );

    new GLUI_Separator( glui );

    // Añade una separación
    new GLUI_StaticText( glui, "" );

    /****** A 'quit' button *****/
    new GLUI_Button( glui, "Salir", 0,(GLUI_Update_CB)exit );

    // Crea la subventana inferior
    glui2 = GLUI_Master.create_glui_subwindow( window_id, GLUI_SUBWINDOW_BOTTOM );

    /**** Link windows to GLUI, and register idle callback ******/
    glui->set_main_gfx_window( window_id );
    glui2->set_main_gfx_window( window_id );

    view_rot = new GLUI_Rotation(glui2, "Rotacion Escena", escena.view_rotate );
    view_rot->set_spin( 1.0 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_xy = new GLUI_Translation(glui2, "Traslacion Escena XY", GLUI_TRANSLATION_XY, escena.view_position );
    trans_xy->set_speed( .005 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_x =  new GLUI_Translation(glui2, "Traslacion Escena X", GLUI_TRANSLATION_X, escena.view_position );
    trans_x->set_speed( .005 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_y = new GLUI_Translation( glui2, "Traslacion Escena Y", GLUI_TRANSLATION_Y, &escena.view_position[1] );
    trans_y->set_speed( .005 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_z = new GLUI_Translation( glui2, "Traslacion Escena Z", GLUI_TRANSLATION_Z, &escena.scale );
    trans_z->set_speed( .005 );

}

/**************************************** TGui::ControlCallback() *******************/

void __fastcall TGui::ControlCallback( int control )
{
    switch (control) {
        case VISUALIZACION_ID:{
            if(escena.visualizacion==0){
                //SOLIDO
                printf("Modo solido...\n");
                glPolygonMode(GL_FRONT, GL_FILL);
                glPolygonMode(GL_BACK, GL_FILL);
            }else if(escena.visualizacion==1){
                //ALAMBRICO
                printf("Modo alambrico...\n");
                glPolygonMode(GL_FRONT, GL_LINE);//DELANTERO
                glPolygonMode(GL_BACK, GL_LINE);//TRASERO

            }else if(escena.visualizacion==2){
                //PUNTOS
                printf("Modo puntos...\n");
                glPolygonMode(GL_FRONT, GL_POINT);//DELANTERO
                glPolygonMode(GL_BACK, GL_POINT);//TRASERO
            }
            break;
        }
        case CULLING_ID: {
            if(escena.culling){
                printf("Culling activado...\n");
                glEnable(GL_CULL_FACE);
                glCullFace(GL_FRONT);
            }
            else{
                printf("Culling desactivado...\n");
                glDisable(GL_CULL_FACE);
            }

            break;
        }case ZBUFFER_ID: {
            if(escena.z_buffer){
                printf("Zbuffer activado...\n");
                glEnable(GL_DEPTH_TEST);

            }else{
                printf("Zbuffer desactivado...\n");
                glDisable(GL_DEPTH_TEST);
            }

            break;
        }
        case SOMBREADO_ID: {
            glEnable(GL_SHADE_MODEL);
            if(escena.sombreado){
                printf("Sombreado activado...\n");
                glShadeModel(GL_SMOOTH);//SUAVIZADO
            }else{
                printf("Sombreado desactivado...\n");
                glShadeModel(GL_FLAT);//PLANO;
            }

            break;
        }
        case SENTIDO_ID: {
            glEnable(GL_FRONT_FACE);
            if(escena.sentido){
                printf("Sentido antihorario...\n");
                glFrontFace(GL_CCW);//ANTIHORARIO
            }else{
                printf("Sentido horario...\n");
                glFrontFace(GL_CW);//HORARIO
            }
            break;
        }
        case LUZ_ESPECULAR_ID: {
            if(escena.luzEspecular){
                printf("Luz especular activada...\n");
                glUniform1i(escena.uLuzEspecularLocation,1);
            }else{
                printf("Luz especular desactivada...\n");
                glUniform1i(escena.uLuzEspecularLocation,0);
            }
            break;
        }
        case LIGHT0_ENABLED_ID: {
            if ( light0_enabled )
                light0_spinner->enable();
            else
                light0_spinner->disable();
            break;
        }
        case LIGHT1_ENABLED_ID: {
            if ( light1_enabled )
                light1_spinner->enable();
            else
                light1_spinner->disable();
            break;
        }
        case LIGHT2_ENABLED_ID: {
            if ( light2_enabled )
                light2_spinner->enable();
            else
                light2_spinner->disable();
            break;
        }
        case LIGHT0_INTENSITY_ID: {

            float v[] = {
                escena.light0_diffuse[0],  escena.light0_diffuse[1],
                escena.light0_diffuse[2],  escena.light0_diffuse[3] };

            v[0] *= light0_intensity;
            v[1] *= light0_intensity;
            v[2] *= light0_intensity;
            glUniform1f(escena.uLight0_iLocation,light0_intensity);
            break;
        }
        case LIGHT0_POSITION_ID: {
            //En escena.light0_position[] tenemos el x y z de la luz
            glUniform1f(escena.uLight0_xLocation,escena.light0_position[0]);
            glUniform1f(escena.uLight0_yLocation,escena.light0_position[1]);
            glUniform1f(escena.uLight0_zLocation,escena.light0_position[2]);

        }
        case LIGHT1_INTENSITY_ID: {

            float v[] = {
                escena.light1_diffuse[0],  escena.light1_diffuse[1],
                escena.light1_diffuse[2],  escena.light1_diffuse[3] };

            v[0] *= light1_intensity;
            v[1] *= light1_intensity;
            v[2] *= light1_intensity;
            glUniform1f(escena.uLight1_iLocation,light1_intensity);
            break;
        }
        case LIGHT1_POSITION_ID: {
            //En escena.light0_position[] tenemos el x y z de la luz
            glUniform1f(escena.uLight1_xLocation,escena.light1_position[0]);
            glUniform1f(escena.uLight1_yLocation,escena.light1_position[1]);
            glUniform1f(escena.uLight1_zLocation,escena.light1_position[2]);

        }
        case LIGHT2_INTENSITY_ID: {

            float v[] = {
                escena.light2_diffuse[0],  escena.light2_diffuse[1],
                escena.light2_diffuse[2],  escena.light2_diffuse[3] };

            v[0] *= light2_intensity;
            v[1] *= light2_intensity;
            v[2] *= light2_intensity;
            glUniform1f(escena.uLight2_iLocation,light2_intensity);
            break;
        }
        case LIGHT2_POSITION_ID: {
            //En escena.light0_position[] tenemos el x y z de la luz
            glUniform1f(escena.uLight2_xLocation,escena.light2_position[0]);
            glUniform1f(escena.uLight2_yLocation,escena.light2_position[1]);
            glUniform1f(escena.uLight2_zLocation,escena.light2_position[2]);

        }
        case ENABLE_ID: {
            glui2->enable();
            break;
        }
        case DISABLE_ID: {
            glui2->disable();
            break;
        }
        case RESET_ID: {
            memcpy(escena.view_position,view_position_c,3*sizeof(float));
            view_rot->reset();
            escena.scale = 1.0;
            break;
        }
        case SEL_ID: {
            escena.seleccion = sel;
            //GLUI_Master.SetFocus(true);
            glutSetWindow( glui->get_glut_window_id() );
            break;
        }
        case CAM_ID: {
            escena.camara_modo = escena.camara;
            printf("Camara seleccionada numero: %d \n",escena.camara_modo);
            break;
        }
  } // switch
}

/***************************************** TGui::Idle() ***********/

void __fastcall TGui::Idle( void )
{
  /* According to the GLUT specification, the current window is
     undefined during an idle callback.  So we need to explicitly change
     it if necessary */
  if ( glutGetWindow() != window_id )
    glutSetWindow(window_id);

  /*  GLUI_Master.sync_live_all();  -- not needed - nothing to sync in this
                                       application  */
    if (enable_panel2)

        glui2->enable();
    else
        glui2->disable();

  glutPostRedisplay();
}

/**************************************** TGui::reshape() *************/

void __fastcall TGui::Reshape( int x, int y )
{
    int tx, ty, tw, th;
    GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
    glViewport( tx, ty, tw, th );

    escena.xy_aspect = (float)tw / (float)th;
    glMatrixMode(GL_PROJECTION);

    if(escena.tipoVista){
          //glOrtho(left, right, bottom, top, near, far)
         escena.projectionMatrix = glm::ortho(escena.xy_aspect,10*escena.xy_aspect, -10.0f, 10.0f, -100.0f, 1000.0f);
         printf("Seleccionada vista ortogonal\n\n");
    }else{
        //gluPerspective(alpha, aspect, near, far)
         escena.projectionMatrix = glm::perspective(45.0f, escena.xy_aspect, 0.1f, 1000.0f);
         printf("Seleccionada vista perspectiva\n\n");
    }

    glUniformMatrix4fv(escena.uProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(escena.projectionMatrix));
    //std::cout << "xy aspect: " << escena.xy_aspect << std::endl;

    glutPostRedisplay();
}

/***************************************** gui::motion() **********/

void __fastcall TGui::Motion(int x, int y )
{
    glutPostRedisplay();
}

/***************************************** gui::Mouse() **********/

void __fastcall TGui::Mouse(int button, int button_state, int x, int y )
{
    escena.Pick3D(x, y);
}

