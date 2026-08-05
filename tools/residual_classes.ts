#!/usr/bin/env bun
// Rank residual CLASSES by the bytes at stake, instead of ranking files.
//
// Every mechanical sweep in this project so far has been aimed at one defect
// that someone happened to decode by hand. The `bl` displacement rule is the
// clearest example: three owners sat blocked for a long time, each noted as a
// "toolchain quirk, not attempted further", until the encoding was decoded
// rather than reasoned about -- and the rule then closed dozens of owners at
// once. Finding that rule was worth more than any amount of per-file work.
//
// The bottleneck is therefore not throughput, it is knowing WHICH systematic
// defect to decode next. This tool answers that: it compiles each semantic
// owner, disassembles the compiled bytes and the reference bytes at every
// differing halfword, and buckets the mismatch by what actually differs
// (mnemonic, register, immediate, branch displacement, ...). Bucketing by
// total bytes at stake points at the next `bl`-sized rule, if one exists.
//
// It only reports. Nothing here edits a source, so it is safe to run against a
// tree while other work is in flight.
//
// FINDING, and the dead end it closes. The top class is the prologue: the
// pushed callee-saved register list differs, and it is the FIRST divergence in
// 63 of the first 200 owners. Those are the corpus's largest owners -- the top
// twelve alone carry ~19,400 differing bytes behind that one instruction --
// so it looks like the next `bl`-sized lever.
//
// It is not one. Recompiling resource_378:88c, resource_371:c28,
// resource_37f:f8c and resource_379:74 at -O1, -Os and -O3 produces a
// BYTE-IDENTICAL prologue to the routed flags in every case: the push list
// never moves. Register allocation here follows the source's variable
// structure, not the optimisation level, so no flag sweep will close these.
// Nor is there a single direction to correct: most owners push MORE
// callee-saved registers than the reference (got {r5,r6,r7,lr}, want {lr}),
// but resource_379:74 pushes FEWER (got {lr}, want {r5,lr}).
//
// The third class, immediate:ldr (20 of the first 200 owners), fails the same
// way for a different reason. A pc-relative load resolving to a different pool
// offset looks like literal-pool LAYOUT, which would be mechanical. It is not:
// across all 20 owners the compiled and reference bytes share only a minority
// of their 4-byte words (14/44, 1/9, 7/68, 106/509), so the pools hold
// different CONTENTS rather than the same words in a different order. Most of
// those owners also compile to a different size than their span. The differing
// offset is a symptom of structural divergence, not its cause.
//
// So a prologue mismatch means the reconstruction's variable structure
// genuinely differs from the original, and only per-owner rework will fix it.
// The value of this class is therefore TRIAGE, not automation: it identifies
// which large owners are structurally wrong, and -- because a divergence at
// offset 0 desynchronises the whole body -- how many bytes each one is
// actually worth. Do not re-run a flag sweep against it.
import { mkdirSync, readdirSync, readFileSync, rmSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { assembleOverlay, compileOverlayCandidate, OVERLAY_BASE } from "./overlay_disasm.ts";
import { disassembleInstructions } from "./candidate_explain.ts";
import { resolveSpan } from "./alchemist.ts";
import type { Instruction } from "./thumb_disasm.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

export interface Residual {
  /** Coarse bucket: what kind of thing differs. */
  klass: string;
  /** The two sides, for eyeballing a sample. */
  compiled: string;
  expected: string;
}

/** Operand text of a disassembled instruction, normalised for comparison. */
function operandText(instruction: Instruction | undefined): string {
  if (instruction === undefined) return "";
  return instruction.raw.replace(/^\s*\S+\s*/, "").trim();
}

function mnemonicOf(instruction: Instruction | undefined): string {
  return instruction === undefined ? "" : instruction.mnemonic;
}

/**
 * Classify one differing halfword.
 *
 * The point is to separate defects that share a CAUSE, not to describe the
 * bytes. Two owners whose `bl` displacement is wrong belong together even
 * though their byte values differ; an owner whose register allocation drifted
 * belongs somewhere else entirely.
 */
export function classify(compiled: Instruction | undefined, expected: Instruction | undefined): string {
  const compiledMnemonic = mnemonicOf(compiled);
  const expectedMnemonic = mnemonicOf(expected);
  if (compiled === undefined || expected === undefined) return "unaligned";

  if (compiledMnemonic !== expectedMnemonic) {
    // A different opcode entirely. Sub-bucket the common shapes so a big pile
    // of "different mnemonic" does not hide a single decodable rule.
    const pair = [compiledMnemonic, expectedMnemonic].sort().join("/");
    if (/^(ldr|str)/.test(compiledMnemonic) && /^(ldr|str)/.test(expectedMnemonic)) {
      return `memory-width:${pair}`;
    }
    if (/^b/.test(compiledMnemonic) && /^b/.test(expectedMnemonic)) return `branch-form:${pair}`;
    return `mnemonic:${pair}`;
  }

  const compiledOperands = operandText(compiled);
  const expectedOperands = operandText(expected);
  if (compiledOperands === expectedOperands) return "encoding-only";

  // Same instruction, different operands: the interesting case, because these
  // are usually one systematic rule rather than a rewrite.
  if (compiledMnemonic === "bl" || compiledMnemonic === "blx") return "call-displacement";
  if (/^b/.test(compiledMnemonic)) return "branch-displacement";

  const compiledRegisters = compiledOperands.match(/\b(r\d{1,2}|sp|lr|pc)\b/g)?.join(",") ?? "";
  const expectedRegisters = expectedOperands.match(/\b(r\d{1,2}|sp|lr|pc)\b/g)?.join(",") ?? "";
  if (compiledRegisters !== expectedRegisters) return `register-allocation:${compiledMnemonic}`;
  return `immediate:${compiledMnemonic}`;
}

/** Every differing halfword in one owner, already classified. */
export function residualsOf(compiled: Uint8Array, expected: Uint8Array, work: string): Residual[] {
  const compiledPath = join(work, "compiled.bin");
  const expectedPath = join(work, "expected.bin");
  writeFileSync(compiledPath, compiled);
  writeFileSync(expectedPath, expected);
  const compiledInstructions = new Map(
    disassembleInstructions(compiledPath).map((instruction) => [instruction.offset, instruction]),
  );
  const expectedInstructions = new Map(
    disassembleInstructions(expectedPath).map((instruction) => [instruction.offset, instruction]),
  );

  const residuals: Residual[] = [];
  const limit = Math.min(compiled.length, expected.length);
  const seen = new Set<number>();
  for (let byte = 0; byte < limit; byte++) {
    if (compiled[byte] === expected[byte]) continue;
    const halfword = byte & ~1;
    if (seen.has(halfword)) continue;
    seen.add(halfword);
    // Attribute the halfword to the instruction that CONTAINS it: a 32-bit
    // Thumb encoding differing in its second halfword is still one defect in
    // one instruction, and counting it at the halfword address would report an
    // instruction that does not exist there.
    const owning = (map: Map<number, Instruction>): Instruction | undefined =>
      map.get(halfword) ?? map.get(halfword - 2);
    const compiledInstruction = owning(compiledInstructions);
    const expectedInstruction = owning(expectedInstructions);
    residuals.push({
      klass: classify(compiledInstruction, expectedInstruction),
      compiled: compiledInstruction?.raw.trim() ?? "(none)",
      expected: expectedInstruction?.raw.trim() ?? "(none)",
    });
  }
  return residuals;
}

interface Bucket {
  halfwords: number;
  owners: Set<string>;
  samples: Residual[];
}

async function main(): Promise<void> {
  if (Bun.argv.includes("--self-test")) return selfTest();
  const limitArgument = Bun.argv.find((argument) => argument.startsWith("--limit="));
  const limit = limitArgument !== undefined ? Number(limitArgument.split("=")[1]) : Infinity;

  const work = join(ROOT, "out", "residual-classes");
  rmSync(work, { recursive: true, force: true });
  mkdirSync(work, { recursive: true });

  const overlayImages = new Map<string, Uint8Array>();
  const buckets = new Map<string, Bucket>();
  let owners = 0;
  let skipped = 0;
  let cascade = 0;

  const names = readdirSync(join(ROOT, "semantic"))
    .filter((name) => /^resource_[0-9a-f]+_c_[0-9a-f]{8}\.c$/i.test(name))
    .sort();

  for (const name of names) {
    if (owners >= limit) break;
    const matched = /^(resource_[0-9a-f]+)_c_([0-9a-f]{8})\.c$/i.exec(name)!;
    const overlay = matched[1];
    const offset = Number.parseInt(matched[2], 16) - OVERLAY_BASE;
    const span = resolveSpan(overlay, offset);
    if (span === undefined) { skipped++; continue; }

    let image = overlayImages.get(overlay);
    if (image === undefined) {
      try {
        image = assembleOverlay(join(ROOT, "assets", "code", `${overlay}_overlay.s`), OVERLAY_BASE);
      } catch { skipped++; continue; }
      overlayImages.set(overlay, image);
    }
    const expected = image.subarray(offset, offset + span);

    const source = join(ROOT, "semantic", name);
    let compiled: Uint8Array;
    try {
      compiled = compileOverlayCandidate(source, work, overlay).data;
    } catch { skipped++; continue; }

    let residuals: Residual[];
    try {
      residuals = residualsOf(compiled, expected, work);
    } catch { skipped++; continue; }
    if (residuals.length === 0) continue;
    owners++;

    const id = `${overlay}:${offset.toString(16)}`;
    // Only the FIRST divergence is a root cause. Once one instruction differs
    // in width or position, the disassembly desynchronises and every later
    // instruction mismatches too -- so counting all of them ranks cascade, not
    // causes. The first cut of this tool did exactly that and put a spurious
    // "bl vs movs" class on top at 18%, which is just the shifted tail of some
    // earlier defect. What each owner needs fixed first is one instruction.
    const first = residuals[0];
    let bucket = buckets.get(first.klass);
    if (bucket === undefined) {
      bucket = { halfwords: 0, owners: new Set(), samples: [] };
      buckets.set(first.klass, bucket);
    }
    bucket.halfwords++;
    bucket.owners.add(id);
    if (bucket.samples.length < 3) bucket.samples.push(first);
    cascade += residuals.length - 1;
  }

  const ranked = [...buckets.entries()].sort((left, right) => right[1].halfwords - left[1].halfwords);
  const totalHalfwords = ranked.reduce((sum, [, bucket]) => sum + bucket.halfwords, 0);
  console.log(`owners_with_residuals=${owners} skipped=${skipped} root_cause_classes=${ranked.length}`);
  console.log(`first_divergences=${totalHalfwords} cascaded_halfwords_not_counted=${cascade}`);
  console.log("");
  for (const [klass, bucket] of ranked.slice(0, 25)) {
    const share = ((bucket.halfwords / totalHalfwords) * 100).toFixed(1);
    console.log(`${String(bucket.halfwords).padStart(5)} owners  ${share.padStart(5)}%  ${klass}`);
    for (const sample of bucket.samples) {
      console.log(`           got ${sample.compiled}   want ${sample.expected}`);
    }
  }
  console.log("");
  console.log("each owner contributes ONE first divergence, so this ranks root causes, not cascade;");
  console.log("a class holding many owners is a candidate for one decodable rule.");
}

function selfTest(): void {
  const instruction = (offset: number, mnemonic: string, rest: string): Instruction =>
    ({ offset, mnemonic, operands: [], raw: `${mnemonic}\t${rest}` });

  // A differing bl is the call-displacement rule, not a register problem.
  const call = classify(instruction(0, "bl", "0x2000030"), instruction(0, "bl", "0x2000040"));
  if (call !== "call-displacement") throw new Error(`bl misclassified: ${call}`);

  // Same mnemonic and same operands: the bytes differ only in encoding.
  const same = classify(instruction(0, "movs", "r0, #1"), instruction(0, "movs", "r0, #1"));
  if (same !== "encoding-only") throw new Error(`identical operands misclassified: ${same}`);

  // A different register is allocation drift; a different literal is not.
  const register = classify(instruction(0, "movs", "r0, #1"), instruction(0, "movs", "r3, #1"));
  if (register !== "register-allocation:movs") throw new Error(`register misclassified: ${register}`);
  const immediate = classify(instruction(0, "movs", "r0, #1"), instruction(0, "movs", "r0, #2"));
  if (immediate !== "immediate:movs") throw new Error(`immediate misclassified: ${immediate}`);

  // Load vs store of differing width is its own bucket, and is order-stable so
  // the two directions of the same defect land together.
  const width = classify(instruction(0, "ldrb", "r0, [r1]"), instruction(0, "ldr", "r0, [r1]"));
  if (width !== "memory-width:ldr/ldrb") throw new Error(`width misclassified: ${width}`);
  const reversed = classify(instruction(0, "ldr", "r0, [r1]"), instruction(0, "ldrb", "r0, [r1]"));
  if (reversed !== width) throw new Error(`bucket is order-dependent: ${reversed} vs ${width}`);

  // A halfword with no instruction on one side cannot be compared.
  if (classify(undefined, instruction(0, "movs", "r0, #1")) !== "unaligned") {
    throw new Error("missing instruction should be unaligned");
  }
  console.log("self-test=ok tool=residual_classes");
}

if (import.meta.main) await main();
