/* Tetris game
 *
 * Program author
 * Name: Jori Rintakangas
 * Student number: 291679
 * UserID: ksjori
 * E-Mail: jori.rintakangas@tuni.fi
*/

#include "mainwindow.hh"
#include <QApplication>


/* Main function of the program
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
