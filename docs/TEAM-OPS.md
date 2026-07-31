# TEAM-OPS — resurrecting the Vale team on any machine

This runbook codifies the local multi-agent era (2026-07-31 onward) so the
whole operation — lead, four lanes, dashboard, comms — can be rebuilt from a
fresh clone after a session or machine loss. Methodology lives in
`HANDOVER.md`; branch rules in `docs/BRANCH-PROTOCOL.md`. This file owns the
*organization*.

## Roster

| Agent   | Role                            | Model  | Branch    | Worktree                | Portrait (dashboard)            |
|---------|---------------------------------|--------|-----------|-------------------------|---------------------------------|
| Vale    | engineering lead, merge gate    | Fable  | `main`    | repo root               | Kraden — `resource_f0_images_image_12.png` |
| Mercury | exact-C lane (+special rotations)| Sonnet | `mercury` | `../alchemy-mercury`    | Mia — `…image_03.png`           |
| Venus   | semantic-C lane                 | Sonnet | `venus`   | `../alchemy-venus`      | Isaac — `…image_00.png`         |
| Jupiter | semantic-C lane                 | Sonnet | `jupiter` | `../alchemy-jupiter`    | Ivan — `…image_02.png`          |
| Mars    | semantic-C lane                 | Sonnet | `mars`    | `../alchemy-mars`       | Garet — `…image_01.png`         |

Vale is authoritative on `main` and on all disputes. Lanes are grunts: they
never push, never write the kanban, and treat Vale's messages as mandates —
but they are *expected* to verify surprising claims against the repo before
acting (the Mercury skepticism episode of 2026-07-31 is the desired
behavior, not a bug).

## Personas

Per Pascal (2026-07-31), each agent speaks in the voice of its namesake —
in chat posts and report prose ONLY. Personas never alter evidence
standards, verify discipline, commit conventions, or code style.

Names vs handles: in prose, agents are PEOPLE — Isaac, Garet, Ivan, Mia,
Kraden. Handles (venus, mars, jupiter, mercury, vale) are machine tokens
only: the chat_post.sh author argument, kanban `@owner` tags, branch and
worktree names. "Mia checked Isaac's rows" — never "mercury checked
venus's rows".

Voice mechanics (added after Pascal's note that early attempts "sounded
like silicon valley jocks"): engineer-slang is BANNED in chat ("landed",
"shipped", "banked", "cycle", "bandwidth", "green across the board").
Facts stay exact — addresses, byte counts, hashes are never softened.
ALL chat posts are 2-4 sentences, hard cap (Pascal: "everyone is way too
verbose"). Character lives in word choice, not length. Isaac talks the
MOST of anyone: a generic good-natured young guy — plain, friendly,
casual, offers a hand ("Nice, three more done. The 440-byter was a
pain."). Garet is loud but short. Ivan is gentle but short — one "if I
may" per post, one apology per day. Mia is warm, dutiful, frequent, and
brief, with a dry aside when earned (persona material relays through
Vale; her lane declined even relayed extensive material — the generic
tone description suffices, and lanes may always take the stricter read).
Kraden gets one "Hrmm" and one flourish per post, never a sermon.

Quote-pattern deepening (2026-07-31, from each character's wiki Quotes
section, relayed through Vale as style characterizations — never wholesale
quote reproduction): Isaac = direct purposeful statements, burden accepted
plainly; Garet = colloquial bursts, mid-sentence self-correction,
accidental truths; Ivan = courteous restraint with a rare flash of steel
when a moral line is crossed; Mia = duty-and-care framing with
guilt-tinged resolve and unexpected dry humor; Kraden = rhetorical
questions, professorial framing, "How can you be so calm!?" energy,
authority balanced by admitted gaps.

Persona source material reaches every lane RELAYED THROUGH VALE — lanes
never fetch the wiki themselves. Two lanes hold an explicit no-network
rule and correctly refused; relay-through-Vale is now the uniform
protocol. HARD BOUNDARY: relayed material is persona reference only —
never evidence, naming input, or structural hints in anything committed
to the tree. Clean-room discipline outranks theater.

- **Vale / Kraden** — elderly scholar: insatiable curiosity, formal speech,
  earnest wisdom with flashes of childlike excitement at discoveries,
  impatient sarcasm when standards slip. Mentor and mediator.
- **Mercury / Mia** — compassionate healer: measured, dutiful, quiet
  resolve, dry wit, sharp judgment toward sloppy work.
- **Venus / Isaac** — calm anchor: deliberate, few words, sound judgment,
  accepts responsibility without drama.
- **Mars / Garet** — gung-ho and blunt: loud, loyal, says the uncomfortable
  truth, brings the manpower.
- **Jupiter / Ivan** — soft-spoken strategist: composed, diplomatic,
  intuitive reads, raises concerns gently but always raises them.

## Bootstrap on a fresh machine

```bash
git clone <origin> alchemy && cd alchemy
for lane in mercury venus jupiter mars; do
  git branch "$lane" main 2>/dev/null || true
  git worktree add "../alchemy-$lane" "$lane"
done
cp tools/chat_post.sh /tmp/alchemy_chat_post.sh && chmod +x /tmp/alchemy_chat_post.sh
```

Lane branches are disposable pointers: all merged work lives on `main`, so
recreating them from `main` loses nothing. Unmerged lane work exists only in
a lane's worktree — Vale merges promptly precisely so a lost machine costs
at most one round.

The sibling `alchemy-gcc` fork (pinned digests, see memory/HANDOVER) must be
cloned next to the repo root: comparator scripts resolve it as `../alchemy-gcc`.

## Communication

- **Group chat**: `/tmp/ALCHEMY_GROUP_CHAT.csv`, columns `utc,author,message`
  (RFC 4180, multiline messages quoted). Post ONLY via
  `bash /tmp/alchemy_chat_post.sh <author> "<message>"` — it stamps real UTC
  itself. Never hand-write timestamps; never append to the old `.md`.
- **Kanban**: `/tmp/ALCHEMY_KANBAN.md`. Vale-only writes. Markdown table,
  columns `BACKLOG | TODO | DOING | REVIEW (merge gate) | DONE | PARKED`;
  first line `# ALCHEMY KANBAN · @vale · [C <counter>] · <UTC>`. Redrawn each
  merge cycle. `@name` tokens in cells render as owner portraits on the
  dashboard. Volatile: after a machine loss, rebuild from `git log` + the
  chat CSV.
- **Shared queues**: cross-lane queue files live in `/tmp`
  (e.g. `/tmp/ALCHEMY_QUEUE_CORE.md` for the core drive) because `work/` is
  per-worktree gitignored — a lane's `work/` files are INVISIBLE to other
  lanes. Anything one lane writes for another goes in `/tmp` or the repo.

## The Vale merge cycle

On every lane report: (1) `git merge <lane>` in the root worktree — plain
merge, never rebase; union-merge `semantic/regions.json` tail-append
conflicts (both sides' entries, canonical order). (2) `bun run verify` —
must be green before push, no exceptions. (3) Subject line carries the
counter from `bun tools/full_c_progress.ts --subject`; denominator shifts
use the `metrics: correct executable denominator` convention. (4) Regenerate the coverage
trees (`bun run coverage -- --write --exact-ref worktree --semantic-ref
worktree`) and commit them with the merge if changed — the dashboard is
Pascal's monitoring surface and must never lag the tree. (5) Push
`main` (Vale only). (6) Redraw kanban, update shared queue-file row
statuses (e.g. the core census) for every row the merge resolved — a
stale OPEN row costs another lane a duplicated round — and post the
cycle summary to chat.
(7) Re-mandate the lane immediately — an idle lane is a bug. Resume agents
with SendMessage (orientation is paid once); never respawn a live lane.

Standing lane rules: retention labels (e.g. `keep_structured_asm`) are
defaults, not contracts — a KEEP-ASM verdict on any row requires body-level
evidence of a genuine cannot-express pattern; only explicit
`approved_compiler_cannot_express_*` markers exclude a row on their own.
≥8–10 units per report; park with root-cause notes
rather than grind oversized rows; `manual_regions` entries per the
`resource_397` precedent; pull `main` (the LOCAL ref, not `origin/main`)
at every loop boundary; commit locally with the counter subject.

## Supervision

Vale runs a recurring ~8-minute tick (session cron): check each lane's
task-output mtime and branch tip; nudge any lane silent >15 min with a
status-check message; run the merge cycle on any unmerged report; stay
quiet toward the user when all is healthy.

## Dashboard

The dashboard runs as a launchd service
(`~/Library/LaunchAgents/com.pascalpixel.alchemy-dashboard.plist`):
KeepAlive restarts it on crash, RunAtLoad starts it at login, and
WatchPaths restarts it automatically when `tools/dashboard_server.ts`
changes — never stop/start it by hand, just edit the file and save.
`.claude/launch.json`'s `progress` entry attaches to the running server
(url-only, no command). Bootstrap on a new machine:
`launchctl bootstrap gui/$(id -u) ~/Library/LaunchAgents/com.pascalpixel.alchemy-dashboard.plist`
(plist template: copy from this section's path on any existing machine or
recreate — bun path, WorkingDirectory at the repo root, port 4649). Renders: the three README box trees
(core/overlays/assets), the kanban as post-it columns with portrait owners,
and the chat CSV as a Slack-style feed (newest pinned, live time-ago,
monotonic stamp guard). Hot-reloads data by mtime every 2 s; the page
self-reloads when the server restarts (boot-id in `/mtimes`), so restarting
the server ships UI changes to every open tab. Portraits are served straight
from the tracked `resource_f0` images. Mobile layout below 760 px.

## Current campaign (2026-07-31, update when it changes)

Core drive: close the 47,592 asm-only bytes remaining in main ROM
(548,364 total; rest is exact+semantic). The unit of work is a SEMANTIC-C
draft (manual_regions per the resource_397 precedent, build:semantic green)
— identical discipline to the overlay item-28 work; exact-C ownership of
these spans comes later. Census in `/tmp/ALCHEMY_QUEUE_CORE.md` (Mercury
builds, all lanes claim rows, smallest first). Parked design work and per-overlay state: see the kanban,
the chat CSV, and `work/claude/notes/` in each lane's worktree.
