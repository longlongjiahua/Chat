Chat
====

All common code should be seperated in to c/h files in ./Libs

main.c will hold the simple setup/teardown and loop/fork. All other functions will be seperated in to other files.

    make
    make all
    make client
    make server
    make clean
    make linecount

TODO
====
* ~~makefiles~~
* ~~simple UDP server/client~~
* ~~forking server~~
* forking client
* message format
* handle sequence numbers
* respond to acks
* ...
