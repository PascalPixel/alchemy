import { existsSync, mkdirSync, mkdtempSync, readFileSync, readdirSync, renameSync, rmSync, writeFileSync } from "node:fs";
import { basename, dirname, extname, join } from "node:path";
import {
  CALL_VIA_BASE,
  externalSymbol,
  overlayCallViaBase,
  externalSymbolAssembly,
  sourceToAssemblyPlan,
} from "./alchemy_gcc.ts";
import { Discovery } from "./discover.ts";
import { writeCacheEntryAtomically } from "./cache_entry.ts";

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

// Content-addressed cache for already-adopted overlay C spans.
//
// assembleOverlay splices every adopted function's compiled bytes back into the
// overlay image, so without a cache each call recompiles every prior adoption in
// that overlay — the cost of one verification grows with the progress already
// made. Keying on the source bytes plus the command plan (which names the
// compiler binaries and every routed flag) makes a hit exact: a changed source,
// a changed flag route, or a changed compiler yields a different key.
const OVERLAY_C_CACHE = join(dirname(dirname(Bun.fileURLToPath(import.meta.url))), "out/cache/overlay-c");
const planStampCache = new Map<string, string>();

/**
 * A digest of this file's own source, mixed into every overlay-C cache key.
 *
 * The command plan stamps the compiler binaries and flags, but NOT the work
 * this file does after the compile — the label-word bias, the external-symbol
 * rules, the splice. Those used to be covered by a hand-bumped `-vN` string,
 * which is only ever as good as the next editor's memory.
 *
 * NEVER give this a fallback. A key that quietly stops discriminating is worse
 * than a version string someone forgets to bump, because the forgotten string
 * at least fails loudly the day somebody does remember. If the source cannot be
 * read, that is a broken installation and it must say so.
 */
let selfDigestCache: string | undefined;
export function selfDigest(): string {
  if (selfDigestCache !== undefined) return selfDigestCache;
  const path = Bun.fileURLToPath(import.meta.url);
  let source: Buffer;
  try {
    source = readFileSync(path);
  } catch (cause) {
    throw new Error(`overlay_disasm cannot read its own source at ${path} to key the cache`, { cause });
  }
  if (source.byteLength === 0) throw new Error(`overlay_disasm read an EMPTY source at ${path}; refusing to key the cache`);
  selfDigestCache = new Bun.CryptoHasher("sha256").update(source).digest("hex");
  return selfDigestCache;
}

function planStamp(commands: readonly (readonly string[])[], work: string): string {
  const identity = commands
    .map((command) => command.map((part) => (part.startsWith(work) ? "<work>" : part)).join("|"))
    .join("");
  let stamp = planStampCache.get(identity);
  if (stamp === undefined) {
    const digest = new Bun.CryptoHasher("sha256");
    digest.update(identity);
    for (const command of commands) {
      const binary = command[0];
      if (binary.startsWith("/")) {
        try {
          digest.update(readFileSync(binary));
        } catch {
          digest.update("unreadable");
        }
      }
    }
    stamp = digest.digest("hex");
    planStampCache.set(identity, stamp);
  }
  return stamp;
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
    // THE ROOT PROLOGUE DOOR, stated because it is a design constraint and not
    // a one-line bug (2026-08-01). Discovery is seeded on `push {..., lr}`
    // and on the veneer shape below, and on nothing else -- so the inventory
    // this produces STRUCTURALLY CANNOT contain a leaf, and every consumer that
    // filters on `starts_with_prologue` inherits that blindness rather than
    // adding it. Four real leaves were adopted on 2026-08-01 and not one of them
    // has an inventory row.
    //
    // It is not fixable here. A leaf offers no entry signature at all; you
    // cannot walk backwards from a `bx lr` to a function start. The addresses
    // that DO find leaves come from outside this file -- published pointer
    // words and resolved `bl` targets, which sweeps A and B scan -- so the
    // sanctioned path for a leaf is a hand-written `manual_regions` entry with
    // a ROM-measured span, and `semantic_regions_sync` now says so by name when
    // it declines one.
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

export function overlayCSources(source: string | URL): string[] {
  if (source instanceof URL || source.includes("\n") || !existsSync(source)) return [];
  // Flat layout: overlay C replacements are prefix_c_<address>.c siblings.
  const directory = dirname(source);
  const prefix = `${basename(source).replace(/overlay\.s$/, "")}c_`;
  return readdirSync(directory)
    .filter((name) => name.startsWith(prefix) && name.endsWith(".c"))
    .sort()
    .map((name) => join(directory, name));
}

export function overlayCAddresses(source: string | URL): Set<number> {
  return new Set(overlayCSources(source).map((path) => {
    const stem = basename(path, extname(path)).slice(-8);
    if (!/^[0-9a-f]{8}$/i.test(stem)) throw new Error(`overlay C filename is not an address: ${path}`);
    return Number.parseInt(stem, 16);
  }));
}

function checked(command: string[], cwd: string): string {
  const process = Bun.spawnSync(command, { cwd, stdout: "pipe", stderr: "pipe" });
  if (process.exitCode !== 0) {
    const detail = (process.stderr.toString() || process.stdout.toString()).trim();
    throw new Error(`${basename(command[0])} failed${detail ? `: ${detail}` : ""}`);
  }
  return process.stdout.toString();
}

// An overlay image is linked 0x8000 above the address it is loaded at, so every
// ABSOLUTE in-image code pointer the ROM stores is spelled `base + 0x8000`
// (HANDOVER, "In-image pointers are spelled base + 0x8000"). Hand-written exact
// C carries that bias in its literals, but the compiler cannot: when a switch
// becomes a `mov pc, rN` dispatch, gcc emits the jump table as bare `.word .LN`
// rows and the pool word holding the table's own base as one more of them, and
// `ld -Ttext` resolves them all to the load address with no bias at all.
//
// The whole difference is those words. Shifting `-Ttext` instead would move
// every instruction address too, and the `bl` identity in `externalSymbol`
// (`name_address = insn_address + 2 + true_target_offset`, HANDOVER §5b3a)
// pins the callee name to a FIXED literal -- so a global shift would move every
// call by -0x8000 while fixing the table. The bias belongs on the label
// references alone.
//
// Only a bare reference to a label DEFINED IN THIS FILE is biased. `.word` rows
// naming an external symbol, an integer, or a label DIFFERENCE (`.L5-.L2`, a
// pc-relative table) are all left alone: a difference of two in-image addresses
// is already right, and biasing it would be wrong by 0x8000.
const LOCAL_LABEL_DEFINITION = /^(\.L[A-Za-z0-9_$.]*):/;
const LOCAL_LABEL_WORD = /^(\s*\.word\s+)(\.L[A-Za-z0-9_$.]*)\s*$/;
export const OVERLAY_LINK_BIAS = 0x8000;

export function biasInImageLabelWords(assembly: string): { text: string; biased: number } {
  const lines = assembly.split("\n");
  const defined = new Set<string>();
  for (const line of lines) {
    const found = LOCAL_LABEL_DEFINITION.exec(line.trim());
    if (found !== null) defined.add(found[1]);
  }
  let biased = 0;
  const text = lines.map((line) => {
    const found = LOCAL_LABEL_WORD.exec(line);
    if (found === null || !defined.has(found[2])) return line;
    biased++;
    return `${found[1]}${found[2]} + 0x${OVERLAY_LINK_BIAS.toString(16)}`;
  }).join("\n");
  return { text, biased };
}

function compileOverlayC(
  source: string,
  work: string,
  overlay: string,
  routingSource = source,
  extraFlags: readonly string[] = [],
): { address: number; data: Buffer } {
  const callViaBase = overlayCallViaBase(overlay, source);
  const stem = basename(source, extname(source)).slice(-8);
  if (!/^[0-9a-f]{8}$/i.test(stem)) throw new Error(`overlay C filename is not an address: ${source}`);
  const address = Number.parseInt(stem, 16);
  const symbol = `Func_${stem.toLowerCase()}`;
  if (!new RegExp(`\\b${symbol}\\s*\\([^;{}]*\\)\\s*\\{`).test(readFileSync(source, "utf8"))) {
    throw new Error(`overlay C source does not define ${symbol}: ${source}`);
  }
  const assembly = join(work, `${stem}.s`);
  const object = join(work, `${stem}.o`);
  const symbolsSource = join(work, `${stem}.symbols.s`);
  const symbolsObject = join(work, `${stem}.symbols.o`);
  const elf = join(work, `${stem}.elf`);
  const binary = join(work, `${stem}.bin`);
  const plan = sourceToAssemblyPlan({
    target: "gs1",
    routingSource,
    input: source,
    output: assembly,
    preprocessedOutput: join(work, `${stem}.i`),
    flags: extraFlags.length > 0 ? { addFlags: extraFlags } : undefined,
  });
  const keyDigest = new Bun.CryptoHasher("sha256");
  // The key carries a digest of THIS FILE's own source, not a hand-bumped
  // version string.
  //
  // It was `overlay-c-v3`, bumped by hand whenever the post-compile rewriting
  // changed — correctly bumped, with an honest comment about why. The defect
  // was the MECHANISM: it held only while every future editor remembered. The
  // cost was measured, not argued — one key present in two checkouts with
  // different contents and different lengths, 160 bytes against 164, and a
  // poisoned entry that made `verify` die in `build_assets` on resource_39c.
  // Because `git checkout` does not touch `out/`, runs at three different
  // commits all shared that entry and none was a test of its commit.
  //
  // Hashing the source means any edit to the compile plan, the label-word bias
  // or the external-symbol rules moves every key automatically. It cannot rot,
  // because it derives from the thing that actually changes rather than from a
  // number describing it.
  keyDigest.update(`overlay-c:${selfDigest()}:${hex(address)}:${hex(callViaBase)}\0`);
  keyDigest.update(planStamp(plan.steps.map((step) => step.command), work));
  keyDigest.update("\0");
  keyDigest.update(readFileSync(source));
  const cached = join(OVERLAY_C_CACHE, `${keyDigest.digest("hex")}.bin`);
  // A cache hit returns before the compile steps ever run, so extra debug
  // flags (e.g. candidate_explain.ts's -dR dump) would silently produce no
  // side-effect files on a repeat invocation. Callers requesting extra flags
  // are asking for a live compile's side effects, not just its .bin result.
  if (extraFlags.length === 0 && existsSync(cached)) return { address, data: readFileSync(cached) };
  for (const step of plan.steps) checked([...step.command], work);
  writeFileSync(assembly, biasInImageLabelWords(readFileSync(assembly, "utf8")).text);
  checked(["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork", "-o", object, assembly], work);
  const undefinedSymbols = checked(["arm-none-eabi-nm", "-u", object], work)
    .split(/\r?\n/)
    .filter(Boolean)
    .map((line) => line.trim().split(/\s+/).at(-1)!);
  for (const external of undefinedSymbols) {
    if (externalSymbol(external, callViaBase) === null) throw new Error(`unsupported overlay C external symbol: ${external}`);
  }
  writeFileSync(
    symbolsSource,
    ".syntax unified\n.thumb\n" + undefinedSymbols.map((name) => externalSymbolAssembly(name, callViaBase)).join(""),
  );
  checked(["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork", "-o", symbolsObject, symbolsSource], work);
  checked(["arm-none-eabi-ld", `-Ttext=0x${hex(address)}`, "-e", symbol, "-o", elf, object, symbolsObject], work);
  checked(["arm-none-eabi-objcopy", "-O", "binary", "-j", ".text", elf, binary], work);
  const row = checked(["arm-none-eabi-nm", "-S", elf], work)
    .split(/\r?\n/)
    .find((line) => line.endsWith(` ${symbol}`));
  if (row === undefined) throw new Error(`missing linked overlay C symbol: ${symbol}`);
  const size = Number.parseInt(row.trim().split(/\s+/)[1], 16);
  const data = readFileSync(binary).subarray(0, size);
  try {
    mkdirSync(OVERLAY_C_CACHE, { recursive: true });
    writeCacheEntryAtomically(cached, data);
  } catch {
    // A cache write failure must never fail a verification.
  }
  return { address, data };
}

/**
 * Compile one prospective overlay C owner without installing a placeholder or
 * touching the canonical overlay assembly.  Exact-source iteration and
 * parallel agents need this read-only path: `overlay_adopt` deliberately
 * rehearses in place so path-routed flags see the installed filename, which
 * makes concurrent dry runs against one overlay race with each other.
 */
export function compileOverlayCandidate(
  source: string,
  work: string,
  overlay: string,
  routingSource = source,
  extraFlags: readonly string[] = [],
): { address: number; data: Buffer } {
  return compileOverlayC(source, work, overlay, routingSource, extraFlags);
}

// Address of the overlay's own `_call_via_rN` bank, or the main image's if it
// has none.
//
// An indirect call compiles to `bl _call_via_rN`, a stub that is nothing but
// `bx rN`. Every overlay carries its own bank of them, and resolving the stub to
// the main image's bank instead puts the branch a few bytes wrong -- the row
// then fails adoption while comparing clean, because the comparator never links.
// Roughly half the overlay rows that still have a semantic reference make an
// indirect call, so this is not a corner.
//
// The signature is four consecutive `bx r0 / nop … bx r3 / nop` pairs, which is
// unambiguous: a `bx` of a *different* register or a missing `nop` breaks it, and
// the run has to start at r0. Scanning the assembled image rather than a table
// keeps this derived from tracked evidence.
export function callViaBankBase(image: Uint8Array, base = OVERLAY_BASE): number | null {
  const halfword = (offset: number): number => image[offset] | (image[offset + 1] << 8);

  // The bank itself, found by its shape: `bx r0 / nop … bx r3 / nop`. A `bx` of
  // a different register or a missing `nop` breaks the run, and it has to start
  // at r0, so the match is unambiguous.
  let bank = -1;
  for (let offset = 0; offset + 16 <= image.length && bank < 0; offset += 2) {
    let matched = true;
    for (let slot = 0; slot < 4 && matched; slot += 1) {
      matched = halfword(offset + slot * 4) === (0x4700 | (slot << 3)) &&
        halfword(offset + slot * 4 + 2) === 0x46c0;
    }
    if (matched) bank = offset;
  }
  if (bank < 0) return null;

  // The bank's address in the image is not the address a `bl` to it encodes.
  // An overlay `bl` stores `target - 2` as a displacement that is not the
  // assembler's PC-relative one, so the two disagree by a per-overlay constant
  // and the assembler needs the *linking* address to reproduce the bytes.
  //
  // Recover it from any call the overlay already makes into its own bank. One
  // site gives both readings of the same branch: the stored rule says which
  // slot it reaches, which names the register, and the assembler's rule says
  // what address that slot must have for these bytes to come out. The base is
  // then the linking address of slot zero. Verified against two overlays whose
  // constants differ (resource_373 +0x60, resource_3bc +0x19e), so a fixed
  // offset would have been wrong.
  // Take the consensus over every such site rather than the first. A rehearsal
  // blanks the span it is about to replace, so the first matching site can be
  // the one inside the placeholder -- and a lone stray `bl` whose displacement
  // happens to land on a slot would otherwise decide the whole overlay.
  const votes = new Map<number, number>();
  for (let site = 0; site + 4 <= image.length; site += 2) {
    const high = halfword(site);
    const low = halfword(site + 2);
    if ((high & 0xf800) !== 0xf000 || (low & 0xf800) !== 0xf800) continue;
    const stored = (((high & 0x7ff) << 12) | ((low & 0x7ff) << 1)) << 9 >> 9;
    const slot = stored + 2 - bank;
    if (slot < 0 || slot > 13 * 4 || slot % 4 !== 0) continue;
    if (halfword(bank + slot) !== (0x4700 | ((slot / 4) << 3))) continue;
    const candidate = base + site + 4 + stored - slot;
    votes.set(candidate, (votes.get(candidate) ?? 0) + 1);
  }
  let best: number | null = null;
  let bestVotes = 0;
  for (const [candidate, count] of votes) {
    if (count > bestVotes) {
      best = candidate;
      bestVotes = count;
    }
  }
  return best;
}

/**
 * The image-offset span of every already-adopted exact-C row in an overlay.
 *
 * An exact-C row states no span anywhere in the tree: its extent is whatever
 * the compiler produces. That gap is not cosmetic -- it is what let a whole
 * class of work hide. A data-installed callback sitting behind an exact row
 * could be ruled neither inside that row's body nor outside it, so
 * `tools/overlay_published.ts` had to report UNRULED and defer to a human.
 * `compileOverlayC` already computes the answer and discards it; this exports
 * it.
 *
 * Each row compiles in its own temp directory, so asking for spans never
 * touches the image build. `compileOverlayC` caches content-addressed, so a
 * second call after a verification is nearly free.
 *
 * A row that fails to compile is OMITTED rather than guessed at, and callers
 * must read a missing start as "still unknown". Reporting a span this function
 * could not produce would recreate exactly the false confidence it exists to
 * remove.
 */
export function overlayCSpans(source: string | URL, base = OVERLAY_BASE): { start: number; end: number }[] {
  const overlay = basename(String(source)).replace(/_overlay\.s$/, "");
  const spans: { start: number; end: number }[] = [];
  for (const cSource of overlayCSources(source)) {
    const work = mkdtempSync(join(TMPDIR, "alchemy-overlay-span-"));
    try {
      const compiled = compileOverlayC(cSource, work, overlay);
      const start = compiled.address - base;
      spans.push({ start, end: start + compiled.data.length });
    } catch {
      // Omitted on purpose; see the note above.
    } finally {
      rmSync(work, { recursive: true, force: true });
    }
  }
  return spans.sort((a, b) => a.start - b.start);
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
    const result = Buffer.from(readFileSync(binary));
    const overlay = basename(String(source)).replace(/_overlay\.s$/, "");
    const occupied = new Set<number>();
    for (const cSource of overlayCSources(source)) {
      // Name the row. `compileOverlayC` runs the whole compile plan, and a
      // toolchain failure inside it surfaced as a bare `xgcc failed: …` with no
      // indication of which overlay, or which of its rows, was being built.
      let compiled: { address: number; data: Buffer };
      try {
        compiled = compileOverlayC(cSource, work, overlay);
      } catch (cause) {
        throw new Error(
          `${overlay}: ${basename(cSource)}: ${cause instanceof Error ? cause.message : String(cause)}`,
          { cause },
        );
      }
      const offset = compiled.address - base;
      if (offset < 0 || offset + compiled.data.length > result.length) {
        throw new Error(`overlay C span is outside ${source}: ${cSource}`);
      }
      for (let byte = offset; byte < offset + compiled.data.length; byte++) {
        if (occupied.has(byte)) throw new Error(`overlapping overlay C span: ${cSource}`);
        occupied.add(byte);
        if (result[byte] !== 0) throw new Error(`overlay C placeholder is not zero at 0x${hex(base + byte)}`);
      }
      compiled.data.copy(result, offset);
    }
    return result;
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

// The in-image bias rewrite is the only step between the compiler's assembly
// and the assembler, so it gets its own test rather than riding on a byte
// comparison somewhere downstream. Both directions matter: a row that must be
// biased and, just as much, the four `.word` shapes that must NOT be.
function selfTest(): void {
  const assembly = [
    "\t.text",
    ".L2:",
    "\t.word\t.L4",
    "\t.word\t.L4-.L2",
    "\t.word\tData_02000240",
    "\t.word\t265",
    "\t.word\t.Lelsewhere",
    "\t.word\t.L2",
    ".L4:",
    "\tbx\tlr",
  ].join("\n");
  const { text, biased } = biasInImageLabelWords(assembly);
  if (biased !== 2) throw new Error(`bias self-test: expected 2 biased words, got ${biased}`);
  const lines = text.split("\n");
  if (lines[2] !== "\t.word\t.L4 + 0x8000") throw new Error(`bias self-test: jump-table row not biased: ${lines[2]}`);
  if (lines[7] !== "\t.word\t.L2 + 0x8000") throw new Error(`bias self-test: table-base pool word not biased: ${lines[7]}`);
  for (const index of [3, 4, 5, 6]) {
    if (lines[index] !== assembly.split("\n")[index]) throw new Error(`bias self-test: row ${index} must be left alone: ${lines[index]}`);
  }
  if (biasInImageLabelWords("\t.word\t.L4").biased !== 0) {
    throw new Error("bias self-test: a label with no definition in the file must not be biased");
  }

  // The cache key must move with THIS FILE. Asserted on synthetic input so
  // unrelated source progress cannot break it: the digest of the real source must be a
  // stable 64-hex string, and any edit at all must produce a different one.
  const digest = selfDigest();
  if (!/^[0-9a-f]{64}$/.test(digest)) throw new Error(`self-digest is not a sha256: ${digest}`);
  if (selfDigest() !== digest) throw new Error("self-digest must be stable within a run");
  const real = readFileSync(Bun.fileURLToPath(import.meta.url));
  const edited = new Bun.CryptoHasher("sha256").update(Buffer.concat([real, Buffer.from("//\n")])).digest("hex");
  if (edited === digest) throw new Error("self-digest must change when the source changes");
  // And it must never silently degrade to a constant: an empty read is fatal.
  if (new Bun.CryptoHasher("sha256").update(Buffer.alloc(0)).digest("hex") === digest) {
    throw new Error("self-digest collapsed to the empty digest");
  }

  console.log("self-test=ok (including source-keyed cache digest)");
}

if (import.meta.main) {
  if (Bun.argv[2] === "--self-test") selfTest();
  else throw new Error("usage: overlay_disasm.ts --self-test");
}
