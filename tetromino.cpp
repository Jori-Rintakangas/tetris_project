/* Class: Tetromino
 * Program author:
 * Jori Rintakangas
 */

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
    int offset = 80;
    if ( tetromino_type_ == HORIZONTAL )
    {
        center_brick_ = 2;
        for ( int i = 0; i < 4; ++i )
        {
            square_ = scene_->addRect(0, 0, SIZE, SIZE, blackPen, redBrush);
            square_->moveBy(offset, 0);
            tetromino_bricks_.push_back(square_);
            offset += 20;
        }
    }
    if ( tetromino_type_ == RIGHT_CORNER )
    {
        center_brick_ = 1;
        for ( int i = 0; i < 3; ++i )
        {
            square_ = scene_->addRect(0, 0, SIZE, SIZE, blackPen, blueBrush);
            square_->moveBy(offset, 0);
            tetromino_bricks_.push_back(square_);
            offset += 20;
        }
        square_ = scene_->addRect(0, 0, SIZE, SIZE, blackPen, blueBrush);
        square_->moveBy(offset - SIZE, -SIZE);
        tetromino_bricks_.push_back(square_);
    }
    if ( tetromino_type_ == LEFT_CORNER )
    {
        center_brick_ = 1;
        for ( int i = 0; i < 3; ++i )
        {
            square_ = scene_->addRect(0, 0, SIZE, SIZE, blackPen, yellowBrush);
            square_->moveBy(offset, 0);
            tetromino_bricks_.push_back(square_);
            offset += 20;
        }
        square_ = scene_->addRect(0, 0, SIZE, SIZE, blackPen, yellowBrush);
        square_->moveBy((offset - 3 * SIZE), -SIZE);
        tetromino_bricks_.push_back(square_);
    }
    if ( tetromino_type_ == SQUARE )
    {
        int offset = 100;
        for ( int i = 0; i < 2; ++i )
        {
            square_ = scene_->addRect(0, 0, SIZE, SIZE, blackPen, greenBrush);
            square_->moveBy(offset, 0);
            tetromino_bricks_.push_back(square_);
            offset += 20;
        }
        square_ = scene_->addRect(0, 0, SIZE, SIZE, blackPen, greenBrush);
        square_->moveBy(offset - SIZE, -SIZE);
        tetromino_bricks_.push_back(square_);

        square_ = scene_->addRect(0, 0, SIZE, SIZE, blackPen, greenBrush);
        square_->moveBy(offset - 2 * SIZE, -SIZE);
        tetromino_bricks_.push_back(square_);
    }
    return tetromino_bricks_;
}

void Tetromino::rotate_tetromino(std::vector<std::pair<qreal, qreal>> new_coords)
{
    int i = 0;
    for ( auto &brick : tetromino_bricks_ )
    {
        qreal old_x = brick->x();
        qreal old_y = brick->y();
        brick->moveBy(new_coords.at(i).first - old_x, new_coords.at(i).second - old_y);
        i++;
    }
}

void Tetromino::move_horizontal(bool dir_right)
{
    if ( dir_right )
    {
        for ( auto &brick : tetromino_bricks_ )
        {
            brick->moveBy(STEP, 0);
        }
    }
    else
    {
        for ( auto &brick : tetromino_bricks_ )
        {
            brick->moveBy(-STEP, 0);
        }
    }
}

void Tetromino::erase_brick(std::vector<std::pair<qreal, qreal>> bricks)
{
    for ( unsigned int i = 0; i < bricks.size(); ++i )
    {
        for ( auto it = tetromino_bricks_.begin();
                   it != tetromino_bricks_.end(); ++it )
        {
            if ( (*it)->x() == bricks.at(i).first && (*it)->y() == bricks.at(i).second )
            {
                delete *it;
                tetromino_bricks_.erase(it);
                it--;
            }
        }
    }
}

std::vector<QGraphicsRectItem *> Tetromino::get_tetromino_info()
{
    return tetromino_bricks_;
}

int Tetromino::get_center_brick()
{
    return center_brick_;
}

int Tetromino::get_tetromino_type()
{
    return tetromino_type_;
}

bool Tetromino::is_destroyed()
{
    return tetromino_bricks_.empty();
}

