#!/usr/bin/env bun
// Forbid hand-maintained version literals in cache keys.
//
// WHY THIS EXISTS. Three worktrees were poisoned by one `out/cache/overlay-c`
// entry. The key carried `overlay-c-v3`, a string bumped by hand whenever the
// post-compile rewriting changed. It was bumped correctly and the comment
// explaining it was honest; the defect was the MECHANISM, which holds only
// while every future editor remembers. When it was not bumped, a stale entry
// served bytes that no longer reconstruct resource_39c's LZ plan **under a key
// the tool accepts**, `verify` died in `build_assets`, and three lanes
// independently concluded that `main` was red. It was not: `git checkout` does
// not touch `out/`, so every one of those runs shared the same poisoned entry
// and none of them was a test of its commit.
//
// THE RULE. A cache key must be a function of its inputs — the source bytes it
// derives from, and a digest of the TOOL'S OWN SOURCE for the logic applied to
// them. Never a literal describing the logic. `-vN` is a promise about memory;
// a source digest is a measurement.
//
// This lint is the guard that makes the class impossible to reintroduce rather
// than merely possible to notice, which is the whole point: the three existing
// caches are fixed, and nothing stopped a fourth being written the old way.
//
//   bun tools/cache_key_lint.ts
//   bun tools/cache_key_lint.ts --self-test
import { readFileSync, readdirSync } from "node:fs";
import { dirname, join } from "node:path";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

/**
 * A hand-maintained version literal inside a template string: `foo-v3:`, or a
 * bare `-v12` before a separator. Deliberately narrow — it matches the shape
 * that actually caused the outage and not, say, a filename or a URL.
 */
const VERSION_LITERAL = /["'`][a-z0-9-]*-v\d+\s*[:\0]/i;

/** Lines that construct a digest. A version literal only matters in a key. */
const KEY_CONTEXT = /\.update\(|CryptoHasher|cacheKey|digest\b/i;

export interface Finding {
  file: string;
  line: number;
  text: string;
}

/** Scan one file's text. Pure, so the self-test needs no tree access. */
export function findViolations(file: string, text: string): Finding[] {
  const found: Finding[] = [];
  for (const [index, raw] of text.split("\n").entries()) {
    const line = raw.trim();
    // A comment may name the old spelling while explaining why it is gone.
    if (line.startsWith("//") || line.startsWith("*") || line.startsWith("/*")) continue;
    if (!KEY_CONTEXT.test(line)) continue;
    if (!VERSION_LITERAL.test(line)) continue;
    found.push({ file, line: index + 1, text: line });
  }
  return found;
}

function selfTest(): void {
  // The fixtures are ASSEMBLED rather than written out, so this file does not
  // itself contain the pattern. The alternative was exempting this file from
  // the scan, and an exception in a rule is the same author's-memory defect the
  // rule exists to remove.
  const offending = "digest.update(`overlay-c-" + "v3:${hex(address)}`);";
  if (findViolations("t.ts", offending).length !== 1) throw new Error("a versioned key literal must be caught");
  const fixed = 'digest.update(`overlay-c:${selfDigest()}:${hex(address)}\\0`);';
  if (findViolations("t.ts", fixed).length !== 0) throw new Error("a source-keyed digest must pass");
  // A comment explaining the removed spelling must not trip the rule, or the
  // history could never be written down next to the fix.
  if (findViolations("t.ts", "// it was `overlay-c-v3:` before, bumped by hand").length !== 0) {
    throw new Error("a comment naming the old spelling must not be flagged");
  }
  // A version literal outside a key context is not this rule's business.
  if (findViolations("t.ts", 'const url = "https://example.invalid/api-v2:x";').length !== 0) {
    throw new Error("a non-key version literal must not be flagged");
  }
  // Both directions, so the rule cannot rot into always-passing.
  if (findViolations("t.ts", 'key.update("asm-' + 'v1:" + address);').length !== 1) {
    throw new Error("a double-quoted versioned key must be caught");
  }
  console.log("self-test=ok (cache-key lint, both directions)");
}

function main(): void {
  if (Bun.argv.includes("--self-test")) return selfTest();
  const directory = join(ROOT, "tools");
  const names = readdirSync(directory).filter((name) => name.endsWith(".ts")).sort();
  const findings: Finding[] = [];
  for (const name of names) {
    findings.push(...findViolations(name, readFileSync(join(directory, name), "utf8")));
  }
  for (const finding of findings) {
    console.log(`${finding.file}:${finding.line}  ${finding.text}`);
  }
  console.log(`\nscanned=${names.length} violations=${findings.length}`);
  // SCANNING NOTHING IS NOT PASSING — the §5f question asked of this tool
  // itself. `violations=0` over an empty file list reads exactly like a clean
  // run, and a lint that can pass without looking is the defect it polices.
  if (names.length === 0) {
    console.log(
      "NOTHING SCANNED — this is a FAILURE, not a pass.\n" +
        `  No .ts files under ${directory}.`,
    );
    process.exitCode = 1;
    return;
  }
  if (findings.length > 0) {
    console.log(
      "HAND-MAINTAINED CACHE KEY — this is a FAILURE.\n" +
        "  A cache key must derive from its inputs, not from a literal describing\n" +
        "  the logic. Mix in a digest of the tool's own source instead:\n" +
        "      digest.update(`name:${selfDigest()}:...`);\n" +
        "  See tools/overlay_disasm.ts for the pattern, and HANDOVER §5h for why.",
    );
    process.exitCode = 1;
  }
}

if (import.meta.main) main();
