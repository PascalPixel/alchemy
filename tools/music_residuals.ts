#!/usr/bin/env bun
import {
  existsSync, lstatSync, mkdirSync, mkdtempSync, readFileSync, readdirSync, renameSync, realpathSync, rmSync, symlinkSync,
  writeFileSync,
} from "node:fs";
import { dirname, isAbsolute, join, relative, resolve } from "node:path";
import { build_sequence, type SequenceSource } from "./music_sequence.ts";

const ROM_BASE = 0x08000000;
const ROM_SIZE = 0x00800000;
const SOUND_TABLE_ADDRESS = 0x080fc684;
const YOBI_ADDRESS = 0x081819b0;
const YOBI_END = 0x081819c2;
const ALIGNMENT_ADDRESS = 0x08184698;
const ALIGNMENT_END = 0x08185000;

interface EmptyHeader {
  name: string;
  address: string;
  track_count: 0;
  block_count: number;
  priority: number;
  reverb: number;
  tone_bank: string;
}

interface OrphanStream {
  address: string;
  size: number;
  source: string;
  between: [string, string];
}

interface TailAlignment {
  address: string;
  end: string;
  boundary: number;
  fill: 0;
}

interface ResidualIndex {
  format: 1;
  kind: "golden-sun-music-residuals";
  empty_headers: EmptyHeader[];
  orphan_stream: OrphanStream;
  tail_alignment: TailAlignment;
}

export interface BuiltMusicResidual {
  address: number;
  data: Buffer;
  sources: string[];
}

const EMPTY_HEADERS: readonly EmptyHeader[] = [
  { name: "sound_019", address: "0x081652d8", track_count: 0, block_count: 0, priority: 0, reverb: 178, tone_bank: "0x080fba78" },
  { name: "sound_095", address: "0x081810b8", track_count: 0, block_count: 0, priority: 0, reverb: 178, tone_bank: "0x080fba78" },
  { name: "sound_288", address: "0x081841f8", track_count: 0, block_count: 0, priority: 120, reverb: 0, tone_bank: "0x080fc138" },
  { name: "sound_298", address: "0x08184358", track_count: 0, block_count: 0, priority: 120, reverb: 0, tone_bank: "0x080fc138" },
] as const;

const SOUND_LINKS = [
  [19, 0x081652d8],
  [95, 0x081810b8],
  [138, 0x081819a4],
  [139, 0x081819d4],
  [288, 0x081841f8],
  [298, 0x08184358],
] as const;

const YOBI_SOURCE: SequenceSource = {
  format: 1,
  engine: "smsh-sequence",
  base: "0x081819b0",
  externals: {},
  layout: [
    {
      kind: "stream",
      label: "yobi_track",
      events: [
        ["volume", 120],
        ["key_shift", 0],
        ["tempo", 30],
        ["voice", 21],
        ["note", 1, 61, 127],
        ["wait", 1],
        ["note_running", 1, 66],
        ["wait", 1],
        ["note", 44, 69],
        ["wait", 48],
        ["fine"],
      ],
    },
  ],
};

function exactKeys(value: Record<string, unknown>, keys: readonly string[], label: string): void {
  const actual = Object.keys(value).sort();
  const expected = [...keys].sort();
  if (actual.length !== expected.length || actual.some((key, index) => key !== expected[index])) {
    throw new Error(`${label} has unexpected fields`);
  }
}

function integer(value: unknown, label: string): number {
  const parsed = typeof value === "string" ? Number(value) : value;
  if (!Number.isSafeInteger(parsed)) throw new Error(`${label} must be an integer`);
  return parsed as number;
}

function byte(value: unknown, label: string): number {
  const parsed = integer(value, label);
  if (parsed < 0 || parsed > 0xff) throw new Error(`${label} does not fit u8`);
  return parsed;
}

function physicalPath(path: string): string {
  const absolute = resolve(path);
  let ancestor = absolute;
  while (!existsSync(ancestor)) {
    const parent = dirname(ancestor);
    if (parent === ancestor) return absolute;
    ancestor = parent;
  }
  return resolve(realpathSync(ancestor), relative(ancestor, absolute));
}

function same(left: string, right: string): boolean {
  return physicalPath(left) === physicalPath(right);
}

function containsPath(directory: string, path: string): boolean {
  const position = relative(physicalPath(directory), physicalPath(path));
  return position === "" || (position !== ".." && !position.startsWith("../") &&
    !position.startsWith("..\\") && !isAbsolute(position));
}

function validateExportDestination(romPath: string, directory: string): void {
  if (same(romPath, directory) || containsPath(directory, romPath)) {
    throw new Error("music residual export directory must not contain its input ROM");
  }
  const destination = resolve(directory);
  if (!existsSync(destination)) return;
  if (!lstatSync(destination).isDirectory()) throw new Error("music residual export destination must be a directory");
  if (readdirSync(destination).length === 0) return;
  const marker = join(destination, "index.json");
  if (!existsSync(marker) || !lstatSync(marker).isFile()) {
    throw new Error("refusing to replace a directory that is not a music residual package");
  }
  try { readIndex(marker); }
  catch { throw new Error("refusing to replace a directory that is not a canonical music residual package"); }
}

function child(indexPath: string, name: string): string {
  if (name !== "sound_138_yobi.json") throw new Error("music residual source name differs");
  const root = realpathSync(dirname(indexPath));
  const path = realpathSync(resolve(root, name));
  if (relative(root, path) !== name) throw new Error("music residual source escaped its directory");
  return path;
}

function jsonDocument(path: string, label: string): unknown {
  const text = readFileSync(path, "utf8");
  const value = JSON.parse(text);
  if (text !== `${JSON.stringify(value, null, 2)}\n`) throw new Error(`${label} is not canonical JSON`);
  return value;
}

function emptyHeader(header: EmptyHeader): Buffer {
  const result = Buffer.alloc(8);
  result[0] = byte(header.track_count, "track count");
  result[1] = byte(header.block_count, "block count");
  result[2] = byte(header.priority, "priority");
  result[3] = byte(header.reverb, "reverb");
  const toneBank = integer(header.tone_bank, "tone bank");
  if (toneBank < ROM_BASE || toneBank >= 0x0a000000 || toneBank % 4) throw new Error("tone bank is not an aligned ROM address");
  result.writeUInt32LE(toneBank, 4);
  return result;
}

function readIndex(indexPath: string): ResidualIndex {
  const value = jsonDocument(indexPath, "music residual index");
  if (typeof value !== "object" || value === null || Array.isArray(value)) throw new Error("music residual index must be an object");
  const index = value as ResidualIndex;
  exactKeys(index as unknown as Record<string, unknown>, ["format", "kind", "empty_headers", "orphan_stream", "tail_alignment"], "music residual index");
  if (index.format !== 1 || index.kind !== "golden-sun-music-residuals" || !Array.isArray(index.empty_headers) ||
      index.empty_headers.length !== EMPTY_HEADERS.length) {
    throw new Error("unsupported music residual index");
  }
  index.empty_headers.forEach((header, position) => {
    exactKeys(header as unknown as Record<string, unknown>, [
      "name", "address", "track_count", "block_count", "priority", "reverb", "tone_bank",
    ], "empty sound header");
    if (!Bun.deepEquals(header, EMPTY_HEADERS[position], true)) throw new Error("empty sound header differs from the audited catalog");
  });
  exactKeys(index.orphan_stream as unknown as Record<string, unknown>, ["address", "size", "source", "between"], "orphan sound stream");
  if (index.orphan_stream.address !== "0x081819b0" || index.orphan_stream.size !== 18 ||
      index.orphan_stream.source !== "sound_138_yobi.json" ||
      !Bun.deepEquals(index.orphan_stream.between, ["sound_138", "sound_139"], true)) {
    throw new Error("orphan sound stream differs from the audited layout");
  }
  exactKeys(index.tail_alignment as unknown as Record<string, unknown>, ["address", "end", "boundary", "fill"], "music tail alignment");
  if (index.tail_alignment.address !== "0x08184698" || index.tail_alignment.end !== "0x08185000" ||
      index.tail_alignment.boundary !== 0x1000 || index.tail_alignment.fill !== 0 ||
      ALIGNMENT_END % index.tail_alignment.boundary !== 0) {
    throw new Error("music tail alignment differs from the audited layout");
  }
  return index;
}

function readYobi(path: string): SequenceSource {
  const source = jsonDocument(path, "reserve sound stream") as SequenceSource;
  if (!Bun.deepEquals(source, YOBI_SOURCE, true)) throw new Error("reserve sound stream differs from the audited events");
  return source;
}

export function build_music_residuals(indexPath: string): BuiltMusicResidual[] {
  const index = readIndex(indexPath);
  const canonicalIndex = realpathSync(indexPath);
  const regions: BuiltMusicResidual[] = index.empty_headers.map((header) => ({
    address: Number(header.address), data: emptyHeader(header), sources: [canonicalIndex],
  }));
  const yobiPath = child(indexPath, index.orphan_stream.source);
  const [yobi, report] = build_sequence(readYobi(yobiPath));
  if (report.base !== Number(index.orphan_stream.address) || report.bytes !== index.orphan_stream.size ||
      report.streams !== 1 || report.tracks !== 0 || report.events !== 11) {
    throw new Error("reserve sound stream has an unexpected report");
  }
  regions.push({ address: report.base, data: yobi, sources: [canonicalIndex, yobiPath] });
  regions.push({
    address: ALIGNMENT_ADDRESS,
    data: Buffer.alloc(ALIGNMENT_END - ALIGNMENT_ADDRESS),
    sources: [canonicalIndex],
  });
  return regions.sort((left, right) => left.address - right.address);
}

function verifyRegions(rom: Buffer, regions: readonly BuiltMusicResidual[]): number {
  let bytes = 0;
  for (const region of regions) {
    const original = rom.subarray(region.address - ROM_BASE, region.address - ROM_BASE + region.data.length);
    if (!region.data.equals(original)) throw new Error(`music residual at 0x${region.address.toString(16)} differs from ROM`);
    bytes += region.data.length;
  }
  return bytes;
}

function verifySoundTableLinks(rom: Buffer): void {
  for (const [sound, header] of SOUND_LINKS) {
    const offset = SOUND_TABLE_ADDRESS - ROM_BASE + sound * 8;
    if (offset < 0 || offset + 8 > rom.length) throw new Error(`sound-table entry ${sound} lies outside ROM`);
    if (rom.readUInt32LE(offset) !== header) throw new Error(`sound-table entry ${sound} does not select its residual header`);
    if (rom.readUInt16LE(offset + 4) !== rom.readUInt16LE(offset + 6)) {
      throw new Error(`sound-table entry ${sound} does not mirror its player selector`);
    }
  }
  const sound138 = SOUND_LINKS[2][1] - ROM_BASE;
  const sound139 = SOUND_LINKS[3][1] - ROM_BASE;
  if (sound138 < 0 || sound138 + 12 > rom.length || sound139 < 0 || sound139 + 12 > rom.length) {
    throw new Error("reserve sound neighbors lie outside ROM");
  }
  if (rom[sound138] !== 1 || SOUND_LINKS[2][1] + 12 !== YOBI_ADDRESS ||
      rom.readUInt32LE(sound138 + 8) !== 0x08181988) {
    throw new Error("reserve sound stream does not follow sound 138");
  }
  if (rom[sound139] !== 1 || rom.readUInt32LE(sound139 + 8) !== YOBI_END) {
    throw new Error("reserve sound stream does not precede sound 139");
  }
}

function replaceDirectory<T>(directory: string, write: (staged: string) => T): T {
  const destination = resolve(directory), parent = dirname(destination);
  if (destination === parent) throw new Error("music residual export requires a dedicated directory");
  mkdirSync(parent, { recursive: true });
  const transaction = mkdtempSync(join(parent, ".music-residual-export-"));
  const staged = join(transaction, "new"), previous = join(transaction, "previous");
  let installed = false;
  try {
    const result = write(staged);
    if (existsSync(destination)) renameSync(destination, previous);
    try {
      renameSync(staged, destination);
      installed = true;
    } catch (error) {
      if (existsSync(previous)) renameSync(previous, destination);
      throw error;
    }
    return result;
  } finally {
    if (!installed && existsSync(previous) && !existsSync(destination)) renameSync(previous, destination);
    rmSync(transaction, { recursive: true, force: true });
  }
}

function writeMusicResiduals(rom: Buffer, directory: string): ResidualIndex {
  if (rom.length !== ROM_SIZE) throw new Error("music residual exporter requires the 8 MiB canonical ROM");
  verifySoundTableLinks(rom);
  mkdirSync(directory, { recursive: true });
  const yobiPath = resolve(directory, "sound_138_yobi.json");
  const indexPath = resolve(directory, "index.json");
  writeFileSync(yobiPath, `${JSON.stringify(YOBI_SOURCE, null, 2)}\n`);
  const index: ResidualIndex = {
    format: 1,
    kind: "golden-sun-music-residuals",
    empty_headers: EMPTY_HEADERS.map((header) => ({ ...header })),
    orphan_stream: {
      address: "0x081819b0",
      size: 18,
      source: "sound_138_yobi.json",
      between: ["sound_138", "sound_139"],
    },
    tail_alignment: {
      address: "0x08184698",
      end: "0x08185000",
      boundary: 0x1000,
      fill: 0,
    },
  };
  writeFileSync(indexPath, `${JSON.stringify(index, null, 2)}\n`);
  verifyRegions(rom, build_music_residuals(indexPath));
  return index;
}

export function export_music_residuals(romPath: string, directory: string): ResidualIndex {
  validateExportDestination(romPath, directory);
  const rom = readFileSync(romPath);
  return replaceDirectory(directory, (staged) => writeMusicResiduals(rom, staged));
}

export function verify_music_residuals(romPath: string, indexPath: string): void {
  const rom = readFileSync(romPath);
  if (rom.length !== ROM_SIZE) throw new Error("music residual verifier requires the 8 MiB canonical ROM");
  verifySoundTableLinks(rom);
  const bytes = verifyRegions(rom, build_music_residuals(indexPath));
  console.log(`identical=true regions=6 source_bytes=${bytes}`);
}

export function self_test(): void {
  for (const header of EMPTY_HEADERS) {
    const built = emptyHeader(header);
    if (built.length !== 8 || built[0] !== 0 || built.readUInt32LE(4) !== Number(header.tone_bank)) {
      throw new Error("empty sound header self-test failed");
    }
  }
  const [, report] = build_sequence(YOBI_SOURCE);
  if (report.bytes !== 18 || report.events !== 11 || report.tracks !== 0) throw new Error("reserve sound stream self-test failed");
  const temporary = mkdtempSync("/private/tmp/music-residual-self-test-");
  try {
    const reject = (action: () => void): void => {
      let failed = false;
      try { action(); } catch { failed = true; }
      if (!failed) throw new Error("music residual adversarial self-test failed");
    };
    const duplicate = join(temporary, "duplicate.json");
    writeFileSync(duplicate, "{\"format\":0,\"format\":1}\n");
    reject(() => jsonDocument(duplicate, "test document"));
    const linked = Buffer.alloc(SOUND_LINKS[3][1] - ROM_BASE + 12);
    for (const [sound, header] of SOUND_LINKS) {
      const offset = SOUND_TABLE_ADDRESS - ROM_BASE + sound * 8;
      linked.writeUInt32LE(header, offset);
      linked.writeUInt16LE(sound, offset + 4);
      linked.writeUInt16LE(sound, offset + 6);
    }
    const sound138 = SOUND_LINKS[2][1] - ROM_BASE;
    const sound139 = SOUND_LINKS[3][1] - ROM_BASE;
    linked[sound138] = 1;
    linked.writeUInt32LE(0x08181988, sound138 + 8);
    linked[sound139] = 1;
    linked.writeUInt32LE(YOBI_END, sound139 + 8);
    verifySoundTableLinks(linked);
    const entry19 = SOUND_TABLE_ADDRESS - ROM_BASE + 19 * 8;
    linked.writeUInt32LE(SOUND_LINKS[0][1] + 4, entry19);
    reject(() => verifySoundTableLinks(linked));
    linked.writeUInt32LE(SOUND_LINKS[0][1], entry19);
    linked.writeUInt32LE(YOBI_END + 1, sound139 + 8);
    reject(() => verifySoundTableLinks(linked));
    const destination = join(temporary, "destination");
    mkdirSync(destination);
    reject(() => validateExportDestination(join(destination, "rom.gba"), destination));
    const physicalRom = join(destination, "physical.gba");
    const romLink = join(temporary, "rom-link.gba");
    writeFileSync(physicalRom, "rom");
    symlinkSync(physicalRom, romLink);
    reject(() => validateExportDestination(romLink, destination));
    const unrelated = join(temporary, "unrelated");
    mkdirSync(unrelated);
    writeFileSync(join(unrelated, "keep"), "old");
    reject(() => validateExportDestination(join(temporary, "rom.gba"), unrelated));
    writeFileSync(join(destination, "stale"), "old");
    reject(() => replaceDirectory(destination, (staged) => {
      mkdirSync(staged);
      writeFileSync(join(staged, "partial"), "new");
      throw new Error("synthetic failure");
    }));
    if (!existsSync(join(destination, "stale")) || existsSync(join(destination, "partial"))) {
      throw new Error("failed music residual export changed its destination");
    }
    replaceDirectory(destination, (staged) => {
      mkdirSync(staged);
      writeFileSync(join(staged, "current"), "new");
    });
    if (existsSync(join(destination, "stale")) || !existsSync(join(destination, "current"))) {
      throw new Error("music residual export did not replace its whole destination");
    }
  } finally {
    rmSync(temporary, { recursive: true, force: true });
  }
  console.log("self-test=ok");
}

function main(args: string[]): void {
  if (args.length === 1 && args[0] === "--self-test") { self_test(); return; }
  if (args.length === 1 && (args[0] === "-h" || args[0] === "--help")) {
    console.log("usage: music_residuals.ts export ROM --directory DIR | verify ROM --index INDEX | --self-test");
    return;
  }
  if (args.length === 4 && args[0] === "export" && args[2] === "--directory") {
    export_music_residuals(args[1], args[3]);
    console.log(`regions=6 source_bytes=${ALIGNMENT_END - ALIGNMENT_ADDRESS + 50}`);
    return;
  }
  if (args.length === 4 && args[0] === "verify" && args[2] === "--index") {
    verify_music_residuals(args[1], args[3]);
    return;
  }
  throw new Error("usage: music_residuals.ts export ROM --directory DIR | verify ROM --index INDEX | --self-test");
}

if (import.meta.main) main(Bun.argv.slice(2));
