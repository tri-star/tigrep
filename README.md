tigrep
===================================

# Overview

`tigrep` is yet another [timegrep](https://github.com/linux-wizard/timegrep).

This command searches log file by specified date interval.

Such like "extract logs between 18:30:00 and 19:00:00".

## Description
`tigrep` is yet another [timegrep](https://github.com/linux-wizard/timegrep).

This is useful for search logs that "date part does not appeared each line" such as MySQL general log.

If input data is a file, tigrep is search the start position with "binary search", so scanning is very fast even if log file is large.

## Usage

### General usage
General usage is:

```
tigrep --regex="[regex-to-extract-date-part]" --format="[date-format]"
--from="[start-date]" --to="[end-date]" [input-file]

or

# scan log file with predefined regex and date format.
tigrep --log-type=[log-type-name] --from="[start-date]" --to="[end-date]" [input-file]

or

# list supported log types.
tigrep -L

```

* **regex**: regular expression pattern. The pattern uses boost::regex as its engine, 
  see [boost::regex documentation](http://www.boost.org/doc/libs/1_55_0/libs/regex/doc/html/boost_regex/syntax.html) (Perl Regular Expression Syntax, Character Class Names, etc)for pattern usage.
 
* **format**: date format such as "%Y-%m-%d %H:%M:%S", this string is passed to strftime,
  see [strftime(3)](http://man7.org/linux/man-pages/man3/strftime.3.html) about format information.

* **log-type** log type name, specify one of following `apache`,`mysql-general`,`mysql-slow`,`syslog`. for more information, please refer `tigrep -L`.

* **from**: date time to start extract. Currently, the format is needed to match "format" parameter.

* **to**: date time to start extract. Currently, the format is needed to match "format" parameter.

* **input-file**: Input file to extract. If this parameter is omitted, then stdin will used instead.

For quick instruction, see "Example" on this page.

For detailed information:
  
`tigrep --help`

  
### Example
Let's say we have following log file.

example Log file(MySQL general log) - demo.log:

```
131027 15:53:20     4 Connect   jfs@localhost on jfs
                    4 Query     SHOW FULL TABLES WHERE Table_type = 'BASE TABLE'
...
131027 15:53:33     5 Connect   jfs@localhost on jfs
                    5 Query     SHOW FULL TABLES WHERE Table_type = 'BASE TABLE'
                    5 Query     SELECT COLUMN_NAME AS Field, COLUMN_TYPE AS Type, ...
...
```

And we want to extract lines between "131027 16:00:00" and "131027 17:30:00",
try following command.

```
tigrep --regex="^([0-9]{6} [0-9:]{8})" --from="131027 16:00:00" --to="131027 17:30:00"
  --format="%y%m%d %H:%M:%S" demo.log
```

This will output log lines between above ranges to stdout.


### Predefined log types

```
Available log types:
* apache
  - Regex:  ^[^ ]+? [^ ]+? [^ ]+? \[([^\]]+?)\]
  - Format: %d/%b/%Y:%H:%M:%S %z
* mysql-general
  - Regex:  ^([0-9]{6} [0-9:]{8})
  - Format: %y%m%d %H:%M:%S
* mysql-slow
  - Regex:  ^# Time: ([0-9\s:]+)
  - Format: %y%m%d %H:%M:%S
* syslog
  - Regex:  ^(.+? [0-9]+ [0-9:]+) 
  - Format: %b %d %H:%M:%S
```

## Install

### Prerequisites
To use tigrep, following packages are required.

* gcc-c++ (libstdc++)

### Binary installation

work in progress...


### From source code

To build from source code, following packages are required:

* gcc-c++
* make
* cmake
* boost
  My environment is boost-1.55.0, but it will possible to build with older version.
* gtest (google test)
  My environment is gtest-1.7.0, but it will possible to build with older version.

Build instruction: 

```
$ git clone https://github.com/tri-star/tigrep.git
$ cd tigrep
$ mkdir build
$ cmake ..
$ make
$ sudo make install
```

By default, `tigrep` is installed on /usr/local/bin.
You could change install location with cmake option.

example: change install dir to /usr/bin

```
#replace above "cmake .." to:
cmake -DCMAKE_INSTALL_PREFIX=/usr
```

## Road map

* Support to extend "log-type" by putting file into plugin directory.


## Contribution

1. Fork ([https://github.com/tri-star/tigrep/fork](https://github.com/tri-star/tigrep/fork))
2. Create a feature branch
3. Commit your changes
4. Rebase your local changes against the master branch
5. Run test suite with the `[project-dir]/bin/run_tests` command and confirm that it passes
6. Create new Pull Request

## Licence

[MIT](https://github.com/tri-star/tigrep/blob/master/LICENSE)

## Author

[tri-star](https://github.com/tri-star)
