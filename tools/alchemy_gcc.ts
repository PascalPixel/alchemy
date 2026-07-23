#!/usr/bin/env bun
// Tool role: both; imported by tools/build_claimed.ts, tools/decomp_module.ts, tools/decompile_batch.ts (+6 more); invoked by CONVENTIONS.md, LAWS.md, PLAYBOOK.md (+1 more).
import { readFileSync, statSync } from "node:fs";
import { basename, dirname, extname, join, relative, resolve, sep } from "node:path";

export const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
// 承認済みコンパイラ束は姉妹リポジトリ PascalPixel/alchemy-gcc の dist/ に住む。
// ソースからの再構築手順は同リポジトリの build.sh、検証は下の validateBundle。
export const BUNDLE = join(ROOT, "..", "alchemy-gcc", "dist");
export const DRIVER = join(BUNDLE, "xgcc");
export const M2C = join(BUNDLE, "m2c-venv/bin/m2c");
export type CompilerTarget = "gs1" | "gs2";
export const GS2_BUNDLE = join(BUNDLE, "gs2");
export const GS2_DRIVER = join(GS2_BUNDLE, "xgcc");
export const AGBCC_BUNDLE = join(BUNDLE, "agbcc");
export const AGBCC_DRIVER = join(AGBCC_BUNDLE, "old_agbcc");
export const CFLAGS = [
  "-O2", "-mthumb", "-mthumb-interwork", "-mcpu=arm7tdmi",
  "-fno-builtin", "-nostdinc", "-ffreestanding", "-fcall-used-r4",
] as const;
export const GS2_CFLAGS = [...CFLAGS, "-ffixed-r7"] as const;
export const AGBCC_CFLAGS = [
  "-mthumb-interwork", "-O2", "-fno-builtin", "-ffreestanding",
] as const;

export function bundleForTarget(target: CompilerTarget): string {
  return target === "gs1" ? BUNDLE : GS2_BUNDLE;
}

export function driverForTarget(target: CompilerTarget): string {
  return target === "gs1" ? DRIVER : GS2_DRIVER;
}

export function cflagsForTarget(target: CompilerTarget): readonly string[] {
  return target === "gs1" ? CFLAGS : GS2_CFLAGS;
}

// These tiny command-stream handlers never use r3 in the reference code.  GCC
// reproduces their allocation exactly when the translation unit reserves r3,
// consistent with an original file-scope register assignment.  Keep this
// evidence as compiler configuration instead of expressing a register pin in C.
const FIXED_R3_SOURCES = new Set([
  "080fb6ec", "080fb700", "080fb714", "080fb728", "080fb73c",
  "080fb750", "080fb75c", "080fb768", "080fb77c",
]);
// This default-ABI palette family reserves lr while keeping long-lived loop
// constants in r8/r9; allowing lr as a general register changes the prologue
// and allocation before source shaping can affect either.
const FIXED_LR_SOURCES = new Set<string>();
// These compact hardware helpers match the reference load/store order at -O1;
// -O2 only swaps independent descriptor setup instructions.
const OPTIMIZE_O1_SOURCES = new Set(["080049e8", "08021e28"]);
const UNSCHEDULED_SOURCES = new Set([
  "08006b84",
  "08004198", "08004358", "0800439c",
  "08029274",
  "080fb714", "080fb728", "080fb73c", "080fb750", "080fb75c",
  "080fb768", "080fb77c",
]);
// This decoder has mutually exclusive switch arms that reuse the same input
// base.  Following jumps during CSE rematerializes one arm's base in r3;
// disabling that pass preserves the reference's r6 lifetime and coalescing.
const NO_CSE_FOLLOW_SOURCES = new Set(["0800f9f4"]);
// This unrolled six-item display setup shares one resource-ID base and one
// signed sentinel.  Global CSE expands them back into independent constants;
// disabling it preserves the reference's r7/r8 lifetimes.
const NO_GCSE_SOURCES = new Set(["080a45cc"]);
// This bounded-angle convergence loop only retains the reference allocation
// when GCC does not perform its expensive-expression rewrite.  The rewrite
// rotates r0-r2 and folds each signed clamp into a shorter non-reference form.
const NO_EXPENSIVE_SOURCES = new Set(["08092878"]);
// This four-step signed index loop is emitted as an ascending loop in the
// reference translation unit. Strength reduction rewrites it to a descending
// counter and changes both the allocation and loop tail.
const NO_STRENGTH_REDUCE_SOURCES = new Set(["080200cc"]);
// This effect-dispatch wrapper's explicit entry and exit blocks are preserved
// in the reference.  Sibling-call optimization merges the final cases and
// rotates the call arguments even though no source tail call is present.
const NO_OPTIMIZE_SIBLING_CALLS_SOURCES = new Set(["080b110c"]);
// These functions construct a three-word DMA descriptor whose historical
// Thumb lowering uses one writeback STMIA and restores the descriptor base.
const GROUPED_DMA_STORE_SOURCES = new Set([
  "080049e8", "08004a28", "08004a5c", "080958a8", "0809bb34",
, "08004a44", "08004838", "08004858", "08004a94"]);

// Nine sound-request entry wrappers: the entry pool load precedes the
// parameter copies only under the evidenced entry-literal-first mode with
// the second scheduling pass off (work/hand/compiler-cohort-integration.md).
const ENTRY_LITERAL_FIRST_SOURCES = new Set([
  "0800383c", "0800387c", "080038bc", "080038fc", "0800393c",
  "0800397c", "080039bc", "080039fc", "08003a3c",
]);
const HIGH_REGISTER_MOVE_FIRST_SOURCES = new Set(["0808b8e8"]);
const EARLY_FRAME_ALLOCATION_SOURCES = new Set(["0809802c"]);
// These overlay-local object constructors share one exact compiler fingerprint:
// immediately before a call, the independent r0 register copy precedes the r1
// immediate. Their common filename is not unique, so routing must use the
// canonical resource path rather than the 020000a0 stem.
const CALL_ARG0_MOVE_FIRST_OVERLAY_SOURCES = new Set([
  "assets/code/resource_380_c_020000a0.c",
  "assets/code/resource_382_c_020000a0.c",
  "assets/code/resource_385_c_020000a0.c",
  "assets/code/resource_387_c_020000a0.c",
  "assets/code/resource_38a_c_020000a0.c",
  "assets/code/resource_396_c_020000a0.c",
  "assets/code/resource_39b_c_020000a0.c",
  "assets/code/resource_39c_c_020000a0.c",
  "assets/code/resource_39d_c_020000a0.c",
  "assets/code/resource_39e_c_020000a0.c",
  "assets/code/resource_3a0_c_020000a0.c",
  "assets/code/resource_3a1_c_020000a0.c",
  "assets/code/resource_3a5_c_020000a0.c",
  "assets/code/resource_3a6_c_020000a0.c",
  "assets/code/resource_3ab_c_020000a0.c",
  "assets/code/resource_3b3_c_020000a0.c",
  "assets/code/resource_3be_c_020000a0.c",
  "assets/code/resource_3c0_c_020000a0.c",
  "assets/code/resource_3c9_c_020000a0.c",
]);
// 既定ABI(標準のr4被呼出保存)で構築された収蔵ライブラリ翻訳単位。
// 証拠: r4を保存する序文は -fcall-used-r4 の下では出ない
// (割込保護記録08006a00、バイト複写08006b84、比較08006c24、
// フラッシュ書込列08006dec、LAWS.md「第四層」)。
// 同一cc1・既定フラグ。
const DEFAULT_ABI_SOURCES = new Set([
  "08006a00", "08006b84", "08006ba8", "08006c24", "08006dec", "08007098",
  "080fadf0",
]);
// The stock m4a object linked into GS1 was built with the public old_agbcc
// compiler rather than Camelot's gcc-2.96 fork. Keep adoption source-scoped:
// every listed unit must have an independent exact-byte proof.
const AGBCC_SOURCES = new Set([
  "080fa1fc", "080fa2a0", "080fa324", "080fa350", "080fa39c", "080fa3f0",
  "080fa424", "080fa458", "080fa490", "080fa514", "080fa83c", "080fa8d4", "080fa928", "080fa9a4",
  "080fa9e0", "080fab3c", "080facf8", "080fb2cc", "080fb334", "080fb3a8", "080fb430", "080fb4a4",
  "080fb670",
  "080fb6a4",
]);
// This command-table lookup is byte-identical only when the independent
// literal load precedes its adjacent index shift, matching the stock object.
const AGBCC_LITERAL_BEFORE_SHIFT_SOURCES = new Set(["080fb670"]);
const AGBCC_OPTIMIZE_O1_SOURCES = new Set(["080fa514"]);
const AGBCC_COMMUTATIVE_COPY_CONSTANT_SOURCES = new Set(["080fa514"]);
const AGBCC_PROLOGUE_NEXT_HIGH_REG_SOURCES = new Set([
  "080fb2cc", "080fb334", "080fb3a8",
]);

function sourceStem(source: string): string {
  return basename(source, extname(source));
}

function sourceKey(source: string): string {
  return relative(ROOT, resolve(ROOT, source)).split(sep).join("/");
}

export function cflagsForSource(source: string): readonly string[] {
  const stem = sourceStem(source);
  const base = DEFAULT_ABI_SOURCES.has(stem)
    ? CFLAGS.filter((flag) => flag !== "-fcall-used-r4")
    : [...CFLAGS];
  return [
    ...base,
    ...(FIXED_R3_SOURCES.has(stem) ? ["-ffixed-r3"] : []),
    ...(FIXED_LR_SOURCES.has(stem) ? ["-ffixed-r14"] : []),
    ...(OPTIMIZE_O1_SOURCES.has(stem) ? ["-O1"] : []),
    ...(UNSCHEDULED_SOURCES.has(stem) ? ["-fno-schedule-insns", "-fno-schedule-insns2"] : []),
    ...(NO_CSE_FOLLOW_SOURCES.has(stem) ? ["-fno-cse-follow-jumps"] : []),
    ...(NO_GCSE_SOURCES.has(stem) ? ["-fno-gcse"] : []),
    ...(NO_EXPENSIVE_SOURCES.has(stem) ? ["-fno-expensive-optimizations"] : []),
    ...(NO_STRENGTH_REDUCE_SOURCES.has(stem) ? ["-fno-strength-reduce"] : []),
    ...(ENTRY_LITERAL_FIRST_SOURCES.has(stem)
      ? ["-fno-schedule-insns2", "-mthumb-entry-literal-first"] : []),
    ...(HIGH_REGISTER_MOVE_FIRST_SOURCES.has(stem) ? ["-mhigh-register-move-first"] : []),
    ...(EARLY_FRAME_ALLOCATION_SOURCES.has(stem) ? ["-mearly-frame-allocation"] : []),
    ...(NO_OPTIMIZE_SIBLING_CALLS_SOURCES.has(stem) ? ["-fno-optimize-sibling-calls"] : []),
    ...(GROUPED_DMA_STORE_SOURCES.has(stem) ? ["-mgrouped-dma-store"] : []),
    ...(CALL_ARG0_MOVE_FIRST_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-mcall-arg0-move-first"]
      : []),
  ];
}

export function cflagsForTargetSource(target: CompilerTarget, source: string): readonly string[] {
  if (target === "gs1" && AGBCC_SOURCES.has(sourceStem(source))) {
    return [
      ...AGBCC_CFLAGS,
      ...(AGBCC_OPTIMIZE_O1_SOURCES.has(sourceStem(source)) ? ["-O1"] : []),
      ...(AGBCC_LITERAL_BEFORE_SHIFT_SOURCES.has(sourceStem(source))
        ? ["-mliteral-before-shift"]
        : []),
      ...(AGBCC_COMMUTATIVE_COPY_CONSTANT_SOURCES.has(sourceStem(source))
        ? ["-mcommutative-copy-constant"]
        : []),
      ...(AGBCC_PROLOGUE_NEXT_HIGH_REG_SOURCES.has(sourceStem(source))
        ? ["-mprologue-next-high-reg"]
        : []),
    ];
  }
  return target === "gs1" ? cflagsForSource(source) : [...GS2_CFLAGS];
}

export function usesAgbccCompiler(target: CompilerTarget, source: string): boolean {
  return target === "gs1" && AGBCC_SOURCES.has(sourceStem(source));
}

const ADDRESS_SYMBOL = /^(Func|Data|Value)_([0-9a-f]{8})$/;
const CALL_VIA_SYMBOL = /^_call_via_r(1[0-3]|[0-9])$/;
const CALL_VIA_ALIAS = /^_call_via_(sl|fp|ip|sp)$/;
const CALL_VIA_BASE = 0x080072e4;
const CALL_VIA_REGISTERS: Record<string, number> = { sl: 10, fp: 11, ip: 12, sp: 13 };

export interface ExternalSymbol {
  address: number;
  thumb: boolean;
}

export function externalSymbol(name: string): ExternalSymbol | null {
  const addressed = name.match(ADDRESS_SYMBOL);
  if (addressed !== null) {
    return {
      address: Number.parseInt(addressed[2], 16),
      thumb: addressed[1] === "Func",
    };
  }
  const callVia = name.match(CALL_VIA_SYMBOL);
  if (callVia !== null) {
    return {
      address: CALL_VIA_BASE + Number.parseInt(callVia[1], 10) * 4,
      thumb: true,
    };
  }
  const callViaAlias = name.match(CALL_VIA_ALIAS);
  if (callViaAlias !== null) {
    return {
      address: CALL_VIA_BASE + CALL_VIA_REGISTERS[callViaAlias[1]] * 4,
      thumb: true,
    };
  }
  return null;
}

export function externalSymbolAssembly(name: string): string {
  const symbol = externalSymbol(name);
  if (symbol === null) throw new Error(`unsupported external symbol: ${name}`);
  return `.global ${name}\n${symbol.thumb ? ".thumb_func\n" : ""}.set ${name}, 0x${symbol.address.toString(16).padStart(8, "0")}\n`;
}

const EXPECTED: Record<CompilerTarget, Record<string, string>> = {
  gs1: {
    xgcc: "87e09e3f1e2fd711e952d6831c73099b14a059a6ca594b16c11b9a83394483ed",
    cpp: "f72b13ad2368419f2cc8c24966e030a57638bfce3f97868043196dac41e13575",
    tradcpp: "822c5cf4b38ea231f6eeeadcdf3a457518a25202c8a0a04aadf0942154e5436b",
    cc1: "dc670142e6a2daeaaf64a67fa50b391af79cb7b167305dfa559576a818a90db3",
  },
  gs2: {
    xgcc: "128520f13ff01aee64a984b1279a6e3a682a3679de44c99296064f46fb1e8ec2",
    cpp0: "b4ac7f5ff7fd74f4eca40385832fd0360d13cb5d4f0b6c8b3ead4a67d2f3d5b0",
    tradcpp0: "7698319dfea3647dace68ffb5c3dbc0fd459f3a859699acb47c669d3eb8956a3",
    cc1: "91b2a67275a100e8b6695d85ef2d82d1fd144853cbcb361ddf1d8be31858230f",
  },
};

const validated = new Set<CompilerTarget>();
let agbccValidated = false;
const AGBCC_EXPECTED = "4f7664872d10a737184fb2e0502c407c9d74505f0cff7313ba4e9083736c2207";

function outputText(value: Uint8Array): string {
  return Buffer.from(value).toString("utf8");
}

export function validateBundle(target: CompilerTarget = "gs1"): void {
  if (validated.has(target)) return;
  if (process.platform !== "darwin" || process.arch !== "arm64") {
    throw new Error("alchemy-gcc requires native arm64 macOS");
  }
  const bundle = bundleForTarget(target);
  for (const [name, expected] of Object.entries(EXPECTED[target])) {
    const path = join(bundle, name);
    let mode = 0;
    try {
      mode = statSync(path).mode;
    } catch {
      throw new Error(`alchemy-gcc ${target} bundle is missing executable ${name}`);
    }
    if ((mode & 0o111) === 0) {
      throw new Error(`alchemy-gcc ${target} bundle is missing executable ${name}`);
    }
    const actual = new Bun.CryptoHasher("sha256").update(readFileSync(path)).digest("hex");
    if (actual !== expected) {
      throw new Error(`alchemy-gcc ${target}/${name} has an unapproved digest`);
    }
  }
  // 起動確認。並列処理の前に移設済み補助実行体を一つずつ起動する。
  // 初回起動を同時に行うとmacOSの検証処理が競合する。
  const smoke = Bun.spawnSync(
    [driverForTarget(target), `-B${bundle}/`, "-S", "-x", "c", "-o", "/dev/null", "/dev/null"],
    { cwd: ROOT, stdout: "pipe", stderr: "pipe" },
  );
  if (smoke.exitCode !== 0) {
    const detail = (outputText(smoke.stderr) || outputText(smoke.stdout)).trim();
    throw new Error(`alchemy-gcc ${target} smoke compile failed: ${detail}`);
  }
  validated.add(target);
}

export function validateAgbccBundle(): void {
  if (agbccValidated) return;
  if (process.platform !== "darwin" || process.arch !== "arm64") {
    throw new Error("alchemy-gcc requires native arm64 macOS");
  }
  let mode = 0;
  try {
    mode = statSync(AGBCC_DRIVER).mode;
  } catch {
    throw new Error("alchemy-gcc agbcc bundle is missing executable old_agbcc");
  }
  if ((mode & 0o111) === 0) {
    throw new Error("alchemy-gcc agbcc bundle is missing executable old_agbcc");
  }
  const actual = new Bun.CryptoHasher("sha256").update(readFileSync(AGBCC_DRIVER)).digest("hex");
  if (actual !== AGBCC_EXPECTED) {
    throw new Error("alchemy-gcc agbcc/old_agbcc has an unapproved digest");
  }
  const smoke = Bun.spawnSync(
    [AGBCC_DRIVER, "/dev/null", "-mthumb-interwork", "-O2", "-o", "/dev/null"],
    { cwd: ROOT, stdout: "pipe", stderr: "pipe" },
  );
  if (smoke.exitCode !== 0) {
    const detail = (outputText(smoke.stderr) || outputText(smoke.stdout)).trim();
    throw new Error(`alchemy-gcc agbcc smoke compile failed: ${detail}`);
  }
  agbccValidated = true;
}

export function compilerCommand(...arguments_: Array<string | number>): string[] {
  return compilerCommandForTarget("gs1", ...arguments_);
}

export function compilerCommandForTarget(
  target: CompilerTarget,
  ...arguments_: Array<string | number>
): string[] {
  validateBundle(target);
  const bundle = bundleForTarget(target);
  return [driverForTarget(target), `-B${bundle}/`, ...arguments_.map(String)];
}

export function compilerCommandForTargetSource(
  target: CompilerTarget,
  source: string,
  ...arguments_: Array<string | number>
): string[] {
  if (!usesAgbccCompiler(target, source)) {
    return compilerCommandForTarget(target, ...arguments_);
  }
  validateAgbccBundle();
  return [
    AGBCC_DRIVER,
    ...arguments_.filter((argument) => argument !== "-S").map(String),
  ];
}

// Hot-search pipeline: invoke the approved preprocessor and cc1 directly, saving
// one driver process per candidate. These arguments are the exact subprocesses
// emitted by xgcc for CFLAGS; ordinary builds use the source-aware command API.
export function directPreprocessorCommand(input: string, output: string): string[] {
  validateBundle();
  return [
    join(BUNDLE, "cpp"), "-lang-c", "-nostdinc",
    "-D__GNUC__=2", "-D__GNUC_MINOR__=96", "-D__GNUC_PATCHLEVEL__=0",
    "-Acpu(arm)", "-Amachine(arm)", "-D__CHAR_UNSIGNED__", "-D__OPTIMIZE__",
    "-D__ARM_ARCH_4T__", "-D__APCS_32__", "-D__ARMEL__", "-D__THUMBEL__",
    "-Darm_elf", "-D__ELF__", "-Dthumb", "-D__thumb__", input, output,
  ];
}

export function directCompilerCommand(
  input: string,
  output: string,
  dumpbase: string,
  source = dumpbase,
): string[] {
  const stem = sourceStem(source);
  validateBundle();
  return [
    join(BUNDLE, "cc1"), input, "-quiet", "-dumpbase", dumpbase,
    "-mthumb", "-mthumb-interwork", "-mcpu=arm7tdmi", "-O2",
    "-fno-builtin", "-ffreestanding",
    ...(DEFAULT_ABI_SOURCES.has(stem) ? [] : ["-fcall-used-r4"]),
    ...(FIXED_R3_SOURCES.has(stem) ? ["-ffixed-r3"] : []),
    ...(FIXED_LR_SOURCES.has(stem) ? ["-ffixed-r14"] : []),
    ...(OPTIMIZE_O1_SOURCES.has(stem) ? ["-O1"] : []),
    ...(UNSCHEDULED_SOURCES.has(stem) ? ["-fno-schedule-insns", "-fno-schedule-insns2"] : []),
    ...(NO_CSE_FOLLOW_SOURCES.has(stem) ? ["-fno-cse-follow-jumps"] : []),
    ...(NO_GCSE_SOURCES.has(stem) ? ["-fno-gcse"] : []),
    ...(NO_EXPENSIVE_SOURCES.has(stem) ? ["-fno-expensive-optimizations"] : []),
    ...(NO_STRENGTH_REDUCE_SOURCES.has(stem) ? ["-fno-strength-reduce"] : []),
    ...(ENTRY_LITERAL_FIRST_SOURCES.has(stem)
      ? ["-fno-schedule-insns2", "-mthumb-entry-literal-first"] : []),
    ...(HIGH_REGISTER_MOVE_FIRST_SOURCES.has(stem) ? ["-mhigh-register-move-first"] : []),
    ...(EARLY_FRAME_ALLOCATION_SOURCES.has(stem) ? ["-mearly-frame-allocation"] : []),
    ...(NO_OPTIMIZE_SIBLING_CALLS_SOURCES.has(stem) ? ["-fno-optimize-sibling-calls"] : []),
    ...(CALL_ARG0_MOVE_FIRST_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-mcall-arg0-move-first"]
      : []),
    "-o", output,
  ];
}

export function directCompilerCommandForSource(
  source: string,
  input: string,
  output: string,
  dumpbase: string,
): string[] {
  if (!usesAgbccCompiler("gs1", source)) {
    return directCompilerCommand(input, output, dumpbase, source);
  }
  validateAgbccBundle();
  return [
    AGBCC_DRIVER, input, "-dumpbase", dumpbase,
    ...cflagsForTargetSource("gs1", source), "-o", output,
  ];
}

function selfTest(): void {
  const expected = [
    "080fa1fc", "080fa2a0", "080fa324", "080fa350", "080fa39c", "080fa3f0",
    "080fa424", "080fa458", "080fa490", "080fa514", "080fa83c", "080fa8d4", "080fa928", "080fa9a4",
    "080fa9e0", "080fab3c", "080facf8", "080fb2cc", "080fb334", "080fb3a8", "080fb430", "080fb4a4",
    "080fb670",
    "080fb6a4",
  ];
  if (JSON.stringify([...AGBCC_SOURCES].sort()) !== JSON.stringify(expected)) {
    throw new Error("old_agbcc source allowlist self-test failed");
  }
  for (const stem of expected) {
    const source = `/tmp/${stem}.c`;
    if (!usesAgbccCompiler("gs1", source) || usesAgbccCompiler("gs2", source)) {
      throw new Error(`old_agbcc target routing self-test failed for ${stem}`);
    }
    const expectedFlags = [
      ...AGBCC_CFLAGS,
      ...(stem === "080fa514" ? ["-O1", "-mcommutative-copy-constant"] : []),
      ...(stem === "080fb670" ? ["-mliteral-before-shift"] : []),
      ...(["080fb2cc", "080fb334", "080fb3a8"].includes(stem)
        ? ["-mprologue-next-high-reg"]
        : []),
    ];
    if (JSON.stringify(cflagsForTargetSource("gs1", source)) !== JSON.stringify(expectedFlags)) {
      throw new Error(`old_agbcc flags self-test failed for ${stem}`);
    }
  }
  if (usesAgbccCompiler("gs1", "/tmp/080000c0.c") ||
      JSON.stringify(cflagsForTargetSource("gs1", "/tmp/080000c0.c")) === JSON.stringify(AGBCC_CFLAGS)) {
    throw new Error("old_agbcc unrelated-source routing self-test failed");
  }
  const groupedDma = [...GROUPED_DMA_STORE_SOURCES].sort();
  if (JSON.stringify(groupedDma) !== JSON.stringify([
    "080049e8", "08004a28", "08004a5c", "080958a8", "0809bb34",
  ])) {
    throw new Error("grouped DMA source allowlist self-test failed");
  }
  for (const stem of groupedDma) {
    if (!cflagsForTargetSource("gs1", `/tmp/${stem}.c`).includes("-mgrouped-dma-store")) {
      throw new Error(`grouped DMA flags self-test failed for ${stem}`);
    }
  }
  if (cflagsForTargetSource("gs1", "/tmp/080000c0.c").includes("-mgrouped-dma-store") ||
      cflagsForTargetSource("gs2", "/tmp/080958a8.c").includes("-mgrouped-dma-store")) {
    throw new Error("grouped DMA unrelated-source routing self-test failed");
  }
  if (!cflagsForTargetSource("gs1", "/tmp/080049e8.c").includes("-O1") ||
      cflagsForTargetSource("gs1", "/tmp/08004a28.c").includes("-O1") ||
      cflagsForTargetSource("gs1", "/tmp/08004a5c.c").includes("-O1") ||
      cflagsForTargetSource("gs2", "/tmp/080049e8.c").includes("-O1")) {
    throw new Error("grouped DMA O1 routing self-test failed");
  }
  const callArg0MoveFirstOverlays = [...CALL_ARG0_MOVE_FIRST_OVERLAY_SOURCES].sort();
  if (callArg0MoveFirstOverlays.length !== 19) {
    throw new Error("overlay call-argument source allowlist self-test failed");
  }
  for (const path of callArg0MoveFirstOverlays) {
    if (!cflagsForTargetSource("gs1", join(ROOT, path)).includes("-mcall-arg0-move-first") ||
        cflagsForTargetSource("gs2", join(ROOT, path)).includes("-mcall-arg0-move-first")) {
      throw new Error(`overlay call-argument flags self-test failed for ${path}`);
    }
  }
  if (cflagsForTargetSource("gs1", "/tmp/020000a0.c").includes("-mcall-arg0-move-first") ||
      cflagsForTargetSource("gs1", join(ROOT, "assets/code/resource_381/c/020000a0.c"))
        .includes("-mcall-arg0-move-first")) {
    throw new Error("overlay call-argument unrelated-source routing self-test failed");
  }
  console.log(`self-test=ok agbcc_sources=${expected.length} grouped_dma_sources=${groupedDma.length} overlay_call_arg_sources=${callArg0MoveFirstOverlays.length}`);
}

function main(): void {
  const argument = Bun.argv[2] ?? "gs1";
  if (argument === "--self-test") {
    selfTest();
    return;
  }
  if (argument === "agbcc") {
    validateAgbccBundle();
    console.log(`alchemy-gcc=agbcc ok host=arm64 files=1 bytes=${statSync(AGBCC_DRIVER).size}`);
    return;
  }
  if (argument !== "gs1" && argument !== "gs2") {
    throw new Error(`unsupported compiler target: ${argument}`);
  }
  const target: CompilerTarget = argument;
  validateBundle(target);
  const bundle = bundleForTarget(target);
  const size = Object.keys(EXPECTED[target])
    .map((name) => statSync(join(bundle, name)).size)
    .reduce((sum, value) => sum + value, 0);
  const label = target === "gs1" ? "alchemy-gcc=ok" : "alchemy-gcc=gs2 ok";
  console.log(`${label} host=arm64 files=${Object.keys(EXPECTED[target]).length} bytes=${size}`);
}

if (import.meta.main) main();
