#!/usr/bin/env bash
#
# Greedy fixpoint sweep of the callee-return-type lever over one MAIN-IMAGE draft.
#
#   tools/return_type_sweep_main.sh <draft.c> [--flags -fa,-fb] [--out FILE]
#
# The overlay sweep (tools/return_type_sweep.sh) scores through
# work/claude/overlay_verify.ts, which only exists on the semantic branch and only
# accepts <overlay:offset> targets. Main-image drafts therefore had no way to run
# this lever at all, so every main-image park predates it -- the same stale-
# evidence shape HANDOVER.md §5 describes for the pre-2026-07-30 sweep nulls.
# This scores through tools/candidate_show.ts, whose first line already reports
# candidate/reference sizes and the differing-halfword count.
#
# Lever, restated from HANDOVER.md §4: a callee's DECLARED RETURN TYPE decides
# argument-setter order -- `s32` emits `movs r1` before `movs r0`, `void` emits r0
# first. `(void)Func(...)` does not work; the CALL_EXPR's own type is what counts.
# Both flip directions matter, so each callee is tried both ways to a fixpoint.
#
# Scope: the lever moves ONLY a transposition of two `movs` argument setters. A
# `movs` against an `lsls`/`ldr`/`adds`/`str` is the immediate-build
# transposition instead and no return-type spelling touches it -- expect a null.
#
# Never writes over the input draft: the improved source goes to --out (default
# <draft>.swept.c), so a sweep can never corrupt a recorded park.
set -uo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT" || exit 1

DRAFT="${1:?usage: return_type_sweep_main.sh <draft.c> [--flags -fa,-fb] [--out FILE]}"
shift
FLAGS=""
OUT=""
while [ $# -gt 0 ]; do
  case "$1" in
    --flags) FLAGS="$2"; shift 2 ;;
    --out) OUT="$2"; shift 2 ;;
    *) echo "unknown argument: $1" >&2; exit 2 ;;
  esac
done
[ -n "$OUT" ] || OUT="${DRAFT%.c}.swept.c"

WORK="$(mktemp -d "${TMPDIR:-/tmp}/alchemy-sweep-main-XXXXXX")"
trap 'rm -rf "$WORK"' EXIT
CURRENT="$WORK/current.c"
CANDIDATE="$WORK/candidate.c"
cp "$DRAFT" "$CURRENT"

# candidate_show routes compiler flags by the SOURCE STEM, so the probe copy has
# to keep the draft's 8-hex basename or it silently compiles at baseline flags --
# the §7 routing trap, which presents as verify and adopt disagreeing.
STEM="$(basename "$DRAFT" .c)"

score() {  # $1 = source file -> "diff size ref", empty when it does not compile
  local probe="$WORK/probe/$STEM.c"
  mkdir -p "$WORK/probe"
  cp "$1" "$probe"
  local line
  if [ -n "$FLAGS" ]; then
    line=$(bun tools/candidate_show.ts "$probe" --flags "$FLAGS" --work "$WORK/run" 2>/dev/null | head -1)
  else
    line=$(bun tools/candidate_show.ts "$probe" --work "$WORK/run" 2>/dev/null | head -1)
  fi
  [[ "$line" =~ candidate=([0-9]+)\ reference=([0-9]+)\ differing_halfwords=([0-9]+) ]] || return 1
  echo "${BASH_REMATCH[3]} ${BASH_REMATCH[1]} ${BASH_REMATCH[2]}"
}

START="$(score "$CURRENT")"
[ -n "$START" ] || { echo "draft does not compile: $DRAFT" >&2; exit 1; }
read -r BEST BESTSIZE REFSIZE <<< "$START"
echo "start differing_halfwords=$BEST size=$BESTSIZE/$REFSIZE"

try_flip() {   # $1 = callee, $2 = to-type
  cp "$CURRENT" "$CANDIDATE"
  # Match ANY declared return type, not just void/s32: a callee declared u32,
  # u16, s16 or pointer-returning is otherwise invisible and the sweep returns a
  # false null on exactly the function the lever would have closed.
  sed -i '' -E "s/^(extern +)?[A-Za-z_][A-Za-z0-9_]* +\**${1}\(/\1$2 ${1}(/" "$CANDIDATE"
  cmp -s "$CANDIDATE" "$CURRENT" && return 1
  local result size ref got
  got="$(score "$CANDIDATE")"
  # An uncompilable flip is a dead end, not an improvement: fall through rather
  # than comparing empty strings as integers.
  [ -n "$got" ] || return 1
  read -r result size ref <<< "$got"
  # Size equality is the primary signal (§5): prefer a size-exact result even at
  # an equal halfword count, because a size-exact residual is an allocation
  # problem the non-flag levers finish.
  if [ "$result" -lt "$BEST" ] ||
     { [ "$result" -eq "$BEST" ] && [ "$size" -eq "$ref" ] && [ "$BESTSIZE" -ne "$REFSIZE" ]; }; then
    BEST=$result; BESTSIZE=$size
    cp "$CANDIDATE" "$CURRENT"
    echo "  ${1} -> $2   differing_halfwords=$BEST size=$size/$ref"
    return 0
  fi
  return 1
}

improved=1
while [ "$improved" = 1 ]; do
  improved=0
  for callee in $(grep -oE '^(extern +)?[A-Za-z_][A-Za-z0-9_]* +\**Func_[0-9a-f]+\(' "$CURRENT" \
                  | grep -oE 'Func_[0-9a-f]+' | sort -u); do
    try_flip "$callee" s32 && improved=1
    try_flip "$callee" void && improved=1
  done
done

echo "final differing_halfwords=$BEST size=$BESTSIZE/$REFSIZE"
if ! cmp -s "$CURRENT" "$DRAFT"; then
  cp "$CURRENT" "$OUT"
  echo "wrote improved draft to $OUT"
fi
