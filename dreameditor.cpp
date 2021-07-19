#include "dreameditor.h"
#include "ui_dreameditor.h"

#include "dreamnotebook.h"

DreamEditor::DreamEditor(QString pathfile, QString namefile, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DreamEditor)
{
    ui->setupUi(this);

    this->move(0, 40);

    *path = pathfile;
    *oldname = namefile;
    this->show();

    readDream();

    switch(statDream)
    {
    case statusDream::New:
        ifNewDream();
        break;
    case statusDream::Existing:
        ifOldDream();
        break;
    }

    ui->dreamEdit->zoomIn(4);
}

DreamEditor::~DreamEditor()
{
    delete ui;
}


void DreamEditor::on_buttSave_released()
{
    writeDream();
    emit closing();
    delete(path);
    delete(oldname);
    delete(newname);
    close();
}

void DreamEditor::on_buttDel_released()
{
    QFile::remove(*path);
    emit closing();
    delete(path);
    delete(oldname);
    delete(newname);
    close();
}


void DreamEditor::readDream()
{
    QFile dreamFile(*path);

    if(dreamFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream dreamStream(&dreamFile);
        ui->dreamEdit->setText(dreamStream.readAll());
    }
    dreamFile.close();

    if(ui->dreamEdit->toPlainText() == "")
    {
        statDream = statusDream::New;
    }
    else
    {
        statDream = statusDream::Existing;
    }

    ui->lineName->setText(*oldname);
}

void DreamEditor::writeDream()
{
    *newname = ui->lineName->text();

    if(*oldname != *newname)
    {
        QFile::remove(*path);
        *path = "dreams/" + *newname + ".txt";
    }

    QFile dreamFile(*path);

    if(dreamFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream dreamStream(&dreamFile);
        dreamStream << ui->dreamEdit->toPlainText();
    }
    dreamFile.close();
}

void DreamEditor::ifNewDream()
{
    QDateTime awakening(QDateTime::currentDateTime());
    QString Date = awakening.date().toString(),
            Time = awakening.time().toString();
    ui->dreamEdit->setText(Date + " " + Time + "\n");
    ui->dreamEdit->setFocus();
    ui->dreamEdit->moveCursor(QTextCursor::End);
}

void DreamEditor::ifOldDream()
{
    ui->dreamEdit->setFocus();
    ui->dreamEdit->moveCursor(QTextCursor::End);
}


