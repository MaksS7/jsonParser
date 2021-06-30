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

QT_BEGIN_NAMESPACE
namespace Ui { class Parser; }
QT_END_NAMESPACE

class Parser : public QMainWindow
{
    Q_OBJECT

public:
    Parser(QWidget *parent = nullptr);
    ~Parser();
    void loadGame();
    void read(const QJsonObject &json);

private slots:
    void on_pushButton_clicked();

private:
    QMultiMap<QString, QStringList> boxInTheImage;
    Ui::Parser *ui;
};
#endif // PARSER_H
