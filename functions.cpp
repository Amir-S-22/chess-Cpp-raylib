#include "raylib.h"
#include <iostream>
#include "classes.h"

void Game::print_board(){
	//prints board to terminal with pieces as enum equivalent pieces, pawn, white = {1,1}
	
	std::cout << '\n';
	for(int i = 0; i < 64; i++){
		if(i%8 == 0 && i != 0) std::cout << '\n';
		std::cout << "(" << board[i][0] << "," << board[i][1] << ") ";
	}
	std::cout << '\n';
}

int xy_to_i(int x, int y){
	//converts chess coordinates (x,y) to board index
	//returns -1 if out of bounds
	
	if(x > 8 || y > 8) return -1;
	else if(x < 1 || y < 1) return -1;
	return ( (x-1) + (8*(y-1)) );
}


void Game::move_piece(int x_1, int y_1, int x_2, int y_2){
	//checks if piece can move via can_piece_move() and moves piece
	//changes game state if king is dead

	//nothing happens if (x_1,y_1) == (x_2,y_2)
	if(x_1 == x_2 && y_1 == y_2) return;

	//refuse illegal move via can_piece_move()
	if(!can_piece_move(x_1, y_1, x_2, y_2)) return;

	//winner detection
	if(board[xy_to_i(x_2, y_2)][0] == KING && board[xy_to_i(x_2, y_2)][1] != board[xy_to_i(x_1, y_1)][1]){
		if			(board[xy_to_i(x_1, y_1)][1] == WHITE_PIECE) winner = WHITE_WINNER;
		else if (board[xy_to_i(x_1, y_1)][1] == BLACK_PIECE) winner = BLACK_WINNER;
	}

	//copy piece on (x_1,y_1) to (x_2,y_2)
	board[xy_to_i(x_2, y_2)][0] = board[xy_to_i(x_1, y_1)][0];
	board[xy_to_i(x_2, y_2)][1] = board[xy_to_i(x_1, y_1)][1];

	//remove piece on (x_2,y_2)
	board[xy_to_i(x_1, y_1)][0] = NO_PIECE;
	board[xy_to_i(x_1, y_1)][1] = NO_PIECE_COLOUR;

}

bool Game::can_piece_move(int x_1, int y_1, int x_2, int y_2){
	//move logic validation of selectec piece (x_1,y_1)
	//checks movement rules + captures
	//blocks moves that land on same colour piece
	//uses is_path_clear() for sliding pieces
	
	PIECE 			 current_piece        = static_cast<PIECE>       (board[xy_to_i(x_1, y_1)][0]);
	PIECE_COLOUR current_piece_colour = static_cast<PIECE_COLOUR>(board[xy_to_i(x_1, y_1)][1]);

	PIECE 			 future_piece         = static_cast<PIECE>       (board[xy_to_i(x_2, y_2)][0]);
	PIECE_COLOUR future_piece_colour  = static_cast<PIECE_COLOUR>(board[xy_to_i(x_2, y_2)][1]);

	//basic (x_1,y_1) chekcs
	if(current_piece == NO_PIECE) return 0;
	if(current_piece_colour == future_piece_colour) return 0;
	if(x_2 < 1 || x_2 > 8 || y_2 < 1 || y_2 > 8) return 0;


	//for each piece validate specific movement pattern
	switch (current_piece){
		case NO_PIECE:
					break;
		case PAWN:

			//move
			if(current_piece_colour == WHITE_PIECE){
				if     ( (x_2 == x_1) && (y_2 == y_1 - 1) && (future_piece != NO_PIECE)) return 0;
				else if( (x_2 == x_1) && (y_2 == y_1 - 1) ) return 1;
				else if( (y_1 == 7) && (x_2 == x_1) && (y_2 == y_1 - 2 && is_path_clear(x_1,y_1, x_2,y_2))) return 1;
			}else{
				if     ( (x_2 == x_1) && (y_2 == y_1 + 1) && (future_piece != NO_PIECE)) return 0;
				else if( (x_2 == x_1) && (y_2 == y_1 + 1) ) return 1;
				else if( (y_1 == 2) && (x_2 == x_1) && (y_2 == y_1 + 2 && is_path_clear(x_1,y_1, x_2,y_2))) return 1;
			}

			//kill
			if(current_piece_colour == WHITE_PIECE){
				if		 ( (x_2 == x_1 - 1) && (y_2 == y_1 - 1) && future_piece_colour == BLACK_PIECE ) return 1;
				else if((x_2 == x_1 + 1) && (y_2 == y_1 - 1) && future_piece_colour == BLACK_PIECE ) return 1;
			}else{
				if		 ( (x_2 == x_1 - 1) && (y_2 == y_1 + 1) && future_piece_colour == WHITE_PIECE ) return 1;
				else if((x_2 == x_1 + 1) && (y_2 == y_1 + 1) && future_piece_colour == WHITE_PIECE ) return 1;
			}
			break;

		case KNIGHT:

			//move
			if ((current_piece_colour == WHITE_PIECE || current_piece_colour == BLACK_PIECE) && future_piece == NO_PIECE){
				if     ( (x_2 == x_1 - 2) && ( (y_2 == y_1 + 1) || (y_2 == y_1 - 1) ) ) return 1; 
				else if( (y_2 == y_1 - 2) && ( (x_2 == x_1 + 1) || (x_2 == x_1 - 1) ) ) return 1;
				else if( (x_2 == x_1 + 2) && ( (y_2 == y_1 + 1) || (y_2 == y_1 - 1) ) ) return 1;
				else if( (y_2 == y_1 + 2) && ( (x_2 == x_1 + 1) || (x_2 == x_1 - 1) ) ) return 1;
			}

			//kill
			if(current_piece_colour == WHITE_PIECE){
				if     ( (x_2 == x_1 - 2) && ( (y_2 == y_1 + 1) || (y_2 == y_1 - 1) ) && future_piece_colour == BLACK_PIECE) return 1; 
				else if( (y_2 == y_1 - 2) && ( (x_2 == x_1 + 1) || (x_2 == x_1 - 1) ) && future_piece_colour == BLACK_PIECE) return 1;
				else if( (x_2 == x_1 + 2) && ( (y_2 == y_1 + 1) || (y_2 == y_1 - 1) ) && future_piece_colour == BLACK_PIECE) return 1;
				else if( (y_2 == y_1 + 2) && ( (x_2 == x_1 + 1) || (x_2 == x_1 - 1) ) && future_piece_colour == BLACK_PIECE) return 1;
			}else{
				if     ( (x_2 == x_1 - 2) && ( (y_2 == y_1 + 1) || (y_2 == y_1 - 1) ) && future_piece_colour == WHITE_PIECE) return 1; 
				else if( (y_2 == y_1 - 2) && ( (x_2 == x_1 + 1) || (x_2 == x_1 - 1) ) && future_piece_colour == WHITE_PIECE) return 1;
				else if( (x_2 == x_1 + 2) && ( (y_2 == y_1 + 1) || (y_2 == y_1 - 1) ) && future_piece_colour == WHITE_PIECE) return 1;
				else if( (y_2 == y_1 + 2) && ( (x_2 == x_1 + 1) || (x_2 == x_1 - 1) ) && future_piece_colour == WHITE_PIECE) return 1;
			}
			break;

		case BISHOP:

			//check if move is diagonal
			if(std::abs(x_2 - x_1) != std::abs(y_2 - y_1)) return 0;
			//check if path is clear
			if(!is_path_clear(x_1, y_1, x_2, y_2)) return 0;

			//move
			if(future_piece == NO_PIECE && std::abs(x_2 - x_1) == std::abs(y_2 - y_1)) return 1;

			//kill
			if(future_piece_colour != current_piece_colour && (std::abs(x_2 - x_1) == std::abs(y_2 - y_1))) return 1;
			break;

		case ROOK:

			//check if move is straight
			if((x_1 != x_2) && (y_2 != y_1)) return 0;
			//check if path is clear
			if(!is_path_clear(x_1, y_1, x_2, y_2)) return 0;
	
			//move
			if(future_piece == NO_PIECE && ((x_1 == x_2) || (y_2 == y_1))) return 1;

			//kill
			if(future_piece_colour != current_piece_colour && ((x_1 == x_2) || (y_2 == y_1))) return 1;
			break;

		case QUEEN:
			
			//check diagonal or straight
			if( (std::abs(x_2 - x_1) != std::abs(y_2 - y_1)) && ((x_1 != x_2) && (y_2 != y_1)) ) return 0;
			//check if path is clear
			if(!is_path_clear(x_1, y_1, x_2, y_2)) return 0;

			//move
			if(future_piece == NO_PIECE) return 1;

			//kill
			if(future_piece_colour != current_piece_colour) return 1;
			break;

		case KING:

			//check if square is > 1 away
			if(std::abs(x_2 - x_1) > 1 || std::abs(y_2 - y_1) > 1) return 0;

			//move
			if(future_piece == NO_PIECE) return 1;

			//kill
			if(future_piece_colour != current_piece_colour) return 1;
			break;

	}
	return 0;
}

void Game::print_move_map(int x_, int y_){
	//prints all available moves for any piece on (x_,y_)
	
	std::cout << '\n';
	for(int y = 1; y < 9; y++){
		for(int x = 1; x < 9; x++){
			if(x == x_ && y == y_){
				std::cout << "X ";
				continue;
			}
			std::cout << can_piece_move(x_, y_, x, y) << ' ';
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

bool Game::is_path_clear(int x_1, int y_1, int x_2, int y_2){
	//checks if all squares between (x_1,y_1)-(x_2,y_2) are empty
	//used by all non-jump pieces

	int gradient_x = 0;
	int gradient_y = 0;
	PIECE piece = static_cast<PIECE>(board[xy_to_i(x_1, y_1)][0]); 

	if			(x_2 > x_1) gradient_x = 1;
	else if (x_2 < x_1) gradient_x = -1;
	else    						gradient_x = 0;

	if			(y_2 > y_1) gradient_y = 1;
	else if (y_2 < y_1) gradient_y = -1;
	else    						gradient_y = 0;


	if(piece == BISHOP){

		//check if diagonal move
		if((std::abs(x_2 - x_1) != std::abs(y_2 - y_1))) return false;

		//diagonal
		if(std::abs(gradient_x) == std::abs(gradient_y)){
			for(int i = 1; i < std::abs(x_2 - x_1); i++){
				if(board[xy_to_i(x_1 + (i * gradient_x) , y_1 + (i * gradient_y))][0] == NO_PIECE) continue;
				else return false;
			}
		}
	}

	if(piece == ROOK){

		//check if straight move
		if((x_1 != x_2) && (y_2 != y_1)) return false;

		//straight sight
		if			(gradient_x != 0 || gradient_y == 0){
			for(int i = 1; i < std::abs(x_2 - x_1); i++){
				if(board[xy_to_i(x_1 + (i * gradient_x), y_1)][0] == NO_PIECE) continue;
				else return false;
			}
		}else if(gradient_x == 0 || gradient_y != 0){
			for(int i = 1; i < std::abs(y_2 - y_1); i++){
				if(board[xy_to_i(x_1, y_1 + (i * gradient_y))][0] == NO_PIECE) continue;
				else return false;
			}
		}
	}
	if(piece == QUEEN){

		if(std::abs(gradient_x) == std::abs(gradient_y)){
			for(int i = 1; i < std::abs(x_2 - x_1); i++){
				if(board[xy_to_i(x_1 + (i * gradient_x) , y_1 + (i * gradient_y))][0] == NO_PIECE) continue;
				else{
				return false;
				}
			}
		}

		if			(gradient_x != 0 || gradient_y == 0){
			for(int i = 1; i < std::abs(x_2 - x_1); i++){
				if(board[xy_to_i(x_1 + (i * gradient_x), y_1 + (i * gradient_y))][0] == NO_PIECE) continue;
				else return false;
			}
		}else if(gradient_x == 0 || gradient_y != 0){
			for(int i = 1; i < std::abs(y_2 - y_1); i++){
				if(board[xy_to_i(x_1 + (i * gradient_x), y_1 + (i * gradient_y))][0] == NO_PIECE) continue;
				else return false;
			}
		}
	}

	return true;
}

void UI::give_board_coordinates(){
	//iterate through board_array to give it all Square objects inside their coordinates

		Rectangle temp {top_left_coordinate.x, top_left_coordinate.y,
											static_cast<float>(square_len), static_cast<float>(square_len)};
	for(int y_i = 0; y_i < 8; y_i++){
		for(int x_i = 0; x_i < 8; x_i++){
			temp.x += (square_len * x_i);
			temp.y += (square_len * y_i);

			board_array[(y_i*8) + x_i] = temp;

			//reset temp coordinates for next square
			temp.x = top_left_coordinate.x;
			temp.y = top_left_coordinate.y;
		}
	}
}

void UI::draw_board(){
	// draw board background
	// nothing to do with pieces

		for(int i = 0; i < 64; i++){
			if((i/8)%2 == 0){
				if(i % 2 == 0) DrawRectangleRec(board_array[i], BROWN);
				else DrawRectangleRec(board_array[i], DARKBROWN);
			}else{
				if(i % 2 == 0) DrawRectangleRec(board_array[i], DARKBROWN);
				else DrawRectangleRec(board_array[i], BROWN);
			}
		}
}

void UI::draw_outline(){
	//draw outline of board and all squares
	
		for(int i = 0; i < 64; i++){
			DrawRectangleLines(board_array[i].x, board_array[i].y, board_array[i].width, board_array[i].height, BLACK);
		}
		Vector2 top_left_coordinate = {board_array[0].x, board_array[0].y};
		DrawRectangleLines(top_left_coordinate.x, top_left_coordinate.y, square_len*8, square_len*8, BLACK);
}

Vector2 UI::mouse_to_xy(Vector2 mouse_position){
	//converts pixel position of mouse to board (x,y)
	//returns {0,0} square if mouse is out of bounds

	Vector2 mouse_xy = {0, 0};

	float x_left = 0;
	float x_right = 0;
	float y_top = 0;
	float y_bottom = 0;

	for(int i = 0; i < 64; i++){
		x_left = board_array[i].x;
		x_right = board_array[i].x + square_len;

		y_top = board_array[i].y;
		y_bottom = board_array[i].y + square_len;

		//is mouse inside square?
		if(mouse_position.x < x_right && mouse_position.x > x_left && 
				mouse_position.y < y_bottom && mouse_position.y > y_top){
			//convert index to x coordinate
			if(i == 0) mouse_xy.x = 1;
			else if(i%8 == 0) mouse_xy.x = 1;
			else mouse_xy.x = i%8 + 1;
		
			//convert index to y coordinate
			mouse_xy.y = static_cast<int>((i/8) + 1);
		}	
	}
	return mouse_xy;
}

void UI::draw_pieces(){
	//draw all pieces on on the right square

	for(int i = 0; i < 64; i++){
		switch (board[i][0]){
			case PAWN:
				if		 (board[i][1] == WHITE_PIECE) DrawTexture(pawn_w, board_array[i].x, board_array[i].y, WHITE);
				else if(board[i][1] == BLACK_PIECE) DrawTexture(pawn_b, board_array[i].x, board_array[i].y, WHITE);
				break;

			case KNIGHT:
				if		 (board[i][1] == WHITE_PIECE) DrawTexture(knight_w, board_array[i].x, board_array[i].y, WHITE);
				else if(board[i][1] == BLACK_PIECE) DrawTexture(knight_b, board_array[i].x, board_array[i].y, WHITE);
				break;

			case BISHOP:
				if		 (board[i][1] == WHITE_PIECE) DrawTexture(bishop_w, board_array[i].x, board_array[i].y, WHITE);
				else if(board[i][1] == BLACK_PIECE) DrawTexture(bishop_b, board_array[i].x, board_array[i].y, WHITE);
				break;

			case ROOK:
				if		 (board[i][1] == WHITE_PIECE) DrawTexture(rook_w, board_array[i].x, board_array[i].y, WHITE);
				else if(board[i][1] == BLACK_PIECE) DrawTexture(rook_b, board_array[i].x, board_array[i].y, WHITE);
				break;

			case QUEEN:
				if		 (board[i][1] == WHITE_PIECE) DrawTexture(queen_w, board_array[i].x, board_array[i].y, WHITE);
				else if(board[i][1] == BLACK_PIECE) DrawTexture(queen_b, board_array[i].x, board_array[i].y, WHITE);
				break;

			case KING:
				if		 (board[i][1] == WHITE_PIECE) DrawTexture(king_w, board_array[i].x, board_array[i].y, WHITE);
				else if(board[i][1] == BLACK_PIECE) DrawTexture(king_b, board_array[i].x, board_array[i].y, WHITE);
				break;
		}
	}
}

void UI::draw_moves(int x, int y){
	//highlights all legal moves

	Color light_up = {230, 41, 55, 100};

	for(int y_ = 1; y_ < 9; y_++){
		for(int x_ = 1; x_ < 9; x_++){

			//don't highlight starting square
			if(x == x_ && y == y_){
				continue;
			}
			else if(can_piece_move(x,y, x_,y_)){
				DrawRectangleRec(board_array[xy_to_i(x_,y_)], light_up);
			}
		}
	}
}

void UI::draw_turn(){
	//draw who's turn it is
	
	Rectangle turn_rectangle = {90.0, 30.0, static_cast<float>(square_len /2), static_cast<float>(square_len /2)};
	DrawText("Turn:", 20 , turn_rectangle.y + 15, 20, BLACK);
	if(game_state == WHITE_TURN){
		DrawRectangleRec(turn_rectangle, RAYWHITE);
	}else if(game_state == BLACK_TURN){
		DrawRectangleRec(turn_rectangle, BLACK);
	}
	
	//outline of box
	DrawRectangleLines(turn_rectangle.x, turn_rectangle.y, turn_rectangle.width, turn_rectangle.height, BLACK);
}
