#!/bin/sh
#
# Coding style: http://code.google.com/p/arora/wiki/CodingStyle
#

case "${1}" in
    --about )
        echo "Hack of a script to find some common style errors."
        exit 0
        ;;
esac


args=$1
if [ -z "${args}" ] ; then
  args="*"
fi

options="-n --exclude=moc_* --exclude=qrc_* --include=*.cpp --include=*.h -r"

grep $options '( ' $args
grep $options ' )' $args
grep $options 'if(' $args
grep $options 'for(' $args
grep $options 'while(' $args
grep $options 'switch(' $args
grep $options 'foreach(' $args
grep $options ' $' $args
grep $options '^{ }*{' $args
grep $options '){' $args
grep $options '	' $args
grep $options ' \*>' $args
grep $options '#include "q' $args
egrep $options '\(.*\* .*\)' $args | grep '::'
grep $options '[a-z]++' $args | grep 'for'
grep $options '[a-z]--' $args | grep 'for'

# var *name;
grep $options '[^\* \/]\* ' $args
grep $options '[^& ]& ' $args

exit 0
