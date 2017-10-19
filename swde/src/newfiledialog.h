#ifndef NEWFILEDIALOG_H
#define NEWFILEDIALOG_H

#include <QDialog>

class QLineEdit;
class QWidget;
class QMessageBox;
class QIntValidator;

class NewFileDialog : public QDialog
{
private:
    QLineEdit *widthField;
    QLineEdit *heightField;
    QIntValidator *validator;

public:
    NewFileDialog(QWidget *parent);
    int getWidth() const;
    int getHeight() const;

public slots:
    void accept() override;
};

#endif // NEWFILEDIALOG_H
