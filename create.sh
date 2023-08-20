#!/bin/bash

for ((i=1; i<=4500; i++))
do
    random_id=$((RANDOM % 300 + 1))

    id="${random_id}d"
    name="test${i}"
    ./bin/spoor -c "$name", "$id"
done

