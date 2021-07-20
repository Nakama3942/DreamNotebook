#include "dreamnotebook.h"
#include "ui_dreamnotebook.h"

#include "dreameditor.h"

DreamNotebook::DreamNotebook(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DreamNotebook)
{
    ui->setupUi(this);

    //Устанавливаю рассположение окна
    this->move(0, 40);

    //Создаю модель дерева записей снов
    dreams = new QFileSystemModel;
    dreams->setNameFilters(QStringList() << "*.txt");
    dreams->setNameFilterDisables(false);

    //Создаю модель дерева мусора
    trash = new QFileSystemModel;
    trash->setNameFilters(QStringList() << "*.txt");
    trash->setNameFilterDisables(false);

    //Устанавливаю начальную модель
    ui->buttDreams->setChecked(true);
    viewMode = ModeModel::Dreams;
    setTree();

    //Устанавливаю размер первой колонки и убираю лишние
    ui->dreamTree->setColumnWidth(0, 500);
    ui->dreamTree->hideColumn(1);
    ui->dreamTree->hideColumn(2);
}

DreamNotebook::~DreamNotebook()
{
    delete ui;
}

//Слот создания нового файла записи сна
void DreamNotebook::on_newDream_released()
{
    bool ok = false;

    //Создаю диалог ввода, где пользовательдолжен ввести название сна
    filepath = QInputDialog::getText(this,
                                     tr("Добавить сон"),
                                     tr("Название"),
                                     QLineEdit::Normal,
                                     "", &ok);

    //Если название введено
    if(ok)
    {
        //Создаю путь к файлу
        filepath = "dreams/" + filepath + ".txt";

        //Создаю файл по этому пути
        QFile newdream(filepath);
        newdream.open(QIODevice::WriteOnly);
        newdream.close();

        //Вызываю метод, который
        openDream();
    }
}

//Слот, который устанавливает режим отображения записей сна
void DreamNotebook::on_buttDreams_released()
{
    viewMode = ModeModel::Dreams;
    setTree();
}

//Слот, который устанавливает режим отображения мусора
void DreamNotebook::on_buttTrash_released()
{
    viewMode = ModeModel::Trash;
    setTree();
}

//Слот, обрабатывающий выбранный файл
void DreamNotebook::on_dreamTree_activated(const QModelIndex &index)
{
    //Беру путь файла из индекса системного дерева
    filepath = dreams->filePath(index);

    //Если установлена проверка "удаления" - вызываю метод
    //уделания файла, иначе - метод открытия
    if(ui->checkDel->isChecked())
    {
        deleteDream();
    }
    else
    {
        openDream();
    }
}

//Слот вывода информации о программе
void DreamNotebook::on_aboutProg_triggered()
{
    QMessageBox Inform;
    Inform.setIcon(QMessageBox::Information);
    Inform.setText(tr("<html><head/><body><p><span style=\" font-size:12pt;\">Про программу</span></p></body></html>"));
    Inform.setInformativeText("<html><head/><body><p><span style=\" font-size:11pt; font-style:italic;\">"
                              "Это простая программа для записи своих снов.</span></p></body></html>");
    Inform.setStandardButtons(QMessageBox::Ok);
    Inform.setDefaultButton(QMessageBox::Ok);
    int clearer = Inform.exec();
    switch (clearer) { case QMessageBox::Ok: break; }
}

//Слот, открывающий репозиторий проекта в браузере
void DreamNotebook::on_source_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/Nakama3942/DreamNotebook"));
}

//Слот, открывающий информацию о Qt
void DreamNotebook::on_aboutQt_triggered()
{
    QMessageBox::aboutQt(this, "Про Qt");
}

//Метод установки модели файлового дерева
void DreamNotebook::setTree()
{
    //Если режим модели установлен на отображение записей снов - устанавливаю
    //Определённую модель отображения записей снов, иначе - модель мусора
    switch(viewMode)
    {
    case ModeModel::Dreams:
        //Передаю модель в TreeView
        ui->dreamTree->setModel(dreams);
        ui->dreamTree->setRootIndex(dreams->index("dreams/"));
        dreams->setRootPath("dreams/"); //Устанавливаю текущий каталог
        break;
    case ModeModel::Trash:
        //Передаю модель в TreeView
        ui->dreamTree->setModel(trash);
        ui->dreamTree->setRootIndex(trash->index("trash/"));
        trash->setRootPath("trash/"); //Устанавливаю текущий каталог
        break;
    }
}

//Метод открытия файлов записей сна
void DreamNotebook::openDream()
{
    QRegularExpression rx("[/.]"); //Указываю разделители
    filename = filepath.split(rx); //Разбиваю список на элементы

    //Формирую путь к файлу
    filepath = filename.value(filename.size() - 3) + "/" +
               filename.value(filename.size() - 2) + "." +
               filename.value(filename.size() - 1);

    //Открываю окно редактирования записей снов, передавая
    //ему путь к файлу и его название
    DreamEditor *editor = new DreamEditor(filepath, filename.value(filename.size() - 2));
    editor->show();

    //Связываю новое окно с методом возобновления текущего при закрытии нового
    connect(editor, &DreamEditor::closing, this, &DreamNotebook::resume);

    //Скрываю текущее окно
    this->hide();
}

//Метод удаления заметок
void DreamNotebook::deleteDream()
{
    QRegularExpression rx("[/.]"); //Указываю разделители
    filename = filepath.split(rx); //Разбиваю список на элементы

    //Если установлена проверка "удаления, минуя корзину" - удаляю файл сразу,
    //иначе: если установлен режим отображения записей снов - переношу файл записи
    //в директорию мусора - иначе - удаляю файл из директории мусора
    if(ui->checkDestroy->isChecked())
    {
        QFile::remove(filepath);
    }
    else
    {
        switch(viewMode)
        {
        case ModeModel::Dreams:
            QFile::rename(filepath, "trash/" + filename.value(filename.size() - 2) + ".txt");
            break;
        case ModeModel::Trash:
            QFile::remove(filepath);
            break;
        }
    }
}

//Метод возобновления окна
void DreamNotebook::resume()
{
    //Устанавливаю модель файлового дерева и отображаю окно
    setTree();
    this->show();
}
