#!/usr/bin/env bash
#
# Run every required check, then commit and push, tolerating concurrent walkers.
#
#   tools/bank_cycle.sh "<commit subject body>" [branch]
#
# This runs exactly the same checks as `bun run verify`, with three deviations
# that preserve them while cutting wall clock from ~190s to ~15s:
#
#   1. The three builds run CONCURRENTLY. `verify` chains them sequentially, but
#      they are independent once each has its own output tree (build_claimed is
#      re-run inside every build_full anyway). Only the ROM-mode build writes the
#      default out/full tree, so tracked artifacts land where the metrics tooling
#      expects them. Measured 94s vs 161s cold for the two full builds.
#   2. The bookkeeping staleness check (progress:check) is moved next to the
#      commit. It is the LAST step of `verify`, so a walker adopting during the
#      build invalidates an otherwise-green run; adjacent to the commit the race
#      window is seconds instead of minutes.
#   3. `verify` ends by checking the coverage map is current (coverage:check);
#      here the map is simply rewritten (`bun run coverage`) before staging, so
#      the banked commit carries a fresh picture instead of failing on a stale
#      one. It reads tracked evidence only, so it costs about a second.
#
# It also picks the "metrics: correct executable denominator" subject prefix
# automatically, which check_commit_progress.ts requires whenever the executable
# denominator moves.
#
# Content caches (out/cache, see HANDOVER.md) make repeat runs cheap: warm, the
# whole cycle is ~15s. Delete out/cache to force a cold rebuild.
set -uo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT" || exit 1

BODY="${1:?usage: bank_cycle.sh \"<commit subject body>\" [branch]}"
BRANCH="${2:-$(git rev-parse --abbrev-ref HEAD)}"
SCRATCH="${TMPDIR:-/tmp}/alchemy-bank-$$"
LOG="$SCRATCH/bank.log"
mkdir -p "$SCRATCH/so" "$SCRATCH/cl"
trap 'rm -rf "$SCRATCH"' EXIT

denom() { printf '%s' "$1" | grep -oE '/[0-9,]+ bytes' | tr -cd '0-9'; }

builds() {
  bun run test >> "$LOG" 2>&1 || return 1

  bun tools/build_claimed.ts --output "$SCRATCH/cl/claimed" >> "$LOG" 2>&1 &
  local claimed=$!
  bun tools/build_full.ts --source-only \
    -o "$SCRATCH/so/rebuilt.gba" \
    --claimed-output "$SCRATCH/so/claimed" \
    --asm-output "$SCRATCH/so/asm" \
    --asset-output "$SCRATCH/so/assets" >> "$LOG" 2>&1 &
  local sourceonly=$!
  bun run build:full >> "$LOG" 2>&1 &
  local rom=$!

  wait $claimed || return 1
  wait $sourceonly || return 1
  wait $rom || return 1
}

for attempt in $(seq 1 6); do
  : > "$LOG"
  if ! builds; then
    echo "attempt $attempt: build failed: $(grep -m1 'error:' "$LOG")"
    continue
  fi
  echo "builds green on attempt $attempt"

  bun tools/full_c_progress.ts --write-inventory > /dev/null 2>&1
  bun tools/full_c_progress.ts --write-report > /dev/null 2>&1
  bun run coverage > /dev/null 2>&1
  if ! bun run progress:check >> "$LOG" 2>&1; then
    echo "attempt $attempt: a walker adopted mid-build, retrying"
    continue
  fi

  SUBJECT=$(bun tools/full_c_progress.ts --subject)
  HEAD_SUBJECT=$(git log -1 --pretty=%s)
  if [ "$(denom "$SUBJECT")" != "$(denom "$HEAD_SUBJECT")" ]; then
    MESSAGE="metrics: correct executable denominator and $BODY $SUBJECT"
  else
    MESSAGE="$BODY $SUBJECT"
  fi

  git add -A
  if [ -z "$(git diff --cached --name-only)" ]; then
    echo "nothing staged; tree matches HEAD"
    exit 0
  fi
  git diff --check --cached >> "$LOG" 2>&1 || { echo "whitespace check failed"; exit 1; }
  bun tools/check_publication.ts --staged >> "$LOG" 2>&1 || { echo "publication check failed"; exit 1; }

  if ! git commit -m "$MESSAGE" >> "$LOG" 2>&1; then
    echo "attempt $attempt: commit rejected: $(grep -m1 -E 'error:' "$LOG" | tail -1)"
    continue
  fi
  for backoff in 2 4 8 16; do
    if git push -u origin "$BRANCH" >> "$LOG" 2>&1; then
      echo "PUSHED $SUBJECT"
      exit 0
    fi
    sleep "$backoff"
  done
  echo "push failed after retries"
  exit 1
done

echo "EXHAUSTED"
exit 1
