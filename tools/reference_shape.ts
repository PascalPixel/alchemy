#!/usr/bin/env bun
// Read compiler-decidable facts off each remaining region's own disassembly, so a
// drafter is told them instead of discovering them through a byte diff.
//
//   bun tools/reference_shape.ts [--stem 08011fd8] [--drafts DIR ...] [--mismatch-only]
//
// Why this exists: HANDOVER.md §4 states "the epilogue states the return type:
// `pop {r5} / pop {r0} / bx r0` means void (r0 dead); `pop {r1} / bx r1` means r0
// is live". That fact is free -- it is sitting in `asm/<stem>.s` -- but nothing
// extracted it, so every draft guessed and paid a probe cycle to find out. Getting
// it wrong is invisible in the source and shows up only as an epilogue diff.
// `080b5ad4` was closed on 2026-07-30 by exactly this correction: its
// `pop {r1} / bx r1` tail meant the function returns a value, and respelling the
// tail as `return Func_...(...)` took it from 9 differing halfwords to 0.
//
// It also reports every draft whose declared return type contradicts its
// reference, which is a list of near-certain improvements rather than a guess.
//
// Prints decoded facts derived from the reconstructed assembly this repo already
// tracks; it never emits a raw ROM span.
import { readFileSync, existsSync, readdirSync, statSync } from "node:fs";
import { basename, dirname, join } from "node:path";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

export type Shape =
  | { kind: "value-returning"; register: string }
  | { kind: "void"; register: string }
  | { kind: "leaf-bx-lr" }
  | { kind: "pop-pc" }
  | { kind: "unknown" };

// 末尾の復帰列から戻り値の有無を読む。`pop {rN}` の直後に `bx rN` が続く形が
// 判定できる唯一の形で、rN が r0 なら r0 は死んでいる(void)。
export function shapeOfAssembly(text: string): Shape {
  const lines = text
    .split("\n")
    .map((line) => line.replace(/@.*$/, "").trim())
    .filter((line) => line !== "" && !line.startsWith("."));
  // 復帰は最後の分岐。後ろから最初の `bx` / `pop {..pc}` を探す。
  for (let index = lines.length - 1; index >= 0; index--) {
    const line = lines[index];
    const popPc = /^pop\s*\{([^}]*)\}/.exec(line);
    if (popPc !== null && /\bpc\b/.test(popPc[1])) return { kind: "pop-pc" };
    const branch = /^bx\s+(r\d+|lr|ip)\b/.exec(line);
    if (branch === null) continue;
    const register = branch[1];
    if (register === "lr") return { kind: "leaf-bx-lr" };
    // 直前の `pop {rN}` が同じレジスタを積み直しているか。
    for (let back = index - 1; back >= 0 && back >= index - 3; back--) {
      const popped = /^pop\s*\{([^}]*)\}/.exec(lines[back]);
      if (popped === null) continue;
      const registers = popped[1].split(",").map((name) => name.trim());
      if (!registers.includes(register)) continue;
      return register === "r0"
        ? { kind: "void", register }
        : { kind: "value-returning", register };
    }
    return { kind: "unknown" };
  }
  return { kind: "unknown" };
}

// 下書きが宣言している戻り値型。`Func_<stem>` の定義行から読む。
export function declaredReturnType(source: string, stem: string): string | null {
  const pattern = new RegExp(
    `(?:^|\\n)\\s*([A-Za-z_][A-Za-z0-9_]*(?:\\s+[A-Za-z_][A-Za-z0-9_]*)*\\s*\\**)\\s*Func_${stem}\\s*\\(`,
  );
  const match = pattern.exec(source);
  if (match === null) return null;
  const words = match[1].trim().replace(/\s+/g, " ");
  if (words === "" || /^(?:extern|static|inline)$/.test(words)) return null;
  return words;
}

export function returnsValue(declared: string): boolean {
  return !/^(?:void)$/.test(declared.replace(/\s*\*+$/, "").trim()) || /\*/.test(declared);
}

function selfTest(): void {
  const valueTail = "push {r5, lr}\npop {r5}\npop {r1}\nbx r1\n";
  const shape = shapeOfAssembly(valueTail);
  if (shape.kind !== "value-returning") throw new Error("self-test: pop {r1}/bx r1 must be value-returning");
  const voidTail = "push {r5, lr}\npop {r5}\npop {r0}\nbx r0\n";
  if (shapeOfAssembly(voidTail).kind !== "void") throw new Error("self-test: pop {r0}/bx r0 must be void");
  if (shapeOfAssembly("bx lr\n").kind !== "leaf-bx-lr") throw new Error("self-test: bx lr must be leaf");
  if (shapeOfAssembly("push {r4, lr}\npop {r4, pc}\n").kind !== "pop-pc") throw new Error("self-test: pop pc");
  if (declaredReturnType("s32 Func_08011fd8(s32 a)\n{\n}\n", "08011fd8") !== "s32") {
    throw new Error("self-test: declared return type");
  }
  if (declaredReturnType("void Func_08011fd8(void)\n{}\n", "08011fd8") !== "void") {
    throw new Error("self-test: declared void");
  }
  if (returnsValue("void")) throw new Error("self-test: void does not return a value");
  if (!returnsValue("u16")) throw new Error("self-test: u16 returns a value");
  if (!returnsValue("s16 *")) throw new Error("self-test: pointer returns a value");
  console.log("self-test=ok");
}

const argv = Bun.argv.slice(2);
if (argv.includes("--self-test")) { selfTest(); process.exit(0); }
selfTest();

let onlyStem: string | null = null;
let mismatchOnly = false;
const draftRoots: string[] = [join(ROOT, "work")];
for (let index = 0; index < argv.length; index++) {
  if (argv[index] === "--stem") onlyStem = argv[++index];
  else if (argv[index] === "--mismatch-only") mismatchOnly = true;
  else if (argv[index] === "--drafts") draftRoots.length = 0, draftRoots.push(argv[++index]);
}

// 下書きの索引。同じ stem に複数あるので、すべて突き合わせる。
const drafts = new Map<string, string[]>();
function walk(directory: string): void {
  let entries: string[];
  try { entries = readdirSync(directory); } catch { return; }
  for (const entry of entries) {
    const path = join(directory, entry);
    let info;
    try { info = statSync(path); } catch { continue; }
    if (info.isDirectory()) walk(path);
    else if (/^[0-9a-f]{8}\.c$/.test(entry)) {
      const stem = basename(entry, ".c");
      const list = drafts.get(stem) ?? [];
      list.push(path);
      drafts.set(stem, list);
    }
  }
}
for (const root of draftRoots) walk(root);

const stems = onlyStem !== null
  ? [onlyStem]
  : readdirSync(join(ROOT, "asm")).filter((name) => name.endsWith(".s")).map((name) => basename(name, ".s")).sort();

const counts = new Map<string, number>();
const mismatches: string[] = [];
for (const stem of stems) {
  const path = join(ROOT, "asm", `${stem}.s`);
  if (!existsSync(path)) continue;
  const shape = shapeOfAssembly(readFileSync(path, "utf8"));
  counts.set(shape.kind, (counts.get(shape.kind) ?? 0) + 1);
  if (shape.kind !== "void" && shape.kind !== "value-returning") {
    if (!mismatchOnly && onlyStem !== null) console.log(`${stem}\t${shape.kind}`);
    continue;
  }
  const wants = shape.kind === "value-returning";
  const paths = drafts.get(stem) ?? [];
  const disagreeing: string[] = [];
  for (const draft of paths) {
    const declared = declaredReturnType(readFileSync(draft, "utf8"), stem);
    if (declared === null) continue;
    if (returnsValue(declared) !== wants) disagreeing.push(`${draft} declares ${declared}`);
  }
  if (!mismatchOnly) {
    console.log(`${stem}\t${shape.kind}\tvia ${shape.register}\tdrafts=${paths.length}\tdisagreeing=${disagreeing.length}`);
  }
  if (disagreeing.length > 0) {
    mismatches.push(`${stem}\twants ${wants ? "a value-returning" : "void"} tail (${shape.kind} via ${shape.register})\t${disagreeing.length}/${paths.length} drafts disagree`);
    if (mismatchOnly) {
      console.log(mismatches[mismatches.length - 1]);
      for (const line of disagreeing.slice(0, 3)) console.log(`    ${line}`);
    }
  }
}

console.log("---");
for (const [kind, count] of [...counts].sort((a, b) => b[1] - a[1])) console.log(`${kind}=${count}`);
console.log(`regions=${stems.length} stems_with_disagreeing_drafts=${mismatches.length}`);
