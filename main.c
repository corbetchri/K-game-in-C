/**
 * NWEN 241 Programming Assignment 3
 * main.c C Source File
 *
 * This program is provide as-is without any warranty. You may use this file to
 * test your function implementations.
 *
 * This program will accept the following keystrokes as valid:
 * - UP/DOWN/LEFT/RIGHT arrow keys for making a move
 * - S or s to save game
 * - L or l to load a saved game
 * - X or x to exit
 */


#include <stdio.h>
#include <string.h>
#include <curses.h>


#include "kgame.h"

#define VALID_INPUTS_STRING   "Valid Inputs: [UP/DN/LT/RT] [S/s:Save] [L/l:Load] [X/x:Exit]"
#define DEFAULT_OUTPUT_STRING "You have not implemented kgame_render() yet."

static kgame_t kgame;
static char kgame_buffer[KGAME_OUTPUT_BUFLEN] = DEFAULT_OUTPUT_STRING;
static WINDOW *header;
static WINDOW *status;
static WINDOW *field;

static char *dir_string[] = {
    "UNKNOWN", "UP", "DOWN", "LEFT", "RIGHT"
};

static void init_screen(void)
{
    initscr();
    cbreak();
    noecho();

    nonl();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);

    refresh();
}

static int get_user_input(void)
{
    int c;
    do {
        c = getch();
        switch(c) {
            case KEY_UP:
                return UP;
            case KEY_DOWN:
                return DOWN;
            case KEY_LEFT:
                return LEFT;
            case KEY_RIGHT:
                return RIGHT;
            case 'S':
            case 's':
                return SAVE;
            case 'L':
            case 'l':
                return LOAD;
            case 'X':
            case 'x':
                return EXIT;
            default:
                break;
        }
    } while(1);
}

static void init_header(void)
{
    header = newwin(3, COLS-1, 0, 0);
    wmove(header, 0, (COLS-strlen(KGAME_TITLE))/2);
    waddstr(header, KGAME_TITLE);
    wmove(header, 1, (COLS-strlen(VALID_INPUTS_STRING))/2);
    waddstr(header, VALID_INPUTS_STRING);
    wrefresh(header);
}

static void init_status(void)
{
    status = newwin(1, COLS-1, LINES-1, 0);
    wmove(status, 0, 0);
    waddstr(status, "Game started. Press any valid input key to proceed... ");
    wrefresh(status);
}

static void init_field(void)
{
    field = newwin(LINES-4, COLS-1, 3, 0);
}

static void update_status(char *s)
{
    wclear(status);
    wmove(status, 0, 0);
    waddstr(status, s);
    wrefresh(status);
}

int main(void)
{
    bool render = true;

    /* Initialize the screen */
    init_screen();

    /* Initialize the game */
    kgame_init(&kgame);

    /* Initialize the "windows" */
    init_header();
    init_field();
    init_status();

    /* Game loop */
    while (1) {
        /* Render */
        if(render == true) {
            kgame_render(kgame_buffer, &kgame);
            wclear(field);
            wmove(field, 0, 0);
            waddstr(field, kgame_buffer);
            wrefresh(field);
            wrefresh(status);
        }

        /* Check if game is won, and do what is right */
        if(kgame_is_won(&kgame)) {
            update_status("Congratulations! You won the game.");
            break;
        }

        /* Check if move is still possible, and do what is right */
        if(kgame_is_move_possible(&kgame) == false) {
            update_status("Sorry, you ran out of moves. Game over!");
            break;
        }

        /* Get a valid input */
        int in = get_user_input();
        char move[32] = "Last move: ";

        switch(in) {
            case EXIT:
                update_status("Exiting...");
                return 0;
            case SAVE:
                kgame_save(&kgame);
                update_status("Tried to save game.");
                break;
            case LOAD:
                render = kgame_load(&kgame);
                update_status("Tried to load game.");
                break;
            case UP:
            case DOWN:
            case LEFT:
            case RIGHT:
                render = kgame_update(&kgame, (dir_t)in);
                update_status(strcat(move, dir_string[in]));
                break;
            default:
                break;
        }

        refresh();
    }

//    endwin();


    return 0;
}
