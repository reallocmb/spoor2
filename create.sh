#!/bin/bash

for ((i=1; i<=300; i++))
do
    random_id=$((RANDOM % 300 + 1))

    id="${random_id}d"
    name="test${i}"
    echo "Ausführung $i: Name=$name, ID=$id"
    spoor -c "$name", "$id"
done

