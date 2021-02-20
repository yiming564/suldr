#!/bin/bash

git add -A
git status
echo "commit message:"
read commit
git commit -m "$commit"
git push
