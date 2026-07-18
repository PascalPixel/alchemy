export type SoundTableEntry = [header: string, player: number];

export interface SoundTableSource {
  format: number;
  fields: ["header", "player"];
  auxiliary: "copy-player";
  symbols: Record<string, string | number>;
  entries: SoundTableEntry[];
}

export interface SoundTableReport {
  entries: number;
  unique_headers: number;
  players: Record<string, number>;
  mirrored_auxiliary: true;
}

function integer(value: string | number, label: string): number {
  const result = typeof value === "string" ? Number(value) : value;
  if (!Number.isInteger(result)) throw new Error(`${label} must be an integer`);
  return result;
}

function u16(value: number, label: string): number {
  if (value < 0 || value > 0xffff) throw new Error(`${label} does not fit u16`);
  return value;
}

function address(value: string | number, label: string): number {
  const result = integer(value, label);
  if (result < 0x08000000 || result >= 0x0a000000 || result % 4 !== 0) {
    throw new Error(`${label} is not an aligned ROM address`);
  }
  return result;
}

function symbolName(index: number): string {
  return `sound_${String(index).padStart(3, "0")}`;
}

export function build_sound_table(source: SoundTableSource): [Buffer, SoundTableReport] {
  if (source.format !== 1 || !source.symbols || !Array.isArray(source.entries) ||
      !Bun.deepEquals(source.fields, ["header", "player"], true) || source.auxiliary !== "copy-player") {
    throw new Error("unsupported sound-table source");
  }
  const symbols = new Map<string, number>();
  const addresses = new Set<number>();
  for (const [name, value] of Object.entries(source.symbols)) {
    if (!/^[a-z][a-z0-9_]*$/.test(name)) throw new Error(`invalid sound symbol: ${name}`);
    const resolved = address(value, `symbol ${name}`);
    if (addresses.has(resolved)) throw new Error(`duplicate sound-header address: ${name}`);
    symbols.set(name, resolved);
    addresses.add(resolved);
  }
  const used = new Set<string>();
  const players: Record<string, number> = {};
  const built = Buffer.alloc(source.entries.length * 8);
  source.entries.forEach((entry, index) => {
    if (!Array.isArray(entry) || entry.length !== 2 || typeof entry[0] !== "string" || !symbols.has(entry[0])) {
      throw new Error(`entry ${index} references an unknown sound header`);
    }
    const player = u16(integer(entry[1], `entry ${index} player`), `entry ${index} player`);
    built.writeUInt32LE(symbols.get(entry[0])!, index * 8);
    built.writeUInt16LE(player, index * 8 + 4);
    built.writeUInt16LE(player, index * 8 + 6);
    used.add(entry[0]);
    players[String(player)] = (players[String(player)] ?? 0) + 1;
  });
  const unused = [...symbols.keys()].filter((name) => !used.has(name));
  if (unused.length !== 0) throw new Error(`unused sound symbols: ${unused.join(", ")}`);
  return [built, {
    entries: source.entries.length,
    unique_headers: symbols.size,
    players,
    mirrored_auxiliary: true,
  }];
}

export function extract_sound_table(rom: Uint8Array, start: number, count: number): SoundTableSource {
  const data = Buffer.from(rom);
  const offset = start - 0x08000000;
  if (!Number.isInteger(count) || count <= 0 || offset < 0 || offset + count * 8 > data.length) {
    throw new Error("sound table lies outside ROM");
  }
  const byAddress = new Map<number, string>();
  const symbols: Record<string, string> = {};
  const entries: SoundTableEntry[] = [];
  for (let index = 0; index < count; index++) {
    const entryOffset = offset + index * 8;
    const headerAddress = address(data.readUInt32LE(entryOffset), `entry ${index} header`);
    let name = byAddress.get(headerAddress);
    if (name === undefined) {
      const headerOffset = headerAddress - 0x08000000;
      const empty = headerOffset >= 0 && headerOffset + 4 <= data.length && data.readUInt32LE(headerOffset) === 0;
      name = empty && symbols.sound_empty === undefined ? "sound_empty" : symbolName(index);
      byAddress.set(headerAddress, name);
      symbols[name] = `0x${headerAddress.toString(16).padStart(8, "0")}`;
    }
    const player = data.readUInt16LE(entryOffset + 4);
    const auxiliary = data.readUInt16LE(entryOffset + 6);
    if (auxiliary !== player) throw new Error(`entry ${index} does not mirror its player selector`);
    entries.push([name, player]);
  }
  return { format: 1, fields: ["header", "player"], auxiliary: "copy-player", symbols, entries };
}

export function self_test(): void {
  const source: SoundTableSource = {
    format: 1,
    fields: ["header", "player"],
    auxiliary: "copy-player",
    symbols: { sound_000: "0x08000100", sound_001: "0x08000200" },
    entries: [
      ["sound_000", 1],
      ["sound_001", 7],
      ["sound_000", 1],
    ],
  };
  const [built, report] = build_sound_table(source);
  const expected = Buffer.alloc(24);
  expected.writeUInt32LE(0x08000100, 0);
  expected.writeUInt16LE(1, 4);
  expected.writeUInt16LE(1, 6);
  expected.writeUInt32LE(0x08000200, 8);
  expected.writeUInt16LE(7, 12);
  expected.writeUInt16LE(7, 14);
  expected.writeUInt32LE(0x08000100, 16);
  expected.writeUInt16LE(1, 20);
  expected.writeUInt16LE(1, 22);
  if (!built.equals(expected) || report.entries !== 3 || report.unique_headers !== 2) {
    throw new Error("sound-table self-test failed");
  }
  const extracted = extract_sound_table(Buffer.concat([Buffer.alloc(0x100), built]), 0x08000100, 3);
  const [roundTrip] = build_sound_table(extracted);
  if (!roundTrip.equals(built)) throw new Error("sound-table round-trip failed");
  console.log("self-test=ok");
}

function option(args: string[], name: string): string {
  const index = args.indexOf(name);
  if (index < 0 || index + 1 >= args.length) throw new Error(`${name} is required`);
  return args[index + 1];
}

async function main(args: string[]): Promise<void> {
  if (args.includes("--self-test")) {
    self_test();
    return;
  }
  if (args[0] !== "extract-sound-table" || !args[1]) {
    console.log("usage: music.ts extract-sound-table ROM --address ADDRESS --count COUNT -o OUTPUT");
    console.log("       music.ts --self-test");
    return;
  }
  const source = extract_sound_table(
    new Uint8Array(await Bun.file(args[1]).arrayBuffer()),
    integer(option(args, "--address"), "--address"),
    integer(option(args, "--count"), "--count"),
  );
  const output = option(args, "-o");
  await Bun.write(output, JSON.stringify(source, null, 2) + "\n");
  const [built, report] = build_sound_table(source);
  console.log(`entries=${report.entries} unique_headers=${report.unique_headers} bytes=${built.length}`);
}

if (import.meta.main) await main(Bun.argv.slice(2));
