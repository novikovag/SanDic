#include "orderdialog.h"
#include "ui_orderdialog.h"

OrderDialog::OrderDialog(Config* config, QSqlDatabase* db)
    : config(config), db(db)
{
  setupUi(this);
  QSqlQuery query("SELECT name FROM dictOrigins", *db);
  QStringList names;

  while (query.next()) {
    names << query.value(0).toString();
  }

  foreach (const QString& name, config->order[db->databaseName()].toStringList()) {
    if (name.startsWith(":H:")) {
      addItem(name.mid(3), false);
      names.removeAt(names.indexOf(name.mid(3)));
      continue;
    }

    addItem(name);
    names.removeAt(names.indexOf(name));
  }

  foreach (const QString& name, names) {
    addItem(name);
  }

  setOrigins();
  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

void OrderDialog::addItem(QString name, bool checked)
{
  QListWidgetItem* item = new QListWidgetItem(name);
  item->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
  nameList->addItem(item);
}

void OrderDialog::setOrigins()
{
  config->originKeys.clear();
  config->originSuff = "";

  for (int i = 0; i < nameList->count(); i++) {
    if (nameList->item(i)->checkState() == Qt::Checked) {
      if (config->originSuff.isEmpty()) {
        config->originSuff = " AND (";
      } else {
        config->originSuff += " OR ";
      }

      config->originSuff += "origin = '" + nameList->item(i)->text() + "'";
      config->originKeys << nameList->item(i)->text();
    }
  }

  config->originSuff += ") ";
}

void OrderDialog::closeEvent(QCloseEvent* /*event*/)
{
  config->order[db->databaseName()].clear();
  QStringList names;

  for (int i = 0; i < nameList->count(); i++) {
    names << (nameList->item(i)->checkState() == Qt::Checked ? "" : ":H:") + nameList->item(i)->text();
  }

  config->order[db->databaseName()] = names;
  setOrigins();
}
