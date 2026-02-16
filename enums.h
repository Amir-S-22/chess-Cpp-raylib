#ifndef ENUMS
#define ENUMS

//GAME_STATE    tracks whos turn it is
//PIECE       	type of a piece
//PIECE_COLOUR	colour of a piece
//WINNER      	game outcome


enum GAME_STATE   { WHITE_TURN, BLACK_TURN };
enum PIECE        { NO_PIECE, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };
enum PIECE_COLOUR { NO_PIECE_COLOUR, WHITE_PIECE, BLACK_PIECE };
enum WINNER       { NO_WINNER, WHITE_WINNER, BLACK_WINNER };

#endif
