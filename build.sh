#!/bin/bash
OS=`uname -s`
BUILD_DIR="aws-sdk-cpp.build"
INSTALL_PREFIX="/usr/local"
SRC_ROOT="${PWD}/aws-sdk-cpp"
AWS_TARGETS="s3;sqs;email"

SUDO=""

if [ "${OS}" = "Darwin" ]; then
    AWS_CPP_VERSION="1.7.218"
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
    AWS_CPP_VERSION="1.7.218"
	if [ "${UID}" != 0 ]; then
		sudo $0
		exit
	fi
	JOBS=`grep -c ^processor /proc/cpuinfo`
fi

NEED_AWS_SDK=1
make check_awssdk &> /dev/null
if [ $? = 0 ]; then
	CURRENT_AWS_CPP_VERSION=`build/check_awssdk`
	if [ "${CURRENT_AWS_CPP_VERSION}" = "${AWS_CPP_VERSION}" ]; then
		echo "AWS SDK already at version ${AWS_CPP_VERSION}, skipping download/build."
		NEED_AWS_SDK=0
	fi
fi


if [ $NEED_AWS_SDK = 1 ]; then
	if [ ! -d "${SRC_ROOT}" ]; then
		git clone --branch "${AWS_CPP_VERSION}" https://github.com/aws/aws-sdk-cpp.git "${SRC_ROOT}"
		RET=$?
		if [ $RET != 0 ]; then
			rm -rf "${SRC_ROOT}"
			exit $RET
		fi
	fi
	mkdir -p "${BUILD_DIR}" || exit $?
	cd "${BUILD_DIR}" || exit $?
	cmake -DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -DENABLE_TESTING=OFF -DBUILD_ONLY="${AWS_TARGETS}" "${SRC_ROOT}"
	RET=$?
	if [ "$?" != 0 ]; then
		rm -rf "${BUILD_DIR}"
		exit $RET
	fi
	make -j ${JOBS} || exit $?
	make install || exit $?
	cd ..
fi

make clean
make -j ${JOBS}
make install || exit $?
