#! /bin/bash

case ${1} in
    "server")
        ./server 8087
    ;;
    "client")
        ./client "127.0.0.1" 8087
    ;;
    *)
    ;;
esac
