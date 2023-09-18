#include "config.h"
#include "mainwindow.h"

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  QFontDatabase::addApplicationFont(":/rc/siddhanta-calcutta.ttf");
  QFontDatabase::addApplicationFont(":/rc/siddhanta.ttf");

  Config config;

  QString locale = config.langs.value(config.lang);

  QTranslator trans1;
  std::ignore = trans1.load(":/rc/sandic_" + locale);
  app.installTranslator(&trans1);

  QTranslator trans2;
  std::ignore = trans2.load(":/rc/qt_" + locale);
  app.installTranslator(&trans2);

  QDir bin(QCoreApplication::applicationDirPath());

  #ifdef Q_OS_MACOS
    // Для перехода в текущий каталог нужно выйти из .app бандла.
    bin.cdUp();
    bin.cdUp();
    bin.cdUp();
  #endif

  QString dbName = bin.absolutePath().append("/").append(DBNAME);
  QStringList argvList = QCoreApplication::arguments();

  for (int i = 0; i < argvList.count(); i++) {
    if (argvList[i] == "db") {
      dbName = argvList[i + 1];
      break;
    }
  }

  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  db.setConnectOptions("QSQLITE_OPEN_READONLY;QSQLITE_ENABLE_REGEXP");
  db.setDatabaseName(dbName);

  if (!QFile::exists(dbName) || !db.open()) {
    QMessageBox::critical(0, QObject::tr("Database error"), QObject::tr("Can't open database file: %1").arg(dbName));
    exit(-1);
  }

  QSqlQuery query("PRAGMA cache_size = -100000", db);
  query.next();

  QSplashScreen splash(QPixmap(QString(":/rc/splash_%1.png").arg(locale)));
  splash.show();

  MainWindow window(&config, &db);
  window.show();
  
  splash.finish(&window);

  return app.exec();
}
