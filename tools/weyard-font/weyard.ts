// Weyard font pipeline: extract the text font from all twelve Golden Sun ROMs,
// write one PNG sheet each, and merge them into a single weyard.otf.
//
//   bun tools/weyard-font/weyard.ts
//
// Output is ROM-derived and therefore generated, not tracked: it lands in
// ignored out/fonts/. Re-run to reproduce every sheet and the font.
//
// Two glyph formats, both validated against ROM bytes:
//   latin  32-byte entries, 16x15px: [advance][0][15 rows x 2 bytes LE, MSB-first]
//          codes 0x20..0xFF, Latin-1 identity encoding
//   kana   26-byte entries, 16x13px: [13 rows x 2 bytes LE, MSB-first], fixed width
//          codes 0x20.., ASCII below 0x80; JIS X 0201 katakana at 0xA1..0xDF;
//          hiragana mirroring that layout at -0x20 (0x86..0x9F) and +0x20 (0xE0..0xFD)
const fs = require("fs");
const ROM = 0x08000000;

type Kind = "latin" | "kana";
type Font = { rom: string; kind: Kind; base: number; glyphs: Glyph[] };
type Glyph = { advance: number; rows: number[] };

const SOURCES: { rom: string; kind: Kind; base: number }[] = [
  { rom: "gs1-ja", kind: "kana", base: 0x08032470 },
  { rom: "gs1-en", kind: "latin", base: 0x08032224 },
  { rom: "gs1-de", kind: "latin", base: 0x08030fd4 },
  { rom: "gs1-es", kind: "latin", base: 0x080314b4 },
  { rom: "gs1-fr", kind: "latin", base: 0x080312cc },
  { rom: "gs1-it", kind: "latin", base: 0x0803236c },
  { rom: "gs2-ja", kind: "kana", base: 0x0805a8cc },
  { rom: "gs2-en", kind: "latin", base: 0x0805a4e0 },
  { rom: "gs2-de", kind: "latin", base: 0x0805a7d4 },
  { rom: "gs2-es", kind: "latin", base: 0x0805a760 },
  { rom: "gs2-fr", kind: "latin", base: 0x0805a7e8 },
  { rom: "gs2-it", kind: "latin", base: 0x0805a6e4 },
];

const LATIN_ROWS = 15;
const KANA_ROWS = 13;
const LATIN_COUNT = 224; // 0x20..0xFF
const KANA_COUNT = 224; // 0x20..0xFF
const KANJI_COUNT = 114;
const KANJI_ROWS = 12;
const KANJI =
  "神殿名前中武器長剣発動呪使水火風地毒兄宝石町行炎船海氷道具島男女力土大上玉山気目入口岩天空防母北戦手出下品同死木憶以宮村東南森西灯台寺分先遺跡立人方時様主者陸説明士世光知伝金売客商屋子×年兄川官錬日民父冬古代夜雪春「」草原黄文○";
const CELL = 16;

function extract(rom: Uint8Array, kind: Kind, base: number): Glyph[] {
  const at = base - ROM;
  const stride = kind === "latin" ? 32 : 26;
  const rowCount = kind === "latin" ? LATIN_ROWS : KANA_ROWS;
  const count = kind === "latin" ? LATIN_COUNT : KANA_COUNT;
  const skip = kind === "latin" ? 2 : 0;
  const out: Glyph[] = [];
  for (let g = 0; g < count; g++) {
    const start = at + g * stride;
    const rows: number[] = [];
    for (let r = 0; r < rowCount; r++) {
      const o = start + skip + 2 * r;
      rows.push(rom[o] | (rom[o + 1] << 8));
    }
    out.push({ advance: kind === "latin" ? rom[start] : CELL, rows });
  }
  if (kind === "kana") {
    // The kanji block follows the kana block with its own geometry:
    // 114 entries, 24-byte stride, 12 rows. Pad to 13 so sheets stay uniform.
    const kanjiBase = at + KANA_COUNT * 26;
    for (let g = 0; g < KANJI_COUNT; g++) {
      const start = kanjiBase + g * 24;
      const rows: number[] = [];
      for (let r = 0; r < KANJI_ROWS; r++) {
        const o = start + 2 * r;
        rows.push(rom[o] | (rom[o + 1] << 8));
      }
      rows.push(0);
      out.push({ advance: CELL, rows });
    }
  }
  return out;
}

// ---------------------------------------------------------------- PNG output
function crc32(buf: Uint8Array): number {
  const t: number[] = [];
  for (let i = 0; i < 256; i++) {
    let c = i;
    for (let k = 0; k < 8; k++) c = c & 1 ? 0xedb88320 ^ (c >>> 1) : c >>> 1;
    t[i] = c >>> 0;
  }
  let c = 0xffffffff;
  for (const b of buf) c = t[(c ^ b) & 255] ^ (c >>> 8);
  return (c ^ 0xffffffff) >>> 0;
}
function writePng(path: string, w: number, h: number, px: Uint8Array) {
  const raw = Buffer.alloc((w + 1) * h);
  for (let y = 0; y < h; y++) {
    raw[y * (w + 1)] = 0;
    Buffer.from(px.subarray(y * w, (y + 1) * w)).copy(raw, y * (w + 1) + 1);
  }
  const z = require("zlib").deflateSync(raw, { level: 9 });
  const chunk = (tag: string, data: Buffer) => {
    const len = Buffer.alloc(4);
    len.writeUInt32BE(data.length);
    const body = Buffer.concat([Buffer.from(tag), data]);
    const c = Buffer.alloc(4);
    c.writeUInt32BE(crc32(body));
    return Buffer.concat([len, body, c]);
  };
  const ihdr = Buffer.alloc(13);
  ihdr.writeUInt32BE(w, 0);
  ihdr.writeUInt32BE(h, 4);
  ihdr[8] = 8;
  ihdr[9] = 0;
  fs.writeFileSync(
    path,
    Buffer.concat([
      Buffer.from([0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a]),
      chunk("IHDR", ihdr),
      chunk("IDAT", z),
      chunk("IEND", Buffer.alloc(0)),
    ]),
  );
}
function sheet(font: Font, path: string, scale = 3) {
  const rowCount = font.kind === "latin" ? LATIN_ROWS : KANA_ROWS;
  const cols = 16;
  const rows = Math.ceil(font.glyphs.length / cols);
  const cw = CELL + 1;
  const chh = rowCount + 1;
  const w = cols * cw * scale;
  const h = rows * chh * scale;
  const px = new Uint8Array(w * h).fill(0x1c);
  font.glyphs.forEach((glyph, index) => {
    const gx = (index % cols) * cw;
    const gy = Math.floor(index / cols) * chh;
    glyph.rows.forEach((v, r) => {
      for (let x = 0; x < CELL; x++) {
        if ((v >> (15 - x)) & 1) {
          for (let sy = 0; sy < scale; sy++)
            for (let sx = 0; sx < scale; sx++) {
              const yy = (gy + r) * scale + sy;
              const xx = (gx + x) * scale + sx;
              if (yy < h && xx < w) px[yy * w + xx] = 0xff;
            }
        }
      }
    });
  });
  writePng(path, w, h, px);
  return { w, h, rows };
}

// ------------------------------------------------------------ JA -> Unicode
function kanaMap(): Map<number, number> {
  const m = new Map<number, number>();
  for (let c = 0x20; c < 0x80; c++) m.set(c, c);
  m.set(0x5c, 0x00a5); // JIS X 0201 keeps yen at the backslash slot
  const katakana =
    "。「」、・ヲァィゥェォャュョッ" +
    "ーアイウエオカキクケコサシスセソ" +
    "タチツテトナニヌネノハヒフヘホマ" +
    "ミムメモヤユヨラリルレロワン゛゜";
  for (let i = 0; i < katakana.length; i++)
    m.set(0xa1 + i, katakana.codePointAt(i)!);
  // hiragana mirrors the katakana layout: 0x86..0x9F is katakana-0x20,
  // 0xE0..0xFD is katakana+0x20. Convert katakana -> hiragana where one exists.
  const toHira = (u: number) => (u >= 0x30a1 && u <= 0x30f3 ? u - 0x60 : u);
  for (let c = 0x86; c <= 0x9f; c++) {
    const k = m.get(c + 0x20);
    if (k !== undefined) m.set(c, toHira(k));
  }
  for (let c = 0xe0; c <= 0xfd; c++) {
    const k = m.get(c - 0x20);
    if (k !== undefined) m.set(c, toHira(k));
  }
  return m;
}

// ------------------------------------------------------------- font assembly
const UNITS = 1024;
const PX = 64;
const BASELINE = 11;

function contours(rows: number[]): [number, number][][] {
  const shapes: [number, number][][] = [];
  rows.forEach((v, r) => {
    let x = 0;
    while (x < CELL) {
      if ((v >> (15 - x)) & 1) {
        const start = x;
        while (x < CELL && (v >> (15 - x)) & 1) x++;
        const top = (BASELINE - r) * PX;
        const bottom = top - PX;
        shapes.push([
          [start * PX, bottom],
          [x * PX, bottom],
          [x * PX, top],
          [start * PX, top],
        ]);
      } else x++;
    }
  });
  return shapes;
}

function glyfEntry(rows: number[]): Buffer {
  const shapes = contours(rows);
  if (!shapes.length) return Buffer.alloc(0);
  const pts = shapes.flat();
  const xs = pts.map((p) => p[0]);
  const ys = pts.map((p) => p[1]);
  const head = Buffer.alloc(10);
  head.writeInt16BE(shapes.length, 0);
  head.writeInt16BE(Math.min(...xs), 2);
  head.writeInt16BE(Math.min(...ys), 4);
  head.writeInt16BE(Math.max(...xs), 6);
  head.writeInt16BE(Math.max(...ys), 8);
  const ends = Buffer.alloc(shapes.length * 2);
  let acc = -1;
  shapes.forEach((s, i) => {
    acc += s.length;
    ends.writeUInt16BE(acc, i * 2);
  });
  const instr = Buffer.alloc(2); // zero instructions
  const flags = Buffer.alloc(pts.length, 0x01);
  const xb = Buffer.alloc(pts.length * 2);
  const yb = Buffer.alloc(pts.length * 2);
  let px = 0;
  let py = 0;
  pts.forEach((p, i) => {
    xb.writeInt16BE(p[0] - px, i * 2);
    yb.writeInt16BE(p[1] - py, i * 2);
    px = p[0];
    py = p[1];
  });
  let out = Buffer.concat([head, ends, instr, flags, xb, yb]);
  if (out.length % 4)
    out = Buffer.concat([out, Buffer.alloc(4 - (out.length % 4))]);
  return out;
}

function cmapTable(codeToIndex: Map<number, number>): Buffer {
  const codes = [...codeToIndex.keys()].sort((a, b) => a - b);
  const segs: [number, number][] = [];
  let s = -1;
  let p = -1;
  for (const c of codes) {
    if (s < 0) {
      s = p = c;
    } else if (c === p + 1) p = c;
    else {
      segs.push([s, p]);
      s = p = c;
    }
  }
  if (s >= 0) segs.push([s, p]);
  segs.push([0xffff, 0xffff]);
  const n = segs.length;
  let search = 1;
  let sel = 0;
  while (search * 2 <= n) {
    search *= 2;
    sel++;
  }
  const parts: Buffer[] = [];
  const h = Buffer.alloc(14);
  h.writeUInt16BE(4, 0);
  h.writeUInt16BE(16 + 8 * n, 2);
  h.writeUInt16BE(0, 4);
  h.writeUInt16BE(n * 2, 6);
  h.writeUInt16BE(search * 2, 8);
  h.writeUInt16BE(sel, 10);
  h.writeUInt16BE(n * 2 - search * 2, 12);
  parts.push(h);
  const endB = Buffer.alloc(n * 2);
  segs.forEach((sg, i) => endB.writeUInt16BE(sg[1], i * 2));
  parts.push(endB, Buffer.alloc(2));
  const startB = Buffer.alloc(n * 2);
  segs.forEach((sg, i) => startB.writeUInt16BE(sg[0], i * 2));
  parts.push(startB);
  const deltaB = Buffer.alloc(n * 2);
  segs.forEach((sg, i) => {
    const d = sg[0] === 0xffff ? 1 : (codeToIndex.get(sg[0])! - sg[0]) & 0xffff;
    deltaB.writeUInt16BE(d, i * 2);
  });
  parts.push(deltaB, Buffer.alloc(n * 2));
  const sub = Buffer.concat(parts);
  const idx = Buffer.alloc(12);
  idx.writeUInt16BE(0, 0);
  idx.writeUInt16BE(1, 2);
  idx.writeUInt16BE(3, 4);
  idx.writeUInt16BE(1, 6);
  idx.writeUInt32BE(12, 8);
  return Buffer.concat([idx, sub]);
}

function nameTable(strings: [number, string][]): Buffer {
  const recs: Buffer[] = [];
  const store: Buffer[] = [];
  let offset = 0;
  for (const [id, text] of strings) {
    const data = Buffer.from(text, "utf16le").swap16();
    const r = Buffer.alloc(12);
    r.writeUInt16BE(3, 0);
    r.writeUInt16BE(1, 2);
    r.writeUInt16BE(0x409, 4);
    r.writeUInt16BE(id, 6);
    r.writeUInt16BE(data.length, 8);
    r.writeUInt16BE(offset, 10);
    recs.push(r);
    store.push(data);
    offset += data.length;
  }
  const h = Buffer.alloc(6);
  h.writeUInt16BE(0, 0);
  h.writeUInt16BE(strings.length, 2);
  h.writeUInt16BE(6 + 12 * strings.length, 4);
  return Buffer.concat([h, ...recs, ...store]);
}

function buildFont(glyphs: Map<number, Glyph>, path: string) {
  const codes = [...glyphs.keys()].sort((a, b) => a - b);
  const order: (number | null)[] = [null, ...codes];
  const codeToIndex = new Map<number, number>();
  order.forEach((c, i) => {
    if (c !== null) codeToIndex.set(c, i);
  });
  const glyfParts: Buffer[] = [];
  const loca: number[] = [0];
  const advances: number[] = [];
  let total = 0;
  for (const c of order) {
    const entry = c === null ? Buffer.alloc(0) : glyfEntry(glyphs.get(c)!.rows);
    glyfParts.push(entry);
    total += entry.length;
    loca.push(total);
    advances.push((c === null ? 8 : glyphs.get(c)!.advance) * PX);
  }
  const glyf = Buffer.concat(glyfParts);
  const longLoca = total > 0x1fffe;
  const locaBuf = Buffer.alloc(loca.length * (longLoca ? 4 : 2));
  loca.forEach((v, i) =>
    longLoca
      ? locaBuf.writeUInt32BE(v, i * 4)
      : locaBuf.writeUInt16BE(v / 2, i * 2),
  );
  const num = order.length;

  const head = Buffer.alloc(54);
  head.writeUInt32BE(0x00010000, 0);
  head.writeUInt32BE(0x00010000, 4);
  head.writeUInt32BE(0, 8);
  head.writeUInt32BE(0x5f0f3cf5, 12);
  head.writeUInt16BE(3, 16);
  head.writeUInt16BE(UNITS, 18);
  head.writeInt16BE(0, 36);
  head.writeInt16BE(-(LATIN_ROWS - BASELINE) * PX, 38);
  head.writeInt16BE(CELL * PX, 40);
  head.writeInt16BE(BASELINE * PX, 42);
  head.writeUInt16BE(0, 44);
  head.writeUInt16BE(8, 46);
  head.writeInt16BE(2, 48);
  head.writeInt16BE(longLoca ? 1 : 0, 50);
  head.writeInt16BE(0, 52);

  const hhea = Buffer.alloc(36);
  hhea.writeUInt32BE(0x00010000, 0);
  hhea.writeInt16BE(BASELINE * PX, 4);
  hhea.writeInt16BE(-(LATIN_ROWS - BASELINE) * PX, 6);
  hhea.writeInt16BE(0, 8);
  hhea.writeUInt16BE(Math.max(...advances), 10);
  hhea.writeInt16BE(1, 30);
  hhea.writeUInt16BE(num, 34);

  const hmtx = Buffer.alloc(num * 4);
  advances.forEach((a, i) => {
    hmtx.writeUInt16BE(a, i * 4);
    hmtx.writeInt16BE(0, i * 4 + 2);
  });

  const maxp = Buffer.alloc(32);
  maxp.writeUInt32BE(0x00010000, 0);
  maxp.writeUInt16BE(num, 4);
  maxp.writeUInt16BE(CELL * LATIN_ROWS * 4, 6);
  maxp.writeUInt16BE(CELL * LATIN_ROWS, 8);
  maxp.writeUInt16BE(2, 14);

  const os2 = Buffer.alloc(96);
  os2.writeUInt16BE(4, 0);
  os2.writeInt16BE(PX * 8, 2);
  os2.writeUInt16BE(400, 4);
  os2.writeUInt16BE(5, 6);
  os2.writeInt16BE(PX * 8, 8 + 60);
  os2.writeUInt32BE(0, 42);
  Buffer.from("WYRD").copy(os2, 58);
  os2.writeUInt16BE(0, 62);
  os2.writeUInt16BE(codes[0], 64);
  os2.writeUInt16BE(
    codes[codes.length - 1] > 0xffff ? 0xffff : codes[codes.length - 1],
    66,
  );
  os2.writeInt16BE(BASELINE * PX, 68);
  os2.writeInt16BE(-(LATIN_ROWS - BASELINE) * PX, 70);
  os2.writeUInt16BE(0, 72);
  os2.writeUInt16BE(BASELINE * PX, 74);
  os2.writeUInt16BE((LATIN_ROWS - BASELINE) * PX, 76);
  os2.writeInt16BE(PX * 8, 88);
  os2.writeInt16BE(PX * 5, 90);

  const post = Buffer.alloc(32);
  post.writeUInt32BE(0x00030000, 0);
  post.writeUInt16BE(1, 12);

  const name = nameTable([
    [0, "Reconstructed from Golden Sun ROM glyph data."],
    [1, "Weyard"],
    [2, "Regular"],
    [3, "Weyard 1.0"],
    [4, "Weyard"],
    [5, "Version 1.0"],
    [6, "Weyard-Regular"],
  ]);

  const tables: [string, Buffer][] = [
    ["OS/2", os2],
    ["cmap", cmapTable(codeToIndex)],
    ["glyf", glyf],
    ["head", head],
    ["hhea", hhea],
    ["hmtx", hmtx],
    ["loca", locaBuf],
    ["maxp", maxp],
    ["name", name],
    ["post", post],
  ];
  tables.sort((a, b) => (a[0] < b[0] ? -1 : 1));
  const count = tables.length;
  let search = 1;
  let sel = 0;
  while (search * 2 <= count) {
    search *= 2;
    sel++;
  }
  const header = Buffer.alloc(12);
  header.writeUInt32BE(0x00010000, 0);
  header.writeUInt16BE(count, 4);
  header.writeUInt16BE(search * 16, 6);
  header.writeUInt16BE(sel, 8);
  header.writeUInt16BE(count * 16 - search * 16, 10);
  let offset = 12 + 16 * count;
  const dir: Buffer[] = [];
  const body: Buffer[] = [];
  for (const [tag, data] of tables) {
    const padded =
      data.length % 4
        ? Buffer.concat([data, Buffer.alloc(4 - (data.length % 4))])
        : data;
    let sum = 0;
    for (let i = 0; i < padded.length; i += 4)
      sum = (sum + padded.readUInt32BE(i)) >>> 0;
    const rec = Buffer.alloc(16);
    Buffer.from(tag).copy(rec, 0);
    rec.writeUInt32BE(sum >>> 0, 4);
    rec.writeUInt32BE(offset, 8);
    rec.writeUInt32BE(data.length, 12);
    dir.push(rec);
    body.push(padded);
    offset += padded.length;
  }
  const out = Buffer.concat([header, ...dir, ...body]);
  fs.writeFileSync(path, out);
  return { glyphs: num, bytes: out.length };
}

// --------------------------------------------------------------------- main
const outDir = process.env.WEYARD_OUT ?? "out/fonts";
fs.mkdirSync(outDir, { recursive: true });
const fonts: Font[] = [];
console.log("rom      kind   base        glyphs  sheet");
for (const src of SOURCES) {
  const rom = new Uint8Array(fs.readFileSync(`roms/${src.rom}.gba`));
  const glyphs = extract(rom, src.kind, src.base);
  const font: Font = { ...src, glyphs };
  fonts.push(font);
  const path = `${outDir}/${src.rom}.png`;
  const { w, h } = sheet(font, path);
  console.log(
    `${src.rom.padEnd(8)} ${src.kind.padEnd(6)} 0x${src.base.toString(16)}  ${String(glyphs.length).padStart(4)}   ${path} (${w}x${h})`,
  );
}

// Merge. Latin editions vote per Latin-1 code; JA supplies everything else.
const merged = new Map<number, Glyph>();
const provenance: string[] = [];
let conflicts = 0;
for (let code = 0x20; code <= 0xff; code++) {
  const seen = new Map<string, { n: number; glyph: Glyph }>();
  for (const f of fonts) {
    if (f.kind !== "latin") continue;
    const g = f.glyphs[code - 0x20];
    if (!g || !g.rows.some((v) => v)) continue;
    const key = g.rows.join(",");
    const e = seen.get(key) ?? { n: 0, glyph: g };
    e.n++;
    seen.set(key, e);
  }
  if (!seen.size) continue;
  if (seen.size > 1) conflicts++;
  const best = [...seen.values()].sort((a, b) => b.n - a.n)[0];
  merged.set(code, best.glyph);
}
const map = kanaMap();
const ja = fonts.find((f) => f.kind === "kana")!;
let jaAdded = 0;
let kanji = 0;
for (let index = 0; index < ja.glyphs.length; index++) {
  const glyph = ja.glyphs[index];
  if (!glyph.rows.some((v) => v)) continue;
  let unicode: number | undefined;
  if (index < KANA_COUNT) {
    unicode = map.get(0x20 + index);
  } else {
    unicode = KANJI.codePointAt(index - KANA_COUNT);
    if (unicode !== undefined) kanji++;
  }
  if (unicode === undefined) continue;
  if (!merged.has(unicode)) {
    merged.set(unicode, glyph);
    jaAdded++;
  }
}
const path = `${outDir}/weyard.otf`;
const info = buildFont(merged, path);
console.log(
  `\nmerged: ${merged.size} characters ` +
    `(Latin-1 from 10 ROMs, ${conflicts} codes where editions differ; ` +
    `${jaAdded} Japanese from JA, of which ${kanji} kanji)`,
);
console.log(`font:   ${path}  ${info.glyphs} glyphs, ${info.bytes} bytes`);
