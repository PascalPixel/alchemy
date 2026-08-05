#!/usr/bin/env bun
// Main-image counterpart to mechanical_unsign_sweep.ts: same evidenced
// signed/unsigned loop-bound transform, same "only keep what a real byte
// comparison proves helps" discipline, but for semantic/<address>.c files
// via verifyCandidate against the real ROM (there is no main-image
// equivalent of alchemist.ts's licensed-move search, so this only tests the
// type flip itself -- still gated entirely by the byte oracle).
import { execFileSync } from "node:child_process";
import { mkdirSync, readFileSync, readdirSync, writeFileSync } from "node:fs";
import { basename, dirname, join } from "node:path";
import { ROM_BASE, verifyCandidate } from "./match_m2c.ts";

const ROOT = dirname(Bun.fileURLToPath(import.meta.url).replace(/^file:\/\//, "")).replace(/\/tools$/, "");

const FOR_PATTERN = /for\s*\(\s*(\w+)\s*=\s*0\s*;\s*\1\s*<=?\s*(\d+)/g;
const WHILE_PATTERN = /while\s*\(\s*(\w+)\s*<=?\s*(\d+)\s*\)/g;

interface Candidate { name: string }

function findCandidates(text: string): Candidate[] {
  const names = new Set<string>();
  for (const pattern of [FOR_PATTERN, WHILE_PATTERN]) {
    pattern.lastIndex = 0;
    let match: RegExpExecArray | null;
    while ((match = pattern.exec(text)) !== null) names.add(match[1]);
  }
  const candidates: Candidate[] = [];
  for (const name of names) {
    const declaration = new RegExp(`\\b(s32|int)\\s+${name}\\s*[;=]`);
    if (declaration.test(text)) candidates.push({ name });
  }
  return candidates;
}

function applyTransform(text: string, candidates: Candidate[]): string {
  let output = text;
  for (const { name } of candidates) {
    output = output.replace(
      new RegExp(`\\b(s32|int)(\\s+${name}\\s*[;=])`, "g"),
      (_full, kind: string, rest: string) => (kind === "s32" ? `u32${rest}` : `unsigned int${rest}`),
    );
  }
  return output;
}

// Compares against a FIXED-length expected slice, never the candidate's own
// length. verifyCandidate() derives its own "expected" by slicing the ROM to
// the CANDIDATE's self-reported linked size -- sound for a single compile,
// but comparing two independent compiles this way is not: if a transform
// changes the compiled size (which a real code edit legitimately can), the
// "before" and "after" checks silently measure two different ROM byte
// ranges, and any resulting "improvement" is meaningless. Anchoring both
// checks to the size measured BEFORE the transform (matching how
// alchemist.ts always diffs against a fixed, independently-sourced span)
// makes the comparison valid; a transform that changes the linked size no
// longer produces a false "improved" reading, it fails this shape check.
function differingBytes(actual: Uint8Array, expected: Uint8Array): number {
  let differing = Math.abs(actual.length - expected.length);
  const limit = Math.min(actual.length, expected.length);
  for (let index = 0; index < limit; index++) {
    if (actual[index] !== expected[index]) differing++;
  }
  return differing;
}

async function main(): Promise<void> {
  const limitArg = Bun.argv.find((arg) => arg.startsWith("--limit="));
  const limit = limitArg !== undefined ? Number.parseInt(limitArg.split("=")[1], 10) : Infinity;
  const onlyArg = Bun.argv.find((arg) => arg.startsWith("--only="));
  const only = onlyArg !== undefined ? new Set(onlyArg.split("=")[1].split(",")) : null;
  const semanticDir = join(ROOT, "semantic");
  const files = readdirSync(semanticDir)
    .filter((name) => /^[0-9a-f]{8}\.c$/i.test(name))
    .filter((name) => only === null || only.has(basename(name, ".c")))
    .map((name) => join(semanticDir, name))
    .slice(0, limit);
  const rom = readFileSync(join(ROOT, "roms/gs1-en.gba"));
  const work = join(ROOT, "work", "mechanical-unsign-main");

  let closed = 0;
  let improved = 0;
  let reverted = 0;
  let skipped = 0;

  for (const path of files) {
    const original = readFileSync(path, "utf8");
    const candidates = findCandidates(original);
    if (candidates.length === 0) { skipped++; continue; }

    const beforeWork = join(work, `${basename(path, ".c")}-before`);
    const afterWork = join(work, `${basename(path, ".c")}-after`);
    mkdirSync(beforeWork, { recursive: true });
    mkdirSync(afterWork, { recursive: true });

    let before: { actual: Uint8Array; expected: Uint8Array };
    try {
      before = await verifyCandidate(path, rom, beforeWork, [], ROM_BASE, "gs1", { family: "routed", removeFlags: [] });
    } catch {
      skipped++; continue;
    }
    const preBytes = differingBytes(before.actual, before.expected);
    if (preBytes === 0) { skipped++; continue; }

    const transformed = applyTransform(original, candidates);
    if (transformed === original) { skipped++; continue; }
    writeFileSync(path, transformed);

    let after: { actual: Uint8Array; expected: Uint8Array };
    try {
      after = await verifyCandidate(path, rom, afterWork, [], ROM_BASE, "gs1", { family: "routed", removeFlags: [] });
    } catch (error) {
      writeFileSync(path, original);
      reverted++;
      console.log(`${basename(path, ".c")} REVERTED (compile error after transform: ${(error as Error).message.slice(0, 150)})`);
      continue;
    }
    if (after.actual.length !== before.actual.length) {
      writeFileSync(path, original);
      reverted++;
      console.log(`${basename(path, ".c")} REVERTED (linked size changed ${before.actual.length} -> ${after.actual.length}, comparison would be invalid)`);
      continue;
    }
    // Anchor to before.expected (the fixed reference slice), never
    // after.expected -- see the differingBytes() comment above.
    const postBytes = differingBytes(after.actual, before.expected);

    if (postBytes === 0) {
      const dest = join(ROOT, "exact", basename(path));
      try {
        execFileSync("git", ["mv", path, dest], { cwd: ROOT, encoding: "utf8" });
        closed++;
        console.log(`${basename(path, ".c")} CLOSED via mechanical sign-fix (${candidates.map((c) => c.name).join(",")}), was ${preBytes} differing bytes -- moved to exact/`);
      } catch (error) {
        closed++;
        console.log(`${basename(path, ".c")} CLOSED via mechanical sign-fix (${candidates.map((c) => c.name).join(",")}), was ${preBytes} differing bytes -- git mv FAILED: ${(error as Error).message.slice(0, 150)}`);
      }
    } else if (postBytes < preBytes) {
      improved++;
      console.log(`${basename(path, ".c")} IMPROVED via mechanical sign-fix (${candidates.map((c) => c.name).join(",")}): ${preBytes} -> ${postBytes} differing bytes`);
    } else {
      writeFileSync(path, original);
      reverted++;
    }
  }

  console.log(`closed=${closed} improved=${improved} reverted=${reverted} skipped=${skipped} total=${files.length}`);
}

if (import.meta.main) await main();
