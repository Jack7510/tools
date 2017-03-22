Mar 21, 2017

Test framework for kid watch project, other Linux device as well

[Build]
#build proxy
cc -o proxy msg.c test_proxy.cpp

#build target
integrate in Xcode or Open Embedded

#test_host.py
test_host.py is for Robot Framework

[Files]
README.md			readme

msg.c				POSIX message queue interface
msg.h			
msg.inc

test_target.cpp 	test demo for target program
process_json.cpp
test_target.h

test_proxy.cpp 		test proxy will forward JSON-RPC message between target and host
test_host.py 		Python script sample for Robot Framework

[Usage]
# run host
python test_host.py

# run target
./test_target tid

# proxy command line
./test_proxy -t tid -j json-script
