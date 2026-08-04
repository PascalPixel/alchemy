#!/usr/bin/env bun
// Extends the already-proven include/types.h consolidation (src/: 1,133 of
// 1,184 files already migrated) to the overlay-harvesting tree, which grew
// up as a separate C corpus and never adopted it: assets/code/,
// semantic/main/, semantic/overlays/ collectively still inline the same
// eight typedef lines (plus `bool`) in ~2,365 files.
//
// Every candidate file is recompiled and diffed byte-for-byte BEFORE and
// AFTER the edit; a file is only rewritten if the two outputs are IDENTICAL.
// A file whose typedef spelling doesn't match the known safe set, or whose
// post-edit compile isn't byte-identical, is left untouched and reported --
// this tool never trusts pattern-matching alone for byte-exact-critical
// files.
import { readdirSync, readFileSync, writeFileSync, mkdirSync } from "node:fs";
import { basename, dirname, join, resolve } from "node:path";
import { compileOverlayCandidate } from "./overlay_disasm.ts";
import { verifyCandidate, ROM_BASE } from "./match_m2c.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

// The exact known-safe typedef lines (both `typedef signed X` and the bare
// `typedef X` spelling, since both appear and are semantically identical in
// C -- `int`/`short` default to signed). Order-independent; only removes an
// EXACT line match, never a partial/custom struct typedef.
const SAFE_LINES = new Set([
  "typedef signed char s8;", "typedef unsigned char u8;",
  "typedef signed short s16;", "typedef short s16;", "typedef unsigned short u16;",
  "typedef signed int s32;", "typedef int s32;", "typedef unsigned int u32;",
  "typedef signed long long s64;", "typedef unsigned long long u64;",
  "typedef int bool;",
]);

interface MigrationResult {
  path: string;
  status: "migrated" | "unchanged" | "verify-failed";
  detail?: string;
}

// Splits a single line that packs multiple `typedef ... ;` statements
// together (10 files in the corpus do this) into individual statements, so
// each can be matched against SAFE_LINES independently.
function splitPackedTypedefs(line: string): string[] {
  const matches = line.match(/typedef\s+[^;]+;/g);
  return matches ?? [line];
}

export function migrateFile(text: string): { text: string; changed: boolean } {
  const lines = text.split("\n");
  const output: string[] = [];
  let removed = 0;
  let insertedAt = -1;
  for (const line of lines) {
    const trimmed = line.trim();
    if (trimmed.startsWith("typedef") && trimmed.includes(";")) {
      const statements = splitPackedTypedefs(trimmed);
      if (statements.length > 0 && statements.every((statement) => SAFE_LINES.has(statement))) {
        if (insertedAt === -1) insertedAt = output.length;
        removed += statements.length;
        continue;
      }
    }
    output.push(line);
  }
  if (removed === 0) return { text, changed: false };
  output.splice(insertedAt, 0, '#include "types.h"');
  return { text: output.join("\n"), changed: true };
}

async function migrateOverlayFile(path: string, workRoot: string): Promise<MigrationResult> {
  const original = readFileSync(path, "utf8");
  const { text, changed } = migrateFile(original);
  if (!changed) return { path, status: "unchanged" };

  const stem = /([0-9a-f]{8})\.c$/i.exec(path)?.[1];
  const overlay = /(resource_[0-9a-f]+)_/i.exec(path)?.[1];
  if (stem === undefined || overlay === undefined) {
    return { path, status: "verify-failed", detail: "cannot determine overlay/address from filename" };
  }

  // A shared work directory across the whole run caused every file's compile
  // artifacts (named from the entry address, e.g. "0200006c.o") to collide
  // with any OTHER file at the same low address in a different overlay --
  // this corpus reuses low addresses constantly across overlays. Isolate
  // per-file, like alchemist.ts already does for the same reason.
  const work = join(workRoot, `${overlay}_${stem}`);
  // compileOverlayC uses its `work` parameter as the spawned compiler's cwd
  // (overlay_disasm.ts `checked()`) but never creates that directory itself
  // -- it assumes the caller already did. A cache hit (the common case for
  // an unmodified "before" compile, since these files have already been
  // compiled countless times this session) returns before ever touching the
  // directory, masking a missing mkdir until the first cache MISS, where
  // Bun's posix_spawn reports the missing cwd as an ENOENT on the
  // EXECUTABLE PATH -- a real, reproducible POSIX misattribution, not a
  // filesystem race. Create both subdirectories explicitly.
  const beforeWork = join(work, "before");
  const afterWork = join(work, "after");
  mkdirSync(beforeWork, { recursive: true });
  mkdirSync(afterWork, { recursive: true });
  // Some semantic drafts don't compile cleanly as-is yet (still WIP, not a
  // closed owner) -- that's a pre-existing property of the file, unrelated
  // to this edit, so it must be caught here too, not just around the "after"
  // compile below.
  let before: { data: Buffer };
  try {
    before = compileOverlayCandidate(path, beforeWork, overlay, path, []);
  } catch (error) {
    return { path, status: "verify-failed", detail: `before-edit compile failed: ${(error as Error).message.slice(0, 100)}` };
  }

  // Compile the edit IN PLACE at the real path, never a scratch copy: per-
  // source flag routing (NO_CSE_SHIFT_IMMEDIATE_OVERLAY_SOURCES and friends
  // in alchemy_gcc.ts) matches on the file's exact project path, and a
  // scratch routingSource silently falls through to no special routing --
  // compiling genuinely different bytes for any routed file, not a harmless
  // no-op. The real file is restored unless verification succeeds.
  writeFileSync(path, text);
  try {
    const after = compileOverlayCandidate(path, afterWork, overlay, path, []);
    if (Buffer.compare(before.data, after.data) !== 0) {
      writeFileSync(path, original);
      return { path, status: "verify-failed", detail: "post-edit compile is not byte-identical" };
    }
    return { path, status: "migrated" };
  } catch (error) {
    writeFileSync(path, original);
    return { path, status: "verify-failed", detail: `after-edit compile failed: ${(error as Error).message.slice(0, 100)}` };
  }
}

async function migrateMainFile(path: string, workRoot: string, rom: Buffer): Promise<MigrationResult> {
  const original = readFileSync(path, "utf8");
  const { text, changed } = migrateFile(original);
  if (!changed) return { path, status: "unchanged" };

  const work = join(workRoot, basename(path, ".c"));
  const beforeWork = join(work, "before");
  const afterWork = join(work, "after");
  mkdirSync(beforeWork, { recursive: true });
  mkdirSync(afterWork, { recursive: true });
  let before: { actual: Uint8Array };
  try {
    before = await verifyCandidate(path, rom, beforeWork, [], ROM_BASE, "gs1", { family: "routed", removeFlags: [] });
  } catch (error) {
    return { path, status: "verify-failed", detail: `before-edit compile failed: ${(error as Error).message.slice(0, 100)}` };
  }

  // Same in-place rationale as migrateOverlayFile: verifyCandidate always
  // routes on its own `source` argument, so a scratch path would silently
  // drop any route this file carries.
  writeFileSync(path, text);
  try {
    const after = await verifyCandidate(path, rom, afterWork, [], ROM_BASE, "gs1", { family: "routed", removeFlags: [] });
    if (Buffer.compare(Buffer.from(before.actual), Buffer.from(after.actual)) !== 0) {
      writeFileSync(path, original);
      return { path, status: "verify-failed", detail: "post-edit compile is not byte-identical" };
    }
    return { path, status: "migrated" };
  } catch (error) {
    writeFileSync(path, original);
    return { path, status: "verify-failed", detail: `after-edit compile failed: ${(error as Error).message.slice(0, 100)}` };
  }
}

function selfTest(): void {
  const fixture = [
    "typedef signed int s32;",
    "typedef unsigned char u8;",
    "",
    "extern u8 *Data_03001ebc;",
    "",
    "s32 Func_02000000(void) { return 0; }",
    "",
  ].join("\n");
  const result = migrateFile(fixture);
  if (!result.changed) throw new Error("expected the fixture's typedefs to be recognized");
  if (!result.text.includes('#include "types.h"')) throw new Error(`expected an #include line, got:\n${result.text}`);
  if (result.text.includes("typedef signed int s32;")) throw new Error("typedef line was not removed");
  if (!result.text.includes("extern u8 *Data_03001ebc;")) throw new Error("unrelated content was disturbed");

  // A custom struct typedef must never be touched, even if it sits right
  // next to safe primitive typedefs.
  const withStruct = [
    "typedef signed int s32;",
    "typedef struct { s32 unk0; } Desc;",
    "",
    "void Func_02000000(Desc *d) {}",
    "",
  ].join("\n");
  const structResult = migrateFile(withStruct);
  if (!structResult.text.includes("typedef struct { s32 unk0; } Desc;")) {
    throw new Error(`custom struct typedef was wrongly removed:\n${structResult.text}`);
  }
  if (structResult.text.includes("typedef signed int s32;")) {
    throw new Error("safe typedef next to a struct typedef was not removed");
  }

  // A packed one-line multi-typedef statement must split and match.
  const packed = "typedef signed char s8; typedef unsigned char u8; typedef signed int s32;\nvoid Func_02000000(void) {}\n";
  const packedResult = migrateFile(packed);
  if (!packedResult.changed || packedResult.text.includes("typedef signed char")) {
    throw new Error(`packed typedef line was not fully recognized:\n${packedResult.text}`);
  }

  // A file with no typedefs at all is left untouched.
  const noTypedefs = "void Func_02000000(void) {}\n";
  if (migrateFile(noTypedefs).changed) throw new Error("a file with no typedefs should report unchanged");

  console.log("self-test=ok tool=typedef-header-migrate");
}

// Since the exact/semantic tree consolidation, exact/ and semantic/ each
// hold both main-image and overlay sources together, so the main-vs-overlay
// choice must be made per file by filename shape, not by directory scope.
const MAIN_IMAGE_NAME = /^[0-9a-f]{8}\.c$/i;

async function main(): Promise<void> {
  if (Bun.argv.includes("--self-test")) return selfTest();
  const args = Bun.argv.slice(2);
  const scope = args.find((arg) => !arg.startsWith("--")) ?? "exact";
  const dryRun = args.includes("--dry-run");
  const work = resolve(ROOT, "work/typedef-migrate");

  const dir = resolve(ROOT, scope);
  const files = readdirSync(dir).filter((name) => name.endsWith(".c")).map((name) => join(dir, name));
  console.log(`scanning ${files.length} files in ${scope}${dryRun ? " (dry run)" : ""}`);

  const rom = readFileSync(join(ROOT, "roms/gs1-en.gba"));
  let migrated = 0;
  let unchanged = 0;
  let failed = 0;
  for (const file of files) {
    if (dryRun) {
      const { changed } = migrateFile(readFileSync(file, "utf8"));
      if (changed) migrated++; else unchanged++;
      continue;
    }
    const result = MAIN_IMAGE_NAME.test(basename(file))
      ? await migrateMainFile(file, work, rom)
      : await migrateOverlayFile(file, work);
    if (result.status === "migrated") migrated++;
    else if (result.status === "unchanged") unchanged++;
    else { failed++; console.log(`verify-failed: ${result.path}: ${result.detail}`); }
  }
  console.log(`migrated=${migrated} unchanged=${unchanged} verify-failed=${failed}`);
}

if (import.meta.main) await main();
