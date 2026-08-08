#!/bin/zsh
# Differential parity: the TypeScript oracle and the Rust port, both live, on
# two loopback ports, compared route by route.
#
# Every server started here is killed on exit. Both sides bind 127.0.0.1 only.
# Nothing is written outside this crate's target/ directory.

set -u
export PATH=/opt/homebrew/bin:$PATH

CRATE=${0:A:h}
ROOT=${CRATE:h:h}
WORK=$CRATE/target/parity-$$
mkdir -p "$WORK"

fail() { print -r -- "PARITY FAIL: $*"; exit 1; }

cleanup() {
  [[ -n ${TS_PID:-} ]] && kill "$TS_PID" 2>/dev/null
  [[ -n ${RS_PID:-} ]] && kill "$RS_PID" 2>/dev/null
  [[ -n ${PERTURB_PID:-} ]] && kill "$PERTURB_PID" 2>/dev/null
  return 0
}
trap cleanup EXIT INT TERM

# ---------------------------------------------------------------- preflight
command -v bun >/dev/null || fail "bun is not on PATH; there is no oracle to compare against"
command -v curl >/dev/null || fail "curl is not on PATH"
ORACLE=$ROOT/tools/metrics/dashboard_server.ts
[[ -s $ORACLE ]] || fail "the oracle $ORACLE is missing or empty"
BIN=$CRATE/target/release/dashboard-server
[[ -x $BIN ]] || fail "build the release binary first: cargo build --release"

free_port() {
  # An ephemeral port the kernel just handed back, so two runs never collide.
  python3 -c 'import socket;s=socket.socket();s.bind(("127.0.0.1",0));print(s.getsockname()[1]);s.close()'
}

start_ts() {
  # cwd MATTERS: Bun.build writes a banner comment holding the entry point's
  # path RELATIVE TO THE CURRENT DIRECTORY, so `/client.js` is 6 bytes longer
  # when the oracle is launched from this crate than from the repository root.
  # The repository root is how the tool is actually run, so that is what the
  # port embeds and what the oracle is given here.
  (cd "$ROOT" && ALCHEMY_DASHBOARD_PORT=$1 exec bun "$ORACLE") >"$WORK/ts.log" 2>&1 &
  TS_PID=$!
}
start_rs() {
  ALCHEMY_DASHBOARD_PORT=$1 "$BIN" >"$WORK/rs.log" 2>&1 &
  RS_PID=$!
}

await() {
  local port=$1 tries=0
  while (( tries < 200 )); do
    curl -s -o /dev/null "http://127.0.0.1:$port/snapshot" && return 0
    sleep 0.1
    (( tries++ ))
  done
  return 1
}

ROUTES=(/ /styles.css /client.js /snapshot /weyard.otf /svg/core /svg/overlays /svg/images /svg/music /nope /svg/CORE /svg/core/extra)

# status + the two headers that carry meaning here.
probe_head() {
  curl -s -D - -o /dev/null "http://127.0.0.1:$1$2" \
    | awk 'BEGIN{IGNORECASE=1} /^HTTP\//{print $2} /^content-type:|^cache-control:/{sub(/\r$/,"");print tolower($0)}' \
    | sort
}
probe_body() {
  curl -s "http://127.0.0.1:$1$2" -o "$3"
}

CASES=0
DIFFS=0
BYTES=0
NONTRIVIAL=0
HEADER_CASES=0
typeset -A SEEN_PAYLOAD

compare_all() {
  local ts_port=$1 rs_port=$2 label=$3
  CASES=0; DIFFS=0; BYTES=0; NONTRIVIAL=0; HEADER_CASES=0
  SEEN_PAYLOAD=()
  local route
  for route in $ROUTES; do
    local a="$WORK/ts.body" b="$WORK/rs.body"
    probe_body "$ts_port" "$route" "$a"
    probe_body "$rs_port" "$route" "$b"

    local ts_status=$(curl -s -o /dev/null -w '%{http_code}' "http://127.0.0.1:$ts_port$route")
    local ts_size=$(wc -c <"$a" | tr -d ' ')
    if [[ $ts_status == 200 && $ts_size -gt 32 ]]; then
      (( NONTRIVIAL++ ))
    fi

    # The font is binary; compare it by digest rather than piping it.
    local na nb
    if [[ $route == /weyard.otf ]]; then
      na=$(shasum -a 1 <"$a"); nb=$(shasum -a 1 <"$b")
    else
      na=$("$BIN" --normalise --root "$ROOT" <"$a")
      nb=$("$BIN" --normalise --root "$ROOT" <"$b")
    fi
    (( CASES++ ))
    (( BYTES += ts_size ))
    SEEN_PAYLOAD[$(printf '%s' "$na" | shasum -a 1 | cut -c1-16)]=1
    if [[ $na != $nb ]]; then
      (( DIFFS++ ))
      print -r -- "  [$label] BODY DIFF $route (ts=${#na}B rs=${#nb}B)"
    fi

    local ha=$(probe_head "$ts_port" "$route")
    local hb=$(probe_head "$rs_port" "$route")
    (( CASES++ )); (( HEADER_CASES++ ))
    if [[ $ha != $hb ]]; then
      (( DIFFS++ ))
      print -r -- "  [$label] HEAD DIFF $route"
      print -r -- "    ts: ${ha//$'\n'/ | }"
      print -r -- "    rs: ${hb//$'\n'/ | }"
    fi
  done
}

# ------------------------------------------------------- negative control
# BEFORE any real comparison: prove the harness can see a difference at all,
# by comparing the oracle against a deliberately perturbed Rust server. Exactly
# one route is perturbed, so exactly one body difference must appear.
print -- "negative control (one perturbed route must yield exactly one body difference)"
TS_PORT=$(free_port); start_ts "$TS_PORT"
await "$TS_PORT" || { cat "$WORK/ts.log"; fail "the TypeScript server never came up"; }

PERTURB_DIR=$CRATE/target/perturbed-$$
mkdir -p "$PERTURB_DIR"
cp "$BIN" "$PERTURB_DIR/dashboard-server"
# Perturb the document title in the copied binary: it is served by exactly ONE
# route (`/`), unlike the 404 body, which three routes share. Same length, so
# no offsets move and nothing else in the image changes.
python3 - "$PERTURB_DIR/dashboard-server" <<'PY'
import sys
path = sys.argv[1]
blob = open(path, 'rb').read()
needle = b'<title>Alchemy</title>'
assert blob.count(needle) == 1, 'expected exactly one title literal, got %d' % blob.count(needle)
open(path, 'wb').write(blob.replace(needle, b'<title>Alch3my</title>'))
PY
[[ $? -eq 0 ]] || fail "could not perturb the binary for the negative control"
chmod +x "$PERTURB_DIR/dashboard-server"
# macOS refuses to exec a binary whose code signature no longer matches the
# bytes. Re-sign the patched copy ad-hoc, and hard-fail if that does not work
# rather than silently reporting a control that never ran.
codesign --force --sign - "$PERTURB_DIR/dashboard-server" 2>/dev/null \
  || fail "could not re-sign the perturbed binary; the negative control cannot run"
PERTURB_PORT=$(free_port)
ALCHEMY_DASHBOARD_PORT=$PERTURB_PORT "$PERTURB_DIR/dashboard-server" >"$WORK/perturb.log" 2>&1 &
PERTURB_PID=$!
await "$PERTURB_PORT" || { cat "$WORK/perturb.log"; fail "the perturbed server never came up"; }

compare_all "$TS_PORT" "$PERTURB_PORT" "control"
CONTROL_DIFFS=$DIFFS
CONTROL_CASES=$CASES
kill "$PERTURB_PID" 2>/dev/null; PERTURB_PID=""
print -- "negative control: cases=$CONTROL_CASES diffs=$CONTROL_DIFFS"
(( CONTROL_DIFFS == 1 )) || fail "the negative control found $CONTROL_DIFFS differences, expected exactly 1; the harness is not comparing what it claims to"

# ------------------------------------------------------- real comparison
print -- "real comparison"
RS_PORT=$(free_port); start_rs "$RS_PORT"
await "$RS_PORT" || { cat "$WORK/rs.log"; fail "the Rust server never came up"; }
compare_all "$TS_PORT" "$RS_PORT" "real"

DISTINCT=${#SEEN_PAYLOAD}

# ------------------------------------------------------------------ floors
# Every floor counts the TYPESCRIPT side of the comparison, never the union:
# a Rust server that answered everything and an oracle that answered nothing
# must fail, not pass.
(( CASES >= 24 ))       || fail "only $CASES cases compared, floor is 24; scanning nothing is not passing"
(( DISTINCT >= 9 ))     || fail "only $DISTINCT distinct payloads, floor is 9; the routes are collapsing into one body"
(( NONTRIVIAL >= 8 ))   || fail "only $NONTRIVIAL TypeScript routes returned 200 with a non-trivial body, floor is 8"
(( BYTES >= 2000000 ))  || fail "only $BYTES TypeScript bytes compared, floor is 2000000"
(( HEADER_CASES >= 12 ))|| fail "only $HEADER_CASES header comparisons, floor is 12"

print -- "cases=$CASES distinct_payloads=$DISTINCT nontrivial_200=$NONTRIVIAL ts_bytes=$BYTES header_cases=$HEADER_CASES diffs=$DIFFS"
if (( DIFFS != 0 )); then
  fail "$DIFFS differences between the oracle and the port"
fi
print -- "PARITY OK"
