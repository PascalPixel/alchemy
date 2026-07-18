export interface WaveRecordSource {
  frequency: string | number;
  loop_start: number | null;
  size: string | number;
}

function integer(value: string | number, label: string): number {
  const result = typeof value === "string" ? Number(value) : value;
  if (!Number.isInteger(result) || result < 0) throw new Error(`${label} must be a nonnegative integer`);
  return result;
}

function fourCc(data: Buffer, offset: number, expected: string): void {
  if (data.toString("ascii", offset, offset + 4) !== expected) {
    throw new Error(`WAV is missing ${expected}`);
  }
}

export function wavFromSignedPcm(samples: Uint8Array, rate: number): Buffer {
  if (!Number.isInteger(rate) || rate < 1 || rate > 0xffffffff) throw new Error("invalid WAV rate");
  const result = Buffer.alloc(44 + samples.length);
  result.write("RIFF", 0, "ascii");
  result.writeUInt32LE(36 + samples.length, 4);
  result.write("WAVE", 8, "ascii");
  result.write("fmt ", 12, "ascii");
  result.writeUInt32LE(16, 16);
  result.writeUInt16LE(1, 20);
  result.writeUInt16LE(1, 22);
  result.writeUInt32LE(rate, 24);
  result.writeUInt32LE(rate, 28);
  result.writeUInt16LE(1, 32);
  result.writeUInt16LE(8, 34);
  result.write("data", 36, "ascii");
  result.writeUInt32LE(samples.length, 40);
  for (let index = 0; index < samples.length; index++) result[44 + index] = (samples[index] + 128) & 0xff;
  return result;
}

export function signedPcmFromWav(data: Buffer, expectedRate: number): Buffer {
  if (data.length < 44) throw new Error("WAV is truncated");
  fourCc(data, 0, "RIFF");
  fourCc(data, 8, "WAVE");
  fourCc(data, 12, "fmt ");
  fourCc(data, 36, "data");
  if (data.readUInt32LE(4) !== data.length - 8 || data.readUInt32LE(16) !== 16 ||
      data.readUInt16LE(20) !== 1 || data.readUInt16LE(22) !== 1 ||
      data.readUInt32LE(24) !== expectedRate || data.readUInt32LE(28) !== expectedRate ||
      data.readUInt16LE(32) !== 1 || data.readUInt16LE(34) !== 8 ||
      data.readUInt32LE(40) !== data.length - 44) {
    throw new Error("WAV is not canonical mono 8-bit PCM");
  }
  const result = Buffer.alloc(data.length - 44);
  for (let index = 0; index < result.length; index++) result[index] = (data[44 + index] - 128) & 0xff;
  return result;
}

export function buildWaveRecord(source: WaveRecordSource, wav: Buffer): [Buffer, Record<string, number | boolean | null>] {
  const frequency = integer(source.frequency, "wave frequency");
  const size = integer(source.size, "wave record size");
  const rate = Math.round(frequency / 1024);
  const samples = signedPcmFromWav(wav, rate);
  if (samples.length === 0) throw new Error("wave record has no samples");
  const loop = source.loop_start === null ? null : integer(source.loop_start, "wave loop start");
  if (loop !== null && loop >= samples.length) throw new Error("wave loop starts beyond sample data");
  if (size < 16 + samples.length || size - 16 - samples.length > 3) {
    throw new Error("wave record size has invalid alignment");
  }
  const result = Buffer.alloc(size);
  result.writeUInt32LE(loop === null ? 0 : 0x40000000, 0);
  result.writeUInt32LE(frequency, 4);
  result.writeUInt32LE(loop ?? 0, 8);
  result.writeUInt32LE(samples.length - 1, 12);
  samples.copy(result, 16);
  return [result, {
    samples: samples.length,
    rate,
    frequency,
    looped: loop !== null,
    loop_start: loop,
  }];
}

export function selfTest(): void {
  const samples = Buffer.from([0x80, 0xff, 0, 1, 0x7f]);
  const wav = wavFromSignedPcm(samples, 22050);
  const [built, report] = buildWaveRecord({ frequency: 22050 * 1024, loop_start: 2, size: 24 }, wav);
  if (built.length !== 24 || built.readUInt32LE(0) !== 0x40000000 ||
      built.readUInt32LE(12) !== 4 || !built.subarray(16, 21).equals(samples) ||
      report.samples !== 5) {
    throw new Error("wave-record self-test failed");
  }
  console.log("self-test=ok");
}

if (import.meta.main) {
  if (Bun.argv.slice(2).includes("--self-test")) selfTest();
  else console.log("usage: audio_wave.ts --self-test");
}
