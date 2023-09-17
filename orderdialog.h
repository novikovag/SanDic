#ifndef ORDERDIALOG_H
#define ORDERDIALOG_H

#include <QtSql>
#include <QtWidgets>

#include "config.h"
#include "ui_orderdialog.h"

class OrderDialog : public QDialog, private Ui::OrderDialog
{
  Q_OBJECT
  
public:
  OrderDialog(Config*, QSqlDatabase*);

private:
  void closeEvent(QCloseEvent*);
  void addItem(QString, bool = true);
  void setOrigins();

  Config* config;
  QSqlDatabase* db;
};

#endif // ORDERDIALOG_H
