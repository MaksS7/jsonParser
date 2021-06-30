#include "parser.h"
#include "ui_parser.h"

Parser::Parser(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Parser)
{
    ui->setupUi(this);
}

Parser::~Parser()
{
    delete ui;
}
void Parser::loadGame()
{
    QFile loadFile(QStringLiteral("save.json"));

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");

    }

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    read(loadDoc.object());


//    QTextStream(stdout) << "Loaded save for "
//                        << loadDoc["player"]["name"].toString()
//                        << " using "
//                        << (saveFormat != Json ? "binary " : "") << "JSON...\n";
}

void Parser::read(const QJsonObject &json)
{
    if (json.isEmpty()) {
        qDebug() << "file empty";
        return;
    }

    QStringList nameFileImages = json.keys();
    foreach (auto NameFile, nameFileImages) {
        QJsonObject temporaryObject = json[NameFile].toObject();
        if (temporaryObject.contains("annotations") && temporaryObject["annotations"].isArray()) {
            QJsonArray tempArray = temporaryObject["annotations"].toArray();
            foreach (auto element, tempArray) {
                QJsonObject boxObject =  element.toObject();
                QStringList tempListString;
                tempListString.push_back(boxObject["x"].toString());
                tempListString.push_back(boxObject["y"].toString());
                tempListString.push_back(boxObject["width"].toString());
                tempListString.push_back(boxObject["height"].toString());
                boxInTheImage.insert(boxObject["label"].toString(), tempListString);
            }
        }
    }
}

void Parser::on_pushButton_clicked()
{
    loadGame();
}
