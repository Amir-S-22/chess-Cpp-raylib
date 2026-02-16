#include <iostream>
#include "raylib.h"
#include "enums.h"
#include "functions.h"
#include "classes.h"

int main(){
	
	//window initialization
	const int window_width 	{1200};
	const int window_height {800};
	const int target_fps    {60};
	const char *window_name = "Chess_v3";
	const Color background_colour = {212, 202, 163, 255};

	InitWindow(window_width, window_height, window_name);
	SetTargetFPS(target_fps);


	//game engine object (for future debug purposes)
	Game game;

	//game state starts with WHITE_TURN;
	WINNER winner = NO_WINNER;

	//ui object handles drawing and game logic thorugh inheritance
	UI ui;
	ui.give_board_coordinates();

	//Vector2's to hold start and end mouse positions
	Vector2 mouse_xy_1 = {};
	Vector2 mouse_xy_2 = {};

	while(!WindowShouldClose()){
		//main game loop

		if(winner == NO_WINNER){
			bool needs_to_move = false;

			if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
				//piece selection happens with left mouse button click
				
				mouse_xy_1 = ui.mouse_to_xy(GetMousePosition());
			}else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
				mouse_xy_2 = ui.mouse_to_xy(GetMousePosition());
				needs_to_move = true;
			}

			//king kill detection
			if((needs_to_move) && ui.can_piece_move(mouse_xy_1.x, mouse_xy_1.y, mouse_xy_2.x, mouse_xy_2.y)){
				if(ui.board[xy_to_i(mouse_xy_2.x,mouse_xy_2.y)][0] == KING && 
						ui.board[xy_to_i(mouse_xy_2.x,mouse_xy_2.y)][1] == WHITE_PIECE) winner = BLACK_WINNER;
				else if(ui.board[xy_to_i(mouse_xy_2.x,mouse_xy_2.y)][0] == KING && 
						ui.board[xy_to_i(mouse_xy_2.x,mouse_xy_2.y)][1] == BLACK_PIECE) winner = WHITE_WINNER;
			}
			
			//Which piece can move
			if(needs_to_move && ui.can_piece_move(mouse_xy_1.x, mouse_xy_1.y, mouse_xy_2.x, mouse_xy_2.y)){
				if(ui.game_state == WHITE_TURN && ui.board[xy_to_i(mouse_xy_1.x,mouse_xy_1.y)][1] == WHITE_PIECE){
					ui.move_piece(mouse_xy_1.x, mouse_xy_1.y, mouse_xy_2.x,  mouse_xy_2.y); 
					ui.game_state = BLACK_TURN;
				}
				else if(ui.game_state == BLACK_TURN && ui.board[xy_to_i(mouse_xy_1.x,mouse_xy_1.y)][1] == BLACK_PIECE){
					ui.move_piece(mouse_xy_1.x, mouse_xy_1.y, mouse_xy_2.x,  mouse_xy_2.y); 
					ui.game_state = WHITE_TURN;
				}
			}
		}

		BeginDrawing();
			//drawing each frame

			ClearBackground(background_colour);

			//draw board + pieces
			ui.draw_board();
			ui.draw_pieces();

		if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
			//show legal moves for selected piece

				ui.draw_moves(mouse_xy_1.x, mouse_xy_1.y);
			}

			//draw board + square outlines
			ui.draw_outline();

			//print winner
			if       (winner == WHITE_WINNER){
				DrawText("White wins", 400,400, 40, BLACK);
			}else if (winner == BLACK_WINNER){
				DrawText("Black wins", 400,400, 40, BLACK);
			}

			//draw turn square
		ui.draw_turn();

		EndDrawing();
	}
	return 0;
}
