import { existsSync, mkdtempSync, readFileSync, rmSync, writeFileSync } from "node:fs";
import { join } from "node:path";
import { Discovery } from "./discover.ts";

export const ROM_BASE = 0x08000000;
export const OVERLAY_BASE = 0x02000000;
const ROW = /\s*([0-9a-f]+):\t((?:[0-9a-f]{2,4} )+)\s*(\S.*)/;
const TARGET = /\b(b|bl|beq|bne|bcs|bcc|bmi|bpl|bvs|bvc|bhi|bls|bge|blt|bgt|ble|bhs|blo)(\.[nw])?\s+0x([0-9a-f]+)\b/;
const ERRLINE = /:(\d+): Error:/;
const TMPDIR = Bun.env.TMPDIR ?? Bun.env.TMP ?? Bun.env.TEMP ?? "/tmp";

type Row = [width: number, mnemonic: string];
type BodyRow = [address: number, kind: "label" | "code" | "data", line: string];

function hex(value: number, width = 8): string {
  return value.toString(16).padStart(width, "0");
}

function objdumpRows(data: Uint8Array, base: number): Map<number, Row> {
  const work = mkdtempSync(join(TMPDIR, "alchemy-overlay-objdump-"));
  try {
    const binary = join(work, "overlay.bin");
    writeFileSync(binary, data);
    const process = Bun.spawnSync(["arm-none-eabi-objdump", "-D", "-b", "binary", "-marmv4t", "-Mforce-thumb", `--adjust-vma=0x${hex(base)}`, binary], { stdout: "pipe", stderr: "pipe" });
    if (process.exitCode !== 0) throw new Error(process.stderr.toString().trim());
    const rows = new Map<number, Row>();
    for (const line of process.stdout.toString().split(/\r?\n/)) {
      const match = ROW.exec(line);
      if (!match) continue;
      rows.set(Number.parseInt(match[1], 16), [2 * match[2].trim().split(/\s+/).length, match[3].split(";")[0].trim()]);
    }
    return rows;
  } finally {
    rmSync(work, { recursive: true, force: true });
  }
}

function reachable(input: Uint8Array, base: number): Map<number, number> {
  const data = Buffer.from(input);
  const discovery = new Discovery(data, base);
  for (let offset = 0; offset < data.length - 1; offset += 2) {
    if ((data.readUInt16LE(offset) & 0xff00) === 0xb500) discovery.add_seed(base + offset, "thumb", "prologue");
  }
  for (let offset = 0; offset < data.length - 8; offset += 2) {
    const word = data.readUInt16LE(offset);
    const following = data.readUInt16LE(offset + 2);
    if (word >= 0x4800 && word <= 0x48ff && following >= 0x4700 && following <= 0x47ff) {
      const target = data.readUInt32LE(offset + 4);
      if (base <= target && target < base + data.length) discovery.add_seed(target & ~1, "thumb", "stub");
    }
  }
  for (const entry of [...discovery.functions.keys()]) {
    try { discovery.walk_function(entry); } catch {}
  }
  return new Map([...discovery.instructions].filter(([address]) => base <= address && address < base + data.length).map(([address, info]) => [address, info.size]));
}

function sourceText(source: string | URL): string {
  if (source instanceof URL) return readFileSync(source, "utf8");
  if (!source.includes("\n") && existsSync(source)) return readFileSync(source, "utf8");
  return source;
}

export function assembleOverlay(source: string | URL, base = OVERLAY_BASE): Buffer {
  const work = mkdtempSync(join(TMPDIR, "alchemy-overlay-"));
  try {
    const assembly = join(work, "o.s");
    const object = join(work, "o.o");
    const elf = join(work, "o.elf");
    const binary = join(work, "o.bin");
    writeFileSync(assembly, sourceText(source));
    const assembled = Bun.spawnSync(["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork", "-o", object, assembly], { stdout: "pipe", stderr: "pipe" });
    if (assembled.exitCode !== 0) throw new Error(assembled.stderr.toString().trim());
    const linked = Bun.spawnSync(["arm-none-eabi-ld", `-Ttext=0x${hex(base)}`, "-o", elf, object], { stdout: "pipe", stderr: "pipe" });
    if (linked.exitCode !== 0) throw new Error(linked.stderr.toString().trim());
    const copied = Bun.spawnSync(["arm-none-eabi-objcopy", "-O", "binary", "-j", ".text", elf, binary], { stdout: "pipe", stderr: "pipe" });
    if (copied.exitCode !== 0) throw new Error(copied.stderr.toString().trim());
    return readFileSync(binary);
  } finally {
    rmSync(work, { recursive: true, force: true });
  }
}
export const assemble_overlay = assembleOverlay;

export function buildOverlaySource(input: Uint8Array, base = OVERLAY_BASE): string {
  const decoded = Buffer.from(input);
  if (decoded.length % 2) throw new Error("overlay has an odd byte length");
  const rows = objdumpRows(decoded, base);
  const instructions = reachable(decoded, base);
  const covered = new Set<number>();
  for (const [address, size] of instructions) for (let byte = address; byte < address + size; byte++) covered.add(byte);
  const labels = new Map<number, string>();
  for (const address of [...instructions.keys()].sort((a, b) => a - b)) {
    const row = rows.get(address);
    if (!row) continue;
    const found = TARGET.exec(row[1]);
    if (found) {
      const target = Number.parseInt(found[3], 16);
      if (instructions.has(target) && !labels.has(target)) labels.set(target, `.L_${hex(target)}`);
    }
  }
  let externals = new Map<number, string>();
  const replacement = (target: number): string => {
    const local = labels.get(target);
    if (local) return local;
    const symbol = `sub_${hex(target)}`;
    externals.set(target, symbol);
    return symbol;
  };
  const retarget = (mnemonic: string): string => {
    const found = TARGET.exec(mnemonic);
    if (!found) return mnemonic;
    return mnemonic.slice(0, mnemonic.lastIndexOf("0x")) + replacement(Number.parseInt(found[3], 16));
  };
  const end = base + decoded.length;
  const raw = new Set<number>();
  const emit = (): BodyRow[] => {
    const body: BodyRow[] = [];
    let cursor = base;
    while (cursor < end) {
      const local = labels.get(cursor);
      if (local) body.push([cursor, "label", `${local}:`]);
      const row = rows.get(cursor);
      if (!raw.has(cursor) && instructions.has(cursor) && row && Array.from({ length: row[0] }, (_, byte) => cursor + byte).every((byte) => covered.has(byte))) {
        body.push([cursor, "code", `\t${retarget(row[1])}`]);
        cursor += row[0];
        continue;
      }
      const aligned = cursor % 4 === 0 && cursor + 4 <= end && Array.from({ length: 4 }, (_, byte) => cursor + byte).every((byte) => !covered.has(byte)) && !instructions.has(cursor + 4);
      if (aligned) {
        body.push([cursor, "data", `\t.4byte 0x${hex(decoded.readUInt32LE(cursor - base))}`]);
        cursor += 4;
      } else {
        body.push([cursor, "data", `\t.2byte 0x${hex(decoded.readUInt16LE(cursor - base), 4)}`]);
        cursor += 2;
      }
    }
    return body;
  };
  for (let attempt = 0; attempt < decoded.length; attempt++) {
    externals = new Map();
    const body = emit();
    const head = [".syntax unified", "\t.thumb"];
    for (const [target, name] of [...externals].sort((a, b) => a[0] - b[0])) head.push(`\t.set ${name}, 0x${hex(target)}`);
    head.push(`\t.global Overlay_${hex(base)}`, `Overlay_${hex(base)}:`);
    const text = `${[...head, ...body.map((row) => row[2])].join("\n")}\n`;
    let built: Buffer;
    try {
      built = assembleOverlay(text, base);
    } catch (error) {
      const match = ERRLINE.exec(String(error));
      if (!match) throw error;
      const index = Number(match[1]) - head.length - 1;
      if (index < 0 || index >= body.length || raw.has(body[index][0])) throw new Error(`cannot reconstruct near line ${match[1]}`);
      raw.add(body[index][0]);
      continue;
    }
    if (built.equals(decoded)) return text;
    let difference = 0;
    while (difference < Math.min(built.length, decoded.length) && built[difference] === decoded[difference]) difference++;
    const owner = base + difference;
    let forced = false;
    for (const [address, size] of instructions) {
      if (address <= owner && owner < address + size && !raw.has(address)) {
        raw.add(address);
        forced = true;
        break;
      }
    }
    if (!forced) throw new Error(`irreducible mismatch at 0x${hex(owner)}`);
  }
  throw new Error("overlay reconstruction did not converge");
}
export const build_overlay_source = buildOverlaySource;
