#!/usr/bin/env bun
// Tool role: entrypoint; invoked by package.json.
// Audit tracked TypeScript tools by both imports and explicit tool-path use.
import { existsSync, readFileSync, writeFileSync } from "node:fs";
import { basename, dirname, join } from "node:path";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const HEADER = /^\/\/ Tool role: (entrypoint|library|both|dead); /;

interface Classification {
  path: string;
  role: "entrypoint" | "library" | "both" | "dead";
  importedBy: string[];
  invokedBy: string[];
}

function trackedFiles(): string[] {
  const result = Bun.spawnSync(["git", "ls-files"], { cwd: ROOT, stdout: "pipe", stderr: "pipe" });
  if (result.exitCode !== 0) throw new Error(result.stderr.toString().trim() || "git ls-files failed");
  return result.stdout.toString().split(/\r?\n/).filter(Boolean);
}

function context(paths: string[], verb: string): string {
  if (paths.length === 0) return "no tracked importer or invocation; audit before use";
  const shown = paths.slice(0, 3).join(", ");
  return `${verb} ${shown}${paths.length > 3 ? ` (+${paths.length - 3} more)` : ""}`;
}

function classify(files: string[]): Classification[] {
  const tools = files.filter((name) => /^tools\/[^/]+\.ts$/.test(name)).sort();
  const readable = files.filter((name) => /(?:\.md|\.ts|package\.json)$/.test(name) && existsSync(join(ROOT, name)));
  const documents = new Map(readable.map((name) => [
    name,
    readFileSync(join(ROOT, name), "utf8").replace(/^\/\/ Tool role:.*$/m, ""),
  ]));
  return tools.map((path) => {
    const name = basename(path), module = name.replace(/\.ts$/, "");
    const importPattern = new RegExp(`(?:from\\s+|import\\()(["'])[^"']*(?:/|^)${module}\\.ts\\1`, "g");
    const invocation = `tools/${name}`;
    const importedBy: string[] = [], invokedBy: string[] = [];
    for (const [source, text] of documents) {
      if (source !== path && importPattern.test(text)) importedBy.push(source);
      importPattern.lastIndex = 0;
      if (source !== path && text.includes(invocation)) invokedBy.push(source);
    }
    const role = importedBy.length > 0 && invokedBy.length > 0 ? "both"
      : importedBy.length > 0 ? "library" : invokedBy.length > 0 ? "entrypoint" : "dead";
    return { path, role, importedBy, invokedBy };
  });
}

function expectedHeader(item: Classification): string {
  if (item.role === "both") {
    return `// Tool role: both; ${context(item.importedBy, "imported by")}; ${context(item.invokedBy, "invoked by")}.`;
  }
  if (item.role === "library") return `// Tool role: library; ${context(item.importedBy, "imported by")}.`;
  if (item.role === "entrypoint") return `// Tool role: entrypoint; ${context(item.invokedBy, "invoked by")}.`;
  return `// Tool role: dead; ${context([], "")}.`;
}

function missingReferences(files: string[]): string[] {
  const present = new Set(files);
  const readable = files.filter((name) => /(?:\.md|\.ts|package\.json)$/.test(name) && existsSync(join(ROOT, name)));
  const missing = new Set<string>();
  for (const source of readable) {
    const text = readFileSync(join(ROOT, source), "utf8").replace(/^\/\/ Tool role:.*$/m, "");
    for (const match of text.matchAll(/tools\/([A-Za-z0-9_-]+\.ts)\b/g)) {
      const target = `tools/${match[1]}`;
      if (!present.has(target)) missing.add(`${source} -> ${target}`);
    }
    if (source.startsWith("tools/")) {
      for (const match of text.matchAll(/(?:from\s+|import\()(["'])\.\/([^"']+\.ts)\1/g)) {
        const target = `tools/${match[2]}`;
        if (!present.has(target)) missing.add(`${source} -> ${target}`);
      }
    }
  }
  return [...missing].sort();
}

function writeHeaders(items: Classification[]): void {
  for (const item of items) {
    const path = join(ROOT, item.path), lines = readFileSync(path, "utf8").split("\n");
    const index = lines[0]?.startsWith("#!") ? 1 : 0;
    const header = expectedHeader(item);
    if (HEADER.test(lines[index] ?? "")) lines[index] = header;
    else lines.splice(index, 0, header);
    writeFileSync(path, lines.join("\n"));
  }
}

function main(arguments_: string[]): void {
  const write = arguments_.includes("--write-headers");
  if (arguments_.some((argument) => !["--write-headers", "--self-test"].includes(argument))) {
    throw new Error("usage: check_tool_references.ts [--self-test] [--write-headers]");
  }
  const files = trackedFiles(), items = classify(files);
  const missing = missingReferences(files);
  if (missing.length > 0) throw new Error(`missing tool references:\n${missing.join("\n")}`);
  if (write) writeHeaders(items);
  const stale = items.filter((item) => {
    const lines = readFileSync(join(ROOT, item.path), "utf8").split("\n");
    const index = lines[0]?.startsWith("#!") ? 1 : 0;
    return lines[index] !== expectedHeader(item);
  });
  if (stale.length > 0) throw new Error(`missing or stale tool-role headers:\n${stale.map((item) => item.path).join("\n")}`);
  const counts = Object.fromEntries(["entrypoint", "library", "both", "dead"].map((role) =>
    [role, items.filter((item) => item.role === role).length]));
  console.log(`tools=${items.length} entrypoint=${counts.entrypoint} library=${counts.library} both=${counts.both} dead=${counts.dead}`);
  for (const item of items.filter((entry) => entry.role === "dead")) console.log(`dead ${item.path}`);
}

if (import.meta.main) main(Bun.argv.slice(2));
