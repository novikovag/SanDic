#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtSql>
#include <QtWidgets>

#include "config.h"
#include "card.h"
#include "mainwidget.h"
#include "trans.h"
#include "orderdialog.h"

#define CONTRANS(t) connect(t, SIGNAL(UTF8(QString)),   mainWidget, SLOT(setDevaText(QString))); \
                    connect(t, SIGNAL(returnPressed()), mainWidget, SLOT(search()))

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(Config*, QSqlDatabase*);

private slots:
  void showDock();
  void wordClicked(QString);
  void changeLang();
  void order() { orderDialog->exec(); }
  void about();
  void loadStyle();
  void setStyle();

private:
  void addDock(QWidget*, QAction*, QString, Qt::DockWidgetArea = Qt::BottomDockWidgetArea);
  void closeEvent(QCloseEvent*);
  bool eventFilter(QObject*, QEvent*);
  QAction* styleAction(QString);

  QHash<QString, QAction*> docks;
  Config* config;
  QSqlDatabase* db;
  Card* mainCard;
  OrderDialog* orderDialog;
  QTextEdit* abbs;
  QTextEdit* transMap;
  QStringList html; // [0] abbs [1] map
  QActionGroup* stylesGroup;
  QMenu* stylesMenu;
};

#endif // MAINWINDOW_H
