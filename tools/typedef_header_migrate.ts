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
import { dirname, join, resolve } from "node:path";
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

function compileOverlayBytes(source: string, work: string, overlay: string): Buffer {
  const compiled = compileOverlayCandidate(source, work, overlay, source, []);
  return compiled.data;
}

async function migrateOverlayFile(path: string, work: string): Promise<MigrationResult> {
  const original = readFileSync(path, "utf8");
  const { text, changed } = migrateFile(original);
  if (!changed) return { path, status: "unchanged" };

  const stem = /([0-9a-f]{8})\.c$/i.exec(path)?.[1];
  const overlay = /(resource_[0-9a-f]+)_/i.exec(path)?.[1];
  if (stem === undefined || overlay === undefined) {
    return { path, status: "verify-failed", detail: "cannot determine overlay/address from filename" };
  }

  mkdirSync(work, { recursive: true });
  const beforeBytes = compileOverlayBytes(path, join(work, "before"), overlay);
  const tmpPath = join(work, `${overlay}_c_${stem}.c`);
  writeFileSync(tmpPath, text);
  let afterBytes: Buffer;
  try {
    afterBytes = compileOverlayBytes(tmpPath, join(work, "after"), overlay);
  } catch (error) {
    return { path, status: "verify-failed", detail: `after-edit compile failed: ${(error as Error).message.slice(0, 100)}` };
  }
  if (Buffer.compare(beforeBytes, afterBytes) !== 0) {
    return { path, status: "verify-failed", detail: "post-edit compile is not byte-identical" };
  }
  writeFileSync(path, text);
  return { path, status: "migrated" };
}

async function migrateMainFile(path: string, work: string, rom: Buffer): Promise<MigrationResult> {
  const original = readFileSync(path, "utf8");
  const { text, changed } = migrateFile(original);
  if (!changed) return { path, status: "unchanged" };

  mkdirSync(work, { recursive: true });
  const before = await verifyCandidate(path, rom, join(work, "before"), [], ROM_BASE, "gs1", { family: "routed", removeFlags: [] });
  const tmpPath = join(work, "candidate.c");
  writeFileSync(tmpPath, text);
  let after;
  try {
    after = await verifyCandidate(tmpPath, rom, join(work, "after"), [], ROM_BASE, "gs1", { family: "routed", removeFlags: [] });
  } catch (error) {
    return { path, status: "verify-failed", detail: `after-edit compile failed: ${(error as Error).message.slice(0, 100)}` };
  }
  if (Buffer.compare(Buffer.from(before.actual), Buffer.from(after.actual)) !== 0) {
    return { path, status: "verify-failed", detail: "post-edit compile is not byte-identical" };
  }
  writeFileSync(path, text);
  return { path, status: "migrated" };
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

async function main(): Promise<void> {
  if (Bun.argv.includes("--self-test")) return selfTest();
  const args = Bun.argv.slice(2);
  const scope = args.find((arg) => !arg.startsWith("--")) ?? "assets/code";
  const dryRun = args.includes("--dry-run");
  const work = resolve(ROOT, "work/typedef-migrate");

  const dir = resolve(ROOT, scope);
  const files = readdirSync(dir).filter((name) => name.endsWith(".c")).map((name) => join(dir, name));
  console.log(`scanning ${files.length} files in ${scope}${dryRun ? " (dry run)" : ""}`);

  const rom = scope === "semantic/main" ? readFileSync(join(ROOT, "roms/gs1-en.gba")) : Buffer.alloc(0);
  let migrated = 0;
  let unchanged = 0;
  let failed = 0;
  for (const file of files) {
    if (dryRun) {
      const { changed } = migrateFile(readFileSync(file, "utf8"));
      if (changed) migrated++; else unchanged++;
      continue;
    }
    const result = scope === "semantic/main"
      ? await migrateMainFile(file, work, rom)
      : await migrateOverlayFile(file, work);
    if (result.status === "migrated") migrated++;
    else if (result.status === "unchanged") unchanged++;
    else { failed++; console.log(`verify-failed: ${result.path}: ${result.detail}`); }
  }
  console.log(`migrated=${migrated} unchanged=${unchanged} verify-failed=${failed}`);
}

if (import.meta.main) await main();
