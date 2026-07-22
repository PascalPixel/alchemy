#!/usr/bin/env bun
// Tool role: entrypoint; invoked by tools/decomp_overnight.ts.
// 慣用句採掘。一致済みCの各文を錨（呼出し先やフィールド接近の形）で
// 索引し、証明済みの言い回し雛形を辞書として書き出す。雛形の局所
// 変数は位置プレースホルダへ抽象化し、他関数の同型文へ移植できる。
// 一致が増えるほど辞書が濃くなり、置換探索の提案が賢くなる。
import { mkdirSync, readFileSync, readdirSync, writeFileSync } from "node:fs";
import { basename, dirname, join } from "node:path";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const OUTPUT = join(ROOT, "out/permute1/idioms.json");

// 文の錨: 呼出し先シンボル、またはフィールド接近の型と変位の組。
export function anchorsOf(statement: string): string[] {
  const anchors: string[] = [];
  for (const match of statement.matchAll(/Func_08[0-9a-f]{6}/g)) anchors.push(`call:${match[0]}`);
  for (const match of statement.matchAll(/M2C_FIELD\([^,]+, (\w+) \*, (0x[0-9A-Fa-f]+|\d+)\)/g)) {
    anchors.push(`field:${match[1]}:${Number(match[2]).toString(16)}`);
  }
  for (const match of statement.matchAll(/\*\((volatile )?(\w+) \*\)(0x[0-9A-Fa-f]{8})/g)) {
    anchors.push(`abs:${match[2]}:${match[3].toLowerCase()}`);
  }
  return anchors;
}

// 局所変数名を出現順のプレースホルダへ置き換えた雛形を返す。
export function templateOf(statement: string): { template: string; slots: number } {
  const seen = new Map<string, string>();
  const template = statement.replace(/\b(?!Func_|Data_|M2C_FIELD\b|volatile\b|return\b|if\b|while\b|do\b|else\b)([a-z_]\w*)\b/g, (name) => {
    if (/^(s8|u8|s16|u16|s32|u32|void|do|if|else|while|return)$/.test(name)) return name;
    if (!seen.has(name)) seen.set(name, `%v${seen.size}`);
    return seen.get(name)!;
  });
  return { template, slots: seen.size };
}

function main(): void {
  const dictionary = new Map<string, Set<string>>();
  let statements = 0;
  for (const name of readdirSync(join(ROOT, "src")).filter((entry) => entry.endsWith(".c"))) {
    const text = readFileSync(join(ROOT, "src", name), "utf8");
    for (const raw of text.split("\n")) {
      const statement = raw.trim();
      if (!statement.endsWith(";") || statement.startsWith("typedef") || statement.startsWith("#") ||
          statement.startsWith("extern") || /^\w[\w* ]*Func_08[0-9a-f]{6}\(/.test(statement)) continue;
      const anchors = anchorsOf(statement);
      if (anchors.length === 0) continue;
      const { template } = templateOf(statement);
      statements++;
      for (const anchor of anchors) {
        if (!dictionary.has(anchor)) dictionary.set(anchor, new Set());
        dictionary.get(anchor)!.add(template);
      }
    }
  }
  const serializable: Record<string, string[]> = {};
  for (const [anchor, templates] of dictionary) serializable[anchor] = [...templates];
  mkdirSync(dirname(OUTPUT), { recursive: true });
  writeFileSync(OUTPUT, JSON.stringify({ format: 1, statements, anchors: dictionary.size, dictionary: serializable }));
  console.log(`idioms=${dictionary.size} statements=${statements} -> ${OUTPUT}`);
}

if (import.meta.main) main();
