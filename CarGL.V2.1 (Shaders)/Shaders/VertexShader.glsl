
attribute vec4 a_Position;	        // in: Posición de cada vértice
attribute vec3 a_Normal;	        // in: Normal de cada vértice

uniform mat4 u_ProjectionMatrix; 	// in: Matriz Projection
uniform mat4 u_MVMatrix;	        // in: Matriz ModelView
uniform mat4 u_VMatrix;             // in: Matriz View (cámara)
uniform vec4 u_Color;		        // in: Color del objeto
uniform int  clickado;

uniform int  u_Luz0;                // in: Indica si la luz 0 esta encedida
uniform float u_Light0_x;
uniform float u_Light0_y;
uniform float u_Light0_z;
uniform float u_Light0_i;

uniform int   u_Luz1;
uniform float u_Light1_x;
uniform float u_Light1_y;
uniform float u_Light1_z;
uniform float u_Light1_i;

uniform int  u_Luz2;
uniform float u_Light2_x;
uniform float u_Light2_y;
uniform float u_Light2_z;
uniform float u_Light2_i;

//uniform int u_luz_especular;

varying vec4 v_Color;		        // out: Color al fragment shader

void main()
{
    if (clickado == 0) {

        vec4 LightPos0 = u_VMatrix*vec4( u_Light0_x, u_Light0_y, u_Light0_z, 1);		// Posicion de la luz0
        vec4 LightPos1 = u_VMatrix*vec4( u_Light1_x, u_Light1_y, u_Light1_z, 1);		// Posicion de la luz1
        vec4 LightPos2 = u_VMatrix*vec4( u_Light2_x, u_Light2_y, u_Light2_z, 1);		// Posicion de la luz2
        vec3 P = vec3(u_MVMatrix * a_Position);	            // Posicion del vertice
        vec3 N = vec3(u_MVMatrix * vec4(a_Normal, 0.0));    // Normal del vertice

        float d0 = length(LightPos0.xyz - P);			        // distancia de la luz
        float d1 = length(LightPos1.xyz - P);
        float d2 = length(LightPos2.xyz - P);
        vec3  L0 = normalize(LightPos0.xyz - P);			    // Vector Luz
        vec3  L1 = normalize(LightPos1.xyz - P);
        vec3  L2 = normalize(LightPos2.xyz - P);

        float ambient = 0.15;                               // (15% de int. ambiente)
        float diffuse = 0.0;

        float resultado = 0.0;



        if (u_Luz0>0) {                                     // Si la luz 0 esta encendida se calcula la intesidad difusa de L
            diffuse = max(dot(N, L0), 0.0);		            // Calculo de la int. difusa
            // C�lculo de la atenuacion
            float attenuation0 = 80.0/(0.25+(0.01*d0)+(0.003*d0*d0));

            resultado += diffuse*attenuation0*u_Light0_i;
        }
        if (u_Luz1>0) {                                     // Si la luz 1 esta encendida se calcula la intesidad difusa de L
            diffuse = max(dot(N, L1), 0.0);		            // C�lculo de la int. difusa
            // C�lculo de la atenuacion
            float attenuation1 = 80.0/(0.25+(0.01*d1)+(0.003*d1*d1));

            resultado += diffuse*attenuation1*u_Light1_i;
        }
        if (u_Luz2>0) {                                     // Si la luz 2 esta encendida se calcula la intesidad difusa de L
            diffuse = max(dot(N, L2), 0.0);		            // Calculo de la int. difusa
            // C�lculo de la atenuacion
            float attenuation2 = 80.0/(0.25+(0.01*d2)+(0.003*d2*d2));

            resultado += diffuse*attenuation2*u_Light2_i;
        }
        v_Color = u_Color * (resultado);
        gl_Position = u_ProjectionMatrix * vec4(P, 1.0);

    } else {

        vec3 P = vec3(u_MVMatrix * a_Position);	            // Posicion del vertice
        v_Color.rgba = u_Color.rrra;
        gl_Position = u_ProjectionMatrix * vec4(P, 1.0);
    }
}
