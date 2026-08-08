#!/usr/bin/env bun
// Differential-parity driver for tools-rs/verify.
//
// This file is NOT part of the TypeScript toolchain. It imports the live
// `verify()` export from tools/lib/verify.ts unmodified and drives it over an
// explicit source list, so the Rust port can be compared case by case.
//
// It exists because tools/lib/verify.ts's own main() cannot run at all: its
// ROOT is dirname(dirname(...)) of tools/lib/verify.ts, which is <repo>/tools,
// a stale two-level climb left behind when the file moved into tools/lib/.
// The exported verify() takes every path explicitly, so it is unaffected.
//
// usage: bun ts_verify.ts <repoRoot> <romPath> <outDir> <listFile>
import { createHash } from "node:crypto";
import { readFileSync } from "node:fs";
import { basename } from "node:path";
import { verify } from "../../../tools/lib/verify.ts";

const [, , _root, romPath, outDir, listFile] = Bun.argv;
const rom = new Uint8Array(readFileSync(romPath));
const sources = readFileSync(listFile, "utf8").split("\n").filter(Boolean);

function digest(bytes: Uint8Array): string {
  return createHash("sha256").update(bytes).digest("hex");
}

function category(message: string): string {
  if (message.includes("unsupported external symbol")) return "unsupported-external-symbol";
  if (message.includes("missing linked symbol")) return "missing-linked-symbol";
  const failed = message.match(/^([A-Za-z0-9_.-]+) failed/);
  if (failed) return `tool-failed:${failed[1]}`;
  // Bun words a missing subprocess as `Executable not found in $PATH: "x"`,
  // with no `error: ` prefix; the Rust side words the same failure as
  // `x failed to start: ...`. Same failure, different prose, so both map here.
  const absent = message.match(/Executable not found in \$PATH: "([^"]+)"/);
  if (absent) return `tool-failed:${absent[1]}`;
  if (message.includes("ENOENT")) return "enoent";
  return "other";
}

for (const source of sources) {
  const name = basename(source);
  try {
    const [actual, expected, size] = verify(source, rom, outDir, true, "gs1");
    console.log(
      JSON.stringify({
        source: name,
        status: "ok",
        size,
        matched: Buffer.from(actual).equals(Buffer.from(expected)),
        actual: digest(actual),
        expected: digest(expected),
        actualLength: actual.length,
        expectedLength: expected.length,
      }),
    );
  } catch (error) {
    const message = error instanceof Error ? error.message : String(error);
    console.log(
      JSON.stringify({ source: name, status: "error", category: category(message), message }),
    );
  }
}
