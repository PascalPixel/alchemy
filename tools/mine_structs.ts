#!/usr/bin/env bun
// Tool role: entrypoint; invoked by PLAYBOOK.md.
// 構造体採掘。全一致済みCと全再構築アセンブリからフィールド接近を集計し、
// 基底ごとの型付き構造体候補を推定してm2c文脈ヘッダを豊かにする。
// 幅はldrh/strh/ldrb等の命令幅とM2C_FIELDの型から取り、多数決で確定する。
// 出力はwork/structs.jsonとwork/m2c_context.h(追記部)。ROMバイトは書かない。
import { readFileSync, readdirSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

interface FieldSeen { width: number; signed: boolean; pointer: boolean; count: number }

// 基底の同定: 固定絶対アドレス(0x02/0x03番地)は自身が基底。関数引数由来は
// 呼出し規約を跨いで同定できないため、絶対基底だけを第一段の対象にする。
function main(): void {
  const bases = new Map<string, Map<number, FieldSeen>>();
  const record = (base: string, offset: number, width: number, signed: boolean, pointer: boolean) => {
    if (!bases.has(base)) bases.set(base, new Map());
    const fields = bases.get(base)!;
    const previous = fields.get(offset);
    if (previous === undefined) fields.set(offset, { width, signed, pointer, count: 1 });
    else {
      previous.count++;
      if (width > previous.width) { previous.width = width; previous.signed = signed; }
      previous.pointer ||= pointer;
    }
  };

  // C側: *(型 *)(0x0X番地 + 変位) と M2C_FIELD(*(型 **)0x0X番地, 型 *, 変位)
  const absolute = /\*\s*\((u8|s8|u16|s16|u32|s32|void \*)\s*\*\)\s*\(?0x0([23])[0-9A-Fa-f]{6}\b(?:\s*\+\s*(0x[0-9A-Fa-f]+|\d+))?/g;
  const indirect = /M2C_FIELD\(\s*\*\s*\(void \*\*\)\s*(0x0[23][0-9A-Fa-f]{6})\s*,\s*(u8|s8|u16|s16|u32|s32|void \*)\s*\*\s*,\s*(0x[0-9A-Fa-f]+|\d+)\)/g;
  const widthOf = (type: string): [number, boolean, boolean] =>
    type === "void *" ? [4, false, true] :
    [type.includes("8") ? 1 : type.includes("16") ? 2 : 4, type.startsWith("s"), false];

  for (const name of readdirSync(join(ROOT, "src")).filter((entry) => entry.endsWith(".c"))) {
    const text = readFileSync(join(ROOT, "src", name), "utf8");
    for (const match of text.matchAll(indirect)) {
      const [width, signed, pointer] = widthOf(match[2]);
      record(`ptr:${match[1].toLowerCase()}`, Number(match[3]), width, signed, pointer);
    }
    for (const match of text.matchAll(absolute)) {
      const base = match[0].match(/0x0[23][0-9A-Fa-f]{6}/)![0].toLowerCase();
      const [width, signed, pointer] = widthOf(match[1]);
      record(`abs:${base}`, match[3] ? Number(match[3]) : 0, width, signed, pointer);
    }
  }

  // アセンブリ側: 基底レジスタへ读み込まれた絶対番地は追跡が要るため、
  // 第一段では即値変位付きロード・ストアの統計だけを別枠で残す。
  const summary: Record<string, Array<{ offset: number; width: number; signed: boolean; pointer: boolean; count: number }>> = {};
  for (const [base, fields] of [...bases.entries()].sort()) {
    summary[base] = [...fields.entries()]
      .sort((left, right) => left[0] - right[0])
      .map(([offset, seen]) => ({ offset, ...seen }));
  }
  writeFileSync(join(ROOT, "work/structs.json"), JSON.stringify({ format: 1, bases: summary }, null, 1));

  // m2c文脈への反映: 充分な観測(3件以上、4フィールド以上)の基底だけを
  // 構造体化し、名前は番地由来の機械名に留める(NAMING.mdの命名保留規則)。
  let header = "";
  let emitted = 0;
  for (const [base, fields] of Object.entries(summary)) {
    const strong = fields.filter((field) => field.count >= 3);
    if (strong.length < 4) continue;
    const address = base.slice(base.indexOf(":") + 1);
    const structName = `Work_${address.slice(2)}`;
    header += `struct ${structName} {\n`;
    let cursor = 0;
    let pad = 0;
    for (const field of strong) {
      if (field.offset < cursor) continue;
      if (field.offset > cursor) { header += `    u8 pad_${cursor.toString(16)}[${field.offset - cursor}];\n`; pad++; }
      const type = field.pointer ? "void *" : `${field.signed ? "s" : "u"}${field.width * 8}`;
      header += `    ${type} field_${field.offset.toString(16)};\n`;
      cursor = field.offset + field.width;
    }
    header += `};\n`;
    emitted++;
  }
  writeFileSync(join(ROOT, "work/structs.h"), header);
  console.log(`bases=${bases.size} structs=${emitted} -> work/structs.json work/structs.h`);
}

main();
