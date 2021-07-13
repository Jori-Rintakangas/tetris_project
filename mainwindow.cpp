/* Class: MainWindow
 * Program author:
 * Jori Rintakangas
 */

#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsRectItem>



MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);

    scene_ = new QGraphicsScene(this);

    int left_margin = 100;
    int top_margin = 150;

    ui_->graphicsView->setGeometry(left_margin, top_margin,
                                  BORDER_RIGHT + 2, BORDER_DOWN + 2);
    ui_->graphicsView->setScene(scene_);

    scene_->setSceneRect(0, 0, BORDER_RIGHT - 1, BORDER_DOWN - 1);

    ui_->textBrowser->setTextColor(Qt::black);
    ui_->textBrowser->setStyleSheet("background-color: red");

    int seed = time(0);
    randomEng.seed(seed);
    distr = std::uniform_int_distribution<int>(0, 3);
    distr(randomEng); // Wiping out the first random number

    ui_->lcd_number_minutes->setStyleSheet("background-color: red");
    ui_->lcd_number_seconds->setStyleSheet("background-color: red");

    timer_.setSingleShot(false);
    game_timer_.setSingleShot(false);
    connect(&timer_, &QTimer::timeout, this, &MainWindow::move_tetrominos);
    connect(&game_timer_, &QTimer::timeout, this, &MainWindow::time_passed);
}


MainWindow::~MainWindow()
{
    delete ui_;
}


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
        if ( new_tetromino_->get_tetromino_type() != SQUARE )
        {
            if ( can_rotate(true) )
            {
                new_tetromino_->rotate_tetromino(new_coords_);
            }
        }
    }
    if ( event->key() == Qt::Key_O )
    {
        if ( new_tetromino_->get_tetromino_type() != SQUARE )
        {
            if ( can_rotate(false) )
            {
                new_tetromino_->rotate_tetromino(new_coords_);
            }
        }
    }
}


void MainWindow::on_start_push_button_clicked()
{
    ui_->start_push_button->setDisabled(true);
    ui_->textBrowser->setText("Game on!");
    initialize_screen_layout();
    timer_.start(500);
    game_timer_.start(1000);
    new_tetromino();
}


void MainWindow::on_reset_push_button_clicked()
{
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


void MainWindow::new_tetromino()
{
    new_tetromino_ = new Tetromino(scene_);
    int random = distr(randomEng);
    tetromino_ = new_tetromino_->create_tetromino(random);
    tetrominos_.push_back(new_tetromino_);
}


bool MainWindow::can_move_down()
{
    for ( auto& square : tetromino_ )
    {
        qreal current_y = square->y();
        qreal current_x = square->x();

        if ( current_y == BORDER_DOWN - SQUARE_SIDE )
        {
            return false;
        }
        qreal next_y = current_y + STEP;
        if ( screen_layout_.at({current_x, next_y}) == true )
        {
            return false;
        }
    }
    return true;
}


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


void MainWindow::initialize_screen_layout()
{
    qreal x_coordinate = 0;
    qreal y_coordinate = -STEP;
    for ( int i = 0; i <= 11; ++i )
    {
        screen_layout_.insert({{x_coordinate, y_coordinate}, false});
        x_coordinate += STEP;
    }
    x_coordinate = 0;
    y_coordinate = 0;
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
        qreal next_x = current_x + STEP;
        if ( screen_layout_.at({next_x, current_y} ) == true )
        {
            return false;
        }
    }
    return true;

}


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
       qreal next_x = current_x - STEP;
       if ( screen_layout_.at({next_x, current_y}) == true )
       {
           return false;
       }
    }
    return true;
}

bool MainWindow::can_rotate(bool clockwise)
{
    std::vector<QGraphicsRectItem*> bricks = new_tetromino_->get_tetromino_info();
    qreal center_x = bricks.at(new_tetromino_->get_center_brick())->x();
    qreal center_y = bricks.at(new_tetromino_->get_center_brick())->y();
    new_coords_.clear();
    for ( auto &brick : bricks )
    {
        qreal old_x = brick->x();
        qreal old_y = brick->y();
        qreal new_x = 0;
        qreal new_y = 0;
        if ( clockwise )
        {
            new_x = old_y + center_x - center_y;
            new_y = -old_x + center_x + center_y;
        }
        else
        {
            new_x = -old_y + center_x + center_y;
            new_y = old_x - center_x + center_y;
        }
        if ( new_x > BORDER_RIGHT - STEP || new_x < BORDER_LEFT )
        {
            return false;
        }
        else if ( new_y > BORDER_DOWN - STEP || new_y < BORDER_UP )
        {
            return false;
        }
        else if ( screen_layout_.at({new_x, new_y}) == true )
        {
            return false;
        }
        new_coords_.push_back({new_x, new_y});
    }
    return true;
}


void MainWindow::check_for_full_row()
{

}


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
