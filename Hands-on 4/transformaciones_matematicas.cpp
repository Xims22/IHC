#include "raylib.h"
#include <cmath>

// Función matemática de rotación
Vector2 RotarPunto(Vector2 p, float angulo)
{
    Vector2 r;
    r.x = p.x * cosf(angulo) - p.y * sinf(angulo);
    r.y = p.x * sinf(angulo) + p.y * cosf(angulo);
    return r;
}

int main()
{
    const int screenWidth = 1200;
    const int screenHeight = 650;
    InitWindow(screenWidth, screenHeight, "Aplicacion Unificada: Transformaciones y Movimiento");
    SetTargetFPS(60);

    // estados y variables de los componentes
    float tiempoAcumulado = 0.0f;

    // 1. Variables para Traslación (Basado en translation_kyb_box.cpp)
    Vector2 posTraslacion = { 150, 200 };
    float velocidadTeclado = 250.0f;

    // 2. Variables para Rotación (Basado en rotate_box.cpp)
    Vector2 centroRotacion = { 450, 200 };
    float ladoCubo = 80.0f;
    float thetaRotacion = 0.0f;

    // 3. Variables para Rebote (Basado en fps_vel_rebound.cpp)
    // El rebote ocurrirá dentro de un contenedor asignado en el eje X
    float limiteIzquierdoRebote = 750.0f;
    float limiteDerechoRebote = 1150.0f;
    Vector2 posRebote = { 950, 200 };
    float rapidezRebote = 200.0f;
    int direccionRebote = 1;
    float radioRebote = 20.0f;

    // 4. Variables para Movimiento Senoidal (Basado en movimiento_osc.cpp)
    Vector2 centroOscilacion = { 300, 500 };
    float amplitudSenoidal = 60.0f;
    float velocidadAngularSen = 3.0f;

    // 5. Variables para Órbita / Trayectoria (Basado en movimiento_cir.cpp)
    Vector2 centroOrbita = { 850, 500 };
    float radioOrbita = 60.0f;
    float velocidadAngularOrb = 2.0f;

    // ==========================================
    // BUCLE PRINCIPAL
    // ==========================================
    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        tiempoAcumulado += dt;

        // ------------------------------------------
        // ACTUALIZACIÓN DE LÓGICAS (UPDATE)
        // ------------------------------------------

        // Lógica 1: Traslación Interactiva por Teclado
        if (IsKeyDown(KEY_RIGHT)) posTraslacion.x += velocidadTeclado * dt;
        if (IsKeyDown(KEY_LEFT))  posTraslacion.x -= velocidadTeclado * dt;
        if (IsKeyDown(KEY_UP))    posTraslacion.y -= velocidadTeclado * dt;
        if (IsKeyDown(KEY_DOWN))  posTraslacion.y += velocidadTeclado * dt;

        // Lógica 2: Rotación Automática por Matriz
        thetaRotacion += 1.5f * dt;

        // Lógica 3: Rebote Físico con cambio de signo en dirección
        posRebote.x += (direccionRebote * rapidezRebote) * dt;
        if (posRebote.x >= limiteDerechoRebote - radioRebote)
        {
            posRebote.x = limiteDerechoRebote - radioRebote;
            direccionRebote = -1;
        }
        if (posRebote.x <= limiteIzquierdoRebote + radioRebote)
        {
            posRebote.x = limiteIzquierdoRebote + radioRebote;
            direccionRebote = 1;
        }

        // Lógica 4: Movimiento Senoidal (Oscilación Vertical)
        float oscilacionY = amplitudSenoidal * sinf(velocidadAngularSen * tiempoAcumulado);

        // Lógica 5: Trayectoria Órbita Circular (X y Y combinados)
        float orbitaX = centroOrbita.x + radioOrbita * cosf(velocidadAngularOrb * tiempoAcumulado);
        float orbitaY = centroOrbita.y + radioOrbita * sinf(velocidadAngularOrb * tiempoAcumulado);


        // ------------------------------------------
        // RENDERIZADO / DIBUJO (DRAW)
        // ------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Título Principal de la Entrega
        DrawText("TRANSFORMACIONES MATEMATICAS Y ANIMACION", 280, 20, 28, DARKBLUE);
        DrawLine(0, 60, screenWidth, 60, LIGHTGRAY);

        // Líneas Divisorias del Plano Contiguo
        DrawLine(400, 60, 400, screenHeight, LIGHTGRAY);
        DrawLine(720, 60, 720, screenHeight, LIGHTGRAY);
        DrawLine(0, 350, screenWidth, 350, LIGHTGRAY);


        // ------------------------------------------
        // ESTACIÓN 1: TRASLACIÓN (Arriba Izquierda)
        // ------------------------------------------
        DrawText("1. TRASLACION (Teclado)", 20, 80, 20, DARKGRAY);
        DrawText("Usa las FLECHAS para mover", 20, 110, 14, GRAY);
        // Dibujamos el objeto trasladado
        DrawRectangle(posTraslacion.x - 30, posTraslacion.y - 30, 60, 60, BLUE);
        DrawCircleV(posTraslacion, 4, WHITE); // Centro


        // ------------------------------------------
        // ESTACIÓN 2: ROTACIÓN POR MATRIZ (Arriba Centro)
        // ------------------------------------------
        DrawText("2. ROTACION AUTOMATICA", 420, 80, 20, DARKGRAY);
        DrawText("Matriz Cos/Sin sobre origen", 420, 110, 14, GRAY);

        // Definimos vértices locales centrados en (0,0)
        Vector2 v1 = {-ladoCubo/2, -ladoCubo/2};
        Vector2 v2 = { ladoCubo/2, -ladoCubo/2};
        Vector2 v3 = { ladoCubo/2,  ladoCubo/2};
        Vector2 v4 = {-ladoCubo/2,  ladoCubo/2};

        // Multiplicamos por la matriz de rotación
        v1 = RotarPunto(v1, thetaRotacion);
        v2 = RotarPunto(v2, thetaRotacion);
        v3 = RotarPunto(v3, thetaRotacion);
        v4 = RotarPunto(v4, thetaRotacion);

        // Trasladamos al centro de su estación correspondiente
        v1.x += centroRotacion.x; v1.y += centroRotacion.y;
        v2.x += centroRotacion.x; v2.y += centroRotacion.y;
        v3.x += centroRotacion.x; v3.y += centroRotacion.y;
        v4.x += centroRotacion.x; v4.y += centroRotacion.y;

        // Dibujamos las líneas del contorno del cuadrado rotado
        DrawLineEx(v1, v2, 3, RED);
        DrawLineEx(v2, v3, 3, RED);
        DrawLineEx(v3, v4, 3, RED);
        DrawLineEx(v4, v1, 3, RED);
        DrawCircleV(centroRotacion, 4, BLACK);


        // ------------------------------------------
        // ESTACIÓN 3: REBOTE (Arriba Derecha)
        // ------------------------------------------
        DrawText("3. REBOTE EN LIMITES", 740, 80, 20, DARKGRAY);
        DrawText("Invierte velocidad en fronteras", 740, 110, 14, GRAY);
        
        // Líneas de las fronteras de rebote
        DrawLine(limiteIzquierdoRebote, 130, limiteIzquierdoRebote, 270, MAROON);
        DrawLine(limiteDerechoRebote, 130, limiteDerechoRebote, 270, MAROON);
        
        // Objeto en movimiento lineal con rebote
        DrawCircleV(posRebote, radioRebote, ORANGE);


        // ------------------------------------------
        // ESTACIÓN 4: MOVIMIENTO SENOIDAL (Abajo Izquierda)
        // ------------------------------------------
        DrawText("4. MOVIMIENTO SENOIDAL", 20, 370, 20, DARKGRAY);
        DrawText("y = y0 + A * sin(w * t)", 20, 400, 14, GRAY);
        
        // Línea de trayectoria esperada
        DrawLine(centroOscilacion.x, centroOscilacion.y - amplitudSenoidal, centroOscilacion.x, centroOscilacion.y + amplitudSenoidal, LIGHTGRAY);
        
        // Círculo oscilante
        DrawCircle(centroOscilacion.x, centroOscilacion.y + oscilacionY, 18, LIME);


        // ------------------------------------------
        // ESTACIÓN 5: TRAYECTORIA / ÓRBITA (Abajo Derecha)
        // ------------------------------------------
        DrawText("5. TRAYECTORIA / ORBITA", 740, 370, 20, DARKGRAY);
        DrawText("Usa funciones trigonometricas puras", 740, 400, 14, GRAY);
        
        // Guía del camino circular
        DrawCircleLines(centroOrbita.x, centroOrbita.y, radioOrbita, LIGHTGRAY);
        DrawCircle(centroOrbita.x, centroOrbita.y, 3, BLACK);
        
        // Línea de radio vector y el cuerpo orbitando
        DrawLine(centroOrbita.x, centroOrbita.y, orbitaX, orbitaY, GRAY);
        DrawCircle(orbitaX, orbitaY, 12, PURPLE);


        // Información técnica en la base de la pantalla
        DrawText(TextFormat("FPS: %d", GetFPS()), 440, 610, 16, DARKGRAY);
        DrawText(TextFormat("Tiempo de Frame (dt): %.4f s", dt), 570, 610, 16, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}