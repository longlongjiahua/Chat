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

Client
===

Sending: input (myname, server ip, port, destination, message)
* Send message to server:port as myname meant for destination
* Wait for ack (if no ack in $TIME, then resend)

Getting: input (myname, server ip, port, GET)
* send get request to the server
* set up sliding window for responses
* send acks for in-order messages
* after each ack, print the messages in order.

Server
===

Recieve message
* fork
* hash, and check against last message in the file for duplicates (optional, add after everything else)
* append message to destination's file
* ack

Get request
* fork
* set up pointer for destination of the most recent ack (if not null then fail to prevent overlap)
* read file -> mes[i]
* loop through mes: fork
* in each fork, send message with seq number i; sleep for timout; check ack pointer to see if an ack of seq equal or greater has been recieved; if no ack then resend and wait.

Recieve ACK
* fork 
* update the correct pointer

