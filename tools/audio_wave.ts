// Tool role: both; imported by tools/build_assets.ts, tools/build_soundfont.ts, tools/extract_audio.ts (+1 more); invoked by package.json, tools/build_soundfont.ts.
export interface WaveRecordSource {
  frequency: string | number;
  loop_start: number | null;
  size: string | number;
  header?: ExactWaveHeaderSource;
  padding?: ExactWavePaddingSource;
}

export interface ExactWaveHeaderSource {
  control: string | number;
  frequency: string | number;
  loop_start: string | number;
  sample_count: string | number;
}

export interface ExactWavePaddingSource {
  size: string | number;
  fill: string | number;
}

export interface ProbedWaveRecord {
  header: ExactWaveHeaderSource;
  padding: ExactWavePaddingSource;
  samples: Buffer;
}

function integer(value: string | number, label: string): number {
  const result = typeof value === "string" ? Number(value) : value;
  if (!Number.isInteger(result) || result < 0) throw new Error(`${label} must be a nonnegative integer`);
  return result;
}

function uint32(value: string | number, label: string): number {
  const result = integer(value, label);
  if (result > 0xffffffff) throw new Error(`${label} must fit an unsigned word`);
  return result;
}

function byte(value: string | number, label: string): number {
  const result = integer(value, label);
  if (result > 0xff) throw new Error(`${label} must fit an unsigned byte`);
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
  const exact = source.header;
  const frequency = uint32(exact?.frequency ?? source.frequency, "wave frequency");
  if (exact !== undefined && uint32(source.frequency, "wave catalog frequency") !== frequency) {
    throw new Error("wave catalog frequency differs from exact header");
  }
  const size = integer(source.size, "wave record size");
  const rate = Math.round(frequency / 1024);
  const samples = signedPcmFromWav(wav, rate);
  if (samples.length === 0) throw new Error("wave record has no samples");
  const control = exact === undefined ? (source.loop_start === null ? 0 : 0x40000000) :
    uint32(exact.control, "wave control");
  const loopStart = exact === undefined ?
    (source.loop_start === null ? 0 : uint32(source.loop_start, "wave loop start")) :
    uint32(exact.loop_start, "wave loop start");
  const looped = (control & 0xc0000000) !== 0;
  const catalogLoop = source.loop_start === null ? null : uint32(source.loop_start, "wave catalog loop start");
  if (exact !== undefined && catalogLoop !== (looped ? loopStart : null)) {
    throw new Error("wave catalog loop differs from exact header");
  }
  if (looped && loopStart >= samples.length) throw new Error("wave loop starts beyond sample data");
  if (exact !== undefined && integer(exact.sample_count, "wave sample count") !== samples.length) {
    throw new Error("wave sample count differs from WAV data");
  }
  const paddingSize = size - 16 - samples.length;
  if (paddingSize < 0) throw new Error("wave record size is shorter than its sample data");
  let paddingFill = 0;
  if (exact === undefined) {
    if (paddingSize > 3) throw new Error("wave record size has invalid alignment");
  } else {
    if (source.padding === undefined ||
        integer(source.padding.size, "wave padding size") !== paddingSize) {
      throw new Error("wave padding size differs from record extent");
    }
    paddingFill = byte(source.padding.fill, "wave padding fill");
  }
  const result = Buffer.alloc(size, paddingFill);
  result.writeUInt32LE(control, 0);
  result.writeUInt32LE(frequency, 4);
  result.writeUInt32LE(loopStart, 8);
  result.writeUInt32LE(samples.length - 1, 12);
  samples.copy(result, 16);
  return [result, {
    samples: samples.length,
    rate,
    frequency,
    control,
    looped,
    loop_start: looped ? loopStart : null,
    padding_bytes: paddingSize,
    padding_fill: paddingFill,
  }];
}

export function probeWaveRecord(data: Buffer, offset: number, size: number): ProbedWaveRecord {
  if (!Number.isInteger(offset) || !Number.isInteger(size) || offset < 0 || size < 16 || offset + size > data.length) {
    throw new Error("wave probe extent is invalid");
  }
  const control = data.readUInt32LE(offset);
  const frequency = data.readUInt32LE(offset + 4);
  const loopStart = data.readUInt32LE(offset + 8);
  const sampleCount = data.readUInt32LE(offset + 12) + 1;
  if (frequency === 0) throw new Error("wave frequency is zero");
  if (sampleCount > size - 16) throw new Error("wave samples extend beyond the record");
  if ((control & 0xc0000000) !== 0 && loopStart >= sampleCount) {
    throw new Error("wave loop starts beyond sample data");
  }
  const samples = Buffer.from(data.subarray(offset + 16, offset + 16 + sampleCount));
  const tail = data.subarray(offset + 16 + sampleCount, offset + size);
  const fill = tail[0] ?? 0;
  if (tail.some((value) => value !== fill)) throw new Error("wave padding is not uniform");
  return {
    header: {
      control: `0x${control.toString(16).padStart(8, "0")}`,
      frequency,
      loop_start: loopStart,
      sample_count: sampleCount,
    },
    padding: { size: tail.length, fill },
    samples,
  };
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
  const exactSamples = Buffer.from([0x80, 0xc0, 0, 0x40, 0x7f, 0x40, 0, 0xc0]);
  const exactWav = wavFromSignedPcm(exactSamples, 4096);
  const exactSource: WaveRecordSource = {
    frequency: 4096 * 1024,
    loop_start: 0,
    size: 28,
    header: {
      control: "0xc0000000",
      frequency: 4096 * 1024,
      loop_start: 0,
      sample_count: exactSamples.length,
    },
    padding: { size: 4, fill: 0x5a },
  };
  const [exactBuilt] = buildWaveRecord(exactSource, exactWav);
  const probed = probeWaveRecord(exactBuilt, 0, exactBuilt.length);
  if (exactBuilt.readUInt32LE(0) !== 0xc0000000 ||
      !exactBuilt.subarray(16, 24).equals(exactSamples) ||
      !exactBuilt.subarray(24).equals(Buffer.alloc(4, 0x5a)) ||
      probed.header.control !== "0xc0000000" || probed.header.sample_count !== 8 ||
      probed.padding.size !== 4 || probed.padding.fill !== 0x5a ||
      !probed.samples.equals(exactSamples)) {
    throw new Error("exact wave-record self-test failed");
  }
  console.log("self-test=ok");
}

if (import.meta.main) {
  if (Bun.argv.slice(2).includes("--self-test")) selfTest();
  else console.log("usage: audio_wave.ts --self-test");
}
