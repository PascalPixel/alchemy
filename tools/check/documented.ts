#!/usr/bin/env bun
// Every tool must be documented in AGENTS.md, or it must not exist.
//
// This repository reached 184 tools because adding one cost nothing and nobody
// could see the total. The rule that stops it recurring is not a budget, it is
// this gate: a tool with no section in AGENTS.md fails the build, and the only
// two ways to pass are to write the section or delete the tool.
//
// A "tool" is an entry point: tools/<name>/index.ts, or a top-level tools/*.ts.
// Files under tools/lib/ and the modules a tool dispatches to are implementation
// and are deliberately not listed -- pokeemerald documents 11 binaries, not the
// 82 source files behind them.
//
//   bun tools/check/documented.ts
//   bun tools/check/documented.ts --self-test
import { existsSync, readFileSync, readdirSync } from "node:fs";
import { dirname, join } from "node:path";

const ROOT = dirname(dirname(dirname(Bun.fileURLToPath(import.meta.url))));

export function entryPoints(names: readonly string[], isDirectory: (name: string) => boolean): string[] {
  const tools: string[] = [];
  for (const name of names) {
    if (isDirectory(name)) {
      if (name !== "lib") tools.push(name);
    } else if (name.endsWith(".ts")) {
      tools.push(name.slice(0, -3));
    }
  }
  return tools.sort();
}

// A heading names the tool, so `## overlay` and `## overlay — ...` both count.
// Requiring a paragraph as well is the point: a bare heading is a listing, not
// documentation, and the rule is that a reader learns what the tool is for.
export function documented(markdown: string): Map<string, number> {
  const found = new Map<string, number>();
  const lines = markdown.split("\n");
  for (let at = 0; at < lines.length; at++) {
    const heading = /^#{2,3}\s+`?([a-z0-9_]+)`?\b/.exec(lines[at] ?? "");
    if (!heading) continue;
    let words = 0;
    for (let scan = at + 1; scan < lines.length; scan++) {
      const line = lines[scan] ?? "";
      if (/^#{2,3}\s/.test(line)) break;
      words += line.trim().split(/\s+/).filter((word) => word.length > 0).length;
    }
    found.set(heading[1]!, words);
  }
  return found;
}

const MINIMUM_WORDS = 20;

export function violations(tools: readonly string[], docs: Map<string, number>): string[] {
  const problems: string[] = [];
  for (const tool of tools) {
    const words = docs.get(tool);
    if (words === undefined) {
      problems.push(`${tool}: no section in AGENTS.md -- document it or delete the tool`);
    } else if (words < MINIMUM_WORDS) {
      problems.push(`${tool}: only ${words} words; a heading is a listing, not documentation`);
    }
  }
  for (const [name] of docs) {
    if (name.length > 2 && !tools.includes(name) && existsSync(join(ROOT, "tools", `${name}.ts`))) {
      problems.push(`${name}: documented but is not an entry point`);
    }
  }
  return problems;
}

function selfTest(): void {
  const names = ["overlay", "lib", "verify.ts", "notes.md"];
  const tools = entryPoints(names, (name) => !name.includes("."));
  if (tools.join(",") !== "overlay,verify") throw new Error(`entryPoints gave ${tools.join(",")}`);
  const docs = documented("## overlay\n\n" + "word ".repeat(30) + "\n\n## verify\n\ntoo short\n");
  if ((docs.get("overlay") ?? 0) < MINIMUM_WORDS) throw new Error("a real paragraph must count");
  const problems = violations(["overlay", "verify", "ghost"], docs);
  if (!problems.some((p) => p.startsWith("ghost:"))) throw new Error("an undocumented tool must fail");
  if (!problems.some((p) => p.startsWith("verify:"))) throw new Error("a stub paragraph must fail");
  if (problems.some((p) => p.startsWith("overlay:"))) throw new Error("a documented tool must pass");
  if (documented("## overlay — the code overlays\n\n" + "word ".repeat(30)).get("overlay") === undefined) {
    throw new Error("a heading with a dash suffix must still match");
  }
  console.log("documented self-test ok");
}

function main(): void {
  if (Bun.argv.includes("--self-test")) return selfTest();
  const tools = entryPoints(
    readdirSync(join(ROOT, "tools")),
    (name) => !name.includes("."),
  );
  const problems = violations(tools, documented(readFileSync(join(ROOT, "AGENTS.md"), "utf8")));
  if (problems.length > 0) {
    for (const problem of problems) console.error(`error: ${problem}`);
    console.error(`\n${problems.length} tool(s) undocumented. AGENTS.md is the list; there is no other list.`);
    process.exit(1);
  }
  console.log(`documented ok: ${tools.length} tools, all in AGENTS.md`);
}

if (import.meta.main) main();
