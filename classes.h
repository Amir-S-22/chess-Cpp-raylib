#ifndef CLASSES
#define CLASSES

#include "raylib.h"
#include "enums.h"

// contains the chess engine model and the UI layer on top of the engine
// game class contains the actual chess engine with logic without a UI
// UI class requires be inherited from game, it acts on top of the game class


//------------------------------------------------- GAME CLASS -----------------------------------------------

class Game{

	public:
		//player turn
		GAME_STATE game_state = WHITE_TURN;

		//stores winner
		WINNER winner;

		//stores board layout as a 2d array: (piece, colour)
		//index starts from top left square and works its way right and donw
		//for indexing, use xy_to_i( int x, int y)
		//{1 (pawn), 2 (black)} = pawn, black
		//{1 (pawn), 1 (white)} = pawn, white
		int board [64][2] = {{ROOK,2}, {KNIGHT,2}, {BISHOP,2}, {QUEEN,2}, {KING,2}, {BISHOP,2}, {KNIGHT,2}, {ROOK,2}, 
					   						{1,2}, {1,2}, {1,2}, {1,2}, {1,2}, {1,2}, {1,2}, {1,2},
												{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, 
						 						{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, 
					   						{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, 
						 						{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, 
						 						{1,1}, {1,1}, {1,1}, {1,1}, {1,1}, {1,1}, {1,1}, {1,1}, 
						 						{ROOK,1}, {KNIGHT,1}, {BISHOP,1}, {QUEEN,1}, {KING,1}, {BISHOP,1}, {KNIGHT,1}, {ROOK,1}};

		//for debug: prints all squares to terminal
		void print_board();

		//moves a piece if it's allowed
		void move_piece(int x_1, int y_1, int x_2, int y_2);

		//returns bool if x_1,y_1 piece is allowed to move to x_2,y_2 square
		bool can_piece_move(int x_1, int y_1, int x_2, int y_2);
		
		//for debug: prints everywhere the piece on that square can legally move
		void print_move_map(int x_, int y_);

		//checks path between two squares to see if path is clear of any other pieces
		bool is_path_clear(int x_1, int y_1, int x_2, int y_2);
};

//------------------------------------------------- UI CLASS -----------------------------------------------

class UI:public Game{

	//for rendering and input handling

	public:

		//white piece textures
		Texture2D pawn_w = LoadTexture("../textures/pawn_w.png");
		Texture2D knight_w = LoadTexture("../textures/knight_w.png");
		Texture2D bishop_w = LoadTexture("../textures/bishop_w.png");
		Texture2D rook_w = LoadTexture("../textures/rook_w.png");
		Texture2D queen_w = LoadTexture("../textures/queen_w.png");
		Texture2D king_w = LoadTexture("../textures/king_w.png");

		//black piece textures
		Texture2D pawn_b = LoadTexture("../textures/pawn_b.png");
		Texture2D knight_b = LoadTexture("../textures/knight_b.png");
		Texture2D bishop_b = LoadTexture("../textures/bishop_b.png");
		Texture2D rook_b = LoadTexture("../textures/rook_b.png");
		Texture2D queen_b = LoadTexture("../textures/queen_b.png");
		Texture2D king_b = LoadTexture("../textures/king_b.png");

		//square size in pixels
		double square_len = 90;

		//board top-left coordinate
		Vector2 top_left_coordinate = {static_cast<float>((1200/2) - square_len*4), 30.0};

		//coordinates of each square (their top-left corner coordinate)
		Rectangle board_array [64] = {};

		//convert mouse position to square 
		Vector2 mouse_to_xy(Vector2 mouse_position);

		//initialize board_array with it's coordinates
		void give_board_coordinates();

		//draw board tiles
		void draw_board();

		//draw board outlines
		void draw_outline();

		//draw all pieces
		void draw_pieces();

		//highlight legal moves
		void draw_moves(int x, int y);

		//drawn turn box on top left of screen
		void draw_turn();
};

#endif
