#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtSql>
#include <QtWidgets>

#include "config.h"
#include "ui_mainwidget.h"

#define GETWORD(i) i.sibling(i.row(), 1).isValid() ? i.sibling(i.row(), 0).data().toString() : i.data().toString()

class ItemDelegate : public QItemDelegate
{
public:
  void paint(QPainter*, const QStyleOptionViewItem&, const QModelIndex&) const;
};

class MainWidget : public QWidget, private Ui::MainWidget
{
  Q_OBJECT

public:
  MainWidget(Config*, QSqlDatabase*);

signals:
  void wordChanged(QString, QString);
  void wordClicked(QString);

private slots:
  void addDevaChar(QChar ch)    { tabWidget->currentIndex() ? searchCombo->lineEdit()->insert(ch) :
                                                              indexCombo->lineEdit()->insert(ch); }
  void setDevaText(QString txt) { tabWidget->currentIndex() ? searchCombo->lineEdit()->setText(txt) :
                                                              indexCombo->lineEdit()->setText(txt); }
  void rowChanged(const QModelIndex&, const QModelIndex&);
  void clicked(QModelIndex index);
  void doubleClicked(QModelIndex index);
  void editTextChanged(const QString&);
  void search();

private:
  bool eventFilter(QObject*, QEvent*);

  Config* config;
  QSqlDatabase* db;
};

#endif // MAINWIDGET_H
