#include <ncurses.h>
#include <string>
#include <time.h>

#define HEIGHT 22
#define WIDTH HEIGHT *3

// fix stones teleporting
// make destroying stones possilbe
// DONT fix stone overlapping

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

    bool over = false;
    keypad(win, true);
    nodelay(win, true);

    int xPos = 4;
    int yPos = 10;
    int yDir = 0;
    int xDir = 0;
    bool moveY;

    int yBullet[50];
    int xBullet[50];
    int b = 0;
    int xCollision = 28;

    // 3 stones
    int yStone[5];
    int xStone[5] = {WIDTH, WIDTH+20, WIDTH+40, WIDTH+60, WIDTH+80};

    int score = 0;
    int health = 3;

    srand(time(NULL));
    int input;



 // game loop
    while (!over) {

        // get and process input
        input = wgetch(win);
        switch (input) {
            case 'w':
            case KEY_UP:
                // yDir = -1;
                yPos--;
                break;
            case 's':
            case KEY_DOWN:
                // yDir = 1;
                yPos++;
                break;
            case 'q':
                over = true;
                break;
            default:
                break;
        }

        // move rocket based on input, y every second frame
        // if (moveY == false) {
        //     moveY = true;
        //     yPos += yDir;
        // } else {
        //     moveY = false;
        // };
        yPos += yDir;

        // rocket borders
        if (yPos < 2) {
            yPos = 2;
        } else if (yPos > HEIGHT-3) {
            yPos = HEIGHT-3;
        }

        // remove old frame
        werase(win);
        box(win, 0, 0);

        // draw moved rocket
        mvwaddch(win, yPos-1, xPos+2, '\\');
        mvwaddstr(win, yPos, xPos, "=#)0)}-");
        mvwaddch(win, yPos+1, xPos+2, '/');

        // draw stone
        for (int i=0; i<5; i++) {

            // teleport stone
            if (xStone[i] < -3) {
                xStone[i] = WIDTH + rand() % 10;    // versetzt
                yStone[i] = rand() % 18 + 2;    // random y
            }

            // move stone
            xStone[i]--;

            // draw stone
            if (yStone[i] != -10) {
                mvwaddstr(win, yStone[i]-1, xStone[i],  "/[I]\\");
                mvwaddstr(win, yStone[i],   xStone[i],  "[[I]]");
                mvwaddstr(win, yStone[i]+1, xStone[i], "\\[I]/");
            }
        }



        // set bullet position
        yBullet[b] = yPos;      // current yPos
        xBullet[b] = xPos+8;    // current xPos (constant)
        // draw bullets
        for (int i = 0; i < 28; i++) {

            // // bullet stone collision
            // for (int j=0; j<5; j++) {
            //     if (xBullet[i] == xStone[j]) yStone[j] = -10;    // ich weiss es doch auch nicht
            //     score += 10;
            //     mvaddstr(0, 0, std::to_string(score).c_str());
            // }

            // 2 turrets (above and below)
            mvwaddstr(win, yBullet[i], xBullet[i], "> ");
            //mvwaddstr(win, yBullet[i]+1, xBullet[i], "--");
            // move bullet for next frame, y stays the same
            xBullet[i] += 2;
        }
        b++;
        if (b > 27) b = 0;








        refresh();
        wrefresh(win);
        // usleep(100000);
        napms(50-score*0.1);
    }

    getch();
    endwin();

    return 0;
}