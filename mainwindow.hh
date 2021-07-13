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
#include <QGraphicsItemGroup>
#include <QRect>
#include <QTimer>
#include <random>
#include <vector>
#include <algorithm>
#include <cctype>
#include <map>


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
    Ui::MainWindow *ui_;

    QGraphicsScene* scene_;

    QTimer game_timer_;

    QTimer timer_;

    void move_tetrominos();

    void new_tetromino();

    bool can_move_down();

    void store_item_info();

    void initialize_screen_layout();

    bool can_move_right();

    bool can_move_left();

    bool can_rotate(bool clockwise);

    void check_for_full_row();

    void time_passed();

    bool game_over_ = false;

    const int STEP = 20;
    const int BORDER_UP = 0;
    const int BORDER_DOWN = 480;
    const int BORDER_LEFT = 0;
    const int BORDER_RIGHT = 240;

    const int SQUARE_SIDE = 20;
    const int COLUMNS = BORDER_RIGHT / SQUARE_SIDE;
    const int ROWS = BORDER_DOWN / SQUARE_SIDE;


    QGraphicsRectItem* square_;

    std::vector<QGraphicsRectItem*> tetromino_;
    Tetromino* new_tetromino_;

    std::default_random_engine randomEng;
    std::uniform_int_distribution<int> distr;

    std::map<std::pair<qreal,qreal>,bool> screen_layout_;

    std::vector<std::pair<qreal,qreal>> new_coords_;
    std::vector<Tetromino*> tetrominos_;

    int minutes_ = 0;
    int seconds_ = 0;
};

#endif // MAINWINDOW_HH
