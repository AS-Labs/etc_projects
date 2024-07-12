#include "raylib.h"



int main(void) {
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "raylib [core] example - 2d camera");

	RenderTexture2D player = LoadRenderTexture(16, 16);


	BeginTextureMode(player);
	ClearBackground(BLANK);
	for(int y=player.texture.height;y>0;y--) {
		BeginTextureMode(player);
		DrawLine(0+y, player.texture.height-y,player.texture.width-y/2,player.texture.height-y,WHITE);
		EndTextureMode();
	}

	SetTargetFPS(60);


	int shipTurnSpeed = 3;
	int shipAngle = 0;

	while (!WindowShouldClose()) {
		for (int i = 0; i < shipTurnSpeed; i++) {
			if (IsKeyDown(KEY_LEFT)) {
				shipAngle--;
			}
			if (IsKeyDown(KEY_RIGHT)) {
				shipAngle++;
			}
			if (shipAngle < 0) {
				shipAngle = 359;
			}
			if (shipAngle > 359) {
				shipAngle = 0;
			}
		}

		BeginDrawing();
		ClearBackground(BLACK);
		DrawText("Use Cursor Left and Right to Rotate ship, escape to exit", 2,2,20,DARKGRAY);

		DrawTexturePro(player.texture, (Rectangle) {
			0,0,player.texture.width,-player.texture.height
		},
		(Rectangle) {
			320,200,player.texture.width,player.texture.height
		},
		(Vector2) {
		 player.texture.width/2,player.texture.height/2
		}, shipAngle,WHITE);
		EndDrawing();

		
	}

	UnloadRenderTexture(player);
	CloseWindow();
	return 0;
}
