#!/usr/bin/env bun
// Tool role: entrypoint; invoked by package.json.
import { mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { basename, dirname, isAbsolute, join, relative, resolve } from "node:path";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const ROM_BASE = 0x08000000;

type PieceKind = "incbin" | "skeleton";
type Piece = [address: number, size: number, kind: PieceKind, payload: string | null];

interface Options {
  rom: string;
  output: string;
  jobs: number;
}

function rooted(path: string): string {
  return isAbsolute(path) ? path : resolve(ROOT, path);
}

function usage(): void {
  console.log("usage: build_rom.ts [-h] [-o OUTPUT] [--jobs JOBS] [rom]");
}

function parseArgs(argv: string[]): Options {
  const options: Options = { rom: "roms/gs1-en.gba", output: "out/rom", jobs: 8 };
  let positional = false;
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "-h" || argument === "--help") {
      usage();
      process.exit(0);
    }
    const equal = argument.indexOf("=");
    const option = equal >= 0 ? argument.slice(0, equal) : argument;
    if (option === "-o" || option === "--output" || option === "--jobs") {
      const value = equal >= 0 ? argument.slice(equal + 1) : argv[++index];
      if (value === undefined) throw new Error(`${option} requires a value`);
      if (option === "--jobs") options.jobs = Number.parseInt(value, 10);
      else options.output = value;
    } else if (!argument.startsWith("-") && !positional) {
      options.rom = argument;
      positional = true;
    } else {
      throw new Error(`unrecognized argument: ${argument}`);
    }
  }
  if (!Number.isInteger(options.jobs) || options.jobs < 1) throw new Error("jobs must be positive");
  return options;
}

async function run(command: string[]): Promise<string> {
  const child = Bun.spawn(command, { cwd: ROOT, stdout: "pipe", stderr: "pipe" });
  const [stdout, stderr, exitCode] = await Promise.all([
    new Response(child.stdout).text(),
    new Response(child.stderr).text(),
    child.exited,
  ]);
  if (exitCode !== 0) throw new Error((stderr || stdout).trim() || `${basename(command[0])} failed`);
  return stdout;
}

function readJson(path: string): any {
  return JSON.parse(readFileSync(path, "utf8"));
}

async function main(): Promise<void> {
  const args = parseArgs(Bun.argv.slice(2));
  const romPath = resolve(process.cwd(), args.rom);
  const rom = readFileSync(romPath);
  const out = rooted(args.output);
  mkdirSync(out, { recursive: true });

  // 三種のソース群を構築し、それぞれ正確な領域一覧を出力する。
  await run([process.execPath, "tools/build_claimed.ts", romPath,
    "--jobs", String(args.jobs), "--output", "out/claimed"]);
  await run([process.execPath, "tools/build_asm.ts", romPath, "--output", "out/asm"]);
  await run([process.execPath, "tools/build_assets.ts", romPath, "--output", "out/assets"]);

  // 各確定領域は検証済みの正確なバイト列を供給する。
  // ソースから構築した領域別バイナリをアドレス順に配置する。
  const bytesDir = join(out, "regions");
  mkdirSync(bytesDir, { recursive: true });
  const pieces: Piece[] = [];
  const claimed = readJson(join(ROOT, "out/claimed/manifest.json"));
  const image = readFileSync(join(ROOT, "out/claimed/claimed.bin"));
  const imageBase = Number(claimed.image_base);
  for (const region of claimed.regions) {
    const address = Number(region.address);
    const size = Number(region.size);
    const start = address - imageBase;
    const blob = join(bytesDir, `${address.toString(16).padStart(8, "0")}.bin`);
    writeFileSync(blob, image.subarray(start, start + size));
    pieces.push([address, size, "incbin", relative(ROOT, blob)]);
  }
  const asmManifest = readJson(join(ROOT, "out/asm/manifest.json"));
  for (const region of asmManifest.regions) {
    pieces.push([
      Number(region.address), Number(region.size), "incbin",
      relative(ROOT, region.output),
    ]);
  }
  const assets = readJson(join(ROOT, "out/assets/manifest.json"));
  for (const region of assets.regions) {
    pieces.push([
      Number(region.address), Number(region.size), "incbin",
      relative(ROOT, region.output),
    ]);
  }

  pieces.sort((left, right) => left[0] - right[0] || left[1] - right[1]);
  // 確定済み断片間の全間隙をbaseromのincbin骨格で埋める。
  const filled: Piece[] = [];
  let cursor = ROM_BASE;
  for (const [address, size, kind, payload] of pieces) {
    if (address < cursor) {
      throw new Error(`overlapping piece at 0x${address.toString(16).padStart(8, "0")}`);
    }
    if (address > cursor) filled.push([cursor, address - cursor, "skeleton", null]);
    filled.push([address, size, kind, payload]);
    cursor = address + size;
  }
  if (cursor < ROM_BASE + rom.length) {
    filled.push([cursor, ROM_BASE + rom.length - cursor, "skeleton", null]);
  }

  // 一つのオブジェクトに各領域を個別セクションとして格納する。
  const fillLines = [".syntax unified"];
  const placements: string[] = [];
  const fillObject = join(out, "rom_fill.o");
  for (const [address, size, kind, payload] of filled) {
    const name = `.rom_${address.toString(16).padStart(8, "0")}`;
    fillLines.push(`\t.section ${name}, "ax", %progbits`);
    if (kind === "incbin") fillLines.push(`\t.incbin "${payload}"`);
    else fillLines.push(`\t.incbin "${romPath}", 0x${(address - ROM_BASE).toString(16)}, 0x${size.toString(16)}`);
    placements.push(`    KEEP(${relative(ROOT, fillObject)}(${name}))`);
  }
  const fillSource = join(out, "rom_fill.s");
  writeFileSync(fillSource, fillLines.join("\n") + "\n");
  await run([
    "arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork",
    "-o", fillObject, fillSource,
  ]);

  // 全領域を0x08000000からアドレス順に連続配置する。
  const linker = join(out, "ld_script.ld");
  writeFileSync(linker,
    "OUTPUT_ARCH(arm)\n" +
    "MEMORY { ROM (rx) : ORIGIN = 0x08000000, LENGTH = 32M }\n" +
    "SECTIONS\n{\n" +
    "  .rom 0x08000000 : SUBALIGN(1)\n  {\n" + placements.join("\n") +
    "\n  } > ROM\n" +
    "  /DISCARD/ : { *(.comment) *(.note*) *(.ARM.*) }\n" +
    "}\n",
  );
  const elf = join(out, "rom.elf");
  await run(["arm-none-eabi-ld", "-T", linker, "-o", elf, relative(ROOT, fillObject)]);
  const built = join(out, "rom.gba");
  await run(["arm-none-eabi-objcopy", "-O", "binary", elf, built]);
  const builtImage = readFileSync(built);
  if (!builtImage.equals(rom)) {
    let first: number | undefined;
    const common = Math.min(builtImage.length, rom.length);
    for (let index = 0; index < common; index++) {
      if (builtImage[index] !== rom[index]) {
        first = index;
        break;
      }
    }
    if (first !== undefined) {
      throw new Error(
        `linked ROM differs (size 0x${builtImage.length.toString(16)} vs 0x${rom.length.toString(16)}, ` +
        `first diff at 0x${(ROM_BASE + first).toString(16).padStart(8, "0")})`,
      );
    }
    throw new Error(`linked ROM size 0x${builtImage.length.toString(16)} vs 0x${rom.length.toString(16)}`);
  }
  const claimedBytes = filled
    .filter(([, , kind]) => kind !== "skeleton")
    .reduce((sum, [, size]) => sum + size, 0);
  const regions = filled.filter(([, , kind]) => kind !== "skeleton").length;
  console.log(
    `identical=True pieces=${filled.length} regions=${regions} source_bytes=${claimedBytes} ` +
    `skeleton_bytes=${rom.length - claimedBytes}`,
  );
}

if (import.meta.main) await main();
