export interface ZeroSkipDecoded {
  pixels: Buffer;
  bytes: number;
}

export type StaticMode3Operation = ["l", number] | ["c", number, number];

export interface StaticMode3Plan {
  split: number;
  operations?: StaticMode3Operation[];
  final_flags?: number;
}

export interface StaticMode3Decoded {
  stream: Buffer;
  bytes: number;
  plan: StaticMode3Plan;
}

export interface StaticSpritePlan {
  format: number;
  codec: string;
  pixels: number;
  stream_bytes: number;
  encoded_bytes: number;
  wrapper: StaticMode3Plan;
}

export interface StaticSpriteDecoded {
  pixels: Buffer;
  stream: Buffer;
  encodedSize: number;
  plan: StaticSpritePlan;
}

const STATIC_CODEC = "golden-sun-static-sprite-mode3";
const MAX_PIXELS = 0x1000000;

function integer(value: number, minimum: number, maximum: number, name: string): void {
  if (!Number.isInteger(value) || value < minimum || value > maximum)
    throw new Error(`${name} is outside ${minimum}..${maximum}`);
}

function byte(data: Uint8Array, offset: number, name: string): number {
  if (offset < 0 || offset >= data.length) throw new Error(`${name} is truncated`);
  return data[offset];
}

function be16(data: Uint8Array, offset: number, name: string): number {
  return byte(data, offset, name) << 8 | byte(data, offset + 1, name);
}

export function decode_zero_skip_trace(
  data: Uint8Array, offset = 0, maxPixels = MAX_PIXELS,
): ZeroSkipDecoded {
  integer(offset, 0, data.length, "zero-skip offset");
  integer(maxPixels, 0, Number.MAX_SAFE_INTEGER, "zero-skip pixel limit");
  const pixels: number[] = [];
  let cursor = offset;
  while (cursor < data.length) {
    const value = data[cursor++];
    if (value === 0) {
      return { pixels: Buffer.from(pixels), bytes: cursor - offset };
    }
    if (value <= 0xdf) {
      if (pixels.length >= maxPixels) throw new Error("zero-skip pixel limit exceeded");
      pixels.push(value);
      continue;
    }
    const count = value - 0xdf;
    if (pixels.length + count > maxPixels) throw new Error("zero-skip pixel limit exceeded");
    for (let index = 0; index < count; index++) pixels.push(0);
  }
  throw new Error("zero-skip stream has no terminator");
}

export function encode_zero_skip(pixels: Uint8Array): Buffer {
  const output: number[] = [];
  let cursor = 0;
  while (cursor < pixels.length) {
    const value = pixels[cursor];
    if (value !== 0) {
      integer(value, 1, 0xdf, "zero-skip literal");
      output.push(value);
      cursor++;
      continue;
    }
    let end = cursor + 1;
    while (end < pixels.length && pixels[end] === 0) end++;
    let count = end - cursor;
    while (count > 0) {
      const chunk = Math.min(count, 32);
      output.push(0xdf + chunk);
      count -= chunk;
    }
    cursor = end;
  }
  output.push(0);
  return Buffer.from(output);
}

export function decode_mode3_trace(arena: Uint8Array, frameOffset: number): StaticMode3Decoded {
  integer(frameOffset, 0, arena.length - 2, "mode-3 frame offset");
  const split = byte(arena, frameOffset, "mode-3 header") |
    byte(arena, frameOffset + 1, "mode-3 header") << 8;
  const base = frameOffset + 2;
  if (split === 0) {
    const decoded = decode_zero_skip_trace(arena, base);
    return {
      stream: Buffer.from(arena.subarray(base, base + decoded.bytes)),
      bytes: 2 + decoded.bytes,
      plan: { split: 0 },
    };
  }
  if (split < 2 || frameOffset + split >= arena.length)
    throw new Error("mode-3 split is outside the encoded arena");
  let literalCursor = base;
  let controlCursor = frameOffset + split;
  const controlStart = controlCursor;
  const output: number[] = [];
  const operations: StaticMode3Operation[] = [];
  let ended = false;
  let finalFlags = 0;
  while (!ended) {
    const flags = byte(arena, controlCursor++, "mode-3 flag group");
    for (let bit = 0; bit < 8; bit++) {
      if (flags >>> bit & 1) {
        if (literalCursor >= controlStart)
          throw new Error("mode-3 literal prefix is exhausted");
        output.push(arena[literalCursor++]);
        const previous = operations.at(-1);
        if (previous?.[0] === "l") previous[1]++;
        else operations.push(["l", 1]);
        continue;
      }
      const word = be16(arena, controlCursor, "mode-3 copy token");
      controlCursor += 2;
      if (word === 0) {
        finalFlags = flags & ~((1 << (bit + 1)) - 1);
        ended = true;
        break;
      }
      const distance = word & 0x0fff;
      if (distance === 0) throw new Error("mode-3 copy has zero distance");
      const nibble = word >>> 12;
      const length = nibble === 0
        ? byte(arena, controlCursor++, "mode-3 extended length") + 18
        : nibble + 2;
      const source = base - distance;
      if (source < 0 || source + length > arena.length)
        throw new Error("mode-3 copy is outside the encoded arena");
      for (let index = 0; index < length; index++) output.push(arena[source + index]);
      operations.push(["c", distance, length]);
    }
  }
  if (literalCursor !== controlStart)
    throw new Error("mode-3 literal prefix was not consumed exactly");
  return {
    stream: Buffer.from(output), bytes: controlCursor - frameOffset,
    plan: { split, operations, ...(finalFlags ? { final_flags: finalFlags } : {}) },
  };
}

interface CopyCheck {
  output: number;
  distance: number;
  length: number;
}

function copy_word(distance: number, length: number): [number, number | undefined] {
  integer(distance, 1, 0x0fff, "mode-3 copy distance");
  integer(length, 3, 273, "mode-3 copy length");
  if (length <= 17)
    return [((length - 2) << 12) | distance, undefined];
  return [distance, length - 18];
}

export function encode_mode3_plan(
  stream: Uint8Array, plan: StaticMode3Plan, precedingArena: Uint8Array,
): Buffer {
  if (!plan || !Number.isInteger(plan.split)) throw new Error("invalid mode-3 plan");
  integer(plan.split, 0, 0xffff, "mode-3 split");
  if (plan.split === 0) {
    if ((plan.operations !== undefined && plan.operations.length !== 0) || plan.final_flags !== undefined)
      throw new Error("raw mode-3 plan cannot contain operations");
    const header = Buffer.alloc(2);
    return Buffer.concat([header, Buffer.from(stream)]);
  }
  if (plan.split < 2 || !Array.isArray(plan.operations))
    throw new Error("compressed mode-3 plan requires operations");
  const literals: number[] = [];
  const checks: CopyCheck[] = [];
  const bits: boolean[] = [];
  const payloads: Array<number[] | undefined> = [];
  let output = 0;
  for (const operation of plan.operations) {
    if (!Array.isArray(operation) || (operation[0] !== "l" && operation[0] !== "c"))
      throw new Error("unknown mode-3 operation");
    if (operation[0] === "l") {
      const count = Number(operation[1]);
      integer(count, 1, stream.length, "mode-3 literal run");
      if (output + count > stream.length) throw new Error("mode-3 literal exceeds decoded stream");
      for (let index = 0; index < count; index++) {
        literals.push(stream[output++]);
        bits.push(true);
        payloads.push(undefined);
      }
    } else {
      if (operation.length !== 3) throw new Error("mode-3 copy operation has the wrong shape");
      const distance = Number(operation[1]), length = Number(operation[2]);
      const [word, extra] = copy_word(distance, length);
      if (output + length > stream.length)
          throw new Error("mode-3 copy exceeds decoded stream");
      checks.push({ output, distance, length });
      output += length;
      bits.push(false);
      payloads.push([word >>> 8, word & 0xff, ...(extra === undefined ? [] : [extra])]);
    }
  }
  if (output !== stream.length) throw new Error("mode-3 plan does not cover the decoded stream");
  if (literals.length !== plan.split - 2)
    throw new Error("mode-3 split differs from its literal prefix");
  bits.push(false);
  payloads.push([0, 0]);
  const control: number[] = [];
  const finalUsed = bits.length % 8 || 8;
  const finalMask = ~((1 << finalUsed) - 1) & 0xff;
  const finalFlags = Number(plan.final_flags ?? 0);
  integer(finalFlags, 0, 0xff, "mode-3 final flags");
  if (finalFlags & ~finalMask) throw new Error("mode-3 final flags overlap planned operations");
  for (let group = 0; group < bits.length; group += 8) {
    const used = Math.min(8, bits.length - group);
    let flags = group + used === bits.length ? finalFlags : 0;
    for (let bit = 0; bit < used; bit++) if (bits[group + bit]) flags |= 1 << bit;
    control.push(flags);
    for (let bit = 0; bit < used; bit++) {
      const payload = payloads[group + bit];
      if (payload) control.push(...payload);
    }
  }
  const header = Buffer.alloc(2);
  header.writeUInt16LE(plan.split);
  const frame = Buffer.concat([header, Buffer.from(literals), Buffer.from(control)]);
  const combined = Buffer.concat([Buffer.from(precedingArena), frame]);
  const frameOffset = precedingArena.length;
  const base = frameOffset + 2;
  for (const check of checks) {
    const source = base - check.distance;
    if (source < 0 || source + check.length > combined.length)
      throw new Error("mode-3 copy source has not been reconstructed");
    for (let index = 0; index < check.length; index++) {
      if (stream[check.output + index] !== combined[source + index])
        throw new Error("mode-3 copy differs from the encoded arena dictionary");
    }
  }
  return frame;
}

export function decode_static_frame_trace(
  arena: Uint8Array, frameOffset: number,
): StaticSpriteDecoded {
  const wrapper = decode_mode3_trace(arena, frameOffset);
  const zeroSkip = decode_zero_skip_trace(wrapper.stream);
  if (zeroSkip.bytes !== wrapper.stream.length)
    throw new Error("mode-3 output continues after the zero-skip terminator");
  return {
    pixels: zeroSkip.pixels,
    stream: wrapper.stream,
    encodedSize: wrapper.bytes,
    plan: {
      format: 1,
      codec: STATIC_CODEC,
      pixels: zeroSkip.pixels.length,
      stream_bytes: wrapper.stream.length,
      encoded_bytes: wrapper.bytes,
      wrapper: wrapper.plan,
    },
  };
}

export function encode_static_frame_plan(
  pixels: Uint8Array, plan: StaticSpritePlan, precedingArena: Uint8Array,
): Buffer {
  if (!plan || plan.format !== 1 || plan.codec !== STATIC_CODEC)
    throw new Error("unsupported static-sprite plan");
  if (pixels.length !== Number(plan.pixels))
    throw new Error("static-sprite pixel count differs from plan");
  const stream = encode_zero_skip(pixels);
  if (stream.length !== Number(plan.stream_bytes))
    throw new Error("static-sprite stream size differs from plan");
  const frame = encode_mode3_plan(stream, plan.wrapper, precedingArena);
  if (frame.length !== Number(plan.encoded_bytes))
    throw new Error("static-sprite encoded size differs from plan");
  return frame;
}

function expect_failure(action: () => unknown, text: string): void {
  try { action(); } catch { return; }
  throw new Error(text);
}

export function self_test(): void {
  const rawPixels = Buffer.concat([Buffer.from([7]), Buffer.alloc(37), Buffer.from([9])]);
  const rawStream = Buffer.from([7, 0xff, 0xe4, 9, 0]);
  const rawFrame = Buffer.concat([Buffer.alloc(2), rawStream]);
  const raw = decode_static_frame_trace(rawFrame, 0);
  if (!raw.pixels.equals(rawPixels) || raw.encodedSize !== rawFrame.length ||
      !encode_static_frame_plan(raw.pixels, raw.plan, Buffer.alloc(0)).equals(rawFrame))
    throw new Error("raw static-sprite replay failed");

  const preceding = Buffer.alloc(96, 0x55);
  preceding.set([1, 2, 3, 4], 10);
  preceding.set(Array.from({ length: 18 }, (_, index) => index + 6), 20);
  preceding.set([25, 26, 27], 40);
  preceding.set([29, 30, 31, 32], 44);
  preceding.set([34, 35, 36, 37, 38, 39, 40, 0xe0], 50);
  const literals = Buffer.from([5, 24, 28, 33, 41, 0]);
  const control = Buffer.from([
    0xaa, 0x20, 88, 0x00, 78, 0x00, 0x10, 58, 0x20, 54,
    0xa6, 0x60, 48, 0x00, 0x00,
  ]);
  const header = Buffer.alloc(2); header.writeUInt16LE(8);
  const compressedFrame = Buffer.concat([header, literals, control]);
  const arena = Buffer.concat([preceding, compressedFrame]);
  const compressed = decode_static_frame_trace(arena, preceding.length);
  const expectedPixels = Buffer.from([...Array.from({ length: 40 }, (_, index) => index + 1), 0, 41]);
  if (!compressed.pixels.equals(expectedPixels) ||
      !encode_static_frame_plan(compressed.pixels, compressed.plan, preceding).equals(compressedFrame))
    throw new Error("compressed static-sprite replay failed");
  const operations = compressed.plan.wrapper.operations!;
  if (operations.length !== 10 || !operations.some((operation) =>
    operation[0] === "c" && operation[2] === 18))
    throw new Error("mode-3 trace grammar failed");

  const damaged = Buffer.from(preceding); damaged[20] ^= 1;
  expect_failure(
    () => encode_static_frame_plan(compressed.pixels, compressed.plan, damaged),
    "mode-3 dictionary mismatch was accepted",
  );
  const badRun = Buffer.from(raw.pixels);
  badRun[0] = 0xe0;
  expect_failure(
    () => encode_static_frame_plan(badRun, raw.plan, Buffer.alloc(0)),
    "out-of-range zero-skip literal was accepted",
  );
  const truncated = Buffer.concat([preceding, compressedFrame.subarray(0, -2)]);
  expect_failure(
    () => decode_static_frame_trace(truncated, preceding.length),
    "truncated mode-3 terminator was accepted",
  );
  console.log("self-test=ok");
}

if (import.meta.main) {
  if (!process.argv.includes("--self-test"))
    throw new Error("usage: static_sprite_bank.ts --self-test");
  self_test();
}
