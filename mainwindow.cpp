/* Class: MainWindow
 * This file contains the implementations of MainWindow class methods.
 *
 * Program author
 * Name: Jori Rintakangas
 * Student number: 291679
 * UserID: ksjori
 * E-Mail: jori.rintakangas@tuni.fi
 */

#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsRectItem>


/* Constructor
 */
MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);

    // Graphics scene where rectangles are drawed.
    scene_ = new QGraphicsScene(this);

    // The graphicsView object is placed on the window
    // at the following coordinates
    int left_margin = 100; // x coordinate
    int top_margin = 150; // y coordinate

    // Setting the graphicsView size.
    ui_->graphicsView->setGeometry(left_margin, top_margin,
                                  BORDER_RIGHT + 2, BORDER_DOWN + 2);
    ui_->graphicsView->setScene(scene_);

    // Setting the game screen size.
    scene_->setSceneRect(0, 0, BORDER_RIGHT - 1, BORDER_DOWN - 1);

    // Setting colours to the text browser
    ui_->textBrowser->setTextColor(Qt::black);
    ui_->textBrowser->setStyleSheet("background-color: red");

    // Setting random engine to get tetrominos randomly.
    int seed = time(0);
    randomEng.seed(seed);
    distr = std::uniform_int_distribution<int>(0, 3);
    distr(randomEng); // Wiping out the first random number

    // Setting colours for gaming time displays.
    ui_->lcd_number_minutes->setStyleSheet("background-color: red");
    ui_->lcd_number_seconds->setStyleSheet("background-color: red");

    // Setting timers for tetromino movements and for gaming time
    timer_.setSingleShot(false);
    game_timer_.setSingleShot(false);
    connect(&timer_, &QTimer::timeout, this, &MainWindow::move_tetrominos);
    connect(&game_timer_, &QTimer::timeout, this, &MainWindow::time_passed);
}


/* Destructor
 */
MainWindow::~MainWindow()
{
    delete ui_;
}


/* Executes a tetromino movement according to the given key input.
 */
void MainWindow::keyPressEvent(QKeyEvent* event)
{
    qreal delta_x = STEP;
    qreal delta_y = 0;
    if ( event->key() == Qt::Key_D )
    {
        if ( can_move_right() )
        {
            for ( auto& square : tetromino_ )
            {
                square->moveBy(delta_x, delta_y);
            }
        }
    }
    if ( event->key() == Qt::Key_A )
    {
        if ( can_move_left() )
        {
            for ( auto& square : tetromino_ )
            {
                square->moveBy(-delta_x, delta_y);
            }
        }
    }
    if ( event->key() == Qt::Key_P )
    {
        if ( can_rotate_clockwise() )
        {
            new_tetromino_->rotate_clockwise();
        }
    }
    if ( event->key() == Qt::Key_O )
    {
        if ( can_rotate_anticlockwise() )
        {
            new_tetromino_->rotate_anticlockwise();
        }
    }
}


/* Sets start button disabled, initializes screen layout, starts timers
 * and calls function new_tetromino to create tetromino on the screen.
 */
void MainWindow::on_start_push_button_clicked()
{
    ui_->start_push_button->setDisabled(true);
    ui_->textBrowser->setText("Game on!");
    initialize_screen_layout();
    timer_.start(500);
    game_timer_.start(1000);
    new_tetromino();
}


/* This method resets the game when reset button is pressed.
 */
void MainWindow::on_reset_push_button_clicked()
{
    // List contains all rectItems on the screen.
    QList<QGraphicsItem*> items = ui_->graphicsView->items();

    for ( auto& item : items )
    {
        scene_->removeItem(item);
    }
    seconds_ = 0;
    minutes_ = 0;
    timer_.stop();
    game_timer_.stop();

    ui_->lcd_number_seconds->display(seconds_);
    ui_->lcd_number_minutes->display(minutes_);

    game_over_ = false;
    screen_layout_.clear();
    tetromino_.clear();
    ui_->textBrowser->clear();
    ui_->start_push_button->setDisabled(false);
}


/* Moves a single tetromino one step down if moving is possible.
 *
 */
void MainWindow::move_tetrominos()
{
    qreal delta_x = 0;
    qreal delta_y = STEP;

    if ( can_move_down() )
    {
        for ( auto& square : tetromino_ )
        {
            square->moveBy(delta_x, delta_y);
        }
    }
    else
    {
        store_item_info();
        if ( not game_over_ )
        {
            tetromino_.clear();
            new_tetromino();
        }
        else
        {
            timer_.stop();
            game_timer_.stop();
            ui_->textBrowser->setTextColor(Qt::black);
            ui_->textBrowser->setText("Game over!");
        }
    }
}


/* Creates a random tetromino according to the random number value.
 */
void MainWindow::new_tetromino()
{
    new_tetromino_ = new Tetromino(scene_);
    int random = distr(randomEng); // random value to get random shape
    tetromino_ = new_tetromino_->create_tetromino(random);
    tetrominos_.push_back(new_tetromino_);
}


/* Checks if moving single tetromino one step down is possible.
 * Returns true if possible, false otherwise.
 */
bool MainWindow::can_move_down()
{
    for ( auto& square : tetromino_ )
    {
        qreal current_y = square->y();
        qreal current_x = square->x();
        // If tetromino reached the bottom of the screen.
        if ( current_y == BORDER_DOWN - SQUARE_SIDE )
        {
            return false;
        }
        qreal next_y = current_y + STEP; // where tetromino is about to move
        if ( screen_layout_.at({current_x, next_y}) == true )
        {
            return false;
        }
    }
    return true;
}


/* After tetromino can not move anymore, this method sets the has_item
 * value to true of those coordinates where tetromino stopped. Also checks
 * if game is over.
 */
void MainWindow::store_item_info()
{
    for ( auto& square : tetromino_ )
    {
        qreal current_y = square->y();
        qreal current_x = square->x();
        if ( current_y == BORDER_UP )
        {
            game_over_ = true;
        }
        screen_layout_.at({current_x, current_y}) = true;
    }
}


/* Initializes screen layout by storing every coordinate of the game screen
 * and truth value has_item to the struct ScreenSquare.
 * These structs are stored to the vector screen_layout_.
 */
void MainWindow::initialize_screen_layout()
{
    qreal x_coordinate = 0;
    qreal y_coordinate = 0;
    for ( int i = 1; i <= COLUMNS * ROWS; ++i )
    {
        screen_layout_.insert({{x_coordinate, y_coordinate}, false});
        x_coordinate += STEP;
        if ( i % 12 == 0 )
        {
            y_coordinate += STEP;
            x_coordinate = 0;
        }
    }
}


/* Checks if moving tetromino to the right is possible.
 * Returns true if moving possible, false otherwise.
 */
bool MainWindow::can_move_right()
{
    for ( auto& square : tetromino_ )
    {
        qreal current_y = square->y();
        qreal current_x = square->x();
        if ( current_x == BORDER_RIGHT - SQUARE_SIDE )
        {
            return false;
        }
        qreal next_x = current_x + STEP; // where tetromino is about to move
        if ( screen_layout_.at({next_x, current_y} ) == true )
        {
            return false;
        }
    }
    return true;

}


/* Checks if moving tetromino to the left is possible.
 * Returns true if moving possible, false otherwise.
 */
bool MainWindow::can_move_left()
{
    for ( auto& square : tetromino_ )
    {
       qreal current_y = square->y();
       qreal current_x = square->x();
       if ( current_x == BORDER_LEFT )
       {
           return false;
       }
       qreal next_x = current_x - STEP; // where tetromino is about to move
       if ( screen_layout_.at({next_x, current_y}) == true )
       {
           return false;
       }
    }
    return true;
}

bool MainWindow::can_rotate_clockwise()
{
    std::map<int, QGraphicsRectItem*> bricks = new_tetromino_->get_tetromino_info();

    qreal center_x = bricks.at(CENTER_BLOCK)->x();
    qreal center_y = bricks.at(CENTER_BLOCK)->y();

    for ( auto block : bricks )
    {
        qreal old_x = block.second->x();
        qreal old_y = block.second->y();
        qreal new_x = old_y + center_x - center_y;
        qreal new_y = -old_x + center_x + center_y;

        if ( new_x > BORDER_RIGHT - STEP || new_x < BORDER_LEFT )
        {
            return false;
        }
        else if ( new_y > BORDER_DOWN - STEP )
        {
            return false;
        }
    }
    return true;
}

bool MainWindow::can_rotate_anticlockwise()
{
    std::map<int, QGraphicsRectItem*> bricks = new_tetromino_->get_tetromino_info();

    qreal center_x = bricks.at(CENTER_BLOCK)->x();
    qreal center_y = bricks.at(CENTER_BLOCK)->y();

    for ( auto block : bricks )
    {
        qreal old_x = block.second->x();
        qreal old_y = block.second->y();
        qreal new_x = -old_y + center_x + center_y;
        qreal new_y = old_x - center_x + center_y;

        if ( new_x > BORDER_RIGHT - STEP || new_x < BORDER_LEFT )
        {
            return false;
        }
        else if ( new_y > BORDER_DOWN - STEP )
        {
            return false;
        }
    }
    return true;
}

void MainWindow::check_for_full_row()
{

}


/* This method calculates correct numbers to display gaming time
 * on the screen.
 */
void MainWindow::time_passed()
{
    ++seconds_;

    if ( seconds_ >= 60 )
    {
        ++minutes_;
        seconds_ = 0;
    }
    ui_->lcd_number_seconds->display(seconds_);
    ui_->lcd_number_minutes->display(minutes_);
}
