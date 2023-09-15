## Sanskrit-English Dictionary

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

N.B.
SanDic executable file and extracted database file (sandic.db) must be in the same directory.

## How to replace Siddhanta-Calcutta fonts with Siddhanta fonts
Download fonts from:  https://sites.google.com/site/bayaryn/siddhanta.ttf?attredirects=0
```
git clone https://github.com/novikovag/SanDic.git; cd SanDic
mv siddhanta.ttf rc/
sed -i '' 's/siddhanta-calcutta/siddhanta/g' main.cpp
```
Then rebuild with the following steps

## How to build on MacOS
```
brew install qt5
brew link qt5 --force
git clone https://github.com/novikovag/SanDic.git
cd SanDic
qmake -o Makefile SanDic.pro
make
make clean
mv sandic.app /Applications
cd db; unzip sandic.db.zip; unzip yukta.db.zip
```

![Alt text](/misc/imgs/1.jpg?raw=true "Screen1")
![Alt text](/misc/imgs/2.jpg?raw=true "Screen2")
![Alt text](/misc/imgs/3.jpg?raw=true "Screen3")

