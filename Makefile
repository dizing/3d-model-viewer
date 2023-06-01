# THAT'S STANDARD GNU TARGETS MAKEFILE
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
CXX_COMPILER = -D CMAKE_CXX_COMPILER=clang++
APP_OPEN = @open buildRelease/s21_viewer.app
BROWSER_OPENER = @open
endif
ifeq ($(UNAME), Linux)
CXX_COMPILER = 
APP_OPEN = ./buildRelease/s21_viewer
BROWSER_OPENER = @x-www-browser
endif
.PHONY: install uninstall clean dvi dist test gcov_report
install: buildRelease
	@cmake --build buildRelease

uninstall: clean
# END THAT

open: install
	${APP_OPEN}

clean:
	@rm -rf buildRelease 2>/dev/null
	@rm -rf buildDebug 2>/dev/null
	@rm *.tar.gz 2>/dev/null || true

dvi: buildRelease
	@cmake --build buildRelease --target generate_docs
	${BROWSER_OPENER} buildRelease/html/index.html
	

dist: buildRelease
	@cmake --build buildRelease --target package_source
	@cp buildRelease/*.tar.gz .
test: buildRelease
	@cmake --build buildRelease --target test
	@echo "\033[0;32m-----------TESTS------------:\033[0m"
	@./buildRelease/viewer_graph_lib/test
	@echo "\033[0;32m----------------------------:\033[0m"
	@echo "\033[0;32m------CHECK FORMATTING------:\033[0m"
	@cmake --build buildRelease --target format-check
	@echo "\033[0;32m----------------------------:\033[0m"

gcov_report: buildDebug
	@cmake --build buildDebug --target test_coverage
	${BROWSER_OPENER} buildDebug/test_coverage/index.html
	

buildRelease:
	@cmake -S . -B buildRelease -D CMAKE_C_COMPILER=gcc $(CXX_COMPILER) -D CMAKE_BUILD_TYPE=Release
buildDebug:
	@cmake -S . -B buildDebug -D CMAKE_C_COMPILER=gcc $(CXX_COMPILER) -D CMAKE_BUILD_TYPE=Debug 