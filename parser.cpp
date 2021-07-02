#include "parser.h"
#include "ui_parser.h"

Parser::Parser(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Parser)
{
    ui->setupUi(this);
    ui->labelCountAllImage->setVisible(false);
    ui->lineEditCountAllBox->setVisible(false);

    ui->tableWidgetCountClasses->setHorizontalHeaderLabels({"Class", "Count"});
    ui->tableWidgetCountClasses->setVisible(false);
}

Parser::~Parser()
{
    delete ui;
}
void Parser::load()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(
                            this, "Select one or more files to open",
                            QDir::currentPath(), "Json(s) (*.json)");

    if (fileNames.isEmpty()) {
        QMessageBox::warning(this, tr("Parser"), tr("No file for parser"));
        return;
    }

    ui->spinBoxCountJsonFiles->setValue(fileNames.size());

    QFileInfo info(fileNames[0]);
    curentPath = info.path();

    foreach (auto &file, fileNames) {
        QFile loadFile(file);
        if (!loadFile.open(QIODevice::ReadOnly)) {
            qWarning("Couldn't open file.");
        }
        QJsonDocument loadDoc(QJsonDocument::fromJson(loadFile.readAll()));
        read(loadDoc.object());
        loadFile.close();
    }
}

void Parser::read(const QJsonObject &json)
{
    if (json.isEmpty()) {
        qWarning("file empty");
        return;
    }

    QStringList nameFileImages = json.keys();
    foreach (auto& NameFile, nameFileImages) {
        QJsonObject temporaryObject = json[NameFile].toObject();
        if (temporaryObject.contains("annotations") && temporaryObject["annotations"].isArray()) {
            QJsonArray tempArray = temporaryObject["annotations"].toArray();
            countBoxInAllImages += tempArray.size();
            foreach (auto element, tempArray) {
                QJsonObject boxObject =  element.toObject();
                box inBox;
                inBox.id = boxObject["label"].toString().toInt();
                inBox.x = boxObject["x"].toString().toFloat();
                inBox.y = boxObject["y"].toString().toFloat();
                inBox.width = boxObject["width"].toString().toFloat();
                inBox.height = boxObject["height"].toString().toFloat();
                boxInTheImage.push_back(inBox);

                clasesInFiles.insert(QString::number(inBox.id), clasesInFiles[QString::number(inBox.id)] + 1);
            }
            if (ui->checkBoxConvertCoordinates->isChecked() &&
                      !(ui->lineEditWidth->text().isEmpty() || ui->lineEditHeight->text().isEmpty())) {
                qDebug() << "converting...";
                convertCoordinates();
            }
            if (ui->checkBoxSaveToTxt->isChecked()) {
                qDebug() << "saving...";
                QString tempNameFile = NameFile;
                tempNameFile.replace(tempNameFile.size() - 4, 4, ".txt");
                saveToTxt(tempNameFile);
            }
        }
        boxInTheImage.clear();
    }
    ui->checkBoxShowAdditInfo->setCheckable(true);
}

void Parser::on_btnOpenFileJsons_clicked()
{
    load();
}

void Parser::convertCoordinates()
{
    uint w = ui->lineEditWidth->text().toUInt();
    uint h = ui->lineEditHeight->text().toUInt();
    foreach (auto element, boxInTheImage) {
        element.sizeImage.setX(w);
        element.sizeImage.setY(h);
        element.paramX = (element.x + element.width / 2) / w;
        element.paramX = (element.y + element.height / 2) / h;
        element.paramWidth = element.width / w;
        element.paramHeight = element.height / h;
    }
    qDebug() << "converted";
}

void Parser::saveToTxt(const QString &name)
{
    QFile loadFile(curentPath + "/" + name);
    if (!loadFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning("Couldn't open file.");
    }
    QTextStream streamToFile(&loadFile);
    foreach(auto& theBox, boxInTheImage) {
        if (ui->checkBoxConvertCoordinates->isChecked()) {
            streamToFile << theBox.id << " " << theBox.paramX << " " << theBox.paramY << " " << theBox.paramWidth << " " << theBox.paramHeight << "\n";
        } else {
            streamToFile << theBox.id << " " << theBox.x << " " << theBox.y << " " << theBox.width << " " << theBox.height << "\n";
        }
    }
    loadFile.close();
    qDebug() << "saved";
}


void Parser::on_checkBoxShowAdditInfo_clicked(bool checked)
{
    if (checked) {
        ui->lineEditCountAllBox->setText(QString::number(countBoxInAllImages));
        int count = 0;
        ui->tableWidgetCountClasses->setRowCount(clasesInFiles.size());
        foreach (auto item, clasesInFiles.keys()) {
            QTableWidgetItem *classes = new QTableWidgetItem(item);
            QTableWidgetItem *countClasses = new QTableWidgetItem(QString::number(clasesInFiles[item]));
            ui->tableWidgetCountClasses->setItem(count, 0, classes);
            ui->tableWidgetCountClasses->setItem(count++, 1, countClasses);
        }
        ui->tableWidgetCountClasses->setColumnWidth(0, 4);
        ui->tableWidgetCountClasses->setColumnWidth(1, 4);
    }
    ui->labelCountAllImage->setVisible(checked);
    ui->lineEditCountAllBox->setVisible(checked);
    ui->tableWidgetCountClasses->setVisible(checked);

}
