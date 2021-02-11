#!/bin/bash
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

sh_files=$(find -name "*.sh")
exe_files=$(grep -Erl "#\!/bin/(bash|sh)" --exclude=*.* package)
files="$sh_files\n$exe_files"
#find no files
if [[ "$files" == "\n" ]];then
  echo -e "${YELLOW}No shell files found!${NC}"
  exit 0
fi

echo  -e "$files"| xargs shellcheck -s bash
if [[ $? -eq 0 ]];then
  echo "AWESOME!No problems found."
  exit 0
else
  echo -e "Understand errors in: ${BLUE}https://github.com/koalaman/shellcheck/wiki${NC}"
  exit 1
fi

