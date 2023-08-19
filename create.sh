#!/bin/bash

for ((i=1; i<=300; i++))
do
    random_id=$((RANDOM % 300 + 1))

    id="${random_id}d"
    name="test${i}"
    ./bin/spoor -c "$name", "$id"
done

