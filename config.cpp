#include "config.h"

Config::Config() : QSettings(ORGNAME, PROGNAME)
{
  langs.insert("English", "en_EN");
  langs.insert("Russian", "ru_RU");
  lang = value("lang", "English").toString();

  style = value("style").toString();
  styles = value("styles").toMap();

  if (styles.isEmpty()) {
    styles[":Calcutta"] = readFile(":/rc/siddhanta-calcutta.css");
    styles[":Siddhanta"] = readFile(":/rc/siddhanta.css");
  }

  if (styles.count() == 2 && !style.startsWith(':')) { // Или удален последний пользовательский стиль.
    style = ":Calcutta";
  }

  order = value("order").toMap();

  mainWindowGeometry = value("mainWindowGeometry").toByteArray();
  mainWindowState = value("mainWindowState").toByteArray();

  cardSize = value("cardSize", QSize(500, 500)).toSize();
}

Config::~Config()
{
  setValue("lang", lang);
  setValue("style", style);
  setValue("styles", styles);
  setValue("order", order);
  setValue("mainWindowGeometry", mainWindowGeometry);
  setValue("mainWindowState", mainWindowState);
  setValue("cardSize", cardSize);
}

QString Config::readFile(QString name)
{
  QFile file(name);
  file.open(QFile::ReadOnly);
  return file.readAll();
}
