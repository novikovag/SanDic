#include "card.h"

Card::Card(Card* card) : config(card->config)
{
  setWindowTitle(card->windowTitle());
//    setHtml(card->toHtml());
  setDocument(card->document()->clone());
  setReadOnly(true);
  resize(config->cardSize);
}

Card::Card(Config* config, QSqlDatabase* db)
    : config(config), db(db)
{
  QSqlQuery query("SELECT origin, abb, desc FROM dictAbbs ORDER BY length(abb) DESC", *db);

  while (query.next()) {
      abbs[query.value(0).toString()] << AbbItem(query.value(1).toString(), query.value(2).toString());
  }
  // teaser
  setHtml("<center><br><img src=':/rc/home.png'><h1>https://github.com/novikovag/sandic</h1></center>");
  setReadOnly(true);
}

void Card::resizeEvent(QResizeEvent* event)
{
  if (windowType()) { // Qt::Widget == 0 -> Main Card
    config->cardSize = event->size();
  }

  QTextEdit::resizeEvent(event);
}

void Card::article(QString word, QString search)
{
  setWindowTitle(word);

  QString txt = "<p class='header'>" + word + "</p>";
  QSqlQuery query("SELECT origin, hom, desc FROM dictEntries WHERE word MATCH '" + word + "' " +
                  config->originSuff + " ORDER BY origin, hom", *db);
  QMap<QString, QList<QStringList> > recs;

  while (query.next()) {
    recs[query.value(0).toString()] << query.value(2).toString().split("\n", Qt::SkipEmptyParts);
  }

  foreach (const QString& name, config->originKeys) {
    if (recs[name].count()) {
      txt += buildRec(name, recs[name]);
    }
  }

  setHtml(txt);

  if (search.isEmpty()) {
    return;
  }

  QRegularExpression rx;
  rx.setPatternOptions(QRegularExpression::CaseInsensitiveOption);

  if (search.startsWith(":RX:")) {
    rx.setPattern("(?:" + search.mid(4) + ")");
  } else {
    static QRegularExpression rx2("\\s+");
    rx.setPattern("\\b(?:" + search.replace(rx2, "|") + ")\\b");
  }

  QTextCursor cursor;
  QList<ExtraSelection> selections;
  QTextCharFormat format;
  //format.setForeground(Qt::black);
  format.setBackground(Qt::yellow);

  while (!(cursor = document()->find(rx, cursor, QTextDocument::FindCaseSensitively)).isNull()) {
    ExtraSelection selection;
    selection.format = format;
    selection.cursor = cursor;
    selections.append(selection);
  }

  if (selections.count()) {
    setExtraSelections(selections);
    cursor = selections[0].cursor;
    cursor.movePosition(QTextCursor::StartOfWord);
    setTextCursor(cursor);
    ensureCursorVisible();
  }
}

QString Card::buildRec(QString origin, QList<QStringList> desc)
{
  QString txt = "<p class='name'>" + origin + "</p>";

  if (desc.count() == 1) {
    txt += buildList(desc[0]);
  } else {
    for (int i = 0; i < desc.count(); i++) {
        txt += "<p class='count'>" + QString("%1").arg(i + 1) + "</p>" + buildList(desc[i]);
    }
  }

  for (int i = 0; i < abbs[origin].count(); i++) {
    txt = txt.replace(abbs[origin][i].rx, "\\1<strong class='abbs' title='" +
                                           abbs[origin][i].desc + "'>\\2</strong>\\3");
  }

  return txt + "<br>";
}

QString Card::buildList(QStringList desc)
{
  static QRegularExpression rx("([\u0900-\u097f]+)");
  QString txt;

  for (int i = 0; i < desc.count(); i++) {
    txt += "<p class='item'><span class='bull'>•</span><code>   </code>" +
            desc[i].replace(rx, "<strong class='deva'>\\1</strong>") + "</p>";
  }

  return txt;
}
