#!/bin/bash
AWS_CPP_VERSION="1.2.25"
OS=`uname -s`
BUILD_DIR="aws-sdk-cpp.build"
INSTALL_PREFIX="/usr/local"
SRC_ROOT="${PWD}/aws-sdk-cpp"
AWS_TARGETS="s3;sqs;email"

if [ ! -d "${SRC_ROOT}" ]; then
	git clone --branch "${AWS_CPP_VERSION}" https://github.com/aws/aws-sdk-cpp.git "${SRC_ROOT}"
	RET=$?
	if [ $RET != 0 ]; then
		rm -rf "${SRC_ROOT}"
		exit $RET
	fi
fi

if [ "${OS}" = "Darwin" ]; then
	if [ "${UID}" != 0 ]; then
		CHECK_DIRS=("${INSTALL_PREFIX}/include" "${INSTALL_PREFIX}/lib")
		for D in "${CHECK_DIRS[@]}"; do
            if [ ! -w "${D}" ]; then
                echo "${D} is not writable (is homebrew installed for this user?)"
                echo "Fix this or run this script as root"
                exit 1
            fi
		done
	fi
	JOBS=`sysctl -n hw.logicalcpu_max`
else
	if [ "${UID}" != 0 ]; then
		echo "Installation on non Darwin OS requires this script to be ran as root"
		exit 1
	fi
	JOBS=`grep -c ^processor /proc/cpuinfo`
fi

mkdir -p "${BUILD_DIR}" || exit $?
cd "${BUILD_DIR}" || exit $?
cmake -DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -DENABLE_TESTING=OFF -DBUILD_ONLY="${AWS_TARGETS}" "${SRC_ROOT}" || exit $?
make -j ${JOBS} || exit $?
make install || exit $?
cd ..
rm -rf "${BUILD_DIR}" "${SRC_ROOT}"
make -j ${JOBS} install
