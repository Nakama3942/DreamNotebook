#ifndef DREAMEDITOR_H
#define DREAMEDITOR_H

#include <QWidget>

#include <QDateTime>

namespace Ui {
class DreamEditor;
}

class DreamEditor : public QWidget
{
    Q_OBJECT

    enum statusDream
    {
        New,
        Existing
    };

public:
    explicit DreamEditor(QString pathfile, QString namefile, QWidget *parent = nullptr);
    ~DreamEditor();

signals:
    void closing();

private slots:
    void on_buttSave_released(); //Сохраняю сон в файл
    void on_buttDel_released(); //Удаляю файл

private:
    Ui::DreamEditor *ui;

    QString *path = new QString, *oldname = new QString, *newname = new QString;

    statusDream statDream;

    void readDream(); //Читаю файл
    void writeDream(); //Записываю файл
    void ifNewDream(); //Настройка созданного файла
    void ifOldDream(); //Настройка открытого файла
};

#endif // DREAMEDITOR_H
