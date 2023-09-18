#ifndef CONFIG_H
#define CONFIG_H

#include <QtWidgets>

#define ORGNAME  "NovikovAG"
#define PROGNAME "SanDic"
#define VERSION  "2.7"
#define DBNAME   "sandic.db"

class Config : public QSettings
{
public:
  Config();
 ~Config();

  QString lang;
  QMap<QString, QString> langs;

  QString style;
  QMap<QString, QVariant> styles;

  QStringList originKeys;
  QString originSuff;
  QMap<QString, QVariant> order;

  QByteArray mainWindowGeometry;
  QByteArray mainWindowState;

  QSize cardSize;
  QString readFile(QString);
};

#endif // CONFIG_H
