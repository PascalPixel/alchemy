#!/usr/bin/env bash
#
# Run the whole ENUMERABLE search over one main-image draft, in parallel, and
# report the best result with the exact flags needed to reproduce it.
#
#   tools/finish_draft.sh <draft.c> [--jobs N] [--out DIR] [--quick]
#
# Why this exists: HANDOVER.md §9 now records the measurement that decides how to
# spend effort here -- `xargs` over candidate_show sustains ~114 ms per probe at
# 18-way concurrency, so a few thousand probes cost seconds, while an agent
# exploring twenty source spellings costs minutes. Everything that can be
# enumerated should therefore be enumerated before anyone reasons about the
# residual. Those steps existed as three separate tools plus a hand-built flag
# list; assembling them by hand each time is the waste this removes.
#
# Order follows §5's measured protocol: CSE modes first (each alone AND paired,
# never one alone as a shortcut), then the scheduler modes, then the return-type
# sweep, then re-sweep because a scheduler flag changes WHICH return types help,
# then statement order.
#
# It never writes to src/ or asm/ and never runs a build, git or the bank cycle.
# Output is a best-of report plus the surviving residual, which is exactly the
# input a human or a strong model needs in order to pick a lever.
set -uo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT" || exit 1

DRAFT="${1:?usage: finish_draft.sh <draft.c> [--jobs N] [--out DIR] [--quick]}"
shift
JOBS=$(( $(sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 8) ))
OUT=""
QUICK=0
while [ $# -gt 0 ]; do
  case "$1" in
    --jobs) JOBS="$2"; shift 2 ;;
    --out) OUT="$2"; shift 2 ;;
    --quick) QUICK=1; shift ;;
    *) echo "unknown argument: $1" >&2; exit 2 ;;
  esac
done

STEM="$(basename "$DRAFT" .c)"
case "$STEM" in
  [0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f]) ;;
  *) echo "draft basename must be the 8-hex stem: got '$STEM'" >&2; exit 2 ;;
esac
WORK="$(mktemp -d "${TMPDIR:-/tmp}/alchemy-finish-XXXXXX")"
trap 'rm -rf "$WORK"' EXIT
[ -n "$OUT" ] || OUT="$WORK/out"
mkdir -p "$OUT"

# 経路はステム名で決まる。別名で置くと §7 の罠でフラグが素に落ちる。
probe_of() {  # $1 = source, $2 = flags ("" for baseline) -> "diff size ref"
  local slot; slot="$WORK/p$$RANDOM"
  mkdir -p "$slot"
  cp "$1" "$slot/$STEM.c"
  local line
  if [ -z "$2" ]; then
    line=$(bun tools/candidate_show.ts "$slot/$STEM.c" --work "$slot/w" 2>/dev/null | head -1)
  else
    line=$(bun tools/candidate_show.ts "$slot/$STEM.c" --flags "$2" --work "$slot/w" 2>/dev/null | head -1)
  fi
  rm -rf "$slot"
  [[ "$line" =~ candidate=([0-9]+)\ reference=([0-9]+)\ differing_halfwords=([0-9]+) ]] || return 1
  echo "${BASH_REMATCH[3]} ${BASH_REMATCH[1]} ${BASH_REMATCH[2]}"
}

START="$(probe_of "$DRAFT" "")" || { echo "draft does not compile: $DRAFT" >&2; exit 1; }
read -r BEST BESTSIZE REFSIZE <<< "$START"
BESTFLAGS=""
BESTSRC="$OUT/$STEM.c"
cp "$DRAFT" "$BESTSRC"
echo "start   diff=$BEST size=$BESTSIZE/$REFSIZE  ($STEM)"

# ---- phase 1: flag matrix, all at once -------------------------------------
# §5: probe both CSE modes together AND each alone; a note recording only one is
# unmeasured. §7: never combine -fsched-low-dest-first with
# -mthumb-immediate-latency, the latter subsumes then breaks the same targets.
FLAGSETS="$WORK/flagsets.txt"
cat > "$FLAGSETS" <<'EOF'
-fno-cse-two-insn-immediate
-fno-cse-pool-immediate
-fno-cse-two-insn-immediate,-fno-cse-pool-immediate
-fsched-low-dest-first
-fsched-high-dest-first
-fno-sched-alias
-fsched-store-first
-fno-sched-depend-count
-fno-gcse-insert-load
-mthumb-immediate-latency
-mgrouped-dma-store
-fthumb-group-value2-in-place
-fno-cse-two-insn-immediate,-fsched-low-dest-first
-fno-cse-two-insn-immediate,-fsched-high-dest-first
-fno-cse-two-insn-immediate,-fno-cse-pool-immediate,-fsched-low-dest-first
-fno-cse-two-insn-immediate,-fno-cse-pool-immediate,-fsched-high-dest-first
-fno-cse-two-insn-immediate,-fsched-store-first
-fno-cse-two-insn-immediate,-fno-sched-depend-count
-fno-cse-two-insn-immediate,-fno-sched-alias
-fsched-low-dest-first,-fno-sched-alias
-fno-rerun-cse-after-loop
-fno-regmove
-fno-gcse
-fno-cse-follow-jumps
-fno-expensive-optimizations
-fno-strength-reduce
-fno-schedule-insns2
-fno-canonicalize-comparison
-fno-thumb-contiguous-immediate
-fno-regmove,-fno-schedule-insns2
-fno-rerun-cse-after-loop,-fno-cse-two-insn-immediate
EOF
if [ "$QUICK" = 0 ]; then
  # 探索器が知っているフォーク独自モードも一通り当てる。
  bun -e '
    const s = await Bun.file("tools/mode_sweep.ts").text();
    const m = /export const FORK_MODES = \[(.*?)\] as const;/s.exec(s);
    if (m) for (const f of m[1].match(/"([^"]+)"/g) ?? []) console.log(f.replaceAll("\"",""));
  ' 2>/dev/null >> "$FLAGSETS"
fi
sort -u "$FLAGSETS" -o "$FLAGSETS"

RUN="$WORK/run.sh"
cat > "$RUN" <<EOF
#!/bin/bash
cd "$ROOT"
slot="$WORK/j\$\$"
mkdir -p "\$slot"
cp "$BESTSRC" "\$slot/$STEM.c"
line=\$(bun tools/candidate_show.ts "\$slot/$STEM.c" --flags "\$1" --work "\$slot/w" 2>/dev/null | head -1)
rm -rf "\$slot"
if [[ "\$line" =~ candidate=([0-9]+)\ reference=([0-9]+)\ differing_halfwords=([0-9]+) ]]; then
  echo -e "\${BASH_REMATCH[3]}\t\${BASH_REMATCH[1]}/\${BASH_REMATCH[2]}\t\$1"
fi
EOF
chmod +x "$RUN"
xargs -P "$JOBS" -n 1 "$RUN" < "$FLAGSETS" > "$WORK/matrix.tsv" 2>/dev/null
# サイズ一致を優先する。§5: 残差の判断はまず大きさ。
pick() {  # reads matrix, echoes "diff size flags" of the best row
  awk -F'\t' -v bd="$1" -v bs="$2" -v br="$3" '
    { split($2,a,"/");
      exact = (a[1]==a[2]);
      better = ($1 < bd) || ($1 == bd && exact && bs != br);
      if (better && (best=="" || $1 < bestd)) { bestd=$1; best=$0 } }
    END { if (best != "") print best }' "$WORK/matrix.tsv"
}
# ---- phase 1b: pair the modes that are known to interact -------------------
# Expanding only the best-scoring singles was tried first and is WRONG, for the
# reason §6 already documents about the CSE pair: the combination that wins is
# often built from modes that are individually neutral or worse. Measured on
# 08091174, where `-mgrouped-dma-store,-fno-cse-pool-immediate` reaches 3 while
# neither flag is in the top six alone; and on 0800300c, where
# `-fno-cse-pool-immediate,-fno-schedule-insns2,-fthumb-group-control-last`
# reaches 2 the same way. So pair exhaustively over a curated pool of modes that
# are known to interact, rather than ranking first. ~90 pairs is a few seconds.
if [ "$QUICK" = 0 ] && [ -s "$WORK/matrix.tsv" ]; then
  # macOS ships bash 3.2, which has no `mapfile`; it fails silently and leaves an
  # empty array, so build the pool portably.
  TOP=()
  while IFS= read -r flag; do
    [ -n "$flag" ] && TOP+=("$flag")
  done <<'POOL'
-fno-cse-two-insn-immediate
-fno-cse-pool-immediate
-fno-schedule-insns2
-fsched-low-dest-first
-fsched-high-dest-first
-fsched-store-first
-fno-sched-depend-count
-fno-sched-alias
-mgrouped-dma-store
-fthumb-group-control-last
-fthumb-group-value2-in-place
-fthumb-split-group-base
-mthumb-immediate-latency
-fno-gcse-insert-load
-fno-regmove
-fno-rerun-cse-after-loop
POOL
  COMBOS="$WORK/combos.txt"
  : > "$COMBOS"
  count=${#TOP[@]}
  for ((i = 0; i < count; i++)); do
    for ((j = i + 1; j < count; j++)); do
      echo "${TOP[i]},${TOP[j]}" >> "$COMBOS"
    done
  done
  # Descriptor-family triples: these regions repeatedly need a grouper plus a CSE
  # mode plus an ordering mode together.
  for a in -mgrouped-dma-store -fthumb-group-control-last -fthumb-group-value2-in-place; do
    for b in -fno-cse-pool-immediate -fno-cse-two-insn-immediate; do
      for c in -fno-schedule-insns2 -fsched-store-first -fno-sched-depend-count; do
        echo "$a,$b,$c" >> "$COMBOS"
      done
    done
  done
  # §7: never combine -fsched-low-dest-first with -mthumb-immediate-latency, the
  # latter subsumes and then breaks the same targets.
  grep -v -e '-fsched-low-dest-first.*-mthumb-immediate-latency' \
          -e '-mthumb-immediate-latency.*-fsched-low-dest-first' "$COMBOS" \
    | sort -u > "$COMBOS.clean"
  mv "$COMBOS.clean" "$COMBOS"
  if [ -s "$COMBOS" ]; then
    xargs -P "$JOBS" -n 1 "$RUN" < "$COMBOS" >> "$WORK/matrix.tsv" 2>/dev/null
    echo "combos  tried=$(wc -l < "$COMBOS" | tr -d ' ') from the interacting-mode pool"
  fi
fi

# ---- phase 1c: the other approved compiler families ------------------------
# Two of the 20 conversions banked on 2026-07-30 needed `--family old-agbcc`
# rather than the routed gcc 2.96: 08006cdc (0 with old-agbcc -O1 against 56
# routed) and 08006878. Region families cluster, so a stem whose neighbours are
# already in AGBCC_SOURCES is a strong candidate. Probing four family settings
# costs four probes, and missing one costs the whole function.
if [ "$QUICK" = 0 ]; then
  FAMOUT="$WORK/families.tsv"
  : > "$FAMOUT"
  for fam in old-agbcc gcc2951 pret-early-thumb; do
    for extra in "" "-O1"; do
      slot="$WORK/fam-$fam${extra}"
      mkdir -p "$slot"
      cp "$BESTSRC" "$slot/$STEM.c"
      if [ -z "$extra" ]; then
        line=$(bun tools/candidate_show.ts "$slot/$STEM.c" --family "$fam" --work "$slot/w" 2>/dev/null | head -1)
      else
        line=$(bun tools/candidate_show.ts "$slot/$STEM.c" --family "$fam" --flags "$extra" --work "$slot/w" 2>/dev/null | head -1)
      fi
      rm -rf "$slot"
      if [[ "$line" =~ candidate=([0-9]+)\ reference=([0-9]+)\ differing_halfwords=([0-9]+) ]]; then
        printf '%s\t%s/%s\t--family %s%s\n' \
          "${BASH_REMATCH[3]}" "${BASH_REMATCH[1]}" "${BASH_REMATCH[2]}" "$fam" \
          "${extra:+ --flags $extra}" >> "$FAMOUT"
      fi
    done
  done
  if [ -s "$FAMOUT" ]; then
    FAMBEST=$(sort -n "$FAMOUT" | head -1)
    FAMDIFF=$(cut -f1 <<< "$FAMBEST")
    FAMSIZE=$(cut -f2 <<< "$FAMBEST")
    if [ "$FAMDIFF" -lt "$BEST" ]; then
      echo "family  diff=$FAMDIFF size=$FAMSIZE  via $(cut -f3 <<< "$FAMBEST")  (NOT a --flags value; pass these options directly)"
    fi
    sort -n "$FAMOUT" | head -3 > "$OUT/family-probes.tsv"
  fi
fi

ROW="$(pick "$BEST" "$BESTSIZE" "$REFSIZE")"
if [ -n "$ROW" ]; then
  BEST=$(cut -f1 <<< "$ROW")
  BESTSIZE=$(cut -f2 <<< "$ROW" | cut -d/ -f1)
  BESTFLAGS=$(cut -f3 <<< "$ROW")
  echo "flags   diff=$BEST size=$BESTSIZE/$REFSIZE  via $BESTFLAGS"
else
  echo "flags   no improvement over baseline ($(wc -l < "$FLAGSETS" | tr -d ' ') settings tried)"
fi
sort -n "$WORK/matrix.tsv" | head -8 > "$OUT/flag-matrix-top.tsv"

if [ "$BEST" = 0 ] && [ "$BESTSIZE" = "$REFSIZE" ]; then
  echo "EXACT   diff=0 size=$BESTSIZE/$REFSIZE flags=${BESTFLAGS:-BASE} source=$BESTSRC"
  echo "${BESTFLAGS:-BASE}" > "$OUT/flags.txt"
  exit 0
fi

# ---- phase 2: return-type sweep, at baseline and at the best flag set -------
for setting in "" "$BESTFLAGS"; do
  [ -n "$setting" ] || [ -z "$BESTFLAGS" ] || continue
  if [ -z "$setting" ]; then
    tools/return_type_sweep_main.sh "$BESTSRC" --out "$WORK/rt.c" > "$WORK/rt.log" 2>&1
  else
    tools/return_type_sweep_main.sh "$BESTSRC" --flags "$setting" --out "$WORK/rt.c" > "$WORK/rt.log" 2>&1
  fi
  if [ -f "$WORK/rt.c" ]; then
    if R="$(probe_of "$WORK/rt.c" "$setting")"; then
      read -r d sz rf <<< "$R"
      if [ "$d" -lt "$BEST" ]; then
        BEST=$d; BESTSIZE=$sz; BESTFLAGS="$setting"
        cp "$WORK/rt.c" "$BESTSRC"
        echo "rettype diff=$BEST size=$BESTSIZE/$REFSIZE  via ${setting:-BASE}"
      fi
    fi
    rm -f "$WORK/rt.c"
  fi
done

# ---- phase 3: statement-order sweep ---------------------------------------
if [ -z "$BESTFLAGS" ]; then
  bun tools/statement_order_sweep_main.ts "$BESTSRC" --out "$WORK/so.c" --jobs "$JOBS" > "$WORK/so.log" 2>&1
else
  bun tools/statement_order_sweep_main.ts "$BESTSRC" --flags "$BESTFLAGS" --out "$WORK/so.c" --jobs "$JOBS" > "$WORK/so.log" 2>&1
fi
PROBES=$(grep -oE 'probes=[0-9]+' "$WORK/so.log" | tail -1 | cut -d= -f2)
if [ -f "$WORK/so.c" ] && R="$(probe_of "$WORK/so.c" "$BESTFLAGS")"; then
  read -r d sz rf <<< "$R"
  if [ "$d" -lt "$BEST" ]; then
    BEST=$d; BESTSIZE=$sz
    cp "$WORK/so.c" "$BESTSRC"
    echo "stmtord diff=$BEST size=$BESTSIZE/$REFSIZE"
  fi
fi
# §5 の注意: 探索数が 1-3 なら「効かなかった」ではなく「試せていない」。
echo "stmtord probes=${PROBES:-0}$( [ "${PROBES:-0}" -le 3 ] && echo '  (too few independent statements -- lever NOT exercised)' )"

echo "---"
# A different compiler family is not expressible as a --flags value, so it is
# reported separately -- but it must not be buried under a worse gcc-2.96 number.
if [ -s "$OUT/family-probes.tsv" ]; then
  FAMTOP=$(head -1 "$OUT/family-probes.tsv")
  FAMD=$(cut -f1 <<< "$FAMTOP")
  if [ "$FAMD" -lt "$BEST" ]; then
    echo "BEST IS A FAMILY, not a flag set: diff=$FAMD size=$(cut -f2 <<< "$FAMTOP") via $(cut -f3 <<< "$FAMTOP")"
    echo "  reproduce: bun tools/candidate_show.ts <draft>/$STEM.c $(cut -f3 <<< "$FAMTOP") --work <dir>"
    echo "  to adopt, route the stem through AGBCC_SOURCES (and AGBCC_OPTIMIZE_O1_SOURCES for -O1)"
  fi
fi
echo "best    diff=$BEST size=$BESTSIZE/$REFSIZE flags=${BESTFLAGS:-BASE}  (gcc 2.96 routed lane)"
echo "source  $BESTSRC"
echo "${BESTFLAGS:-BASE}" > "$OUT/flags.txt"
echo "residual (candidate | reference):"
cp "$BESTSRC" "$WORK/final/$STEM.c" 2>/dev/null || { mkdir -p "$WORK/final"; cp "$BESTSRC" "$WORK/final/$STEM.c"; }
if [ -z "$BESTFLAGS" ]; then
  bun tools/candidate_show.ts "$WORK/final/$STEM.c" --work "$WORK/final/w" 2>/dev/null | grep -E '^\s*!' | head -20
else
  bun tools/candidate_show.ts "$WORK/final/$STEM.c" --flags "$BESTFLAGS" --work "$WORK/final/w" 2>/dev/null | grep -E '^\s*!' | head -20
fi
echo "(enumerable search is exhausted; what remains needs a lever chosen from HANDOVER.md §4/§5)"
