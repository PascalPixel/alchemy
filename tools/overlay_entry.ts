#!/usr/bin/env bun
// Tool role: entrypoint; invoked by PLAYBOOK.md.
// Recover the true entry of an overlay function that discovery seeded partway
// into its body, by walking back to the previous function's return.
//
// Discovery seeds from tagged pointers and from control flow, so a function
// reached only through a pointer into its middle -- or one whose start was
// never reached at all -- is inventoried at the wrong address. The reported
// span then begins mid-body, and everything downstream degrades: the candidate
// misses, and a template match against the exact-C corpus cannot fire at all
// because the fragment is not the whole function.
//
// That last effect is the expensive one. resource_3c8 was inventoried at 00f6,
// twenty-two bytes into an accumulator whose true entry is 00e0; at the correct
// entry it is byte-identical to the already-converted src/0809a44c.c and
// converts by retargeting that source. Fixing the boundary is what makes the
// twin visible, so boundary recovery runs before template matching, not beside
// it.
//
// Reports addresses and decoded instruction kinds, never a ROM span.
import { readFileSync, readdirSync } from "node:fs";
import { dirname, join } from "node:path";
import { assembleOverlay, OVERLAY_BASE } from "./overlay_disasm.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
// `bx rN`. The reconstruction's epilogues end `pop {rN}` / `bx rN`, and leaf
// functions end `bx lr`; both terminate on this encoding.
const BX_MASK = 0xff87;
const BX = 0x4700;
const PROLOGUE_MASK = 0xff00;
const PROLOGUE = 0xb500;
// `mov r8, r8` (the canonical Thumb nop) and zero padding sit between a return
// and the next entry; so do the previous function's pool words, which is why
// the walk forward stops at the first halfword that is neither.
const PADDING = new Set([0x0000, 0x46c0]);

interface Recovered { entry: number; movedBy: number; startsWithPrologue: boolean }

// 与えられた位置から後方へ最も近い戻りを探し、その直後を真の入口とみなす。
// 戻りと入口の間の詰め物は読み飛ばす。見つからなければ null。
export function recoverEntry(data: Buffer, offset: number, window = 512): Recovered | null {
  for (let at = offset - 2; at >= Math.max(0, offset - window); at -= 2) {
    const half = data.readUInt16LE(at);
    if ((half & BX_MASK) !== BX) continue;
    let entry = at + 2;
    while (entry + 2 <= data.length && PADDING.has(data.readUInt16LE(entry))) entry += 2;
    // 戻りの直後が必ず次の入口とは限らない: 直前の関数のプール語が挟まる。
    // 語境界のプロローグが手前に見つかればそれを採る。見つからなければ詰め物の
    // 直後をそのまま採る -- 葉関数はプロローグを持たないので、これが正しい。
    if ((data.readUInt16LE(entry) & PROLOGUE_MASK) !== PROLOGUE) {
      for (let scan = (entry + 3) & ~3; scan < offset; scan += 4) {
        if ((data.readUInt16LE(scan) & PROLOGUE_MASK) === PROLOGUE) { entry = scan; break; }
      }
    }
    if (entry > offset) return null;
    return {
      entry,
      movedBy: offset - entry,
      startsWithPrologue: (data.readUInt16LE(entry) & PROLOGUE_MASK) === PROLOGUE,
    };
  }
  return null;
}

function selfTest(): void {
  // 前の関数の戻り / 詰め物 / 入口 / 復号できる本体 / 戻り。
  const halves = [
    0x2000, 0x4708, 0x0000,
    0xb500, 0x2001, 0x2002, 0x2003, 0x2004, 0x2005, 0x4770,
  ];
  const data = Buffer.alloc(halves.length * 2);
  halves.forEach((half, index) => data.writeUInt16LE(half, index * 2));
  // 本体の途中(0x0a)から呼んでも、詰め物を越えて 0x06 を返すこと。
  const found = recoverEntry(data, 10);
  if (found === null || found.entry !== 6) throw new Error("overlay entry self-test: wrong entry");
  if (found.movedBy !== 4 || !found.startsWithPrologue) throw new Error("overlay entry self-test: wrong metadata");
  // 戻りが手前に無ければ何も主張しない。
  if (recoverEntry(Buffer.alloc(8), 6) !== null) throw new Error("overlay entry self-test: must not invent an entry");
  console.log("overlay entry self-test passed");
}

function main(): void {
  const argv = Bun.argv.slice(2);
  if (argv.includes("--self-test")) {
    selfTest();
    return;
  }
  const all = argv.includes("--all");
  const positional = argv.filter((argument) => !argument.startsWith("--"));
  const cache = new Map<string, Buffer>();
  const load = (overlay: string): Buffer => {
    if (!cache.has(overlay)) {
      cache.set(overlay, assembleOverlay(join(ROOT, "assets/code", `${overlay}_overlay.s`), OVERLAY_BASE));
    }
    return cache.get(overlay)!;
  };

  if (!all) {
    const [overlay, offsetText] = positional;
    if (overlay === undefined || offsetText === undefined) {
      throw new Error("usage: overlay_entry.ts <overlay> <offsetHex> | --all");
    }
    const offset = Number.parseInt(offsetText, 16);
    const found = recoverEntry(load(overlay), offset);
    if (found === null) {
      console.log(`${overlay}:${offsetText} no return found within the window; entry unchanged`);
      return;
    }
    console.log(`${overlay}:${offsetText} -> ${found.entry.toString(16).padStart(4, "0")} (back ${found.movedBy} bytes, prologue=${found.startsWithPrologue})`);
    return;
  }

  const inventory = JSON.parse(readFileSync(join(ROOT, "out/decomp/overlays.json"), "utf8")) as
    { functions: Array<Record<string, any>> };
  const converted = new Set(
    readdirSync(join(ROOT, "assets/code"))
      .map((name) => /_c_([0-9a-f]{8})\.c$/.exec(name)?.[1])
      .filter((stem): stem is string => stem !== undefined)
      .map((stem) => Number.parseInt(stem, 16)),
  );
  let examined = 0, moved = 0;
  for (const fn of inventory.functions) {
    if (fn.structural_veneer || fn.data_walk || fn.code_bytes < 8) continue;
    if (converted.has(fn.entry) || (fn.contained_by ?? []).length > 0) continue;
    const data = load(fn.overlay);
    if ((data.readUInt16LE(fn.offset) & PROLOGUE_MASK) === PROLOGUE) continue;
    examined++;
    const found = recoverEntry(data, fn.offset);
    if (found === null || found.movedBy === 0) continue;
    moved++;
    console.log(`  ${fn.id} -> ${fn.overlay}:${found.entry.toString(16).padStart(4, "0")} back=${found.movedBy} prologue=${found.startsWithPrologue}`);
  }
  console.log(`examined=${examined} recovered=${moved}`);
}

if (import.meta.main) main();
