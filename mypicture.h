#ifndef MYPICTURE_H
#define MYPICTURE_H

#include <QWidget>
#include <QFileDialog>
#include <QPainter>
#include <QMouseEvent>
#include <QCursor>
#include <QApplication>
#include <QScreen>
enum class RectPoint { topLeft, topRight, bottomLeft, bottomRight };
class MyPicture : public QWidget
{
    Q_OBJECT
    QImage pic; //выбранное изображение
    QPixmap pixmap; //pixmap с выбранным изображением
    bool isDrawing=0; //флаг рисования
    QPoint topleft; //верхняя левая точка изображаемого полигона
    QPoint botright; //правая нижняя точка изображаемого полигона
    QVector<QRect> RList; //контейнер с полигонами
    bool isDragging=0; // флаг перетаскивания
    QRect* findRectByCurPos(); //функция, возвращающая указатель на полигон под курсором
    QRect* currentRect; //указатель на изменяемый полигон
    QRect* findResizingRectByCurPos(); //функция, возвращающая указатель на полигон под курсором, вершину которого возможно перетащить
    bool isResizing=0; //флаг изменения размера полигона
    RectPoint resizingPoint; //вершина полигона, подлежащая перетаскиванию
    bool permitToDraw =0; //флаг разрешения рисования
public:
    explicit MyPicture(QWidget *parent = nullptr);
    void  paintEvent(QPaintEvent * event);
    bool isCursorClose(const QPoint& p); //возвращает true если курсор близко (+/- 3 пикселя) к точке

signals:
public slots:
    void uploadPicture(QString s); // загрузка изображения
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

};

#endif // MYPICTURE_H
