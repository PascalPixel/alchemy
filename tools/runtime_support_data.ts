#!/usr/bin/env bun
// Tool role: both; imported by tools/build_assets.ts; invoked by package.json.
import { lstatSync, renameSync, statSync, unlinkSync } from "fs";

export const RUNTIME_SUPPORT_ADDRESS = 0x0800795c;
export const RUNTIME_SUPPORT_END = 0x08009000;
export const ROM_SIZE = 0x00800000;

type JsonObject = Record<string, unknown>;

interface TimingSet {
  address: string;
  slots: number[][];
}

interface FlashDevice {
  address: string;
  callbacks: {
    program_sector: string;
    erase_chip: string;
    erase_sector: string;
    wait_write: string;
  };
  timing: string;
  geometry: {
    total_bytes: number;
    sector_bytes: number;
    sector_shift: number;
    sector_count: number;
    top_sector: number;
  };
  wait_control: number[];
  manufacturer_id: string;
  device_id: string;
}

interface RuntimeSupportDocument {
  format: 1;
  kind: "golden-sun-runtime-support-data";
  number_format: {
    address: string;
    end: string;
    hexadecimal_digits: string;
    trailing_zero_bytes: number;
    decimal_divisors: number[];
  };
  transfer_test: {
    address: string;
    end: string;
    identity: string;
    identity_field_bytes: number;
    record_marker: string;
    reserved_zero_bytes: number;
    packets: Array<{ text: string; field_bytes: number }>;
    flash_version: string;
    flash_version_field_bytes: number;
  };
  flash_directory: {
    address: string;
    devices: string[];
  };
  timing_sets: Record<string, TimingSet>;
  flash_devices: Record<string, FlashDevice>;
  alignment_fill: {
    address: string;
    end: string;
    value: 0;
  };
}

export interface RuntimeSupportBuild {
  regions: Map<number, Buffer>;
  source_bytes: number;
}

const NUMBER_ADDRESS = 0x0800795c;
const NUMBER_END = 0x08007994;
const TRANSFER_ADDRESS = 0x080079b0;
const TRANSFER_END = 0x08007a20;
const DIRECTORY_ADDRESS = 0x08007a0c;
const FLASH_REGIONS: Array<[number, number]> = [
  [0x08007aa4, 0x08007b14],
  [0x08007b3c, 0x08007bc4],
  [0x08007bcc, 0x08007c3c],
];
const ALIGNMENT_ADDRESS = 0x08007c64;
const ALIGNMENT_END = 0x08009000;

function object(value: unknown, label: string): JsonObject {
  if (typeof value !== "object" || value === null || Array.isArray(value)) throw new Error(`${label} must be an object`);
  return value as JsonObject;
}

function exact_keys(value: JsonObject, keys: string[], label: string): void {
  const actual = Object.keys(value).sort();
  const expected = [...keys].sort();
  if (actual.length !== expected.length || actual.some((key, index) => key !== expected[index])) {
    throw new Error(`${label} fields differ`);
  }
}

function integer(value: unknown, minimum: number, maximum: number, label: string): number {
  if (!Number.isSafeInteger(value) || Number(value) < minimum || Number(value) > maximum) {
    throw new Error(`${label} is outside the supported range`);
  }
  return Number(value);
}

function address(value: unknown, expected: number, label: string): number {
  const canonical = `0x${expected.toString(16).padStart(8, "0")}`;
  if (value !== canonical) throw new Error(`${label} differs`);
  return expected;
}

function byte_id(value: unknown, label: string): number {
  if (typeof value !== "string" || !/^0x[0-9a-f]{2}$/.test(value)) throw new Error(`${label} is not a canonical byte`);
  return Number.parseInt(value.slice(2), 16);
}

function ascii(value: unknown, label: string): string {
  if (typeof value !== "string" || [...value].some((character) => {
    const code = character.charCodeAt(0);
    return code < 0x20 || code > 0x7e;
  })) throw new Error(`${label} must contain printable ASCII`);
  return value;
}

function function_pointer(value: unknown, label: string): number {
  if (typeof value !== "string" || !/^Func_080[0-9a-f]{5}$/.test(value)) throw new Error(`${label} is not a canonical function symbol`);
  return Number.parseInt(value.slice(5), 16) | 1;
}

function fixed_ascii(value: string, width: number, label: string): Buffer {
  const data = Buffer.from(value, "ascii");
  if (data.length > width) throw new Error(`${label} exceeds its field`);
  const output = Buffer.alloc(width);
  data.copy(output);
  return output;
}

function runtime_document(value: unknown): RuntimeSupportDocument {
  const source = object(value, "runtime support source");
  exact_keys(source, [
    "format", "kind", "number_format", "transfer_test", "flash_directory",
    "timing_sets", "flash_devices", "alignment_fill",
  ], "runtime support source");
  if (source.format !== 1 || source.kind !== "golden-sun-runtime-support-data") throw new Error("unsupported runtime support source");

  const numberFormat = object(source.number_format, "number format");
  exact_keys(numberFormat, ["address", "end", "hexadecimal_digits", "trailing_zero_bytes", "decimal_divisors"], "number format");
  address(numberFormat.address, NUMBER_ADDRESS, "number-format address");
  address(numberFormat.end, NUMBER_END, "number-format end");
  if (numberFormat.hexadecimal_digits !== "0123456789ABCDEF") throw new Error("hexadecimal digit table differs");
  if (integer(numberFormat.trailing_zero_bytes, 0, 16, "number-format trailing zero bytes") !== 4) throw new Error("number-format alignment differs");
  const divisors = numberFormat.decimal_divisors;
  const expectedDivisors = [1_000_000_000, 100_000_000, 10_000_000, 1_000_000, 100_000, 10_000, 1_000, 100, 10];
  if (!Array.isArray(divisors) || divisors.length !== expectedDivisors.length ||
      divisors.some((item, index) => item !== expectedDivisors[index])) throw new Error("decimal divisor table differs");

  const transfer = object(source.transfer_test, "transfer test");
  exact_keys(transfer, [
    "address", "end", "identity", "identity_field_bytes", "record_marker", "reserved_zero_bytes",
    "packets", "flash_version", "flash_version_field_bytes",
  ], "transfer test");
  address(transfer.address, TRANSFER_ADDRESS, "transfer-test address");
  address(transfer.end, TRANSFER_END, "transfer-test end");
  ascii(transfer.identity, "transfer identity");
  ascii(transfer.record_marker, "transfer marker");
  ascii(transfer.flash_version, "flash version");
  integer(transfer.identity_field_bytes, 1, 32, "identity field bytes");
  integer(transfer.reserved_zero_bytes, 0, 32, "reserved zero bytes");
  integer(transfer.flash_version_field_bytes, 1, 32, "flash-version field bytes");
  if (!Array.isArray(transfer.packets) || transfer.packets.length !== 2) throw new Error("transfer packet collection differs");
  transfer.packets.forEach((item, index) => {
    const packet = object(item, `transfer packet ${index}`);
    exact_keys(packet, ["text", "field_bytes"], `transfer packet ${index}`);
    ascii(packet.text, `transfer packet ${index} text`);
    integer(packet.field_bytes, 1, 64, `transfer packet ${index} field bytes`);
  });

  const directory = object(source.flash_directory, "flash directory");
  exact_keys(directory, ["address", "devices"], "flash directory");
  address(directory.address, DIRECTORY_ADDRESS, "flash-directory address");
  if (!Array.isArray(directory.devices) || directory.devices.length !== 5 || directory.devices.some((item) => typeof item !== "string")) {
    throw new Error("flash-directory device collection differs");
  }

  const timingSets = object(source.timing_sets, "timing sets");
  const devices = object(source.flash_devices, "flash devices");
  if (Object.keys(timingSets).length !== 4 || Object.keys(devices).length !== 6) throw new Error("flash catalog size differs");
  const timingAddresses = new Set<number>();
  for (const [name, raw] of Object.entries(timingSets)) {
    if (!/^[a-z0-9_]+$/.test(name)) throw new Error("timing-set name is not canonical");
    const timing = object(raw, `timing set ${name}`);
    exact_keys(timing, ["address", "slots"], `timing set ${name}`);
    if (typeof timing.address !== "string" || !/^0x080[0-9a-f]{5}$/.test(timing.address)) throw new Error(`timing set ${name} address is not canonical`);
    const start = Number(timing.address);
    if (timingAddresses.has(start)) throw new Error("timing-set address is duplicated");
    timingAddresses.add(start);
    if (!Array.isArray(timing.slots) || timing.slots.length !== 4) throw new Error(`timing set ${name} slot collection differs`);
    timing.slots.forEach((slot, slotIndex) => {
      if (!Array.isArray(slot) || slot.length !== 3) throw new Error(`timing set ${name} slot ${slotIndex} differs`);
      slot.forEach((item, itemIndex) => integer(item, -0x8000, 0x7fff, `timing set ${name} slot ${slotIndex}:${itemIndex}`));
    });
  }

  const deviceAddresses = new Set<number>();
  for (const [name, raw] of Object.entries(devices)) {
    if (!/^[a-z0-9_]+$/.test(name)) throw new Error("flash-device name is not canonical");
    const device = object(raw, `flash device ${name}`);
    exact_keys(device, ["address", "callbacks", "timing", "geometry", "wait_control", "manufacturer_id", "device_id"], `flash device ${name}`);
    if (typeof device.address !== "string" || !/^0x080[0-9a-f]{5}$/.test(device.address)) throw new Error(`flash device ${name} address is not canonical`);
    const start = Number(device.address);
    if (deviceAddresses.has(start)) throw new Error("flash-device address is duplicated");
    deviceAddresses.add(start);
    const callbacks = object(device.callbacks, `flash device ${name} callbacks`);
    exact_keys(callbacks, ["program_sector", "erase_chip", "erase_sector", "wait_write"], `flash device ${name} callbacks`);
    for (const [role, symbol] of Object.entries(callbacks)) function_pointer(symbol, `flash device ${name} ${role}`);
    if (typeof device.timing !== "string" || !(device.timing in timingSets)) throw new Error(`flash device ${name} timing differs`);
    const geometry = object(device.geometry, `flash device ${name} geometry`);
    exact_keys(geometry, ["total_bytes", "sector_bytes", "sector_shift", "sector_count", "top_sector"], `flash device ${name} geometry`);
    const total = integer(geometry.total_bytes, 1, 0xffffffff, `flash device ${name} total bytes`);
    const sector = integer(geometry.sector_bytes, 1, 0xffff, `flash device ${name} sector bytes`);
    const shift = integer(geometry.sector_shift, 0, 31, `flash device ${name} sector shift`);
    const count = integer(geometry.sector_count, 1, 0xffff, `flash device ${name} sector count`);
    integer(geometry.top_sector, 0, 0xffffffff, `flash device ${name} top sector`);
    if (sector !== 2 ** shift || sector * count !== total) throw new Error(`flash device ${name} geometry is inconsistent`);
    if (!Array.isArray(device.wait_control) || device.wait_control.length !== 2) throw new Error(`flash device ${name} wait control differs`);
    device.wait_control.forEach((item, index) => integer(item, 0, 0xffff, `flash device ${name} wait control ${index}`));
    byte_id(device.manufacturer_id, `flash device ${name} manufacturer ID`);
    byte_id(device.device_id, `flash device ${name} device ID`);
  }
  const directoryNames = directory.devices as string[];
  if (new Set(directoryNames).size !== directoryNames.length || directoryNames.some((name) => !(name in devices))) {
    throw new Error("flash-directory device reference differs");
  }

  const alignment = object(source.alignment_fill, "alignment fill");
  exact_keys(alignment, ["address", "end", "value"], "alignment fill");
  address(alignment.address, ALIGNMENT_ADDRESS, "alignment-fill address");
  address(alignment.end, ALIGNMENT_END, "alignment-fill end");
  if (alignment.value !== 0) throw new Error("alignment fill is not zero");
  return source as unknown as RuntimeSupportDocument;
}

function build_number_format(source: RuntimeSupportDocument): Buffer {
  const output = Buffer.alloc(NUMBER_END - NUMBER_ADDRESS);
  let cursor = 0;
  cursor += output.write(source.number_format.hexadecimal_digits, cursor, "ascii");
  cursor += source.number_format.trailing_zero_bytes;
  for (const divisor of source.number_format.decimal_divisors) {
    output.writeUInt32LE(divisor, cursor);
    cursor += 4;
  }
  if (cursor !== output.length) throw new Error("number-format layout differs");
  return output;
}

function timing_buffer(source: RuntimeSupportDocument, name: string): Buffer {
  const timing = source.timing_sets[name];
  if (timing === undefined) throw new Error(`unknown timing set ${name}`);
  const output = Buffer.alloc(24);
  timing.slots.flat().forEach((value, index) => output.writeInt16LE(value, index * 2));
  return output;
}

function device_buffer(source: RuntimeSupportDocument, name: string): Buffer {
  const device = source.flash_devices[name];
  if (device === undefined) throw new Error(`unknown flash device ${name}`);
  const output = Buffer.alloc(44);
  const callbacks = [device.callbacks.program_sector, device.callbacks.erase_chip, device.callbacks.erase_sector, device.callbacks.wait_write];
  callbacks.forEach((symbol, index) => output.writeUInt32LE(function_pointer(symbol, `${name} callback ${index}`), index * 4));
  output.writeUInt32LE(Number(source.timing_sets[device.timing].address), 16);
  output.writeUInt32LE(device.geometry.total_bytes, 20);
  output.writeUInt16LE(device.geometry.sector_bytes, 24);
  output.writeUInt16LE(device.geometry.sector_shift, 28);
  output.writeUInt16LE(device.geometry.sector_count, 30);
  output.writeUInt32LE(device.geometry.top_sector, 32);
  output.writeUInt16LE(device.wait_control[0], 36);
  output.writeUInt16LE(device.wait_control[1], 38);
  output.writeUInt8(byte_id(device.manufacturer_id, `${name} manufacturer ID`), 40);
  output.writeUInt8(byte_id(device.device_id, `${name} device ID`), 41);
  return output;
}

function build_transfer(source: RuntimeSupportDocument): Buffer {
  const output = Buffer.alloc(TRANSFER_END - TRANSFER_ADDRESS);
  let cursor = 0;
  fixed_ascii(source.transfer_test.identity, source.transfer_test.identity_field_bytes, "transfer identity").copy(output, cursor);
  cursor += source.transfer_test.identity_field_bytes;
  cursor += output.write(source.transfer_test.record_marker, cursor, "ascii");
  cursor += source.transfer_test.reserved_zero_bytes;
  for (const [index, packet] of source.transfer_test.packets.entries()) {
    fixed_ascii(packet.text, packet.field_bytes, `transfer packet ${index}`).copy(output, cursor);
    cursor += packet.field_bytes;
  }
  fixed_ascii(source.transfer_test.flash_version, source.transfer_test.flash_version_field_bytes, "flash version").copy(output, cursor);
  cursor += source.transfer_test.flash_version_field_bytes;
  if (TRANSFER_ADDRESS + cursor !== DIRECTORY_ADDRESS) throw new Error("transfer-test record does not reach its flash directory");
  source.flash_directory.devices.forEach((name, index) => output.writeUInt32LE(Number(source.flash_devices[name].address), cursor + index * 4));
  cursor += source.flash_directory.devices.length * 4;
  if (cursor !== output.length) throw new Error("transfer-test layout differs");
  return output;
}

function build_flash_regions(source: RuntimeSupportDocument): Map<number, Buffer> {
  const pieces = new Map<number, Buffer>();
  for (const [name, timing] of Object.entries(source.timing_sets)) pieces.set(Number(timing.address), timing_buffer(source, name));
  for (const [name, device] of Object.entries(source.flash_devices)) pieces.set(Number(device.address), device_buffer(source, name));
  const regions = new Map<number, Buffer>();
  for (const [start, end] of FLASH_REGIONS) {
    const output = Buffer.alloc(end - start);
    const members = [...pieces].filter(([address]) => start <= address && address < end).sort((left, right) => left[0] - right[0]);
    let cursor = start;
    for (const [address, data] of members) {
      if (address !== cursor || address + data.length > end) throw new Error(`flash region 0x${start.toString(16)} has a gap or overlap`);
      data.copy(output, address - start);
      cursor += data.length;
    }
    if (cursor !== end) throw new Error(`flash region 0x${start.toString(16)} is incomplete`);
    regions.set(start, output);
  }
  if (pieces.size !== 10) throw new Error("flash region membership differs");
  return regions;
}

export function build_runtime_support_data(value: unknown): RuntimeSupportBuild {
  const source = runtime_document(value);
  const regions = new Map<number, Buffer>([
    [NUMBER_ADDRESS, build_number_format(source)],
    [TRANSFER_ADDRESS, build_transfer(source)],
    ...build_flash_regions(source),
    [ALIGNMENT_ADDRESS, Buffer.alloc(ALIGNMENT_END - ALIGNMENT_ADDRESS)],
  ]);
  const sourceBytes = [...regions.values()].reduce((sum, data) => sum + data.length, 0);
  if (sourceBytes !== 5_548) throw new Error("runtime-support source-byte total differs");
  return { regions, source_bytes: sourceBytes };
}

export function build_runtime_support_component(value: unknown, componentAddress: number, componentSize: number): Buffer {
  const built = build_runtime_support_data(value);
  const component = built.regions.get(componentAddress);
  if (component === undefined || component.length !== componentSize) throw new Error("runtime-support component extent differs");
  return component;
}

export function verify_runtime_support_data(rom: Uint8Array, value: unknown): RuntimeSupportBuild {
  if (rom.length !== ROM_SIZE) throw new Error("canonical ROM must contain exactly 8 MiB");
  const built = build_runtime_support_data(value);
  for (const [start, data] of built.regions) {
    const offset = start - 0x08000000;
    if (offset < 0 || offset + data.length > rom.length || !data.equals(Buffer.from(rom.subarray(offset, offset + data.length)))) {
      throw new Error(`runtime-support region 0x${start.toString(16)} differs`);
    }
  }
  return built;
}

class StrictJsonParser {
  private position = 0;

  constructor(private readonly text: string) {}

  parse(): unknown {
    const value = this.value();
    this.space();
    if (this.position !== this.text.length) throw new Error("runtime-support JSON has trailing input");
    return value;
  }

  private space(): void {
    while (this.position < this.text.length && /\s/.test(this.text[this.position])) this.position++;
  }

  private value(): unknown {
    this.space();
    const character = this.text[this.position];
    if (character === "{") return this.record();
    if (character === "[") return this.array();
    if (character === '"') return this.string();
    if (this.text.startsWith("true", this.position)) { this.position += 4; return true; }
    if (this.text.startsWith("false", this.position)) { this.position += 5; return false; }
    if (this.text.startsWith("null", this.position)) { this.position += 4; return null; }
    const match = /^-?(?:0|[1-9][0-9]*)(?:\.[0-9]+)?(?:[eE][+-]?[0-9]+)?/.exec(this.text.slice(this.position));
    if (match === null) throw new Error(`runtime-support JSON value is invalid at ${this.position}`);
    this.position += match[0].length;
    const numberValue = Number(match[0]);
    if (!Number.isFinite(numberValue)) throw new Error("runtime-support JSON number is not finite");
    return numberValue;
  }

  private string(): string {
    const start = this.position++;
    while (this.position < this.text.length) {
      const character = this.text[this.position++];
      if (character === '"') return JSON.parse(this.text.slice(start, this.position));
      if (character === "\\") {
        const escape = this.text[this.position++];
        if (escape === "u") {
          if (!/^[0-9a-fA-F]{4}$/.test(this.text.slice(this.position, this.position + 4))) {
            throw new Error("runtime-support JSON Unicode escape is invalid");
          }
          this.position += 4;
        } else if (escape === undefined || !'"\\/bfnrt'.includes(escape)) {
          throw new Error("runtime-support JSON escape is invalid");
        }
      } else if (character.charCodeAt(0) < 0x20) {
        throw new Error("runtime-support JSON string contains a control character");
      }
    }
    throw new Error("runtime-support JSON string is unterminated");
  }

  private record(): JsonObject {
    this.position++;
    const result: JsonObject = {};
    const keys = new Set<string>();
    this.space();
    if (this.text[this.position] === "}") { this.position++; return result; }
    while (true) {
      this.space();
      if (this.text[this.position] !== '"') throw new Error("runtime-support JSON object key is missing");
      const key = this.string();
      if (keys.has(key)) throw new Error(`runtime-support JSON has duplicate key ${JSON.stringify(key)}`);
      keys.add(key);
      this.space();
      if (this.text[this.position++] !== ":") throw new Error("runtime-support JSON object colon is missing");
      Object.defineProperty(result, key, { value: this.value(), enumerable: true, configurable: true, writable: true });
      this.space();
      const separator = this.text[this.position++];
      if (separator === "}") return result;
      if (separator !== ",") throw new Error("runtime-support JSON object separator is invalid");
    }
  }

  private array(): unknown[] {
    this.position++;
    const result: unknown[] = [];
    this.space();
    if (this.text[this.position] === "]") { this.position++; return result; }
    while (true) {
      result.push(this.value());
      this.space();
      const separator = this.text[this.position++];
      if (separator === "]") return result;
      if (separator !== ",") throw new Error("runtime-support JSON array separator is invalid");
    }
  }
}

function canonical_keys(path: string[]): string[] | undefined {
  const joined = path.join(".");
  if (joined === "") return ["format", "kind", "number_format", "transfer_test", "flash_directory", "timing_sets", "flash_devices", "alignment_fill"];
  if (joined === "number_format") return ["address", "end", "hexadecimal_digits", "trailing_zero_bytes", "decimal_divisors"];
  if (joined === "transfer_test") return ["address", "end", "identity", "identity_field_bytes", "record_marker", "reserved_zero_bytes", "packets", "flash_version", "flash_version_field_bytes"];
  if (/^transfer_test\.packets\.[0-9]+$/.test(joined)) return ["text", "field_bytes"];
  if (joined === "flash_directory") return ["address", "devices"];
  if (joined === "timing_sets") return ["flash_common", "flash_c2_1c", "flash_32_1b", "flash_1f_3d"];
  if (/^timing_sets\.[a-z0-9_]+$/.test(joined)) return ["address", "slots"];
  if (joined === "flash_devices") return ["generic", "bf_d4", "c2_1c", "32_1b", "1f_3d_4k", "1f_3d_128"];
  if (/^flash_devices\.[a-z0-9_]+$/.test(joined)) return ["address", "callbacks", "timing", "geometry", "wait_control", "manufacturer_id", "device_id"];
  if (/^flash_devices\.[a-z0-9_]+\.callbacks$/.test(joined)) return ["program_sector", "erase_chip", "erase_sector", "wait_write"];
  if (/^flash_devices\.[a-z0-9_]+\.geometry$/.test(joined)) return ["total_bytes", "sector_bytes", "sector_shift", "sector_count", "top_sector"];
  if (joined === "alignment_fill") return ["address", "end", "value"];
  return undefined;
}

function compact_array(path: string[]): boolean {
  const joined = path.join(".");
  return /^timing_sets\.[a-z0-9_]+\.slots\.[0-9]+$/.test(joined) ||
    /^flash_devices\.[a-z0-9_]+\.wait_control$/.test(joined);
}

function canonical_json(value: unknown, depth = 0, path: string[] = []): string {
  if (Array.isArray(value)) {
    if (value.length === 0) return "[]";
    if (compact_array(path)) {
      if (value.some((item) => typeof item === "object")) throw new Error("compact runtime-support array contains a compound value");
      return `[${value.map((item, index) => canonical_json(item, depth, [...path, String(index)])).join(", ")}]`;
    }
    const indentation = " ".repeat(depth + 2);
    return `[\n${value.map((item, index) => `${indentation}${canonical_json(item, depth + 2, [...path, String(index)])}`).join(",\n")}\n${" ".repeat(depth)}]`;
  }
  if (typeof value === "object" && value !== null) {
    const record = value as JsonObject;
    const keys = canonical_keys(path);
    if (keys === undefined || keys.length !== Object.keys(record).length || keys.some((key) => !(key in record))) {
      throw new Error(`runtime-support JSON object order is undefined at ${path.join(".") || "root"}`);
    }
    const indentation = " ".repeat(depth + 2);
    return `{\n${keys.map((key) => `${indentation}${JSON.stringify(key)}: ${canonical_json(record[key], depth + 2, [...path, key])}`).join(",\n")}\n${" ".repeat(depth)}}`;
  }
  const encoded = JSON.stringify(value);
  if (encoded === undefined) throw new Error("runtime-support JSON contains an unsupported value");
  return encoded;
}

export function parse_runtime_support_source(text: string): unknown {
  const value = new StrictJsonParser(text).parse();
  runtime_document(value);
  if (text !== `${canonical_json(value)}\n`) throw new Error("runtime-support source is not canonical JSON text");
  return value;
}

async function self_test_document(): Promise<unknown> {
  return parse_runtime_support_source(await Bun.file(new URL("../assets/data/runtime_support.json", import.meta.url)).text());
}

export async function self_test(): Promise<void> {
  const source = await self_test_document();
  const canonicalSource = `${canonical_json(source)}\n`;
  if (JSON.stringify(parse_runtime_support_source(canonicalSource)) !== JSON.stringify(source)) {
    throw new Error("runtime-support canonical parser self-test failed");
  }
  const invalidTexts = [
    canonicalSource.slice(0, -1),
    canonicalSource.replace('  "kind":', '   "kind":'),
    canonicalSource.replace('  "format": 1,', '  "format": 1,\n  "format": 1,'),
    `${canonicalSource}null\n`,
  ];
  for (const text of invalidTexts) {
    let rejected = false;
    try { parse_runtime_support_source(text); } catch { rejected = true; }
    if (!rejected) throw new Error("runtime-support canonical parser accepted invalid text");
  }
  const built = build_runtime_support_data(source);
  if (built.source_bytes !== 5_548 || built.regions.size !== 6 || built.regions.get(NUMBER_ADDRESS)?.length !== 56 ||
      built.regions.get(ALIGNMENT_ADDRESS)?.some((value) => value !== 0)) throw new Error("runtime-support builder self-test failed");
  let shortRomRejected = false;
  try { verify_runtime_support_data(new Uint8Array(ROM_SIZE - 1), source); } catch { shortRomRejected = true; }
  if (!shortRomRejected) throw new Error("runtime-support ROM-size self-test failed");
  const failures: Array<(copy: any) => void> = [
    (copy) => { copy.extra = true; },
    (copy) => { copy.number_format.hexadecimal_digits = "0123456789abcdef"; },
    (copy) => { copy.number_format.decimal_divisors.pop(); },
    (copy) => { copy.transfer_test.packets[0].field_bytes = 1; },
    (copy) => { copy.flash_directory.devices[0] = "missing"; },
    (copy) => { copy.timing_sets.flash_common.slots[0].push(0); },
    (copy) => { copy.flash_devices.generic.callbacks.program_sector = "08006e24"; },
    (copy) => { copy.flash_devices.generic.geometry.sector_count = 15; },
    (copy) => { copy.flash_devices.generic.address = copy.flash_devices.bf_d4.address; },
    (copy) => { copy.alignment_fill.value = 1; },
  ];
  for (const mutate of failures) {
    const copy = structuredClone(source);
    mutate(copy);
    let rejected = false;
    try { build_runtime_support_data(copy); } catch { rejected = true; }
    if (!rejected) throw new Error("runtime-support adversarial self-test failed");
  }
  const verifyOptions = parse_runtime_support_args(["verify", "rom.gba", "source.json"]);
  const buildOptions = parse_runtime_support_args([
    "build", "source.json", "--size", "0x38", "--output", "region.bin", "--address", "0x0800795c",
  ]);
  if (parse_runtime_support_args(["--self-test"]).command !== "self-test" ||
      verifyOptions.command !== "verify" || verifyOptions.rom !== "rom.gba" || verifyOptions.source !== "source.json" ||
      buildOptions.command !== "build" || buildOptions.address !== NUMBER_ADDRESS || buildOptions.size !== 0x38 ||
      buildOptions.output !== "region.bin") throw new Error("runtime-support argument parser self-test failed");
  const argumentFailures = [
    ["--bogus"],
    ["--self-test", "--bogus"],
    ["--self-test", "--self-test"],
    ["verify", "rom.gba"],
    ["verify", "rom.gba", "source.json", "trailing"],
    ["verify", "--bogus", "source.json"],
    ["build", "--address", "0x0800795c", "--size", "0x38", "-o", "region.bin"],
    ["build", "source.json"],
    ["build", "source.json", "--address", "0x0800795c", "--size", "0x38"],
    ["build", "source.json", "--address", "--size", "0x38", "-o", "region.bin"],
    ["build", "source.json", "--address", "0x0800795c", "--size", "-o", "region.bin"],
    ["build", "source.json", "--address", "0x0800795c", "--size", "0x38", "-o", "--bogus"],
    ["build", "source.json", "--bogus", "value", "--address", "0x0800795c", "--size", "0x38", "-o", "region.bin"],
    ["build", "source.json", "--address", "0x0800795c", "--address", "0x0800795c", "--size", "0x38", "-o", "region.bin"],
    ["build", "source.json", "--address", "0x0800795c", "--size", "0x38", "--size", "0x38", "-o", "region.bin"],
    ["build", "source.json", "--address", "0x0800795c", "--size", "0x38", "-o", "one.bin", "--output", "two.bin"],
    ["build", "source.json", "--address", "0x0800795c", "--size", "0x38", "-o", "region.bin", "trailing"],
    ["build", "source.json", "--address", "0x0800795c", "--size", "0x38", "-o", "source.json"],
  ];
  for (const args of argumentFailures) {
    let rejected = false;
    try { parse_runtime_support_args(args); } catch { rejected = true; }
    if (!rejected) throw new Error(`runtime-support argument parser accepted ${JSON.stringify(args)}`);
  }
  const sourcePath = Bun.fileURLToPath(new URL("../assets/data/runtime_support.json", import.meta.url));
  let aliasRejected = false;
  try { await refuse_source_alias(sourcePath, sourcePath); } catch { aliasRejected = true; }
  if (!aliasRejected) throw new Error("runtime-support physical-alias self-test failed");
  let directoryRejected = false;
  try { await refuse_source_alias(sourcePath, "/private/tmp"); } catch { directoryRejected = true; }
  if (!directoryRejected) throw new Error("runtime-support output-type self-test failed");
  const outputPath = `/private/tmp/runtime-support-${process.pid}-${crypto.randomUUID()}.bin`;
  try {
    const expected = built.regions.get(NUMBER_ADDRESS)!;
    await atomic_write(outputPath, expected);
    if (!Buffer.from(await Bun.file(outputPath).arrayBuffer()).equals(expected)) throw new Error("runtime-support atomic-write self-test failed");
  } finally { try { unlinkSync(outputPath); } catch (error) { if ((error as { code?: string }).code !== "ENOENT") throw error; } }
}

export type RuntimeSupportOptions =
  | { command: "help" }
  | { command: "self-test" }
  | { command: "verify"; rom: string; source: string }
  | { command: "build"; source: string; address: number; size: number; output: string };

function path_argument(value: string | undefined, label: string): string {
  if (value === undefined || value.length === 0 || value.startsWith("-")) throw new Error(`${label} is missing or is a flag`);
  return value;
}

function cli_integer(value: string | undefined, label: string): number {
  if (value === undefined || !/^(?:0x[0-9a-f]+|[0-9]+)$/.test(value)) throw new Error(`${label} is not a canonical nonnegative integer`);
  const parsed = Number(value);
  if (!Number.isSafeInteger(parsed) || parsed < 0 || parsed > 0xffffffff) throw new Error(`${label} is outside the supported range`);
  return parsed;
}

export function parse_runtime_support_args(args: string[]): RuntimeSupportOptions {
  if (args.length === 0 || (args.length === 1 && (args[0] === "-h" || args[0] === "--help"))) return { command: "help" };
  if (args.includes("--self-test")) {
    if (args.length !== 1 || args[0] !== "--self-test") throw new Error("--self-test must be used alone");
    return { command: "self-test" };
  }
  const command = args[0];
  if (command === "verify") {
    if (args.length !== 3) throw new Error("verify requires exactly ROM and SOURCE");
    return {
      command,
      rom: path_argument(args[1], "verify ROM"),
      source: path_argument(args[2], "verify source"),
    };
  }
  if (command !== "build") throw new Error(`unknown command ${command}`);
  const source = path_argument(args[1], "build source");
  let addressValue: number | undefined;
  let sizeValue: number | undefined;
  let outputValue: string | undefined;
  for (let index = 2; index < args.length; index += 2) {
    const flag = args[index];
    const value = args[index + 1];
    if (flag === "--address") {
      if (addressValue !== undefined) throw new Error("--address may only be supplied once");
      addressValue = cli_integer(value, "build address");
    } else if (flag === "--size") {
      if (sizeValue !== undefined) throw new Error("--size may only be supplied once");
      sizeValue = cli_integer(value, "build size");
    } else if (flag === "-o" || flag === "--output") {
      if (outputValue !== undefined) throw new Error("build output may only be supplied once");
      outputValue = path_argument(value, "build output");
    } else if (flag === undefined) {
      throw new Error("build has a trailing positional argument");
    } else {
      throw new Error(`unknown build option ${flag}`);
    }
  }
  if (addressValue === undefined || sizeValue === undefined || outputValue === undefined) {
    throw new Error("build requires exactly one --address, --size, and -o/--output");
  }
  if (source === outputValue) throw new Error("build output must differ from its source");
  return { command, source, address: addressValue, size: sizeValue, output: outputValue };
}

function entry_status(path: string): ReturnType<typeof lstatSync> | undefined {
  try { return lstatSync(path); }
  catch (error) {
    if ((error as { code?: string }).code === "ENOENT") return undefined;
    throw error;
  }
}

function physical_identity(path: string): string {
  const status = statSync(path, { bigint: true });
  if (!status.isFile()) throw new Error(`${path} does not resolve to a regular file`);
  return `${status.dev}:${status.ino}`;
}

async function refuse_source_alias(source: string, output: string): Promise<void> {
  if (entry_status(source) === undefined) throw new Error("build source does not exist");
  const sourceIdentity = physical_identity(source);
  const outputEntry = entry_status(output);
  if (outputEntry !== undefined && !outputEntry.isFile()) throw new Error("existing build output is not a regular file");
  if (outputEntry !== undefined && physical_identity(output) === sourceIdentity) {
    throw new Error("build output aliases its source");
  }
}

async function atomic_write(path: string, data: Uint8Array): Promise<void> {
  const temporary = `${path}.runtime-support-${process.pid}-${crypto.randomUUID()}.tmp`;
  let written = false;
  try {
    await Bun.write(temporary, data);
    written = true;
    const outputEntry = entry_status(path);
    if (outputEntry !== undefined && !outputEntry.isFile()) throw new Error("existing build output is not a regular file");
    renameSync(temporary, path);
    written = false;
  } finally {
    if (written || entry_status(temporary) !== undefined) {
      try { unlinkSync(temporary); } catch (error) { if ((error as { code?: string }).code !== "ENOENT") throw error; }
    }
  }
}

async function main(args: string[]): Promise<void> {
  const options = parse_runtime_support_args(args);
  if (options.command === "help") {
    console.log("usage: runtime_support_data.ts {--self-test|verify ROM SOURCE|build SOURCE --address ADDRESS --size SIZE -o FILE}");
    return;
  }
  if (options.command === "self-test") {
    await self_test();
    console.log("self-test=ok");
    return;
  }
  if (options.command === "verify") {
    const rom = new Uint8Array(await Bun.file(options.rom).arrayBuffer());
    const source = parse_runtime_support_source(await Bun.file(options.source).text());
    const built = verify_runtime_support_data(rom, source);
    console.log(`identical=true regions=${built.regions.size} source_bytes=${built.source_bytes}`);
    return;
  }
  if (options.command === "build") {
    await refuse_source_alias(options.source, options.output);
    const source = parse_runtime_support_source(await Bun.file(options.source).text());
    const data = build_runtime_support_component(source, options.address, options.size);
    await atomic_write(options.output, data);
    console.log(`bytes=${data.length}`);
    return;
  }
}

if (import.meta.main) await main(Bun.argv.slice(2));
