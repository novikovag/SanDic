#include "mainwidget.h"

void ItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  if (option.state & QStyle::State_Selected) {
    painter->fillRect(option.rect, option.palette.highlight());
  }

  QTextDocument doc;
  doc.setDefaultStyleSheet(qApp->styleSheet());

  static QRegularExpression rx("([\u0900-\u097f]+)");
  doc.setHtml("<span class='result'>" + index.data(Qt::DisplayRole).toString().replace(rx, "<span class='deva'>\\1</span>") + "</span>");

  painter->translate(option.rect.topLeft());
  doc.drawContents(painter);
  painter->translate(-option.rect.topLeft());
}

MainWidget::MainWidget(Config* config, QSqlDatabase* db)
    : config(config), db(db)
{
  setupUi(this);
  searchCombo->installEventFilter(this);

  QSqlQueryModel* model = new QSqlQueryModel;
  model->setQuery("SELECT DISTINCT word FROM dictEntries ORDER BY word ASC");

  indexList->setUniformItemSizes(true);
  indexList->setModel(model);
  // Для поиска при наборе нужны все слова.
  while (model->canFetchMore()) {
    model->fetchMore();
  }

  searchTable->setModel(new QSqlQueryModel);
  searchTable->setItemDelegate(new ItemDelegate);
  searchTable->horizontalHeader()->setStretchLastSection(true);
  searchTable->setGridStyle(Qt::NoPen);

  connect(indexList->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), SLOT(rowChanged(const QModelIndex&, const QModelIndex&)));
  connect(indexList, SIGNAL(doubleClicked(QModelIndex)), SLOT(doubleClicked(QModelIndex)));
  connect(indexList, SIGNAL(clicked(QModelIndex)), SLOT(clicked(QModelIndex)));
  connect(indexCombo, SIGNAL(editTextChanged(const QString&)), SLOT(editTextChanged(const QString&)));
  connect(searchTable->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)), SLOT(rowChanged(const QModelIndex&, const QModelIndex&)));
  connect(searchTable, SIGNAL(doubleClicked(QModelIndex)), SLOT(doubleClicked(QModelIndex)));
  connect(searchButton,SIGNAL(clicked()), SLOT(search()));

  rxCheck->setChecked(false);
  rxCheck->setVisible(true);
}

void MainWidget::rowChanged(const QModelIndex& current, const QModelIndex& previous)
{
  if (current.row() == -1) {
    return;
  }

  QString word = GETWORD(current);

  if (word == (GETWORD(previous))) {
    return;
  }

  emit wordChanged(word, tabWidget->currentIndex() ?
                         ((rxCheck->isChecked() ? ":RX:" : "") + searchCombo->itemText(searchCombo->currentIndex())) : "");
}

void MainWidget::clicked(QModelIndex index)
{
  indexCombo->blockSignals(true);
  indexCombo->setEditText(index.data().toString());
  indexCombo->blockSignals(false);
}

void MainWidget::doubleClicked(QModelIndex index)
{
  if (index.sibling(index.row(), 1).isValid()) {
    emit wordClicked(index.sibling(index.row(), 0).data().toString());
  } else {

    if (indexCombo->findText(index.data().toString()) == -1) {
      indexCombo->insertItem(0, index.data().toString());
    }

    emit wordClicked(index.data().toString());
  }
}

void MainWidget::editTextChanged(const QString& txt)
{
  QModelIndexList indices = indexList->model()->match(indexList->model()->index(0, 0), Qt::DisplayRole, txt);

  if (!indices.isEmpty()) {
    indexList->setCurrentIndex(QModelIndex(indices.first()));
  }
}

void MainWidget::search()
{
  // Запрос на поиск поступает от транса, проверка текущей вкладки на 1.
  if (!tabWidget->currentIndex()) {
    return;
  }

  QString txt = searchCombo->currentText().trimmed();
  // Небольшая проверка регулярки.
  if (rxCheck->isChecked()) {
    while (txt.startsWith("^*") || txt.startsWith("*")) {
      txt.remove(0, 1);
    }
  }

  if (txt.isEmpty()) {
    return;
  }

  QApplication::setOverrideCursor(Qt::WaitCursor);

  static_cast<QSqlQueryModel*>(searchTable->model())
      ->setQuery(rxCheck->isChecked()
                     ? QString("SELECT word FROM dictEntries WHERE word REGEXP '%1' UNION "
                               "SELECT word FROM dictEntries WHERE desc REGEXP '%1' %2 GROUP BY "
                               "word ORDER BY word")
                           .arg(txt, config->originSuff)
                     : QString("SELECT DISTINCT(word), quote(word) FROM dictEntries WHERE word "
                               "MATCH '%1' UNION "
                               "SELECT word, snippet(dictEntries, '<strong>', '</strong>', "
                               "'<em>...</em>', -1, 15) "
                               "FROM dictEntries WHERE desc MATCH '%1' %2 ORDER BY word")
                           .arg(txt, config->originSuff));

  searchTable->resizeColumnToContents(0);
  QApplication::restoreOverrideCursor();

  if (searchTable->model()->rowCount()) {
    int index = searchCombo->findText(txt, Qt::MatchFixedString);

    if (index == -1) {
      searchCombo->insertItem(0, txt);
      searchCombo->setCurrentIndex(0);
    } else {
      searchCombo->setItemText(index, txt); // если различаются регистром
      searchCombo->setCurrentIndex(index);
    }
  }
}

bool MainWidget::eventFilter(QObject* obj, QEvent* event)
{
  if (event->type() == QEvent::KeyPress && static_cast<QKeyEvent*>(event)->key() == Qt::Key_Return) {
    search();
  }

  return QObject::eventFilter(obj, event);
}
