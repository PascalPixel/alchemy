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
      // resource_3c5:28a0 found a second real bug: a split-store move can
      // rename a variable's ASSIGNMENT to a fresh `permuted_N` temp without
      // correctly threading every later read of the original name back
      // through it, leaving the original genuinely uninitialized. Byte-diff-
      // neutral by coincidence (undefined behavior that happened to compile
      // to the same bytes), so only a source-level check catches it: every
      // `X = ...;` line the diff removed in favor of `permuted_N = ...;`
      // must be followed somewhere by `X = permuted_N` reconnecting it.
      const beforeText = readFileSync(path, "utf8");
      const afterText = readFileSync(result.saved, "utf8");
      const removedAssignments = beforeText.split("\n")
        .filter((line) => !afterText.includes(line))
        .map((line) => /^\s*([A-Za-z_][A-Za-z0-9_]*)\s*=(?!=)/.exec(line)?.[1])
        .filter((name): name is string => name !== undefined);
      const newTemps = [...afterText.matchAll(/\bpermuted_\d+\b/g)].map((match) => match[0]);
      const uniqueTemps = [...new Set(newTemps)];
      const brokenSplit = removedAssignments.some((name) =>
        uniqueTemps.some((temp) => afterText.includes(`${temp} = `) && !new RegExp(`\\b${name}\\s*=\\s*${temp}\\b`).test(afterText)),
      ) && uniqueTemps.length > 0;
      if (brokenSplit) {
        console.log(`${id}: REFUSED a saved variant (a split temp was never reconnected to its original variable, likely leaves it uninitialized)`);
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
