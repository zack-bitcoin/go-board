#include "raylib.h"
#include "stdio.h"
void write_to(char * x)
{
  FILE *file = fopen("coordinates", "a");
  if (file == NULL) {
    perror("Error opening file");
    return;
  }
  fprintf(file, x);
  fclose(file);
}
int main(void)
{
  char s[64];
    const int screenWidth = 741;
    const int screenHeight = 741;
    SetTraceLogLevel(7);
    InitWindow(screenWidth, screenHeight, "go board");
    Vector2 ballPosition = { -100.0f, -100.0f };
    Texture2D texture = LoadTexture("board.png");
    SetTargetFPS(10);
    while (!WindowShouldClose()){    // Detect window close button or ESC key
      if (IsKeyPressed(KEY_S)){write_to("score\n");}
      if (IsKeyPressed(KEY_U)){write_to("undo\n");}
      ballPosition = GetMousePosition();
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
          sprintf(s, "%f, %f", ballPosition.x, ballPosition.y);
          write_to(s);
        }
      BeginDrawing();
      UnloadTexture(texture);
      texture = LoadTexture("board.png");
      DrawTexture(texture, screenWidth/2 - texture.width/2, screenHeight/2 - texture.height/2, WHITE);
      EndDrawing();
    }
    //if the gui gets the kill signal, we need to propagate that to the rest of the program.
    write_to("quit\n");
    CloseWindow();        // Close window and OpenGL context
    return 0;
}
