# CYLIB-Speculos
CYLIB integrated in Speculos environment

This repository is only dedicated to a quick compilation and testing of CYLIB in the Speculos environment.
Compilation and installation are exactly the same as Speculos, follow instructions given here : https://speculos.ledger.com/

## INSTALL/EXECUTE
The source of CYLIB is in the src/innovation directory which is a mirror of the CYLIB project.
The 'main' program is defined in tests/inno/cy_tests_libs_all.c
The execution of  cylib modules is done via the command:

./build/tests/pbc/test_libs_all 

This repo is destined to be suppressed after some makefile work in CYLIB to deal with Speculos as a pure external project.
