/* Class: Tetromino
 * Tetromino class represents a single tetris block.
 *
 * Program author:
 * Jori Rintakangas
 */

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

    void rotate_tetromino(std::vector<std::pair<qreal,qreal>> new_coords);

    std::pair<qreal, qreal> calculate_new_coords(bool direction);

    std::vector<QGraphicsRectItem*> get_tetromino_info();

    int get_center_brick();

    int get_tetromino_type();

private:

    int center_brick_;
    int tetromino_type_;

    QGraphicsScene* scene_;
    QGraphicsRectItem* square_;

    std::vector<QGraphicsRectItem*> tetromino_bricks_;
};

#endif // TETROMINO_HH
