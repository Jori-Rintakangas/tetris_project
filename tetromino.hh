#ifndef TETROMINO_HH
#define TETROMINO_HH

#include <QGraphicsScene>
#include <QGraphicsItem>

const int SIZE = 20;

class Tetromino
{
public:
    Tetromino(QGraphicsScene* scene);

    std::vector<QGraphicsRectItem*> create_tetromino(int random);

private:

    QGraphicsScene* scene_;
    QGraphicsRectItem* square_;
    std::vector<QGraphicsRectItem*> tetromino_blocks_;

};

#endif // TETROMINO_HH
