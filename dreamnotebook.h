#ifndef DREAMNOTEBOOK_H
#define DREAMNOTEBOOK_H

#include <QMainWindow>

#include <QFileSystemModel>
#include <QInputDialog>
#include <QDesktopServices>

#include <QMessageBox>
#include <QUrl>

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

    void on_aboutProg_triggered(); //Открывает окно "О программе"
    void on_source_triggered(); //Ссылается на репозиторий на ГитХаб
    void on_aboutQt_triggered(); //Открывает окно "О Qt"

private:
    Ui::DreamNotebook *ui;

    ModeModel viewMode;

    QFileSystemModel *dreams, *trash;

    QString filepath;
    QStringList filename;

    void setTree(); //Установка файлового дерева
    void openDream(); //Открытие файла записи сна
    void deleteDream(); //Удаление файла записи сна
    void resume(); //Возобновляю текущее окно
};
#endif // DREAMNOTEBOOK_H
