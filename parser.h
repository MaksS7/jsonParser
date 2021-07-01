#ifndef PARSER_H
#define PARSER_H

#include <QMainWindow>
#include <QTextStream>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QString>
#include <QMap>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class Parser; }
QT_END_NAMESPACE

class Parser : public QMainWindow
{
    Q_OBJECT

public:
    Parser(QWidget *parent = nullptr);
    ~Parser();
    void load();
    void read(const QJsonObject &json);

private slots:
    void on_btnOpenFileJsons_clicked();

private:
    struct box
    {
        int id;
        float x;
        float y;
        float width;
        float height;
        QPoint sizeImage;
        float paramX;
        float paramY;
        float paramWidth;
        float paramHeight;
    };

    QString pathToDir;
    QString curentPath;

    void convertCoordinates();
    void saveToTxt(const QString &name);

    QVector<box> boxInTheImage;
    Ui::Parser *ui;
};
#endif // PARSER_H
