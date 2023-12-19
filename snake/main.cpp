#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <queue>

#define HEIGHT 14
#define WIDTH HEIGHT *3


int main() {

    initscr();
    noecho();
    cbreak();
    curs_set(0);

    int max_y = getmaxy(stdscr);
    int max_x = getmaxx(stdscr);
    int start_y = (max_y - HEIGHT) / 2;
    int start_x = (max_x - WIDTH) / 2;

    WINDOW *win = newwin(HEIGHT, WIDTH, start_y, start_x);
    box(win, 0, 0);
    mvaddstr(start_y -1, start_x, "Score: 0");
    refresh();
    wrefresh(win);

    // keypad(win, true);   // no special keys in use
    nodelay(win, true);

    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);

    bool over = false;
    int yPos = 5;
    int xPos = 5;
    int yDir = 0;
    int xDir = 1;
    int xTail[100], yTail[100];
    int yFood = 10;
    int xFood = 12;
    chtype snake = 'o';
    chtype food = 'A';
    int score = 0;

    srand(time(NULL));
    int input;

    // game loop
    while (!over) {

        // get and process input
        input = wgetch(win);
        switch (input) {
            case 'w':
                yDir = -1;
                xDir = 0;
                break;
            case 's':
                yDir = 1;
                xDir = 0;
                break;
            case 'a':
                yDir = 0;
                xDir = -1;
                break;
            case 'd':
                yDir = 0;
                xDir = 1;
                break;
            case 'q':
                over = true;
                break;
            default:
                break;
        }

        // move snake based on input
        yPos += yDir;
        xPos += xDir;

       // snake body
        int yPrev = yTail[0];
        int xPrev = xTail[0];
        yTail[0] = yPos;
        xTail[0] = xPos;
        int yTemp, xTemp;

        for (int i=1; i<score+1; i++) {  // +1 bc score starts at 0
            yTemp = yTail[i];
            xTemp = xTail[i];
            yTail[i] = yPrev;
            xTail[i] = xPrev;
            yPrev = yTemp;
            xPrev = xTemp;

            // check body collision
            if (yPos == yTail[i] && xPos == xTail[i]) {
                over = true;
                mvaddstr(8, max_x/2 - 5, "GAME OVER!");
            }
        }

        // check borders collision
        if (yPos == 0 || yPos == HEIGHT-1 || xPos == 0 || xPos == WIDTH-1) {
            over = true;
            mvaddstr(8, max_x/2 - 5, "GAME OVER!");
        }

        // rm old snake tails
        werase(win);
        box(win, 0, 0);
        // draw new snake tails
        for (int i=0; i<score+1; i++) {  // +1 bc score starts at 0
            mvwaddch(win, yTail[i], xTail[i], snake);
        }
        // change snake head on collision
        if (over) {
            mvwaddch(win, yPos, xPos, 'x');
            mvwchgat(win, yPos, xPos, 1, A_BOLD, 2, NULL);
        }

        // eat apple
        if (yPos == yFood && xPos == xFood) {
            // increase score
            score++;
            mvaddstr(start_y -1, start_x +7, std::to_string(score).c_str());

            // snake eats apple -> replace apple with snake
            mvwaddch(win, yFood, xFood, snake);

            // get empty space
            while((mvwinch(win, yFood, xFood) & A_CHARTEXT) != ' ') {
                yFood = rand() % ((HEIGHT-2) + 1);
                xFood = rand() % ((WIDTH-2) + 1);
            }
        }
        // new apple
        mvwaddch(win, yFood, xFood, food);
        mvwchgat(win, yFood, xFood, 1, A_NORMAL, 1, NULL);


        refresh();
        wrefresh(win);
        napms(125);
    }


    getch();
    endwin();

    return 0;
}
