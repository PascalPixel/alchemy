#!/usr/bin/env bun
import {
  buildWaveRecord,
  probeWaveRecord,
  wavFromSignedPcm,
  type ProbedWaveRecord,
  type WaveRecordSource,
} from "./audio_wave.ts";

interface ToneRecord {
  kind?: unknown;
  sample?: unknown;
}

interface ToneBank {
  records?: unknown;
}

interface ToneDocument {
  banks?: unknown;
}

interface ProbedEntry extends WaveRecordSource {
  name: string;
  address: string;
  source: string;
  header: ProbedWaveRecord["header"];
  padding: ProbedWaveRecord["padding"];
  samples: Buffer;
}

function option(args: string[], name: string): string {
  const index = args.indexOf(name);
  if (index < 0 || index + 1 >= args.length) throw new Error(`${name} is required`);
  return args[index + 1];
}

function hexadecimal(value: number): string {
  return `0x${value.toString(16).padStart(8, "0")}`;
}

function integer(value: string, label: string): number {
  const result = Number(value);
  if (!Number.isSafeInteger(result) || result < 0) throw new Error(`${label} is invalid`);
  return result;
}

export function referencedWavePointers(document: ToneDocument, start: number, end: number): number[] {
  if (!Array.isArray(document.banks) || !Number.isSafeInteger(start) || !Number.isSafeInteger(end) || start >= end) {
    throw new Error("wave pointer range is invalid");
  }
  const pointers = new Set<number>();
  for (const bank of document.banks as ToneBank[]) {
    if (!Array.isArray(bank.records)) throw new Error("tone bank records are invalid");
    for (const record of bank.records as ToneRecord[]) {
      if (record.kind !== "pcm" || typeof record.sample !== "string") continue;
      const pointer = Number(record.sample);
      if (Number.isSafeInteger(pointer) && pointer >= start && pointer < end) pointers.add(pointer);
    }
  }
  const result = [...pointers].sort((left, right) => left - right);
  if (result.length === 0 || result[0] !== start) throw new Error("wave range does not begin at a referenced record");
  return result;
}

export function probeReferencedWaveRange(
  image: Buffer,
  imageBase: number,
  document: ToneDocument,
  start: number,
  end: number,
): ProbedEntry[] {
  const pointers = referencedWavePointers(document, start, end);
  return pointers.map((address, index) => {
    const next = pointers[index + 1] ?? end;
    const size = next - address;
    const probe = probeWaveRecord(image, address - imageBase, size);
    const suffix = address.toString(16).padStart(8, "0");
    return {
      name: `wave_${suffix}`,
      address: hexadecimal(address),
      size: hexadecimal(size),
      frequency: probe.header.frequency,
      loop_start: (Number(probe.header.control) & 0xc0000000) !== 0 ? Number(probe.header.loop_start) : null,
      header: probe.header,
      padding: probe.padding,
      source: `wave_${suffix}.pcm8.wav`,
      samples: probe.samples,
    };
  });
}

function serializable(entries: ProbedEntry[]): Record<string, unknown>[] {
  return entries.map(({ samples: _samples, ...entry }) => entry);
}

export async function writeWaveFragment(directory: string, start: number, end: number, entries: ProbedEntry[]): Promise<void> {
  await Bun.$`mkdir -p ${directory}`.quiet();
  for (const entry of entries) {
    const rate = Math.round(Number(entry.header.frequency) / 1024);
    await Bun.write(`${directory}/${entry.source}`, wavFromSignedPcm(entry.samples, rate));
  }
  const fragment = {
    format: 1,
    engine: "smsh-pcm-wave-series",
    address: hexadecimal(start),
    end: hexadecimal(end),
    waves: serializable(entries),
  };
  await Bun.write(`${directory}/index.json`, JSON.stringify(fragment, null, 2) + "\n");
}

async function selfTest(): Promise<void> {
  const addresses = [0x0811dac8, 0x0811dae0, 0x0811daf8, 0x0811db10, 0x0811db24];
  const end = 0x0811db38;
  const parts: Buffer[] = [];
  for (const [index, address] of addresses.entries()) {
    const size = (addresses[index + 1] ?? end) - address;
    const samples = Buffer.from(Array.from({ length: size - 16 }, (_, sample) => (sample * 17 + index) & 0xff));
    const frequency = 4096 * 1024;
    const source: WaveRecordSource = {
      frequency,
      loop_start: 0,
      size,
      header: {
        control: index & 1 ? "0xc0000000" : "0x40000000",
        frequency,
        loop_start: 0,
        sample_count: samples.length,
      },
      padding: { size: 0, fill: 0 },
    };
    parts.push(buildWaveRecord(source, wavFromSignedPcm(samples, 4096))[0]);
  }
  const tones = {
    banks: [{ records: addresses.map((sample) => ({ kind: "pcm", sample: hexadecimal(sample) })) }],
  };
  const entries = probeReferencedWaveRange(Buffer.concat(parts), addresses[0], tones, addresses[0], end);
  if (entries.length !== 5 || entries.map((entry) => Number(entry.size)).join(",") !== "24,24,24,20,20" ||
      entries[1].header.control !== "0xc0000000" || entries[4].header.sample_count !== 4) {
    throw new Error("referenced wave-range probe self-test failed");
  }
  console.log("self-test=ok");
}

async function main(args: string[]): Promise<void> {
  if (args.includes("--self-test")) {
    await selfTest();
    return;
  }
  if (args.length === 0 || args.includes("-h") || args.includes("--help")) {
    console.log("usage: probe_audio_waves.ts ROM --tones FILE --start ADDRESS --end ADDRESS [-o DIRECTORY]");
    return;
  }
  const romPath = args[0];
  const tonePath = option(args, "--tones");
  const start = integer(option(args, "--start"), "wave start");
  const end = integer(option(args, "--end"), "wave end");
  const image = Buffer.from(await Bun.file(romPath).arrayBuffer());
  const tones = await Bun.file(tonePath).json() as ToneDocument;
  const entries = probeReferencedWaveRange(image, 0x08000000, tones, start, end);
  const outputIndex = args.indexOf("-o");
  if (outputIndex >= 0) {
    if (outputIndex + 1 >= args.length) throw new Error("-o is required");
    await writeWaveFragment(args[outputIndex + 1], start, end, entries);
  } else {
    console.log(JSON.stringify({
      format: 1,
      engine: "smsh-pcm-wave-series",
      address: hexadecimal(start),
      end: hexadecimal(end),
      waves: serializable(entries),
    }, null, 2));
  }
}

if (import.meta.main) await main(Bun.argv.slice(2));
