#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "flowchartitem.h"

class Canvas;
class QGraphicsView;
class QHBoxLayout;
class QToolBox;
class QButtonGroup;
class QComboBox;
class QToolButton;
class QFontComboBox;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QHBoxLayout *layout;
    QGraphicsView *view;
    Canvas *canvas;

    // top menus
    QMenu *file_menu;
    QMenu *edit_menu;
    QMenu *about_menu;

    // bottom bar
    QToolBox *toolbox;
    QButtonGroup *item_buttons;
    QButtonGroup *background_buttons;

    // top bar
    QToolBar *toolbar;
    QToolBar *edit_toolbar;
    QToolBar *text_toolbar;
    QToolBar *item_toolbar;

    QComboBox *scale_combo;
    QComboBox *font_size_combo;
    QComboBox *text_color_combo;
    QComboBox *item_color_combo;
    QFontComboBox *font_combo;

    QToolButton *fill_color_button;
    QToolButton *font_color_button;
    QToolButton *line_color_button;

    QButtonGroup *fill_collor_buttons;
    QButtonGroup *line_collor_buttons;
    QButtonGroup *pointer_buttons;

    // actions
    QAction *exit_action;
    QAction *about_action;

    QAction *undo_action;
    QAction *copy_action;
    QAction *paste_action;
    QAction *to_front_action;
    QAction *to_back_action;
    QAction *delete_action;
    QAction *copy_format_action;

    QAction *clear_focus_action;
    QAction *fill_action;

    QAction *bold_action;
    QAction *underline_action;
    QAction *italic_action;

    Ui::MainWindow *ui;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void create_actions();
    void create_toolbox();
    void create_toolbars();
    void create_menus();

    static QWidget *widget_layout(QLayout *layout);
    static QIcon create_color_icon(QColor color = Qt::white, int size = 35);

    QWidget *
    create_bg_cell_widget(const QString &text, const QString &image);
    QWidget *
    create_cell_widget(const QString &text,
                       FlowChartItem::FlowChartItemType type);

    QMenu *create_color_menu(const char *slot, QColor default_color=Qt::white);


    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void about();
    void undo();
    void delete_item();
    void put_front();
    void put_back();
    void item_button_clicked(int id);
    void item_color_changed();
    void scale_changed(const QString &scale);
    void clear_focus();
    void fill_button_triggered();
};

#endif // MAINWINDOW_H
