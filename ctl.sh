#!/bin/bash  
CMD=$1
APP_NAME=server_http
BIN_PATH=./bin
LOG_PATH=./var/log
RUN_PATH=./var/run
NOW_DATE=`date +%Y%m%d`

function _isRun() {
    process=$(_checkAppProcess)
    if [ -n "$process" ];then       # notice: the $process need to contain the ""
        echo $process
    else
        echo 0
    fi
}

function _checkAppProcess() {
    echo `ps -ef|grep ${APP_NAME}|grep -v grep|awk '{print $2}'`
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
        echo "Run Success"
    fi
}

function stop() {
    runState=$(_isRun)
    if [ $runState -gt 0 ];then
        kill -9 $runState
        if [ -f "${LOG_PATH}/${NOW_DATE}_run.log" ];then
            rm "${LOG_PATH}/${NOW_DATE}_run.log"
        fi
        echo "Shutdown Success"
    else
        echo "Shutdown alresdy"
    fi
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
