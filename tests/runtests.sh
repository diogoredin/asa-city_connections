#!/bin/bash

################################################################################
# The MIT License
#
# Copyright (c) 2017 Keyaku - https://github.com/Keyaku
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
#  THE SOFTWARE.
################################################################################

# =========== CONSTANTS ===========
# Return values
readonly RET_success=0
readonly RET_error=1
readonly RET_usage=2
readonly RET_help=2

# Colors
readonly RCol='\033[0m'                 # Text Reset
readonly Whi='\033[0;37m'               # White, for small details
readonly Red='\033[0;31m'               # Red, for small details
readonly Gre='\033[0;32m'               # Green, for small details
readonly Yel='\033[0;33m'               # Yellow, for mid-building
readonly BRed='\033[1;31m'              # Bold Red, when an error occurred
readonly BGre='\033[1;32m'              # Bold Green, for successes
readonly BYel='\033[1;33m'              # Bold Yellow, when building stuff
readonly BWhi='\033[1;37m'              # Bold White, when beginning something
readonly URed='\033[4;31m'              # Underline Red, for warnings
readonly UGre='\033[4;32m'              # Underline Green, for smaller successes
readonly UBlu='\033[4;34m'              # Underline Blue, for links
readonly UWhi='\033[4;37m'              # Underline White, for commands

# Strings
readonly Note="${UWhi}Notice${Whi}:${RCol}"
readonly Warn="${BYel}Warning${Yel}:${RCol}"
readonly Err="${BRed}Error${Red}:${RCol}"

readonly ScriptName="$0"

# String Arrays
readonly usage_content=( "Usage: $(basename $ScriptName)"
"HELP:
	-h | --help : Shows this message"
"FILES & DIRECTORIES:
	-e : Specify executable
	-d : Specify tests directory"
"OPTIONS:
	-m | --use-valgrind : Enables testing with valgrind (good for memory tracking)
	-t : Use system timer when running tests
	--show-all : Prints successes as well"
)

# Files & Directories
readonly DIR_current="$(pwd)"

# Options
BOOL_recursive=true
BOOL_showAll=false
BOOL_timer=false

# =========== FUNCTIONS ===========
function usage {
	for i in `seq 0 ${#usage_content[@]}`; do
		echo -e "${usage_content[i]}"
	done
    exit $RET_usage
}

function get_absolute_dir {
	# $1 : directory to parse
	cd "$1" > /dev/null
	temp_dir="$(pwd)"
	cd - > /dev/null
	echo "$temp_dir"
}

function parse_args {
	if [ $# -eq 0 ]; then return 0; fi

	while [ $# -gt 0 ]; do
		case $1 in
			# FILES & DIRECTORIES
			-e )
				shift
				EXEC_prog="$1"
				;;
			-d )
				shift
				DIR_tests="$(get_absolute_dir "$1")"
				if [ "$(ls $1/*.in 2> /dev/null)" -o "$(ls $1/*.out 2> /dev/null)" ]; then
					BOOL_recursive=false
				fi
				;;
			# OPTIONS
			-m | --use-valgrind )
				useValgrind="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --vgdb-error=1"
				;;
			-t )
				BOOL_timer=true
				;;
			--show-all )
				BOOL_showAll=true
				;;
			# HELP
			-h|--help )
				usage
				exit $RET_usage
				;;
			* ) printf "Unknown argument. \"$1\"\n"
				;;
		esac
		shift
	done

	return $RET_success
}

function print_progress {
	# $1 : text to print
	# $2+: formatting args
	printf "\n${BYel}$1\n${RCol}" ${@:2}
}
function print_success {
	# $1 : text to print
	# $2+: formatting args
	printf "\n${UGre}SUCCESS${Gre}:${RCol} $1\n${RCol}" ${@:2}
}
function print_failure {
	# $1 : text to print
	# $2+: formatting args
	printf "\n${URed}FAILURE${Red}:${RCol} $1\n" ${@:2}
}
function print_error {
	# $1 : text to print
	# $2+: formatting args
	printf "\n${BRed}ERROR${Red}:${RCol} $1\n" ${@:2}
}

function set_env {
	if [ "$EXEC_prog" ]; then
		EXEC_prog="$(get_absolute_dir $(dirname $EXEC_prog))/$(basename $EXEC_prog)"
	fi

	# Defining script directories
	cd "$(dirname "$0")"
	DIR_script="$(pwd)"
	DIR_project="$(cd $DIR_script/..; pwd)"
	if [ -z "$DIR_tests" ]; then
		DIR_tests="$DIR_script"
	fi

	if [ -z "$EXEC_prog" ]; then
		EXEC_prog="$DIR_project/bld/proj"
	fi

	if [ $BOOL_timer == true ]; then
		useTimer="time"
	fi
}
function check_env {
	# Checking executable
	if [ ! -e $EXEC_prog ]; then
		print_error "\"$EXEC_prog\" does not exist."
		return $RET_error
	elif [ ! -x $EXEC_prog ]; then
		print_error "\"$EXEC_prog\" is not an executable."
		return $RET_error
	fi

	# Checking valgrind
	if [ "$useValgrind" -a -z "$(which valgrind)" ]; then
		print_error "valgrind is not installed."
		return $RET_error
	fi
}

# Target functionality
function test_dir {
	# $1 : test directory
	if [ $# -lt 1 ]; then
		print_error "test_dir: No arguments given."
		return $RET_error
	elif [ ! -d "$1" ]; then
		print_error "Given argument is not a directory."
		return $RET_error
	elif [ -z "$(ls $1/*.in 2> /dev/null)" -o -z "$(ls $1/*.out 2> /dev/null)" ]; then
		print_error "Given directory does not contain any test files."
		return $RET_error
	fi

	# Run tests
	local fail_count=0
	for test_input in $1/*.in; do
		local test_output="${test_input%.in}.out"
		local test_outhyp="${test_input%.in}.outhyp"
		local test_result="${test_input%.in}.diff"

		# Running program
		if [ "$useValgrind" ]; then
			$useValgrind $EXEC_prog < $test_input
		else
			if [ $BOOL_timer == true ]; then
				printf "Timing $(basename $test_input): "
			fi
			$useTimer $EXEC_prog < $test_input > $test_outhyp
		fi

		if [ $? -ne 0 ]; then
			return $RET_error
		fi

		# Comparing results
	    diff $test_output $test_outhyp > $test_result

		# Act accordingly
		local test_name="$(basename $1)/$(basename $test_input)"
		if [ -s $test_result ]; then
	        print_failure "$test_name. See file $test_result"
			fail_count=$(($fail_count + 1))
	    else
			if [ $BOOL_showAll == true ]; then
				print_success "$test_name"
			fi
	        rm -f $test_result $test_outhyp
	    fi
	done

	local total_count=$(ls -f $1/*.in | wc -l | tr -d ' ')
	if [ $fail_count -gt 0 ]; then
		print_failure "Failed $fail_count / $total_count tests."
	fi

	return $fail_count
}

function cleanup {
	: # Nothing to clean
}

function main {
	parse_args "$@"
	set_env
	check_env
	if [ $? -eq $RET_error ]; then
		usage
		exit $RET_error
	fi

	local retval=$RET_success
	local fail_count=0
	if [ $BOOL_recursive == true ]; then
		for x in $DIR_tests/*/; do
			print_progress "Running through \"$x\""
			test_dir "$x"
			fail_count=$(($fail_count + $?))
		done
	else
		test_dir "$DIR_tests"
		fail_count=$?
	fi
	cleanup

	exit $fail_count
}

# Script starts HERE
main "$@"
