#! /bin/bash

case ${1} in
    "server")
        ./server 8087
    ;;
    "client")
        ./client "140.112.30.186" 8087
    ;;
    *)
    ;;
esac
