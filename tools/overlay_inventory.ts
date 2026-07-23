#!/usr/bin/env bun
// Tool role: both; imported by tools/overlay_match.ts; invoked by PLAYBOOK.md.
// Discover ordinary Thumb functions inside Alchemy's canonical overlay sources
// and group relocation-normalized duplicates. Generated reports remain ignored
// below out/ and contain no copied executable bytes.
import { mkdirSync, readFileSync, readdirSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { Discovery, sx } from "./discover.ts";
import { assembleOverlay, overlayCAddresses, OVERLAY_BASE } from "./overlay_disasm.ts";
import { canonicalJson } from "./canonical_json.ts";

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
  returns: number;
  starts_with_prologue: boolean;
  unresolved: number;
  unresolved_sites: number[];
  jump_tables: number;
  fingerprint: string;
  seed_sources: string[];
  contained_by: string[];
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

function convertedPlaceholders(source: string): Map<number, number> {
  const placeholders = new Map<number, number>();
  const text = readFileSync(source, "utf8");
  const pattern = /^AlchemyC_([0-9a-f]{8}):[ \t]*$/gim;
  for (const match of text.matchAll(pattern)) {
    const address = Number.parseInt(match[1], 16);
    if (placeholders.has(address)) throw new Error(`duplicate overlay C placeholder: ${source}:0x${address.toString(16)}`);
    let size = 0;
    const tail = text.slice(match.index! + match[0].length);
    for (const raw of tail.split(/\r?\n/).slice(1)) {
      const line = raw.trim();
      if (line === "" || /^\.L_[0-9a-z_.$]+:$/i.test(line)) continue;
      const space = /^\.space\s+(0x[0-9a-f]+|[0-9]+)\b/i.exec(line);
      if (space === null) break;
      size += Number.parseInt(space[1], 0);
    }
    if (size === 0) throw new Error(`overlay C placeholder has no space: ${source}:0x${address.toString(16)}`);
    placeholders.set(address, size);
  }
  return placeholders;
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
  let convertedFunctions = 0, convertedInstructionBytes = 0, convertedSpanBytes = 0;
  let convertedInternalEntries = 0, undiscoveredConvertedFunctions = 0;
  for (const overlay of overlays) {
    const data = assembleOverlay(overlay.source, OVERLAY_BASE);
    const converted = overlayCAddresses(overlay.source);
    const placeholders = convertedPlaceholders(overlay.source);
    const missing = [...converted].filter((address) => !placeholders.has(address));
    const orphaned = [...placeholders.keys()].filter((address) => !converted.has(address));
    if (missing.length > 0 || orphaned.length > 0) {
      const detail = [
        missing.length > 0 ? `missing placeholders=${missing.map((address) => `0x${address.toString(16)}`).join(",")}` : "",
        orphaned.length > 0 ? `orphaned placeholders=${orphaned.map((address) => `0x${address.toString(16)}`).join(",")}` : "",
      ].filter(Boolean).join(" ");
      throw new Error(`overlay C/source mismatch: ${overlay.name}: ${detail}`);
    }
    convertedFunctions += converted.size;
    convertedSpanBytes += [...placeholders.values()].reduce((sum, size) => sum + size, 0);
    const convertedSpans = [...placeholders];
    decodedBytes += data.length;
    const discovery = discoverOverlay(data);
    instructionBytes += [...discovery.instructions.values()].reduce((sum, item) => sum + item.size, 0);
    convertedInstructionBytes += [...discovery.instructions].filter(([address]) =>
      convertedSpans.some(([start, size]) => start <= address && address < start + size))
      .reduce((sum, [, item]) => sum + item.size, 0);
    unresolved += discovery.unresolved.size;
    jumpTables += discovery.jump_tables.size;
    const discoveredConverted = new Set<number>();
    for (const [entry, fn] of [...discovery.functions].sort((a, b) => a[0] - b[0])) {
      const addresses = [...fn.instructions].sort((a, b) => a - b);
      if (addresses.length === 0 || addresses[0] !== entry) continue;
      const convertedOwner = convertedSpans.find(([start, size]) => start <= entry && entry < start + size)?.[0];
      if (convertedOwner !== undefined) {
        if (entry === convertedOwner) discoveredConverted.add(entry);
        else convertedInternalEntries++;
        continue;
      }
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
        returns: addresses.filter((address) => discovery.instructions.get(address)!.kind === "return").length,
        starts_with_prologue: (discovery.u16(entry) & 0xff00) === 0xb500,
        unresolved: fn.unresolved.size,
        unresolved_sites: [...fn.unresolved].sort((a, b) => a - b).map((site) => site - OVERLAY_BASE),
        jump_tables: [...discovery.jump_table_sites.keys()].filter((site) => fn.instructions.has(site)).length,
        fingerprint: fingerprint(discovery, addresses),
        seed_sources: [...fn.sources].sort(),
        contained_by: [],
      });
    }
    undiscoveredConvertedFunctions += [...converted].filter((address) => !discoveredConverted.has(address)).length;
  }
  const instructionOwners = new Map<string, Map<number, string[]>>();
  for (const fn of functions) {
    if (!instructionOwners.has(fn.overlay)) instructionOwners.set(fn.overlay, new Map());
    const owners = instructionOwners.get(fn.overlay)!;
    for (const offset of fn.instruction_offsets) {
      const address = fn.entry + offset;
      if (!owners.has(address)) owners.set(address, []);
      owners.get(address)!.push(fn.id);
    }
  }
  for (const fn of functions) {
    fn.contained_by = (instructionOwners.get(fn.overlay)!.get(fn.entry) ?? [])
      .filter((id) => id !== fn.id)
      .sort();
  }
  const ordinary = functions.filter((fn) => fn.unresolved === 0 && fn.jump_tables === 0 &&
    fn.code_bytes >= 8 && fn.span_bytes - fn.code_bytes <= 64);
  const tiny = functions.filter((fn) => fn.code_bytes <= 6);
  const contained = functions.filter((fn) => fn.contained_by.length > 0);
  const returning = functions.filter((fn) => fn.returns > 0);
  const ordinaryPrologueReturn = ordinary.filter((fn) =>
    fn.contained_by.length === 0 && fn.starts_with_prologue && fn.returns > 0);
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
      converted_functions: convertedFunctions,
      unconverted_discoveries: functions.length,
      tiny_unconverted_discoveries: tiny.length,
      contained_unconverted_discoveries: contained.length,
      returning_unconverted_discoveries: returning.length,
      ordinary_prologue_return_discoveries: ordinaryPrologueReturn.length,
      converted_internal_entries: convertedInternalEntries,
      undiscovered_converted_functions: undiscoveredConvertedFunctions,
      functions: functions.length,
      ordinary_unconverted_discoveries: ordinary.length,
      ordinary_functions: ordinary.length,
      instruction_bytes: instructionBytes,
      converted_instruction_bytes: convertedInstructionBytes,
      converted_span_bytes: convertedSpanBytes,
      unresolved,
      jump_tables: jumpTables,
      duplicate_families: families.filter((family) => family.count > 1).length,
      duplicate_functions: families.filter((family) => family.count > 1).reduce((sum, family) => sum + family.count, 0),
    },
    families,
    functions,
  };
  mkdirSync(dirname(options.output), { recursive: true });
  writeFileSync(options.output, canonicalJson(report) + "\n");
  console.log(`overlays=${report.totals.overlays} converted_functions=${convertedFunctions} unconverted_discoveries=${functions.length} ordinary_unconverted_discoveries=${ordinary.length} tiny_unconverted_discoveries=${tiny.length} contained_unconverted_discoveries=${contained.length} returning_unconverted_discoveries=${returning.length} ordinary_prologue_return_discoveries=${ordinaryPrologueReturn.length}`);
  console.log(`decoded_bytes=${decodedBytes} instruction_bytes=${instructionBytes} converted_instruction_bytes=${convertedInstructionBytes} converted_span_bytes=${convertedSpanBytes} converted_internal_entries=${convertedInternalEntries} undiscovered_converted_functions=${undiscoveredConvertedFunctions} unresolved=${unresolved} jump_tables=${jumpTables}`);
  console.log(`duplicate_families=${report.totals.duplicate_families} duplicate_functions=${report.totals.duplicate_functions}`);
  for (const family of families.filter((item) => item.count > 1).slice(0, options.top)) {
    const sample = family.members.slice(0, 12).join(",");
    console.log(`${family.fingerprint}\tcount=${family.count}\toverlays=${family.overlays}\tbytes=${family.code_bytes}\t${sample}${family.count > 12 ? ",..." : ""}`);
  }
  console.log(`report=${options.output}`);
}

if (import.meta.main) main();
