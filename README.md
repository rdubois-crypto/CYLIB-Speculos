# CYLIB-Speculos
CYLIB integrated in Speculos environment

This repository is only dedicated to a quick compilation and testing of CYLIB in the Speculos environment.


## COMPILATION/INSTALLATION
Compilation and installation are exactly the same as Speculos ( detailed instructions about tools you possible miss are given here : https://speculos.ledger.com/).
You might be lucky and have all required packages, and just want to copy paste the current code, goto to root of repo then type:

>cmake -Bbuild -H.
>make -C build/

Note that you might have the error ["CMakeCache.txt" is not ...]
In this case, just erase all CMakeCache.txt, then type the previous commands.



## INSTALL/EXECUTE
The source of CYLIB is in the src/innovation directory which is a mirror of the CYLIB project.
The 'main' program is defined in tests/inno/cy_tests_libs_all.c
The execution of  cylib modules is done via the command:

>./build/tests/pbc/test_libs_all 

This repo is destined to be suppressed after some makefile work in CYLIB to deal with Speculos as a pure external project.

