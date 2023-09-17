#include "mainwindow.h"

MainWindow::MainWindow(Config* config, QSqlDatabase* db)
   : config(config), db(db)
{
  QSqlQuery query("SELECT origin, abb, desc FROM dictAbbs ORDER BY abb", *db);
  QMap<QString, QList<QStringList> > abbsMap;

  while (query.next()) {
    QStringList item;
    item << query.value(1).toString() << query.value(2).toString();
    abbsMap[query.value(0).toString()] << item;
  }

  QString txt = "<table class='abbs' width=100%>";
  int nc = 4; // Столбцов в таблице.

  foreach (const QString& origin, abbsMap.keys()) {
    int nr = qCeil(abbsMap[origin].count() / nc);

    txt += "<tr><th colspan='" + QString("%1").arg(nc * 2) + "'>" + origin + "</th></tr>";

    for (int i = 0; i < nr; i++) {
      txt +=  "<tr>";

      for (int j = 0; j < nc; j++) {
        int c = i + nr * j;

        if (c >= abbsMap[origin].count()) {
          break;
        }

        txt += "<td class='abb'>" + abbsMap[origin][c][0] + "</td><td>" + abbsMap[origin][c][1] + "</td>";
      }

      txt += "</tr>";
    }
  }

  html << txt + "</table>";
  html << config->readFile(":/rc/map.html");

  mainCard = new Card(config, db);
  orderDialog = new OrderDialog(config, db);
  abbs = new QTextEdit;
  abbs->setReadOnly(true);
  transMap = new QTextEdit;
  transMap->setReadOnly(true);

  MainWidget* mainWidget = new MainWidget(config, db);
  Trans* hk     = new Trans(Trans::HK);
  Trans* itrans = new Trans(Trans::ITRANS);
  Trans* slp    = new Trans(Trans::SLP1);
  DevaPicker* devaPicker = new DevaPicker;

  QMenu* menu = menuBar()->addMenu(tr("&Tools"));
  QMenu* subMenu = menu->addMenu(tr("&Language"));
  QActionGroup* actionGroup = new QActionGroup(this);
  QAction* action;

  foreach (const QString& lang, config->langs.keys()) {
    action = subMenu->addAction(lang, this, SLOT(changeLang()));
    action->setCheckable(true);
    action->setActionGroup(actionGroup);

    if (config->lang == lang) {
      action->setChecked(true);
    }
  }

  stylesGroup = new QActionGroup(this);
  stylesMenu = subMenu = menu->addMenu(tr("&Styles"));
  subMenu->addAction(tr("&Load style..."), this, SLOT(loadStyle()));
  subMenu->addSeparator();

  foreach (const QString& style, config->styles.keys()) {
    QAction* action = styleAction(style);

    if (style == config->style) {
      action->trigger();
    }
  }

  stylesMenu->installEventFilter(this);

  action = menu->addAction(tr("&Main card"), QKeySequence::Open, this, SLOT(showDock()));
  addDock(mainCard, action, tr("Main Card"), Qt::RightDockWidgetArea);
  menu->addAction(tr("Or&der..."), this, SLOT(order()));
  menu->addSeparator();
  menu->addAction(tr("&Quit"), QKeySequence::Close, this, SLOT(close()));

  menu = menuBar()->addMenu(tr("Tran&sliteration"));
  action = menu->addAction(tr("&Harvard-Kyoto"), Qt::CTRL | Qt::Key_H, this, SLOT(showDock()));
  addDock(hk, action, tr("Harvard-Kyoto"));
  action = menu->addAction(tr("I&TRANS 5.3"), Qt::CTRL | Qt::Key_T, this, SLOT(showDock()));
  addDock(itrans, action, tr("ITRANS 5.3"));
  action = menu->addAction(tr("&SLP1"), Qt::CTRL | Qt::Key_S, this, SLOT(showDock()));
  addDock(slp, action, tr("SLP1"));
  action = menu->addAction(tr("&DevaPicker"), Qt::CTRL | Qt::Key_D, this, SLOT(showDock()));
  addDock(devaPicker, action, tr("Devanagari character picker"));
  menu = menuBar()->addMenu(tr("&Help"));
  action = menu->addAction(tr("&Abbreviations"), Qt::Key_F2, this, SLOT(showDock()));
  addDock(abbs, action, tr("Abbreviations"));
  action = menu->addAction(tr("&TransMap"), Qt::Key_F3, this, SLOT(showDock()));
  addDock(transMap, action, tr("Transliteration map"));
  menu->addAction(tr("A&bout..."), this, SLOT(about()));

  setCentralWidget(mainWidget);
  setWindowTitle(PROGNAME);
  restoreGeometry(config->mainWindowGeometry);
  restoreState(config->mainWindowState);

  connect(mainWidget, SIGNAL(wordChanged(QString, QString)), mainCard, SLOT(article(QString, QString)));
  connect(mainWidget, SIGNAL(wordClicked(QString)), SLOT(wordClicked(QString)));

  CONTRANS(hk);
  CONTRANS(itrans);
  CONTRANS(slp);
  connect(devaPicker, SIGNAL(UTF8(QChar)), mainWidget, SLOT(addDevaChar(QChar)));
}

void MainWindow::closeEvent(QCloseEvent*)
{
  config->mainWindowGeometry = saveGeometry();
  config->mainWindowState    = saveState();

  foreach (QWidget* widget, QApplication::topLevelWidgets()) {
    widget->close();
  }
}

void MainWindow::addDock(QWidget* widget, QAction* action, QString title, Qt::DockWidgetArea area)
{
  QDockWidget* dock = new QDockWidget(title, this);
  action->setData(QVariant::fromValue(reinterpret_cast<void *>(dock)));
  dock->setObjectName(QString("dock%1").arg(docks.count()));
  dock->setWidget(widget);

  if (config->mainWindowState.isEmpty()) {
    dock->setHidden(true);
  }

  addDockWidget(area, dock);
  connect(dock, SIGNAL(visibilityChanged(bool)), SLOT(showDock()));
  docks[dock->objectName()] = action;
  dock->setVisible(true);
  dock->setEnabled(true);
}

void MainWindow::showDock()
{
  if (dynamic_cast<QAction*>(sender())) {
    bool visible = reinterpret_cast<QDockWidget*>(static_cast<QAction*>(sender())->data().value<void *>())->isVisible();
    reinterpret_cast<QDockWidget*>(static_cast<QAction*>(sender())->data().value<void *>())->setVisible(!visible);
  } else {
    QDockWidget* dock = static_cast<QDockWidget*>(sender());
    docks[dock->objectName()]->setEnabled(true);
  }
}

void MainWindow::wordClicked(QString word)
{
  foreach (QWidget* widget, QApplication::topLevelWidgets()) {
    if (widget->windowTitle() == word) {
      widget->showNormal();
      widget->activateWindow();
      return;
    }
  }

  Card* card = new Card(mainCard);
  card->show();
}

void MainWindow::changeLang()
{
  config->lang = static_cast<QAction*>(sender())->text();
  QMessageBox::information(this, tr("Restart required"), tr("The language change will take effect after a restart program"));
}

void MainWindow::loadStyle()
{
  QString name = QFileDialog::getOpenFileName(this, "", "", tr("Style files (*.css);;All types (*)"));

  if (name.isEmpty()) {
    return;
  }

  QString base = QFileInfo(name).baseName();
  bool isNull = config->styles[base].isNull();
  config->styles[base] = config->readFile(name);

  if (isNull) {
    styleAction(base)->trigger();
  } else {
    foreach (QAction* action, stylesGroup->actions()) {
      if (action->text() == base) {
        action->trigger();
        break;
      }
    }
  }
}

void MainWindow::setStyle()
{
  QAction* action = static_cast<QAction*>(sender());
  config->style = action->text();
  QString style = config->styles[action->text()].toString();
  qApp->setStyleSheet(style);
  mainCard->document()->setDefaultStyleSheet(style);
  // Не запускать при старте.
  if (!mainCard->windowTitle().isEmpty()) {
    mainCard->article(mainCard->windowTitle());
  }

  abbs->document()->setDefaultStyleSheet(style);
  abbs->setHtml(html[0]);
  transMap->document()->setDefaultStyleSheet(style);
  transMap->setHtml(html[1]);
}

QAction* MainWindow::styleAction(QString name)
{
  QAction* action = stylesMenu->addAction(name, this, SLOT(setStyle()));
  action->setCheckable(true);
  action->setActionGroup(stylesGroup);
  return action;
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
  if (event->type() == QEvent::MouseButtonRelease) {
    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

    if (mouseEvent->button() != Qt::RightButton) {
      goto SKIP;
    }

    QAction* action = static_cast<QMenu*>(obj)->actionAt(mouseEvent->pos());

    if (action->text().startsWith(":")) {
      goto SKIP;
    }

    config->styles.remove(action->text());
    delete action;
  }
SKIP:
  return QObject::eventFilter(obj, event);
}

void MainWindow::about()
{
  QString txt = tr("<h3>%1 v%2</h3>"
                   "Based on Qt v%3<br>"
                   "Built on %4 at %5<br>"
                   "Devanagari font: <a href='http://svayambhava.blogspot.com/p/"
                   "siddhanta-devanagariunicode-open-type.html'>Siddhanta</a><br>"
                   "Author: <a href='mailto:feedback@sandic.ru?subject=%1'>"
                       "Novikov Artem Gennadievich</a><br>"
                   "Project page: "
                       "<a href='https://github.com/novikovag/sandic'>"
                       "https://github.com/novikovag/sandic</a><br>"
                   "License: <a href='http://www.gnu.org/licenses/gpl.html'>"
                       "GNU GPL v3</a>").arg(PROGNAME, VERSION, QT_VERSION_STR, __DATE__, __TIME__);

  QSqlQuery query("SELECT author, desc, uri, words, "
                  "(SELECT words FROM dictStats WHERE origin = '-') "
                  "FROM dictOrigins INNER JOIN dictStats ON name = origin ORDER BY author", *db);

  while (query.next()) {
    txt += "<hr><strong>" + query.value(0).toString() + "</strong><br>" + tr("<i>words: %1/%2</i><br>").arg(query.value(3).toInt()).arg(query.value(4).toInt()) +
           query.value(1).toString() + QString("<br><a href='%1'>%1</a>").arg(query.value(2).toString());
  }

  QMessageBox::about(this, tr("About \"%1\"").arg(PROGNAME), txt + "<br>");
}
