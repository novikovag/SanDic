## Sanskrit-English Dictionary v2.7

- Qt v6+ only (tested on Qt v6.5.2).
- Two embeded [Devanagary fonsts](href='http://svayambhava.blogspot.com/p/) - Siddhanta and Siddhanta-Calcutta.
- Supprot user defined widget styles.

Electronic version based on:

Apte Vaman Shivaram
Revised and enlarged edition of Prin. V. S. Apte's The practical Sanskrit-English dictionary. Poona: Prasad Prakashan, 1957-1959. 3v.

Arthur Anthony Macdonell
A practical Sanskrit dictionary with transliteration, accentuation, and etymological analysis throughout. London: Oxford University Press, 1929.

Maṇḍala Pati dāsa
List of verbal roots with final forms
http://sourceforge.net/p/dhatu-patha

Monier Williams
Sanskrit-English Dictionary

![Alt text](/misc/imgs/1.jpg?raw=true "Screen1")

## How to build on MacOS

```
brew install qt
brew link qt --force
git clone https://github.com/novikovag/SanDic.git
cd SanDic
qmake -o Makefile SanDic.pro
make
make clean
mv sandic.app /Applications
cd db
unzip sandic.db.zip
```

## How to build on Ubuntu Linux

```
sudo apt install qt6-base-dev qtchooser
qtchooser -install qt6 $(which qmake6)
sudo mv ~/.config/qtchooser/qt6.conf /usr/share/qtchooser/qt6.conf
sudo mkdir -p /usr/lib/$(uname -p)-linux-gnu/qt-default/qtchooser
sudo ln -n /usr/share/qtchooser/qt6.conf /usr/lib/$(uname -p)-linux-gnu/qt-default/qtchooser/default.conf

mkdir build
cd build
git clone https://github.com/novikovag/SanDic.git
cd SanDic
qmake -o Makefile SanDic.pro
make
make clean
mkdir ~/SanDic
cp sandic ~/SanDic/sandic
unzip ./db/sandic.db.zip -d ~/SanDic
```

* SanDic executable file and extracted database file (sandic.db) must be in the same directory.

## User defined widget styles

* Two styles begin with ':' are predefined.
* Rename and modify file `SanDic/rc/siddhanta-calcutta.css`. 
* Select new style `Tools->Styles->Load style...`.
* Right mouse button delete user style.
