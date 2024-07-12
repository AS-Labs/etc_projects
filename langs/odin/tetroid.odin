package main


import rl "vendor:raylib"

SQUARE_SIZE :: 20
GRID_HORIZONTAL_SIZE :: 12
GRID_VERTICAL_SIZE :: 20

LATERAL_SPEED :: 10
TURNING_SPEED :: 12
FAST_FALL_AWAIT_COUNTER :: 30

FADING_TIME :: 33

SCREEN_WIDTH :: 800
SCREEN_HEIGHT :: 450


Grid_Square :: enum u8 {
	Empty,
	Moving,
	Full,
	Block,
	Fading,
}


game_over := false
pause := false

grid: [GRID_HORIZONTAL_SIZE][GRID_VERTICAL_SIZE]Grid_Square
piece: [4][4]Grid_Square
incoming_piece: [4][4]Grid_Square


peice_position: [2]i32


fading_color: rl.Color


begin_play := true
piece_active := false
detection := false
line_to_delete := false


level := 1
lines := 0


gravity_movement_counter := 0
lateral_movement_counter := 0
trun_movement_counter := 0
fast_fall_movement_counter := 0


fade_line_counter := 0

inverse_gravity_speed := 30




main :: proc() {
	rl.InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tetris")
	defer rl.CloseWindow()


	init_game()


	rl.SetTargetFPS(60)

	for !rl.WindowShouldClose() {
		if !game_over && !pause {
			update_game()
		}

		draw_game()
	}
}
