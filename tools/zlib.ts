interface DeflateOptions {
  level?: 6 | 9;
}

function bytes(value: Uint8Array | ArrayBuffer): Buffer {
  return Buffer.from(value instanceof ArrayBuffer ? new Uint8Array(value) : value);
}

export function crc32(data: Uint8Array): number {
  return Bun.hash.crc32(data) >>> 0;
}

export function deflateSync(data: Uint8Array, options: DeflateOptions = {}): Buffer {
  const level = options.level ?? 6;
  const header = level === 6 ? 0x789c : 0x78da;
  const prefix = Buffer.allocUnsafe(2);
  const suffix = Buffer.allocUnsafe(4);
  prefix.writeUInt16BE(header);
  suffix.writeUInt32BE(Bun.hash.adler32(data) >>> 0);
  return Buffer.concat([prefix, bytes(Bun.deflateSync(data, { level })), suffix]);
}

export function inflateSync(data: Uint8Array): Buffer {
  const input = bytes(data);
  if (input.length < 6) throw new Error("invalid zlib stream");
  const header = input.readUInt16BE(0);
  if ((header & 0x0f00) !== 0x0800 || header % 31 !== 0 || (header & 0x20) !== 0)
    throw new Error("invalid zlib header");
  const output = bytes(Bun.inflateSync(input.subarray(2, -4)));
  if ((Bun.hash.adler32(output) >>> 0) !== input.readUInt32BE(input.length - 4))
    throw new Error("incorrect zlib checksum");
  return output;
}
