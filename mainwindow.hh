/* Class: MainWindow
 * MainWindow class represents the gui of Tetris game.
 *
 * Program author
 * Name: Jori Rintakangas
 * Student number: 291679
 * UserID: ksjori
 * E-Mail: jori.rintakangas@tuni.fi
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

    // The scene where tetrominos are drawed
    QGraphicsScene* scene_;

    // Used to get the gaming time
    QTimer game_timer_;

    // Used for continuous fuction calls
    QTimer timer_;

    // Moves tetromino step down
    void move_tetrominos();

    // Creates new tetromino
    void new_tetromino();

    // Checks if tetromino can move down. True if can, false otherwise
    bool can_move_down();

    // Stores the information of where tetromino stopped
    void store_item_info();

    // Before game creates vector that holds information of tetromino locations
    void initialize_screen_layout();

    // Checks if tetromino can move right. True if can, false otherwise
    bool can_move_right();

    // Checks if tetromino can move left. True if can, false otherwise
    bool can_move_left();

    bool can_rotate(bool clockwise);

    void check_for_full_row();

    // Calculates gaming time
    void time_passed();

    // Game status
    bool game_over_ = false;

    // Constants describing scene coordinates
    const int STEP = 20;
    const int BORDER_UP = 0;
    const int BORDER_DOWN = 480;
    const int BORDER_LEFT = 0;
    const int BORDER_RIGHT = 240;

    // Size of a tetromino component
    const int SQUARE_SIDE = 20;
    // Number of horizontal cells (places for tetromino components)
    const int COLUMNS = BORDER_RIGHT / SQUARE_SIDE;
    // Number of vertical cells (places for tetromino components)
    const int ROWS = BORDER_DOWN / SQUARE_SIDE;

    // Represents single square on the screen,
    // single tetromino is formed by four of these
    QGraphicsRectItem* square_;

    // Represents a single tetromino.
    std::vector<QGraphicsRectItem*> tetromino_;
    Tetromino* new_tetromino_;
    // For randomly selecting the next dropping tetromino.
    std::default_random_engine randomEng;
    std::uniform_int_distribution<int> distr;

    // This map contains all squares on the game layout.
    std::map<std::pair<qreal,qreal>,bool> screen_layout_;

    std::vector<std::pair<qreal,qreal>> new_coords_;
    std::vector<Tetromino*> tetrominos_;

    // These numbers are used when counting the gaming time.
    int minutes_ = 0;
    int seconds_ = 0;
};

#endif // MAINWINDOW_HH
