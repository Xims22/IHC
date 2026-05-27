#include "raylib.h"
#include "rlgl.h"
#include <cmath>

//--------------------------------------------------
// ROTACION 2D
//--------------------------------------------------
Vector2 Rotar(Vector2 p, float angulo)
{
    Vector2 r;

    r.x = p.x * cosf(angulo) - p.y * sinf(angulo);
    r.y = p.x * sinf(angulo) + p.y * cosf(angulo);

    return r;
}

//--------------------------------------------------
// MAIN
//--------------------------------------------------
int main()
{
    const int screenWidth = 1600;
    const int screenHeight = 850;

    InitWindow(screenWidth, screenHeight,
               "Transformaciones Matematicas");

    SetTargetFPS(60);

    //--------------------------------------------------
    // TIEMPO
    //--------------------------------------------------

    float tiempo = 0.0f;

    //--------------------------------------------------
    // TRASLACION
    //--------------------------------------------------

    float transX = 370;

    //--------------------------------------------------
    // ROTACION
    //--------------------------------------------------

    float theta = 0.0f;

    //--------------------------------------------------
    // REBOTE
    //--------------------------------------------------

    float reboteX = 850;
    int direccion = 1;

    //--------------------------------------------------
    // MOVIMIENTO SENOIDAL
    //--------------------------------------------------

    float senoTiempo = 0.0f;

    //--------------------------------------------------
    // TRAYECTORIA
    //--------------------------------------------------

    float trayectoriaT = 0.0f;

    //--------------------------------------------------
    // CAMARA 3D
    //--------------------------------------------------

    Camera3D camera = {0};

    camera.position = {8.0f, 8.0f, 8.0f};
    camera.target = {0.0f, 0.0f, 0.0f};
    camera.up = {0.0f, 1.0f, 0.0f};

    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    //--------------------------------------------------
    // LOOP
    //--------------------------------------------------

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        tiempo += dt;

        //--------------------------------------------------
        // TRASLACION
        //--------------------------------------------------

        transX += 120 * dt;

        if (transX > 500)
            transX = 370;

        //--------------------------------------------------
        // ROTACION
        //--------------------------------------------------

        theta += 1.8f * dt;

        //--------------------------------------------------
        // REBOTE
        //--------------------------------------------------

        reboteX += direccion * 220 * dt;

        if (reboteX > 980)
            direccion = -1;

        if (reboteX < 850)
            direccion = 1;

        //--------------------------------------------------
        // SENOIDAL
        //--------------------------------------------------

        senoTiempo += dt;

        //--------------------------------------------------
        // TRAYECTORIA
        //--------------------------------------------------

        trayectoriaT += dt;

        if (trayectoriaT > 3.5f)
            trayectoriaT = 0.0f;

        //--------------------------------------------------
        // DIBUJO
        //--------------------------------------------------

        BeginDrawing();

        ClearBackground(RAYWHITE);

        //--------------------------------------------------
        // TITULO
        //--------------------------------------------------

        DrawText("TRANSFORMACIONES MATEMATICAS",
                 420, 30, 40, BLACK);

        //--------------------------------------------------
        // LEYENDA IZQUIERDA
        //--------------------------------------------------

        DrawCircle(60, 120, 14, GOLD);
        DrawText("TRASLACION", 90, 110, 28, GOLD);

        DrawCircle(60, 180, 14, RED);
        DrawText("ROTACION", 90, 170, 28, RED);

        DrawCircle(60, 240, 14, GREEN);
        DrawText("REBOTE", 90, 230, 28, GREEN);

        DrawCircle(60, 300, 14, SKYBLUE);
        DrawText("MOVIMIENTO SENOIDAL", 90, 290, 28, SKYBLUE);

        DrawCircle(60, 360, 14, PURPLE);
        DrawText("TRAYECTORIA", 90, 350, 28, PURPLE);

        DrawCircle(60, 420, 14, MAGENTA);
        DrawText("ORBITA 3D", 90, 410, 28, MAGENTA);

        //--------------------------------------------------
        // PLANO PRINCIPAL
        //--------------------------------------------------

        int panelY = 260;
        int panelW = 190;
        int panelH = 220;

        // Paneles
        for (int i = 0; i < 6; i++)
        {
            DrawRectangleLines(
                350 + (i * panelW),
                panelY,
                panelW,
                panelH,
                BLACK
            );
        }

        //--------------------------------------------------
        // 1. TRASLACION
        //--------------------------------------------------

        DrawText("1", 430, 275, 24, GOLD);

        DrawRectangle(
            transX,
            340,
            50,
            50,
            GOLD
        );

        //--------------------------------------------------
        // 2. ROTACION
        //--------------------------------------------------

        DrawText("2", 620, 275, 24, RED);

        Vector2 centro = {635, 370};

        float lado = 70;

        Vector2 v1 = {-lado/2, -lado/2};
        Vector2 v2 = { lado/2, -lado/2};
        Vector2 v3 = { lado/2,  lado/2};
        Vector2 v4 = {-lado/2,  lado/2};

        v1 = Rotar(v1, theta);
        v2 = Rotar(v2, theta);
        v3 = Rotar(v3, theta);
        v4 = Rotar(v4, theta);

        v1.x += centro.x; v1.y += centro.y;
        v2.x += centro.x; v2.y += centro.y;
        v3.x += centro.x; v3.y += centro.y;
        v4.x += centro.x; v4.y += centro.y;

        DrawLineV(v1, v2, RED);
        DrawLineV(v2, v3, RED);
        DrawLineV(v3, v4, RED);
        DrawLineV(v4, v1, RED);

        //--------------------------------------------------
        // 3. REBOTE
        //--------------------------------------------------

        DrawText("3", 810, 275, 24, GREEN);

        DrawCircle(reboteX, 370, 28, GREEN);

        //--------------------------------------------------
        // 4. MOVIMIENTO SENOIDAL
        //--------------------------------------------------

        DrawText("4", 1000, 275, 24, SKYBLUE);

        // Caja del panel
        int senoPanelX = 920;
        int senoPanelY = 260;

        // Movimiento horizontal
        float senoX = senoPanelX + 20 + fmod(tiempo * 90, 150);

        // Onda senoidal
        float senoY = 370 + 50 * sinf((senoX - senoPanelX) * 0.08f);

        // Dibujar curva guía
        for (int i = 0; i < 140; i++)
        {
            float x1 = senoPanelX + 20 + i;
            float y1 = 370 + 50 * sinf((x1 - senoPanelX) * 0.08f);

            float x2 = senoPanelX + 20 + i + 1;
            float y2 = 370 + 50 * sinf((x2 - senoPanelX) * 0.08f);

            DrawLine(x1, y1, x2, y2, LIGHTGRAY);
        }

        // Partícula
        DrawCircle(senoX, senoY, 18, SKYBLUE);

        //--------------------------------------------------
        // 5. TRAYECTORIA
        //--------------------------------------------------

        DrawText("5", 1190, 275, 24, PURPLE);

        int trayX = 1110;
        int trayY = 260;

        // Trayectoria parabólica
        float px = trayX + 15 + trayectoriaT * 45;

        float py = 430 - (120 * trayectoriaT - 18 * trayectoriaT * trayectoriaT);

        // Curva guía
        for (float t = 0; t < 3.3f; t += 0.05f)
        {
            float x1 = trayX + 15 + t * 45;
            float y1 = 430 - (120 * t - 18 * t * t);

            float t2 = t + 0.05f;

            float x2 = trayX + 15 + t2 * 45;
            float y2 = 430 - (120 * t2 - 18 * t2 * t2);

            DrawLine(x1, y1, x2, y2, LIGHTGRAY);
        }

        // Partícula
        DrawCircle(px, py, 16, PURPLE);

        //--------------------------------------------------
        // 6. ORBITA 3D
        //--------------------------------------------------

        DrawText("6", 1380, 275, 24, MAGENTA);

        float centroOrbX = 1395;
        float centroOrbY = 370;

        // Elipse orbital
        DrawEllipseLines(
            centroOrbX,
            centroOrbY,
            55,
            22,
            LIGHTGRAY
        );

        // Simulación 3D
        float orbX = centroOrbX + 55 * cosf(2 * tiempo);
        float orbY = centroOrbY + 22 * sinf(2 * tiempo);

        // Planeta central
        DrawCircle(centroOrbX, centroOrbY, 14, DARKGRAY);

        // Órbita
        DrawCircle(orbX, orbY, 10, MAGENTA);

        // Línea orbital
        DrawLine(
            centroOrbX,
            centroOrbY,
            orbX,
            orbY,
            Fade(MAGENTA, 0.3f)
        );

        //--------------------------------------------------
        // FPS
        //--------------------------------------------------

        DrawText(
            TextFormat("FPS: %d", GetFPS()),
            20,
            780,
            22,
            BLACK
        );

        EndDrawing();
    }

    CloseWindow();

    return 0;
}