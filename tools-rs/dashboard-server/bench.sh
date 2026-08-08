#!/bin/zsh
# Decomposed benchmark: process startup, and per-request latency once warm.
# A server is not a batch tool; startup alone would be a misleading number.

set -u
export PATH=/opt/homebrew/bin:$PATH
CRATE=${0:A:h}
ROOT=${CRATE:h:h}
BIN=$CRATE/target/release/dashboard-server
ORACLE=$ROOT/tools/metrics/dashboard_server.ts

command -v bun >/dev/null || { print -- "bun missing"; exit 1 }
[[ -x $BIN ]] || { print -- "build the release binary first"; exit 1 }

free_port() { python3 -c 'import socket;s=socket.socket();s.bind(("127.0.0.1",0));print(s.getsockname()[1]);s.close()'; }
now_ms() { python3 -c 'import time;print(int(time.time()*1000))'; }

cleanup() { [[ -n ${A:-} ]] && kill $A 2>/dev/null; [[ -n ${B:-} ]] && kill $B 2>/dev/null; return 0 }
trap cleanup EXIT INT TERM

# ------------------------------------------------- 1. self-test, cold start
print -- "== startup, --self-test end to end (full coverage build), 3 runs each"
for label in ts rs; do
  total=0
  for i in 1 2 3; do
    t0=$(now_ms)
    if [[ $label == ts ]]; then (cd "$ROOT" && bun "$ORACLE" --self-test >/dev/null); else "$BIN" --self-test >/dev/null; fi
    t1=$(now_ms)
    (( total += t1 - t0 ))
  done
  print -- "  $label: $(( total / 3 ))ms mean"
done

# ------------------------------------------------- 2. time to first response
print -- "== time from exec to the first served /snapshot"
for label in ts rs; do
  port=$(free_port)
  t0=$(now_ms)
  if [[ $label == ts ]]; then (cd "$ROOT" && ALCHEMY_DASHBOARD_PORT=$port exec bun "$ORACLE") >/dev/null 2>&1 & A=$!
  else ALCHEMY_DASHBOARD_PORT=$port "$BIN" >/dev/null 2>&1 & A=$! ; fi
  while ! curl -s -o /dev/null "http://127.0.0.1:$port/snapshot"; do sleep 0.05; done
  t1=$(now_ms)
  print -- "  $label: $(( t1 - t0 ))ms"
  kill $A 2>/dev/null; A=""
done

# ------------------------------------------------- 3. per-request latency
print -- "== per-request latency, warm, 50 requests per route"
TS_PORT=$(free_port); (cd "$ROOT" && ALCHEMY_DASHBOARD_PORT=$TS_PORT exec bun "$ORACLE") >/dev/null 2>&1 & A=$!
RS_PORT=$(free_port); ALCHEMY_DASHBOARD_PORT=$RS_PORT "$BIN" >/dev/null 2>&1 & B=$!
while ! curl -s -o /dev/null "http://127.0.0.1:$TS_PORT/snapshot"; do sleep 0.05; done
while ! curl -s -o /dev/null "http://127.0.0.1:$RS_PORT/snapshot"; do sleep 0.05; done

for route in /snapshot / /client.js /svg/overlays; do
  line="  $route"
  for port in $TS_PORT $RS_PORT; do
    # Warm.
    for i in 1 2 3 4 5; do curl -s -o /dev/null "http://127.0.0.1:$port$route"; done
    t0=$(now_ms)
    for i in {1..50}; do curl -s -o /dev/null "http://127.0.0.1:$port$route"; done
    t1=$(now_ms)
    label=ts; [[ $port == $RS_PORT ]] && label=rs
    line="$line  $label=$(( (t1 - t0) * 1000 / 50 ))us"
  done
  print -- "$line"
done
print -- "(latency includes curl process startup on both sides, so it is an upper bound that biases neither)"
