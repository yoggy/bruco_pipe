#!/bin/sh
BASE_DIR=`dirname $0`
BRUCO_PIPE=${BASE_DIR}/bruco_pipe
BRUCO_CONFIG=${BASE_DIR}/sample_config.txt
TARGET_CMD=${BASE_DIR}/test_server

exec ${BRUCO_PIPE} ${BRUCO_CONFIG} ${TARGET_CMD}

