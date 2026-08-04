#!/usr/bin/env bun
// One-time migration: collapse the region-split (main vs overlay) exact-C and
// semantic-C trees into a single status-split pair, since the region is
// already unambiguous from filename convention (resource_ prefix = overlay,
// plain hex = main-image):
//   src/*.c + assets/code/*.c            -> exact/*.c
//   semantic/main/*.c + semantic/overlays/*.c -> semantic/*.c (flattened)
// assets/code/*.s and assets/code/*.json (overlay containers, LZ stream
// descriptors -- not C source) are NOT part of this migration and stay put.
//
// Two passes:
//   1. Rewrite every tool's literal .c path references (string and template
//      literals) from the old directory prefix to the new one.
//   2. Physically git-mv the files.
// Run with --dry-run first to see the rewrite diff without touching anything.
import { execSync } from "node:child_process";
import { readFileSync, writeFileSync, readdirSync, existsSync } from "node:fs";
import { dirname, join } from "node:path";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

// Order matters: longer/more specific prefixes first so a shorter prefix
// doesn't partially match inside a longer one.
const REWRITES: Array<[RegExp, string]> = [
  [/"semantic\/overlays\/([^"]*\.c)"/g, '"semantic/$1"'],
  [/"semantic\/main\/([^"]*\.c)"/g, '"semantic/$1"'],
  [/`semantic\/overlays\/([^`]*)`/g, "`semantic/$1`"],
  [/`semantic\/main\/([^`]*)`/g, "`semantic/$1`"],
  [/"assets\/code\/([^"]*\.c)"/g, '"exact/$1"'],
  // Template literals whose STATIC prefix is "assets/code/" and whose
  // content ends in ".c" -- matches regardless of what's interpolated inside,
  // since the discriminator is the LITERAL suffix, not the dynamic part.
  [/`assets\/code\/([^`]*\.c)`/g, "`exact/$1`"],
  // The general `join(ROOT, "assets/code", <arg ending in .c>)` shape: the
  // directory and filename are SEPARATE join() arguments here (unlike the
  // template-literal form above), so the .c-suffix check has to look inside
  // the third argument specifically -- this directory is used for BOTH .c
  // source (moving to exact/) and .s/.json overlay containers (staying
  // put), and only the argument's own suffix distinguishes them.
  [/join\(ROOT, "assets\/code", (`[^`]*\.c`|"[^"]*\.c")\)/g, 'join(ROOT, "exact", $1)'],
  [/join\(ROOT, "semantic", "overlays", (`[^`]*\.c`|"[^"]*\.c")\)/g, 'join(ROOT, "semantic", $1)'],
  [/join\(ROOT, "semantic", "main", (`[^`]*\.c`|"[^"]*\.c")\)/g, 'join(ROOT, "semantic", $1)'],
  [/join\(ROOT, "semantic\/overlays", (`[^`]*\.c`|"[^"]*\.c")\)/g, 'join(ROOT, "semantic", $1)'],
  [/join\(ROOT, "semantic\/main", (`[^`]*\.c`|"[^"]*\.c")\)/g, 'join(ROOT, "semantic", $1)'],
];

// Bare directory references (no filename) -- every one of these was read in
// context by hand (see the commit message) to confirm it is used ONLY for
// enumerating/joining .c source, never .s/.json, before being listed here.
const BARE_DIRECTORY_REWRITES: Array<[RegExp, string]> = [
  [/join\(root, "assets\/code"\)/g, 'join(root, "exact")'],
  [/join\(ROOT, "src"\)/g, 'join(ROOT, "exact")'],
  [/"src"/g, '"exact"'],
];

function rewriteFile(text: string): { text: string; changed: boolean } {
  let result = text;
  let changed = false;
  for (const [pattern, replacement] of [...REWRITES, ...BARE_DIRECTORY_REWRITES]) {
    const next = result.replace(pattern, replacement);
    if (next !== result) changed = true;
    result = next;
  }
  return { text: result, changed };
}

function selfTest(): void {
  const cases: Array<[string, string]> = [
    ['"assets/code/resource_378_c_02002750.c"', '"exact/resource_378_c_02002750.c"'],
    ['"semantic/overlays/resource_378_c_02002750.c"', '"semantic/resource_378_c_02002750.c"'],
    ['"semantic/main/08002ee4.c"', '"semantic/08002ee4.c"'],
    ['`assets/code/resource_3bf_c_${stem}.c`', "`exact/resource_3bf_c_${stem}.c`"],
    ['join(ROOT, "src", `${stems[0]}.c`)', 'join(ROOT, "exact", `${stems[0]}.c`)'],
  ];
  for (const [input, expected] of cases) {
    const { text, changed } = rewriteFile(input);
    if (!changed || text !== expected) {
      throw new Error(`expected ${JSON.stringify(input)} -> ${JSON.stringify(expected)}, got ${JSON.stringify(text)} (changed=${changed})`);
    }
  }
  // Non-.c assets/code references (a .s overlay container, a .json stream
  // descriptor) must be left completely untouched.
  const untouched = [
    'join(ROOT, `assets/code`, `${overlay}_overlay.s`)',
    '`assets/code/resource_${name}`',
    'readdirSync(join(root, "assets/code_other_unrelated_dir"))',
  ];
  for (const input of untouched) {
    const { text, changed } = rewriteFile(input);
    if (changed || text !== input) throw new Error(`non-.c reference was wrongly touched: ${JSON.stringify(input)} -> ${JSON.stringify(text)}`);
  }
  console.log("self-test=ok tool=exact-semantic-tree-migrate");
}

function rewriteTools(dryRun: boolean): void {
  const toolsDir = join(ROOT, "tools");
  // Self-exclude: this file's own self-test fixtures are illustrative path
  // examples, not real references, and would otherwise get mangled.
  const files = readdirSync(toolsDir).filter((name) => name.endsWith(".ts") && name !== "exact_semantic_tree_migrate.ts");
  let changedCount = 0;
  for (const name of files) {
    const path = join(toolsDir, name);
    const original = readFileSync(path, "utf8");
    const { text, changed } = rewriteFile(original);
    if (!changed) continue;
    changedCount++;
    if (dryRun) {
      console.log(`would rewrite: ${name}`);
    } else {
      writeFileSync(path, text);
      console.log(`rewrote: ${name}`);
    }
  }
  console.log(`${dryRun ? "would change" : "changed"} ${changedCount} tool files`);
}

function moveFiles(dryRun: boolean): void {
  const moves: Array<[string, string]> = [];
  for (const dir of ["src", "assets/code"]) {
    const full = join(ROOT, dir);
    if (!existsSync(full)) continue;
    for (const name of readdirSync(full)) {
      if (!name.endsWith(".c")) continue;
      moves.push([join(dir, name), join("exact", name)]);
    }
  }
  for (const dir of ["semantic/main", "semantic/overlays"]) {
    const full = join(ROOT, dir);
    if (!existsSync(full)) continue;
    for (const name of readdirSync(full)) {
      if (!name.endsWith(".c")) continue;
      moves.push([join(dir, name), join("semantic", name)]);
    }
  }
  console.log(`${moves.length} files to move`);
  if (dryRun) {
    console.log(moves.slice(0, 5).map(([a, b]) => `  ${a} -> ${b}`).join("\n"));
    console.log(`  ... and ${moves.length - 5} more`);
    return;
  }
  execSync(`mkdir -p "${join(ROOT, "exact")}"`, { cwd: ROOT });
  // git mv in large batches (arg-length safe) rather than one call per file.
  const BATCH = 200;
  for (let index = 0; index < moves.length; index += BATCH) {
    const batch = moves.slice(index, index + BATCH);
    const args = batch.flatMap(([from, to]) => [from, to]);
    // git mv only supports ONE destination per invocation for multi-source
    // form (a directory), so move file-by-file within the batch instead.
    for (const [from, to] of batch) {
      execSync(`git mv "${from}" "${to}"`, { cwd: ROOT });
    }
    console.log(`moved ${Math.min(index + BATCH, moves.length)}/${moves.length}`);
  }
}

async function main(): Promise<void> {
  if (Bun.argv.includes("--self-test")) return selfTest();
  const dryRun = Bun.argv.includes("--dry-run");
  const step = Bun.argv.includes("--rewrite-only") ? "rewrite" : Bun.argv.includes("--move-only") ? "move" : "both";
  if (step === "rewrite" || step === "both") rewriteTools(dryRun);
  if (step === "move" || step === "both") moveFiles(dryRun);
}

if (import.meta.main) await main();
