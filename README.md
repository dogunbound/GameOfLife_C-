Prep to make the binary:
`./quickstart.sh`

# Release
Build the project after `cd`ing into `release`:
`clear ; cmake ../src/ && make -j19 && ./Conway_GOL`

# Release
Build the project after `cd`ing into `debug`:
`clear ; cmake -DCMAKE_BUILD_TYPE=Debug ../src/ && make -j19`
