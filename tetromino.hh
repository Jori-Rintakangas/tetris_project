#ifndef TETROMINO_HH
#define TETROMINO_HH

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QDebug>

const int SIZE = 20;
const int HORIZONTAL = 0;
const int RIGHT_CORNER = 1;
const int LEFT_CORNER = 2;
const int SQUARE = 3;

class Tetromino
{
public:
    Tetromino(QGraphicsScene* scene);

    std::vector<QGraphicsRectItem*> create_tetromino(int random);

    void rotate_clockwise();

    void rotate_anticlockwise();

    std::pair<qreal, qreal> calculate_new_coords(bool direction);

    std::vector<QGraphicsRectItem*> get_tetromino_info();

    int get_center_brick();

private:

    int center_brick_; // a brick around which tetromino will rotate
    int tetromino_type_;

    QGraphicsScene* scene_;
    QGraphicsRectItem* square_;

    std::vector<QGraphicsRectItem*> tetromino_bricks_;
};

#endif // TETROMINO_HH
