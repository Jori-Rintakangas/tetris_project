/* Class: MainWindow
 * MainWindow class represents the gui of Tetris game.
 *
 * Program author:
 * Jori Rintakangas
 */

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "tetromino.hh"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QRect>
#include <QTimer>
#include <random>
#include <vector>
#include <map>
#include <algorithm>

const int STEP = 20;
const int BORDER_UP = 0;
const int BORDER_DOWN = 480;
const int BORDER_LEFT = 0;
const int BORDER_RIGHT = 240;

const int SQUARE_SIDE = 20;
const int COLUMNS = BORDER_RIGHT / SQUARE_SIDE;
const int ROWS = BORDER_DOWN / SQUARE_SIDE;

const int LEFT_MARGIN = 100;
const int TOP_MARGIN = 150;

struct Comp
{
    bool operator()(std::pair<qreal, qreal> p1, std::pair<qreal, qreal> p2)
    {
        if ( p1.second == p2.second )
        {
            return p1.first > p2.first;
        }
        return p1.second > p2.second;
    }
};


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void keyPressEvent(QKeyEvent* event) override;

private slots:

    void on_start_push_button_clicked();

    void on_reset_push_button_clicked();

private:

    void move_tetrominos();

    void new_tetromino();

    bool can_move_down();

    void store_item_info();

    void initialize_screen_layout();

    bool can_move_right();

    bool can_move_left();

    bool can_rotate(bool clockwise);

    void check_for_full_row();

    void erase_full_row(qreal row_y_coord);

    void time_passed();

    Ui::MainWindow *ui_;

    QGraphicsScene* scene_;

    QTimer game_timer_;
    QTimer timer_;

    Tetromino* tetromino_;

    std::default_random_engine random_eng_;
    std::uniform_int_distribution<int> distr_;

    std::map<std::pair<qreal, qreal>, bool, Comp> screen_layout_;

    std::vector<std::pair<qreal, qreal>> new_coords_;
    std::vector<Tetromino*> tetrominos_;

    bool game_over_ = false;

    int minutes_ = 0;
    int seconds_ = 0;
};

#endif // MAINWINDOW_HH
