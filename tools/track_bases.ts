#!/usr/bin/env bun
// 基底追跡(第二段)。全再構築アセンブリでレジスタの出自を流し、即値変位
// 付きロード・ストアを基底ごとに集計する。基底は絶対番地(プール定数)と
// 関数引数(arg:関数:番号)。呼出し時のレジスタ出自から引数基底同士を
// 合一(union-find)し、呼び手を跨いだ共有構造体を推定する。
import { readFileSync, readdirSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const WIDTHS: Record<string, number> = { ldr: 4, str: 4, ldrh: 2, strh: 2, ldrsh: 2, ldrb: 1, strb: 1, ldsb: 1 };

const parent = new Map<string, string>();
function find(tag: string): string {
  let root = tag;
  while (parent.has(root)) root = parent.get(root)!;
  return root;
}
function union(a: string, b: string): void {
  const ra = find(a), rb = find(b);
  if (ra !== rb) parent.set(ra > rb ? ra : rb, ra > rb ? rb : ra);
}

interface Access { offset: number; width: number; store: boolean }
const accesses = new Map<string, Access[]>();
function record(base: string, offset: number, width: number, store: boolean): void {
  if (!accesses.has(base)) accesses.set(base, []);
  accesses.get(base)!.push({ offset, width, store });
}

function main(): void {
  const files = readdirSync(join(ROOT, "asm")).filter((name) => name.endsWith(".s"));
  for (const name of files) {
    const stem = name.replace(".s", "");
    const text = readFileSync(join(ROOT, "asm", name), "utf8");
    const lines = text.split("\n");
    // プール定数の収集
    const pools: string[] = [];
    for (const line of lines) {
      const pool = line.match(/\.4byte\s+(0x0[23][0-9a-f]{6})\b/);
      if (pool) pools.push(pool[1]);
    }
    let poolCursor = 0;
    // レジスタ出自。関数入口で r0..r3 = 引数。
    const origin = new Map<string, string>();
    for (let index = 0; index < 4; index++) origin.set(`r${index}`, `arg:${stem}:${index}`);
    for (const raw of lines) {
      const line = raw.trim();
      const m = line.match(/^(ldrs?h|ldrs?b|ldsb|ldrb?|strh?|strb?)(?:\.n|\.w)?\s+(r\d+|sl|fp|ip),\s*\[(r\d+)(?:,\s*#(\d+))?\]/);
      if (m) {
        const [, op, dst, baseReg, imm] = m;
        const base = origin.get(baseReg);
        const width = WIDTHS[op.replace(/s(?=[hb])/, "")] ?? WIDTHS[op] ?? 4;
        if (base !== undefined) {
          record(base, imm ? Number(imm) : 0, width, op.startsWith("st"));
          if (op === "ldr" && !op.startsWith("st")) origin.set(dst, `${base}+${imm ?? 0}`);
          else if (!op.startsWith("st")) origin.delete(dst);
        } else if (!op.startsWith("st")) origin.delete(dst);
        continue;
      }
      const pc = line.match(/^ldr\s+(r\d+),\s*\[pc/);
      if (pc) {
        const value = pools[poolCursor] !== undefined ? pools[poolCursor] : null;
        poolCursor++;
        if (value !== null) origin.set(pc[1], `abs:${value}`);
        else origin.delete(pc[1]);
        continue;
      }
      const mov = line.match(/^(?:adds?|movs?)\s+(r\d+),\s*(r\d+)(?:,\s*#0)?$/);
      if (mov) {
        const src = origin.get(mov[2]);
        if (src !== undefined) origin.set(mov[1], src); else origin.delete(mov[1]);
        continue;
      }
      const call = line.match(/^bl\s+Func_(08[0-9a-f]{6})/);
      if (call) {
        // 呼出し規約: r0..r3 の出自を被呼側引数へ合一する。
        for (let index = 0; index < 4; index++) {
          const src = origin.get(`r${index}`);
          if (src !== undefined && !src.includes("+")) union(src, `arg:${call[1]}:${index}`);
        }
        for (let index = 0; index < 4; index++) origin.delete(`r${index}`);
        continue;
      }
      if (/^(b|bx|pop|push|cmp|beq|bne|bgt|blt|bge|ble|bhi|bls|bcc|bcs)\b/.test(line)) continue;
      const clobber = line.match(/^\S+\s+(r\d+)/);
      if (clobber) origin.delete(clobber[1]);
    }
  }
  // 合一後の基底へ集計を畳む
  const merged = new Map<string, Map<number, { width: number; count: number; stores: number }>>();
  for (const [tag, list] of accesses) {
    const root = find(tag);
    if (!merged.has(root)) merged.set(root, new Map());
    const fields = merged.get(root)!;
    for (const access of list) {
      const previous = fields.get(access.offset);
      if (previous === undefined) fields.set(access.offset, { width: access.width, count: 1, stores: access.store ? 1 : 0 });
      else { previous.count++; if (access.store) previous.stores++; if (access.width > previous.width) previous.width = access.width; }
    }
  }
  const rows = [...merged.entries()]
    .map(([base, fields]) => ({ base, fields: [...fields.entries()].sort((a, b) => a[0] - b[0]).map(([offset, f]) => ({ offset, ...f })), total: [...fields.values()].reduce((s, f) => s + f.count, 0) }))
    .filter((row) => row.fields.length >= 4)
    .sort((a, b) => b.total - a.total);
  writeFileSync(join(ROOT, "work/base_structs.json"), JSON.stringify({ format: 1, structs: rows }, null, 1));
  console.log(`bases=${merged.size} strong=${rows.length} top=${rows.slice(0, 5).map((r) => `${r.base}(${r.fields.length}f/${r.total}a)`).join(" ")}`);
}

main();
