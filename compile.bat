setup.py install
move ".\build\lib.win32-2.7\winauto.pyd" ".\winauto.pyd"
@RD /S /Q ".\build"
main.py
PAUSE