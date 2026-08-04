#!/usr/bin/env bun
// Hard gate: C and header sources may never use assembly escape hatches.

import { readFileSync, readdirSync, statSync } from "node:fs";
import { dirname, extname, join, relative } from "node:path";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const SOURCE_ROOTS = ["assets", "games", "include", "semantic", "exact"];
const FORBIDDEN = /\b(?:asm|__asm_*)\b/g;

export interface Finding {
  file: string;
  line: number;
  token: string;
}

/** Remove comments and literals while preserving newlines and byte positions. */
export function codeOnly(text: string): string {
  let output = "";
  let state: "code" | "line" | "block" | "string" | "char" = "code";
  for (let index = 0; index < text.length; index++) {
    const char = text[index];
    const next = text[index + 1];
    if (state === "code") {
      if (char === "/" && next === "/") {
        output += "  ";
        index++;
        state = "line";
      } else if (char === "/" && next === "*") {
        output += "  ";
        index++;
        state = "block";
      } else if (char === '"') {
        output += " ";
        state = "string";
      } else if (char === "'") {
        output += " ";
        state = "char";
      } else {
        output += char;
      }
      continue;
    }
    if (state === "line") {
      if (char === "\n") {
        output += "\n";
        state = "code";
      } else output += " ";
      continue;
    }
    if (state === "block") {
      if (char === "*" && next === "/") {
        output += "  ";
        index++;
        state = "code";
      } else output += char === "\n" ? "\n" : " ";
      continue;
    }
    if (char === "\\" && next !== undefined) {
      output += next === "\n" ? " \n" : "  ";
      index++;
    } else if ((state === "string" && char === '"') || (state === "char" && char === "'")) {
      output += " ";
      state = "code";
    } else output += char === "\n" ? "\n" : " ";
  }
  return output;
}

export function findForbidden(file: string, text: string): Finding[] {
  const code = codeOnly(text);
  const findings: Finding[] = [];
  FORBIDDEN.lastIndex = 0;
  for (let match = FORBIDDEN.exec(code); match; match = FORBIDDEN.exec(code)) {
    findings.push({
      file,
      line: code.slice(0, match.index).split("\n").length,
      token: match[0],
    });
  }
  return findings;
}

function sourceFiles(directory: string): string[] {
  const files: string[] = [];
  for (const name of readdirSync(directory).sort()) {
    const path = join(directory, name);
    if (statSync(path).isDirectory()) files.push(...sourceFiles(path));
    else if (extname(name) === ".c" || extname(name) === ".h") files.push(path);
  }
  return files;
}

function selfTest(): void {
  const bad = ["asm(\"\")", "__asm(\"\")", "__asm__(\"r5\")", "__asm_(\"\")"];
  for (const source of bad) {
    if (findForbidden("bad.c", source).length !== 1) throw new Error(`missed forbidden source: ${source}`);
  }
  const clean = '/* asm("") */\nconst char *s = "__asm__(x)";\nint assembly = 1;\n';
  if (findForbidden("clean.c", clean).length !== 0) throw new Error("comments, strings, or longer names misclassified");
  console.log("self-test=ok (C/H assembly escape gate)");
}

function main(): void {
  if (Bun.argv.includes("--self-test")) return selfTest();
  const files = SOURCE_ROOTS.flatMap((root) => sourceFiles(join(ROOT, root)));
  if (files.length === 0) throw new Error("C/H assembly escape gate scanned no files");
  const findings = files.flatMap((path) => findForbidden(relative(ROOT, path), readFileSync(path, "utf8")));
  for (const finding of findings) console.error(`${finding.file}:${finding.line}: forbidden ${finding.token} construct`);
  console.log(`scanned=${files.length} forbidden=${findings.length}`);
  if (findings.length) {
    console.error("C/H ASSEMBLY ESCAPE — hard failure. Use ordinary C or retain the owner as a .s source.");
    process.exitCode = 1;
  }
}

if (import.meta.main) main();
