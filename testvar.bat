@ECHO OFF
CLS
assembler.exe examples\vartest.basm testvar.vm
bvm.exe testvar.vm
pause
