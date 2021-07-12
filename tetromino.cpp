#include "tetromino.hh"

Tetromino::Tetromino(QGraphicsScene* scene)
{
    scene_ = scene;
}

std::vector<QGraphicsRectItem*> Tetromino::create_tetromino(int random)
{
    QBrush redBrush(Qt::red);
    QBrush blueBrush(Qt::blue);
    QBrush yellowBrush(Qt::yellow);
    QBrush greenBrush(Qt::green);
    QPen blackPen(Qt::black);
    blackPen.setWidth(2);
    tetromino_type_ = random;
    int line = 80; // value for placing tetromino on the screen
    if ( tetromino_type_ == HORIZONTAL )
    {
        center_brick_ = 2;
        for ( int i = 0; i < 4; ++i )
        {
            square_ = scene_->addRect(0, 0, SIZE, SIZE, blackPen, redBrush);
            square_->moveBy(line, 0);
            tetromino_bricks_.push_back(square_);
            line += 20;
        }
    }
    if ( tetromino_type_ == RIGHT_CORNER )
    {
        center_brick_ = 1;
        for ( int i = 0; i < 3; ++i )
        {
            square_ = scene_->addRect(0, 0, SIZE, SIZE, blackPen, blueBrush);
            square_->moveBy(line, 0);
            tetromino_bricks_.push_back(square_);
            line += 20;
        }
        square_ = scene_->addRect(0, 0, SIZE, SIZE, blackPen, blueBrush);
        square_->moveBy(line - SIZE, -SIZE);
        tetromino_bricks_.push_back(square_);
    }
    if ( tetromino_type_ == LEFT_CORNER )
    {
        center_brick_ = 1;
        for ( int i = 0; i < 3; ++i )
        {
            square_ = scene_->addRect(0, 0, SIZE, SIZE, blackPen, yellowBrush);
            square_->moveBy(line, 0);
            tetromino_bricks_.push_back(square_);
            line += 20;
        }
        square_ = scene_->addRect(0, 0, SIZE, SIZE, blackPen, yellowBrush);
        square_->moveBy((line - 3 * SIZE), -SIZE);
        tetromino_bricks_.push_back(square_);
    }
    if ( tetromino_type_ == SQUARE )
    {
        int line = 100; // value for placing tetromino on the screen
        for ( int i = 0; i < 2; ++i )
        {
            square_ = scene_->addRect(0, 0, SIZE, SIZE, blackPen, greenBrush);
            square_->moveBy(line, 0);
            tetromino_bricks_.push_back(square_);
            line += 20;
        }
        square_ = scene_->addRect(0, 0, SIZE, SIZE, blackPen, greenBrush);
        square_->moveBy(line - SIZE, -SIZE);
        tetromino_bricks_.push_back(square_);

        square_ = scene_->addRect(0, 0, SIZE, SIZE, blackPen, greenBrush);
        square_->moveBy(line - 2 * SIZE, -SIZE);
        tetromino_bricks_.push_back(square_);
    }
    return tetromino_bricks_;
}

void Tetromino::rotate_clockwise()
{
    std::vector<qreal> new_coords = {};

    qreal center_x = tetromino_bricks_.at(center_brick_)->x();
    qreal center_y = tetromino_bricks_.at(center_brick_)->y();

    for ( auto &brick : tetromino_bricks_ )
    {
        qreal old_x = brick->x();
        qreal old_y = brick->y();
        qreal new_x = old_y + center_x - center_y;
        qreal new_y = -old_x + center_x + center_y;
        brick->moveBy(new_x - old_x, new_y - old_y);
    }
}

void Tetromino::rotate_anticlockwise()
{
    qreal center_x = tetromino_bricks_.at(center_brick_)->x();
    qreal center_y = tetromino_bricks_.at(center_brick_)->y();

    for ( auto &brick : tetromino_bricks_ )
    {
        qreal old_x = brick->x();
        qreal old_y = brick->y();
        qreal new_x = -old_y + center_x + center_y;
        qreal new_y = old_x - center_x + center_y;
        brick->moveBy(new_x - old_x, new_y - old_y);
    }
}

std::pair<qreal, qreal> Tetromino::calculate_new_coords(bool direction)
{

}

std::vector<QGraphicsRectItem *> Tetromino::get_tetromino_info()
{
    return tetromino_bricks_;
}

int Tetromino::get_center_brick()
{
    return center_brick_;
}

