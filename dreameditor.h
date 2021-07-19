#ifndef DREAMEDITOR_H
#define DREAMEDITOR_H

#include <QWidget>

#include <QDateTime>

#include <QVector>

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
    void on_buttSave_released();

    void on_buttDel_released();

private:
    Ui::DreamEditor *ui;

    QString *path = new QString, *oldname = new QString, *newname = new QString;

    statusDream statDream;

    void readDream();
    void writeDream();
    void ifNewDream();
    void ifOldDream();
};

#endif // DREAMEDITOR_H
