#!/usr/bin/env bun
// Zero-judgment mechanical sweep: several independent harvesting agents this
// session found the SAME hand-fixable defect class independently -- a loop
// counter declared s32/int, bounded 0..N against a small compile-time
// constant, where the reference compiled a bcs/bls/blo branch (unsigned) but
// our signed declaration compiles ble/blt/bgt instead (see resource_39e:cd4,
// resource_39c:15e0, and the general pattern documented across this
// session's harvesting reports). This tries the type flip mechanically
// across every remaining overlay owner and keeps only what a real
// byte-comparison proves helps -- via alchemist.ts, which already owns the
// candidate-vs-reference byte-diff logic, so this tool never reimplements
// it. Never invents a fix beyond this one narrow, evidenced transform.
import { execFileSync } from "node:child_process";
import { readFileSync, readdirSync, writeFileSync } from "node:fs";
import { basename, dirname, join } from "node:path";
import { resolveSpan } from "./alchemist.ts";

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

function overlayIdOf(path: string): string | null {
  const base = basename(path, ".c");
  const overlay = /^(resource_[0-9a-f]+)_c_([0-9a-f]{8})$/i.exec(base);
  if (overlay === null) return null;
  const address = Number.parseInt(overlay[2], 16);
  return `${overlay[1]}:${(address - 0x02000000).toString(16)}`;
}

interface AlchemistVerdict {
  verdict: "exact" | "improved" | "refused" | "exhausted" | "unnecessary";
  baseline_differing_bytes: number;
  differing_bytes: number;
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

function runAdopt(id: string, source: string): boolean {
  try {
    const [overlay, offsetText] = id.split(":");
    const offset = Number.parseInt(offsetText, 16);
    // overlay_adopt.ts's own inventory lookup can miss an entry alchemist.ts
    // already resolved a span for (they draw from overlapping but not
    // identical sources); reuse alchemist's resolveSpan, which just proved
    // itself correct by computing this owner's byte-exact diff, rather than
    // trust adopt's narrower lookup or guess.
    const span = resolveSpan(overlay, offset);
    const args = ["tools/overlay_adopt.ts", id, "--source", source, "--apply"];
    if (span !== undefined) args.push("--span", String(span));
    execFileSync("bun", args, { cwd: ROOT, encoding: "utf8", timeout: 60_000 });
    return true;
  } catch {
    return false;
  }
}

async function main(): Promise<void> {
  const limitArg = Bun.argv.find((arg) => arg.startsWith("--limit="));
  const limit = limitArg !== undefined ? Number.parseInt(limitArg.split("=")[1], 10) : Infinity;
  const semanticDir = join(ROOT, "semantic");
  const files = readdirSync(semanticDir)
    .filter((name) => /^resource_[0-9a-f]+_c_[0-9a-f]{8}\.c$/i.test(name))
    .map((name) => join(semanticDir, name))
    .slice(0, limit);

  let closed = 0;
  let improved = 0;
  let reverted = 0;
  let skipped = 0;
  const log: string[] = [];

  for (const path of files) {
    const id = overlayIdOf(path);
    if (id === null) { skipped++; continue; }
    const original = readFileSync(path, "utf8");
    const candidates = findCandidates(original);
    if (candidates.length === 0) { skipped++; continue; }

    const before = runAlchemist(id, false);
    if (before === null) { skipped++; continue; }
    const preBytes = before.baseline_differing_bytes;
    if (preBytes === 0) { skipped++; continue; }

    const transformed = applyTransform(original, candidates);
    if (transformed === original) { skipped++; continue; }
    writeFileSync(path, transformed);

    const after = runAlchemist(id, true);
    if (after === null) {
      writeFileSync(path, original);
      reverted++;
      log.push(`${id} REVERTED (alchemist error after transform)`);
      continue;
    }

    if (after.verdict === "exact" || after.verdict === "unnecessary") {
      const applied = runAdopt(id, path);
      if (applied) {
        closed++;
        const line = `${id} CLOSED via mechanical sign-fix (${candidates.map((c) => c.name).join(",")}), was ${preBytes} differing bytes`;
        log.push(line); console.log(line);
      } else {
        writeFileSync(path, original);
        reverted++;
        const line = `${id} REVERTED (adopt failed after exact verdict)`;
        log.push(line); console.log(line);
      }
      continue;
    }

    if (after.differing_bytes < preBytes) {
      improved++;
      const line = `${id} IMPROVED via mechanical sign-fix (${candidates.map((c) => c.name).join(",")}): ${preBytes} -> ${after.differing_bytes} differing bytes`;
      log.push(line); console.log(line);
    } else {
      writeFileSync(path, original);
      reverted++;
    }
  }

  console.log(`closed=${closed} improved=${improved} reverted=${reverted} skipped=${skipped} total=${files.length}`);
}

if (import.meta.main) await main();
