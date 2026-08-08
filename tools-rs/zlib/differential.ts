// Differential harness: pushes a corpus through the TypeScript
// tools/lib/zlib.ts and the Rust alchemy-zlib crate and diffs every byte.
//
// Not part of the shipped tooling; run manually:
//   bun tools-rs/zlib/differential.ts
import { readdirSync, readFileSync, statSync } from "node:fs";
import { join } from "node:path";
import { crc32, deflateSync, inflateSync } from "../../tools/lib/zlib.ts";

const RUST = new URL("./target/release/alchemy-zlib", import.meta.url).pathname;

function rust(mode: string, input: Uint8Array): { ok: boolean; out: Uint8Array } {
  const result = Bun.spawnSync([RUST, mode], { stdin: input, stdout: "pipe", stderr: "pipe" });
  return { ok: result.exitCode === 0, out: new Uint8Array(result.stdout) };
}

function equal(a: Uint8Array, b: Uint8Array): boolean {
  if (a.length !== b.length) return false;
  for (let i = 0; i < a.length; i++) if (a[i] !== b[i]) return false;
  return true;
}

function walk(root: string, out: string[], cap: number): void {
  let entries: string[];
  try {
    entries = readdirSync(root);
  } catch {
    return;
  }
  for (const entry of entries) {
    if (out.length >= cap) return;
    const path = join(root, entry);
    let info;
    try {
      info = statSync(path);
    } catch {
      continue;
    }
    if (info.isDirectory()) walk(path, out, cap);
    else if (info.isFile() && info.size > 0 && info.size <= 400_000) out.push(path);
  }
}

const corpus: { name: string; data: Uint8Array }[] = [];

// 1. Real repository data: ROM slices and asset/out files.
const romDir = "roms";
for (const rom of readdirSync(romDir)) {
  const bytes = new Uint8Array(readFileSync(join(romDir, rom)));
  for (let i = 0; i < 12; i++) {
    const offset = Math.floor((bytes.length / 12) * i);
    const length = [1, 2, 3, 17, 1024, 65536, 200_000][i % 7];
    corpus.push({ name: `${rom}@${offset}+${length}`, data: bytes.slice(offset, offset + length) });
  }
}

const files: string[] = [];
walk("assets", files, 900);
walk("out/assets", files, 1200);
for (const path of files) {
  corpus.push({ name: path, data: new Uint8Array(readFileSync(path)) });
}

// 2. Generated edge cases.
let seed = 0x9e3779b9;
const rnd = (): number => {
  seed ^= seed << 13;
  seed ^= seed >>> 17;
  seed ^= seed << 5;
  return (seed >>> 0) % 256;
};
corpus.push({ name: "empty", data: new Uint8Array(0) });
for (let byte = 0; byte < 256; byte++) corpus.push({ name: `single-${byte}`, data: Uint8Array.from([byte]) });
for (const length of [1, 2, 3, 4, 5, 257, 258, 259, 260, 1000, 32767, 32768, 32769, 70000, 300_000]) {
  corpus.push({ name: `same-${length}`, data: new Uint8Array(length).fill(0x5a) });
  corpus.push({ name: `random-${length}`, data: Uint8Array.from({ length }, rnd) });
  corpus.push({ name: `ramp-${length}`, data: Uint8Array.from({ length }, (_, i) => i & 0xff) });
}
for (let alphabet = 1; alphabet <= 32; alphabet++) {
  for (let trial = 0; trial < 8; trial++) {
    const length = 1 + ((rnd() << 8) | rnd()) * 3;
    corpus.push({
      name: `alpha${alphabet}-${trial}`,
      data: Uint8Array.from({ length }, () => rnd() % alphabet),
    });
  }
}

let compared = 0;
let mismatches = 0;
const report = (name: string, what: string): void => {
  mismatches++;
  if (mismatches <= 10) console.error(`MISMATCH ${what} ${name}`);
};

for (const { name, data } of corpus) {
  const tsDeflate = new Uint8Array(deflateSync(data));
  const rsDeflate = rust("--deflate", data);
  compared++;
  if (!rsDeflate.ok || !equal(tsDeflate, rsDeflate.out)) report(name, "deflate");

  // Inflate both ways: each side must decode the other side's bytes.
  const tsInflate = new Uint8Array(inflateSync(tsDeflate));
  const rsInflate = rust("--inflate", tsDeflate);
  compared++;
  if (!rsInflate.ok || !equal(tsInflate, rsInflate.out) || !equal(tsInflate, data)) {
    report(name, "inflate");
  }

  const tsCrc = crc32(data);
  const rsCrc = Bun.spawnSync([RUST, "--crc32"], { stdin: data, stdout: "pipe" });
  compared++;
  if (Number(rsCrc.stdout.toString().trim()) !== tsCrc) report(name, "crc32");
}

// 3. Malformed and truncated input: both sides must agree on accept/reject
//    and on the bytes when accepted.
const base = new Uint8Array(deflateSync(Uint8Array.from({ length: 4000 }, (_, i) => i % 23)));
const broken: { name: string; data: Uint8Array }[] = [];
for (let cut = 0; cut <= base.length; cut++) {
  broken.push({ name: `truncate-${cut}`, data: base.slice(0, cut) });
}
for (let i = 0; i < 4000; i++) {
  const copy = base.slice();
  copy[rnd() % copy.length] ^= 1 << (rnd() % 8);
  broken.push({ name: `flip-${i}`, data: copy });
}
broken.push({ name: "garbage", data: Uint8Array.from([1, 2, 3, 4, 5]) });
broken.push({ name: "header-only", data: Uint8Array.from([0x78, 0x01]) });

let rejected = 0;

for (const { name, data } of broken) {
  let tsOk = true;
  let tsOut = new Uint8Array(0);
  try {
    tsOut = new Uint8Array(inflateSync(data));
  } catch {
    tsOk = false;
  }
  const rs = rust("--inflate", data);
  if (!tsOk) rejected++;
  compared++;
  if (rs.ok !== tsOk || (tsOk && !equal(tsOut, rs.out))) report(name, `malformed(ts=${tsOk} rs=${rs.ok})`);
}

console.log(
  `corpus_inputs=${corpus.length} malformed_inputs=${broken.length} ` +
    `malformed_rejected=${rejected} malformed_accepted=${broken.length - rejected}`,
);
console.log(`cases=${compared} mismatches=${mismatches}`);
if (mismatches > 0) process.exit(1);
