# Tests directory

This is where test files will reside.

To run all tests, simply compile the project, then run `tests/runtests.sh`
and its default settings should do everything for you.

The options available for the script are:
* **HELP**
	* `-h | --help` : Shows "usage" message (kind of pointless, if you ask me)
* **FILES & DIRECTORIES**
	* `-e` : Specify executable
	* `-d` : Specify tests directory (will run recursively only if children directories contain tests)
* **OPTIONS**
	* `-m | --use-valgrind` : Enables testing with valgrind (good for memory tracking).
	Will abort if valgrind is not installed
	* `-t` : Use system timer when running tests
	* `--show-all` : Prints successes as well (only prints failures by default)
