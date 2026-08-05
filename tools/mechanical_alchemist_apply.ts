#!/usr/bin/env bun
// alchemist.ts only ever WRITES to the real source on an "exact" verdict; an
// "improved" verdict's saved variant sits in a scratch work/ directory and
// is meant for a human to inspect as a lead, per its own `next` field. This
// session found real, repeated value in just applying that saved variant
// directly and re-running alchemist -- it keeps hill-climbing (a fresh
// "improved" with a further move) until it either reaches "exact" or
// "exhausted". Every step is gated by alchemist's own byte-diff, so this
// never installs anything that isn't a genuine measured improvement.
import { execFileSync } from "node:child_process";
import { copyFileSync, readdirSync, readFileSync, unlinkSync } from "node:fs";
import { basename, dirname, join } from "node:path";
import { resolveSpan } from "./alchemist.ts";

const ROOT = dirname(Bun.fileURLToPath(import.meta.url).replace(/^file:\/\//, "")).replace(/\/tools$/, "");
const OVERLAY_BASE = 0x02000000;

interface AlchemistVerdict {
  verdict: "exact" | "improved" | "refused" | "exhausted" | "unnecessary";
  baseline_differing_bytes: number;
  differing_bytes: number;
  saved: string | null;
  moves: string[];
}

function runAlchemist(id: string, write: boolean): AlchemistVerdict | null {
  try {
    const args = ["tools/alchemist.ts", id, "--json"];
    if (write) args.push("--write");
    const output = execFileSync("bun", args, { cwd: ROOT, encoding: "utf8", timeout: 90_000 });
    return JSON.parse(output) as AlchemistVerdict;
  } catch {
    return null;
  }
}

function runAdopt(id: string, source: string, span: number): boolean {
  try {
    execFileSync("bun", ["tools/overlay_adopt.ts", id, "--source", source, "--span", String(span), "--apply"], {
      cwd: ROOT, encoding: "utf8", timeout: 60_000,
    });
    return true;
  } catch {
    return false;
  }
}

// General use-before-write check: for every simple local variable
// declaration (`Type name;`), its first appearance in the function body
// after the declaration must be a write (an assignment `name = ...` or
// `name++`/`name--`/compound-assign), not a bare read. This is what a
// compiler's "used uninitialized" warning checks, applied here because
// alchemist.ts's move generator has proven it can reorder a statement past
// its own dependency's definition (two confirmed bugs this session: a
// split-store move that renamed an assignment to a fresh temp without
// reconnecting a later read, and a plain sink move that moved an
// assignment past an existing read of the same variable) -- both were
// byte-diff-neutral by coincidence (undefined behavior that happened to
// compile identically), so only a source-level check catches them.
function usesBeforeWrite(text: string): Set<string> {
  const declared = [...text.matchAll(/^\s*(?:u8|u16|u32|s8|s16|s32|int|char)\s*\*?\s*([A-Za-z_][A-Za-z0-9_]*)\s*;/gm)]
    .map((match) => match[1]);
  const violations = new Set<string>();
  for (const name of declared) {
    const occurrences = [...text.matchAll(new RegExp(`\\b${name}\\b`, "g"))];
    // Skip the declaration occurrence itself (first match on a line ending in `;` with no other use of name).
    const bodyOccurrences = occurrences.filter((match) => {
      const lineStart = text.lastIndexOf("\n", match.index) + 1;
      const lineEnd = text.indexOf("\n", match.index);
      const line = text.slice(lineStart, lineEnd === -1 ? undefined : lineEnd);
      return !new RegExp(`^\\s*(?:u8|u16|u32|s8|s16|s32|int|char)\\s*\\*?\\s*${name}\\s*;`).test(line);
    });
    if (bodyOccurrences.length === 0) continue;
    const first = bodyOccurrences[0];
    const after = text.slice(first.index + name.length);
    const isWrite = /^\s*(=(?!=)|\+\+|--|\+=|-=|\*=|\/=|\|=|&=|\^=|<<=|>>=)/.test(after);
    if (!isWrite) violations.add(name);
  }
  return violations;
}

// Targeted check using the move's OWN description: resource_375:be0 showed
// that a plain "sink"/"hoist" of a bare `X = ...;` assignment can cross a
// READ of X sitting among the statements it jumps over -- reusing X's stale
// prior value instead of the value the assignment was about to give it. This
// is not "use before ANY write" (X may have been legitimately written much
// earlier in the function as a scratch variable reused many times), so
// usesBeforeWrite() alone can't see it; this walks the exact N adjacent
// lines the move's own "by N" distance says it crossed and checks each for
// a read of the assigned variable.
function moveCrossesOwnRead(beforeText: string, moves: string[]): boolean {
  const lines = beforeText.split("\n");
  for (const move of moves) {
    const match = /^(sink|hoist)\s*\[[^\]]*\]\s*"(.*)"\s*by\s*(\d+)/.exec(move);
    if (match === null) continue;
    const [, direction, statement, distanceText] = match;
    const distance = Number.parseInt(distanceText, 10);
    const assign = /^\s*([A-Za-z_][A-Za-z0-9_]*)\s*=(?!=)/.exec(statement);
    if (assign === null) continue; // not a plain local-variable assignment; store-through-pointer targets memory, not a local
    const variable = assign[1];
    const statementLine = lines.findIndex((line) => line.trim() === statement.trim());
    if (statementLine === -1) continue;
    // "sink by N" moves the statement DOWN past the N statements that used
    // to follow it; "hoist by N" moves it UP past the N that used to
    // precede it. Either way, those are exactly the lines it now sits on
    // the other side of.
    const crossedStart = direction === "sink" ? statementLine + 1 : Math.max(0, statementLine - distance);
    const crossedEnd = direction === "sink" ? statementLine + distance : statementLine - 1;
    for (let index = crossedStart; index <= crossedEnd && index < lines.length; index++) {
      const line = lines[index];
      const occurrence = new RegExp(`\\b${variable}\\b`).exec(line);
      if (occurrence === null) continue;
      const isWrite = /^\s*([A-Za-z_][A-Za-z0-9_]*)\s*=(?!=)/.test(line) && line.trim().startsWith(variable);
      if (!isWrite) return true; // a read of the assigned variable sits between the statement's old and new position
    }
  }
  return false;
}

async function main(): Promise<void> {
  const limitArg = Bun.argv.find((arg) => arg.startsWith("--limit="));
  const limit = limitArg !== undefined ? Number.parseInt(limitArg.split("=")[1], 10) : Infinity;
  // --shard=I/N: process only files where index % N === I, so multiple
  // instances can run concurrently over disjoint file sets (each spawns its
  // own alchemist.ts/overlay_adopt.ts subprocesses, so no two instances ever
  // touch the same file).
  const shardArg = Bun.argv.find((arg) => arg.startsWith("--shard="));
  const [shardIndex, shardCount] = shardArg !== undefined
    ? shardArg.split("=")[1].split("/").map(Number)
    : [0, 1];
  const onlyArg = Bun.argv.find((arg) => arg.startsWith("--only="));
  const only = onlyArg !== undefined ? new Set(onlyArg.split("=")[1].split(",")) : null;
  const semanticDir = join(ROOT, "semantic");
  const files = readdirSync(semanticDir)
    .filter((name) => /^resource_[0-9a-f]+_c_[0-9a-f]{8}\.c$/i.test(name))
    .filter((name) => only === null || only.has(basename(name, ".c")))
    .sort()
    .filter((_name, index) => only !== null || index % shardCount === shardIndex)
    .slice(0, limit);

  let closed = 0;
  let improved = 0;
  let unchanged = 0;

  for (const name of files) {
    const path = join(semanticDir, name);
    const match = /^(resource_[0-9a-f]+)_c_([0-9a-f]{8})$/i.exec(basename(name, ".c"));
    if (match === null) continue;
    const overlay = match[1];
    const address = Number.parseInt(match[2], 16);
    const id = `${overlay}:${(address - OVERLAY_BASE).toString(16)}`;

    const initial = runAlchemist(id, false);
    if (initial === null || initial.baseline_differing_bytes === 0) continue;
    const startBytes = initial.baseline_differing_bytes;
    // A real statement sink/hoist relocates ONE statement by 1-3 positions;
    // it can never legitimately change the file's line count by more than a
    // handful of lines. resource_371:28e8 found a real bug in alchemist.ts's
    // renderFunction() (triggered by a chain of textually-identical `else if`
    // conditions): its very first "improved" verdict silently duplicated 16
    // lines of an empty else-if branch, byte-diff-neutral (empty branches
    // compile to nothing) but a nonsense source. Refuse any saved variant
    // whose line count grew past a small, generous bound -- this can never
    // reject a genuine sink/hoist, only a renderer bug like this one.
    const startLines = readFileSync(path, "utf8").split("\n").length;

    let steps = 0;
    let lastBytes = startBytes;
    while (steps < 15) {
      const result = runAlchemist(id, true);
      if (result === null) break;
      if (result.verdict === "exact" || result.verdict === "unnecessary") {
        const span = resolveSpan(overlay, address - OVERLAY_BASE);
        if (span !== undefined && runAdopt(id, path, span)) {
          unlinkSync(path); // overlay_adopt.ts --apply does not do this itself
          closed++;
          console.log(`${id}: CLOSED (was ${startBytes} differing bytes)`);
        }
        break;
      }
      if (result.verdict !== "improved" || result.saved === null) break;
      const savedLines = readFileSync(result.saved, "utf8").split("\n").length;
      if (savedLines > startLines + 10) {
        console.log(`${id}: REFUSED a saved variant (${startLines} -> ${savedLines} lines, likely a renderer bug, not a real move)`);
        break;
      }
      const beforeText = readFileSync(path, "utf8");
      const beforeViolations = usesBeforeWrite(beforeText);
      const afterViolations = usesBeforeWrite(readFileSync(result.saved, "utf8"));
      const newViolations = [...afterViolations].filter((name) => !beforeViolations.has(name));
      if (newViolations.length > 0) {
        console.log(`${id}: REFUSED a saved variant (introduces a use-before-write on ${newViolations.join(",")}, likely undefined behavior that happened to byte-match)`);
        break;
      }
      if (moveCrossesOwnRead(beforeText, result.moves)) {
        console.log(`${id}: REFUSED a saved variant (a sink/hoist crossed a read of the variable it assigns, likely reuses a stale value)`);
        break;
      }
      copyFileSync(result.saved, path);
      lastBytes = result.differing_bytes;
      steps++;
    }
    if (lastBytes < startBytes) {
      console.log(`${id}: ${startBytes} -> ${lastBytes} differing bytes`);
      improved++;
    } else {
      unchanged++;
    }
  }

  console.log(`closed=${closed} improved=${improved} unchanged=${unchanged} total=${files.length}`);
}

if (import.meta.main) await main();
