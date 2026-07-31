#!/usr/bin/env bun
// Permute independent top-level statements of one MAIN-IMAGE draft and keep the
// ordering that matches the reference best.
//
//   bun tools/statement_order_sweep_main.ts <draft.c> [--flags -fa,-fb] [--out FILE]
//                                           [--max-run N] [--jobs N]
//
// Why this exists: HANDOVER.md §4 records that store order moves register
// allocation — allocno priority is 2*refs/live_length, and a parameter's store
// position sets its live length, so permuting independent store groups is the
// general fix when two equal-priority pseudos land in the wrong registers. It
// notes 24 orderings were tried on one function and two of them gave zero where
// the obvious one gave 15. That search was done by hand. §5 adds that a
// permuter's single win in 65,543 candidates was one statement swap.
//
// Nothing automated that lever, so every "register-identity-only swap" park (§6
// lists the class as real) was triaged without it having been swept. This sweeps
// it exhaustively for small runs, which is where the entry-block parameter saves
// live.
//
// Only *independent* statements are permuted, so the transformation preserves
// semantics: a candidate ordering is generated only when no statement in the run
// reads a name another writes, and any statement touching memory or calling a
// function acts as a barrier against other such statements. Plain declarations
// are not memory accesses: the `*` in `s16 *cursor;` is a declarator. Keeping it
// behind the memory barrier hid declaration-order register-allocation wins.
import { readFileSync, writeFileSync, mkdirSync, rmSync } from "node:fs";
import { basename, dirname, join } from "node:path";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

interface Options {
  draft: string;
  out: string;
  flags: string;
  maxRun: number;
  jobs: number;
}

function parseArguments(argv: string[]): Options {
  const options: Options = { draft: "", out: "", flags: "", maxRun: 6, jobs: 12 };
  const rest: string[] = [];
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--flags") options.flags = argv[++index];
    else if (argument === "--out") options.out = argv[++index];
    else if (argument === "--max-run") options.maxRun = Number(argv[++index]);
    else if (argument === "--jobs") options.jobs = Number(argv[++index]);
    else if (argument === "-h" || argument === "--help") {
      console.log("usage: statement_order_sweep_main.ts <draft.c> [--flags -fa,-fb] [--out FILE] [--max-run N] [--jobs N]");
      process.exit(0);
    } else rest.push(argument);
  }
  if (rest.length !== 1) throw new Error("usage: statement_order_sweep_main.ts <draft.c>");
  options.draft = rest[0];
  if (options.out === "") options.out = options.draft.replace(/\.c$/, ".ordered.c");
  return options;
}

// 関数本体を切り出す。最初の `Func_` 定義の `{` から対応する `}` まで。
export function functionBody(source: string): { start: number; end: number } | null {
  const signature = /\b[A-Za-z_][A-Za-z0-9_ *]*\bFunc_[0-9a-f]{8}\s*\([^)]*\)\s*\{/.exec(source);
  if (signature === null) return null;
  const start = signature.index + signature[0].length;
  let depth = 1;
  for (let at = start; at < source.length; at++) {
    if (source[at] === "{") depth++;
    else if (source[at] === "}") {
      depth--;
      if (depth === 0) return { start, end: at };
    }
  }
  return null;
}

// 本体を深さ1の「文」に割る。ブロックや制御構文は一つの塊として扱う。
export function topLevelStatements(body: string): Array<{ text: string; simple: boolean }> {
  const pieces: Array<{ text: string; simple: boolean }> = [];
  let depth = 0;
  let current = "";
  for (let at = 0; at < body.length; at++) {
    const character = body[at];
    current += character;
    if (character === "{") depth++;
    else if (character === "}") {
      depth--;
      if (depth === 0) { pieces.push({ text: current, simple: false }); current = ""; }
    } else if (character === ";" && depth === 0) {
      pieces.push({ text: current, simple: true }); current = "";
    }
  }
  // Preserve trailing whitespace before the function's closing brace. Dropping
  // it made an otherwise valid reordered source end in the needlessly ugly `}}`.
  if (current !== "") pieces.push({ text: current, simple: false });
  return pieces;
}

const KEYWORDS = new Set([
  "if", "else", "for", "while", "do", "switch", "case", "default", "return",
  "goto", "break", "continue", "sizeof", "struct", "union", "enum", "typedef",
  "const", "volatile", "signed", "unsigned", "int", "char", "short", "long",
  "void", "float", "double", "s8", "u8", "s16", "u16", "s32", "u32", "s64", "u64",
]);

function identifiers(text: string): string[] {
  return (text.match(/[A-Za-z_][A-Za-z0-9_]*/g) ?? []).filter((name) => !KEYWORDS.has(name));
}

// 代入対象と参照名をおおまかに拾う。判定は必ず安全側に倒す。
export function writesOf(text: string): Set<string> {
  const written = new Set<string>();
  const assignment = /([A-Za-z_][A-Za-z0-9_]*)\s*(?:\[[^\]]*\]|->[A-Za-z0-9_]+|\.[A-Za-z0-9_]+)*\s*(?:=[^=]|\+=|-=|\*=|\/=|%=|&=|\|=|\^=|<<=|>>=|\+\+|--)/.exec(text);
  if (assignment !== null && !KEYWORDS.has(assignment[1])) written.add(assignment[1]);
  const declaration = /^\s*(?:const\s+|volatile\s+)*(?:[A-Za-z_][A-Za-z0-9_]*)\s+\**\s*([A-Za-z_][A-Za-z0-9_]*)\s*=/.exec(text);
  if (declaration !== null) written.add(declaration[1]);
  return written;
}

function touchesMemoryOrCalls(text: string): boolean {
  if (isPlainDeclaration(text)) return false;
  return /\*|\[|->|\w\s*\(/.test(text.replace(/^\s*[A-Za-z_][A-Za-z0-9_]*\s*=/, ""));
}

// An uninitialised, fixed-size local declaration has no run-time effect. Reject
// variable-length arrays and anything with an initializer; those can evaluate
// expressions and must retain their source position.
export function isPlainDeclaration(text: string): boolean {
  const declaration = text.trim();
  if (declaration.includes("=") || !declaration.endsWith(";")) return false;
  if (/\[[^\]]*[A-Za-z_][A-Za-z0-9_]*[^\]]*\]/.test(declaration)) return false;
  return /^(?:(?:register|const|volatile|signed|unsigned|short|long)\s+)*(?:(?:struct|union|enum)\s+[A-Za-z_][A-Za-z0-9_]*|[A-Za-z_][A-Za-z0-9_]*)\s+(?:\*+\s*)?[A-Za-z_][A-Za-z0-9_]*(?:\s*\[[^\]]+\])?\s*;$/.test(declaration);
}

export function independent(a: string, b: string): boolean {
  const writesA = writesOf(a), writesB = writesOf(b);
  const readsA = new Set(identifiers(a)), readsB = new Set(identifiers(b));
  for (const name of writesA) if (readsB.has(name)) return false;
  for (const name of writesB) if (readsA.has(name)) return false;
  // 両方が記憶域を触るなら順序を保つ。別名かどうかを源から証明できない。
  if (touchesMemoryOrCalls(a) && touchesMemoryOrCalls(b)) return false;
  return true;
}

// 相互に独立な単純文の極大連続列。
export function independentRuns(
  pieces: Array<{ text: string; simple: boolean }>,
  maxRun: number,
): Array<[number, number]> {
  const runs: Array<[number, number]> = [];
  let index = 0;
  while (index < pieces.length) {
    if (!pieces[index].simple) { index++; continue; }
    let end = index;
    while (end + 1 < pieces.length && pieces[end + 1].simple) {
      let mutual = true;
      for (let earlier = index; earlier <= end; earlier++) {
        if (!independent(pieces[earlier].text, pieces[end + 1].text)) { mutual = false; break; }
      }
      if (!mutual) break;
      end++;
    }
    if (end - index + 1 >= 2) runs.push([index, Math.min(end, index + maxRun - 1)]);
    index = end + 1;
  }
  return runs;
}

function selfTest(): void {
  const declarations = [
    "s16 *cursor;",
    "volatile s32 *cursor;",
    "s32 values[4];",
    "struct Position *position;",
  ];
  for (const declaration of declarations) {
    if (!isPlainDeclaration(declaration)) {
      throw new Error(`statement-order self-test rejected declaration: ${declaration}`);
    }
  }
  for (const statement of ["s32 value = 0;", "s32 values[count];", "*cursor = value;"]) {
    if (isPlainDeclaration(statement)) {
      throw new Error(`statement-order self-test accepted effectful statement: ${statement}`);
    }
  }
  const pieces = topLevelStatements("\n    s16 *cursor;\n    volatile s32 *other;\n");
  if (pieces.length !== 3 || !pieces[0].simple || !pieces[1].simple || pieces[2].text !== "\n") {
    throw new Error("statement-order self-test did not preserve trailing whitespace");
  }
  const runs = independentRuns(pieces, 6);
  if (runs.length !== 1 || runs[0][0] !== 0 || runs[0][1] !== 1) {
    throw new Error("statement-order self-test did not expose pointer declarations");
  }
  console.log("self-test=ok");
}

function* permutations<T>(items: readonly T[]): Generator<T[]> {
  if (items.length <= 1) { yield [...items]; return; }
  for (let index = 0; index < items.length; index++) {
    const rest = [...items.slice(0, index), ...items.slice(index + 1)];
    for (const tail of permutations(rest)) yield [items[index], ...tail];
  }
}

if (Bun.argv.length === 3 && Bun.argv[2] === "--self-test") {
  selfTest();
  process.exit(0);
}

const options = parseArguments(Bun.argv.slice(2));
const stem = basename(options.draft, ".c");
const original = readFileSync(options.draft, "utf8");
const bounds = functionBody(original);
if (bounds === null) throw new Error(`no Func_ body found in ${options.draft}`);
const pieces = topLevelStatements(original.slice(bounds.start, bounds.end));

const scratch = join(ROOT, "out/statement-order", stem);
rmSync(scratch, { recursive: true, force: true });
mkdirSync(scratch, { recursive: true });

// 採点は candidate_show の一行目に任せる。経路名は必ず <stem>.c にする。
// 別名にすると §7 の経路トラップでフラグが素の設定に落ちる。
let probes = 0;
async function score(source: string, slot: number): Promise<{ diff: number; size: number; reference: number } | null> {
  const directory = join(scratch, `s${slot}`);
  mkdirSync(directory, { recursive: true });
  const path = join(directory, `${stem}.c`);
  writeFileSync(path, source);
  const command = ["bun", "tools/candidate_show.ts", path, "--work", join(directory, "w")];
  if (options.flags !== "") command.push("--flags", options.flags);
  const child = Bun.spawn(command, { cwd: ROOT, stdout: "pipe", stderr: "pipe" });
  const [text] = await Promise.all([new Response(child.stdout).text(), child.exited]);
  probes++;
  const match = /candidate=(\d+) reference=(\d+) differing_halfwords=(\d+)/.exec(text);
  if (match === null) return null;
  return { diff: Number(match[3]), size: Number(match[1]), reference: Number(match[2]) };
}

function render(current: Array<{ text: string; simple: boolean }>): string {
  return original.slice(0, bounds!.start) + current.map((piece) => piece.text).join("") + original.slice(bounds!.end);
}

const baseline = await score(original, 0);
if (baseline === null) throw new Error(`draft does not compile: ${options.draft}`);
console.log(`start differing_halfwords=${baseline.diff} size=${baseline.size}/${baseline.reference}`);

let bestPieces = pieces;
let best = baseline;
let improved = true;
while (improved) {
  improved = false;
  for (const [from, to] of independentRuns(bestPieces, options.maxRun)) {
    const run = bestPieces.slice(from, to + 1);
    const candidates: Array<Array<{ text: string; simple: boolean }>> = [];
    for (const ordering of permutations(run)) {
      if (ordering.every((piece, at) => piece === run[at])) continue;
      candidates.push([...bestPieces.slice(0, from), ...ordering, ...bestPieces.slice(to + 1)]);
    }
    // 並べ替えは互いに独立なので、まとめて走らせて最良だけ採る。
    for (let at = 0; at < candidates.length; at += options.jobs) {
      const slice = candidates.slice(at, at + options.jobs);
      const scored = await Promise.all(slice.map((candidate, slot) => score(render(candidate), slot + 1)));
      for (let which = 0; which < slice.length; which++) {
        const result = scored[which];
        if (result === null) continue;
        const better = result.diff < best.diff ||
          (result.diff === best.diff && result.size === result.reference && best.size !== best.reference);
        if (better) {
          best = result; bestPieces = slice[which]; improved = true;
          console.log(`  reorder [${from}..${to}]   differing_halfwords=${best.diff} size=${best.size}/${best.reference}`);
        }
      }
      if (best.diff === 0 && best.size === best.reference) break;
    }
    if (best.diff === 0 && best.size === best.reference) break;
  }
}

console.log(`final differing_halfwords=${best.diff} size=${best.size}/${best.reference} probes=${probes}`);
if (bestPieces !== pieces) {
  writeFileSync(options.out, render(bestPieces));
  console.log(`wrote reordered draft to ${options.out}`);
}
rmSync(scratch, { recursive: true, force: true });
