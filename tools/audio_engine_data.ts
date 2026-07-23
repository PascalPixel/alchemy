#!/usr/bin/env bun
// Tool role: both; imported by tools/build_assets.ts; invoked by package.json.
import { canonicalJson, isCanonicalJsonText } from "./canonical_json.ts";
import {
  existsSync, lstatSync, mkdirSync, mkdtempSync, readFileSync, readdirSync, realpathSync, renameSync, rmSync,
  symlinkSync, writeFileSync,
} from "fs";
import { basename, dirname, isAbsolute, join, relative, resolve } from "path";

const ROM_BASE = 0x08000000;
const ROM_SIZE = 0x00800000;
export const AUDIO_ENGINE_ADDRESS = 0x080fb792;
export const AUDIO_ENGINE_END = 0x080fc684;
export const AUDIO_ENGINE_SIZE = AUDIO_ENGINE_END - AUDIO_ENGINE_ADDRESS;

const CONTROL_END = 0x080fba78;
const BANK_0_ADDRESS = 0x080fba78;
const BANK_1_ADDRESS = 0x080fc138;
const WAVEFORM_ADDRESS = 0x080fc504;
const PLAYER_ADDRESS = 0x080fc624;

const SOURCE_NAMES = {
  control: "seigyo.json",
  tones: "onshoku.json",
  waveforms: "hakei.json",
  players: "saisei.json",
} as const;

type JsonObject = Record<string, unknown>;

interface PackageIndex {
  format: 1;
  kind: "golden-sun-audio-engine-data";
  address: string;
  end: string;
  size: number;
  sources: typeof SOURCE_NAMES;
}

interface ControlSource {
  format: 1;
  kind: "golden-sun-audio-engine-control";
  address: string;
  end: string;
  leading_alignment: { address: string; size: number; fill: number };
  diagnostic_sounds: number[];
  command_dispatch: string[];
  direct_pitch_codes: number[];
  direct_frequency_ratios: string[];
  pcm_samples_per_vblank: number[];
  cgb_pitch_codes: number[];
  cgb_frequency_steps: number[];
  noise_pitch_codes: number[];
  cgb_volume_registers: number[];
  wait_durations: number[];
  wait_alignment: { address: string; size: number; fill: number };
  cgb_command_dispatch: string[];
}

interface ToneBankSource {
  format: 1;
  kind: "golden-sun-audio-tone-banks";
  address: string;
  end: string;
  banks: Array<{ name: string; address: string; records: JsonObject[] }>;
}

interface WaveformSource {
  format: 1;
  kind: "golden-sun-cgb-waveforms";
  address: string;
  end: string;
  waveforms: Array<{ name: string; samples: number[] }>;
}

interface PlayerSource {
  format: 1;
  kind: "golden-sun-music-players";
  address: string;
  end: string;
  players: Array<{ name: string; state: string; track_storage: string; max_tracks: number }>;
}

interface BuiltAudioEngineData {
  address: number;
  data: Buffer;
  sources: string[];
}

function hex(value: number): string {
  return `0x${value.toString(16).padStart(8, "0")}`;
}

function pretty(value: unknown): string {
  return `${canonicalJson(value)}\n`;
}

function exactKeys(value: unknown, keys: readonly string[], label: string): asserts value is JsonObject {
  if (typeof value !== "object" || value === null || Array.isArray(value)) throw new Error(`${label} must be an object`);
  const actual = Object.keys(value).sort(), expected = [...keys].sort();
  if (actual.length !== expected.length || actual.some((key, index) => key !== expected[index])) {
    throw new Error(`${label} has unexpected fields`);
  }
}

function integer(value: unknown, minimum: number, maximum: number, label: string): number {
  if (typeof value !== "number" || !Number.isSafeInteger(value) || value < minimum || value > maximum) {
    throw new Error(`${label} is outside its range`);
  }
  return value;
}

function fixedArray(value: unknown, count: number, label: string): unknown[] {
  if (!Array.isArray(value) || value.length !== count) throw new Error(`${label} requires ${count} entries`);
  return value;
}

function address(value: unknown, label: string, minimum = ROM_BASE, maximum = 0x09ffffff): number {
  if (typeof value !== "string" || !/^0x[0-9a-f]{8}$/.test(value)) throw new Error(`${label} is not a canonical address`);
  const parsed = Number(value);
  if (!Number.isSafeInteger(parsed) || parsed < minimum || parsed > maximum || parsed % 4 !== 0) {
    throw new Error(`${label} is outside its address range`);
  }
  return parsed;
}

function canonicalDocument(path: string, label: string): unknown {
  const text = readFileSync(path, "utf8"), value = JSON.parse(text);
  if (!isCanonicalJsonText(text, value)) throw new Error(`${label} is not canonical JSON`);
  return value;
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

function samePath(left: string, right: string): boolean {
  return physicalPath(left) === physicalPath(right);
}

function containsPath(directory: string, path: string): boolean {
  const position = relative(physicalPath(directory), physicalPath(path));
  return position === "" || (position !== ".." && !position.startsWith("../") &&
    !position.startsWith("..\\") && !isAbsolute(position));
}

function child(indexPath: string, name: string): string {
  if (!Object.values(SOURCE_NAMES).includes(name as never)) throw new Error("audio-engine source name differs");
  const prefix = basename(indexPath).replace(/index\.json$/, "");
  const root = realpathSync(dirname(indexPath)), path = realpathSync(resolve(root, prefix + name));
  if (relative(root, path) !== prefix + name || !lstatSync(path).isFile()) throw new Error("audio-engine source escaped its directory");
  return path;
}

function readIndex(indexPath: string): PackageIndex {
  const value = canonicalDocument(indexPath, "audio-engine index");
  exactKeys(value, ["format", "kind", "address", "end", "size", "sources"], "audio-engine index");
  if (value.format !== 1 || value.kind !== "golden-sun-audio-engine-data" ||
      value.address !== hex(AUDIO_ENGINE_ADDRESS) || value.end !== hex(AUDIO_ENGINE_END) ||
      value.size !== AUDIO_ENGINE_SIZE) throw new Error("audio-engine index extent differs");
  exactKeys(value.sources, ["control", "tones", "waveforms", "players"], "audio-engine sources");
  for (const [role, name] of Object.entries(SOURCE_NAMES)) {
    if (value.sources[role] !== name) throw new Error("audio-engine source catalog differs");
  }
  return value as unknown as PackageIndex;
}

function parseFunction(value: unknown, label: string): number {
  if (typeof value !== "string" || !/^Func_0[0-9a-f]{7}$/.test(value)) throw new Error(`${label} is not a function symbol`);
  const code = Number(`0x${value.slice(5)}`);
  if (code < 0x080f9000 || code >= AUDIO_ENGINE_ADDRESS || code % 2 !== 0) {
    throw new Error(`${label} lies outside the audio engine`);
  }
  return code | 1;
}

function functionName(pointer: number): string {
  if ((pointer & 1) === 0) throw new Error("audio-engine dispatch target is not Thumb code");
  const code = pointer & ~1;
  if (code < 0x080f9000 || code >= AUDIO_ENGINE_ADDRESS) throw new Error("audio-engine dispatch target lies outside code");
  return `Func_${code.toString(16).padStart(8, "0")}`;
}

function alignment(value: unknown, expectedAddress: number, expectedSize: number, label: string): Buffer {
  exactKeys(value, ["address", "size", "fill"], label);
  if (value.address !== hex(expectedAddress) || value.size !== expectedSize || value.fill !== 0) {
    throw new Error(`${label} differs`);
  }
  return Buffer.alloc(expectedSize);
}

function unsignedBytes(value: unknown, count: number, label: string): Buffer {
  return Buffer.from(fixedArray(value, count, label).map((item, index) => integer(item, 0, 0xff, `${label} ${index}`)));
}

function unsignedHalfwords(value: unknown, count: number, label: string): Buffer {
  const output = Buffer.alloc(count * 2);
  fixedArray(value, count, label).forEach((item, index) => output.writeUInt16LE(integer(item, 0, 0xffff, `${label} ${index}`), index * 2));
  return output;
}

function signedHalfwords(value: unknown, count: number, label: string): Buffer {
  const output = Buffer.alloc(count * 2);
  fixedArray(value, count, label).forEach((item, index) => output.writeInt16LE(integer(item, -0x8000, 0x7fff, `${label} ${index}`), index * 2));
  return output;
}

function words(value: unknown, count: number, label: string): Buffer {
  const output = Buffer.alloc(count * 4);
  fixedArray(value, count, label).forEach((item, index) => {
    if (typeof item !== "string" || !/^0x[0-9a-f]{8}$/.test(item)) throw new Error(`${label} ${index} is not a word`);
    output.writeUInt32LE(Number(item), index * 4);
  });
  return output;
}

function dispatch(value: unknown, count: number, label: string): Buffer {
  const output = Buffer.alloc(count * 4);
  fixedArray(value, count, label).forEach((item, index) => output.writeUInt32LE(parseFunction(item, `${label} ${index}`), index * 4));
  return output;
}

function readControl(path: string): ControlSource {
  const value = canonicalDocument(path, "audio-engine control");
  exactKeys(value, [
    "format", "kind", "address", "end", "leading_alignment", "diagnostic_sounds", "command_dispatch",
    "direct_pitch_codes", "direct_frequency_ratios", "pcm_samples_per_vblank", "cgb_pitch_codes",
    "cgb_frequency_steps", "noise_pitch_codes", "cgb_volume_registers", "wait_durations", "wait_alignment",
    "cgb_command_dispatch",
  ], "audio-engine control");
  if (value.format !== 1 || value.kind !== "golden-sun-audio-engine-control" ||
      value.address !== hex(AUDIO_ENGINE_ADDRESS) || value.end !== hex(CONTROL_END)) {
    throw new Error("audio-engine control extent differs");
  }
  return value as unknown as ControlSource;
}

function buildControl(source: ControlSource): Buffer {
  const diagnostic = fixedArray(source.diagnostic_sounds, 3, "diagnostic sounds").map((item, index) =>
    integer(item, 0, 0xffff, `diagnostic sound ${index}`));
  if (new Set(diagnostic).size !== diagnostic.length) throw new Error("diagnostic sounds must be distinct");
  const diagnosticBytes = Buffer.alloc(12);
  diagnostic.forEach((item, index) => diagnosticBytes.writeUInt32LE(item, index * 4));
  const ratios = words(source.direct_frequency_ratios, 12, "direct frequency ratios");
  for (let index = 0; index < 12; index++) {
    if ((ratios.readUInt32LE(index * 4) & 0x80000000) === 0 ||
        (index && ratios.readUInt32LE(index * 4) <= ratios.readUInt32LE((index - 1) * 4))) {
      throw new Error("direct frequency ratios are not increasing fixed-point values");
    }
  }
  const samples = unsignedHalfwords(source.pcm_samples_per_vblank, 12, "PCM samples per VBlank");
  for (let index = 0; index < 12; index++) {
    if (samples.readUInt16LE(index * 2) === 0 ||
        (index && samples.readUInt16LE(index * 2) <= samples.readUInt16LE((index - 1) * 2))) {
      throw new Error("PCM sample counts are not increasing");
    }
  }
  const waits = unsignedBytes(source.wait_durations, 49, "wait durations");
  for (let index = 1; index < waits.length; index++) {
    if (waits[index] < waits[index - 1]) throw new Error("wait durations are not ordered");
  }
  const output = Buffer.concat([
    alignment(source.leading_alignment, AUDIO_ENGINE_ADDRESS, 2, "leading alignment"),
    diagnosticBytes,
    dispatch(source.command_dispatch, 36, "command dispatch"),
    unsignedBytes(source.direct_pitch_codes, 180, "direct pitch codes"),
    ratios,
    samples,
    unsignedBytes(source.cgb_pitch_codes, 132, "CGB pitch codes"),
    signedHalfwords(source.cgb_frequency_steps, 12, "CGB frequency steps"),
    unsignedBytes(source.noise_pitch_codes, 60, "noise pitch codes"),
    unsignedBytes(source.cgb_volume_registers, 16, "CGB volume registers"),
    waits,
    alignment(source.wait_alignment, 0x080fba45, 3, "wait alignment"),
    dispatch(source.cgb_command_dispatch, 12, "CGB command dispatch"),
  ]);
  if (output.length !== CONTROL_END - AUDIO_ENGINE_ADDRESS) throw new Error("audio-engine control size differs");
  return output;
}

function toneSymbol(bank: number, record: number): string {
  return `bank_${bank}_${record.toString().padStart(3, "0")}`;
}

function waveformSymbol(index: number): string {
  return `wave_${index.toString().padStart(2, "0")}`;
}

function toneAddress(symbol: unknown): number {
  if (typeof symbol !== "string" || !/^bank_[01]_[0-9]{3}$/.test(symbol)) throw new Error("rhythm tone symbol differs");
  const bank = Number(symbol[5]), record = Number(symbol.slice(7));
  const count = bank === 0 ? 144 : 81;
  const base = bank === 0 ? BANK_0_ADDRESS : BANK_1_ADDRESS;
  if (record >= count) throw new Error("rhythm tone symbol lies outside its bank");
  return base + record * 12;
}

function toneName(pointer: number): string {
  for (const [bank, base, count] of [[0, BANK_0_ADDRESS, 144], [1, BANK_1_ADDRESS, 81]] as const) {
    const delta = pointer - base;
    if (delta >= 0 && delta % 12 === 0 && delta / 12 < count) return toneSymbol(bank, delta / 12);
  }
  throw new Error("rhythm tone pointer does not select a tone record");
}

function waveformAddress(symbol: unknown): number {
  if (typeof symbol !== "string" || !/^wave_[0-9]{2}$/.test(symbol)) throw new Error("waveform symbol differs");
  const index = Number(symbol.slice(5));
  if (index >= 18) throw new Error("waveform symbol lies outside the waveform catalog");
  return WAVEFORM_ADDRESS + index * 16;
}

function waveformName(pointer: number): string {
  const delta = pointer - WAVEFORM_ADDRESS;
  if (delta < 0 || delta % 16 !== 0 || delta / 16 >= 18) throw new Error("wave tone does not select a waveform");
  return waveformSymbol(delta / 16);
}

function envelope(value: unknown, label: string): Buffer {
  return unsignedBytes(value, 4, label);
}

function buildToneRecord(value: unknown, label: string): Buffer {
  if (typeof value !== "object" || value === null || Array.isArray(value)) throw new Error(`${label} must be an object`);
  const kind = (value as JsonObject).kind;
  exactKeys(value, kind === "rhythm"
    ? ["kind", "key", "length", "pan_sweep", "tones", "key_map"]
    : kind === "pcm"
      ? ["kind", "fixed_pitch", "key", "length", "pan_sweep", "sample", "envelope"]
      : kind === "wave"
        ? ["kind", "fixed_pitch", "key", "length", "pan_sweep", "waveform", "envelope"]
        : ["kind", "fixed_pitch", "key", "length", "pan_sweep", "generator", "envelope"], label);
  const output = Buffer.alloc(12);
  output[1] = integer(value.key, 0, 0xff, `${label} key`);
  output[2] = integer(value.length, 0, 0xff, `${label} length`);
  output[3] = integer(value.pan_sweep, 0, 0xff, `${label} pan/sweep`);
  if (value.kind === "rhythm") {
    if (value.key_map !== null) throw new Error(`${label} rhythm key map is not absent`);
    output[0] = 0x80;
    output.writeUInt32LE(toneAddress(value.tones), 4);
    return output;
  }
  const kinds: Record<string, number> = { pcm: 0, pulse_1: 1, pulse_2: 2, wave: 3, noise: 4 };
  if (typeof value.kind !== "string" || kinds[value.kind] === undefined) throw new Error(`${label} tone kind differs`);
  if (typeof value.fixed_pitch !== "boolean") throw new Error(`${label} fixed-pitch flag differs`);
  output[0] = kinds[value.kind] | (value.fixed_pitch ? 8 : 0);
  if (![0, 1, 8, 9, 10, 11, 12].includes(output[0])) throw new Error(`${label} tone type is not used by this package`);
  if (value.kind === "pcm") {
    output.writeUInt32LE(address(value.sample, `${label} sample`, 0x080fd048, 0x0815fb77), 4);
  } else if (value.kind === "wave") {
    output.writeUInt32LE(waveformAddress(value.waveform), 4);
  } else {
    output.writeUInt32LE(integer(value.generator, 0, 3, `${label} generator`), 4);
  }
  envelope(value.envelope, `${label} envelope`).copy(output, 8);
  return output;
}

function readToneBanks(path: string): ToneBankSource {
  const value = canonicalDocument(path, "audio tone banks");
  exactKeys(value, ["format", "kind", "address", "end", "banks"], "audio tone banks");
  if (value.format !== 1 || value.kind !== "golden-sun-audio-tone-banks" ||
      value.address !== hex(BANK_0_ADDRESS) || value.end !== hex(WAVEFORM_ADDRESS)) {
    throw new Error("audio tone-bank extent differs");
  }
  return value as unknown as ToneBankSource;
}

function buildToneBanks(source: ToneBankSource): Buffer {
  const banks = fixedArray(source.banks, 2, "tone banks");
  const output: Buffer[] = [];
  for (let bank = 0; bank < 2; bank++) {
    const item = banks[bank];
    exactKeys(item, ["name", "address", "records"], `tone bank ${bank}`);
    const base = bank === 0 ? BANK_0_ADDRESS : BANK_1_ADDRESS, count = bank === 0 ? 144 : 81;
    if (item.name !== `bank_${bank}` || item.address !== hex(base)) throw new Error(`tone bank ${bank} identity differs`);
    fixedArray(item.records, count, `tone bank ${bank} records`).forEach((record, index) =>
      output.push(buildToneRecord(record, `tone bank ${bank} record ${index}`)));
  }
  const built = Buffer.concat(output);
  if (built.length !== WAVEFORM_ADDRESS - BANK_0_ADDRESS) throw new Error("audio tone-bank size differs");
  return built;
}

function readWaveforms(path: string): WaveformSource {
  const value = canonicalDocument(path, "CGB waveforms");
  exactKeys(value, ["format", "kind", "address", "end", "waveforms"], "CGB waveforms");
  if (value.format !== 1 || value.kind !== "golden-sun-cgb-waveforms" ||
      value.address !== hex(WAVEFORM_ADDRESS) || value.end !== hex(PLAYER_ADDRESS)) {
    throw new Error("CGB waveform extent differs");
  }
  return value as unknown as WaveformSource;
}

function buildWaveforms(source: WaveformSource): Buffer {
  const output = Buffer.alloc(18 * 16);
  fixedArray(source.waveforms, 18, "CGB waveforms").forEach((item, index) => {
    exactKeys(item, ["name", "samples"], `CGB waveform ${index}`);
    if (item.name !== waveformSymbol(index)) throw new Error(`CGB waveform ${index} name differs`);
    const samples = fixedArray(item.samples, 32, `CGB waveform ${index} samples`).map((sample, position) =>
      integer(sample, 0, 15, `CGB waveform ${index} sample ${position}`));
    for (let sample = 0; sample < 32; sample += 2) {
      output[index * 16 + sample / 2] = samples[sample] << 4 | samples[sample + 1];
    }
  });
  return output;
}

function readPlayers(path: string): PlayerSource {
  const value = canonicalDocument(path, "music players");
  exactKeys(value, ["format", "kind", "address", "end", "players"], "music players");
  if (value.format !== 1 || value.kind !== "golden-sun-music-players" ||
      value.address !== hex(PLAYER_ADDRESS) || value.end !== hex(AUDIO_ENGINE_END)) {
    throw new Error("music-player extent differs");
  }
  return value as unknown as PlayerSource;
}

function ewramAddress(value: unknown, label: string): number {
  return address(value, label, 0x02000000, 0x0203ffff);
}

function buildPlayers(source: PlayerSource): Buffer {
  const output = Buffer.alloc(8 * 12), states = new Set<number>(), tracks = new Set<number>();
  fixedArray(source.players, 8, "music players").forEach((item, index) => {
    exactKeys(item, ["name", "state", "track_storage", "max_tracks"], `music player ${index}`);
    if (item.name !== `player_${index}`) throw new Error(`music player ${index} name differs`);
    const state = ewramAddress(item.state, `music player ${index} state`);
    const storage = ewramAddress(item.track_storage, `music player ${index} track storage`);
    if (states.has(state) || tracks.has(storage)) throw new Error("music-player storage must be distinct");
    states.add(state); tracks.add(storage);
    output.writeUInt32LE(state, index * 12);
    output.writeUInt32LE(storage, index * 12 + 4);
    output[index * 12 + 8] = integer(item.max_tracks, 1, 16, `music player ${index} max tracks`);
  });
  return output;
}

export function build_audio_engine_data(indexPath: string): BuiltAudioEngineData {
  const index = readIndex(indexPath), paths = {
    control: child(indexPath, index.sources.control),
    tones: child(indexPath, index.sources.tones),
    waveforms: child(indexPath, index.sources.waveforms),
    players: child(indexPath, index.sources.players),
  };
  const data = Buffer.concat([
    buildControl(readControl(paths.control)),
    buildToneBanks(readToneBanks(paths.tones)),
    buildWaveforms(readWaveforms(paths.waveforms)),
    buildPlayers(readPlayers(paths.players)),
  ]);
  if (data.length !== AUDIO_ENGINE_SIZE) throw new Error("audio-engine package size differs");
  return { address: AUDIO_ENGINE_ADDRESS, data, sources: [realpathSync(indexPath), ...Object.values(paths)] };
}

function wordsAt(data: Buffer, offset: number, count: number): string[] {
  return Array.from({ length: count }, (_, index) => hex(data.readUInt32LE(offset + index * 4)));
}

function functionsAt(data: Buffer, offset: number, count: number): string[] {
  return Array.from({ length: count }, (_, index) => functionName(data.readUInt32LE(offset + index * 4)));
}

function bytesAt(data: Buffer, offset: number, count: number): number[] {
  return Array.from(data.subarray(offset, offset + count));
}

function u16At(data: Buffer, offset: number, count: number): number[] {
  return Array.from({ length: count }, (_, index) => data.readUInt16LE(offset + index * 2));
}

function i16At(data: Buffer, offset: number, count: number): number[] {
  return Array.from({ length: count }, (_, index) => data.readInt16LE(offset + index * 2));
}

function extractToneRecord(data: Buffer, offset: number): JsonObject {
  const type = data[offset], key = data[offset + 1], length = data[offset + 2], pan_sweep = data[offset + 3];
  const pointer = data.readUInt32LE(offset + 4), adsr = bytesAt(data, offset + 8, 4);
  if (type === 0x80) {
    if (data.readUInt32LE(offset + 8) !== 0) throw new Error("rhythm tone has an unsupported key map");
    return { kind: "rhythm", key, length, pan_sweep, tones: toneName(pointer), key_map: null };
  }
  const base = type & 7, fixed_pitch = Boolean(type & 8);
  if (![0, 1, 8, 9, 10, 11, 12].includes(type) || type & ~15) throw new Error("tone record has an unsupported type");
  if (base === 0) return { kind: "pcm", fixed_pitch, key, length, pan_sweep, sample: hex(pointer), envelope: adsr };
  if (base === 3) return { kind: "wave", fixed_pitch, key, length, pan_sweep, waveform: waveformName(pointer), envelope: adsr };
  const kind = base === 1 ? "pulse_1" : base === 2 ? "pulse_2" : base === 4 ? "noise" : undefined;
  if (!kind || pointer > 3) throw new Error("tone generator differs");
  return { kind, fixed_pitch, key, length, pan_sweep, generator: pointer, envelope: adsr };
}

function extractPackage(rom: Buffer): {
  index: PackageIndex; control: ControlSource; tones: ToneBankSource; waveforms: WaveformSource; players: PlayerSource;
} {
  if (rom.length !== ROM_SIZE) throw new Error("audio-engine exporter requires the canonical 8 MiB ROM");
  const data = rom.subarray(AUDIO_ENGINE_ADDRESS - ROM_BASE, AUDIO_ENGINE_END - ROM_BASE);
  if (data.length !== AUDIO_ENGINE_SIZE) throw new Error("audio-engine package lies outside ROM");
  const at = (value: number) => value - AUDIO_ENGINE_ADDRESS;
  if (data.subarray(at(AUDIO_ENGINE_ADDRESS), at(AUDIO_ENGINE_ADDRESS) + 2).some(Boolean)) {
    throw new Error("audio-engine leading alignment is not zero");
  }
  if (data.subarray(at(0x080fba45), at(0x080fba48)).some(Boolean)) {
    throw new Error("audio-engine wait alignment is not zero");
  }
  const control: ControlSource = {
    format: 1,
    kind: "golden-sun-audio-engine-control",
    address: hex(AUDIO_ENGINE_ADDRESS),
    end: hex(CONTROL_END),
    leading_alignment: { address: hex(AUDIO_ENGINE_ADDRESS), size: 2, fill: 0 },
    diagnostic_sounds: Array.from({ length: 3 }, (_, index) => data.readUInt32LE(at(0x080fb794) + index * 4)),
    command_dispatch: functionsAt(data, at(0x080fb7a0), 36),
    direct_pitch_codes: bytesAt(data, at(0x080fb830), 180),
    direct_frequency_ratios: wordsAt(data, at(0x080fb8e4), 12),
    pcm_samples_per_vblank: u16At(data, at(0x080fb914), 12),
    cgb_pitch_codes: bytesAt(data, at(0x080fb92c), 132),
    cgb_frequency_steps: i16At(data, at(0x080fb9b0), 12),
    noise_pitch_codes: bytesAt(data, at(0x080fb9c8), 60),
    cgb_volume_registers: bytesAt(data, at(0x080fba04), 16),
    wait_durations: bytesAt(data, at(0x080fba14), 49),
    wait_alignment: { address: hex(0x080fba45), size: 3, fill: 0 },
    cgb_command_dispatch: functionsAt(data, at(0x080fba48), 12),
  };
  const banks = [["bank_0", BANK_0_ADDRESS, 144], ["bank_1", BANK_1_ADDRESS, 81]] as const;
  const tones: ToneBankSource = {
    format: 1,
    kind: "golden-sun-audio-tone-banks",
    address: hex(BANK_0_ADDRESS),
    end: hex(WAVEFORM_ADDRESS),
    banks: banks.map(([name, base, count]) => ({
      name,
      address: hex(base),
      records: Array.from({ length: count }, (_, index) => extractToneRecord(data, at(base) + index * 12)),
    })),
  };
  const waveforms: WaveformSource = {
    format: 1,
    kind: "golden-sun-cgb-waveforms",
    address: hex(WAVEFORM_ADDRESS),
    end: hex(PLAYER_ADDRESS),
    waveforms: Array.from({ length: 18 }, (_, index) => {
      const packed = data.subarray(at(WAVEFORM_ADDRESS) + index * 16, at(WAVEFORM_ADDRESS) + (index + 1) * 16);
      return {
        name: waveformSymbol(index),
        samples: Array.from(packed).flatMap((value) => [value >>> 4, value & 15]),
      };
    }),
  };
  const players: PlayerSource = {
    format: 1,
    kind: "golden-sun-music-players",
    address: hex(PLAYER_ADDRESS),
    end: hex(AUDIO_ENGINE_END),
    players: Array.from({ length: 8 }, (_, index) => {
      const offset = at(PLAYER_ADDRESS) + index * 12;
      if (data[offset + 9] !== 0 || data.readUInt16LE(offset + 10) !== 0) throw new Error("music-player reserved fields are not zero");
      return {
        name: `player_${index}`,
        state: hex(data.readUInt32LE(offset)),
        track_storage: hex(data.readUInt32LE(offset + 4)),
        max_tracks: data[offset + 8],
      };
    }),
  };
  const index: PackageIndex = {
    format: 1,
    kind: "golden-sun-audio-engine-data",
    address: hex(AUDIO_ENGINE_ADDRESS),
    end: hex(AUDIO_ENGINE_END),
    size: AUDIO_ENGINE_SIZE,
    sources: { ...SOURCE_NAMES },
  };
  buildControl(control);
  buildToneBanks(tones);
  buildWaveforms(waveforms);
  buildPlayers(players);
  return { index, control, tones, waveforms, players };
}

function writePackage(rom: Buffer, directory: string): void {
  const source = extractPackage(rom);
  mkdirSync(directory, { recursive: true });
  writeFileSync(join(directory, SOURCE_NAMES.control), pretty(source.control));
  writeFileSync(join(directory, SOURCE_NAMES.tones), pretty(source.tones));
  writeFileSync(join(directory, SOURCE_NAMES.waveforms), pretty(source.waveforms));
  writeFileSync(join(directory, SOURCE_NAMES.players), pretty(source.players));
  const indexPath = join(directory, "index.json");
  writeFileSync(indexPath, pretty(source.index));
  const built = build_audio_engine_data(indexPath);
  const expected = rom.subarray(AUDIO_ENGINE_ADDRESS - ROM_BASE, AUDIO_ENGINE_END - ROM_BASE);
  if (!built.data.equals(expected)) throw new Error("exported audio-engine package differs from ROM");
}

function validateExportDestination(romPath: string, directory: string): void {
  if (samePath(romPath, directory) || containsPath(directory, romPath)) {
    throw new Error("audio-engine export directory must not contain its input ROM");
  }
  const destination = resolve(directory);
  if (!existsSync(destination)) return;
  if (!lstatSync(destination).isDirectory()) throw new Error("audio-engine export destination must be a directory");
  if (readdirSync(destination).length === 0) return;
  const marker = join(destination, "index.json");
  if (!existsSync(marker) || !lstatSync(marker).isFile()) {
    throw new Error("refusing to replace a directory that is not an audio-engine package");
  }
  try { build_audio_engine_data(marker); }
  catch { throw new Error("refusing to replace a directory that is not a canonical audio-engine package"); }
}

function replaceDirectory(directory: string, write: (staged: string) => void): void {
  const destination = resolve(directory), parent = dirname(destination);
  if (destination === parent) throw new Error("audio-engine export requires a dedicated directory");
  mkdirSync(parent, { recursive: true });
  const transaction = mkdtempSync(join(parent, ".audio-engine-export-"));
  const staged = join(transaction, "new"), previous = join(transaction, "previous");
  let installed = false;
  try {
    write(staged);
    if (existsSync(destination)) renameSync(destination, previous);
    try {
      renameSync(staged, destination);
      installed = true;
    } catch (error) {
      if (existsSync(previous)) renameSync(previous, destination);
      throw error;
    }
  } finally {
    if (!installed && existsSync(previous) && !existsSync(destination)) renameSync(previous, destination);
    rmSync(transaction, { recursive: true, force: true });
  }
}

export function export_audio_engine_data(romPath: string, directory: string): void {
  validateExportDestination(romPath, directory);
  const rom = readFileSync(romPath);
  replaceDirectory(directory, (staged) => writePackage(rom, staged));
}

export function verify_audio_engine_data(romPath: string, indexPath: string): void {
  const rom = readFileSync(romPath);
  if (rom.length !== ROM_SIZE) throw new Error("audio-engine verifier requires the canonical 8 MiB ROM");
  const built = build_audio_engine_data(indexPath);
  const expected = rom.subarray(AUDIO_ENGINE_ADDRESS - ROM_BASE, AUDIO_ENGINE_END - ROM_BASE);
  if (!built.data.equals(expected)) throw new Error("audio-engine package differs from ROM");
  console.log(`identical=true regions=1 source_bytes=${built.data.length}`);
}

export function self_test(): void {
  const temporary = mkdtempSync("/private/tmp/audio-engine-self-test-");
  let rejected = 0;
  const reject = (action: () => void): void => { try { action(); } catch { rejected++; } };
  try {
    const duplicate = join(temporary, "duplicate.json");
    writeFileSync(duplicate, "{\"format\":0,\"format\":1}\n");
    reject(() => canonicalDocument(duplicate, "test document"));
    reject(() => parseFunction("Func_08000000", "test function"));
    reject(() => unsignedBytes([256], 1, "test bytes"));
    reject(() => signedHalfwords([-0x8001], 1, "test halfwords"));
    reject(() => waveformAddress("wave_18"));
    reject(() => toneAddress("bank_0_144"));
    reject(() => buildToneRecord({
      kind: "pcm", fixed_pitch: false, key: 60, length: 0, pan_sweep: 0,
      sample: "0x080f0000", envelope: [1, 2, 3, 4],
    }, "test tone"));
    reject(() => buildToneRecord({
      kind: "rhythm", key: 0, length: 0, pan_sweep: 0, tones: "bank_0_000", key_map: "map",
    }, "test rhythm"));
    const wave: WaveformSource = {
      format: 1, kind: "golden-sun-cgb-waveforms", address: hex(WAVEFORM_ADDRESS), end: hex(PLAYER_ADDRESS),
      waveforms: Array.from({ length: 18 }, (_, index) => ({ name: waveformSymbol(index), samples: Array(32).fill(0) })),
    };
    const packed = buildWaveforms(wave);
    if (packed.length !== 288 || packed.some(Boolean)) throw new Error("CGB waveform packing self-test failed");
    wave.waveforms[0].samples[0] = 16;
    reject(() => buildWaveforms(wave));
    const destination = join(temporary, "destination");
    mkdirSync(destination);
    writeFileSync(join(destination, "stale"), "old");
    reject(() => replaceDirectory(destination, (staged) => {
      mkdirSync(staged);
      writeFileSync(join(staged, "partial"), "new");
      throw new Error("synthetic failure");
    }));
    if (!existsSync(join(destination, "stale")) || existsSync(join(destination, "partial"))) {
      throw new Error("failed audio-engine export changed its destination");
    }
    replaceDirectory(destination, (staged) => {
      mkdirSync(staged);
      writeFileSync(join(staged, "current"), "new");
    });
    if (existsSync(join(destination, "stale")) || !existsSync(join(destination, "current"))) {
      throw new Error("audio-engine export did not replace its whole destination");
    }
    const inside = join(temporary, "inside");
    mkdirSync(inside);
    reject(() => validateExportDestination(join(inside, "rom.gba"), inside));
    const physicalRom = join(inside, "physical.gba"), romLink = join(temporary, "rom-link.gba");
    writeFileSync(physicalRom, "rom");
    symlinkSync(physicalRom, romLink);
    reject(() => validateExportDestination(romLink, inside));
    if (rejected !== 12) throw new Error(`audio-engine adversarial validation count differs: ${rejected}`);
  } finally {
    rmSync(temporary, { recursive: true, force: true });
  }
  console.log("self-test=ok adversarial=12");
}

function main(args: string[]): void {
  if (args.length === 1 && args[0] === "--self-test") { self_test(); return; }
  if (args.length === 4 && args[0] === "export" && args[2] === "--directory") {
    export_audio_engine_data(args[1], args[3]);
    console.log(`regions=1 source_bytes=${AUDIO_ENGINE_SIZE}`);
    return;
  }
  if (args.length === 4 && args[0] === "verify" && args[2] === "--index") {
    verify_audio_engine_data(args[1], args[3]);
    return;
  }
  throw new Error("usage: audio_engine_data.ts export ROM --directory DIR | verify ROM --index INDEX | --self-test");
}

if (import.meta.main) main(Bun.argv.slice(2));
