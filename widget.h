#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QKeyEvent>

struct point
{
    bool move;
    bool status;
    bool del;
    QLabel *label;
};
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void restart();
    void onGenerate();
    void animation();
    void moving();
    void moveLeft();
    void moveRight();
    void checkFullLine();
    void deleter();
    void gameOver();
    void f_pause();
    void moveAfterDelete();
    virtual void keyPressEvent(QKeyEvent *event);
private:
    unsigned score;
    unsigned speed;
    unsigned count_full_line;
    unsigned menu_switcher;

    bool isPause;
    bool isEndGame;
    bool isGeneration;

    bool figure_line, figure_line_vertical, figure_line_horizontal;
    bool figure_sqare;
    bool figure_T, figure_T_up, figure_T_down, figure_T_left, figure_T_right;
    bool figure_J, figure_J_up, figure_J_right, figure_J_left, figure_J_down;
    bool figure_S, figure_S_vertical, figure_S_horizontal;

    QTimer *timer_animation, *timer_animationDelete;
    QLabel *score_label;
    point points[20][40];

    QLabel *gameOver_label, *pause_label, *resume_label, *restart_label, *exit_label;


};
#endif // WIDGET_H

