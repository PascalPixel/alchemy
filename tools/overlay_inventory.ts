#!/usr/bin/env bun
// Tool role: both; imported by tools/overlay_match.ts; invoked by PLAYBOOK.md.
// Discover ordinary Thumb functions inside Alchemy's canonical overlay sources
// and group relocation-normalized duplicates. Generated reports remain ignored
// below out/ and contain no copied executable bytes.
import { mkdirSync, readFileSync, readdirSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { Discovery, sx } from "./discover.ts";
import { assembleOverlay, OVERLAY_BASE } from "./overlay_disasm.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

interface Options { assets: string; output: string; top: number }
interface FunctionRow {
  id: string;
  overlay: string;
  entry: number;
  offset: number;
  instruction_count: number;
  instruction_offsets: number[];
  code_bytes: number;
  span_bytes: number;
  calls: number;
  unresolved: number;
  unresolved_sites: number[];
  jump_tables: number;
  fingerprint: string;
}

function optionsOf(argv: string[]): Options {
  const options: Options = {
    assets: join(ROOT, "assets/code"),
    output: join(ROOT, "out/decomp/overlays.json"),
    top: 20,
  };
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--assets") options.assets = argv[++index];
    else if (argument === "--output" || argument === "-o") options.output = argv[++index];
    else if (argument === "--top") options.top = Number(argv[++index]);
    else if (argument === "-h" || argument === "--help") {
      console.log("usage: overlay_inventory.ts [--assets DIR] [-o OUTPUT] [--top N]");
      process.exit(0);
    } else throw new Error(`unrecognized argument: ${argument}`);
  }
  if (!Number.isInteger(options.top) || options.top < 1) throw new Error("--top must be a positive integer");
  return options;
}

export function discoverOverlay(data: Buffer): Discovery {
  const discovery = new Discovery(data, OVERLAY_BASE);
  // Overlay headers are fixed veneers, not ordinary C functions. The first
  // compiler prologue after that header is the reliable control-flow root.
  for (let offset = 0; offset + 2 <= Math.min(data.length, 0x400); offset += 2) {
    if ((data.readUInt16LE(offset) & 0xff00) !== 0xb500) continue;
    discovery.add_seed(OVERLAY_BASE + offset, "thumb", "first-prologue");
    break;
  }
  for (let offset = 0; offset + 4 <= data.length; offset += 4) {
    const value = data.readUInt32LE(offset);
    const target = value & ~1;
    if ((value & 1) && discovery.inside(target, 2)) {
      const first = discovery.u16(target);
      if ((first & 0xff00) === 0xb500 || first === 0x4770 || (first & 0xf800) === 0x2000) {
        discovery.add_seed(target, "thumb", "local-function-pointer");
      }
    }
  }
  const walked = new Set<number>();
  while (true) {
    const pending = [...discovery.functions.keys()].filter((entry) => !walked.has(entry)).sort((a, b) => a - b);
    if (pending.length === 0) {
      // Recover adjacent, uncalled functions only when a verified function
      // ends in a return and the next aligned halfword is a nearby prologue.
      let added = false;
      for (const fn of discovery.functions.values()) {
        const addresses = [...fn.instructions].sort((a, b) => a - b);
        if (addresses.length === 0) continue;
        const last = addresses.at(-1)!;
        if (discovery.instructions.get(last)?.kind !== "return") continue;
        let candidate = last + discovery.instructions.get(last)!.size;
        for (let skipped = 0; skipped <= 8 && discovery.inside(candidate, 2); skipped += 2, candidate += 2) {
          const half = discovery.u16(candidate);
          if ((half & 0xff00) === 0xb500) {
            added = discovery.add_seed(candidate, "thumb", `after-return:${last.toString(16)}`) || added;
            break;
          }
          if (![0, 0x46c0].includes(half)) break;
        }
      }
      if (!added) break;
      continue;
    }
    for (const entry of pending) {
      try { discovery.walk_function(entry); } catch {}
      walked.add(entry);
    }
  }
  for (const site of discovery.jump_table_sites.keys()) discovery.unresolved.delete(site);
  for (const fn of discovery.functions.values()) {
    for (const site of discovery.jump_table_sites.keys()) fn.unresolved.delete(site);
  }
  return discovery;
}

function targetClass(discovery: Discovery, value: number): string {
  const address = value & ~1;
  if (discovery.inside(address, 2)) return "overlay";
  const bank = value >>> 24;
  if (bank === 0x08 || bank === 0x09) return "rom";
  if (bank === 0x02 || bank === 0x03) return "ram";
  if (bank === 0x04) return "io";
  return `constant:${value <= 0xffff ? value.toString(16) : "wide"}`;
}

function fingerprint(discovery: Discovery, addresses: number[]): string {
  const tokens: string[] = [];
  for (const address of addresses) {
    const instruction = discovery.instructions.get(address)!;
    const half = discovery.u16(address);
    if (instruction.size === 4 && (half & 0xf800) === 0xf000) {
      const low = discovery.u16(address + 2);
      const offset = sx(((half & 0x7ff) << 12) | ((low & 0x7ff) << 1), 23);
      tokens.push(`bl:${targetClass(discovery, address + 4 + offset)}`);
    } else if ((half & 0xf800) === 0xe000) {
      tokens.push(`b:${half >>> 11}`);
    } else if ((half & 0xf000) === 0xd000 && ((half >>> 8) & 0xf) < 0xe) {
      tokens.push(`bc:${(half >>> 8) & 0xf}`);
    } else if ((half & 0xf800) === 0x4800) {
      const literal = ((address + 4) & ~3) + ((half & 0xff) << 2);
      const kind = discovery.inside(literal, 4) ? targetClass(discovery, discovery.u32(literal)) : "external";
      tokens.push(`ldr-lit:r${(half >>> 8) & 7}:${kind}`);
    } else if ((half & 0xf800) === 0xa000) {
      tokens.push(`adr:r${(half >>> 8) & 7}`);
    } else {
      tokens.push(half.toString(16).padStart(4, "0"));
    }
  }
  return Bun.hash(tokens.join(" ")).toString(36);
}

function main(): void {
  const options = optionsOf(Bun.argv.slice(2));
  const overlays = readdirSync(options.assets)
    .filter((name) => name.startsWith("resource_"))
    .sort()
    .map((name) => ({ name, source: join(options.assets, name, "overlay.s") }))
    .filter((item) => Bun.file(item.source).size > 0);
  const functions: FunctionRow[] = [];
  let decodedBytes = 0, instructionBytes = 0, unresolved = 0, jumpTables = 0;
  for (const overlay of overlays) {
    const data = assembleOverlay(overlay.source, OVERLAY_BASE);
    decodedBytes += data.length;
    const discovery = discoverOverlay(data);
    instructionBytes += [...discovery.instructions.values()].reduce((sum, item) => sum + item.size, 0);
    unresolved += discovery.unresolved.size;
    jumpTables += discovery.jump_tables.size;
    for (const [entry, fn] of [...discovery.functions].sort((a, b) => a[0] - b[0])) {
      const addresses = [...fn.instructions].sort((a, b) => a - b);
      if (addresses.length === 0 || addresses[0] !== entry) continue;
      let end = Math.max(...addresses.map((address) => address + discovery.instructions.get(address)!.size));
      for (const address of addresses) {
        const half = discovery.u16(address);
        if ((half & 0xf800) !== 0x4800) continue;
        const literal = ((address + 4) & ~3) + ((half & 0xff) << 2);
        if (discovery.inside(literal, 4)) end = Math.max(end, literal + 4);
      }
      const offset = entry - OVERLAY_BASE;
      functions.push({
        id: `${overlay.name}:${offset.toString(16).padStart(4, "0")}`,
        overlay: overlay.name,
        entry,
        offset,
        instruction_count: addresses.length,
        instruction_offsets: addresses.map((address) => address - entry),
        code_bytes: addresses.reduce((sum, address) => sum + discovery.instructions.get(address)!.size, 0),
        span_bytes: end - entry,
        calls: fn.callees.size + fn.external_callees.size,
        unresolved: fn.unresolved.size,
        unresolved_sites: [...fn.unresolved].sort((a, b) => a - b).map((site) => site - OVERLAY_BASE),
        jump_tables: [...discovery.jump_table_sites.keys()].filter((site) => fn.instructions.has(site)).length,
        fingerprint: fingerprint(discovery, addresses),
      });
    }
  }
  const ordinary = functions.filter((fn) => fn.unresolved === 0 && fn.jump_tables === 0 &&
    fn.code_bytes >= 8 && fn.span_bytes - fn.code_bytes <= 64);
  const groups = new Map<string, FunctionRow[]>();
  for (const fn of ordinary) {
    if (!groups.has(fn.fingerprint)) groups.set(fn.fingerprint, []);
    groups.get(fn.fingerprint)!.push(fn);
  }
  const families = [...groups.entries()].map(([fingerprint, members]) => ({
    fingerprint,
    count: members.length,
    overlays: new Set(members.map((item) => item.overlay)).size,
    code_bytes: members.reduce((sum, item) => sum + item.code_bytes, 0),
    members: members.map((item) => item.id),
  })).sort((left, right) => right.count - left.count || right.code_bytes - left.code_bytes);
  const report = {
    format: 1,
    generated_at: new Date().toISOString(),
    totals: {
      overlays: overlays.length,
      decoded_bytes: decodedBytes,
      functions: functions.length,
      ordinary_functions: ordinary.length,
      instruction_bytes: instructionBytes,
      unresolved,
      jump_tables: jumpTables,
      duplicate_families: families.filter((family) => family.count > 1).length,
      duplicate_functions: families.filter((family) => family.count > 1).reduce((sum, family) => sum + family.count, 0),
    },
    families,
    functions,
  };
  mkdirSync(dirname(options.output), { recursive: true });
  writeFileSync(options.output, JSON.stringify(report, null, 2) + "\n");
  console.log(`overlays=${report.totals.overlays} functions=${report.totals.functions} ordinary=${report.totals.ordinary_functions} decoded_bytes=${decodedBytes} instruction_bytes=${instructionBytes} unresolved=${unresolved} jump_tables=${jumpTables}`);
  console.log(`duplicate_families=${report.totals.duplicate_families} duplicate_functions=${report.totals.duplicate_functions}`);
  for (const family of families.filter((item) => item.count > 1).slice(0, options.top)) {
    const sample = family.members.slice(0, 12).join(",");
    console.log(`${family.fingerprint}\tcount=${family.count}\toverlays=${family.overlays}\tbytes=${family.code_bytes}\t${sample}${family.count > 12 ? ",..." : ""}`);
  }
  console.log(`report=${options.output}`);
}

if (import.meta.main) main();
