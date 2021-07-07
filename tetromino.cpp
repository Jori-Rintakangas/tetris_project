#include "tetromino.hh"

Tetromino::Tetromino(QGraphicsScene* scene)
{
    scene_ = scene;
}

std::vector<QGraphicsRectItem *> Tetromino::create_tetromino(int random)
{
    // Colours for different tetromino shapes.
    QBrush redBrush(Qt::red);
    QBrush blueBrush(Qt::blue);
    QBrush yellowBrush(Qt::yellow);
    QBrush greenBrush(Qt::green);
    QPen blackPen(Qt::black);
    blackPen.setWidth(2);

    int line = 80; // value for placing tetromino on the screen
    if ( random == 0 )
    {   // creating horizontal tetromino
        for ( int i = 0; i < 4; ++i )
        {
            square_ = scene_->addRect(0, 0, SIZE, SIZE, blackPen, redBrush);
            square_->moveBy(line, 0);
            tetromino_blocks_.push_back(square_);
            tetromino_contents_.insert({i, square_});
            line += 20;
        }
    }
    if ( random == 1 )
    {   // creating right corner tetromino
        for ( int i = 0; i < 3; ++i )
        {
            square_ = scene_->addRect(0, 0, SIZE, SIZE, blackPen, blueBrush);
            square_->moveBy(line, 0);
            tetromino_blocks_.push_back(square_);
            tetromino_contents_.insert({i, square_});
            line += 20;
        }
        square_ = scene_->addRect(0, 0, SIZE, SIZE, blackPen, blueBrush);
        square_->moveBy(line - SIZE, -SIZE);
        tetromino_blocks_.push_back(square_);
        tetromino_contents_.insert({3, square_});
    }
    if ( random == 2 )
    {   // creating left corner tetromino
        for ( int i = 0; i < 3; ++i )
        {
            square_ = scene_->addRect(0, 0, SIZE, SIZE, blackPen, yellowBrush);
            square_->moveBy(line, 0);
            tetromino_blocks_.push_back(square_);
            tetromino_contents_.insert({i, square_});
            line += 20;
        }
        square_ = scene_->addRect(0, 0, SIZE, SIZE, blackPen, yellowBrush);
        square_->moveBy((line - 3 * SIZE), -SIZE);
        tetromino_blocks_.push_back(square_);
        tetromino_contents_.insert({3, square_});
    }
    if ( random == 3 )
    {   // creating square tetromino
        int line = 100; // value for placing tetromino on the screen
        for ( int i = 0; i < 2; ++i )
        {
            square_ = scene_->addRect(0, 0, SIZE, SIZE, blackPen, greenBrush);
            square_->moveBy(line, 0);
            tetromino_blocks_.push_back(square_);
            tetromino_contents_.insert({i, square_});
            line += 20;
        }
        square_ = scene_->addRect(0, 0, SIZE, SIZE, blackPen, greenBrush);
        square_->moveBy(line - SIZE, -SIZE);
        tetromino_blocks_.push_back(square_);
        tetromino_contents_.insert({2, square_});

        square_ = scene_->addRect(0, 0, SIZE, SIZE, blackPen, greenBrush);
        square_->moveBy(line - 2 * SIZE, -SIZE);
        tetromino_blocks_.push_back(square_);
        tetromino_contents_.insert({3, square_});
    }
    return tetromino_blocks_;
}

void Tetromino::rotate_clockwise()
{
    qreal center_x = tetromino_contents_.at(CENTER_BLOCK)->x();
    qreal center_y = tetromino_contents_.at(CENTER_BLOCK)->y();

    for ( auto block : tetromino_contents_ )
    {
        qreal old_x = block.second->x();
        qreal old_y = block.second->y();
        qreal new_x = old_y + center_x - center_y;
        qreal new_y = -old_x + center_x + center_y;
        block.second->moveBy(new_x - old_x, new_y - old_y);
    }
}

void Tetromino::rotate_anticlockwise()
{
    qreal center_x = tetromino_contents_.at(CENTER_BLOCK)->x();
    qreal center_y = tetromino_contents_.at(CENTER_BLOCK)->y();

    for ( auto block : tetromino_contents_ )
    {
        qreal old_x = block.second->x();
        qreal old_y = block.second->y();
        qreal new_x = -old_y + center_x + center_y;
        qreal new_y = old_x - center_x + center_y;
        block.second->moveBy(new_x - old_x, new_y - old_y);
    }
}

