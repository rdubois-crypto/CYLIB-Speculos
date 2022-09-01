# CMake generated Testfile for 
# Source directory: /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka/example
# Build directory: /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-build/example
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(simple_test "simple_test")
set_tests_properties(simple_test PROPERTIES  _BACKTRACE_TRIPLES "/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka/cmake/Modules/AddCMockaTest.cmake;116;add_test;/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka/example/CMakeLists.txt;35;add_cmocka_test;/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka/example/CMakeLists.txt;0;")
add_test(allocate_module_test "allocate_module_test")
set_tests_properties(allocate_module_test PROPERTIES  WILL_FAIL "1" _BACKTRACE_TRIPLES "/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka/cmake/Modules/AddCMockaTest.cmake;116;add_test;/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka/example/CMakeLists.txt;42;add_cmocka_test;/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka/example/CMakeLists.txt;0;")
add_test(assert_macro_test "assert_macro_test")
set_tests_properties(assert_macro_test PROPERTIES  WILL_FAIL "1" _BACKTRACE_TRIPLES "/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka/cmake/Modules/AddCMockaTest.cmake;116;add_test;/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka/example/CMakeLists.txt;52;add_cmocka_test;/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka/example/CMakeLists.txt;0;")
add_test(assert_module_test "assert_module_test")
set_tests_properties(assert_module_test PROPERTIES  WILL_FAIL "1" _BACKTRACE_TRIPLES "/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka/cmake/Modules/AddCMockaTest.cmake;116;add_test;/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka/example/CMakeLists.txt;62;add_cmocka_test;/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka/example/CMakeLists.txt;0;")
subdirs("mock")
