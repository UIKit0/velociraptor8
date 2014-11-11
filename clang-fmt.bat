@rem This requires clang to be installed and in %PATH%
@rem http://llvm.org/releases/download.html

@cd src

@rem for /f "delims=|" %%f in ('dir /b *.cpp *.h') do @echo %%f

for /f "delims=|" %%f in ('dir /b *.cpp *.h') do clang-format -style="{BasedOnStyle: Mozilla, IndentWidth: 4, ColumnLimit: 100}" -i %%f
@rem clang-format -style="{BasedOnStyle: Mozilla, IndentWidth: 4, ColumnLimit: 100}" -i *.cpp *.h
