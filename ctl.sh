#!/bin/bash  
CMD=$1
APP_NAME=http_server
BIN_PATH=./bin
LOG_PATH=./var/log
RUN_PATH=./var/run
NOW_DATE=`date +%Y%m%d`

function _isRun() {
    if [ -f "${RUN_PATH}/run.pid" ];then
            echo `cat ${RUN_PATH}/run.pid`
        else
            echo 0
    fi
}

function getStatus() {
    runState=$(_isRun)
    if [ $runState -gt 0 ];then
        echo "Running PID=${runState}"
    else
        echo "Shutdown"
    fi
}

function run (){
    runState=$(_isRun)
    if [ $runState -gt 0 ];then
        echo "Running PID=${runState}"
    else
        nohup $BIN_PATH/$APP_NAME 2>&1 > "${LOG_PATH}/${NOW_DATE}_run.log" & echo $! > "${RUN_PATH}/run.pid"
    fi
}

function stop() {
    kill -9 `cat $RUN_PATH/run.pid`
    rm $RUN_PATH/run.pid
}

function showLog() {
    cat "${LOG_PATH}/${NOW_DATE}_run.log"
}

function gethelp()
{
cat << EOF
usage:
  |-start cmd: run or start
  |
  |-stop cmd: stop or shutdown
  |
  |-status cmd: status or state
  |
  |-show nohup's runtime log cmd: log 
  |
  |-h to get useage help

tips: go to hell !

EOF
}


case $CMD in
    run|start|Start|Run)
        run
        ;;
    stop|shutdown|Stop|Shutdown)
        stop
        ;;
    status|Status|state|State)
        getStatus
        ;;
    log|Log|LOG)
        showLog
        ;;
    *)
        gethelp
        ;;
esac