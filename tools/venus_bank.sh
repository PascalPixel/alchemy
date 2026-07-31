#!/usr/bin/env bash
#
# Venus Lighthouse: verify, then commit and push — in that order, with the
# ordering enforced rather than remembered.
#
#   tools/venus_bank.sh "<commit subject>" ["<body>"]
#
# This exists because of a real near-miss. The banking sequence was being typed
# as a shell chain, and `bun run verify ... ; git commit ...` runs the commit
# whether or not verify passed — a `;` where `&&` was meant. On 2026-07-31 that
# pushed a commit whose verify had failed seconds earlier; it happened to be
# sound only because a concurrent lane fixed the offending file in between.
#
# Ordering that matters and is easy to get wrong:
#   1. sync spans FIRST, so the published map cannot lag the sources in the same
#      commit;
#   2. verify, and STOP if it fails — nothing below runs;
#   3. commit, letting the hooks (progress suffix, publication gate, conflict
#      markers) do their own checking;
#   4. push with backoff.
#
# Concurrent lanes write into this tree continuously, so a verify failure here
# is often another lane mid-file rather than anything wrong with what is staged.
# That is precisely why it must block: telling the two cases apart requires
# looking, and looking is what this script forces.
set -uo pipefail
cd "$(dirname "$0")/.." || exit 1

SUBJECT="${1:?usage: venus_bank.sh \"<subject>\" [\"<body>\"]}"
BODY="${2:-}"
LOG="${TMPDIR:-/tmp}/venus-bank-$$.log"
trap 'rm -f "$LOG"' EXIT

bun tools/semantic_regions_sync.ts --write 2>&1 | tail -1

if ! bun run verify > "$LOG" 2>&1; then
  echo "VERIFY FAILED — nothing committed. First errors:"
  grep -m3 "error:" "$LOG"
  echo "(if this is another lane mid-file, wait and re-run; do not bypass)"
  exit 1
fi
echo "verify green"

SUFFIX=$(bun tools/full_c_progress.ts --subject)
HEAD_DENOM=$(git log -1 --pretty=%s | grep -oE '/[0-9,]+ bytes' | tr -cd '0-9')
NEW_DENOM=$(printf '%s' "$SUFFIX" | grep -oE '/[0-9,]+ bytes' | tr -cd '0-9')
PREFIX=""
[ "$HEAD_DENOM" != "$NEW_DENOM" ] && PREFIX="metrics: correct executable denominator and "

git add -A
if [ -z "$(git diff --cached --name-only)" ]; then
  echo "nothing staged; tree matches HEAD"
  exit 0
fi

MESSAGE="${PREFIX}${SUBJECT} ${SUFFIX}"
[ -n "$BODY" ] && MESSAGE="${MESSAGE}

${BODY}"
MESSAGE="${MESSAGE}

Co-Authored-By: Claude Opus 5 <noreply@anthropic.com>
Claude-Session: https://claude.ai/code/session_01A6PtpLD4adgR8CjMixiRHX"

if ! git commit -m "$MESSAGE" >> "$LOG" 2>&1; then
  echo "commit rejected:"; grep -m2 -E 'error|rejected' "$LOG"; exit 1
fi

for backoff in 2 4 8 16; do
  if git push origin venus >> "$LOG" 2>&1; then
    echo "PUSHED $SUFFIX"
    exit 0
  fi
  sleep "$backoff"
done
echo "push failed after retries"
exit 1
