#!/bin/bash

UMBRELLA="awscsdk.h"
OS=`uname -s`
USER_NAME="${USER}"

if [ "${OS}" = "Darwin" ]; then
	USER_NAME=`id -F`
fi

echo "/* This file is auto generated, do not edit! */" > "${UMBRELLA}" || exit $?
TODAY=`date +"%A %B %d %Y"`
echo "/* Generated by ${USER_NAME} ON  ${TODAY}*/" >> "${UMBRELLA}" || exit $?
echo >> "${UMBRELLA}" || exit $?
echo >> "${UMBRELLA}" || exit $?
echo "#pragma once" >> "${UMBRELLA}" || exit $?
echo >> "${UMBRELLA}" || exit $?
for I in include/*.h ; do 
	echo "#include <awscsdk/${I}>" >> "${UMBRELLA}" || exit $?
done
