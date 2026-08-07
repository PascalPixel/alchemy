#!/usr/bin/env bun
// Classify every remaining c_candidate region by what actually blocks it, so the
// work left is a costed list rather than a count.
//
// Why this exists: "808 regions remain" is not a plan, and the two family sizes
// published on 2026-07-26 were both wrong because they matched on whole-file
// features instead of resolving operands. Every bucket here is decided from the
// region's own decoded instruction stream, and pc-relative loads are resolved
// against the constant pool before anything is counted.
import { readFileSync, existsSync } from "node:fs";
import { basename, dirname, join } from "node:path";

const ROOT = dirname(dirname(dirname(Bun.fileURLToPath(import.meta.url))));

export interface Decoded {
  insns: Array<{ off: number; text: string }>;
  pool: Map<number, number>;
}

// Thumb is fixed 2-byte except bl; pool words follow the code, 4-aligned.
export function decode(text: string): Decoded {
  const insns: Array<{ off: number; text: string }> = [];
  const pool = new Map<number, number>();
  let off = 0;
  for (const raw of text.split("\n")) {
    const line = raw.replace(/@.*$/, "").trimEnd();
    const word = /^\t\.4byte\s+(0x[0-9a-f]+)/.exec(line);
    if (word !== null) {
      if (off % 4) off += 2;
      pool.set(off, Number(word[1]));
      off += 4;
      continue;
    }
    if (!/^\t[a-z]/.test(line)) continue;
    insns.push({ off, text: line.trim() });
    off += /^\tbl\t/.test(line) ? 4 : 2;
  }
  return { insns, pool };
}

// A pc-relative load names a pool slot, not an immediate; without resolving it
// no statement about "the region uses constant X" means anything.
export function poolValue(decoded: Decoded, index: number): number | undefined {
  const insn = decoded.insns[index];
  const load = /^ldr\t(r\d), \[pc, #(\d+)\]/.exec(insn.text);
  if (load === null) return undefined;
  return decoded.pool.get((((insn.off + 4) & ~3) + Number(load[2])));
}

export function maskRegisters(decoded: Decoded, constant: number): boolean {
  const holds = new Set<string>();
  for (let index = 0; index < decoded.insns.length; index++) {
    const text = decoded.insns[index].text;
    const and = /^ands\t(r\d), (r\d)$/.exec(text);
    if (and !== null && holds.has(and[2]) && and[1] !== and[2]) return true;
    const load = /^ldr\t(r\d), \[pc, #\d+\]/.exec(text);
    if (load !== null) {
      if (poolValue(decoded, index) === constant) holds.add(load[1]);
      else holds.delete(load[1]);
      continue;
    }
    const written = /^[a-z]+\t(r\d),/.exec(text);
    if (written !== null) holds.delete(written[1]);
  }
  return false;
}

interface Bucket { name: string; note: string; stems: string[] }

function main(): void {
  const manifest = JSON.parse(readFileSync(join(ROOT, "out/full/asm/manifest.json"), "utf8"));
  const regions = manifest.regions.filter((r: any) => r.retention === "c_candidate");

  const buckets: Bucket[] = [
    { name: "not emittable: return address in ip", note: "neither approved compiler emits `mov ip, pc`; needs a retention class or a mode", stems: [] },
    { name: "twelve-store record group", note: "blocked twice over: -mgrouped-dma-store is needed to reach the four-word transform but its three-word grouper eats the run first, and the transform wants regs_ever_live[0] clear", stems: [] },
    { name: "DMA descriptor with completion poll", note: "shape solved 2026-07-26; one converted (08005a78) with -fthumb-split-group-base and -fthumb-group-control-last", stems: [] },
    { name: "DMA descriptor, no poll", note: "grouped-store laws apply; the usual residual is scheduling around the descriptor", stems: [] },
    { name: "0xffff used as an AND mask", note: "u32 locals reproduce it; 8 of these mask twice and need a combine our GCC performs", stems: [] },
    { name: "bitfield insert", note: "an ordinary C bitfield reproduces ldrh/ands/orrs/strh; only nine converted sources use one", stems: [] },
    { name: "plain", note: "no identified construct blocker; cost is drafting time and the usual allocation residuals", stems: [] },
  ];

  for (const region of regions) {
    if (!existsSync(region.source)) continue;
    const text = readFileSync(region.source, "utf8");
    const stem = basename(region.source, ".s");
    const decoded = decode(text);
    const body = decoded.insns.map((i) => i.text).join("\n");
    let index: number;
    if (/^mov\tip, pc$/m.test(body)) index = 0;
    else if (/stmia\tr\d!, \{r1, r2, r3, r4\}/.test(body)) index = 1;
    else if (/stmia\tr\d!, \{r0, r1, r2\}/.test(body)) index = /ldr\tr\d, \[r\d, #8\]\n\tands/.test(body) ? 2 : 3;
    else if (maskRegisters(decoded, 0xffff)) index = 4;
    else if (/\torrs\t/.test(body) && /\tands\t/.test(body) && /str[hb]?\t/.test(body)) index = 5;
    else index = 6;
    buckets[index].stems.push(stem);
  }

  const total = buckets.reduce((sum, b) => sum + b.stems.length, 0);
  console.log(`${total} c_candidate regions remain\n`);
  for (const bucket of buckets) {
    if (bucket.stems.length === 0) continue;
    console.log(`${String(bucket.stems.length).padStart(4)}  ${bucket.name}`);
    console.log(`      ${bucket.note}`);
  }
}

if (import.meta.main) main();
