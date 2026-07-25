#!/usr/bin/env bun
// Tool role: entrypoint; invoked by PLAYBOOK.md.
// Group the overlay functions that are neither converted nor inventoried by
// their normalized instruction shape, so one hand-derived template can be
// expanded across a whole cohort by tools/overlay_cohort.ts.
//
// Why this exists rather than the inventory: discovery seeds from control flow
// inside the stream, so a function whose only callers live in the main image or
// in an external pointer table is never inventoried at all (LAWS.md, "Overlay
// discovery misses externally-called functions"). Those entries are invisible
// to overlay_match.ts, and they are where the remaining cohorts live.
//
// Reports shapes, never bytes: the grouping key is a masked opcode sequence and
// the output names addresses, so nothing here is a ROM excerpt.
import { mkdirSync, readdirSync, readFileSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { assembleOverlay, overlayCAddresses, OVERLAY_BASE } from "./overlay_disasm.ts";
import { discoverOverlay } from "./overlay_inventory.ts";
import { canonicalJson } from "./canonical_json.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
// Thumb `push {..., lr}`. Every compiler function entry in these streams starts
// with one, and every entry is word aligned; a data word that merely reads as
// `b5xx` is aligned only by chance, so the scan steps by 4.
const PROLOGUE_MASK = 0xff00;
const PROLOGUE = 0xb500;
// ARMv4T reserves 0xe800-0xffff for the two halves of a 4-byte BL, so a
// halfword in that range means the walk is reading data, not code.
const BL_RESERVED = 0xe800;
const RETURN_MASK = 0xff87;
const RETURN = 0x4700;
// A pc-relative load's offset says where its pool word sits, which is a
// per-instance detail; masking it lets two instances of one function shape
// group together even though their constants differ.
const PC_LOAD_MASK = 0xf800;
const PC_LOAD = 0x4800;

interface Found { overlay: string; offset: number; codeBytes: number }
interface Options { assets: string; output: string; top: number; minimum: number }

function optionsOf(argv: string[]): Options {
  const options: Options = {
    assets: join(ROOT, "assets/code"),
    output: join(ROOT, "out/decomp/overlay-shapes.json"),
    top: 10,
    minimum: 2,
  };
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--assets") options.assets = argv[++index];
    else if (argument === "--output" || argument === "-o") options.output = argv[++index];
    else if (argument === "--top") options.top = Number(argv[++index]);
    else if (argument === "--min") options.minimum = Number(argv[++index]);
    else if (argument === "-h" || argument === "--help") {
      console.log("usage: overlay_shapes.ts [--assets DIR] [-o OUTPUT] [--top N] [--min N]");
      process.exit(0);
    } else throw new Error(`unrecognized argument: ${argument}`);
  }
  for (const value of [options.top, options.minimum]) {
    if (!Number.isInteger(value) || value < 1) throw new Error("--top and --min must be positive integers");
  }
  return options;
}

// 未確定プロローグから戻りまでを復号し、pc相対ロードの変位を伏せた形を返す。
// 復号できない(BL予約語に当たる、戻りに届かない)ときは null。
export function shapeAt(data: Buffer, offset: number, limit = 160): { key: string; codeBytes: number } | null {
  const words: string[] = [];
  for (let at = offset; at + 2 <= Math.min(offset + limit, data.length); at += 2) {
    const word = data.readUInt16LE(at);
    if (word >= BL_RESERVED) return null;
    words.push(((word & PC_LOAD_MASK) === PC_LOAD ? PC_LOAD : word).toString(16));
    if ((word & RETURN_MASK) === RETURN) {
      // 6語未満は断片。関数として扱わない。
      return words.length < 6 ? null : { key: words.join(","), codeBytes: at + 2 - offset };
    }
  }
  return null;
}

function scan(assets: string): Map<string, Found[]> {
  const groups = new Map<string, Found[]>();
  for (const name of readdirSync(assets).filter((entry) => entry.endsWith("_overlay.s")).sort()) {
    const overlay = name.replace("_overlay.s", "");
    let data: Buffer;
    try {
      data = assembleOverlay(join(assets, name), OVERLAY_BASE);
    } catch {
      continue;
    }
    const claimed = new Set<number>();
    try {
      for (const fn of discoverOverlay(data).functions.values()) {
        for (const address of fn.instructions) claimed.add(address);
      }
    } catch {}
    let converted: number[] = [];
    try {
      converted = [...overlayCAddresses(join(assets, name))];
    } catch {}
    const convertedSet = new Set(converted);

    for (let offset = 0; offset + 2 <= data.length; offset += 4) {
      const address = OVERLAY_BASE + offset;
      if (claimed.has(address) || convertedSet.has(address)) continue;
      if ((data.readUInt16LE(offset) & PROLOGUE_MASK) !== PROLOGUE) continue;
      // 変換済み領域の直前にある語が偶然プロローグに見えることがある。復号が
      // その置換子へ踏み込む場合は数えない。
      if (converted.some((at) => at > address && at < address + 24)) continue;
      const shape = shapeAt(data, offset);
      if (shape === null) continue;
      if (!groups.has(shape.key)) groups.set(shape.key, []);
      groups.get(shape.key)!.push({ overlay, offset, codeBytes: shape.codeBytes });
    }
  }
  return groups;
}

function selfTest(): void {
  // push {lr} / movs / ldr pc-rel / cmp / bne / ldr pc-rel / pop / bx
  const make = (poolOffset: number): Buffer => {
    const halves = [0xb500, 0x2000, 0x4800 | poolOffset, 0x2b00, 0xd100, 0x4800 | poolOffset, 0xbc02, 0x4708];
    const buffer = Buffer.alloc(halves.length * 2);
    halves.forEach((half, index) => buffer.writeUInt16LE(half, index * 2));
    return buffer;
  };
  const first = shapeAt(make(3), 0);
  const second = shapeAt(make(9), 0);
  if (first === null || second === null) throw new Error("overlay shape self-test failed to decode");
  if (first.key !== second.key) throw new Error("overlay shape self-test: pool offsets must not split a shape");
  if (first.codeBytes !== 16) throw new Error("overlay shape self-test: wrong code extent");
  // BL予約語に当たる復号は関数として採らない。
  const data = Buffer.alloc(8);
  data.writeUInt16LE(0xb500, 0);
  data.writeUInt16LE(0xf000, 2);
  if (shapeAt(data, 0) !== null) throw new Error("overlay shape self-test: BL half must reject the walk");
  console.log("overlay shape self-test passed");
}

function main(): void {
  const argv = Bun.argv.slice(2);
  if (argv.includes("--self-test")) {
    selfTest();
    return;
  }
  const options = optionsOf(argv);
  const groups = scan(options.assets);
  const singles = [...groups.values()].flat();
  const cohorts = [...groups.entries()]
    .filter(([, members]) => members.length >= options.minimum)
    .sort((left, right) => right[1].length - left[1].length || left[1][0].codeBytes - right[1][0].codeBytes);

  mkdirSync(dirname(options.output), { recursive: true });
  writeFileSync(options.output, canonicalJson({
    format: 1,
    unclaimed: singles.length,
    distinct_shapes: groups.size,
    cohorts: cohorts.map(([key, members]) => ({ key, count: members.length, code_bytes: members[0].codeBytes, members })),
    all: singles.sort((left, right) => left.codeBytes - right.codeBytes),
  }) + "\n");

  console.log(`unclaimed=${singles.length} distinct_shapes=${groups.size} cohorts=${cohorts.length} report=${options.output}`);
  for (const [, members] of cohorts.slice(0, options.top)) {
    const listed = members.slice(0, 4).map((m) => `${m.overlay}:${m.offset.toString(16).padStart(4, "0")}`).join(" ");
    console.log(`  count=${members.length} code_bytes=${members[0].codeBytes} :: ${listed}${members.length > 4 ? " ..." : ""}`);
  }
}

if (import.meta.main) main();
