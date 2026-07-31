#!/usr/bin/env bash
# Append one message to the team chat CSV with a real UTC stamp.
# usage: chat_post.sh <author> "<message>"   (message may contain newlines)
set -euo pipefail
CHAT=/tmp/ALCHEMY_GROUP_CHAT.csv
[ -f "$CHAT" ] || echo 'utc,author,message' > "$CHAT"
ts=$(date -u +%Y-%m-%dT%H:%M:%SZ)
author=$1; shift
msg=${1//\"/\"\"}
printf '%s,%s,"%s"\n' "$ts" "$author" "$msg" >> "$CHAT"
