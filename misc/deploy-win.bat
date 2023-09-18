windeployqt --no-quick-import --no-system-d3d-compiler --no-virtualkeyboard --no-opengl-sw --no-translations sandic.exe
rmdir imageformats /s/q  
rmdir networkinformation /s/q  
rmdir generic /s/q  
rmdir iconengines /s/q  
rmdir tls /s/q  
del sqldrivers\qsqlpsql.dll
del sqldrivers\qsqlodbc.dll
del Qt6Network.dll
del Qt6Svg.dll
