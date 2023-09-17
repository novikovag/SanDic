#ifndef CARD_H
#define CARD_H

#include <QtSql>
#include <QtWidgets>
#include <QRegularExpression>

#include "config.h"

struct AbbItem
{
  AbbItem(QString abb, QString desc)
      : rx(QRegularExpression("(^|\\W)(" + QRegularExpression::escape(abb) + ")(\\W|$)")),
        desc(desc.replace("'", "′")) {}
  QRegularExpression rx;
  QString desc;
};

class Card : public QTextEdit
{
  Q_OBJECT

public:
  Card(Card*);
  Card(Config*, QSqlDatabase*);

public slots:
  void article(QString, QString = "");

private:
  void resizeEvent(QResizeEvent*);
  QString buildRec(QString, QList<QStringList>);
  QString buildList(QStringList);

  Config* config;
  QSqlDatabase* db;
  QMap<QString, QList<AbbItem>> abbs;
};

#endif // CARD_H
