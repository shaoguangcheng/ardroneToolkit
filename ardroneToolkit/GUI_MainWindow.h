#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QMutex>

#include <vector>
#include <deque>

#include "GUI_InputEvent.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent, unsigned char *video_buffer);
    ~MainWindow();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    int  getEvent(InputEvent &event);
    void print(char *buf);

protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void timerEvent(QTimerEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    bool                    antialiased;

    QImage                  *m_imgVideo;
    QPixmap                 *m_imgCpl;

    QPlainTextEdit          *m_txtEdit;
    QMutex                  *m_mutexPrint;
    std::vector<QString>    m_arrPrintMsg;

    std::deque<InputEvent>  m_eventQueue;
    QMutex                  *m_mutexEventQueue;
};


#endif // __MAINWINDOW_H__
