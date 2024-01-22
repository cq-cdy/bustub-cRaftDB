#!/bin/bash
source_dir="./"

backup_dirs=( "../dbs/0" "../dbs/1" "../dbs/2" )

exclude_dir="build"

for dir in "${backup_dirs[@]}"; do

    mkdir -p "$dir"

    rsync -av --delete --exclude="$exclude_dir" "$source_dir" "$dir"
done

echo "分发完成"