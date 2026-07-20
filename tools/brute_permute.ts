#!/usr/bin/env bun
// 総当たり置換探索。m2c下書きへ機械的な等価変形をランダムに積み重ね、
// 承認コンパイラのバイト差分を山登りで縮める。推論ではなく試行回数で
// レジスタ割付とスケジューリングの偶発差を破る。
// 一致した関数は長さ検査の上でsrc/へ設置し、対応するasm/を退役させる。
import { existsSync, mkdirSync, readFileSync, readdirSync, rmSync, writeFileSync } from "node:fs";
import { basename, dirname, join } from "node:path";
import { M2C_PREAMBLE, verifyCandidate } from "./match_m2c.ts";
import { candidates, replaceableAssembly, retainedAssemblyStems, threadLeadingArgument, volatileHardware, postIncrementWalk } from "./permute_m2c.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

interface Options {
  rom: string;
  drafts: string;
  report: string;
  maxMismatch: number;
  steps: number;
  restarts: number;
  jobs: number;
  limit?: number;
  shard?: [number, number];
  seed: number;
}

function parseArguments(argv: string[]): Options {
  const options: Options = {
    rom: join(ROOT, "gs1-en.gba"),
    drafts: join(ROOT, "work/m2c-ctx"),
    report: join(ROOT, "work/matches/m2c.json"),
    maxMismatch: 48,
    steps: 400,
    restarts: 6,
    jobs: Math.max(2, Math.min(14, (navigator.hardwareConcurrency || 4) - 2)),
    seed: 0,
  };
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--drafts") options.drafts = argv[++index];
    else if (argument === "--report") options.report = argv[++index];
    else if (argument === "--max-mismatch") options.maxMismatch = Number(argv[++index]);
    else if (argument === "--steps") options.steps = Number(argv[++index]);
    else if (argument === "--restarts") options.restarts = Number(argv[++index]);
    else if (argument === "--jobs") options.jobs = Number(argv[++index]);
    else if (argument === "--limit") options.limit = Number(argv[++index]);
    else if (argument === "--seed") options.seed = Number(argv[++index]);
    else if (argument === "--shard") {
      const [part, total] = argv[++index].split("/").map(Number);
      options.shard = [part, total];
    }
    else if (argument === "-h" || argument === "--help") {
      console.log("usage: brute_permute.ts [--drafts DIR] [--report FILE] [--max-mismatch N] [--steps N] [--restarts N] [--jobs N] [--limit N]");
      process.exit(0);
    } else options.rom = argument;
  }
  return options;
}

// 再現可能な擬似乱数。関数アドレスと再開番号から種を導く。
function makeRandom(seed: number): () => number {
  let state = seed >>> 0 || 1;
  return () => {
    state ^= state << 13; state >>>= 0;
    state ^= state >> 17;
    state ^= state << 5; state >>>= 0;
    return state / 0x100000000;
  };
}

const TYPE_FLIPS: Array<[RegExp, string]> = [
  [/\bs32 (\w+);/, "u32 $1;"],
  [/\bu32 (\w+);/, "s32 $1;"],
  [/\bu8 (\w+);/, "s32 $1;"],
  [/\bs16 (\w+);/, "s32 $1;"],
];

function lines(body: string): string[] {
  return body.split("\n");
}

function identifiers(text: string): Set<string> {
  return new Set(text.match(/[A-Za-z_]\w*/g) ?? []);
}

function independent(a: string, b: string): boolean {
  // 双方が代入や呼出しを含む単文で、識別子集合が交差しないときだけ入替える。
  if (/\b(if|while|do|for|else|return|switch|case|break)\b/.test(a + b)) return false;
  if (!a.trim().endsWith(";") || !b.trim().endsWith(";")) return false;
  const left = identifiers(a), right = identifiers(b);
  for (const name of left) if (right.has(name) && !/^(s8|u8|s16|u16|s32|u32|void|M2C_FIELD)$/.test(name)) return false;
  return true;
}

function mutate(body: string, random: () => number): string {
  const roll = random();
  const rows = lines(body);
  if (roll < 0.2) {
    // 隣接する独立文の入替
    const starts: number[] = [];
    for (let index = 0; index + 1 < rows.length; index++) {
      if (independent(rows[index], rows[index + 1])) starts.push(index);
    }
    if (starts.length === 0) return body;
    const pick = starts[Math.floor(random() * starts.length)];
    const copy = [...rows];
    [copy[pick], copy[pick + 1]] = [copy[pick + 1], copy[pick]];
    return copy.join("\n");
  }
  if (roll < 0.35) {
    // 宣言型の揺らし
    const [pattern, replacement] = TYPE_FLIPS[Math.floor(random() * TYPE_FLIPS.length)];
    const matches = [...body.matchAll(new RegExp(pattern.source, "g"))];
    if (matches.length === 0) return body;
    const target = matches[Math.floor(random() * matches.length)];
    return body.slice(0, target.index) + target[0].replace(pattern, replacement) + body.slice(target.index! + target[0].length);
  }
  if (roll < 0.5) {
    // 可換演算の左右反転
    const pattern = /\(([\w>\[\]().* ]{1,24}) (\+|\||&|\^) ([\w>\[\]().* ]{1,24})\)/g;
    const matches = [...body.matchAll(pattern)];
    if (matches.length === 0) return body;
    const target = matches[Math.floor(random() * matches.length)];
    const swapped = `(${target[3]} ${target[2]} ${target[1]})`;
    return body.slice(0, target.index) + swapped + body.slice(target.index! + target[0].length);
  }
  if (roll < 0.62) {
    // 定数と部分式の局所変数化
    const pattern = /0x0[238][0-9A-Fa-f]{6}|0x[0-9A-Fa-f]{3,5}\b/g;
    const matches = [...new Set(body.match(pattern) ?? [])];
    if (matches.length === 0) return body;
    const value = matches[Math.floor(random() * matches.length)];
    const opening = /\)\s*\{\n/.exec(body);
    if (opening === null || body.includes(`= ${value};`)) return body;
    const cut = opening.index + opening[0].length;
    const name = `_h${Math.floor(random() * 1000)}`;
    let rest = body.slice(cut).replaceAll(value, name);
    return `${body.slice(0, cut)}    s32 ${name} = ${value};\n${rest}`;
  }
  if (roll < 0.72) {
    // 二重使用式の一時変数化: X = A; の右辺を別文に持ち上げる
    const pattern = /^(\s+)(\w[\w>.\[\]*]*) = (.{8,90});$/gm;
    const matches = [...body.matchAll(pattern)];
    if (matches.length === 0) return body;
    const target = matches[Math.floor(random() * matches.length)];
    if (target[3].includes("_t_")) return body;
    const name = `_t_${Math.floor(random() * 1000)}`;
    const replaced = `${target[1]}{ s32 ${name} = (s32) (${target[3]}); ${target[2]} = ${name}; }`;
    return body.slice(0, target.index) + replaced + body.slice(target.index! + target[0].length);
  }
  if (roll < 0.82) {
    // while ↔ if+do の回転
    const whilePattern = /(\s+)while \((.+)\) \{\n([\s\S]+?)\n(\s+)\}/;
    const match = whilePattern.exec(body);
    if (match !== null) {
      return body.replace(whilePattern, `$1if ($2) {\n$1    do {\n$3\n$1    } while ($2);\n$4}`);
    }
    return body;
  }
  if (roll < 0.86) {
    // 条件反転: if (X) A else B ↔ if (!(X)) B else A
    const pattern = /if \(([^)]+)\) \{\n([\s\S]+?\n)(\s+)\} else \{\n([\s\S]+?\n)(\s+)\}/;
    const match = pattern.exec(body);
    if (match === null) return body;
    return body.replace(pattern, `if (!(${match[1]})) {\n${match[4]}${match[3]}} else {\n${match[2]}${match[5]}}`);
  }
  if (roll < 0.92) {
    // 複合代入の展開と圧縮: x += K ↔ x = x + K
    if (random() < 0.5) {
      const pattern = /(\w+) \+= ([^;]+);/;
      const match = pattern.exec(body);
      if (match === null) return body;
      return body.replace(pattern, `${match[1]} = ${match[1]} + ${match[2]};`);
    }
    const pattern = /(\w+) = \1 \+ ([^;]+);/;
    const match = pattern.exec(body);
    if (match === null) return body;
    return body.replace(pattern, `${match[1]} += ${match[2]};`);
  }
  if (roll < 0.96) return volatileHardware(body);
  return postIncrementWalk(body);
}

interface Target {
  stem: string;
  mismatch: number;
}

interface Score {
  mismatch: number;
  size: number;
}

async function score(source: string, stem: string, rom: Uint8Array, scratch: string, expectedSize: number): Promise<Score> {
  const candidate = join(scratch, `${stem}.c`);
  writeFileSync(candidate, source);
  try {
    const { actual, size } = await verifyCandidate(candidate, rom, scratch);
    const address = Number.parseInt(stem, 16) - 0x08000000;
    const expected = Buffer.from(rom).subarray(address, address + expectedSize);
    if (actual.length !== expected.length) {
      return { mismatch: Math.abs(actual.length - expected.length) + 64, size };
    }
    let mismatch = 0;
    for (let index = 0; index < actual.length; index++) if (actual[index] !== expected[index]) mismatch++;
    return { mismatch, size };
  } catch {
    return { mismatch: Number.MAX_SAFE_INTEGER, size: 0 };
  }
}

async function main(): Promise<void> {
  const options = parseArguments(Bun.argv.slice(2));
  const rom = readFileSync(options.rom);
  const output = join(ROOT, "out/brute");
  mkdirSync(output, { recursive: true });
  const tracked = new Set(readdirSync(join(ROOT, "src")).filter((name) => name.endsWith(".c")).map((name) => basename(name, ".c")));
  const retained = retainedAssemblyStems();
  const asmSizes = new Map<string, number>();
  const asmManifest = [join(ROOT, "out/full/asm/manifest.json"), join(ROOT, "out/asm/manifest.json")].find(existsSync);
  if (asmManifest) {
    const document = JSON.parse(readFileSync(asmManifest, "utf8")) as { regions?: Array<{ source: string; size: number }> };
    for (const region of document.regions ?? []) asmSizes.set(basename(region.source, ".s"), region.size);
  }
  const rows = JSON.parse(readFileSync(options.report, "utf8")) as Array<{ entry: number; matched?: boolean; mismatched_bytes?: number }>;
  let targets: Target[] = rows
    .filter((row) => !row.matched && (row.mismatched_bytes ?? 9999) <= options.maxMismatch)
    .map((row) => ({ stem: row.entry.toString(16).padStart(8, "0"), mismatch: row.mismatched_bytes ?? 9999 }))
    .filter((target) => !tracked.has(target.stem) && !retained.has(target.stem) && existsSync(join(ROOT, "asm", `${target.stem}.s`)) && existsSync(join(options.drafts, `${target.stem}.c`)))
    .sort((left, right) => left.mismatch - right.mismatch);
  if (options.shard !== undefined) targets = targets.filter((_, index) => index % options.shard![1] === options.shard![0]);
  if (options.limit !== undefined) targets = targets.slice(0, options.limit);
  console.log(`targets=${targets.length} steps=${options.steps} restarts=${options.restarts} jobs=${options.jobs}`);

  let matchedCount = 0;
  let cursor = 0;
  async function worker(workerIndex: number): Promise<void> {
    const scratch = join(output, `w${workerIndex}`);
    mkdirSync(scratch, { recursive: true });
    while (true) {
      const index = cursor++;
      if (index >= targets.length) return;
      const target = targets[index];
      const raw = readFileSync(join(options.drafts, `${target.stem}.c`), "utf8");
      const bases: string[] = [];
      // 前回までの最良候補があれば出発点に加える。
      const saved = join(ROOT, "out/brute/best", `${target.stem}.c`);
      if (existsSync(saved)) bases.push(readFileSync(saved, "utf8").replace(M2C_PREAMBLE, ""));
      const unknowns = raw.includes("M2C_UNK") ? ["void", "s32", "u32", "u8"] : [null as unknown as string];
      for (const unknown of unknowns) {
        const seeded = unknown === null ? raw : raw.replaceAll("M2C_UNK", unknown);
        bases.push(seeded, ...candidates(seeded));
      }
      let best: { body: string; mismatch: number } | null = null;
      const expectedSize = asmSizes.get(target.stem);
      if (expectedSize === undefined) continue;
      for (const base of new Set(bases)) {
        const { mismatch } = await score(M2C_PREAMBLE + base, target.stem, rom, scratch, expectedSize);
        if (best === null || mismatch < best.mismatch) best = { body: base, mismatch };
      }
      if (best === null || best.mismatch > 96) continue;
      let done = false;
      for (let restart = 0; restart < options.restarts && !done; restart++) {
        const random = makeRandom(Number.parseInt(target.stem, 16) ^ ((restart + options.seed * 64) * 0x9e3779b9));
        let current = { ...best };
        let sinceImprovement = 0;
        for (let step = 0; step < options.steps; step++) {
          // 300手改善が無ければ高原とみなし、この再開を打ち切る。
          if (sinceImprovement >= 300) break;
          const next = mutate(current.body, random);
          if (next === current.body) continue;
          const { mismatch: value, size } = await score(M2C_PREAMBLE + next, target.stem, rom, scratch, expectedSize);
          sinceImprovement++;
          if (value < current.mismatch || (value === current.mismatch && random() < 0.25)) {
            if (value < current.mismatch) sinceImprovement = 0;
            current = { body: next, mismatch: value };
            if (value < best.mismatch) best = { ...current };
          }
          if (value === 0) {
            const installed = M2C_PREAMBLE + next;
            if (!replaceableAssembly(target.stem, size, scratch)) {
              console.log(`skip ${target.stem}: asm region longer than matched C`);
            } else {
              writeFileSync(join(ROOT, "src", `${target.stem}.c`), installed);
              rmSync(join(ROOT, "asm", `${target.stem}.s`), { force: true });
              matchedCount++;
              console.log(`matched ${target.stem} (start=${target.mismatch})`);
            }
            done = true;
            break;
          }
        }
      }
      if (!done) {
        // 到達した最良体を保存し、次回の出発点にする。
        mkdirSync(join(ROOT, "out/brute/best"), { recursive: true });
        writeFileSync(join(ROOT, "out/brute/best", `${target.stem}.c`), M2C_PREAMBLE + best.body);
        console.log(`floor ${target.stem}: ${best.mismatch} (start=${target.mismatch})`);
      }
    }
  }
  await Promise.all(Array.from({ length: options.jobs }, (_, index) => worker(index)));
  console.log(`matched=${matchedCount} of ${targets.length}`);
}

if (import.meta.main) main();
