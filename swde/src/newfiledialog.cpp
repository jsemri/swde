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

    validator = new QIntValidator(500, 5000, this);
    QLineEdit *widthField = new QLineEdit(tr("1000"), this);
    widthField->setValidator(validator);

    QLineEdit *heightField = new QLineEdit(tr("1000"), this);
    heightField->setValidator(validator);

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
    QDialog::accept();
}

int NewFileDialog::getWidth() const {
    return 1000;
}

int NewFileDialog::getHeight() const {
    return 1000;
}

