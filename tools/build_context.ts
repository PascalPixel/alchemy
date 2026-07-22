#!/usr/bin/env bun
// Tool role: dead; no tracked importer or invocation; audit before use.
import { mkdirSync, readFileSync, readdirSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const TYPES = `typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;

`;
const DEFINITION = /^([A-Za-z_][A-Za-z0-9_ \t*]*?)\s+(Func_[0-9a-f]{8})\s*\(([^)]*)\)\s*\{/gm;

function usage(): void {
  console.log("usage: build_context.ts [-h] [-o OUTPUT]");
}

function parseArguments(arguments_: string[]): string {
  let output = join(ROOT, "work/context.c");
  for (let index = 0; index < arguments_.length; index++) {
    const argument = arguments_[index];
    if (argument === "-h" || argument === "--help") {
      usage();
      process.exit(0);
    }
    if (argument === "-o" || argument === "--output") {
      const value = arguments_[++index];
      if (value === undefined) throw new Error(`argument ${argument}: expected one argument`);
      output = value;
      continue;
    }
    throw new Error(`unrecognized argument: ${argument}`);
  }
  return output;
}

function main(): void {
  const output = parseArguments(Bun.argv.slice(2));
  const declarations = new Map<string, string>();
  const sources = readdirSync(join(ROOT, "src"))
    .filter((name) => name.endsWith(".c"))
    .sort();
  for (const name of sources) {
    const source = readFileSync(join(ROOT, "src", name), "utf8");
    for (const match of source.matchAll(DEFINITION)) {
      const returnType = match[1].trim().split(/\s+/).join(" ");
      const functionName = match[2];
      const parameters = match[3].trim().split(/\s+/).filter(Boolean).join(" ");
      declarations.set(functionName, `${returnType} ${functionName}(${parameters});`);
    }
  }
  mkdirSync(dirname(output), { recursive: true });
  const names = [...declarations.keys()].sort();
  writeFileSync(output, TYPES + names.map((name) => declarations.get(name)).join("\n") + "\n");
  console.log(`declarations=${declarations.size}`);
}

if (import.meta.main) main();
