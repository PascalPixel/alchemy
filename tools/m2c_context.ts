#!/usr/bin/env bun
// m2cコンテキスト生成。src/の確定Cから関数定義と宣言を集め、
// 逆コンパイル下書きの型推定に使う一つの宣言ファイルを書き出す。
// 定義を最優先し、宣言同士は引数情報の多いものを残す。
// 出力は生成物であり、無視されるwork/以下にのみ書く。
import { mkdirSync, readFileSync, readdirSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

const PREAMBLE = `typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

`;

const DEFINITION = /^([\w][\w* ]*?)\s*(Func_08[0-9a-f]{6})\(([^)]*)\)\s*\{/m;
const DECLARATION = /^([\w][\w* ]*?)\s*(Func_08[0-9a-f]{6})\(([^)]*)\);/gm;
const DATA = /\bData_08[0-9a-f]{6}\b/g;

interface Signature {
  returnType: string;
  parameters: string;
  fromDefinition: boolean;
}

const KNOWN_TYPES = new Set(["s8", "u8", "s16", "u16", "s32", "u32", "s64", "u64", "bool", "void"]);

// 前置宣言に現れる型が全て基本型のときだけ文脈に採用する。
// srcファイル内で局所定義された構造体名は文脈側に定義が無く、
// m2cの文脈解析を全体ごと失敗させるため除外する。
function primitiveOnly(signature: Signature): boolean {
  const types = [signature.returnType, ...signature.parameters.split(",")];
  return types.every((entry) => {
    const bare = entry.replaceAll("*", " ").trim().split(/\s+/).filter(Boolean);
    const withoutName = bare.length > 1 && !KNOWN_TYPES.has(bare.at(-1)!) ? bare.slice(0, -1) : bare;
    return withoutName.length === 0 || withoutName.every((word) => KNOWN_TYPES.has(word));
  });
}

function score(signature: Signature): number {
  const params = signature.parameters.trim();
  if (params === "" || params === "void") return 1;
  return 1 + params.split(",").length;
}

export function collectContext(sourceDirectory = join(ROOT, "src")): string {
  const signatures = new Map<string, Signature>();
  const data = new Set<string>();
  for (const name of readdirSync(sourceDirectory).filter((entry) => entry.endsWith(".c")).sort()) {
    const text = readFileSync(join(sourceDirectory, name), "utf8");
    const definition = DEFINITION.exec(text);
    if (definition !== null) {
      signatures.set(definition[2], {
        returnType: definition[1].trim(),
        parameters: definition[3].trim(),
        fromDefinition: true,
      });
    }
    for (const match of text.matchAll(DECLARATION)) {
      const candidate: Signature = {
        returnType: match[1].trim(),
        parameters: match[3].trim(),
        fromDefinition: false,
      };
      const existing = signatures.get(match[2]);
      if (existing === undefined || (!existing.fromDefinition && score(candidate) > score(existing))) {
        signatures.set(match[2], candidate);
      }
    }
    for (const match of text.matchAll(DATA)) data.add(match[0]);
  }
  const lines: string[] = [];
  for (const symbol of [...data].sort()) lines.push(`extern u8 ${symbol}[];`);
  for (const [symbol, signature] of [...signatures].sort(([a], [b]) => a.localeCompare(b))) {
    if (!primitiveOnly(signature)) continue;
    const params = signature.parameters === "" ? "void" : signature.parameters;
    lines.push(`${signature.returnType} ${symbol}(${params});`);
  }
  return PREAMBLE + lines.join("\n") + "\n";
}

function main(): void {
  // 採掘済み構造体を文脈へ畳み込む。強い基底だけを機械名で型化し、
  // m2cの推測をフィールド幅で導く(第三段)。
  let structs = "";
  try {
    const mined = JSON.parse(readFileSync(join(ROOT, "work/base_structs.json"), "utf8"));
    for (const row of mined.structs.slice(0, 40)) {
      const address = row.base.replace(/[^0-9a-fx]/g, "").slice(-8);
      const name = `Work_${address}`;
      let cursor = 0;
      let body = "";
      for (const field of row.fields) {
        if (field.count < 3 || field.offset < cursor) continue;
        if (field.offset > cursor) body += `    u8 pad_${cursor.toString(16)}[${field.offset - cursor}];\n`;
        body += `    ${field.width === 1 ? "u8" : field.width === 2 ? "u16" : "s32"} field_${field.offset.toString(16)};\n`;
        cursor = field.offset + field.width;
      }
      if (body.split("\n").length >= 5) structs += `struct ${name} {\n${body}};\n`;
    }
  } catch {}
  const output = join(ROOT, "work/m2c_context.c");
  mkdirSync(dirname(output), { recursive: true });
  const text = collectContext();
  writeFileSync(output, structs + text);
  const declarations = text.split("\n").filter((line) => line.includes("Func_")).length;
  console.log(`context=${output} functions=${declarations}`);
}

if (import.meta.main) main();
