#!/usr/bin/env bun
// Explore source shapes for one C candidate, holding the compiler fixed.
//
// `mode_sweep.ts` searches compiler configurations for a candidate whose source
// is treated as given.  This searches the other axis: the source forms that
// compile to the same semantics, with the routed compiler unchanged.  The two
// are complementary, and a residual that neither moves is a genuine floor
// rather than an unexplored direction.
//
// Transforms are seeded from LAWS.md rather than generated at random.  Each one
// is a producing idiom some confirmed law already names, so a hit both closes
// the owner and re-witnesses the law.
//
// This does not replace `permute_v1.ts` / `permute_overlay.ts`, which anneal
// over a statement model and search far wider.  It is the bounded pass those
// cannot be: annealing is stochastic and unbounded, so a run that finds nothing
// proves nothing, and SANCTUM.md's shape axis needs a search whose exhaustion is
// reproducible.  Run this first and cheaply; escalate to the annealer under the
// bounded-probe rule in HANDOVER.md when it misses.
//
//   bun tools/search/shape_sweep.ts semantic/resource_3ba_c_02002738.c
//   bun tools/search/shape_sweep.ts exact/08011568.c --pairs
//   bun tools/search/shape_sweep.ts --self-test
import { mkdtempSync, readFileSync, rmSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { basename, dirname, join } from "node:path";
import { ROM_BASE, verifyCandidate } from "../lib/match_m2c.ts";
import { assembleOverlay, compileOverlayCandidate } from "../lib/overlay_disasm.ts";

const ROOT = dirname(dirname(dirname(Bun.fileURLToPath(import.meta.url))));
const USAGE = "usage: shape_sweep.ts <candidate.c> [--pairs] [--top N] [--self-test]";

// A transform rewrites C source into an equivalent shape, or returns null when
// its pattern is absent.  Returning null is the normal case: the sweep asks
// every transform about every candidate and only compiles the ones that bite.
export type Transform = {
  id: string;
  law: string;
  apply: (source: string) => string | null;
};

// Body of the first brace-balanced block starting at `open`, exclusive of the
// braces themselves.  Regex cannot balance braces and every structural
// transform below needs a real body, so this is the shared primitive.
function blockAt(source: string, open: number): { body: string; end: number } | null {
  if (source[open] !== "{") return null;
  let depth = 0;
  for (let at = open; at < source.length; at++) {
    const char = source[at];
    if (char === "{") depth++;
    else if (char === "}") {
      depth--;
      if (depth === 0) return { body: source.slice(open + 1, at), end: at };
    }
  }
  return null;
}

function replaceOnce(source: string, pattern: RegExp, replacer: (match: RegExpExecArray) => string): string | null {
  const match = pattern.exec(source);
  if (!match) return null;
  return source.slice(0, match.index) + replacer(match) + source.slice(match.index + match[0].length);
}

// Call arguments split on top-level commas, so nested calls and casts survive.
function splitArguments(text: string): string[] | null {
  const parts: string[] = [];
  let depth = 0;
  let start = 0;
  for (let at = 0; at < text.length; at++) {
    const char = text[at];
    if (char === "(") depth++;
    else if (char === ")") depth--;
    else if (char === "," && depth === 0) {
      parts.push(text.slice(start, at).trim());
      start = at + 1;
    }
  }
  parts.push(text.slice(start).trim());
  return parts.every((part) => part.length > 0) ? parts : null;
}

const CALL = /(\b[A-Za-z_][A-Za-z0-9_]*)\s*\(([^();]*)\)\s*;/;

export const TRANSFORMS: readonly Transform[] = [
  {
    // Witnessed on 080cd358: 79 differing halfwords to 2.  Where the reference
    // materializes arguments other than left to right, binding every argument
    // after the first to a block-scoped local makes the first materialize last.
    id: "hoist-args-after-first",
    law: "argument materialisation order",
    apply: (source) =>
      replaceOnce(source, CALL, (match) => {
        const args = splitArguments(match[2] ?? "");
        if (!args || args.length < 2) return match[0];
        const bound = args.slice(1).map((arg, index) => `s32 shape_arg${index} = ${arg};`);
        const call = `${match[1]}(${[args[0], ...args.slice(1).map((_, index) => `shape_arg${index}`)].join(", ")});`;
        return `{ ${bound.join(" ")} ${call} }`;
      }),
  },
  {
    // Minimal live-variable form: a named temporary used exactly once extends a
    // live range the reference does not, and changes register allocation.
    id: "inline-single-use-temp",
    law: "minimal live-variable form",
    apply: (source) => {
      const declaration = /\n\s*(?:s32|u32|s16|u16|s8|u8|int)\s+([A-Za-z_][A-Za-z0-9_]*)\s*=\s*([^;]+);/;
      const match = declaration.exec(source);
      if (!match) return null;
      const [, name, value] = match;
      if (!name || !value) return null;
      const uses = source.split(new RegExp(`\\b${name}\\b`)).length - 1;
      if (uses !== 2) return null;
      const without = source.slice(0, match.index) + source.slice(match.index + match[0].length);
      return without.replace(new RegExp(`\\b${name}\\b`), `(${value.trim()})`);
    },
  },
  {
    // The inverse direction: some references keep a value in a register across a
    // call, which a repeated subexpression cannot express.
    id: "name-repeated-subexpression",
    law: "minimal live-variable form",
    apply: (source) => {
      const indexed = /\b([A-Za-z_][A-Za-z0-9_]*)\s*\[\s*([A-Za-z_][A-Za-z0-9_]*)\s*\]/g;
      const seen = new Map<string, number>();
      for (const match of source.matchAll(indexed)) seen.set(match[0], (seen.get(match[0]) ?? 0) + 1);
      const repeated = [...seen].find(([, count]) => count >= 2);
      if (!repeated) return null;
      const body = /\)\s*\{/.exec(source);
      if (!body) return null;
      const at = body.index + body[0].length;
      const bound = source.slice(at).replaceAll(repeated[0], "shape_common");
      return `${source.slice(0, at)}\n  s32 shape_common = ${repeated[0]};${bound}`;
    },
  },
  {
    // A signed comparison the reference performs unsigned costs an extra
    // sign-extension the compiler cannot elide.
    id: "unsign-comparison",
    law: "unsigned comparison shape",
    apply: (source) =>
      replaceOnce(source, /\b([A-Za-z_][A-Za-z0-9_]*)\s*(<|<=|>|>=)\s*(\d+)\b/, (match) =>
        `(u32)${match[1]} ${match[2]} ${match[3]}u`),
  },
  {
    // Index and pointer addressing pick different base registers and different
    // increment placement; the reference commits to one.
    id: "index-to-pointer",
    law: "addressing form",
    apply: (source) =>
      replaceOnce(source, /\b([A-Za-z_][A-Za-z0-9_]*)\s*\[\s*([A-Za-z_][A-Za-z0-9_]*)\s*\]/, (match) =>
        `*(${match[1]} + ${match[2]})`),
  },
  {
    // Declaration placement moves the stack slot and therefore the frame layout.
    id: "sink-declaration-to-block",
    law: "declaration scope",
    apply: (source) => {
      const opening = /\)\s*\{/.exec(source);
      if (!opening) return null;
      const at = opening.index + opening[0].length;
      const declaration = /\n\s*((?:s32|u32|s16|u16)\s+[A-Za-z_][A-Za-z0-9_]*\s*;)/.exec(source.slice(at));
      if (!declaration) return null;
      const inner = source.indexOf("{", at + declaration.index);
      if (inner < 0) return null;
      const stripped = source.slice(0, at + declaration.index) +
        source.slice(at + declaration.index + declaration[0].length);
      const insert = stripped.indexOf("{", at);
      if (insert < 0) return null;
      return `${stripped.slice(0, insert + 1)}\n    ${declaration[1]}${stripped.slice(insert + 1)}`;
    },
  },
  {
    // A trailing conditional written as an early return produces the branch
    // polarity the reference emits, where an if/else inverts it.
    id: "invert-to-early-return",
    law: "branch polarity",
    apply: (source) => {
      const guard = /if\s*\(([^)]+)\)\s*\{/.exec(source);
      if (!guard) return null;
      const block = blockAt(source, guard.index + guard[0].length - 1);
      if (!block) return null;
      const after = source.slice(block.end + 1).trim();
      if (!after.startsWith("}")) return null;
      return `${source.slice(0, guard.index)}if (!(${guard[1]})) return;\n${block.body}${source.slice(block.end + 1)}`;
    },
  },
];

function differingHalfwords(actual: Buffer, expected: Buffer): number {
  let differing = Math.abs(actual.length - expected.length) / 2;
  for (let at = 0; at + 1 < Math.min(actual.length, expected.length); at += 2) {
    if (actual.readUInt16LE(at) !== expected.readUInt16LE(at)) differing++;
  }
  return Math.round(differing);
}

export type Attempt = { ids: string[]; source: string };

// Singles first, then compatible pairs: a pair is only worth compiling when
// both members changed the source independently, so an unproductive transform
// cannot multiply the search space.
export function plan(source: string, pairs: boolean): Attempt[] {
  const singles: Attempt[] = [];
  for (const transform of TRANSFORMS) {
    const mutated = transform.apply(source);
    if (mutated && mutated !== source) singles.push({ ids: [transform.id], source: mutated });
  }
  if (!pairs) return singles;
  const combined: Attempt[] = [];
  for (const first of singles) {
    for (const transform of TRANSFORMS) {
      if (first.ids.includes(transform.id)) continue;
      const mutated = transform.apply(first.source);
      if (mutated && mutated !== first.source) {
        combined.push({ ids: [...first.ids, transform.id], source: mutated });
      }
    }
  }
  return [...singles, ...combined];
}

function selfTest(): void {
  const call = "void f(void) {\n  g(a, b, c);\n}\n";
  const hoisted = TRANSFORMS.find((t) => t.id === "hoist-args-after-first")?.apply(call);
  if (!hoisted?.includes("shape_arg0") || !hoisted.includes("g(a, shape_arg0, shape_arg1)")) {
    throw new Error("hoist-args-after-first did not bind arguments after the first");
  }
  const single = "void f(void) {\n  s32 t = load();\n  use(t);\n}\n";
  const inlined = TRANSFORMS.find((t) => t.id === "inline-single-use-temp")?.apply(single);
  if (!inlined || inlined.includes("s32 t =") || !inlined.includes("use((load()))")) {
    throw new Error("inline-single-use-temp did not remove the single-use temporary");
  }
  const twice = "void f(void) {\n  h(p[i]);\n  h(p[i]);\n}\n";
  const named = TRANSFORMS.find((t) => t.id === "name-repeated-subexpression")?.apply(twice);
  if (!named?.includes("shape_common = p[i]") || named.includes("h(p[i])")) {
    throw new Error("name-repeated-subexpression did not bind the repeat");
  }
  if (TRANSFORMS.find((t) => t.id === "inline-single-use-temp")?.apply(twice) !== null) {
    throw new Error("inline-single-use-temp fired on a multi-use temporary");
  }
  // A transform that does not match must be silent, or the sweep compiles noise.
  const bare = "void f(void) {\n  return;\n}\n";
  const fired = TRANSFORMS.filter((transform) => transform.apply(bare) !== null).map((t) => t.id);
  if (fired.length > 0) throw new Error(`transforms fired on an empty body: ${fired.join(",")}`);
  if (plan(call, false).length < 1) throw new Error("plan produced no singles for a call body");
  if (plan(call, true).length < plan(call, false).length) throw new Error("pairs shrank the plan");
  if (differingHalfwords(Buffer.from([1, 0, 2, 0]), Buffer.from([1, 0, 3, 0])) !== 1) {
    throw new Error("differingHalfwords miscounted");
  }
  console.log(`shape_sweep self-test ok (${TRANSFORMS.length} transforms)`);
}

async function main(): Promise<void> {
  const argv = Bun.argv.slice(2);
  if (argv.includes("--self-test")) return selfTest();
  const candidate = argv.find((argument) => !argument.startsWith("--"));
  if (!candidate) {
    console.error(USAGE);
    process.exit(2);
  }
  const top = Number(argv[argv.indexOf("--top") + 1]) || 12;
  const stem = basename(candidate, ".c");
  const overlayMatch = /^(resource_[0-9a-f]+)_c_[0-9a-f]{8}$/.exec(stem);
  const overlay = overlayMatch === null ? null : overlayMatch[1]!;
  const source = readFileSync(join(ROOT, candidate), "utf8");
  const attempts = plan(source, argv.includes("--pairs"));
  if (attempts.length === 0) {
    console.log("no transform matched this source; the shape axis is exhausted for it");
    return;
  }
  const rom = new Uint8Array(readFileSync(join(ROOT, "roms", "gs1-en.gba")));
  const work = mkdtempSync(join(tmpdir(), "shape-sweep-"));
  const rows: { ids: string[]; differing: number; size: number }[] = [];
  try {
    for (const attempt of attempts) {
      const path = join(work, basename(candidate));
      writeFileSync(path, attempt.source);
      try {
        // Overlay owners cannot go through verifyCandidate: it derives the
        // function address by parsing the stem as hex, and an overlay stem is
        // `resource_39c_c_0200xxxx`. Before this split the tool reported
        // `compiled=0` for every overlay -- indistinguishable from "no transform
        // helped", for 60% of the executable.
        if (overlay !== null) {
          const built = compileOverlayCandidate(path, work, overlay, candidate);
          const reference = assembleOverlay(join(ROOT, "assets", "code", `${overlay}_overlay.s`));
          const at = built.address - 0x02000000;
          const expected = reference.subarray(at, at + built.data.length);
          rows.push({
            ids: attempt.ids,
            differing: differingHalfwords(built.data, Buffer.from(expected)),
            size: built.data.length,
          });
        } else {
          const verification = await verifyCandidate(path, rom, work, [], ROM_BASE);
          rows.push({
            ids: attempt.ids,
            differing: differingHalfwords(verification.actual, verification.expected),
            size: verification.size,
          });
        }
      } catch {
        // A transform that produces uncompilable C is a miss, not a failure:
        // these rewrites are textual and the sweep is expected to overreach.
      }
    }
  } finally {
    rmSync(work, { recursive: true, force: true });
  }
  rows.sort((a, b) => a.differing - b.differing);
  console.log(`candidate=${candidate} attempted=${attempts.length} compiled=${rows.length}`);
  for (const row of rows.slice(0, top)) {
    console.log(`${String(row.differing).padStart(4)}hw size=${String(row.size).padStart(4)} ${row.ids.join(" + ")}`);
  }
  if (rows[0]?.differing === 0) console.log("EXACT: install this shape and re-run the owner's gate");
}

if (import.meta.main) await main();
