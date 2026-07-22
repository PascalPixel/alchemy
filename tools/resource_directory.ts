#!/usr/bin/env bun
// Tool role: both; imported by tools/build_assets.ts; invoked by package.json.

export const ROM_BASE = 0x08000000;
export const DIRECTORY_ADDRESS = 0x08320000;
export const DIRECTORY_SLOTS = 1000;

export type DirectorySlot =
  | "rom-base"
  | "directory-self"
  | "reserved-null"
  | `resource:${string}`
  | `alias:${string}`;

export interface ResourceDirectoryDocument {
  format: 1;
  kind: "golden-sun-resource-directory";
  address: string;
  slot_count: number;
  slots: DirectorySlot[];
}

function integer(value: unknown, label: string): number {
  const parsed = typeof value === "string" ? Number(value) : value;
  if (!Number.isSafeInteger(parsed)) throw new Error(`${label} must be an integer`);
  return parsed as number;
}

function address(value: unknown, label: string): number {
  const parsed = integer(value, label);
  if (parsed < ROM_BASE || parsed > 0xffffffff) throw new Error(`${label} is outside the ROM address space`);
  return parsed;
}

function hex(value: number): string {
  return `0x${value.toString(16).padStart(8, "0")}`;
}

function id(value: number, slots: number): string {
  const width = Math.max(3, (slots - 1).toString(16).length);
  return `0x${value.toString(16).padStart(width, "0")}`;
}

function parse_id(value: string, slots: number, label: string): number {
  if (!/^0x[0-9a-f]+$/.test(value)) throw new Error(`${label} is not a canonical hexadecimal ID`);
  const parsed = Number.parseInt(value.slice(2), 16);
  if (parsed < 0 || parsed >= slots || id(parsed, slots) !== value) throw new Error(`${label} is outside the directory`);
  return parsed;
}

function document(value: unknown): ResourceDirectoryDocument {
  if (typeof value !== "object" || value === null) throw new Error("resource directory source must be an object");
  const source = value as Partial<ResourceDirectoryDocument>;
  if (source.format !== 1 || source.kind !== "golden-sun-resource-directory") {
    throw new Error("unsupported resource directory source");
  }
  const start = address(source.address, "resource directory address");
  if (hex(start) !== source.address) throw new Error("resource directory address is not canonical hexadecimal");
  const count = integer(source.slot_count, "resource directory slot count");
  if (count < 2 || count > 0x10000) throw new Error("resource directory slot count is outside the supported range");
  if (!Array.isArray(source.slots) || source.slots.length !== count) {
    throw new Error("resource directory slots do not match slot_count");
  }
  return source as ResourceDirectoryDocument;
}

export function build_resource_directory(value: unknown): Buffer {
  const source = document(value);
  const start = address(source.address, "resource directory address");
  const result = Buffer.alloc(source.slot_count * 4);
  const resolved: Array<number | undefined> = Array(source.slot_count);
  const resolving = new Set<number>();
  const resolve_slot = (index: number): number => {
    if (resolved[index] !== undefined) return resolved[index];
    if (resolving.has(index)) throw new Error(`resource directory alias cycle at ${id(index, source.slot_count)}`);
    resolving.add(index);
    const slot = source.slots[index];
    let value: number;
    if (slot === "rom-base") value = ROM_BASE;
    else if (slot === "directory-self") value = start;
    else if (slot === "reserved-null") value = 0;
    else if (typeof slot === "string" && slot.startsWith("resource:")) {
      const target = address(slot.slice("resource:".length), `resource pointer ${id(index, source.slot_count)}`);
      if (hex(target) !== slot.slice("resource:".length)) {
        throw new Error(`resource pointer ${id(index, source.slot_count)} is not canonical hexadecimal`);
      }
      value = target;
    } else if (typeof slot === "string" && slot.startsWith("alias:")) {
      const target = parse_id(slot.slice("alias:".length), source.slot_count, `alias ${id(index, source.slot_count)}`);
      if (target >= index) throw new Error(`alias ${id(index, source.slot_count)} must reference an earlier slot`);
      value = resolve_slot(target);
      if (value === 0) throw new Error(`alias ${id(index, source.slot_count)} references a reserved slot`);
    } else throw new Error(`invalid resource directory slot ${id(index, source.slot_count)}`);
    resolving.delete(index);
    resolved[index] = value;
    return value;
  };
  for (let index = 0; index < source.slot_count; index++) result.writeUInt32LE(resolve_slot(index), index * 4);
  return result;
}

export function export_resource_directory(
  rom: Uint8Array,
  directoryAddress = DIRECTORY_ADDRESS,
  slotCount = DIRECTORY_SLOTS,
): ResourceDirectoryDocument {
  const start = address(directoryAddress, "resource directory address") - ROM_BASE;
  if (!Number.isSafeInteger(slotCount) || slotCount < 2 || slotCount > 0x10000) {
    throw new Error("resource directory slot count is outside the supported range");
  }
  if (start < 0 || start + slotCount * 4 > rom.length) throw new Error("resource directory lies outside the ROM");
  const view = new DataView(rom.buffer, rom.byteOffset, rom.byteLength);
  const first = new Map<number, number>();
  const slots: DirectorySlot[] = [];
  for (let index = 0; index < slotCount; index++) {
    const pointer = view.getUint32(start + index * 4, true);
    let slot: DirectorySlot;
    if (pointer === 0) slot = "reserved-null";
    else if (pointer === ROM_BASE && !first.has(pointer)) slot = "rom-base";
    else if (pointer === directoryAddress && !first.has(pointer)) slot = "directory-self";
    else if (first.has(pointer)) slot = `alias:${id(first.get(pointer)!, slotCount)}`;
    else {
      if (pointer < ROM_BASE || pointer >= ROM_BASE + rom.length) {
        throw new Error(`slot ${id(index, slotCount)} is not a ROM pointer`);
      }
      slot = `resource:${hex(pointer)}`;
    }
    slots.push(slot);
    if (pointer !== 0 && !first.has(pointer)) first.set(pointer, index);
  }
  const result: ResourceDirectoryDocument = {
    format: 1,
    kind: "golden-sun-resource-directory",
    address: hex(directoryAddress),
    slot_count: slotCount,
    slots,
  };
  const rebuilt = build_resource_directory(result);
  const original = Buffer.from(rom.subarray(start, start + slotCount * 4));
  if (!rebuilt.equals(original)) throw new Error("resource directory export does not round-trip");
  return result;
}

export function self_test(): void {
  const source: ResourceDirectoryDocument = {
    format: 1,
    kind: "golden-sun-resource-directory",
    address: "0x08000100",
    slot_count: 8,
    slots: [
      "rom-base",
      "directory-self",
      "resource:0x08000200",
      "alias:0x002",
      "reserved-null",
      "resource:0x08000300",
      "alias:0x005",
      "reserved-null",
    ],
  };
  const built = build_resource_directory(source);
  const expected = [ROM_BASE, 0x08000100, 0x08000200, 0x08000200, 0, 0x08000300, 0x08000300, 0];
  if (built.length !== 32 || expected.some((value, index) => built.readUInt32LE(index * 4) !== value)) {
    throw new Error("resource directory builder self-test failed");
  }
  const rom = Buffer.alloc(0x400);
  built.copy(rom, 0x100);
  const exported = export_resource_directory(rom, 0x08000100, 8);
  if (!build_resource_directory(exported).equals(built) || exported.slots[3] !== "alias:0x002" ||
      exported.slots[6] !== "alias:0x005") {
    throw new Error("resource directory exporter self-test failed");
  }
}

function option(args: string[], name: string, fallback?: string): string | undefined {
  const index = args.indexOf(name);
  return index >= 0 ? args[index + 1] : fallback;
}

function positional(args: string[]): string[] {
  const valued = new Set(["-o", "--output", "--address", "--slots"]);
  return args.filter((item, index) => !item.startsWith("-") && !valued.has(args[index - 1]));
}

async function main(args: string[]): Promise<void> {
  if (args.includes("--self-test")) {
    self_test();
    console.log("self-test=ok");
    if (args.length === 1) return;
    args = args.filter((item) => item !== "--self-test");
  }
  const [command, input] = positional(args);
  if (command === undefined || args.includes("-h") || args.includes("--help")) {
    console.log("usage: resource_directory.ts {export ROM|verify ROM SOURCE|build SOURCE} [-o FILE] [--address ADDRESS] [--slots COUNT]");
    return;
  }
  const output = option(args, "--output", option(args, "-o"));
  if (command === "export") {
    if (input === undefined || output === undefined) throw new Error("export requires ROM and --output");
    const rom = new Uint8Array(await Bun.file(input).arrayBuffer());
    const result = export_resource_directory(
      rom,
      integer(option(args, "--address", hex(DIRECTORY_ADDRESS)), "resource directory address"),
      integer(option(args, "--slots", String(DIRECTORY_SLOTS)), "resource directory slot count"),
    );
    await Bun.write(output, `${JSON.stringify(result, null, 2)}\n`);
    console.log(`slots=${result.slot_count} bytes=${result.slot_count * 4}`);
    return;
  }
  if (command === "build") {
    if (input === undefined || output === undefined) throw new Error("build requires SOURCE and --output");
    const result = build_resource_directory(await Bun.file(input).json());
    await Bun.write(output, result);
    console.log(`slots=${result.length / 4} bytes=${result.length}`);
    return;
  }
  if (command === "verify") {
    const source = positional(args)[2];
    if (input === undefined || source === undefined) throw new Error("verify requires ROM and SOURCE");
    const rom = new Uint8Array(await Bun.file(input).arrayBuffer());
    const value = await Bun.file(source).json();
    const parsed = document(value);
    const built = build_resource_directory(parsed);
    const start = address(parsed.address, "resource directory address") - ROM_BASE;
    if (start < 0 || start + built.length > rom.length ||
        !built.equals(Buffer.from(rom.subarray(start, start + built.length)))) {
      throw new Error("resource directory differs from ROM");
    }
    console.log(`identical=true slots=${parsed.slot_count} bytes=${built.length}`);
    return;
  }
  throw new Error(`unknown command: ${command}`);
}

if (import.meta.main) await main(Bun.argv.slice(2));
