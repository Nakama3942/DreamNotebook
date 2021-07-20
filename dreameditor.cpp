#include "dreameditor.h"
#include "ui_dreameditor.h"

#include "dreamnotebook.h"

DreamEditor::DreamEditor(QString pathfile, QString namefile, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DreamEditor)
{
    ui->setupUi(this);

    //Устанавливаю рассположение окна
    this->move(0, 40);

    //Устанавливаю пути
    *path = pathfile;
    *oldname = namefile;

    //Отображаю окно
    this->show();

    //Считываю файл
    readDream();

    //Устанавливаю статус файла: Новый или существующий
    switch(statDream)
    {
    case statusDream::New:
        ifNewDream();
        break;
    case statusDream::Existing:
        ifOldDream();
        break;
    }

    //Увеличиваю отображение текста
    ui->dreamEdit->zoomIn(4);
}

DreamEditor::~DreamEditor()
{
    delete ui;
}

//Слот сохранения
void DreamEditor::on_buttSave_released()
{
    //Записываю сон в файл
    writeDream();

    //Симулирую сигнал закрытия
    emit closing();

    //Удаляю пути
    delete(path);
    delete(oldname);
    delete(newname);

    //Закрываю окно
    close();
}

//Слот удаления
void DreamEditor::on_buttDel_released()
{
    //Вывожу предупреждение, если пользователь согласен - удаляю файл
    QMessageBox Warn;
    Warn.setIcon(QMessageBox::Warning);
    Warn.setText(tr("<html><head/><body><p><span style=\" font-size:12pt;\">Внимание</span></p></body></html>"));
    Warn.setInformativeText("Это удалит запись безвозратно! Вы уверенны?");
    Warn.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    Warn.setDefaultButton(QMessageBox::Cancel);
    int clearer = Warn.exec();
    switch (clearer)
    {
    case QMessageBox::Cancel:
        break;
    case QMessageBox::Ok:
        QFile::remove(*path);
        emit closing();
        delete(path);
        delete(oldname);
        delete(newname);
        close();
    }
}

//Метод считывания файлов
void DreamEditor::readDream()
{
    //Открываю файл и считываю поток
    QFile dreamFile(*path);
    if(dreamFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream dreamStream(&dreamFile);
        ui->dreamEdit->setText(dreamStream.readAll());
    }
    dreamFile.close();

    //Если поток пуст - файл созданный, иначе - открытый (существующий)
    if(ui->dreamEdit->toPlainText() == "")
    {
        //Отмечаю статус потока
        statDream = statusDream::New;
    }
    else
    {
        //Отмечаю статус потока
        statDream = statusDream::Existing;
    }

    //Устанавливаю название файла в поле названия
    ui->lineName->setText(*oldname);
}

//Метод записывания файлов
void DreamEditor::writeDream()
{
    //Считываю название
    *newname = ui->lineName->text();

    //если оно не совпадает со старым - пользователь изменил название
    if(*oldname != *newname)
    {
        //Тогда я удаляю старый файл и создаю путь к новому
        QFile::remove(*path);
        *path = "dreams/" + *newname + ".txt";
    }

    //Открываю файл и записываю поток
    QFile dreamFile(*path);
    if(dreamFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream dreamStream(&dreamFile);
        dreamStream << ui->dreamEdit->toPlainText();
    }
    dreamFile.close();
}

//Метод настройки созданных файлов
void DreamEditor::ifNewDream()
{
    //Если файл новый - беру время создания, добаваляю в начало текста,
    //и переношу курсор в конец текста на новую строку и устанавливаю фокус
    QDateTime awakening(QDateTime::currentDateTime());
    QString Date = awakening.date().toString(),
            Time = awakening.time().toString();
    ui->dreamEdit->setText(Date + " " + Time + "\n");
    ui->dreamEdit->setFocus();
    ui->dreamEdit->moveCursor(QTextCursor::End);
}

//Метод настройки открытых файлов
void DreamEditor::ifOldDream()
{
    //Иначе - просто устанавливаю фокур и переношу в курсор в конец файла
    ui->dreamEdit->setFocus();
    ui->dreamEdit->moveCursor(QTextCursor::End);
}
