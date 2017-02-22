#!/bin/sh


list=`ps aux | grep ck.sh | grep -v "grep" | awk {'print $2'}`
kill -9 $list

