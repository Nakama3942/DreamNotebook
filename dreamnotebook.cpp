#include "dreamnotebook.h"
#include "ui_dreamnotebook.h"

#include "dreameditor.h"

DreamNotebook::DreamNotebook(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DreamNotebook)
{
    ui->setupUi(this);

    this->move(0, 40);

    //Аналогично окну Открытия/Создания я создаю окно загрузки картинок//

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

    //Устанавливаю размер колонок
    ui->dreamTree->setColumnWidth(0, 500);
    ui->dreamTree->hideColumn(1);
    ui->dreamTree->hideColumn(2);
}

DreamNotebook::~DreamNotebook()
{
    delete ui;
}


void DreamNotebook::on_newDream_released()
{
    bool ok = false;

    filepath = QInputDialog::getText(this,
                                     tr("Добавить сон"),
                                     tr("Название"),
                                     QLineEdit::Normal,
                                     "", &ok);

    if(ok)
    {
        filepath = "dreams/" + filepath + ".txt";

        QFile newdream(filepath);
        newdream.open(QIODevice::WriteOnly);
        newdream.close();

        openDream();
    }
}


void DreamNotebook::on_buttDreams_released()
{
    viewMode = ModeModel::Dreams;
    setTree();
}


void DreamNotebook::on_buttTrash_released()
{
    viewMode = ModeModel::Trash;
    setTree();
}

void DreamNotebook::on_dreamTree_activated(const QModelIndex &index)
{
    filepath = dreams->filePath(index);

    if(ui->checkDel->isChecked())
    {
        deleteDream();
    }
    else
    {
        openDream();
    }
}

void DreamNotebook::setTree()
{
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

void DreamNotebook::openDream()
{
    QRegularExpression rx("[/.]"); //Указываю разделители
    filename = filepath.split(rx); //Разбиваю список на элементы

    filepath = filename.value(filename.size() - 3) + "/" +
               filename.value(filename.size() - 2) + "." +
               filename.value(filename.size() - 1);

    DreamEditor *editor = new DreamEditor(filepath, filename.value(filename.size() - 2));
    editor->show();

    connect(editor, &DreamEditor::closing, this, &DreamNotebook::resume);

    this->hide();

    close();
}

void DreamNotebook::deleteDream()
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

void DreamNotebook::resume()
{
    setTree();
    this->show();
}







