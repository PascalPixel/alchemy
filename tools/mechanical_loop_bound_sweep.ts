#!/usr/bin/env bun
// Mechanical fix for the "wrong loop exit test" bug, found by hand on
// resource_37b:0c8c: a counted loop written `for (i = 0; i < 30; i++)`
// compiles to `cmp rN, #29 / bls`, while the reference reads `cmp rN, #30 /
// bne`. The two are the same loop; only the exit TEST differs, and gcc emits
// the `bne` form when the source says `i != 30`. On that one owner the
// rewrite closed six of the seven residual instruction groups and took the
// byte diff from 44 to 16, so the defect is worth sweeping rather than
// re-deriving per file.
//
// The rewrite is applied per loop, greedily: a file often mixes loops the
// original wrote as `<` with loops it wrote as `!=`, so flipping all of them
// at once can be a net regression even when one of them is right. Each
// candidate loop is flipped on its own and kept only if alchemist.ts's byte
// diff improves -- same gate and same discipline as
// mechanical_callsym_sweep.ts, including the unconditional adopt attempt,
// which is the project's real definition of exact.
//
//   bun tools/mechanical_loop_bound_sweep.ts [--limit=N] [--only=stem,stem]
import { execFileSync } from "node:child_process";
import { readFileSync, readdirSync, unlinkSync, writeFileSync } from "node:fs";
import { basename, dirname, join } from "node:path";
import { resolveSpan } from "./alchemist.ts";
import { auditedCodeSpan } from "./overlay_adopt.ts";

const ROOT = dirname(Bun.fileURLToPath(import.meta.url).replace(/^file:\/\//, "")).replace(/\/tools$/, "");
const OVERLAY_BASE = 0x02000000;

// `for (<name> = 0; <name> < <literal>; <name>++)`, with the same name in all
// three clauses. Anything else -- a non-zero start, a variable bound, a
// stride other than one -- is left alone: `!=` is only equivalent to `<` when
// the counter is guaranteed to hit the bound exactly.
const COUNTED_LOOP =
  /for\s*\(\s*(\w+)\s*=\s*0\s*;\s*\1\s*<\s*(0x[0-9a-f]+|\d+)\s*;\s*\1\s*\+\+\s*\)/gi;

/** Every counted-loop match in `text`, in source order. */
function countedLoops(text: string): RegExpExecArray[] {
  const found: RegExpExecArray[] = [];
  COUNTED_LOOP.lastIndex = 0;
  for (let match = COUNTED_LOOP.exec(text); match !== null; match = COUNTED_LOOP.exec(text)) {
    found.push(match);
  }
  return found;
}

/** `text` with the loop at `match` rewritten to the `!=` exit test. */
function flipped(text: string, match: RegExpExecArray): string {
  const rewritten = match[0].replace(`${match[1]} <`, `${match[1]} !=`).replace(`${match[1]}<`, `${match[1]} !=`);
  return text.slice(0, match.index) + rewritten + text.slice(match.index + match[0].length);
}

interface AlchemistVerdict {
  verdict: "exact" | "improved" | "refused" | "exhausted" | "unnecessary";
  baseline_differing_bytes: number;
  differing_bytes: number;
}

function runAlchemist(id: string): AlchemistVerdict | null {
  try {
    const output = execFileSync("bun", ["tools/alchemist.ts", id, "--json"], {
      cwd: ROOT,
      encoding: "utf8",
      timeout: 120_000,
    });
    return JSON.parse(output) as AlchemistVerdict;
  } catch {
    return null;
  }
}

function runAdopt(id: string, source: string): boolean {
  try {
    const [overlay, offsetText] = id.split(":");
    const offset = Number.parseInt(offsetText, 16);
    const span = auditedCodeSpan(overlay, OVERLAY_BASE + offset) ?? resolveSpan(overlay, offset);
    const args = ["tools/overlay_adopt.ts", id, "--source", source, "--apply"];
    if (span !== undefined) args.push("--span", String(span));
    execFileSync("bun", args, { cwd: ROOT, encoding: "utf8", timeout: 120_000 });
    return true;
  } catch {
    return false;
  }
}

async function main(): Promise<void> {
  const limitArg = Bun.argv.find((arg) => arg.startsWith("--limit="));
  const limit = limitArg !== undefined ? Number.parseInt(limitArg.split("=")[1], 10) : Infinity;
  const onlyArg = Bun.argv.find((arg) => arg.startsWith("--only="));
  const only = onlyArg !== undefined ? new Set(onlyArg.split("=")[1].split(",")) : null;
  const semanticDir = join(ROOT, "semantic");
  const files = readdirSync(semanticDir)
    .filter((name) => /^resource_[0-9a-f]+_c_[0-9a-f]{8}\.c$/i.test(name))
    .filter((name) => only === null || only.has(basename(name, ".c")))
    .map((name) => join(semanticDir, name))
    .slice(0, limit);

  let closed = 0;
  let improved = 0;
  let skipped = 0;

  for (const path of files) {
    const base = basename(path, ".c");
    const match = /^(resource_[0-9a-f]+)_c_([0-9a-f]{8})$/i.exec(base);
    if (match === null) { skipped++; continue; }
    const overlay = match[1];
    const offset = Number.parseInt(match[2], 16) - OVERLAY_BASE;
    const id = `${overlay}:${offset.toString(16)}`;
    if (resolveSpan(overlay, offset) === undefined) { skipped++; continue; }

    const original = readFileSync(path, "utf8");
    if (countedLoops(original).length === 0) { skipped++; continue; }

    const before = runAlchemist(id);
    if (before === null || before.baseline_differing_bytes === 0) { skipped++; continue; }

    let best = original;
    let bestBytes = before.baseline_differing_bytes;
    // Greedy: re-scan after each accepted flip, since accepting one shifts
    // the offsets of every later match.
    for (let index = 0; index < countedLoops(best).length; index++) {
      const candidate = flipped(best, countedLoops(best)[index]);
      if (candidate === best) continue;
      writeFileSync(path, candidate);
      const after = runAlchemist(id);
      if (after !== null && after.differing_bytes < bestBytes) {
        best = candidate;
        bestBytes = after.differing_bytes;
      }
    }

    if (best === original) {
      writeFileSync(path, original);
      skipped++;
      continue;
    }

    writeFileSync(path, best);
    if (runAdopt(id, path)) {
      unlinkSync(path);
      closed++;
      console.log(`${id} CLOSED via mechanical loop-bound fix, was ${before.baseline_differing_bytes} differing bytes`);
      continue;
    }
    improved++;
    console.log(`${id} IMPROVED via mechanical loop-bound fix: ${before.baseline_differing_bytes} -> ${bestBytes} differing bytes`);
  }

  console.log(`closed=${closed} improved=${improved} skipped=${skipped} total=${files.length}`);
}

await main();
