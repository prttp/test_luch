#include "mypicture.h"

MyPicture::MyPicture(QWidget *parent) : QWidget(parent)
{
    this->setMouseTracking(true);
}
void  MyPicture::paintEvent(QPaintEvent * event) {
    QPainter paint(this);
    QPoint point(0, 0);
    pixmap.convertFromImage(pic);
    paint.drawPixmap(point, pixmap);//отрисовка изображения
    paint.setPen(Qt::red);
    for (int i = 0; i < RList.size(); ++i) { //отрисовка всех полигонов
        paint.drawRect(RList[i]);
    }

    if (isDrawing == 1) {
        paint.drawRect(QRect(topleft,botright)); //отрисовка изображаемого полигона
    }

}
void  MyPicture::uploadPicture(QString s) {
    if (pic.load(s)) {permitToDraw = true;};
    QScreen* screen = QApplication::screens().at(0);
    QSize d = screen->availableSize();
    if ((pic.height()>d.height())||(pic.width()>d.width())) {
            pic=pic.scaled(d, Qt::KeepAspectRatio); //если размер изображения больше размера экрана => подгон изображения под размер экрана
        }
    this->setFixedSize(pic.size()); //размер виджета устанавливается равным размеру изображения
    this->repaint();
}
void MyPicture::mousePressEvent(QMouseEvent *event)
{
    if (permitToDraw) {
    if ((event->button() == Qt::LeftButton)&&(this->findResizingRectByCurPos()!=nullptr)) { //возможность изменить полигон перетягиванием за вершину  1 по приоритету
        isResizing = 1;
        currentRect=this->findResizingRectByCurPos();
    } else if ((event->button() == Qt::LeftButton)&&(this->findRectByCurPos()!=nullptr)) { //возможность перетащить полигон  2 по приоритету
        isDragging =1;
        QCursor c = this->cursor();
        c.setShape(Qt::ClosedHandCursor);
        this->setCursor(c);
        currentRect = this->findRectByCurPos();

    } else if (event->button() == Qt::LeftButton) { ////возможность рисовать полигон 3 по приоритету
        isDrawing = 1;
        QPoint gtopleft = QCursor::pos();
        topleft = mapFromGlobal(gtopleft);
    };
    }
}
void MyPicture::mouseReleaseEvent(QMouseEvent *event) {
    if (isResizing == 1) {
        isResizing = 0;
     }
    if (isDragging == 1) {
        isDragging = 0;
        QCursor c = this->cursor();
        c.setShape(Qt::OpenHandCursor);
        this->setCursor(c);

     }

    if (isDrawing == 1) {
        isDrawing = 0;
        if (!(this->isCursorClose(topleft))) {RList.append(QRect(topleft,botright)); } //защита от добавленя полигонов при клике
     }
}
void MyPicture::mouseMoveEvent(QMouseEvent *event) {
    if (isResizing ==1) { //изменение размера полигона
        this->repaint();
        if (this->resizingPoint==RectPoint::topLeft) {
            currentRect->setTopLeft(mapFromGlobal(QCursor::pos()));
        } else if (this->resizingPoint==RectPoint::topRight) {
            currentRect->setTopRight(mapFromGlobal(QCursor::pos()));
        } else if (this->resizingPoint==RectPoint::bottomLeft) {
            currentRect->setBottomLeft(mapFromGlobal(QCursor::pos()));
        } else if (this->resizingPoint==RectPoint::bottomRight) {
            currentRect->setBottomRight(mapFromGlobal(QCursor::pos()));
        }

    } else if (isDragging == 1) { //перетаскивание полигона
       this->repaint();
       currentRect->moveCenter(mapFromGlobal(QCursor::pos()));
    } else if (isDrawing == 1) { //отрисовка изображаемого полигона
        this->repaint();
        QPoint gbotright = QCursor::pos();
        botright = mapFromGlobal(gbotright);
    } else if (findResizingRectByCurPos()!=nullptr) { //изменения курсора при возмложности изменения размера полигона
        if ((this->resizingPoint==RectPoint::topLeft)||(this->resizingPoint==RectPoint::bottomRight)) {
            QCursor c = this->cursor();
            c.setShape(Qt::SizeFDiagCursor);
            this->setCursor(c);
        } else  if ((this->resizingPoint==RectPoint::topRight)||(this->resizingPoint==RectPoint::bottomLeft)) {
            QCursor c = this->cursor();
            c.setShape(Qt::SizeBDiagCursor);
            this->setCursor(c);
        }
    } else if (findRectByCurPos()!=nullptr) {
        QCursor c = this->cursor();
        c.setShape(Qt::OpenHandCursor);
        this->setCursor(c);
    } else {
        QCursor c = this->cursor();
        c.setShape(Qt::ArrowCursor);
        this->setCursor(c);
    }

}
QRect* MyPicture::findRectByCurPos() {
    for (auto& i : RList) {
        if (i.contains(mapFromGlobal(QCursor::pos()))) {return &i;};
    }
    return nullptr;
}
QRect* MyPicture::findResizingRectByCurPos() {
    for (auto& i : RList) {
        if (this->isCursorClose(i.bottomLeft())) {this->resizingPoint = RectPoint::bottomLeft; return &i;};
        if (this->isCursorClose(i.bottomRight())) {this->resizingPoint = RectPoint::bottomRight; return &i;};
        if (this->isCursorClose(i.topLeft())) {this->resizingPoint = RectPoint::topLeft; return &i;};
        if (this->isCursorClose(i.topRight())) {this->resizingPoint = RectPoint::topRight; return &i;};
        }
    return nullptr;
}
bool MyPicture::isCursorClose(const QPoint& p) {
     if ((p.x()>(mapFromGlobal(QCursor::pos()).x()-3))&&(p.x()<(mapFromGlobal(QCursor::pos()).x()+5))&&(p.y()>(mapFromGlobal(QCursor::pos()).y()-3))&&(p.y()<(mapFromGlobal(QCursor::pos()).y()+5))){
         return true;
     } else {return false;}
}
