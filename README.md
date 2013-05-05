TUI
===

To Run this TUI on your system please follow the following steps-

1) Clone the repo on your system.

2) Downloading dependencies

a) Download the Python 2.7 source tarball and extract it.
It can be accessed from here- http://www.python.org/download/releases/2.7/

then

./configure
make
make install

b) Download and install ncurses.This can be done by-
sudo apt-get install ncurses-dev


3) Change the path of Python.h in input_provider.h 
according to the location of Python.h on your system.

Also change the path of input_provider.h in TUI.c

4) Add to PYTHONPATH the location at which you have cloned this repo.
This can be done by writing this on terminal-
export PYTHONPATH=$PYTHONPATH:/my/other/path

5) Run TUI-
gcc TUI.c -lpython2.7 -lncurses -lform -lmenu

p.s.
Though unlikely but, I might have forgotten some steps.
In case you face some issue please contact me at-

gmail- shubhamsomani92@gmail.com

Freenode IRC- S_Somani