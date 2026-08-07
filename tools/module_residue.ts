#!/usr/bin/env bun
// Measure a multi-entry translation unit against the reference image.
//
// Why this exists: candidate_show.ts and match_m2c.ts both verify a candidate
// through its entry symbol -- they locate `Func_<address>` in the linked ELF
// and require every compiled function to sit inside that symbol's extent. A
// translation unit that defines two public entries sharing one body, or that
// emits a static helper ahead of the entry, fails that check with "compiled
// function symbols differ" before a single byte is compared. 102 main-image
// owners holding 101,128 bytes are in that shape, so every sweep run so far
// has been blind to them: they report neither a residue nor a match.
//
// The region is what the build actually gates on, not the entry symbol. So
// link the unit at its region address, take the whole .text extent, and
// compare it against the reference across the region. That answers the two
// questions a sweep needs -- does it assemble to the right size, and if so how
// many halfwords differ -- without asking which symbol comes first.
//
// Prints counts and offsets, never a raw ROM span.
import { existsSync, mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { basename, dirname, join } from "node:path";
import { externalSymbolAssembly, sourceToAssemblyPlan } from "./alchemy_gcc.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const ROM_BASE = 0x08000000;

async function run(command: string[], cwd?: string): Promise<string> {
  const child = Bun.spawn(command, { cwd, stdout: "pipe", stderr: "pipe" });
  const [code, output, error] = await Promise.all([
    child.exited,
    new Response(child.stdout).text(),
    new Response(child.stderr).text(),
  ]);
  if (code !== 0) throw new Error(`${basename(command[0])} failed: ${(error || output).trim()}`);
  return output;
}

// build_asm.ts has already widened each region to its real extent, so the
// manifest is the authority on both the load address and the byte count.
export function regionOf(stem: string): { address: number; size: number } | null {
  const path = [join(ROOT, "out/full/asm/manifest.json"), join(ROOT, "out/asm/manifest.json")].find(existsSync);
  if (path === undefined) return null;
  const document = JSON.parse(readFileSync(path, "utf8")) as {
    regions?: Array<{ source: string; size: number; address: number }>;
  };
  let address: number | null = null;
  let size = 0;
  for (const region of document.regions ?? []) {
    if (basename(region.source, ".s") !== stem) continue;
    address = address === null ? region.address : Math.min(address, region.address);
    size += region.size;
  }
  return address === null ? null : { address, size };
}

export interface ModuleResidue {
  stem: string;
  address: number;
  expectedSize: number;
  actualSize: number;
  differingHalfwords: number | null;
  offsets: number[];
}

export async function measureModule(
  source: string,
  work: string,
  extraFlags: string[],
  rom: Uint8Array,
): Promise<ModuleResidue> {
  const stem = basename(source, ".c");
  const region = regionOf(stem);
  if (region === null) throw new Error(`no assembled region for ${stem}`);
  mkdirSync(work, { recursive: true });

  const assembly = join(work, `${stem}.s`);
  const object = join(work, `${stem}.o`);
  const stubsSource = join(work, `${stem}.stubs.s`);
  const stubsObject = join(work, `${stem}.stubs.o`);
  const elf = join(work, `${stem}.elf`);
  const binary = join(work, `${stem}.bin`);

  const plan = sourceToAssemblyPlan({
    target: "gs1",
    routingSource: source,
    input: join(ROOT, source),
    output: assembly,
    family: "routed",
    flags: { addFlags: extraFlags, removeFlags: [] },
    preprocessedOutput: join(work, `${stem}.i`),
  });
  for (const step of plan.steps) await run([...step.command]);
  await run(["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork", "-o", object, assembly]);

  // Every unresolved name is an address elsewhere in the image. Their exact
  // values do not matter for a size check and would only matter for the byte
  // check through a relocated field, so give each one an absolute definition
  // at its own encoded address and let the comparison report any field that
  // lands differently.
  const stubs: string[] = [];
  for (const line of (await run(["arm-none-eabi-nm", "-u", object])).split(/\r?\n/).filter(Boolean)) {
    const name = line.trim().split(/\s+/).at(-1)!;
    // externalSymbolAssembly already knows Func_/Data_/Value_ and the
    // `_call_via_rN' veneer bank, and it emits `.thumb_set' where the branch
    // type matters. Reusing it keeps this measurement on exactly the same
    // symbol resolution the real comparator uses.
    // Main-image units link against the main image's own veneer bank, which is
    // externalSymbolAssembly's default.
    stubs.push(externalSymbolAssembly(name));
  }
  writeFileSync(stubsSource, ".syntax unified\n.thumb\n" + stubs.join(""));
  await run(["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork", "-o", stubsObject, stubsSource]);
  await run([
    "arm-none-eabi-ld",
    `-Ttext=0x${region.address.toString(16).padStart(8, "0")}`,
    "-o", elf, object, stubsObject,
  ]);
  await run(["arm-none-eabi-objcopy", "-O", "binary", "-j", ".text", elf, binary]);

  // ld pads .text out to its section alignment, so a unit whose region ends on
  // an odd word boundary links two or three zero bytes longer than the region
  // it fills. Those bytes are the linker's, not the compiler's: drop them
  // rather than reporting a size mismatch the reference does not have.
  let actual = readFileSync(binary);
  if (actual.length > region.size && actual.length - region.size <= 3) {
    const tail = actual.subarray(region.size);
    // Either zero fill or the Thumb alignment nop `mov r8, r8' (0x46c0).
    const padding = tail.every((byte) => byte === 0)
      || (tail.length === 2 && tail.readUInt16LE(0) === 0x46c0);
    if (padding) actual = actual.subarray(0, region.size);
  }
  const result: ModuleResidue = {
    stem,
    address: region.address,
    expectedSize: region.size,
    actualSize: actual.length,
    differingHalfwords: null,
    offsets: [],
  };
  if (actual.length !== region.size) return result;

  const expected = Buffer.from(rom).subarray(
    region.address - ROM_BASE,
    region.address - ROM_BASE + region.size,
  );
  let differing = 0;
  for (let offset = 0; offset + 1 < region.size; offset += 2) {
    if (actual.readUInt16LE(offset) === expected.readUInt16LE(offset)) continue;
    differing += 1;
    if (result.offsets.length < 64) result.offsets.push(offset);
  }
  result.differingHalfwords = differing;
  return result;
}

async function main(): Promise<void> {
  const argv = process.argv.slice(2);
  if (argv.length === 0 || argv[0] === "-h" || argv[0] === "--help") {
    console.log("usage: module_residue.ts <candidate.c> [--work DIR] [--flags -fa,-fb] [--rom FILE]");
    process.exit(argv.length === 0 ? 2 : 0);
  }
  let source = "";
  let work = join(ROOT, "work/module-residue");
  let flags: string[] = [];
  let rom = join(ROOT, "roms/gs1-en.gba");
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--work") work = argv[++index];
    else if (argument === "--flags") flags = argv[++index].split(",").filter(Boolean);
    else if (argument === "--rom") rom = argv[++index];
    else source = argument;
  }
  const result = await measureModule(source, work, flags, readFileSync(rom));
  console.log(
    `stem=${result.stem} address=0x${result.address.toString(16)} ` +
      `actual=${result.actualSize} expected=${result.expectedSize} ` +
      `differing_halfwords=${result.differingHalfwords ?? "size-mismatch"}`,
  );
  if (result.offsets.length > 0) {
    console.log("offsets: " + result.offsets.map((o) => o.toString(16).padStart(4, "0")).join(" "));
  }
}

if (import.meta.main) await main();
