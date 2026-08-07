#!/usr/bin/env bun
// Enforce the shape of tools/, so it cannot drift back to 184 flat files.
//
// Every rule here was an ad-hoc script first. That is the tell: a check worth
// running twice is a tool, and a check that only ever ran by hand is how the
// tree reached 184 tools with 52 of them unreferenced and nobody able to see it.
//
// Three invariants:
//   1. Every tool is documented in AGENTS.md   (delegated to documented.ts)
//   2. A tool never imports from another tool  (shared code belongs in lib/)
//   3. Every module is reachable from a tool, a script, a hook, or a document
//
//   bun tools/check/architecture.ts
//   bun tools/check/architecture.ts --self-test
import { existsSync, readFileSync, readdirSync } from "node:fs";
import { dirname, join } from "node:path";

const ROOT = dirname(dirname(dirname(Bun.fileURLToPath(import.meta.url))));
const TOOLS = join(ROOT, "tools");
const LIBRARY = "lib";

export type Module = { tool: string; name: string; source: string };

export function modules(read: (path: string) => string, list: (path: string) => string[]): Module[] {
  const found: Module[] = [];
  for (const entry of list("")) {
    if (entry.endsWith(".ts")) {
      found.push({ tool: "", name: entry.slice(0, -3), source: read(entry) });
      continue;
    }
    // Skip anything that is not a folder: a loose script at the top of tools/
    // is not a tool, and three of them were dead when this check was written.
    if (entry.includes(".")) continue;
    for (const file of list(entry)) {
      if (file.endsWith(".ts")) {
        found.push({ tool: entry, name: file.slice(0, -3), source: read(`${entry}/${file}`) });
      }
    }
  }
  return found;
}

// `../<tool>/<module>.ts` is a cross-tool import. Importing from lib/ is the
// supported way to share, and importing a sibling inside the same tool is fine.
export function crossToolImports(found: readonly Module[]): string[] {
  const problems: string[] = [];
  for (const module of found) {
    // This file's own self-test fixtures contain deliberate violations as
    // strings; scanning them would make the checker permanently fail itself.
    if (module.tool === "check" && module.name === "architecture") continue;
    for (const [, tool, name] of module.source.matchAll(/from "\.\.\/([a-z0-9_]+)\/([a-z0-9_]+)\.ts"/g)) {
      if (tool === LIBRARY || tool === module.tool) continue;
      problems.push(
        `tools/${module.tool}/${module.name}.ts imports ${tool}/${name} -- ` +
          `a tool may import lib/, never another tool; move the shared part to lib/`,
      );
    }
  }
  return problems;
}

// Every relative import must resolve, static or dynamic. A rename that misses
// a dynamic `import(...)` is invisible until the branch that calls it runs --
// three separate moves in one session shipped exactly that break.
export function brokenImports(found: readonly Module[]): string[] {
  const present = new Set(found.map((module) => `${module.tool}/${module.name}`));
  const problems: string[] = [];
  for (const module of found) {
    if (module.tool === "check" && module.name === "architecture") continue;
    const pattern = /(?:from |import\()"(\.{1,2})\/(?:([a-z0-9_]+)\/)?([a-z0-9_]+)\.ts"/g;
    for (const [, dots, folder, name] of module.source.matchAll(pattern)) {
      // "./x" is a sibling; "../x" is the tools root; "../folder/x" is another
      // folder. Collapsing these is how a rename silently retargets an import.
      const target = folder !== undefined
        ? `${folder}/${name}`
        : dots === "." ? `${module.tool}/${name}` : `/${name}`;
      if (!present.has(target)) {
        problems.push(`tools/${module.tool}/${module.name}.ts imports ${target || name} which does not exist`);
      }
    }
  }
  return problems;
}

export function unreachable(found: readonly Module[], seeds: ReadonlySet<string>): string[] {
  const byName = new Map(found.map((module) => [module.name, module]));
  const seen = new Set<string>();
  const stack = [...seeds];
  while (stack.length > 0) {
    const name = stack.pop()!;
    if (seen.has(name)) continue;
    seen.add(name);
    const module = byName.get(name);
    if (module === undefined) continue;
    for (const [, target] of module.source.matchAll(/from "\.{1,2}\/(?:[a-z0-9_]+\/)?([a-z0-9_]+)\.ts"/g)) {
      if (!seen.has(target)) stack.push(target);
    }
    for (const [, target] of module.source.matchAll(/tools\/(?:[a-z0-9_]+\/)?([a-z0-9_]+)\.ts/g)) {
      if (!seen.has(target)) stack.push(target);
    }
  }
  return found
    .filter((module) => !seen.has(module.name))
    .map((module) => `tools/${module.tool}/${module.name}.ts is reachable from nothing -- wire it up or delete it`);
}

function seedNames(): Set<string> {
  const text = [
    readFileSync(join(ROOT, "package.json"), "utf8"),
    ...readdirSync(join(ROOT, ".hooks")).map((name) => readFileSync(join(ROOT, ".hooks", name), "utf8")),
    ...readdirSync(ROOT).filter((name) => name.endsWith(".md")).map((name) => readFileSync(join(ROOT, name), "utf8")),
  ].join("\n");
  const seeds = new Set<string>();
  for (const [, name] of text.matchAll(/tools\/(?:[a-z0-9_]+\/)?([a-z0-9_]+)\.ts/g)) seeds.add(name);
  // Each tool's index.ts is an entry point by definition, and it spawns its own
  // subcommands by name rather than importing them.
  for (const entry of readdirSync(TOOLS, { withFileTypes: true })) {
    if (!entry.isDirectory() || entry.name === LIBRARY) continue;
    seeds.add("index");
    for (const file of readdirSync(join(TOOLS, entry.name))) {
      if (file.endsWith(".ts")) seeds.add(file.slice(0, -3));
    }
  }
  return seeds;
}

function selfTest(): void {
  const fake: Module[] = [
    { tool: "overlay", name: "a", source: 'from "../lib/shared.ts"' },
    { tool: "overlay", name: "b", source: 'from "./a.ts"' },
    { tool: "search", name: "c", source: 'from "../overlay/a.ts"' },
    { tool: LIBRARY, name: "shared", source: "" },
  ];
  const cross = crossToolImports(fake);
  if (cross.length !== 1 || !cross[0]!.includes("search")) throw new Error("a cross-tool import must be caught");
  if (crossToolImports(fake.filter((m) => m.tool !== "search")).length !== 0) {
    throw new Error("lib/ and same-tool imports must be allowed");
  }
  const orphans = unreachable(fake, new Set(["b"]));
  if (!orphans.some((line) => line.includes("/c.ts"))) throw new Error("an orphan must be reported");
  if (orphans.some((line) => line.includes("/shared.ts"))) throw new Error("transitive reach must count");
  const listed = modules(
    () => 'from "../lib/x.ts"',
    (path) => (path === "" ? ["overlay", "verify.ts"] : ["index.ts"]),
  );
  if (listed.length !== 2) throw new Error(`modules() found ${listed.length}`);
  console.log("architecture self-test ok");
}

function main(): void {
  if (Bun.argv.includes("--self-test")) return selfTest();
  const found = modules(
    (path) => readFileSync(join(TOOLS, path), "utf8"),
    (path) => readdirSync(join(TOOLS, path)),
  );
  const problems = [...crossToolImports(found), ...unreachable(found, seedNames()), ...brokenImports(found)];
  const documented = Bun.spawnSync(["bun", join(TOOLS, "check", "documented.ts")], { cwd: ROOT });
  if (problems.length > 0) {
    for (const problem of problems) console.error(`error: ${problem}`);
    process.exit(1);
  }
  if (documented.exitCode !== 0) {
    console.error(new TextDecoder().decode(documented.stderr).trim());
    process.exit(1);
  }
  const tools = new Set(found.map((module) => module.tool).filter((tool) => tool !== "" && tool !== LIBRARY));
  console.log(
    `architecture ok: ${tools.size} tools, ${found.filter((m) => m.tool === LIBRARY).length} lib modules, ` +
      `no cross-tool imports, nothing unreachable`,
  );
}

if (import.meta.main) main();
