#include <QLineEdit>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QIntValidator>
#include <QLabel>
#include <QDebug>

#include "newfiledialog.h"

NewFileDialog::NewFileDialog(QWidget *parent) : QDialog(parent)
{
    setWindowFlags(Qt::Window);
    setWindowTitle("New File");
    setFixedSize(300, 200);
    QFormLayout *layout = new QFormLayout(this);

    validator = new QIntValidator(500, 10000, this);
    widthField = new QLineEdit(tr("1000"), this);
    widthField->setValidator(validator);

    heightField = new QLineEdit(tr("1000"), this);
    heightField->setValidator(validator);

    layout->addRow(new QLabel("Set canvas size. Both values must be in interval"
                              " <500, 10000>"));
    layout->addRow(new QLabel("width"));
    layout->addRow(widthField);
    layout->addRow(new QLabel("height"));
    layout->addRow(heightField);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
                                                       QDialogButtonBox::Cancel,
                                                       Qt::Horizontal, this);
    layout->addRow(buttonBox);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void NewFileDialog::accept() {
    int pos = 0;
    QString width = widthField->text();
    QString height = heightField->text();
    if (QValidator::Acceptable == validator->validate(width, pos) &&
        QValidator::Acceptable == validator->validate(height, pos) )
    {
        QDialog::accept();
    }
    else {
        QMessageBox msg;
        msg.critical(0, "Error", "Invalid values.");
    }
}

int NewFileDialog::getWidth() const {
    int res = QString(widthField->text()).toInt();
    return res;
}

int NewFileDialog::getHeight() const {
    int res = QString(heightField->text()).toInt();
    return res;
}
