#ifndef TETROMINO_HH
#define TETROMINO_HH

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QDebug>

const int SIZE = 20;
const int CENTER_BLOCK = 2;

class Tetromino
{
public:
    Tetromino(QGraphicsScene* scene);

    std::vector<QGraphicsRectItem*> create_tetromino(int random);

    void rotate_clockwise();

    void rotate_anticlockwise();

    std::pair<qreal, qreal> calculate_new_coords(bool direction);

    std::map<int, QGraphicsRectItem*> get_tetromino_info();

private:

    QGraphicsScene* scene_;
    QGraphicsRectItem* square_;

    std::vector<QGraphicsRectItem*> tetromino_blocks_;
    std::map<int, QGraphicsRectItem*> tetromino_contents_;
};

#endif // TETROMINO_HH
