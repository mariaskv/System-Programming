#!/bin/bash

for N in {1..5}
do
    start=$SECONDS
    
    ./remoteClient -i 127.0.0.1 -p 9002 -d ../watch
    
    end_t=$(date +%s)
    
    elapsed=$((SECONDS - start))
done