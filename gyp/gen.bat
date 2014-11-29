@rem This script assumes that gyp is checked out alongside sumatrapdf directory
@rem and it's being called as: gyp\gen.bat

call ..\gyp\gyp.bat -G msvs_version=2013 -f msvs --depth=. -Igyp\common.gypi gyp\velociraptor8.gyp
