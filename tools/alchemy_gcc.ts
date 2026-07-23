#!/usr/bin/env bun
// Tool role: both; imported by tools/build_claimed.ts, tools/decomp_module.ts, tools/decompile_batch.ts (+6 more); invoked by ALCHEMY_GCC.md, LAWS.md, PLAYBOOK.md.
import { readFileSync, statSync } from "node:fs";
import { basename, dirname, extname, join } from "node:path";

export const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
// 承認済みコンパイラ束は姉妹リポジトリ PascalPixel/alchemy-gcc の dist/ に住む。
// ソースからの再構築手順は同リポジトリの build.sh、検証は下の validateBundle。
export const BUNDLE = join(ROOT, "..", "alchemy-gcc", "dist");
export const DRIVER = join(BUNDLE, "xgcc");
export const M2C = join(BUNDLE, "m2c-venv/bin/m2c");
export const CFLAGS = [
  "-O2", "-mthumb", "-mthumb-interwork", "-mcpu=arm7tdmi",
  "-fno-builtin", "-nostdinc", "-ffreestanding", "-fcall-used-r4",
] as const;

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
const FIXED_LR_SOURCES = new Set(["080fb2cc"]);
// This hardware setup helper matches the reference's compact load/store order
// at -O1; -O2 only swaps two independent setup instructions.
const OPTIMIZE_O1_SOURCES = new Set(["08021e28"]);
const UNSCHEDULED_SOURCES = new Set([
  "08006b84",
  "08004198", "08004358", "0800439c",
  "08029274",
  "080fb2cc",
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
// 既定ABI(標準のr4被呼出保存)で構築された収蔵ライブラリ翻訳単位。
// 証拠: r4を保存する序文は -fcall-used-r4 の下では出ない
// (割込保護記録08006a00、バイト複写08006b84、比較08006c24、
// フラッシュ書込列08006dec、LAWS.md「第四層」)。
// 同一cc1・既定フラグ。
const DEFAULT_ABI_SOURCES = new Set([
  "08006a00", "08006b84", "08006c24", "08006dec", "080fadf0", "080fb2cc",
]);

function sourceStem(source: string): string {
  return basename(source, extname(source));
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
  ];
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

const EXPECTED: Record<string, string> = {
  xgcc: "8087fb1911b00aafe8ba9dc1530ca84a98774206f24d95b3ac8a8f01bf8a6eb6",
  cpp: "28621e18b2a6b663e1ea6e47750ca0133483f4287bc271265cc7e2fcfa69a2eb",
  tradcpp: "88dae1204f5e928c7de003fd25263e91a18802f8ffde48b6f076e2ee1ea3e59a",
  cc1: "8bded78ec14221dc38a8a3142da2b22a15f0633eeeaaa04ac4de6a0522f612a5",
};

let validated = false;

function outputText(value: Uint8Array): string {
  return Buffer.from(value).toString("utf8");
}

export function validateBundle(): void {
  if (validated) return;
  if (process.platform !== "darwin" || process.arch !== "arm64") {
    throw new Error("alchemy-gcc requires native arm64 macOS");
  }
  for (const [name, expected] of Object.entries(EXPECTED)) {
    const path = join(BUNDLE, name);
    let mode = 0;
    try {
      mode = statSync(path).mode;
    } catch {
      throw new Error(`alchemy-gcc is missing executable ${name}`);
    }
    if ((mode & 0o111) === 0) {
      throw new Error(`alchemy-gcc is missing executable ${name}`);
    }
    const actual = new Bun.CryptoHasher("sha256").update(readFileSync(path)).digest("hex");
    if (actual !== expected) {
      throw new Error(`alchemy-gcc/${name} has an unapproved digest`);
    }
  }
  // 起動確認。並列処理の前に移設済み補助実行体を一つずつ起動する。
  // 初回起動を同時に行うとmacOSの検証処理が競合する。
  const smoke = Bun.spawnSync(
    [DRIVER, `-B${BUNDLE}/`, "-S", "-x", "c", "-o", "/dev/null", "/dev/null"],
    { cwd: ROOT, stdout: "pipe", stderr: "pipe" },
  );
  if (smoke.exitCode !== 0) {
    const detail = (outputText(smoke.stderr) || outputText(smoke.stdout)).trim();
    throw new Error(`alchemy-gcc smoke compile failed: ${detail}`);
  }
  validated = true;
}

export function compilerCommand(...arguments_: Array<string | number>): string[] {
  validateBundle();
  return [DRIVER, `-B${BUNDLE}/`, ...arguments_.map(String)];
}

// Hot-search pipeline: invoke the approved preprocessor and cc1 directly, saving
// one driver process per candidate. These arguments are the exact subprocesses
// emitted by xgcc for CFLAGS; ordinary builds continue to use compilerCommand.
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

export function directCompilerCommand(input: string, output: string, dumpbase: string): string[] {
  const stem = sourceStem(dumpbase);
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
    "-o", output,
  ];
}

function main(): void {
  validateBundle();
  const size = Object.keys(EXPECTED)
    .map((name) => statSync(join(BUNDLE, name)).size)
    .reduce((sum, value) => sum + value, 0);
  console.log(`alchemy-gcc=ok host=arm64 files=${Object.keys(EXPECTED).length} bytes=${size}`);
}

if (import.meta.main) main();
