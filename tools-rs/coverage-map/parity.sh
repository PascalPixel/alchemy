#!/bin/bash
# Measured parity: every flag combination, Bun against the release binary.
#
# WHY a script rather than a Rust test: the deliverable is a comparison of two
# *processes*, including exit status and the shape of a failure, and only a
# harness that actually launches both can prove that. It compares stdout byte
# for byte, compares exit status, and for failures compares the `error:` line
# rather than the prose of a stack trace Bun prints and the binary does not.
#
# Guards against a vacuous pass, in the order they run:
#   * a preflight -- missing `bun`, a missing or empty TypeScript oracle, or an
#     unreadable tracked input is a hard failure, never a quiet skip. Scanning
#     nothing is not passing;
#   * a NEGATIVE CONTROL, run BEFORE any real comparison, proving the
#     comparator can see a difference at all: one deliberately perturbed byte
#     must produce exactly one difference and the unperturbed pair must still
#     match. If the control cannot fire, the run stops there;
#   * a hard case floor;
#   * PER-CATEGORY floors counted on the TYPESCRIPT SIDE ONLY -- summarize
#     lines, `error:` lines, and written SVG bytes. A floor on the union of
#     both mirrors would pass on an emptied source, because the Rust side alone
#     could still clear it;
#   * a distinct-payload floor, so a suite where every case said the same thing
#     is reported as degenerate;
#   * `unsupported` must be zero. The port implements every flag.
#
# Tracked-file hazard: `--write` rewrites metrics/gs1-en-coverage-map.json, the
# four assets/readme/gs1-en-*.svg panels and README.md. Those six paths are
# copied byte for byte before the run, restored afterwards, and `cmp`-verified.
# Every deletion path below is a full explicit literal.

set -u
export PATH=/opt/homebrew/bin:$PATH

ROOT=/Users/pixel/Developer/PascalPixel/alchemy
CRATE="$ROOT/tools-rs/coverage-map"
WORK="$CRATE/target/fixtures/parity"
BIN="$CRATE/target/release/coverage-map"
TS="$ROOT/tools/metrics/coverage_map.ts"
CASE_FLOOR=22
PAYLOAD_FLOOR=6
SUMMARY_FLOOR=8
ERROR_FLOOR=5
SVG_BYTE_FLOOR=2000000

rm -rf "/Users/pixel/Developer/PascalPixel/alchemy/tools-rs/coverage-map/target/fixtures/parity"
mkdir -p "$WORK"

cd "$ROOT" || exit 1

echo "== preflight =="
command -v bun >/dev/null 2>&1 || { echo "FAIL: bun is not on PATH; the oracle cannot run"; exit 1; }
[ -s "$TS" ] || { echo "FAIL: $TS is missing or empty"; exit 1; }
for input in \
  "$ROOT/metrics/gs1-en-coverage-map.json" \
  "$ROOT/README.md" \
  "$ROOT/assets/readme/gs1-en-core.svg" \
  "$ROOT/assets/readme/gs1-en-overlays.svg" \
  "$ROOT/assets/readme/gs1-en-images.svg" \
  "$ROOT/assets/readme/gs1-en-music.svg" ; do
  [ -r "$input" ] && [ -s "$input" ] || { echo "FAIL: unreadable or empty input $input"; exit 1; }
done
cargo build --release --manifest-path "$CRATE/Cargo.toml" >/dev/null 2>&1 || {
  echo "FAIL: build failed"; exit 1; }
echo "  bun $(bun --version), oracle $(wc -c < "$TS" | tr -d ' ') bytes, binary built"

# Only the last line of Bun's stderr matters for a throw; native exceptions
# (ENOENT, SyntaxError) print no `error:` line at all, which is why the two
# kinds are counted apart.
error_line() { grep -m1 '^error: ' "$1" 2>/dev/null || true; }

echo "== negative control (before any real comparison) =="
# Establish that the comparator can see a one-byte difference. This runs first
# so a broken comparator can never be reported as parity.
bun "$TS" > "$WORK/control.ts.out" 2>/dev/null
"$BIN"     > "$WORK/control.rs.out" 2>/dev/null
[ -s "$WORK/control.ts.out" ] || { echo "FAIL: the oracle produced no output; nothing to compare"; exit 1; }
cp "$WORK/control.rs.out" "$WORK/control.perturbed.out"
printf 'x' >> "$WORK/control.perturbed.out"
control_diffs=0
cmp -s "$WORK/control.ts.out" "$WORK/control.perturbed.out" || control_diffs=$((control_diffs + 1))
cmp -s "$WORK/control.ts.out" "$WORK/control.rs.out"        || control_diffs=$((control_diffs + 1))
if [ "$control_diffs" = 1 ]; then
  echo "  ok: a one-byte perturbation yields exactly 1 difference (unperturbed still matches)"
else
  echo "  FAIL: expected exactly 1 difference from the perturbation, saw $control_diffs"
  exit 1
fi

cases=0; agreed=0; differed=0; unsupported=0; native_failures=0
ts_summaries=0; ts_errors=0
declare -a DIFFS=()
: > "$WORK/payloads.txt"

compare() {
  local name="$1"; shift
  cases=$((cases + 1))
  local slug; slug=$(printf '%s' "$name" | tr -c 'A-Za-z0-9' '_')
  bun "$TS" "$@" > "$WORK/$slug.ts.out" 2> "$WORK/$slug.ts.err"; local ts_rc=$?
  "$BIN" "$@" > "$WORK/$slug.rs.out" 2> "$WORK/$slug.rs.err"; local rs_rc=$?
  local ts_err rs_err
  ts_err=$(error_line "$WORK/$slug.ts.err")
  # Per-category tallies count the TYPESCRIPT side only. A floor met by the
  # union of both mirrors would still pass if the oracle went silent.
  grep -q 'target=gs' "$WORK/$slug.ts.out" && ts_summaries=$((ts_summaries + 1))
  [ -n "$ts_err" ] && ts_errors=$((ts_errors + 1))
  # The payload of a case is what the TypeScript actually said: its stdout plus
  # its `error:` line. Counting stdout alone would call every failing case
  # identical and understate how varied the suite is.
  { cat "$WORK/$slug.ts.out"; printf '%s\n' "$ts_err"; } >> "$WORK/payloads.txt"
  rs_err=$(error_line "$WORK/$slug.rs.err")
  if [ "$ts_rc" -ne 0 ] && [ -z "$ts_err" ]; then
    native_failures=$((native_failures + 1))
  fi
  # A case the Rust port openly refuses is not parity, and is not counted as
  # agreement. It is reported on its own line so the gap stays visible.
  if printf '%s' "$rs_err" | grep -q 'not implemented in the Rust port'; then
    unsupported=$((unsupported + 1))
    echo "  UNSUPPORTED  $name"
    return
  fi
  if [ "$ts_rc" = "$rs_rc" ] && cmp -s "$WORK/$slug.ts.out" "$WORK/$slug.rs.out" &&
     [ "$ts_err" = "$rs_err" ]; then
    agreed=$((agreed + 1))
    echo "  ok           $name"
  else
    differed=$((differed + 1))
    DIFFS+=("$name (rc $ts_rc/$rs_rc)")
    echo "  DIFFER       $name (rc $ts_rc/$rs_rc)"
    diff "$WORK/$slug.ts.out" "$WORK/$slug.rs.out" | head -5
    [ "$ts_err" = "$rs_err" ] || echo "    ts: $ts_err
    rs: $rs_err"
  fi
}

echo "== non-writing cases =="
compare "default"
compare "target gs1"               --target gs1-en
compare "target gs2"               --target gs2-en
compare "target unknown"           --target gs3
compare "target missing value"     --target
compare "unrecognized"             --nope
compare "help long"                --help
compare "help short"               -h
compare "help wins over bad arg"   --help --nope
compare "bad arg before help"      --nope --help
compare "exact worktree"           --exact-ref worktree
compare "exact unresolvable"       --exact-ref origin/no-such-ref-xyz
compare "exact missing value"      --exact-ref
compare "semantic none"            --semantic-ref none
compare "semantic worktree"        --semantic-ref worktree
compare "semantic unresolvable"    --semantic-ref no-such-ref-xyz
compare "semantic missing value"   --semantic-ref
compare "target and semantic none" --target gs1-en --semantic-ref none
compare "exact and semantic"       --exact-ref worktree --semantic-ref worktree

echo "== formerly unsupported cases =="
compare "self-test"                --self-test
compare "check"                    --check
compare "check gs2"                --target gs2-en --check

echo "== tracked-file hazard: snapshot, run --write, restore, verify =="
TRACKED=(
  "$ROOT/metrics/gs1-en-coverage-map.json"
  "$ROOT/README.md"
)
for svg in "$ROOT"/assets/readme/gs1-en-*.svg; do TRACKED+=("$svg"); done
mkdir -p "$WORK/snapshot" "$WORK/ts-written" "$WORK/rs-written"
snapshot_ok=1
for path in "${TRACKED[@]}"; do
  cp "$path" "$WORK/snapshot/$(basename "$path")" || snapshot_ok=0
done
svg_bytes=0
svg_identical=0
if [ "$snapshot_ok" != 1 ]; then
  echo "  FAIL: snapshot failed; refusing to run --write"
  DIFFS+=("tracked-file snapshot failed")
else
  # The TypeScript writes first; its artefacts are captured and the tree is put
  # back before the Rust binary writes, so the two are compared as independent
  # productions rather than one overwriting the other.
  bun "$TS" --write >/dev/null 2>&1
  for path in "${TRACKED[@]}"; do cp "$path" "$WORK/ts-written/$(basename "$path")"; done
  for path in "${TRACKED[@]}"; do cp "$WORK/snapshot/$(basename "$path")" "$path"; done

  compare "write" --write
  for path in "${TRACKED[@]}"; do cp "$path" "$WORK/rs-written/$(basename "$path")"; done
  for path in "${TRACKED[@]}"; do cp "$WORK/snapshot/$(basename "$path")" "$path"; done

  restored=0
  for path in "${TRACKED[@]}"; do
    cmp -s "$WORK/snapshot/$(basename "$path")" "$path" && restored=$((restored + 1))
  done
  echo "  restored and cmp-verified: $restored/${#TRACKED[@]} tracked files"
  [ "$restored" = "${#TRACKED[@]}" ] || DIFFS+=("a tracked file was not restored")

  # Artefacts are compared BYTE-EXACTLY, not structurally.
  for name in gs1-en-coverage-map.json README.md \
              gs1-en-core.svg gs1-en-overlays.svg gs1-en-images.svg gs1-en-music.svg; do
    if cmp -s "$WORK/ts-written/$name" "$WORK/rs-written/$name"; then
      echo "  byte-identical $(wc -c < "$WORK/ts-written/$name" | tr -d ' ') bytes  $name"
    else
      echo "  ARTEFACT DIFFERS  $name"
      DIFFS+=("written artefact $name differs")
    fi
  done
  # The SVG byte floor counts the TYPESCRIPT-written panels specifically.
  for name in gs1-en-core.svg gs1-en-overlays.svg gs1-en-images.svg gs1-en-music.svg; do
    svg_bytes=$((svg_bytes + $(wc -c < "$WORK/ts-written/$name" | tr -d ' ')))
    cmp -s "$WORK/ts-written/$name" "$WORK/rs-written/$name" && svg_identical=$((svg_identical + 1))
  done
  echo "  TypeScript-written SVG total: $svg_bytes bytes across 4 panels, $svg_identical/4 byte-identical"

  dirty=$(cd "$ROOT" && git status --porcelain -- metrics README.md assets/readme | wc -l | tr -d ' ')
  echo "  git status --porcelain for those paths: $dirty line(s)"
  [ "$dirty" = 0 ] || DIFFS+=("tracked files left dirty")
fi

distinct=$(sort -u "$WORK/payloads.txt" | grep -c . )

echo
echo "cases=$cases agreed=$agreed differed=$differed unsupported=$unsupported"
echo "native-failure cases (no 'error:' line from Bun)=$native_failures"
echo "TypeScript-side categories: summarize lines=$ts_summaries error lines=$ts_errors"
echo "distinct TypeScript payload lines=$distinct"

echo
fail=0
[ "$cases" -ge "$CASE_FLOOR" ] || { echo "FAIL: case floor $CASE_FLOOR not met ($cases)"; fail=1; }
[ "$agreed" -ge "$CASE_FLOOR" ] || { echo "FAIL: every case must agree ($agreed/$cases)"; fail=1; }
[ "$unsupported" = 0 ] || { echo "FAIL: $unsupported case(s) still unsupported"; fail=1; }
[ "$ts_summaries" -ge "$SUMMARY_FLOOR" ] || {
  echo "FAIL: TypeScript summarize-line floor $SUMMARY_FLOOR not met ($ts_summaries)"; fail=1; }
[ "$ts_errors" -ge "$ERROR_FLOOR" ] || {
  echo "FAIL: TypeScript error-line floor $ERROR_FLOOR not met ($ts_errors)"; fail=1; }
[ "$svg_bytes" -ge "$SVG_BYTE_FLOOR" ] || {
  echo "FAIL: TypeScript-written SVG byte floor $SVG_BYTE_FLOOR not met ($svg_bytes)"; fail=1; }
[ "$svg_identical" = 4 ] || {
  echo "FAIL: only $svg_identical/4 SVG panels were byte-identical"; fail=1; }
[ "$distinct" -ge "$PAYLOAD_FLOOR" ] || {
  echo "FAIL: distinct-payload floor $PAYLOAD_FLOOR not met ($distinct); the suite is degenerate"
  fail=1; }
if [ "${#DIFFS[@]}" -gt 0 ]; then
  echo "FAIL: ${#DIFFS[@]} difference(s):"
  printf '  %s\n' "${DIFFS[@]}"
  fail=1
fi
[ "$fail" = 0 ] && echo "PARITY OK"
exit "$fail"
