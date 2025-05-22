#include "raylib.h"
#include "stdio.h"
int main(void)
{
    const int screenWidth = 741;
    const int screenHeight = 741;
    SetTraceLogLevel(7);
    InitWindow(screenWidth, screenHeight, "go board");
    Vector2 ballPosition = { -100.0f, -100.0f };
    Texture2D texture = LoadTexture("board.png");
    SetTargetFPS(10);
    while (!WindowShouldClose()){    // Detect window close button or ESC key
      if (IsKeyPressed(KEY_U))
        {
          FILE *file = fopen("coordinates", "a");
          if (file == NULL) {
            perror("Error opening file");
            return 1;
          }
          fprintf(file, "undo\n");
          fclose(file);
        }
      ballPosition = GetMousePosition();
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
          FILE *file = fopen("coordinates", "a");
          if (file == NULL) {
            perror("Error opening file");
            return 1;
          }
          fprintf(file,"%f, %f\n", ballPosition.x, ballPosition.y);
          fclose(file);
        }
      BeginDrawing();
      UnloadTexture(texture);
      texture = LoadTexture("board.png");
      DrawTexture(texture, screenWidth/2 - texture.width/2, screenHeight/2 - texture.height/2, WHITE);
      EndDrawing();
    }
    //if the gui gets the kill signal, we need to propagate that to the rest of the program.
    FILE *file = fopen("coordinates", "a");
    if (file == NULL) {
      perror("Error opening file");
      return 1;
    }
    fprintf(file, "quit\n");
    fclose(file);
    CloseWindow();        // Close window and OpenGL context
    return 0;
}
