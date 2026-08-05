//
// Cross-platform note: every DEFLATE-compressed byte this repo tracks (PNG
// IDAT chunks) must be reproducible from any supported host: darwin/linux on
// arm64/x86_64. That rule out-ranks compression ratio.
//
// History: this file first wrapped Bun.deflateSync (libdeflate), whose output
// is build-configuration-dependent; the 2026-08-05 recompress moved it to
// node:zlib on the theory that classic zlib is architecture-independent.
// Measured on darwin-arm64 the same day: Bun 1.3.14's node:zlib shim emits a
// same-length, different-byte stream for identical input and settings than
// the linux-x64 build that wrote the tree, so the canonical-bytes invariant
// broke on half the supported hosts. The library behind the shim is not
// something this repository controls.
//
// The compressor below is therefore implemented here, in plain TypeScript,
// with no host library in the loop: one fixed-Huffman DEFLATE block, greedy
// hash-chain LZ77 with fully specified tie-breaking, and an adler32 wrapper.
// Same input, same bytes, on every host and every Bun build, by construction.
// It compresses a little worse than zlib level 9; that is the price of
// determinism, paid knowingly. Decompression and crc32 remain node:zlib:
// inflate output and CRC values are fully specified by RFC 1950/1951, so any
// correct implementation agrees byte-for-byte.
import { crc32 as nodeCrc32, inflateSync as nodeInflateSync } from "node:zlib";

interface DeflateOptions {
  // Accepted for call-site compatibility; the deterministic encoder has a
  // single mode, so the level no longer changes the output.
  level?: 6 | 9;
}

export function crc32(data: Uint8Array): number {
  return nodeCrc32(data) >>> 0;
}

export function inflateSync(data: Uint8Array): Buffer {
  return nodeInflateSync(data);
}

function adler32(data: Uint8Array): number {
  let s1 = 1;
  let s2 = 0;
  // 5552 is zlib's NMAX: the largest run whose sums stay inside a u32
  // between modulo reductions.
  for (let offset = 0; offset < data.length; offset += 5552) {
    const end = Math.min(offset + 5552, data.length);
    for (let index = offset; index < end; index++) {
      s1 += data[index];
      s2 += s1;
    }
    s1 %= 65521;
    s2 %= 65521;
  }
  return ((s2 << 16) >>> 0 | s1) >>> 0;
}

// LSB-first bit writer (RFC 1951 packing: data element bits fill each byte
// starting at its least significant bit).
class BitWriter {
  private bytes: number[] = [];
  private bitBuffer = 0;
  private bitCount = 0;

  // Append `count` bits whose LSB is the first bit of the element.
  writeBits(value: number, count: number): void {
    this.bitBuffer |= value << this.bitCount;
    this.bitCount += count;
    while (this.bitCount >= 8) {
      this.bytes.push(this.bitBuffer & 0xff);
      this.bitBuffer >>>= 8;
      this.bitCount -= 8;
    }
  }

  // Huffman codes are emitted most significant code bit first.
  writeCode(code: number, length: number): void {
    let reversed = 0;
    for (let bit = 0; bit < length; bit++) reversed = (reversed << 1) | ((code >> bit) & 1);
    this.writeBits(reversed, length);
  }

  finish(): Uint8Array {
    if (this.bitCount > 0) {
      this.bytes.push(this.bitBuffer & 0xff);
      this.bitBuffer = 0;
      this.bitCount = 0;
    }
    return Uint8Array.from(this.bytes);
  }
}

function fixedLiteralCode(symbol: number): { code: number; length: number } {
  if (symbol <= 143) return { code: 0x30 + symbol, length: 8 };
  if (symbol <= 255) return { code: 0x190 + (symbol - 144), length: 9 };
  if (symbol <= 279) return { code: symbol - 256, length: 7 };
  return { code: 0xc0 + (symbol - 280), length: 8 };
}

// RFC 1951 section 3.2.5 length and distance code tables.
const LENGTH_BASE = [3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
  35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258];
const LENGTH_EXTRA = [0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
  3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0];
const DISTANCE_BASE = [1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
  257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577];
const DISTANCE_EXTRA = [0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
  7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13];

const WINDOW_SIZE = 32768;
const MIN_MATCH = 3;
const MAX_MATCH = 258;
// Fixed search budget per position. Any fixed budget is deterministic; this
// one keeps worst-case time near-linear while finding almost every long match.
const MAX_CHAIN_STEPS = 4096;
const HASH_BITS = 16;

function emitMatch(writer: BitWriter, length: number, distance: number): void {
  let lengthSymbol = LENGTH_BASE.length - 1;
  while (LENGTH_BASE[lengthSymbol] > length) lengthSymbol--;
  const literal = fixedLiteralCode(257 + lengthSymbol);
  writer.writeCode(literal.code, literal.length);
  writer.writeBits(length - LENGTH_BASE[lengthSymbol], LENGTH_EXTRA[lengthSymbol]);

  let distanceSymbol = DISTANCE_BASE.length - 1;
  while (DISTANCE_BASE[distanceSymbol] > distance) distanceSymbol--;
  writer.writeCode(distanceSymbol, 5);
  writer.writeBits(distance - DISTANCE_BASE[distanceSymbol], DISTANCE_EXTRA[distanceSymbol]);
}

export function deflateSync(data: Uint8Array, _options: DeflateOptions = {}): Buffer {
  const writer = new BitWriter();
  // zlib wrapper: CMF 0x78 (32K window, method 8), FLG 0x01 (FLEVEL 0,
  // no dictionary, check bits making CMF<<8|FLG divisible by 31).
  writer.writeBits(0x78, 8);
  writer.writeBits(0x01, 8);
  // One final fixed-Huffman block (BFINAL=1, BTYPE=01). A single block is
  // valid at any length and removes block-splitting policy from the output.
  writer.writeBits(1, 1);
  writer.writeBits(1, 2);

  const head = new Int32Array(1 << HASH_BITS).fill(-1);
  const previous = new Int32Array(data.length);
  const hashAt = (index: number): number =>
    ((data[index] << 10) ^ (data[index + 1] << 5) ^ data[index + 2]) & ((1 << HASH_BITS) - 1);

  let index = 0;
  while (index < data.length) {
    let bestLength = 0;
    let bestDistance = 0;
    if (index + MIN_MATCH <= data.length) {
      const hash = hashAt(index);
      let candidate = head[hash];
      let steps = 0;
      const limit = Math.min(MAX_MATCH, data.length - index);
      // Chains are most-recent-first, so the first candidate reaching a given
      // length has the smallest distance; requiring strictly greater lengths
      // afterwards makes the (length, distance) choice fully specified.
      while (candidate >= 0 && index - candidate <= WINDOW_SIZE && steps < MAX_CHAIN_STEPS) {
        let matched = 0;
        while (matched < limit && data[candidate + matched] === data[index + matched]) matched++;
        if (matched > bestLength) {
          bestLength = matched;
          bestDistance = index - candidate;
          if (matched === limit) break;
        }
        candidate = previous[candidate];
        steps++;
      }
    }
    if (bestLength >= MIN_MATCH) {
      emitMatch(writer, bestLength, bestDistance);
      const end = Math.min(index + bestLength, data.length - MIN_MATCH + 1);
      for (let position = index; position < end; position++) {
        const hash = hashAt(position);
        previous[position] = head[hash];
        head[hash] = position;
      }
      index += bestLength;
    } else {
      const literal = fixedLiteralCode(data[index]);
      writer.writeCode(literal.code, literal.length);
      if (index + MIN_MATCH <= data.length) {
        const hash = hashAt(index);
        previous[index] = head[hash];
        head[hash] = index;
      }
      index++;
    }
  }
  const endOfBlock = fixedLiteralCode(256);
  writer.writeCode(endOfBlock.code, endOfBlock.length);

  const deflated = writer.finish();
  const output = Buffer.alloc(deflated.length + 4);
  Buffer.from(deflated).copy(output);
  output.writeUInt32BE(adler32(data), deflated.length);
  return output;
}

function selfTest(): void {
  const cases: Uint8Array[] = [
    new Uint8Array(0),
    Uint8Array.from([0]),
    Uint8Array.from([255]),
    Uint8Array.from(Array.from({ length: 256 }, (_, value) => value)),
    new Uint8Array(70000).fill(0x41),
    Uint8Array.from(Array.from({ length: 200000 }, (_, value) => (value * 2654435761) >>> 24)),
    Uint8Array.from(Array.from({ length: 5000 }, (_, value) => value % 7)),
  ];
  for (const [caseIndex, data] of cases.entries()) {
    const compressed = deflateSync(data);
    const restored = inflateSync(compressed);
    if (!Buffer.from(data).equals(restored)) {
      throw new Error(`deflate round-trip failed for case ${caseIndex} (${data.length} bytes)`);
    }
    const again = deflateSync(data);
    if (!compressed.equals(again)) {
      throw new Error(`deflate is not deterministic for case ${caseIndex}`);
    }
  }
  console.log(`self-test=ok tool=zlib cases=${cases.length}`);
}

if (import.meta.main && Bun.argv.includes("--self-test")) selfTest();
