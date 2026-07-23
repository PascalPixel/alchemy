// Tool role: both; imported by tools/extract_audio.ts, tools/midi_roundtrip.ts, tools/midi_sequence.ts (+1 more); invoked by package.json, tools/midi_roundtrip.ts.
import { canonicalJson } from "./canonical_json.ts";
export type ControlName =
  "priority" | "tempo" | "key_shift" | "voice" | "volume" | "pan" |
  "pitch_bend" | "pitch_bend_range" | "lfo_speed" | "lfo_delay" |
  "modulation_depth" | "modulation_type" | "tuning";

export type SequenceEvent =
  ["label", string] |
  ["wait", number] |
  ["fine"] |
  ["goto", string] |
  ["pattern", string] |
  ["pattern_end"] |
  ["repeat", number, string] |
  ["note", number, ...number[]] |
  ["note_running", number, ...number[]] |
  ["note_end_running", number] |
  ["control_running", ControlName, number] |
  ["note_end"] |
  ["note_end", number] |
  [ControlName, number];

export interface StreamSegment {
  kind: "stream";
  label: string;
  events: SequenceEvent[];
}

export interface AlignSegment {
  kind: "align";
  boundary: number;
  fill: number;
}

export interface HeaderSegment {
  kind: "header";
  label: string;
  block_count: number;
  priority: number;
  reverb: number;
  tone_bank: string;
  tracks: string[];
}

export type SequenceSegment = StreamSegment | AlignSegment | HeaderSegment;

export interface SequenceSource {
  format: number;
  engine: "smsh-sequence";
  base: string | number;
  externals: Record<string, string | number>;
  layout: SequenceSegment[];
}

export interface SequenceReport {
  base: number;
  end: number;
  bytes: number;
  streams: number;
  tracks: number;
  events: number;
  labels: number;
}

interface EncodedStream {
  data: Buffer;
  labels: [string, number][];
  events: number;
}

interface DecodedEvent {
  offset: number;
  event: SequenceEvent;
}

interface DecodedReference {
  target: number;
  running: number | undefined;
}

interface DecodedBlock {
  events: DecodedEvent[];
  end: number;
  references: DecodedReference[];
}

const DURATIONS = [
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
  20, 21, 22, 23, 24, 28, 30, 32, 36, 40, 42, 44, 48, 52, 54, 56, 60, 64,
  66, 68, 72, 76, 78, 80, 84, 88, 90, 92, 96,
];

const CONTROL_OPCODES: Record<ControlName, number> = {
  priority: 0xba,
  tempo: 0xbb,
  key_shift: 0xbc,
  voice: 0xbd,
  volume: 0xbe,
  pan: 0xbf,
  pitch_bend: 0xc0,
  pitch_bend_range: 0xc1,
  lfo_speed: 0xc2,
  lfo_delay: 0xc3,
  modulation_depth: 0xc4,
  modulation_type: 0xc5,
  tuning: 0xc8,
};

const OPCODE_CONTROLS = new Map<number, ControlName>(
  Object.entries(CONTROL_OPCODES).map(([name, opcode]) => [opcode, name as ControlName]),
);

const SIGNED_CONTROLS = new Set<ControlName>(["key_shift", "pan", "pitch_bend", "tuning"]);

function integer(value: string | number, label: string): number {
  const result = typeof value === "string" ? Number(value) : value;
  if (!Number.isInteger(result)) throw new Error(`${label} must be an integer`);
  return result;
}

function byte(value: number, label: string): number {
  if (!Number.isInteger(value) || value < 0 || value > 0xff) throw new Error(`${label} does not fit u8`);
  return value;
}

function parameter(value: number, name: ControlName): number {
  if (SIGNED_CONTROLS.has(name)) {
    if (!Number.isInteger(value) || value < -128 || value > 127) throw new Error(`${name} does not fit s8`);
    return value & 0xff;
  }
  return byte(value, name);
}

function decodedParameter(value: number, name: ControlName): number {
  return SIGNED_CONTROLS.has(name) && value >= 0x80 ? value - 0x100 : value;
}

function romAddress(value: string | number, label: string): number {
  const result = integer(value, label);
  if (result < 0x08000000 || result >= 0x0a000000) throw new Error(`${label} is not a ROM address`);
  return result;
}

function symbol(name: unknown, label: string): string {
  if (typeof name !== "string" || !/^[a-z][a-z0-9_]*$/.test(name)) throw new Error(`${label} is invalid`);
  return name;
}

function durationIndex(ticks: number, label: string): number {
  const index = DURATIONS.indexOf(integer(ticks, label));
  if (index < 0) throw new Error(`${label} is not representable by the engine duration table`);
  return index;
}

function noteParameters(values: number[], label: string): number[] {
  if (values.length > 3) throw new Error(`${label} has more than three parameters`);
  return values.map((value, index) => {
    if (!Number.isInteger(value) || value < 0 || value >= 0x80) {
      throw new Error(`${label} parameter ${index} must be below 0x80`);
    }
    return value;
  });
}

function pointer(opcode: number, target: string, resolve?: (name: string) => number): Buffer {
  const result = Buffer.alloc(5);
  result[0] = opcode;
  result.writeUInt32LE(resolve ? resolve(target) : 0, 1);
  return result;
}

function encodeStream(events: SequenceEvent[], resolve?: (name: string) => number): EncodedStream {
  const parts: Buffer[] = [];
  const labels: [string, number][] = [];
  let offset = 0;
  let running: number | undefined;
  let eventCount = 0;
  for (const raw of events) {
    if (!Array.isArray(raw) || raw.length === 0 || typeof raw[0] !== "string") {
      throw new Error("sequence event is malformed");
    }
    const event = raw as SequenceEvent;
    const kind = event[0];
    if (kind === "label") {
      labels.push([symbol(event[1], "event label"), offset]);
      continue;
    }
    eventCount++;
    let encoded: Buffer;
    if (kind === "wait") {
      encoded = Buffer.from([0x80 + durationIndex(event[1], "wait duration")]);
    } else if (kind === "fine") {
      if (event.length !== 1) throw new Error("fine takes no parameters");
      encoded = Buffer.from([0xb1]);
    } else if (kind === "goto") {
      encoded = pointer(0xb2, symbol(event[1], "goto target"), resolve);
    } else if (kind === "pattern") {
      encoded = pointer(0xb3, symbol(event[1], "pattern target"), resolve);
    } else if (kind === "pattern_end") {
      if (event.length !== 1) throw new Error("pattern_end takes no parameters");
      encoded = Buffer.from([0xb4]);
    } else if (kind === "repeat") {
      const count = byte(event[1], "repeat count");
      const target = symbol(event[2], "repeat target");
      encoded = Buffer.alloc(6);
      encoded[0] = 0xb5;
      encoded[1] = count;
      encoded.writeUInt32LE(resolve ? resolve(target) : 0, 2);
    } else if (kind === "note") {
      const opcode = 0xcf + durationIndex(event[1], "note duration");
      const values = noteParameters(event.slice(2) as number[], "note");
      encoded = Buffer.from([opcode, ...values]);
      running = opcode;
    } else if (kind === "note_running") {
      const opcode = 0xcf + durationIndex(event[1], "running note duration");
      const values = noteParameters(event.slice(2) as number[], "running note");
      if (values.length === 0) throw new Error("running note emits no bytes");
      if (running !== undefined && running !== opcode) throw new Error("running note status differs from active status");
      encoded = Buffer.from(values);
      running = opcode;
    } else if (kind === "control_running") {
      const name = event[1];
      if (!(name in CONTROL_OPCODES) || CONTROL_OPCODES[name] < 0xbd) {
        throw new Error("control cannot use running status");
      }
      const value = parameter(event[2], name);
      if (value >= 0x80) throw new Error("running control parameter would be parsed as a command");
      if (running !== undefined && running !== CONTROL_OPCODES[name]) {
        throw new Error("running control status differs from active status");
      }
      encoded = Buffer.from([value]);
      running = CONTROL_OPCODES[name];
    } else if (kind === "note_end_running") {
      const value = noteParameters([event[1]], "running note_end")[0];
      if (running !== undefined && running !== 0xce) {
        throw new Error("running note_end status differs from active status");
      }
      encoded = Buffer.from([value]);
      running = 0xce;
    } else if (kind === "note_end") {
      if (event.length > 2) throw new Error("note_end has too many parameters");
      const values = event.length === 2 ? noteParameters([event[1]], "note_end") : [];
      encoded = Buffer.from([0xce, ...values]);
      running = 0xce;
    } else if (kind in CONTROL_OPCODES) {
      if (event.length !== 2) throw new Error(`${kind} requires one parameter`);
      const name = kind as ControlName;
      const opcode = CONTROL_OPCODES[name];
      encoded = Buffer.from([opcode, parameter(event[1], name)]);
      if (opcode >= 0xbd) running = opcode;
    } else {
      throw new Error(`unsupported sequence event: ${kind}`);
    }
    parts.push(encoded);
    offset += encoded.length;
  }
  return { data: Buffer.concat(parts), labels, events: eventCount };
}

function alignmentSize(offset: number, boundary: number): number {
  if (!Number.isInteger(boundary) || boundary < 2 || boundary > 0x100 || (boundary & (boundary - 1)) !== 0) {
    throw new Error("alignment boundary must be a power of two from 2 through 256");
  }
  return (-offset) & (boundary - 1);
}

function addLabel(labels: Map<string, number>, name: string, value: number): void {
  if (labels.has(name)) throw new Error(`duplicate local label: ${name}`);
  labels.set(name, value);
}

export function build_sequence(source: SequenceSource): [Buffer, SequenceReport] {
  if (source.format !== 1 || source.engine !== "smsh-sequence" || !Array.isArray(source.layout) || !source.externals) {
    throw new Error("unsupported sequence source");
  }
  const base = romAddress(source.base, "sequence base");
  const externals = new Map<string, number>();
  for (const [name, value] of Object.entries(source.externals)) {
    const checked = symbol(name, "external symbol");
    externals.set(checked, romAddress(value, `external ${checked}`));
  }
  const labels = new Map<string, number>();
  const measuredStreams = new Map<number, EncodedStream>();
  let offset = 0;
  let streamCount = 0;
  let trackCount = 0;
  let eventCount = 0;
  source.layout.forEach((segment, index) => {
    if (!segment || typeof segment !== "object") throw new Error(`layout segment ${index} is malformed`);
    if (segment.kind === "stream") {
      addLabel(labels, symbol(segment.label, "stream label"), base + offset);
      const measured = encodeStream(segment.events);
      measuredStreams.set(index, measured);
      for (const [name, innerOffset] of measured.labels) addLabel(labels, name, base + offset + innerOffset);
      offset += measured.data.length;
      streamCount++;
      eventCount += measured.events;
    } else if (segment.kind === "align") {
      byte(segment.fill, "alignment fill");
      offset += alignmentSize(base + offset, segment.boundary);
    } else if (segment.kind === "header") {
      addLabel(labels, symbol(segment.label, "header label"), base + offset);
      if (!Array.isArray(segment.tracks) || segment.tracks.length === 0 || segment.tracks.length > 16) {
        throw new Error("header track list is invalid");
      }
      if (byte(segment.block_count, "header block_count") !== 0) {
        throw new Error("nonzero sequence block_count is not yet supported");
      }
      byte(segment.priority, "header priority");
      byte(segment.reverb, "header reverb");
      offset += 8 + segment.tracks.length * 4;
      trackCount += segment.tracks.length;
    } else {
      throw new Error(`unsupported layout segment ${index}`);
    }
  });
  const used = new Set<string>();
  const resolve = (name: string): number => {
    const local = labels.get(name);
    if (local !== undefined) {
      used.add(name);
      return local;
    }
    const external = externals.get(name);
    if (external !== undefined) {
      used.add(name);
      return external;
    }
    throw new Error(`unknown sequence symbol: ${name}`);
  };
  const parts: Buffer[] = [];
  offset = 0;
  source.layout.forEach((segment, index) => {
    if (segment.kind === "stream") {
      const encoded = encodeStream(segment.events, resolve);
      if (encoded.data.length !== measuredStreams.get(index)!.data.length) throw new Error("stream size changed during resolution");
      parts.push(encoded.data);
      offset += encoded.data.length;
    } else if (segment.kind === "align") {
      const size = alignmentSize(base + offset, segment.boundary);
      parts.push(Buffer.alloc(size, segment.fill));
      offset += size;
    } else {
      const header = Buffer.alloc(8 + segment.tracks.length * 4);
      header[0] = segment.tracks.length;
      header[1] = byte(segment.block_count, "header block_count");
      header[2] = byte(segment.priority, "header priority");
      header[3] = byte(segment.reverb, "header reverb");
      header.writeUInt32LE(resolve(symbol(segment.tone_bank, "tone-bank symbol")), 4);
      segment.tracks.forEach((name, track) => header.writeUInt32LE(resolve(symbol(name, "track symbol")), 8 + track * 4));
      parts.push(header);
      offset += header.length;
    }
  });
  const unusedExternals = [...externals.keys()].filter((name) => !used.has(name));
  if (unusedExternals.length !== 0) throw new Error(`unused sequence externals: ${unusedExternals.join(", ")}`);
  const data = Buffer.concat(parts);
  return [data, {
    base,
    end: base + data.length,
    bytes: data.length,
    streams: streamCount,
    tracks: trackCount,
    events: eventCount,
    labels: labels.size,
  }];
}

function targetName(
  target: number,
  labels: Map<number, string>,
  externals: Record<string, string>,
  prefix: "loop" | "pattern" | "repeat",
): string {
  let name = labels.get(target);
  if (name !== undefined) return name;
  const used = new Set(labels.values());
  let index = 1;
  while (used.has(`${prefix}_${index}`)) index++;
  name = `${prefix}_${index}`;
  labels.set(target, name);
  if (target < 0x08000000 || target >= 0x0a000000) throw new Error("sequence target is not in ROM");
  externals[name] = `0x${target.toString(16).padStart(8, "0")}`;
  return name;
}

function decodeStream(
  data: Buffer,
  start: number,
  limit: number,
  labels: Map<number, string>,
  externals: Record<string, string>,
  initialRunning?: number,
  stopAtPatternEnd = false,
): [DecodedEvent[], number, DecodedReference[]] {
  const events: DecodedEvent[] = [];
  const references: DecodedReference[] = [];
  let cursor = start;
  let running = initialRunning;
  const take = (): number => {
    if (cursor >= limit) throw new Error("sequence stream reached its bound before termination");
    return data[cursor++];
  };
  const takePointer = (): number => {
    if (cursor + 4 > limit) throw new Error("truncated sequence pointer");
    const result = data.readUInt32LE(cursor);
    cursor += 4;
    return result;
  };
  while (cursor < limit) {
    const eventOffset = cursor;
    const opcode = take();
    let event: SequenceEvent;
    let terminal = false;
    if (opcode < 0x80) {
      if (running === undefined) throw new Error("sequence uses running status before establishing a command");
      if (running >= 0xcf) {
        const values = [opcode];
        while (values.length < 3 && cursor < limit && data[cursor] < 0x80) values.push(take());
        event = ["note_running", DURATIONS[running - 0xcf], ...values];
      } else if (running === 0xce) {
        event = ["note_end_running", opcode];
      } else {
        const name = OPCODE_CONTROLS.get(running);
        if (name === undefined || running < 0xbd) throw new Error("unsupported running-status command");
        event = ["control_running", name, decodedParameter(opcode, name)];
      }
    } else if (opcode <= 0xb0) {
      event = ["wait", DURATIONS[opcode - 0x80]];
    } else if (opcode === 0xb1) {
      event = ["fine"];
      terminal = true;
    } else if (opcode === 0xb2 || opcode === 0xb3) {
      const target = takePointer();
      references.push({ target, running });
      const kind = opcode === 0xb2 ? "goto" : "pattern";
      event = [kind, targetName(target, labels, externals, kind === "goto" ? "loop" : "pattern")];
    } else if (opcode === 0xb4) {
      event = ["pattern_end"];
      terminal = stopAtPatternEnd || data.subarray(cursor, limit).every((value) => value === 0);
    } else if (opcode === 0xb5) {
      const count = take();
      const target = takePointer();
      references.push({ target, running });
      event = ["repeat", count, targetName(target, labels, externals, "repeat")];
    } else if (opcode === 0xce) {
      const values = cursor < limit && data[cursor] < 0x80 ? [take()] : [];
      event = values.length ? ["note_end", values[0]] : ["note_end"];
      running = opcode;
    } else if (opcode >= 0xcf) {
      const values: number[] = [];
      while (values.length < 3 && cursor < limit && data[cursor] < 0x80) values.push(take());
      event = ["note", DURATIONS[opcode - 0xcf], ...values];
      running = opcode;
    } else {
      const name = OPCODE_CONTROLS.get(opcode);
      if (name === undefined) throw new Error(`unsupported sequence opcode 0x${opcode.toString(16)}`);
      const value = take();
      event = [name, decodedParameter(value, name)];
      if (opcode >= 0xbd) running = opcode;
    }
    events.push({ offset: eventOffset, event });
    if (terminal) return [events, cursor, references];
  }
  if (!stopAtPatternEnd && events.at(-1)?.event[0] === "pattern_end") {
    return [events, cursor, references];
  }
  throw new Error("sequence stream has no terminal command");
}

function alignForGap(address: number, target: number): number | undefined {
  for (const boundary of [4, 2, 8, 16, 32, 64, 128, 256]) {
    if (address + alignmentSize(address, boundary) === target) return boundary;
  }
  return undefined;
}

export function extract_sequence(
  rom: Uint8Array,
  startAddress: number,
  headerAddress: number,
  name: string,
): SequenceSource {
  const data = Buffer.from(rom);
  const base = romAddress(startAddress, "sequence start");
  const header = romAddress(headerAddress, "sequence header");
  const checkedName = symbol(name, "sequence name");
  if (header < base) throw new Error("sequence header precedes its streams");
  const headerOffset = header - 0x08000000;
  if (headerOffset < 0 || headerOffset + 8 > data.length) throw new Error("sequence header lies outside ROM");
  const tracks = data[headerOffset];
  const headerEnd = headerOffset + 8 + tracks * 4;
  if (tracks === 0 || tracks > 16 || headerEnd > data.length) throw new Error("sequence header has an invalid track count");
  const labels = new Map<number, string>();
  const trackPointers: number[] = [];
  for (let index = 0; index < tracks; index++) {
    const target = data.readUInt32LE(headerOffset + 8 + index * 4);
    if (target < base || target >= header) throw new Error("track pointer lies outside the claimed stream span");
    trackPointers.push(target);
    if (!labels.has(target)) labels.set(target, `track_${index + 1}`);
  }
  const uniqueTracks = [...new Set(trackPointers)].sort((left, right) => left - right);
  if (uniqueTracks[0] !== base) throw new Error("sequence start is not its first track");
  const externals: Record<string, string> = {
    tone_bank: `0x${data.readUInt32LE(headerOffset + 4).toString(16).padStart(8, "0")}`,
  };
  const decoded = new Map<number, DecodedBlock>();
  for (const [index, target] of uniqueTracks.entries()) {
    const limitAddress = uniqueTracks[index + 1] ?? header;
    const [events, end, references] = decodeStream(
      data,
      target - 0x08000000,
      limitAddress - 0x08000000,
      labels,
      externals,
    );
    decoded.set(target, { events, end: end + 0x08000000, references });
  }
  while (true) {
    const localOffsets = new Set<number>();
    for (const block of decoded.values()) for (const event of block.events) localOffsets.add(event.offset + 0x08000000);
    let added = false;
    for (const [target, label] of labels) {
      if (target < base || target >= header) continue;
      delete externals[label];
      if (decoded.has(target) || localOffsets.has(target)) continue;
      const containing = [...decoded.entries()].find(([start, block]) => start < target && target < block.end);
      if (containing !== undefined) throw new Error(`local target ${label} is not on an event boundary`);
      const callers = [...decoded.values()].flatMap((block) => block.references).filter((reference) => reference.target === target);
      const runningValues = new Set(callers.map((reference) => reference.running));
      if (runningValues.size > 1 && data[target - 0x08000000] < 0x80) {
        throw new Error(`local target ${label} has conflicting running-status inputs`);
      }
      const laterStarts = [...decoded.keys(), header].filter((value) => value > target).sort((left, right) => left - right);
      const limitAddress = laterStarts[0] ?? header;
      const [events, end, references] = decodeStream(
        data,
        target - 0x08000000,
        limitAddress - 0x08000000,
        labels,
        externals,
        callers[0]?.running,
        true,
      );
      decoded.set(target, { events, end: end + 0x08000000, references });
      added = true;
      break;
    }
    if (!added) break;
  }
  const finalOffsets = new Set<number>();
  for (const block of decoded.values()) for (const event of block.events) finalOffsets.add(event.offset + 0x08000000);
  for (const [target, label] of labels) {
    if (target < base || target >= header) continue;
    if (!decoded.has(target) && !finalOffsets.has(target)) {
      throw new Error(`local target ${label} is outside decoded event boundaries`);
    }
    delete externals[label];
  }
  const layout: SequenceSegment[] = [];
  let cursor = base;
  for (const target of [...decoded.keys()].sort((left, right) => left - right)) {
    if (target !== cursor) {
      const boundary = alignForGap(cursor, target);
      const gap = data.subarray(cursor - 0x08000000, target - 0x08000000);
      if (boundary === undefined || gap.some((value) => value !== 0)) throw new Error("non-semantic bytes separate sequence streams");
      layout.push({ kind: "align", boundary, fill: 0 });
    }
    const block = decoded.get(target)!;
    const events: SequenceEvent[] = [];
    for (const decodedEvent of block.events) {
      const address = decodedEvent.offset + 0x08000000;
      const innerLabel = labels.get(address);
      if (innerLabel !== undefined && address !== target) events.push(["label", innerLabel]);
      events.push(decodedEvent.event);
    }
    layout.push({ kind: "stream", label: labels.get(target)!, events });
    cursor = block.end;
  }
  if (cursor !== header) {
    const boundary = alignForGap(cursor, header);
    const gap = data.subarray(cursor - 0x08000000, header - 0x08000000);
    if (boundary === undefined || gap.some((value) => value !== 0)) throw new Error("non-semantic bytes precede sequence header");
    layout.push({ kind: "align", boundary, fill: 0 });
  }
  layout.push({
    kind: "header",
    label: checkedName,
    block_count: data[headerOffset + 1],
    priority: data[headerOffset + 2],
    reverb: data[headerOffset + 3],
    tone_bank: "tone_bank",
    tracks: trackPointers.map((target) => labels.get(target)!),
  });
  return {
    format: 1,
    engine: "smsh-sequence",
    base: `0x${base.toString(16).padStart(8, "0")}`,
    externals,
    layout,
  };
}

export function self_test_sequence(): void {
  const simple: SequenceSource = {
    format: 1,
    engine: "smsh-sequence",
    base: "0x08000100",
    externals: { tone_bank: "0x08001000" },
    layout: [
      {
        kind: "stream",
        label: "track_1",
        events: [
          ["volume", 127], ["key_shift", 0], ["tempo", 100], ["voice", 70],
          ["modulation_depth", 5], ["note", 23, 60, 127], ["wait", 23], ["fine"],
        ],
      },
      { kind: "align", boundary: 4, fill: 0 },
      {
        kind: "header", label: "sound_test", block_count: 0, priority: 100,
        reverb: 0, tone_bank: "tone_bank", tracks: ["track_1"],
      },
    ],
  };
  const [simpleBuilt, simpleReport] = build_sequence(simple);
  const syntheticRom = Buffer.alloc(0x100 + simpleBuilt.length);
  simpleBuilt.copy(syntheticRom, 0x100);
  const extracted = extract_sequence(syntheticRom, 0x08000100, 0x08000110, "sound_test");
  const [roundTrip] = build_sequence(extracted);
  if (!roundTrip.equals(simpleBuilt) || simpleReport.bytes !== 28 || simpleReport.events !== 8) {
    throw new Error("simple sequence round-trip failed");
  }

  const controlFlow: SequenceSource = {
    format: 1,
    engine: "smsh-sequence",
    base: "0x08000200",
    externals: { tone_bank: "0x08002000" },
    layout: [
      {
        kind: "stream",
        label: "track_1",
        events: [
          ["voice", 3], ["control_running", "voice", 4], ["label", "loop"],
          ["note", 10, 60, 100], ["wait", 10],
          ["note_running", 10, 62], ["pattern", "pattern_a"], ["repeat", 2, "loop"],
          ["goto", "loop"],
        ],
      },
      {
        kind: "stream", label: "pattern_a",
        events: [["volume", 90], ["pattern_end"]],
      },
      { kind: "align", boundary: 4, fill: 0 },
      {
        kind: "header", label: "sound_flow", block_count: 0, priority: 9,
        reverb: 128, tone_bank: "tone_bank", tracks: ["track_1"],
      },
    ],
  };
  const [flowBuilt, flowReport] = build_sequence(controlFlow);
  const patternOffset = flowBuilt.indexOf(0xb3);
  const repeatOffset = flowBuilt.indexOf(0xb5);
  const gotoOffset = flowBuilt.indexOf(0xb2);
  if (patternOffset < 0 || repeatOffset < 0 || gotoOffset < 0 ||
      flowBuilt.readUInt32LE(patternOffset + 1) <= 0x08000200 ||
      flowBuilt.readUInt32LE(repeatOffset + 2) !== 0x08000203 ||
      flowBuilt.readUInt32LE(gotoOffset + 1) !== 0x08000203 ||
      flowReport.tracks !== 1) {
    throw new Error("symbolic sequence control-flow test failed");
  }
  const flowRom = Buffer.alloc(0x200 + flowBuilt.length);
  flowBuilt.copy(flowRom, 0x200);
  const flowExtracted = extract_sequence(
    flowRom,
    0x08000200,
    0x08000200 + flowBuilt.length - 12,
    "sound_flow",
  );
  const [flowRoundTrip] = build_sequence(flowExtracted);
  if (!flowRoundTrip.equals(flowBuilt)) throw new Error("control-flow sequence round-trip failed");
  const runningEnd: SequenceSource = {
    format: 1,
    engine: "smsh-sequence",
    base: "0x08000300",
    externals: { tone_bank: "0x08002000" },
    layout: [
      {
        kind: "stream",
        label: "track_1",
        events: [["note_end", 60], ["note_end_running", 61], ["fine"]],
      },
      { kind: "align", boundary: 4, fill: 0 },
      {
        kind: "header", label: "sound_end", block_count: 0, priority: 0,
        reverb: 0, tone_bank: "tone_bank", tracks: ["track_1"],
      },
    ],
  };
  const [runningEndBuilt] = build_sequence(runningEnd);
  const runningEndRom = Buffer.alloc(0x300 + runningEndBuilt.length);
  runningEndBuilt.copy(runningEndRom, 0x300);
  const runningEndExtracted = extract_sequence(
    runningEndRom,
    0x08000300,
    0x08000304,
    "sound_end",
  );
  if (!build_sequence(runningEndExtracted)[0].equals(runningEndBuilt)) {
    throw new Error("running note-end round-trip failed");
  }
  console.log("self-test=ok");
}

function option(args: string[], name: string): string {
  const index = args.indexOf(name);
  if (index < 0 || index + 1 >= args.length) throw new Error(`${name} is required`);
  return args[index + 1];
}

async function main(args: string[]): Promise<void> {
  if (args.includes("--self-test")) {
    self_test_sequence();
    return;
  }
  if (args[0] !== "extract-sequence" || !args[1]) {
    console.log("usage: music_sequence.ts extract-sequence ROM --start ADDRESS --header ADDRESS --name NAME -o OUTPUT");
    console.log("       music_sequence.ts --self-test");
    return;
  }
  const source = extract_sequence(
    new Uint8Array(await Bun.file(args[1]).arrayBuffer()),
    integer(option(args, "--start"), "--start"),
    integer(option(args, "--header"), "--header"),
    option(args, "--name"),
  );
  const output = option(args, "-o");
  await Bun.write(output, canonicalJson(source) + "\n");
  const [built, report] = build_sequence(source);
  console.log(`base=0x${report.base.toString(16)} end=0x${report.end.toString(16)} streams=${report.streams} tracks=${report.tracks} events=${report.events} bytes=${built.length}`);
}

if (import.meta.main) await main(Bun.argv.slice(2));
