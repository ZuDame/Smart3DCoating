@echo off

:start
  title Purge Subs from %1
  call :subpurge %1
  FOR /D /R %%a IN (*.*) do call :subpurge "%%a" call :subdel "%%a"


:subpurge 
  echo Purging, %1
  cd "%1"
  call purge.bat

:subdel
  echo Purging, %1
  cd "%1"
  attrib *.* -h
  attrib *.* -r
  del *.dwl
  del std.out
  del *.err
  del *.inf.*
  del *.log.*
  del *.pro
  del *.out.*
