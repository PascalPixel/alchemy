#!/usr/bin/env bash
#
# Greedy fixpoint sweep of the callee-return-type lever over one draft.
#
#   tools/return_type_sweep.sh <draft.c> <overlay:offset> <span> [extra flags...]
#
# A callee's DECLARED RETURN TYPE decides argument-setter order: `s32` makes cc1
# emit `movs r1` before `movs r0`, `void` emits r0 first (HANDOVER.md explains the
# sched2 mechanism). A function with many callees therefore has a large search
# space that is tedious to probe by hand and trivial to sweep: each probe costs
# about 0.12s, so a 60-callee function converges in seconds.
#
# This flips each `void` callee declaration to `s32` in turn, keeps any flip that
# lowers the differing-halfword count, and repeats until nothing improves. It
# writes the improved draft back over <draft.c> only if it strictly improved.
#
# Scope, measured: the lever moves ONLY a transposition of two `movs` argument
# setters. If the residual is `movs r0` against an `lsls`, a pool `ldr`, an `adds`
# or a `str`, that is the immediate-build-transposition fingerprint instead and no
# return-type spelling touches it — expect a null result and move on. The two
# levers compose, so retry with -mthumb-immediate-latency before giving up.
#
# A prototype-less shared declaration blocks the lever entirely; where one symbol
# needs both orders in one function, use a per-site cast
# `((void (*)())Func_02001234)(0, 50)` instead. `(void)Func(...)` does NOT work.
set -uo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT" || exit 1

DRAFT="${1:?usage: return_type_sweep.sh <draft.c> <overlay:offset> <span> [flags...]}"
TARGET="${2:?missing <overlay:offset>}"
SPAN="${3:?missing <span>}"
shift 3

WORK="$(mktemp -d "${TMPDIR:-/tmp}/alchemy-sweep-XXXXXX")"
trap 'rm -rf "$WORK"' EXIT
CURRENT="$WORK/current.c"
CANDIDATE="$WORK/candidate.c"
cp "$DRAFT" "$CURRENT"

BEST=$(bun work/claude/overlay_verify.ts "$TARGET" "$CURRENT" "$SPAN" "$@" \
  | grep -oE 'differing_halfwords=[0-9]+' | grep -oE '[0-9]+$')
echo "start differing_halfwords=$BEST"

try_flip() {   # $1 = callee, $2 = from-type, $3 = to-type
  cp "$CURRENT" "$CANDIDATE"
  sed -i "s/^extern $2 ${1}(/extern $3 ${1}(/; s/^$2 ${1}(/$3 ${1}(/" "$CANDIDATE"
  cmp -s "$CANDIDATE" "$CURRENT" && return 1
  local result
  result=$(bun work/claude/overlay_verify.ts "$TARGET" "$CANDIDATE" "$SPAN" "$@" \
    | grep -oE 'differing_halfwords=[0-9]+' | grep -oE '[0-9]+$')
  [ -z "$result" ] && return 1
  if [ "$result" -lt "$BEST" ]; then
    BEST=$result
    cp "$CANDIDATE" "$CURRENT"
    echo "  ${1}: $2 -> $3   differing_halfwords=$BEST"
    return 0
  fi
  return 1
}

# Both directions matter and each covers half the search space: flipping a
# declared-s32 callee to void closed two functions that the void->s32 direction
# alone could not touch.
improved=1
while [ "$improved" = 1 ]; do
  improved=0
  for callee in $(grep -oE '^(extern )?void Func_[0-9a-f]+' "$CURRENT" \
                  | grep -oE 'Func_[0-9a-f]+' | sort -u); do
    try_flip "$callee" void s32 "$@" && improved=1
  done
  for callee in $(grep -oE '^(extern )?s32 Func_[0-9a-f]+' "$CURRENT" \
                  | grep -oE 'Func_[0-9a-f]+' | sort -u); do
    try_flip "$callee" s32 void "$@" && improved=1
  done
done

echo "final differing_halfwords=$BEST"
if ! cmp -s "$CURRENT" "$DRAFT"; then
  cp "$CURRENT" "$DRAFT"
  echo "wrote improved draft to $DRAFT"
fi
