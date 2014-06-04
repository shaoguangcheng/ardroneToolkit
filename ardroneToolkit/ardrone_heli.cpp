#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#include <QApplication>
#include <QtGui>

#include <cv.h>
#include <highgui.h>

#include "ardrone_FlightTask.h"
#include "ardrone_CHeli.h"

#include "GUI_MainWindow.h"
#include "DEBUG_utils.h"

#include "ardrone_config.h"

//*************************************************************************
//thread variable
static pthread_t control_thread = 0;
static int control_live = 1;

QApplication        *g_app=NULL;
MainWindow          *g_win=NULL;

SHeliData           *g_navData;
radiogp_cmd_t       *g_cmdData;

FlightTask          *g_task;
CHeli               *g_heli;
CRawImage           *g_image;
unsigned char       *g_imgBuff;
unsigned int         g_imgBuffSize;

double marker_size = 0.08;

//****************************************************************************

static void* control_mainloop(void *arg)
{
    Mat             image_temp;

    image_temp.create(ARDRONE_VIDEO_HEIGHT, ARDRONE_VIDEO_WIDTH, CV_8UC3);

    while( control_live ) {
        //image capture
        g_heli->renewImage(g_image);

        g_image->to_mat(image_temp);

        //performing flight task
        g_task->videoControl(&image_temp);

        g_image->from_mat(&image_temp);

        //update GUI video buffer
        memcpy(g_imgBuff, g_image->data, g_imgBuffSize);

        usleep(20000);

        if( g_task->is_stopped() ) {
            g_win->close();
            break;
        }
    }
}

int main(int argc, char* argv[])
{
    // start stack trace
    dbg_stacktrace_setup();

    //set global nav & control data
    g_navData = &helidata;
    g_cmdData = &radiogp_cmd;

    ///////////////////////////////////////////////////////////////////////////
    //initializing stuff
    ///////////////////////////////////////////////////////////////////////////
    g_heli = new CHeli();
    g_image = new CRawImage(ARDRONE_VIDEO_WIDTH, ARDRONE_VIDEO_HEIGHT);
    g_image->getSaveNumber();

    ///////////////////////////////////////////////////////////////////////////
    // UI setting
    ///////////////////////////////////////////////////////////////////////////
    QApplication app(argc, argv);
    g_app = &app;

    // regist Text classes
    qRegisterMetaType<QTextBlock>("QTextBlock");
    qRegisterMetaType<QTextCursor>("QTextCursor");

    g_imgBuffSize = ARDRONE_VIDEO_WIDTH*ARDRONE_VIDEO_HEIGHT*3;
    g_imgBuff = new unsigned char[g_imgBuffSize];
    MainWindow  canvas(NULL, g_imgBuff);
    g_win = &canvas;

    int w, h;
    w = ARDRONE_VIDEO_WIDTH + CONTROL_PANEL_WIDTH + 2;
    h = ARDRONE_VIDEO_HEIGHT + LOG_PANEL_HEIGHT + 2;
    canvas.setWindowTitle("Ardrone Control");
    canvas.setGeometry(0, 0, w, h);
    canvas.setFixedSize(w, h);

    //canvas.showFullScreen();
    canvas.show();

    ///////////////////////////////////////////////////////////////////////////
    // create new task and set flight-task parameters
    ///////////////////////////////////////////////////////////////////////////
    //g_task = new FlightTask_VanishPoint();
    g_task = new FlightTask_ColorTrack();

    g_task->set_heli(g_heli);
    g_task->set_rawimage(g_image);
    g_task->set_gui(g_win);

//    g_task->setMarkersize(marker_size);
//    g_task->setCameraParameters("data/intrinsics.yml");


    ///////////////////////////////////////////////////////////////////////////
    // create control loop
    ///////////////////////////////////////////////////////////////////////////
    control_live = 1;
    if ( pthread_create( &control_thread, NULL, control_mainloop, NULL ) ) {
        printf("pthread_create: %s\n", strerror(errno));
        return -1;
    }

    ///////////////////////////////////////////////////////////////////////////
    // begin Qt GUI loop
    ///////////////////////////////////////////////////////////////////////////
    g_app->exec();

    ///////////////////////////////////////////////////////////////////////////
    // terminate control loop
    ///////////////////////////////////////////////////////////////////////////
    control_live = 0;
    pthread_join(control_thread, NULL);

    delete g_heli;
    delete g_image;
    delete g_task;
    delete g_imgBuff;

    return 0;
}

