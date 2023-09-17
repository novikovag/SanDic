#ifndef TRANS_H
#define TRANS_H

#include <QtWidgets>

// =============================================================================
//   CODE    NAME                     Harvard-Kyoto     ITRANS 5.3     SLP1
// =============================================================================
static QString vowelsIndependent[][6] = {
  {"0905", "letter a",              "a",              "a",           "a"},
  {"0906", "letter aa",             "A",              "A/aa",        "A"},
  {"0907", "letter i",              "i",              "i",           "i"},
  {"0908", "letter ii",             "I",              "I/ii",        "I"},
  {"0909", "letter u",              "u",              "u",           "u"},
  {"090a", "letter uu",             "U",              "U/uu",        "U"},
  {"090b", "letter vocalic r",      "R",              "RRi/R^i",     "f"},
  {"0960", "letter vocalic rr",     "RR",             "RRI/R^I",     "F"},
  {"090c", "letter vocalic l",      "IR",             "LLi/L^i",     "x"},
  {"0961", "letter vocalic ll",     "IRR",            "LLI/L^I",     "X"},
  {"090f", "letter e",              "e",              "e",           "e"},
  {"0910", "letter ai",             "ai",             "ai",          "E"},
  {"0913", "letter o",              "o",              "o",           "o"},
  {"0914", "letter au",             "au",             "au",          "O"}};
static QString vowelsDependent[][6] = {
  {"093e", "vowel sign aa",         "A",              "A/aa",        "A"},
  {"093f", "vowel sign i",          "i",              "i",           "i"},
  {"0940", "vowel sign ii",         "I",              "I/ii",        "I"},
  {"0941", "vowel sign u",          "u",              "u",           "u"},
  {"0942", "vowel sign uu",         "U",              "U/uu",        "U"},
  {"0943", "vowel sign vocalic r",  "R",              "RRi/R^i",     "f"},
  {"0944", "vowel sign vocalic rr", "RR",             "RRI/R^I",     "F"},
  {"0947", "vowel sign e",          "e",              "e",           "e"},
  {"0948", "vowel sign ai",         "ai",             "ai",          "E"},
  {"094b", "vowel sign o",          "o",              "o",           "o"},
  {"094c", "vowel sign au",         "au",             "au",          "O"}};
static QString consonants[][6] = {
  {"0915", "letter ka",             "k",              "k",           "k"},
  {"0916", "letter kha",            "kh",             "kh",          "K"},
  {"0917", "letter ga",             "g",              "g",           "g"},
  {"0918", "letter gha",            "gh",             "gh",          "G"},
  {"0919", "letter nga",            "G",              "~N",          "N"},
  {"091a", "letter ca",             "c",              "ch",          "c"},
  {"091b", "letter cha",            "ch",             "Ch",          "C"},
  {"091c", "letter ja",             "j",              "j",           "j"},
  {"091d", "letter jha",            "jh",             "jh",          "J"},
  {"091e", "letter nya",            "J",              "~n",          "Y"},
  {"091f", "letter tta",            "T",              "T",           "w"},
  {"0920", "letter ttha",           "Th",             "Th",          "W"},
  {"0921", "letter dda",            "D",              "D",           "q"},
  {"0922", "letter ddha",           "Dh",             "Dh",          "Q"},
  {"0923", "letter nna",            "N",              "N",           "R"},
  {"0924", "letter ta",             "t",              "t",           "t"},
  {"0925", "letter tha",            "th",             "th",          "T"},
  {"0926", "letter da",             "d",              "d",           "d"},
  {"0927", "letter dha",            "dh",             "dh",          "D"},
  {"0928", "letter na",             "n",              "n",           "n"},
  {"092a", "letter pa",             "p",              "p",           "p"},
  {"092b", "letter pha",            "ph",             "ph",          "P"},
  {"092c", "letter ba",             "b",              "b",           "b"},
  {"092d", "letter bha",            "bh",             "bh",          "B"},
  {"092e", "letter ma",             "m",              "m",           "m"},
  {"092f", "letter ya",             "y",              "y",           "y"},
  {"0930", "letter ra",             "r",              "r",           "r"},
  {"0932", "letter la",             "l",              "l",           "l"},
  {"0935", "letter va",             "v",              "v/w",         "v"},
  {"0936", "letter sha",            "z",              "sh",          "S"},
  {"0937", "letter ssa",            "S",              "Sh",          "z"},
  {"0938", "letter sa",             "s",              "s",           "s"},
  {"0939", "letter ha",             "h",              "h",           "h"}};
static QString miscellaneous[][6] = {
  {"0902", "sign anusvara",         "M",              "M/.n/.m",     "M"},
  {"0903", "sign visarga",          "H",              "H/.h",        "H"},
  {"093d", "sign avagraha",         "'",              ".a",          "'"},
  {"094d", "sign virama",           "",       	     "",            ""},
  {"0950", "om",                    "oM/o.m",         "OM/AUM",      "oM"},
  {"0964", "danda",                 "|",              "|",           "."},
  {"0965", "double danda",          "||",             "||",          ".."}};
static QString numbers[][6] = {
  {"0966", "digit zero",            "0",              "0",           "0"},
  {"0967", "digit one",             "1",              "1",           "1"},
  {"0968", "digit two",             "2",              "2",           "2"},
  {"0969", "digit three",           "3",              "3",           "3"},
  {"096a", "digit four",            "4",              "4",           "4"},
  {"096b", "digit five",            "5",              "5",           "5"},
  {"096c", "digit six",             "6",              "6",           "6"},
  {"096d", "digit seven",           "7",              "7",           "7"},
  {"096e", "digit eight",           "8",              "8",           "8"},
  {"096f", "digit nine",            "9",              "9",           "9"}};

#define SIZE(s) sizeof(s) / 6 / sizeof(QString)

struct MapItem
{
  MapItem(QString = "", QString = "");
  bool operator<(const MapItem& i) const { return name.length() > i.name.length(); }

  QChar ch;
  QString name;
};


class Trans : public QLineEdit
{
  Q_OBJECT

public:
  enum Scheme { HK = 2, ITRANS, SLP1 };

  Trans(Scheme);

signals:
  void UTF8(QString);

private slots:
  void toUTF8(const QString);

private:
  QList<MapItem> buildMap(Scheme, QString [][6], int);
  QList<MapItem> map[5];
};

class DevaPicker : public QWidget
{
  Q_OBJECT

public:
  DevaPicker();

signals:
  void UTF8(QChar);

private slots:
   void clicked() { emit UTF8(static_cast<QPushButton*>(sender())->text()[0]); }

private:
  QGridLayout* buildLayout(QString group[][6], int size, int columns);
};

#endif // TRANS_H
