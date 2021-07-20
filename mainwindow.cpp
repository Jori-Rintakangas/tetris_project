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
    scene_->setSceneRect(0, 0, BORDER_RIGHT - 1, BORDER_DOWN - 1);

    ui_->graphicsView->setGeometry(LEFT_MARGIN, TOP_MARGIN,
                                   BORDER_RIGHT + 2, BORDER_DOWN + 2);
    ui_->graphicsView->setScene(scene_);

    ui_->textBrowser->setTextColor(Qt::black);
    ui_->textBrowser->setStyleSheet("background-color: red");

    ui_->lcd_number_minutes->setStyleSheet("background-color: red");
    ui_->lcd_number_seconds->setStyleSheet("background-color: red");

    int seed = time(0);
    random_eng_.seed(seed);
    distr_ = std::uniform_int_distribution<int>(0, 3);
    distr_(random_eng_); // Wiping out the first random number

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
    if ( event->key() == Qt::Key_D )
    {
        if ( can_move_right() )
        {
            tetromino_->move_horizontal(true);
        }
    }
    if ( event->key() == Qt::Key_A )
    {
        if ( can_move_left() )
        {
            tetromino_->move_horizontal(false);
        }
    }
    if ( event->key() == Qt::Key_W )
    {
        if ( tetromino_->get_tetromino_type() != SQUARE )
        {
            if ( can_rotate() )
            {
                tetromino_->rotate_tetromino(new_coords_);
            }
        }
    }
}


void MainWindow::on_start_push_button_clicked()
{
    ui_->start_push_button->setDisabled(true);
    ui_->textBrowser->setText("Game on!");
    initialize_screen_layout();
    timer_.start(300);
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
    ui_->textBrowser->clear();
    ui_->start_push_button->setDisabled(false);
}


void MainWindow::move_tetrominos()
{
    qreal delta_x = 0;
    qreal delta_y = STEP;
    std::vector<QGraphicsRectItem*> bricks = tetromino_->get_tetromino_info();
    if ( can_move_down() )
    {
        for ( auto &brick : bricks )
        {
            brick->moveBy(delta_x, delta_y);
        }
    }
    else
    {
        store_item_info();
        check_for_full_row();
        if ( !game_over_ )
        {
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
    tetromino_ = new Tetromino(scene_);
    int random = distr_(random_eng_);
    tetromino_->create_tetromino(random);
    tetrominos_.push_back(tetromino_);
}


bool MainWindow::can_move_down()
{
    std::vector<QGraphicsRectItem*> bricks = tetromino_->get_tetromino_info();
    for ( auto &brick : bricks )
    {
        qreal current_y = brick->y();
        qreal current_x = brick->x();

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
    std::vector<QGraphicsRectItem*> bricks = tetromino_->get_tetromino_info();
    for ( auto &brick : bricks )
    {
        qreal current_y = brick->y();
        qreal current_x = brick->x();
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
    std::vector<QGraphicsRectItem*> bricks = tetromino_->get_tetromino_info();
    for ( auto &brick : bricks )
    {
        qreal current_y = brick->y();
        qreal current_x = brick->x();
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
    std::vector<QGraphicsRectItem*> bricks = tetromino_->get_tetromino_info();
    for ( auto &brick : bricks )
    {
       qreal current_y = brick->y();
       qreal current_x = brick->x();
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


bool MainWindow::can_rotate()
{
    std::vector<QGraphicsRectItem*> bricks = tetromino_->get_tetromino_info();
    qreal center_x = bricks.at(tetromino_->get_center_brick())->x();
    qreal center_y = bricks.at(tetromino_->get_center_brick())->y();
    new_coords_.clear();
    for ( auto &brick : bricks )
    {
        qreal old_x = brick->x();
        qreal old_y = brick->y();
        qreal new_x = -old_y + center_x + center_y;
        qreal new_y = old_x - center_x + center_y;

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
    for ( int y = 0; y < BORDER_DOWN; y += STEP )
    {
        auto row_begin = screen_layout_.find({BORDER_LEFT, y});
        auto row_end = screen_layout_.find({BORDER_RIGHT-SQUARE_SIDE, y});
        if ( std::all_of(row_begin, ++row_end, [](auto& p){return p.second;}))
        {
            erase_full_row(y);
        }
    }
}


void MainWindow::erase_full_row(qreal row_y_coord)
{
    for ( auto it = tetrominos_.begin(); it != tetrominos_.end(); ++it )
    {
        bool bricks_in_row = false;
        std::vector<QGraphicsRectItem*> bricks = (*it)->get_tetromino_info();
        std::vector<std::pair<qreal, qreal>> bricks_to_erase = {};

        for ( auto &brick : bricks )
        {
            if ( brick->y() == row_y_coord )
            {
                bricks_in_row = true;
                scene_->removeItem(brick);
                screen_layout_.at({brick->x(), brick->y()}) = false;
                bricks_to_erase.push_back({brick->x(), brick->y()});
            }
        }
        if ( !bricks_in_row )
        {
            continue;
        }
        (*it)->erase_brick(bricks_to_erase);
        if ( (*it)->is_destroyed() )
        {
            tetrominos_.erase(it);
            it--;
            //delete *it;
        }
    }
    update_scene(row_y_coord);
}


void MainWindow::update_scene(qreal y)
{
    for ( auto &tetromino : tetrominos_ )
    {
        std::vector<QGraphicsRectItem*> bricks = tetromino->get_tetromino_info();
        for ( auto &brick : bricks )
        {
            if ( brick->y() < y )
            {
                screen_layout_.at({brick->x(), brick->y()}) = false;
                brick->moveBy(0, STEP);
                screen_layout_.at({brick->x(), brick->y()}) = true;
            }
        }
    }
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
