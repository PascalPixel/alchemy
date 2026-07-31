#!/usr/bin/env bash
#
# Venus Lighthouse: pull `origin/mercury` into `venus` in ONE atomic step.
#
#   tools/venus_pull.sh
#
# Why this is a script rather than a sequence of commands: a `git merge` that
# conflicts leaves conflict markers in the WORKING TREE, and concurrent lanes
# read that tree. A conflicted `package.json` is worse than it sounds — `bun`
# fails to parse it, so `bun run <anything>` stops working, including
# `bun run build:semantic`. Three separate lanes reported losing part of a
# session to exactly that window while the merge owner resolved conflicts across
# separate tool calls.
#
# Doing merge-and-resolve in one process closes the window to milliseconds.
#
# Resolution rules, all of them load-bearing:
#   * MEETING.md   keep BOTH sides, re-ordered by timestamp (Vale's rule). An
#                  entry lost to a merge is worse than a duplicate.
#   * HANDOVER.md  keep both sides; the sections rarely overlap.
#   * package.json UNION the test chains. This is NOT always a no-op — it caught
#                  a self-test Mercury had added and Venus lacked. Taking a side
#                  silently shrinks the chain. Union then strip the trailing
#                  comma it leaves, which `bun` tolerates and `JSON.parse` does
#                  not.
#   * src/, tools/alchemy_gcc.ts  take Mercury's on add/add; it owns the exact
#                  lane and its routing.
#
# Marker detection follows Mercury's rule in check_publication.ts: a marker is
# `<<<<<<< ` WITH the trailing space, anchored to line start. Without that, this
# script trips on prose — MEETING.md itself discusses conflict markers in
# backticks, and an unanchored substring test reported that file as conflicted
# when it was clean.
set -uo pipefail
cd "$(dirname "$0")/.." || exit 1

# REFUSE TO MERGE OVER A LANE'S UNBANKED WORK.
#
# A repair lane reported its uncommitted edits being reverted twice mid-session.
# The mechanism is this script: `git merge` (and the `git checkout --theirs`
# resolution below) will happily replace a file that a concurrent lane is part
# way through writing, and the lane has no way to notice. An hour of a lane's
# analysis is worth far more than a merge landing promptly.
#
# Banking first is always the right answer: a committed file cannot be lost, and
# `venus_bank.sh` already knows how to bank while another gate is red.
DIRTY=$(git status --porcelain -- semantic/ src/ assets/code/ | grep -c .)
if [ "$DIRTY" -gt 0 ] && [ "${ALLOW_DIRTY_MERGE:-}" != "1" ]; then
  echo "REFUSING TO MERGE: $DIRTY unbanked file(s) under semantic/, src/ or assets/code/."
  echo "A concurrent lane may be mid-write; merging can silently revert its work."
  echo "Bank first (tools/venus_bank.sh \"<subject>\"), then re-run."
  echo "(override with ALLOW_DIRTY_MERGE=1 only if you know the tree is yours)"
  git status --porcelain -- semantic/ src/ assets/code/ | head -5
  exit 1
fi

git fetch origin mercury >/dev/null 2>&1 || { echo "fetch failed"; exit 1; }
AHEAD=$(git rev-list --count venus..origin/mercury)
echo "mercury ahead: $AHEAD"
[ "$AHEAD" -eq 0 ] && { echo "nothing to merge"; exit 0; }

git merge origin/mercury --no-commit >/dev/null 2>&1

# Resolve every conflict before returning to the caller, so no lane ever sees a
# broken tree.
python3 - <<'RESOLVE'
import json, re, subprocess

def sides(text):
    return re.compile(r'<<<<<<< HEAD\n(.*?)=======\n(.*?)>>>>>>> origin/mercury\n', re.S)

def keep_both_by_timestamp(match):
    ours, theirs = match.group(1), match.group(2)
    if ours.strip() == theirs.strip():
        return ours
    entries = lambda text: [e for e in re.split(r'(?m)^(?=### )', text) if e.strip()]
    seen, merged = set(), []
    for entry in entries(ours) + entries(theirs):
        key = entry.strip()[:120]
        if key in seen:
            continue
        seen.add(key)
        merged.append(entry)
    stamp = lambda e: (re.search(r'(\d{4}-\d{2}-\d{2}T\d{2}:\d{2}Z)', e) or [None, ""])[0] or ""
    merged.sort(key=stamp, reverse=True)
    return "".join(merged)

for path in ("MEETING.md", "HANDOVER.md"):
    try:
        text = open(path).read()
    except FileNotFoundError:
        continue
    if not re.search(r'(?m)^<<<<<<< ', text):
        continue
    resolver = keep_both_by_timestamp if path == "MEETING.md" else (
        lambda m: m.group(1) if m.group(1).strip() == m.group(2).strip() else m.group(1) + m.group(2))
    # Conflicts can NEST — a non-greedy match takes the inner pair and leaves the
    # outer, which then reaches the publication gate as a stray marker. Loop
    # until the file is clean rather than substituting once.
    count = 0
    for _ in range(8):
        text, n = sides(text).subn(resolver, text)
        count += n
        if not re.search(r'(?m)^<<<<<<< ', text):
            break
    open(path, "w").write(text)
    status = "clean" if not re.search(r'(?m)^<<<<<<< ', text) else "STILL CONFLICTED"
    print(f"{path}: {count} hunk(s) resolved, {status}")

text = open("package.json").read()
match = re.search(r'<<<<<<< HEAD\n(.*?)\n=======\n(.*?)\n>>>>>>> origin/mercury\n', text, re.S)
if match:
    commands = lambda line: re.search(r'"test": "(.*)"', line).group(1).split(" && ")
    ours, theirs = commands(match.group(1)), commands(match.group(2))
    union = list(ours) + [c for c in theirs if c not in ours]
    gained = [c for c in theirs if c not in ours]
    if gained:
        print("package.json: union gained", gained)
    text = text[:match.start()] + '    "test": "' + " && ".join(union) + '",\n' + text[match.end():]
    # Taking both sides of an object member leaves a trailing comma.
    text = re.sub(r',(\s*\n\s*\})', r'\1', text)
    open("package.json", "w").write(text)
    json.load(open("package.json"))
    print(f"package.json: valid, {len(union)} commands")

remaining = subprocess.run(["git", "diff", "--name-only", "--diff-filter=U"],
                           capture_output=True, text=True).stdout.split()

# Overlay sources conflict on ONE recurring thing: a declaration's return type.
# The rename pass collapses two pre-rule names onto one import, and the two
# branches can disagree about whether the survivor is `void` or typed. The typed
# side is right whenever a call site consumes the result — and a `void`
# declaration against a consuming call site is a HARD COMPILE ERROR, while the
# reverse merely loses information. So prefer the side that declares a type.
declaration = re.compile(r'<<<<<<< HEAD\n(.*?)=======\n(.*?)>>>>>>> [^\n]*\n', re.S)
def prefer_typed(match):
    ours, theirs = match.group(1), match.group(2)
    void_only = lambda text: all(
        line.strip() == "" or line.lstrip().startswith("void ")
        for line in text.splitlines())
    if void_only(ours) and not void_only(theirs):
        return theirs
    if void_only(theirs) and not void_only(ours):
        return ours
    return ours + theirs          # not this case; keep both and let the gate speak
for path in list(remaining):
    if not path.startswith("semantic/overlays/") or not path.endswith(".c"):
        continue
    text = open(path).read()
    resolved, count = declaration.subn(prefer_typed, text)
    if count == 0 or re.search(r'(?m)^<<<<<<< ', resolved):
        continue
    open(path, "w").write(resolved)
    subprocess.run(["git", "add", "--", path])
    remaining.remove(path)
    print(f"{path}: {count} declaration conflict(s) resolved, kept the typed side")
for path in remaining:
    if path.startswith("src/") or path == "tools/alchemy_gcc.ts":
        subprocess.run(["git", "checkout", "--theirs", "--", path])
        subprocess.run(["git", "add", "--", path])
        print(f"{path}: took mercury's")

# Stage the files resolved above. Editing the working tree does NOT clear a
# path's unmerged state — only `git add` does — so without this the caller's
# unresolved check fires on files that are already correct.
for path in ("MEETING.md", "HANDOVER.md", "package.json"):
    if path in remaining:
        subprocess.run(["git", "add", "--", path])
        print(f"{path}: staged")
RESOLVE

# Anything still conflicted is genuinely novel and wants a human decision.
STILL=$(git diff --name-only --diff-filter=U)
if [ -n "$STILL" ]; then
  echo "UNRESOLVED, needs attention:"; echo "$STILL"; exit 1
fi

# Exact C supersedes semantic C; the build hard-errors if one is missed.
SUPERSEDED=$(bun tools/semantic_superseded.ts 2>/dev/null)
if [ -n "$SUPERSEDED" ]; then
  echo "$SUPERSEDED" | xargs -r git rm -q
  echo "deleted $(echo "$SUPERSEDED" | grep -c .) superseded semantic source(s)"
fi

bun tools/semantic_regions_sync.ts --write 2>&1 | tail -1
git add -A
echo "merge resolved; tree is consistent"
