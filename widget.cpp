/**********************************************************************************************
 * Author: Ivanov Athur
 * Framework: Qt
 * Language: C++
***********************************************************************************************/

#include "widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(300,525);

    count_full_line = 0;       //Для подсчета полной линии, когда линия полна она удаляется и начисляются очки
    score = 0;                 //Счетчик очков
    speed = 120;               //Скорость движения фигур

    figure_S = false;
    figure_S_vertical = false;
    figure_S_horizontal = false;
    figure_line  = false;
    figure_line_vertical = false;
    figure_line_horizontal = false;  //Флаги фигур,а так же состояний игры устанавливаются в false
    figure_sqare = false;
    figure_T = false;
    figure_T_up = false;
    figure_T_down = false;
    figure_T_left = false;
    figure_T_right = false;
    figure_J = false;
    figure_J_up = false;
    figure_J_right = false;
    figure_J_left = false;
    figure_J_down = false;

    isPause = false;
    isEndGame = false;
    isGeneration = false;


    for(int i=0; i<=19; i++)
    {
        for(int j=0; j<=39; j++)
        {
            points[i][j].del = false;
            points[i][j].status = false;
            points[i][j].move = false;
            points[i][j].label = new QLabel(this);              //Визуализируем массив ячеек(структур)
            points[i][j].label->resize(15,15);                  //И выставляем значения переменных структуры в false
            points[i][j].label->setPixmap(QPixmap("://0.png"));
            points[i][j].label->move(i*15,(j-5)*15);
            points[i][j].label->hide();
        }
    }

    score_label = new QLabel("score: 0",this);
    score_label->setGeometry(2,2,300,40);
    QPalette *pal = new QPalette;
    pal->setBrush(score_label->foregroundRole(), QBrush(Qt::red));  //Визуализация надписи набранных очков
    score_label->setPalette(*pal);
    score_label->setFont(QFont("font",19,19));
    delete pal;

    for(int i=0; i<=19; i++)
    {
        for(int j=5; j<=39; j++)
        {
            points[i][j].label->show();   //Рисуем игровое поле
        }
    }


    timer_animationDelete = new QTimer;
    QObject::connect(timer_animationDelete, &QTimer::timeout,
                     [&]()
                     {
                         for(int i=0; i<=19; i++)
                         {
                             for(int j=0; j<=39; j++)
                             {
                                 if(points[i][j].del)
                                 {
                                     points[i][j].del = false;
                                     points[i][j].move = false;
                                     points[i][j].status = false;  //Таймер и анимация удаления линии
                                 }
                             }
                         }

                         moveAfterDelete();

                         animation();
                         timer_animationDelete->stop();
                         isGeneration = false;
                         onGenerate();
                     });

    timer_animation = new QTimer;
    QObject::connect(timer_animation, &QTimer::timeout,
                     [&]()
                     {
                         moving();
                         animation();
                     });
    timer_animation->start(speed);
    onGenerate();

    gameOver_label = new QLabel(this);
    gameOver_label->resize(300,525);
    gameOver_label->setPixmap(QPixmap("://gameOver.png"));   //Инициализация надписи конец игры
    gameOver_label->hide();

    pause_label = new QLabel(this);
    pause_label->resize(300,525);
    pause_label->setPixmap(QPixmap("://pause_fon.png"));     //Инициализация блюр эффекта паузы(меню)
    pause_label->hide();

    resume_label = new QLabel(this);
    resume_label->setGeometry(0,100,300,100);
    resume_label->setPixmap(QPixmap(":/pause_resume_on.png"));
    resume_label->hide();

    restart_label = new QLabel(this);
    restart_label->setGeometry(0,200,300,100);
    restart_label->setPixmap(QPixmap(":/pause_restart_off.png"));   //Инициализация надписей главного меню(паузы)
    restart_label->hide();

    exit_label = new QLabel(this);
    exit_label->setGeometry(0,300,300,100);
    exit_label->setPixmap(QPixmap(":/pause_exit_off.png"));
    exit_label->hide();

}

Widget::~Widget()
{
    delete score_label;
    delete timer_animation;
}

void Widget::onGenerate()
{
    //Функция генерации рандомных(из имеющихся) фигур
    timer_animation->stop();
    checkFullLine();
    if(!isGeneration)
    {
        isGeneration     =  true;
        unsigned  d      =  rand()%5;

        //T_figure
        if(d==0)
        {
            figure_T  =  true;
            figure_T_up      =  true;

            figure_T_down    =  false;
            figure_T_left    =  false;
            figure_T_right   =  false;
            figure_S     =  false;
            figure_line  =  false;
            figure_sqare =  false;
            figure_J     =  false;

            points[9][5].status  = true;
            points[9][5].move    = true;

            points[10][4].status = true;
            points[10][4].move   = true;

            points[10][5].status = true;
            points[10][5].move   = true;

            points[11][5].status = true;
            points[11][5].move   = true;

        }
        //sqare_figure
        if(d==1)
        {
            figure_sqare  = true;
            figure_S      = false;
            figure_T   = false;
            figure_line   = false;
            figure_J      = false;

            points[9][4].status   =  true;
            points[10][4].status  =  true;
            points[9][5].status   =  true;
            points[10][5].status  =  true;
            points[9][5].move     =  true;
            points[10][4].move    =  true;
            points[9][4].move     =  true;
            points[10][5].move    =  true;
        }
        //line_figure
        if(d==2)
        {
            figure_line  = true;
            figure_line_vertical     = true;

            figure_line_horizontal     = false;
            figure_S     = false;
            figure_T  = false;
            figure_sqare = false;
            figure_J     = false;

            points[9][2].status = true;
            points[9][3].status = true;
            points[9][4].status = true;
            points[9][2].move   = true;
            points[9][3].move   = true;
            points[9][4].move   = true;
        }
        //s_figure
        if(d==3)
        {
            figure_S       =  true;
            figure_S_horizontal          =  true;
            figure_S_vertical          =  false;
            figure_line    =  false;
            figure_sqare   =  false;
            figure_T    =  false;
            figure_J       =  false;

            points[8][5].move   = true;
            points[8][5].status = true;
            points[9][5].move   = true;
            points[9][5].status = true;
            points[9][4].move   = true;
            points[9][4].status = true;
            points[10][4].move  = true;
            points[10][4].status= true;

        }
        //J_figure
        if(d==4)
        {
            figure_J     = true;
            figure_J_up         = true;

            figure_J_down       = false;
            figure_J_left       = false;
            figure_J_right      = false;
            figure_T  = false;
            figure_S     = false;
            figure_line  = false;
            figure_sqare = false;

            points[9][4].status  = true;
            points[9][4].move    = true;

            points[10][4].status = true;
            points[10][4].move   = true;

            points[9][5].status  = true;
            points[9][5].move    = true;

            points[9][6].status  = true;
            points[9][6].move    = true;
        }
    }
}

void Widget::animation()
{
    QString z;
    z.setNum(score);
    score_label->setText("score: "+z);

    for(int i=0; i<=19; i++)
    {
        for(int j=0; j<=39; j++)
        {
            if(points[i][j].del)
            {
                points[i][j].label->setPixmap(QPixmap("://del.png"));
            }
            else if(points[i][j].status)
            {
                points[i][j].label->setPixmap(QPixmap("://1.png"));    //Анимация и окрашивание ячеек массива
            }
            else                                                        //В соответствии с их статусом
            {
                points[i][j].label->setPixmap(QPixmap("://0.png"));
            }
        }
    }
}

void Widget::moving()
{
    for(int i=0; i<=19; i++)
    {
        for(int j=0; j<=39; j++)
        {
            if(points[i][j].status && !points[i][j].move && j == 5) //Функция движения фигур
            {
                gameOver();
            }
        }
    }
    //Отрисовка фигур
    if(figure_sqare)
    {
        for(int i=0; i<=19; i++)
        {
            for(int j=0; j<=39; j++)
            {
                if(points[i][j].move && points[i][j-1].move && points[i+1][j].move && points[i+1][j-1].move)
                {
                    if(points[i][j+1].status || points[i+1][j+1].status || j>=39)
                    {
                        points[i][j].move=false;
                        points[i][j-1].move=false;
                        points[i+1][j].move=false;
                        points[i+1][j-1].move=false;
                        isGeneration = false;
                        timer_animation->setInterval(speed);
                        onGenerate();
                        return;
                    }


                    points[i][j-1].move=false;
                    points[i][j-1].status=false;
                    points[i+1][j-1].move=false;
                    points[i+1][j-1].status=false;

                    points[i][j+1].move=true;
                    points[i][j+1].status=true;
                    points[i+1][j+1].move=true;
                    points[i+1][j+1].status=true;
                    return;
                }
            }
        }
    }
    if(figure_J)
    {

        if(figure_J_up)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i+1][j].move && points[i][j+1].move && points[i][j+2].move)
                    {
                        if(j+2==39 || points[i][j+3].status || points[i+1][j+1].status)
                        {
                            points[i][j].move=false;
                            points[i+1][j].move=false;
                            points[i][j+1].move=false;
                            points[i][j+2].move=false;
                            isGeneration = false;
                            timer_animation->setInterval(speed);
                            onGenerate();
                            return;
                        }
                        points[i][j].status = false;
                        points[i][j].move = false;
                        points[i+1][j].status = false;
                        points[i+1][j].move = false;

                        points[i][j+3].status = true;
                        points[i][j+3].move = true;
                        points[i+1][j+1].status = true;
                        points[i+1][j+1].move = true;
                        return;
                    }
                }
            }
        }
        if(figure_J_down)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i+1][j].move && points[i+1][j-1].move && points[i+1][j-2].move)
                    {
                        if(j==39 || points[i][j+1].status || points[i+1][j+1].status)
                        {
                            points[i][j].move=false;
                            points[i+1][j].move=false;
                            points[i+1][j-1].move=false;
                            points[i+1][j-2].move=false;
                            isGeneration = false;
                            timer_animation->setInterval(speed);
                            onGenerate();
                            return;
                        }
                        points[i][j].status = false;
                        points[i][j].move = false;
                        points[i+1][j-2].status = false;
                        points[i+1][j-2].move = false;

                        points[i][j+1].status = true;
                        points[i][j+1].move = true;
                        points[i+1][j+1].status = true;
                        points[i+1][j+1].move = true;
                        return;
                    }
                }
            }
        }
        if(figure_J_right)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i+1][j].move && points[i+2][j+1].move && points[i+2][j].move)
                    {
                        if(j+1==39 || points[i][j+1].status || points[i+1][j+1].status || points[i+2][j+2].status)
                        {
                            points[i][j].move=false;
                            points[i+1][j].move=false;
                            points[i+2][j+1].move=false;
                            points[i+2][j].move=false;
                            isGeneration = false;
                            timer_animation->setInterval(speed);
                            onGenerate();
                            return;
                        }
                        points[i][j].status = false;
                        points[i][j].move = false;
                        points[i+1][j].status = false;
                        points[i+1][j].move = false;
                        points[i+2][j].status = false;
                        points[i+2][j].move = false;

                        points[i][j+1].status = true;
                        points[i][j+1].move = true;
                        points[i+1][j+1].status = true;
                        points[i+1][j+1].move = true;
                        points[i+2][j+2].status = true;
                        points[i+2][j+2].move = true;
                        return;
                    }
                }
            }
        }
        if(figure_J_left)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i+1][j].move && points[i+2][j].move && points[i][j-1].move)
                    {
                        if(j==39 || points[i][j+1].status || points[i+1][j+1].status || points[i+2][j+1].status)
                        {
                            points[i][j].move=false;
                            points[i+1][j].move=false;
                            points[i+2][j].move=false;
                            points[i][j-1].move=false;
                            isGeneration = false;
                            timer_animation->setInterval(speed);
                            onGenerate();
                            return;
                        }
                        points[i][j-1].status = false;
                        points[i][j-1].move = false;
                        points[i+1][j].status = false;
                        points[i+1][j].move = false;
                        points[i+2][j].status = false;
                        points[i+2][j].move = false;

                        points[i][j+1].status = true;
                        points[i][j+1].move = true;
                        points[i+1][j+1].status = true;
                        points[i+1][j+1].move = true;
                        points[i+2][j+1].status = true;
                        points[i+2][j+1].move = true;
                        return;
                    }
                }
            }
        }
    }
    if(figure_T)
    {
        if(figure_T_up)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i][j-1].move && points[i+1][j].move && points[i-1][j].move)
                    {
                        if(points[i][j+1].status || j >= 39 || points[i-1][j+1].status || points[i+1][j+1].status)
                        {
                            points[i][j].move=false;
                            points[i][j-1].move=false;
                            points[i+1][j].move=false;
                            points[i-1][j].move=false;
                            isGeneration = false;
                            timer_animation->setInterval(speed);
                            onGenerate();
                            return;
                        }
                        points[i][j+1].status = true;
                        points[i][j+1].move = true;
                        points[i][j].status = true;
                        points[i][j].move = true;
                        points[i-1][j+1].status = true;
                        points[i-1][j+1].move = true;
                        points[i+1][j+1].status = true;
                        points[i+1][j+1].move = true;

                        points[i][j-1].status = false;
                        points[i][j-1].move = false;
                        points[i+1][j].status = false;
                        points[i+1][j].move = false;
                        points[i-1][j].status = false;
                        points[i-1][j].move = false;


                        return;
                    }
                }
            }
        }
        if(figure_T_down)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i][j+1].move && points[i+1][j].move && points[i-1][j].move)
                    {
                        if(points[i][j+2].status || j+1 >= 39 || points[i-1][j+1].status || points[i+1][j+1].status)
                        {
                            points[i][j].move=false;
                            points[i][j+1].move=false;
                            points[i+1][j].move=false;
                            points[i-1][j].move=false;
                            isGeneration = false;
                            timer_animation->setInterval(speed);
                            onGenerate();
                            return;
                        }
                        points[i][j+2].status = true;
                        points[i][j+2].move = true;
                        points[i-1][j+1].status = true;
                        points[i-1][j+1].move = true;
                        points[i+1][j+1].status = true;
                        points[i+1][j+1].move = true;

                        points[i][j].status = false;
                        points[i][j].move = false;
                        points[i-1][j].status = false;
                        points[i-1][j].move = false;
                        points[i+1][j].status = false;
                        points[i+1][j].move = false;


                        return;
                    }
                }
            }
        }
        if(figure_T_left)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i][j+1].move && points[i][j-1].move && points[i-1][j].move)
                    {
                        if(points[i][j+2].status || j+1 >= 39 || points[i-1][j+1].status)
                        {
                            points[i][j].move=false;
                            points[i][j+1].move=false;
                            points[i][j-1].move=false;
                            points[i-1][j].move=false;
                            isGeneration = false;
                            timer_animation->setInterval(speed);
                            onGenerate();
                            return;
                        }
                        points[i][j+2].status = true;
                        points[i][j+2].move = true;
                        points[i-1][j+1].status = true;
                        points[i-1][j+1].move = true;

                        points[i][j-1].status = false;
                        points[i][j-1].move = false;
                        points[i-1][j].status = false;
                        points[i-1][j].move = false;


                        return;
                    }
                }
            }
        }
        if(figure_T_right)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i][j+1].move && points[i][j-1].move && points[i+1][j].move)
                    {
                        if(points[i][j+2].status || j+1 >= 39 || points[i+1][j+1].status)
                        {
                            points[i][j].move=false;
                            points[i][j+1].move=false;
                            points[i+1][j].move=false;
                            points[i][j-1].move=false;
                            isGeneration = false;
                            timer_animation->setInterval(speed);
                            onGenerate();
                            return;
                        }
                        points[i][j+2].status = true;
                        points[i][j+2].move = true;
                        points[i+1][j+1].status = true;
                        points[i+1][j+1].move = true;

                        points[i][j-1].status = false;
                        points[i][j-1].move = false;
                        points[i+1][j].status = false;
                        points[i+1][j].move = false;
                        return;
                    }
                }
            }
        }
    }
    if(figure_line)
    {
        if(figure_line_horizontal)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i+1][j].move && points[i+2][j].move)
                    {
                        if(points[i][j+1].status || points[i+1][j+1].status || points[i+2][j+1].status)
                        {
                            points[i][j].move=false;
                            points[i+1][j].move=false;
                            points[i+2][j].move=false;
                            isGeneration = false;
                            timer_animation->setInterval(speed);
                            onGenerate();
                            return;
                        }
                        if(j >= 39)
                        {
                            points[i][j].move=false;
                            points[i+1][j].move=false;
                            points[i+2][j].move=false;
                            isGeneration = false;
                            timer_animation->setInterval(speed);
                            onGenerate();
                            return;
                        }
                        points[i][j+1].status = true;
                        points[i][j+1].move = true;
                        points[i+2][j+1].status = true;
                        points[i+2][j+1].move = true;
                        points[i+1][j+1].status = true;
                        points[i+1][j+1].move = true;

                        points[i][j].status = false;
                        points[i][j].move = false;
                        points[i+2][j].status = false;
                        points[i+2][j].move = false;
                        points[i+1][j].status = false;
                        points[i+1][j].move = false;

                        return;

                    }
                }
            }
        }
        if(figure_line_vertical)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i][j-1].move && points[i][j+1].move)
                    {
                        if(points[i][j+2].status || j+1 >= 39)
                        {
                            points[i][j-1].move=false;
                            points[i][j].move=false;
                            points[i][j+1].move=false;
                            isGeneration = false;
                            timer_animation->setInterval(speed);
                            onGenerate();
                            return;
                        }

                        points[i][j+2].status = true;
                        points[i][j+2].move = true;

                        points[i][j-1].status = false;
                        points[i][j-1].move = false;

                        return;
                    }
                }
            }
        }
    }
    if(figure_S)
    {
        if(figure_S_horizontal)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i-1][j].move && points[i][j-1].move && points[i+1][j-1].move)
                    {
                        if(points[i][j+1].status || points[i-1][j+1].status || j>=39 || points[i+1][j].status)
                        {
                            points[i][j].move=false;
                            points[i][j-1].move=false;
                            points[i+1][j-1].move=false;
                            points[i-1][j].move=false;
                            isGeneration = false;
                            timer_animation->setInterval(speed);
                            onGenerate();
                            return;
                        }


                        points[i][j-1].move=false;
                        points[i][j-1].status=false;
                        points[i-1][j].move=false;
                        points[i-1][j].status=false;
                        points[i+1][j-1].move=false;
                        points[i+1][j-1].status=false;

                        points[i+1][j].move=true;
                        points[i+1][j].status=true;
                        points[i][j+1].move=true;
                        points[i][j+1].status=true;
                        points[i-1][j+1].move=true;
                        points[i-1][j+1].status=true;
                        return;
                    }
                }
            }
        }
        if(figure_S_vertical)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i][j-1].move && points[i+1][j].move && points[i+1][j+1].move)
                    {
                        if(points[i][j+1].status || points[i+1][j+2].status || j>=38)
                        {
                            points[i][j].move=false;
                            points[i][j-1].move=false;
                            points[i+1][j].move=false;
                            points[i+1][j+1].move=false;
                            isGeneration = false;
                            timer_animation->setInterval(speed);
                            onGenerate();
                            return;
                        }


                        points[i+1][j].move=false;
                        points[i+1][j].status=false;
                        points[i][j-1].move=false;
                        points[i][j-1].status=false;
                        points[i][j+1].move=true;
                        points[i][j+1].status=true;
                        points[i+1][j+2].move=true;
                        points[i+1][j+2].status=true;
                        return;
                    }
                }
            }
        }
    }
}

void Widget::gameOver()
{
    //функция окончаниы игры
    timer_animation->stop();
    isEndGame=true;
    gameOver_label->show();
}

void Widget::f_pause()
{
    //Функция главного меню(паузы)
    if(isPause)
    {
        timer_animation->start(speed);
        isPause = false;
        pause_label->hide();
        resume_label->hide();
        restart_label->hide();
        exit_label->hide();
    }
    else
    {
        menu_switcher = 1;
        restart_label->setPixmap(QPixmap(":/pause_restart_off.png"));
        resume_label->setPixmap(QPixmap(":/pause_resume_on.png"));
        exit_label->setPixmap(QPixmap(":/pause_exit_off.png"));
        timer_animation->stop();
        isPause = true;
        pause_label->show();
        resume_label->show();
        restart_label->show();
        exit_label->show();
    }
}

void Widget::moveAfterDelete()
{
    //Сдвиг всех фигур на одну клетку вниз при удалении полной линии
    for(int i=0; i<count_full_line; i++)
    {
        for(int i=19; i>=0; i--)
        {
            for(int j=39; j>=0; j--)
            {
                if(points[i][j].status && j!=39)
                {
                    if(!points[i][j+1].status)
                    {
                        points[i][j+1].status = true;
                        points[i][j].status=false;
                    }

                }
            }
        }
    }
    count_full_line = 0;
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    if(!isEndGame && !isPause)
    {
        if(event->key() == Qt::Key_Down)
        {
            timer_animation->setInterval(1);
        }
        if(event->key() == Qt::Key_Left)
        {
            moveLeft();
        }
        if(event->key() == Qt::Key_Right)
        {
            moveRight();
        }
        if(event->key() == Qt::Key_Escape)
        {
            f_pause();
        }
        if(event->key() == Qt::Key_Space)
        {
            if(figure_T)
            {
                if(figure_T_up)
                {
                    for(int i=0; i<=19; i++)
                    {
                        for(int j=0; j<=39; j++)
                        {
                            if(points[i][j].move && points[i][j-1].move && points[i+1][j].move && points[i-1][j].move)
                            {
                                if(!points[i][j+1].status)
                                {
                                    points[i][j+1].status = true;
                                    points[i-1][j].status = false;
                                    points[i][j+1].move = true;
                                    points[i-1][j].move = false;

                                    figure_T_right = true;
                                    figure_T_up = false;
                                    figure_T_down = false;
                                    figure_T_left = false;
                                }
                            }
                        }
                    }
                }
                else if(figure_T_right)
                {
                    for(int i=0; i<=19; i++)
                    {
                        for(int j=0; j<=39; j++)
                        {
                            if(points[i][j].move && points[i][j-1].move && points[i+1][j].move && points[i][j+1].move)
                            {
                                if(!points[i-1][j].status && i!=0)
                                {
                                    points[i-1][j].status = true;
                                    points[i-1][j].move = true;
                                    points[i][j-1].status = false;
                                    points[i][j-1].move = false;
                                    figure_T_right = false;
                                    figure_T_up = false;
                                    figure_T_down = true;
                                    figure_T_left = false;
                                }
                            }
                        }
                    }
                }
                else if(figure_T_down)
                {
                    for(int i=0; i<=19; i++)
                    {
                        for(int j=0; j<=39; j++)
                        {
                            if(points[i][j].move && points[i][j+1].move && points[i+1][j].move && points[i-1][j].move)
                            {
                                points[i][j-1].status = true;
                                points[i][j-1].move = true;
                                points[i+1][j].status = false;
                                points[i+1][j].move = false;

                                figure_T_right = false;
                                figure_T_up = false;
                                figure_T_down = false;
                                figure_T_left = true;
                            }
                        }
                    }
                }
                else if(figure_T_left)
                {
                    for(int i=0; i<=19; i++)
                    {
                        for(int j=0; j<=39; j++)
                        {
                            if(points[i][j].move && points[i-1][j].move && points[i][j-1].move && points[i][j+1].move)
                            {
                                if(!points[i+1][j].status)
                                {
                                    points[i+1][j].status = true;
                                    points[i+1][j].move = true;
                                    points[i][j+1].status = false;
                                    points[i][j+1].move = false;

                                    figure_T_right = false;
                                    figure_T_up = true;
                                    figure_T_down = false;
                                    figure_T_left = false;
                                }
                            }
                        }
                    }
                }

            }
            if(figure_line)
            {
                if(figure_line_horizontal)
                {
                    for(int i=0; i<=19; i++)
                    {
                        for(int j=0; j<=39; j++)
                        {
                            if(points[i][j].move && points[i-1][j].move && points[i+1][j].move)
                            {
                                if(points[i][j+1].status)
                                {
                                    return;
                                }
                                points[i+1][j].status = false;
                                points[i+1][j].move = false;
                                points[i-1][j].status = false;
                                points[i-1][j].move = false;

                                points[i][j-1].status = true;
                                points[i][j-1].move = true;
                                points[i][j+1].status = true;
                                points[i][j+1].move = true;

                                figure_line_horizontal = false;
                                figure_line_vertical = true;

                                return;
                            }
                        }
                    }
                }
                else if(figure_line_vertical)
                {
                    for(int i=0; i<=19; i++)
                    {
                        for(int j=0; j<=39; j++)
                        {
                            if(points[i][j].move && points[i][j-1].move && points[i][j+1].move)
                            {
                                if(points[i-1][j].status || i <= 0 || points[i+1][j].status || i >=19)
                                {
                                    return;
                                }
                                points[i+1][j].status = true;
                                points[i+1][j].move = true;
                                points[i-1][j].status = true;
                                points[i-1][j].move = true;

                                points[i][j-1].status = false;
                                points[i][j-1].move = false;
                                points[i][j+1].status = false;
                                points[i][j+1].move = false;

                                figure_line_horizontal = true;
                                figure_line_vertical = false;

                                return;
                            }
                        }
                    }
                }
            }
            if(figure_S)
            {
                if(figure_S_horizontal)
                {
                    for(int i=0; i<=19; i++)
                    {
                        for(int j=0; j<=39; j++)
                        {
                            if(points[i][j].move && points[i+1][j].move && points[i+1][j-1].move && points[i+2][j-1].move)
                            {
                                if(points[i+2][j].status || points[i+2][j+1].status)
                                {
                                    return;
                                }
                                points[i][j].move = false;
                                points[i][j].status = false;
                                points[i+2][j-1].move = false;
                                points[i+2][j-1].status = false;

                                points[i+2][j].move = true;
                                points[i+2][j].status = true;
                                points[i+2][j+1].move = true;
                                points[i+2][j+1].status = true;

                                figure_S_horizontal = false;
                                figure_S_vertical = true;
                                return;
                            }
                        }
                    }
                }
                if(figure_S_vertical)
                {
                    for(int i=0; i<=19; i++)
                    {
                        for(int j=0; j<=39; j++)
                        {
                            if(points[i][j].move && points[i][j-1].move && points[i+1][j].move && points[i+1][j+1].move)
                            {
                                if(i == 0)
                                {
                                    return;
                                }
                                if(points[i-1][j].status && points[i+1][j-1].status)
                                {
                                    return;
                                }
                                points[i+1][j].move = false;
                                points[i+1][j].status = false;
                                points[i+1][j+1].move = false;
                                points[i+1][j+1].status = false;

                                points[i-1][j].move = true;
                                points[i-1][j].status = true;
                                points[i+1][j-1].move = true;
                                points[i+1][j-1].status = true;

                                figure_S_horizontal = true;
                                figure_S_vertical = false;
                                return;
                            }
                        }
                    }
                }
            }
            if(figure_J)
            {
                if(figure_J_up)
                {
                    for(int i=0; i<=19; i++)
                    {
                        for(int j=0; j<=39; j++)
                        {
                            if(points[i][j].move && points[i+1][j].move && points[i][j+1].move && points[i][j+2].move)
                            {
                                if(i+1 >= 19 || i == 0)
                                {
                                    return;
                                }
                                if(points[i+1][j+1].status || points[i+1][j+2].status || points[i-1][j+1].status)
                                {
                                    return;
                                }
                                points[i][j].status = false;
                                points[i][j].move = false;
                                points[i+1][j].status = false;
                                points[i+1][j].move = false;
                                points[i][j+2].status = false;
                                points[i][j+2].move = false;

                                points[i-1][j+1].status = true;
                                points[i-1][j+1].move = true;
                                points[i+1][j+1].status = true;
                                points[i+1][j+1].move = true;
                                points[i+1][j+2].status = true;
                                points[i+1][j+2].move = true;
                                figure_J_up = false;
                                figure_J_right = true;
                                return;
                            }
                        }
                    }
                }
                else if(figure_J_right)
                {
                    for(int i=0; i<=19; i++)
                    {
                        for(int j=0; j<=39; j++)
                        {
                            if(points[i][j].move && points[i+1][j].move && points[i+2][j+1].move && points[i+2][j].move)
                            {
                                if(points[i+1][j-1].status || points[i][j+1].status || points[i+1][j+1].status)
                                {
                                    return;
                                }
                                points[i][j].status = false;
                                points[i][j].move = false;
                                points[i+2][j].status = false;
                                points[i+2][j].move = false;
                                points[i+2][j+1].status = false;
                                points[i+2][j+1].move = false;

                                points[i+1][j-1].status = true;
                                points[i+1][j-1].move = true;
                                points[i+1][j+1].status = true;
                                points[i+1][j+1].move = true;
                                points[i][j+1].status = true;
                                points[i][j+1].move = true;
                                figure_J_right = false;
                                figure_J_down = true;
                                return;
                            }
                        }
                    }
                }
                else if(figure_J_down)
                {
                    for(int i=0; i<=19; i++)
                    {
                        for(int j=0; j<=39; j++)
                        {
                            if(points[i][j].move && points[i+1][j].move && points[i+1][j-1].move && points[i+1][j-2].move)
                            {
                                if(j+1 >= 39)
                                {
                                    return;
                                }
                                if(points[i][j-1].status || points[i][j-2].status || points[i+2][j-1].status)
                                {
                                    return;
                                }
                                points[i][j].status = false;
                                points[i][j].move = false;
                                points[i+1][j].status = false;
                                points[i+1][j].move = false;
                                points[i+1][j-2].status = false;
                                points[i+1][j-2].move = false;

                                points[i][j-1].status = true;
                                points[i][j-1].move = true;
                                points[i][j-2].status = true;
                                points[i][j-2].move = true;
                                points[i+2][j-1].status = true;
                                points[i+2][j-1].move = true;

                                figure_J_down = false;
                                figure_J_left = true;
                                return;
                            }
                        }
                    }
                }
                else if(figure_J_left)
                {
                    for(int i=0; i<=19; i++)
                    {
                        for(int j=0; j<=39; j++)
                        {
                            if(points[i][j].move && points[i+1][j].move && points[i+2][j].move && points[i][j-1].move)
                            {
                                if(j >= 39)
                                {
                                    return;
                                }
                                if(points[i+1][j+1].status || points[i+1][j-1].status || points[i+1][j-2].status)
                                {
                                    return;
                                }
                                points[i][j].status = false;
                                points[i][j].move = false;
                                points[i][j-1].status = false;
                                points[i][j-1].move = false;
                                points[i+2][j].status = false;
                                points[i+2][j].move = false;

                                points[i+1][j-1].status = true;
                                points[i+1][j-1].move = true;
                                points[i+1][j+1].status = true;
                                points[i+1][j+1].move = true;
                                points[i+2][j-1].status = true;
                                points[i+2][j-1].move = true;

                                figure_J_left = false;
                                figure_J_up = true;
                                return;
                            }
                        }
                    }
                }
            }
        }
    }
    else if(isPause)
    {
        //Переключение в меню
        if(event->key() == Qt::Key_Escape)
        {
            f_pause();
        }

        if(event->key() == Qt::Key_Down)
        {
            if(menu_switcher == 1)
            {
                menu_switcher = 2;
                restart_label->setPixmap(QPixmap(":/pause_restart_on.png"));
                resume_label->setPixmap(QPixmap(":/pause_resume_off.png"));
                exit_label->setPixmap(QPixmap(":/pause_exit_off.png"));
            }
            else if(menu_switcher == 2)
            {
                menu_switcher = 3;
                restart_label->setPixmap(QPixmap(":/pause_restart_off.png"));
                resume_label->setPixmap(QPixmap(":/pause_resume_off.png"));
                exit_label->setPixmap(QPixmap(":/pause_exit_on.png"));
            }
            else if(menu_switcher == 3)
            {
                menu_switcher = 1;
                restart_label->setPixmap(QPixmap(":/pause_restart_off.png"));
                resume_label->setPixmap(QPixmap(":/pause_resume_on.png"));
                exit_label->setPixmap(QPixmap(":/pause_exit_off.png"));
            }
        }

        if(event->key() == Qt::Key_Up)
        {
            if(menu_switcher == 1)
            {
                menu_switcher = 3;
                restart_label->setPixmap(QPixmap(":/pause_restart_off.png"));
                resume_label->setPixmap(QPixmap(":/pause_resume_off.png"));
                exit_label->setPixmap(QPixmap(":/pause_exit_on.png"));
            }
            else if(menu_switcher == 2)
            {
                menu_switcher = 1;
                restart_label->setPixmap(QPixmap(":/pause_restart_off.png"));
                resume_label->setPixmap(QPixmap(":/pause_resume_on.png"));
                exit_label->setPixmap(QPixmap(":/pause_exit_off.png"));
            }
            else if(menu_switcher == 3)
            {
                menu_switcher = 2;
                restart_label->setPixmap(QPixmap(":/pause_restart_on.png"));
                resume_label->setPixmap(QPixmap(":/pause_resume_off.png"));
                exit_label->setPixmap(QPixmap(":/pause_exit_off.png"));
            }
        }

        if(event->key() == Qt::Key_Space)
        {
            if(menu_switcher == 1)
            {
                f_pause();
            }
            else if(menu_switcher == 2)
            {
                restart();
            }
            else if(menu_switcher == 3)
            {
                this->close();
            }
        }

        if(event->key() == Qt::Key_Enter || event->key() == 16777220)
        {
            if(menu_switcher == 1)
            {
                f_pause();
            }
            else if(menu_switcher == 2)
            {
                restart();
            }
            else if(menu_switcher == 3)
            {
                this->close();
            }
        }
    }
    else if(isEndGame)
    {
        if(event->key() == Qt::Key_Space || event->key() == Qt::Key_Enter || event->key() == Qt::Key_Escape || event->key() == 16777220)
        {
            restart();
        }
    }
}


void Widget::moveLeft()
{
    if(figure_S)
    {
        if(figure_S_horizontal)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i-1][j].move && points[i][j-1].move && points[i+1][j-1].move)
                    {
                        if(points[i-2][j].status || points[i-1][j-1].status || i-1 == 0)
                        {
                            return;
                        }


                        points[i+1][j-1].move=false;
                        points[i+1][j-1].status=false;
                        points[i][j].move=false;
                        points[i][j].status=false;

                        points[i-2][j].move=true;
                        points[i-2][j].status=true;
                        points[i-1][j-1].move=true;
                        points[i-1][j-1].status=true;
                        return;
                    }
                }
            }
        }
        if(figure_S_vertical)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i][j-1].move && points[i+1][j].move && points[i+1][j+1].move)
                    {
                        if(i <= 0)
                        {
                            return;
                        }
                        if(points[i-1][j].status || points[i][j+1].status || points[i-1][j-1].status)
                        {
                            return;
                        }

                        points[i-1][j].status = true;
                        points[i-1][j].move = true;
                        points[i][j+1].status = true;
                        points[i][j+1].move = true;
                        points[i-1][j-1].status = true;
                        points[i-1][j-1].move = true;


                        points[i][j-1].status=false;
                        points[i][j-1].move=false;
                        points[i+1][j].status=false;
                        points[i+1][j].move=false;
                        points[i+1][j+1].move=false;
                        points[i+1][j+1].status=false;
                        return;
                    }
                }
            }
        }
    }
    if(figure_line)
    {
        if(figure_line_horizontal)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i+1][j].move && points[i+2][j].move)
                    {
                        if(i == 0)
                        {
                            return;
                        }
                        if(points[i-1][j].status)
                        {
                            return;
                        }
                        points[i-1][j].status = true;
                        points[i-1][j].move = true;

                        points[i+2][j].status = false;
                        points[i+2][j].move = false;

                        return;
                    }
                }
            }
        }
        if(figure_line_vertical)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i][j-1].move && points[i][j+1].move)
                    {
                        if(points[i-1][j].status || points[i-1][j-1].status || points[i-1][j+1].status || i == 0)
                        {
                            return;
                        }
                        points[i-1][j-1].status = true;
                        points[i-1][j-1].move = true;
                        points[i-1][j].status = true;
                        points[i-1][j].move = true;
                        points[i-1][j+1].status = true;
                        points[i-1][j+1].move = true;

                        points[i][j].status = false;
                        points[i][j].move = false;
                        points[i][j-1].status = false;
                        points[i][j-1].move = false;
                        points[i][j+1].status = false;
                        points[i][j+1].move = false;
                        return;
                    }
                }
            }
        }
    }
    if(figure_sqare)
    {
        for(int i=0; i<=19; i++)
        {
            for(int j=0; j<=39; j++)
            {
                if(points[i][j].move && points[i][j-1].move && points[i+1][j].move && points[i+1][j-1].move)
                {
                    if(points[i-1][j].status || points[i-1][j-1].status || i == 0)
                    {
                        return;
                    }
                    points[i-1][j].status = true;
                    points[i-1][j].move = true;
                    points[i-1][j-1].status = true;
                    points[i-1][j-1].move = true;

                    points[i+1][j].status = false;
                    points[i+1][j].move = false;
                    points[i+1][j-1].status = false;
                    points[i+1][j-1].move = false;
                    return;
                }
            }
        }
    }
    if(figure_T)
    {
        if(figure_T_up)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i][j-1].move && points[i+1][j].move && points[i-1][j].move)
                    {
                        if(i-1 == 0)
                        {
                            return;
                        }
                        if(points[i-2][j].status)
                        {
                            return;
                        }
                        if(points[i-1][j-1].status)
                        {
                            return;
                        }
                        points[i-2][j].status = true;
                        points[i-2][j].move = true;
                        points[i-1][j].status = true;
                        points[i-1][j].move = true;
                        points[i][j].status = true;
                        points[i][j].move = true;
                        points[i-1][j-1].status = true;
                        points[i-1][j-1].move = true;

                        points[i][j-1].status = false;
                        points[i][j-1].move = false;
                        points[i+1][j].status = false;
                        points[i+1][j].move = false;
                        return;

                    }
                }
            }
        }
        if(figure_T_down)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i][j+1].move && points[i+1][j].move && points[i-1][j].move)
                    {
                        if(i-1 == 0)
                        {
                            return;
                        }
                        if(points[i-2][j].status)
                        {
                            return;
                        }
                        if(points[i-1][j+1].status)
                        {
                            return;
                        }
                        points[i-2][j].status = true;
                        points[i-2][j].move = true;
                        points[i-1][j+1].status = true;
                        points[i-1][j+1].move = true;

                        points[i][j+1].status = false;
                        points[i][j+1].move = false;
                        points[i+1][j].status = false;
                        points[i+1][j].move = false;
                        return;

                    }
                }
            }
        }
        if(figure_T_left)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i][j+1].move && points[i-1][j].move && points[i][j-1].move)
                    {
                        if(i-1 == 0)
                        {
                            return;
                        }
                        if(points[i-2][j].status)
                        {
                            return;
                        }
                        if(points[i-1][j+1].status)
                        {
                            return;
                        }
                        points[i-2][j].status = true;
                        points[i-2][j].move = true;
                        points[i-1][j+1].status = true;
                        points[i-1][j+1].move = true;
                        points[i-1][j-1].status = true;
                        points[i-1][j-1].move = true;

                        points[i][j+1].status = false;
                        points[i][j+1].move = false;
                        points[i][j].status = false;
                        points[i][j].move = false;
                        points[i][j-1].status = false;
                        points[i][j-1].move = false;
                        return;

                    }
                }
            }
        }
        if(figure_T_right)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i][j+1].move && points[i+1][j].move && points[i][j-1].move)
                    {
                        if(i == 0)
                        {
                            return;
                        }
                        if(points[i-1][j].status)
                        {
                            return;
                        }
                        if(points[i-1][j+1].status)
                        {
                            return;
                        }
                        if(points[i-1][j-1].status)
                        {
                            return;
                        }
                        points[i-1][j].status = true;
                        points[i-1][j].move = true;
                        points[i-1][j+1].status = true;
                        points[i-1][j+1].move = true;
                        points[i-1][j-1].status = true;
                        points[i-1][j-1].move = true;

                        points[i][j+1].status = false;
                        points[i][j+1].move = false;
                        points[i][j-1].status = false;
                        points[i][j-1].move = false;
                        points[i+1][j].status = false;
                        points[i+1][j].move = false;
                        return;

                    }
                }
            }
        }
    }
    if(figure_J)
    {
        if(figure_J_up)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i+1][j].move && points[i][j+1].move && points[i][j+2].move)
                    {
                        if(i<=0)
                        {
                            return;
                        }
                        if(points[i-1][j].status || points[i-1][j+1].status || points[i-1][j+2].status)
                        {
                            return;
                        }
                        points[i][j+1].status = false;
                        points[i][j+1].move = false;
                        points[i][j+2].status = false;
                        points[i][j+2].move = false;
                        points[i+1][j].status = false;
                        points[i+1][j].move = false;

                        points[i-1][j].status = true;
                        points[i-1][j].move = true;
                        points[i-1][j+1].status = true;
                        points[i-1][j+1].move = true;
                        points[i-1][j+2].status = true;
                        points[i-1][j+2].move = true;
                        return;
                    }
                }
            }
        }
        if(figure_J_down)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i+1][j].move && points[i+1][j-1].move && points[i+1][j-2].move)
                    {
                        if(i<=0)
                        {
                            return;
                        }
                        if(points[i-1][j].status || points[i][j-1].status || points[i][j-2].status)
                        {
                            return;
                        }
                        points[i+1][j].status = false;
                        points[i+1][j].move = false;
                        points[i+1][j-1].status = false;
                        points[i+1][j-1].move = false;
                        points[i+1][j-2].status = false;
                        points[i+1][j-2].move = false;

                        points[i-1][j].status = true;
                        points[i-1][j].move = true;
                        points[i][j-1].status = true;
                        points[i][j-1].move = true;
                        points[i][j-2].status = true;
                        points[i][j-2].move = true;
                        return;
                    }
                }
            }
        }
        if(figure_J_right)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i+1][j].move && points[i+2][j+1].move && points[i+2][j].move)
                    {
                        if(i<=0)
                        {
                            return;
                        }
                        if(points[i-1][j].status || points[i+1][j+1].status)
                        {
                            return;
                        }
                        points[i+2][j].status = false;
                        points[i+2][j].move = false;
                        points[i+2][j+1].status = false;
                        points[i+2][j+1].move = false;

                        points[i-1][j].status = true;
                        points[i-1][j].move = true;
                        points[i+1][j+1].status = true;
                        points[i+1][j+1].move = true;
                        return;
                    }
                }
            }
        }
        if(figure_J_left)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i+1][j].move && points[i+2][j].move && points[i][j-1].move)
                    {
                        if(i<=0)
                        {
                            return;
                        }
                        if(points[i-1][j].status || points[i-1][j-1].status)
                        {
                            return;
                        }
                        points[i][j-1].status = false;
                        points[i][j-1].move = false;
                        points[i+2][j].status = false;
                        points[i+2][j].move = false;

                        points[i-1][j].status = true;
                        points[i-1][j].move = true;
                        points[i-1][j-1].status = true;
                        points[i-1][j-1].move = true;
                        return;
                    }
                }
            }
        }
    }
}

void Widget::moveRight()
{
    if(figure_S)
    {
        if(figure_S_horizontal)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i-1][j].move && points[i][j-1].move && points[i+1][j-1].move)
                    {
                        if(points[i+1][j].status || points[i+2][j-1].status || i+1 == 19)
                        {
                            return;
                        }


                        points[i-1][j].move=false;
                        points[i-1][j].status=false;
                        points[i][j-1].move=false;
                        points[i][j-1].status=false;

                        points[i+1][j].move=true;
                        points[i+1][j].status=true;
                        points[i+2][j-1].move=true;
                        points[i+2][j-1].status=true;
                        return;
                    }
                }
            }
        }
        if(figure_S_vertical)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i][j-1].move && points[i+1][j].move && points[i+1][j+1].move)
                    {
                        if(i+1 >= 19)
                        {
                            return;
                        }
                        if(points[i+2][j].status || points[i+1][j-1].status || points[i+2][j+1].status)
                        {
                            return;
                        }

                        points[i+2][j].status = true;
                        points[i+2][j].move = true;
                        points[i+2][j+1].status = true;
                        points[i+2][j+1].move = true;
                        points[i+1][j-1].status = true;
                        points[i+1][j-1].move = true;


                        points[i][j].status=false;
                        points[i][j].move=false;

                        points[i][j-1].status=false;
                        points[i][j-1].move=false;

                        points[i+1][j+1].status=false;
                        points[i+1][j+1].move=false;
                        return;
                    }
                }
            }
        }
    }

    if(figure_sqare)
    {
        for(int i=0; i<=19; i++)
        {
            for(int j=0; j<=39; j++)
            {
                if(points[i][j].move && points[i][j-1].move && points[i+1][j].move && points[i+1][j-1].move)
                {
                    if(points[i+2][j].status || points[i+2][j-1].status || i+1 == 19)
                    {
                        return;
                    }
                    points[i+2][j].status = true;
                    points[i+2][j].move = true;
                    points[i+2][j-1].status = true;
                    points[i+2][j-1].move = true;

                    points[i][j].status = false;
                    points[i][j].move = false;
                    points[i][j-1].status = false;
                    points[i][j-1].move = false;
                    return;
                }
            }
        }
    }

    if(figure_T)
    {
        if(figure_T_up)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i][j-1].move && points[i+1][j].move && points[i-1][j].move)
                    {
                        if(i+1 == 19)
                        {
                            return;
                        }
                        if(points[i+2][j].status)
                        {
                            return;
                        }
                        if(points[i+1][j-1].status)
                        {
                            return;
                        }
                        points[i][j].status = true;
                        points[i][j].move = true;
                        points[i+2][j].status = true;
                        points[i+2][j].move = true;
                        points[i+1][j].status = true;
                        points[i+1][j].move = true;

                        points[i+1][j-1].status = true;
                        points[i+1][j-1].move = true;

                        points[i][j-1].status = false;
                        points[i][j-1].move = false;
                        points[i-1][j].status = false;
                        points[i-1][j].move = false;
                        return;

                    }
                }
            }
        }
        if(figure_T_down)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i][j+1].move && points[i+1][j].move && points[i-1][j].move)
                    {
                        if(i+1 == 19)
                        {
                            return;
                        }
                        if(points[i+2][j].status)
                        {
                            return;
                        }
                        if(points[i+1][j+1].status)
                        {
                            return;
                        }
                        points[i+2][j].status = true;
                        points[i+2][j].move = true;
                        points[i+1][j+1].status = true;
                        points[i+1][j+1].move = true;

                        points[i][j+1].status = false;
                        points[i][j+1].move = false;
                        points[i-1][j].status = false;
                        points[i-1][j].move = false;
                        return;

                    }
                }
            }
        }
        if(figure_T_left)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i][j+1].move && points[i-1][j].move && points[i][j-1].move)
                    {
                        if(i == 19)
                        {
                            return;
                        }
                        if(points[i+1][j].status)
                        {
                            return;
                        }
                        if(points[i+1][j+1].status)
                        {
                            return;
                        }
                        if(points[i+1][j-1].status)
                        {
                            return;
                        }
                        points[i+1][j].status = true;
                        points[i+1][j].move = true;
                        points[i+1][j+1].status = true;
                        points[i+1][j+1].move = true;
                        points[i+1][j-1].status = true;
                        points[i+1][j-1].move = true;

                        points[i][j+1].status = false;
                        points[i][j+1].move = false;
                        points[i][j-1].status = false;
                        points[i][j-1].move = false;
                        points[i-1][j].status = false;
                        points[i-1][j].move = false;
                        return;

                    }
                }
            }
        }
        if(figure_T_right)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i][j+1].move && points[i+1][j].move && points[i][j-1].move)
                    {
                        if(i+1 == 19)
                        {
                            return;
                        }
                        if(points[i+2][j].status)
                        {
                            return;
                        }
                        if(points[i+1][j+1].status)
                        {
                            return;
                        }
                        if(points[i+1][j-1].status)
                        {
                            return;
                        }
                        points[i+2][j].status = true;
                        points[i+2][j].move = true;
                        points[i+1][j+1].status = true;
                        points[i+1][j+1].move = true;
                        points[i+1][j-1].status = true;
                        points[i+1][j-1].move = true;

                        points[i][j+1].status = false;
                        points[i][j+1].move = false;
                        points[i][j-1].status = false;
                        points[i][j-1].move = false;
                        points[i][j].status = false;
                        points[i][j].move = false;
                        return;

                    }
                }
            }
        }
    }

    if(figure_line)
    {
        if(figure_line_horizontal)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i+2][j].move && points[i+1][j].move)
                    {
                        if((i+2 != 19) && !points[i+3][j].status)
                        {
                            points[i+3][j].status = true;
                            points[i+3][j].move = true;

                            points[i][j].status = false;
                            points[i][j].move = false;
                        }

                        return;
                    }
                }
            }
        }
        if(figure_line_vertical)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i][j-1].move && points[i][j+1].move)
                    {
                        if(points[i+1][j].status || points[i+1][j-1].status
                            ||points[i+1][j+1].status || i >= 19)
                        {
                            return;
                        }
                        points[i+1][j-1].status = true;
                        points[i+1][j-1].move = true;
                        points[i+1][j].status = true;
                        points[i+1][j].move = true;
                        points[i+1][j+1].status = true;
                        points[i+1][j+1].move = true;


                        points[i][j].status = false;
                        points[i][j].move = false;
                        points[i][j-1].status = false;
                        points[i][j-1].move = false;
                        points[i][j+1].status = false;
                        points[i][j+1].move = false;
                        return;
                    }
                }
            }
        }
    }

    if(figure_J)
    {
        if(figure_J_up)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i+1][j].move && points[i][j+1].move && points[i][j+2].move)
                    {
                        if(i+1>=19)
                        {
                            return;
                        }
                        if(points[i+2][j].status || points[i+1][j+1].status || points[i+1][j+2].status)
                        {
                            return;
                        }
                        points[i][j+1].status = false;
                        points[i][j+1].move = false;
                        points[i][j+2].status = false;
                        points[i][j+2].move = false;
                        points[i][j].status = false;
                        points[i][j].move = false;

                        points[i+2][j].status = true;
                        points[i+2][j].move = true;
                        points[i+1][j+1].status = true;
                        points[i+1][j+1].move = true;
                        points[i+1][j+2].status = true;
                        points[i+1][j+2].move = true;
                        return;
                    }
                }
            }
        }
        if(figure_J_down)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i+1][j].move && points[i+1][j-1].move && points[i+1][j-2].move)
                    {
                        if(i+1>=19)
                        {
                            return;
                        }
                        if(points[i+2][j].status || points[i+2][j-1].status || points[i+2][j-2].status)
                        {
                            return;
                        }
                        points[i][j].status = false;
                        points[i][j].move = false;
                        points[i+1][j-1].status = false;
                        points[i+1][j-1].move = false;
                        points[i+1][j-2].status = false;
                        points[i+1][j-2].move = false;

                        points[i+2][j].status = true;
                        points[i+2][j].move = true;
                        points[i+2][j-1].status = true;
                        points[i+2][j-1].move = true;
                        points[i+2][j-2].status = true;
                        points[i+2][j-2].move = true;
                        return;
                    }
                }
            }
        }
        if(figure_J_right)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i+1][j].move && points[i+2][j+1].move && points[i+2][j].move)
                    {
                        if(i+2>=19)
                        {
                            return;
                        }
                        if(points[i+3][j].status || points[i+3][j+1].status)
                        {
                            return;
                        }
                        points[i][j].status = false;
                        points[i][j].move = false;
                        points[i+2][j+1].status = false;
                        points[i+2][j+1].move = false;

                        points[i+3][j].status = true;
                        points[i+3][j].move = true;
                        points[i+3][j+1].status = true;
                        points[i+3][j+1].move = true;
                        return;
                    }
                }
            }
        }
        if(figure_J_left)
        {
            for(int i=0; i<=19; i++)
            {
                for(int j=0; j<=39; j++)
                {
                    if(points[i][j].move && points[i+1][j].move && points[i+2][j].move && points[i][j-1].move)
                    {
                        if(i+2>=19)
                        {
                            return;
                        }
                        if(points[i+3][j].status || points[i+1][j-1].status)
                        {
                            return;
                        }
                        points[i][j].status = false;
                        points[i][j].move = false;
                        points[i][j-1].status = false;
                        points[i][j-1].move = false;

                        points[i+3][j].status = true;
                        points[i+3][j].move = true;
                        points[i+1][j-1].status = true;
                        points[i+1][j-1].move = true;
                        return;
                    }
                }
            }
        }
    }
}

void Widget::checkFullLine()
{
    count_full_line = 0;
    for(int i=0; i<=39; i++)
    {
        bool delet = true;
        for(int j=0; j<=19; j++)
        {
            if(!points[j][i].status)
            {
                delet = false;
                break;
            }
        }
        if(delet)
        {
            count_full_line += 1;
            isGeneration = true;
            for(int j=0; j<=19; j++)
            {
                if(!points[j][i].move)
                    points[j][i].del = true;
                score += 1;
            }

            if(speed > 1)
            {
                speed -= 1;
            }
            deleter();
        }

    }

    timer_animation->start(speed);
}

void Widget::deleter()
{
    animation();
    timer_animationDelete->start(500);
}

void Widget::restart()
{

    gameOver_label->hide();
    pause_label->hide();
    resume_label->hide();
    restart_label->hide();
    exit_label->hide();

    count_full_line   =  0;
    score             =  0;
    speed             =  120;

    figure_S          =  false;
    figure_S_vertical             =  false;
    figure_S_horizontal             =  false;
    figure_line       =  false;
    figure_line_vertical          =  false;
    figure_line_horizontal          =  false;
    figure_sqare      =  false;
    figure_T       =  false;
    figure_T_up           =  false;
    figure_T_down         =  false;
    figure_T_left         =  false;
    figure_T_right        =  false;
    figure_J          =  false;
    figure_J_up              =  false;
    figure_J_right           =  false;
    figure_J_left            =  false;
    figure_J_down            =  false;
    isPause           =  false;
    isEndGame         =  false;
    isGeneration      =  false;


    for(int i=0; i<=19; i++)
    {
        for(int j=0; j<=39; j++)
        {
            points[i][j].del = false;
            points[i][j].status = false;
            points[i][j].move = false;
        }
    }
    animation();
    score_label->setText("score: 0");
    timer_animation->start(speed);
    onGenerate();
}

