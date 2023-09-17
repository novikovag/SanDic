#include "trans.h"

MapItem::MapItem(QString code, QString cname)
{
  name = QRegularExpression::escape(cname);
  bool ok;
  ch = QChar(code.toInt(&ok, 16));
}

Trans::Trans(Scheme scheme)
{
  map[0] = buildMap(scheme, vowelsIndependent, SIZE(vowelsIndependent));
  map[1] = buildMap(scheme, vowelsDependent, SIZE(vowelsDependent));
  map[2] = buildMap(scheme, consonants, SIZE(consonants));
  map[3] = buildMap(scheme, miscellaneous, SIZE(miscellaneous));
  map[4] = buildMap(scheme, numbers, SIZE(numbers));

  connect(this, SIGNAL(textChanged(const QString)), SLOT(toUTF8(const QString)));
}

QList<MapItem> Trans::buildMap(Scheme scheme, QString group[][6], int size)
{
  QList<MapItem> map;

  for (int i = 0; i < size; i++) {
    if (group[i][scheme].isEmpty()) {
      continue;
    }

    QStringList cnames = group[i][scheme].split("/");

    for (int c = 0; c < cnames.length(); c++) {
      map << MapItem(group[i][0], cnames[c]);
    }
  }

  std::sort(map.begin(), map.end());
  return map;
}

void Trans::toUTF8(const QString edit)
{
  QString txt = edit;

  for (int i = 0; i < map[4].length(); i++) {
    txt = txt.replace(QRegularExpression(map[4][i].name), map[4][i].ch);
  }
  for (int i = 0; i < map[3].length(); i++) {
    txt = txt.replace(QRegularExpression(map[3][i].name), map[3][i].ch);
  }
  for (int i = 0; i < map[2].length(); i++) {
    txt = txt.replace(QRegularExpression(map[2][i].name), QString("%1%2").arg(map[2][i].ch, QChar(0x094d)));
  }
  for (int i = 0; i < map[1].length(); i++) {
    txt = txt.replace(QRegularExpression(QString("([\\x0915-\\x0939])\\x094d%1").arg(map[1][i].name)), QString("\\1%1").arg(map[1][i].ch));
  }
  for (int i = 0; i < map[0].length(); i++) {
    txt = txt.replace(QRegularExpression(map[0][i].name), map[0][i].ch);
  }

  txt.replace(QRegularExpression("([\\x0915-\\x0939])\\x094d\\x0905"), "\\1");
  emit UTF8(txt);
}

DevaPicker::DevaPicker()
{
  QVBoxLayout* layout = new QVBoxLayout;
  layout->addLayout(buildLayout(vowelsIndependent, SIZE(vowelsIndependent), 7));
  layout->addLayout(buildLayout(vowelsDependent, SIZE(vowelsDependent), 7));
  layout->addLayout(buildLayout(consonants, SIZE(consonants), 5));
  layout->addLayout(buildLayout(miscellaneous, SIZE(miscellaneous), 7));
  layout->addLayout(buildLayout(numbers, SIZE(numbers), 10));
  setLayout(layout);
}

QGridLayout* DevaPicker::buildLayout(QString group[][6], int size, int columns)
{
  QGridLayout* layout = new QGridLayout;

  int row    = 0;
  int column = 0;
  bool ok;

  for (int i = 0; i < size; i++) {
    QPushButton* button = new QPushButton(QChar(group[i][0].toInt(&ok, 16)));
    button->setToolTip(group[i][1]);
    connect(button, SIGNAL(clicked()), SLOT(clicked()));
    layout->addWidget(button, row++ / columns, column++ % columns);
  }

  return layout;
}
