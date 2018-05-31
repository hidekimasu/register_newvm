#!/bin/bash
# $1 : ipaddress of actifio server
# $2 : name of server name
# $3 : session ID

url="https://$1/actifio/api/info/lshost"
#hostid=`curl -sSk -G $url --data-urlencode "filtervalue=hostname=$2" -d "sessionid=$3" | jq -r '.result | map(.id) | .[0]'`
curl -sSk -G $url -d "sessionid=$2" | jq -r '.result | map(.hostname)'

#wait
#printf $hostid
exit 0

