#!/usr/bin/env bun
// Tool role: both; imported by tools/build_claimed.ts, tools/build_full.ts, tools/match_m2c.ts (+1 more); invoked by package.json.

export type DecompTargetId = "gs1-en" | "gs2-en";
export type DecompCompilerTarget = "gs1" | "gs2";

export interface DecompTarget {
  readonly id: DecompTargetId;
  readonly rom: string;
  readonly romSize: number;
  readonly compiler: DecompCompilerTarget;
  readonly sourceDir: string;
  readonly asmDir: string;
  readonly assetManifest: string;
  readonly outputDir: string;
}

export const DEFAULT_TARGET: DecompTargetId = "gs1-en";

const TARGETS: Readonly<Record<DecompTargetId, DecompTarget>> = Object.freeze({
  "gs1-en": Object.freeze({
    id: "gs1-en",
    rom: "roms/gs1-en.gba",
    romSize: 0x00800000,
    compiler: "gs1",
    sourceDir: "src",
    asmDir: "asm",
    assetManifest: "assets/manifest.json",
    outputDir: "out",
  }),
  "gs2-en": Object.freeze({
    id: "gs2-en",
    rom: "roms/gs2-en.gba",
    romSize: 0x01000000,
    compiler: "gs2",
    sourceDir: "games/gs2/src",
    asmDir: "games/gs2/asm",
    assetManifest: "games/gs2/assets/manifest.json",
    outputDir: "out/gs2-en",
  }),
});

const TARGET_IDS = Object.freeze(Object.keys(TARGETS) as DecompTargetId[]);

export function parseDecompTarget(value: string): DecompTargetId {
  if (!Object.prototype.hasOwnProperty.call(TARGETS, value)) {
    throw new Error(
      `unsupported decomp target ${JSON.stringify(value)}; expected ${TARGET_IDS.join(" or ")}`,
    );
  }
  return value as DecompTargetId;
}

export function decompTarget(id: string = DEFAULT_TARGET): DecompTarget {
  return TARGETS[parseDecompTarget(id)];
}

function relativePath(path: string): boolean {
  return path.length > 0 &&
    !path.startsWith("/") &&
    !path.startsWith("\\") &&
    !path.split(/[\\/]/).includes("..");
}

export function selfTest(): void {
  const gs1 = decompTarget();
  const gs2 = decompTarget("gs2-en");
  if (gs1.id !== DEFAULT_TARGET || gs1.rom !== "roms/gs1-en.gba" ||
      gs1.romSize !== 0x00800000 || gs1.compiler !== "gs1") {
    throw new Error("GS1 target registry self-test failed");
  }
  if (gs2.rom !== "roms/gs2-en.gba" || gs2.romSize !== 0x01000000 ||
      gs2.compiler !== "gs2") {
    throw new Error("GS2 target registry self-test failed");
  }
  for (const target of [gs1, gs2]) {
    for (const path of [
      target.rom,
      target.sourceDir,
      target.asmDir,
      target.assetManifest,
      target.outputDir,
    ]) {
      if (!relativePath(path)) throw new Error(`${target.id} has a non-relative path`);
    }
  }
  for (const field of ["sourceDir", "asmDir", "assetManifest", "outputDir"] as const) {
    if (gs1[field] === gs2[field]) throw new Error(`${field} is not target-isolated`);
  }
  for (const invalid of ["", "gs1", "GS1-en", "gs1-en ", "gs2-ja"]) {
    try {
      parseDecompTarget(invalid);
      throw new Error(`invalid target was accepted: ${invalid}`);
    } catch (error) {
      if ((error as Error).message === `invalid target was accepted: ${invalid}`) throw error;
    }
  }
  console.log(`self-test=ok targets=${TARGET_IDS.length} default=${DEFAULT_TARGET}`);
}

if (import.meta.main) selfTest();
