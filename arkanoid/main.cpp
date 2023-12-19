#include <ncurses.h>
#include <time.h>
#include <string>

#define HEIGHT 20
#define WIDTH 42

// TODO:    RESPONSIBE INPUT
//          set destroyed blocks to -1

int main()
{
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
    refresh();
    wrefresh(win);

    keypad(win, true);   // no special keys in use
    nodelay(win, true);

    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);


    bool over = false;
    int xPos = 3;
    int xDir = 0;
    int yPosBall = 6;
    int xPosBall = 23;
    int yDirBall = -1;
    int xDirBall = 1;
    bool moveBall = false;
    int Blocks[4][10] = {(10, 0), (10, 0), (10, 0), (10, 0)};
    std::string paddle = "\\______/";

    int score = 0;
    int health= 3;
    std::string healthStr = "+++";
    mvaddstr(start_y + HEIGHT, start_x, healthStr.c_str());

    srand(time(NULL));
    int input;


    // game loop
    while (!over) {

        // get and process input
        input = wgetch(win);
        switch (input) {
            case 'a':
                xDir = -1;
                break;
            case KEY_LEFT:
                xDir = -1;
                break;
            case 'd':
                xDir = 1;
                break;
            case KEY_RIGHT:
                xDir = 1;
                break;
            case 'q':
                over = true;
                mvaddstr(3, 5, "q pressed");
                break;
            default:
                break;
        }

        // paddle direction indicator
        if (xDir == 1) {
            mvaddch(start_y-2, max_x/2, '>');
        } else if (xDir == -1) {
            mvaddch(start_y-2, max_x/2, '<');
        } else {
            mvaddch(start_y-2, max_x/2, ' ');
        }


    // #################### move #################### //
        // move paddle based on input
        xPos += xDir*2;

        // move ball based on input
        xPosBall += xDirBall;
        yPosBall += yDirBall;

    // #################### collision #################### //
        // paddle border collision
        if (xPos <= 1) {
            xDir = 0;
            xPos = 1;
        }
        else if (xPos >= WIDTH-8) {
            xDir = 0;
            xPos = WIDTH-9;
        }

        // ball border collision
        if (yPosBall >= HEIGHT) {               // bottom
            yDirBall = -1;
            if (health % 2) xDirBall = 1;
            else xDirBall = -1;
            health--;
            healthStr.resize(healthStr.size()-1);
            mvaddstr(start_y + HEIGHT, start_x, "   ");
            mvaddstr(start_y + HEIGHT, start_x, healthStr.c_str());
        } else if (xPosBall >= WIDTH-2) {       // right
            xDirBall = -1;
        } else if (xPosBall <= 1) {             // left
            xDirBall = 1;
        } else if (yPosBall <= 1) {             // top
            yDirBall = 1;
        }

        // ball paddle collision
        if (yPosBall == HEIGHT-3) {
            if (xPosBall == xPos) {                                 // far left
                xDirBall = -2;
                yDirBall = -1;
            }else if (xPosBall == xPos+1 || xPosBall == xPos+2) {   // left
                xDirBall = -1;
                yDirBall = -1;
            } else if (xPosBall == xPos+3) {    // middle
                xDirBall = 0;
                yDirBall = -1;
            } else if (xPosBall == xPos+4 || xPosBall == xPos+5) {    //  right
                xDirBall = 1;
                yDirBall = -1;
            } else if (xPosBall == xPos+6) {    // far right
                xDirBall = 2;
                yDirBall = -1;
            }

        }


    // #################### draw #################### //
        // rm olf frame
        werase(win);
        box(win, 0, 0);
        // draw moved paddle
        mvwaddstr(win, HEIGHT-3, xPos, paddle.c_str());

        // draw blocks
        int c = 1;
        for (int i=0; i<4; i++) {
            for (int j=0; j<10; j++) {

                // ich habe keine ahnung was hier passiert und bedanke mich beim github copilot gott
                if (Blocks[i][j] != -1) {
                    mvwaddstr(win, i+1, (j*4)+1, ".  .");
                    mvwchgat(win, i+1, (j*4)+1, 4, A_REVERSE, c, NULL);
                }
                c++;
                if (c > 3) c = 1;

                // ball block collision
                if (yPosBall == i+1) {
                    if(xPosBall == (j*4)+1 || xPosBall == (j*4)+2 || xPosBall == (j*4)+3 || xPosBall == (j*4)+4) {
                        if (Blocks[i][j] != -1) {
                            score++;
                            Blocks[i][j] = -1;
                            yDirBall *= -1;
                            xDirBall *= -1;
                        }
                    }
                }
            }
        }

        // draw ball
        mvwaddch(win, yPosBall, xPosBall, 'o');

        // win
        if (score == 40) {
            over = true;
            mvaddstr(8, max_x/2 - 5, "YOU WIN!");
        } else if (health == 0) {
            over = true;
            mvaddstr(8, max_x/2 - 5, "YOU LOSE!");
        }


        refresh();
        wrefresh(win);
        napms(80-score);
    }




    getch();
    endwin();

    return 0;
}