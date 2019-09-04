/**
 * NWEN 241 Programming Assignment 3
 * kgame.c C Source File
 *
 * Name: Chris Corbett
 * Student ID: 300370980
 *
 * IMPORTANT: Implement the functions specified in kgame.h here.
 * You are free to implement additional functions that are not specified in kgame.h here.
 */

#include <stdlib.h>
#include "kgame.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool gameSaved = false;
bool gameLoaded = false;

/**
 * Initialize the game.
 * Set all elements in the game board to the space ' '  character
 * and set score to 0.
 * @param game Pointer to the game structure
 */
void kgame_init(kgame_t *game)
{
	if(!game) return;

	for(int i=0; i<KGAME_SIDES; i++)
		for(int j=0; j<KGAME_SIDES; j++)
			game->board[i][j] = ' ';
	game->score = 0;
}

/**
 * Adds random A or B tile to the game.
 * You have to fix the current implementation which is very dumb (see kgame.c).
 * @param game Pointer to the game structure
 */
void kgame_add_random_tile(kgame_t *game)
{
	int row, col;

	if(!game) return;

	//	CHECKS ALL SPACES AND ADDS TO COUNT IF EMPTY. IF COUNT = 16/BOARD HAS NO EMPTY SPACES, THEN RETURN(DON'T ADD A TILE)
	int count = 0;
	for(int i = 0; i < KGAME_SIDES; i++){
		for(int j = 0; j < KGAME_SIDES; j++){
			if(game->board[i][j] != ' ') count++;
		}
	}
	if(count == 16) return;

	do {
		row = rand() % 4;
		col = rand() % 4;
	} while(game->board[row][col] != ' ');

	game->board[row][col] = 'A' + (rand() % 2);

}

/**
 * Renders the game, show the score and game board.
 * In this assignment, we will not specify how the game field should be
 * drawn. Feel free to implement your own design. The function must not directly
 * output to the screen, rather, the output should be directed to
 * output_buffer. The output should not exceed 18 lines, and every line should not
 * exceed 40 characters.
 * @param output_buffer Pointer to an array of characters
 * @param game Pointer to the game structure to render
 */
void kgame_render(char *output_buffer, const kgame_t *game)
{

	// CREATES TWO ARRAYS, ONE FOR SPRINTF FUNCTION TO PRINT TO, THE OTHER TO CONCAT A FINAL PRODUCT TO DISPLAY
	char edit[KGAME_OUTPUT_BUFLEN] = {"\0"};
	char concat[KGAME_OUTPUT_BUFLEN] = {"\0"};

	sprintf(edit,"\n\n   Current Score: %d\n   ", game->score);
	strcat(concat, edit);

	//	PRINT TOP BORDER OF GAME FIELD
	for (int i = 0; i < KGAME_SIDES; i++){
		sprintf(edit,"%s","+---");
		strcat(concat, edit);
	}
	sprintf(edit,"+");
	strcat(concat, edit);

	//	PRINT GAME FIELD/CHARACTERS
	for(int i = 0; i < KGAME_SIDES; i++){

		sprintf(edit,"\n   |");
		strcat(concat, edit);

		for (int j = 0; j < KGAME_SIDES; j++){

			if(game->board[i][j] == ' ') {
				sprintf(edit,"   |");
				strcat(concat, edit);
			}
			else {
				sprintf(edit," %c |", game->board[i][j]);
				strcat(concat, edit);
			}
		}

		//	PRINT HORIZONTAL BORDERS INSIDE GAME FIELD
		sprintf(edit,"\n   +");
		strcat(concat, edit);
		for(int i = 0; i < KGAME_SIDES; i++){
			sprintf(edit,"---+");
			strcat(concat, edit);
		}

	}

	sprintf(edit,"\n");
	strcat(concat, edit);

	// NOTIFY USER OF SUCCESSFULL GAME SAVE
	if(gameSaved == true){
		sprintf(edit,"\n The game was saved successfully.");
		strcat(concat, edit);
		gameSaved = false;
	}

	// NOTIFY USER OF SUCCESSFULL GAME LOAD
	if(gameLoaded == true){
		sprintf(edit,"\n The game was loaded successfully.");
		strcat(concat, edit);
		gameLoaded = false;
	}

	// PRINT OUTPUT(CONCAT)
	sprintf(output_buffer, "%s" , concat);

}


/**
 * Checks whether game is already won.
 * Returns true, if tile with letter 'K' is located on the board. Returns
 * false, if it is not.
 * @param game Pointer to the game structure
 * @return true, if game is won, or false ot->herwise.
 */
bool kgame_is_won(const kgame_t *game)
{

	// RETURNS TRUE IF A K TILE IS FOUND/THE GAME IS WON
	for(int i = 0; i < KGAME_SIDES; i++){
		for(int j = 0; j < KGAME_SIDES; j++){
			if(game->board[i][j] == 'K'){
				return true;
			}
		}
	}

	return false;

}

/**
 * Checks whether it is possible to make move.
 * Function checks game board if it is possible to make another move. A move
 * is possible if there is at least one empty field (tile) in the game field, or,
 * at least two adjacent tiles of the same character are located either
 * vertically or horizontally in the grid.
 * @param game Pointer to the game structure
 * @return true, if another movement is possible, or false otherwise.
 */
bool kgame_is_move_possible(const kgame_t *game)
{


	// RETURNS TRUE IF THERE ARE ANY EMPTY TILES ON THE BOARD
	for(int i = 0; i < KGAME_SIDES; i++){
		for(int j = 0; j < KGAME_SIDES; j++){
			if(game->board[i][j] == ' '){
				return true;
			}
		}
	}

	// RETURNS TRUE IF THERE ARE TWO HORIZONTAL ADJACENT TILES ON THE BOARD
	for(int i = 0; i < KGAME_SIDES; i++){
		for(int j = 0; j < KGAME_SIDES - 1; j++){
			if(game->board[i][j] == game->board[i][j+1]){
				return true;
			}
		}
	}

	// RETURNS TRUE IF THERE ARE TWO VERTICAL ADJACENT TILES ON THE BOARD
	for(int i = 0; i < KGAME_SIDES - 1; i++){
		for(int j = 0; j < KGAME_SIDES; j++){
			if(game->board[i][j] == game->board[i+1][j]){
				return true;
			}
		}
	}

	return false;

}

/**
 * Updates the score given the value of two merged tiles
 * @param game Pointer to the game structure
 * @param piece(s) being merged
 * @return the value of the score to be added (is multiplied*2 in update method for two tile merge)
 */
int update_score(const kgame_t *game, char piece){
	if(piece == 'A') return 2;
	else if(piece == 'B') return 4;
	else if(piece == 'C') return 8;
	else if(piece == 'D') return 16;
	else if(piece == 'E') return 32;
	else if(piece == 'F') return 64;
	else if(piece == 'G') return 128;
	else if(piece == 'H') return 256;
	else if(piece == 'I') return 512;
	else if(piece == 'J') return 1024;
	else if(piece == 'K') return 2048;
	else return 0;
}


/**
 * Makes a move in the given direction.
 * If it is possible, function makes move in given direction, updates the
 * current game state (board and score) and returns true. If it is not
 * possible to move, returns false.
 * @param game Pointer to the game structure
 * @param direction Direction of move (UP, DOWN, LEFT or RIGHT)
 * @return true, if game state was updated, false otherwise
 */
bool kgame_update(kgame_t *game, dir_t direction)
{

	// I HAVE COMMENTED THE FIRST 'RIGHT' PART ONLY. OTHER DIRECTIONS ARE EXACT SAME LOGIC YET OBVS VICE VERSA/ETC FOR ROW/COL SWTICHES BLAH BLAH

	if(direction == RIGHT){

		// START AT TOP ROW AND SECOND FROM RIGHT COL(J=KGAME_SIDES - 2). WE DONT NEED TO CHECK FAR RIGHT COL AS IT CANNOT SLIDE FURTHER RIGHT(PREVENT OUT OF BOUNDS EXCEPTION WHEN CHECKING +1'S)
		for(int i = 0; i < KGAME_SIDES; i++){
			int count = -1;	// COUNTS HOW MANY TIMES A TILE HAS MERGED(ONLY ALLOWS IT ONCE PER ROW)(BECAUSE IT RESETS EACH ROW ITERATION) (IT STARTS AT MINUS ONE SO THAT WHEN IN THE 0th ROW IT STILL CHECKS (BECAUSE COUNT=MOVE BEFORE THE 'BREAK' CALL))
			for(int j = KGAME_SIDES - 2; j >= 0; j--){
				if(game->board[i][j] != ' '){
					// THEN SLIDE UNTIL ALL IN ROW HAVE MOVED
					for(int move = j; move < KGAME_SIDES - 1; move++){
						if(game->board[i][move+1] == ' '){
							game->board[i][move+1] = game->board[i][move];
							game->board[i][move] = ' ';
						}
						// AND IF THERE ARE TWO ADJACENT TILES, THEN MERGE THEM AND UPDATE SCORE. THE COUNT ONLY ALLOWS TWO TILES TO MERGE ONCE PER ROW. (STOPS BBBB ENDING UP AS D, RATHER, IT STOPS AT --CC)
						if(game->board[i][move] == game->board[i][move+1] && count != move){
							game->board[i][move+1] += 1;
							game->score += update_score(game, game->board[i][move]) * 2;
							game->board[i][move] = ' ';
							count = move;
							break;
						}
					}
				}
			}
		}
		kgame_add_random_tile(game);
		return true;
	}

	if(direction == LEFT){

		for(int i = 0; i < KGAME_SIDES; i++){
			int count = -1;
			for(int j = 1; j < KGAME_SIDES; j++){
				if(game->board[i][j] != ' '){
					for(int move = j; move > 0 ; move--){
						if(game->board[i][move-1] == ' '){
							game->board[i][move-1] = game->board[i][move];
							game->board[i][move] = ' ';
						}
						if(game->board[i][move] == game->board[i][move-1] && count != move){
							game->board[i][move-1] += 1;
							game->score += update_score(game, game->board[i][move]) * 2;
							game->board[i][move] = ' ';
							count = move;
							break;
						}
					}
				}
			}
		}
		kgame_add_random_tile(game);
		return true;
	}

	if(direction == UP){
		for(int j = 0; j < KGAME_SIDES; j++){
			int count = -1;
			for(int i = 1; i < KGAME_SIDES; i++){
				if(game->board[i][j] != ' '){
					for(int move = i; move > 0 ; move--){
						if(game->board[move-1][j] == ' '){
							game->board[move-1][j] = game->board[move][j];
							game->board[move][j] = ' ';
						}
						if(game->board[move][j] == game->board[move-1][j] && count != move){
							game->board[move-1][j] += 1;
							game->score += update_score(game, game->board[move][j]) * 2;
							game->board[move][j] = ' ';
							count = move;
							break;
						}
					}
				}
			}
		}
		kgame_add_random_tile(game);
		return true;
	}

	//	SLIDE DOWN
	if(direction == DOWN){

		for(int j = 0; j < KGAME_SIDES; j++){
			int count = -1;
			for(int i = KGAME_SIDES-2; i >= 0; i--){
				if(game->board[i][j] != ' '){
					for(int move = i; move < KGAME_SIDES - 1; move++){
						if(game->board[move+1][j] == ' '){
							game->board[move+1][j] = game->board[move][j];
							game->board[move][j] = ' ';
						}
						if(game->board[move][j] == game->board[move+1][j] && count != move){
							game->board[move+1][j] += 1;
							game->score += update_score(game, game->board[move][j]) * 2;
							game->board[move][j] = ' ';
							count = move;
							break;
						}
					}
				}
			}
		}
		kgame_add_random_tile(game);
		return true;
	}

	return false;
}


/**
 * Saves the game state to a file.
 * The function must write the contents of game to a file. The name of the file
 * is specified by the symbolic constant KGAME_SAVE_FILE. If the file already
 * exists, its contents are overwritten. The format of the file is simple and is
 * shown in the Assignment 3 Task 5 specifications.
 * @param game Pointer to the game structure
 */
void kgame_save(const kgame_t *game)
{

	// DECLARES A FILE TO WRITE TO
	FILE *fp;
	fp = fopen(KGAME_SAVE_FILE, "w");

	if(fp == NULL){
		return;
	}

	// ITERATE THROUGH GAME BOARD. ADD TO PIECE TO FILE WITH PUTC. (CHANGES A SPACE TO A '-')
	char piece;
	for(int i = 0; i < KGAME_SIDES; i++){
		for(int j = 0; j < KGAME_SIDES; j++){
			if(game->board[i][j] == ' ') piece = '-';
			else piece = game->board[i][j];
			putc(piece, fp);
		}
	}

	// PRINTS THE SCORE TO THE END OF THE FILE
	fprintf(fp, " %d", game->score);
	fclose(fp);

	// HELPS NOTIFY USER OF SUCCESSFUL GAME SAVE IN kgame_render
	gameSaved = true;
}


/**
 * Load game state from a file.
 * The function must read the contents of KGAME_SAVE_FILE and ``copy'' the
 * contents to the structure pointed to by game accordingly. If the file
 * does not exist, or if the file format is invalid, or the contents are invalid,
 * the function must not modify game and must return false. If the file exists
 * and its contents are valid, then the function must modify game accordingly and
 * return true.
 * @param game Pointer to the game structure
 */
bool kgame_load(kgame_t *game)
{

	int score;

	// DECLARES A FILE TO READ FROM
	FILE *fp;
	fp = fopen(KGAME_SAVE_FILE, "r");

	if(fp == NULL) return false;
	// invalid format or contents are invalid? return false ???

	// ITERATE THROUGH FILE. FILL GAME BOARD WITH PIECES USING FGETC AND CHAR C
	for(int i = 0; i < KGAME_SIDES; i++){
		for(int j = 0; j < KGAME_SIDES; j++){
			char c = fgetc(fp);
			if(c == '-') game->board[i][j] = ' ';
			else game->board[i][j] = c;
		}
	}

	// GET SCORE FROM END OF FILE
	fscanf(fp, " %d", &score);
	game->score = score;

	// CLOSE THE FILE
	fclose(fp);

	// HELPS NOTIFY USER OF SUCCESSFUL GAME LOAD IN kgame_render
	gameLoaded = true;

	return true;

}


