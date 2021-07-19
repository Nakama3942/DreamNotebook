#ifndef DREAMNOTEBOOK_H
#define DREAMNOTEBOOK_H

#include <QMainWindow>

#include <QFileSystemModel>

#include <QInputDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class DreamNotebook; }
QT_END_NAMESPACE

class DreamNotebook : public QMainWindow
{
    Q_OBJECT

    enum ModeModel{
        Dreams,
        Trash
    };

public:
    DreamNotebook(QWidget *parent = nullptr);
    ~DreamNotebook();

private slots:
    void on_newDream_released();

    void on_buttDreams_released(); //Установка модели заметок сна
    void on_buttTrash_released(); //Установка модели мусора
    void on_dreamTree_activated(const QModelIndex &index); //Выбирает элемент модели

private:
    Ui::DreamNotebook *ui;

    ModeModel viewMode;

    QFileSystemModel *dreams, *trash;

    QString filepath;
    QStringList filename;

    void setTree();
    void openDream();
    void deleteDream();
    void resume();
};
#endif // DREAMNOTEBOOK_H
