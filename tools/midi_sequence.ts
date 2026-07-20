// midi_sequence.ts — MIDI + 逸脱サイドカー コーデック
//
// 目的: エンジン固有 sequence-JSON の冗長な列挙を、追跡対象の標準 MIDI
//       ファイル (.mid) と最小の「逸脱サイドカー (deviation sidecar)」に
//       置き換える。.mid が正典コア (canonical core) を担い、サイドカーは
//       ROM が既定規則から外れた箇所だけを記録する。両者からエンジンバイト列を
//       バイト完全に復元できる。
//
// ── 既定規則 (default rules) ────────────────────────────────────────
//   .mid から SequenceSource を復元するときの正準化方針。この既定に一致する
//   曲はサイドカーを持たない (163/260 曲)。
//     1. 走行ステータス (running-status): 直前の音符命令と同じデュレーション
//        opcode で、かつ 1 バイト以上のパラメータを出す音符は "note_running"
//        にする (opcode バイトを省略)。それ以外は明示 "note"。
//     2. 音符パラメータ省略: 走行中の key / velocity と一致する末尾パラメータは
//        省略する (velocity が一致すれば velocity を、key も一致すれば key も)。
//     3. wait のトークン分割: デュレーション表を「大きい順の貪欲法」で分割する。
//   これらは music_sequence.ts のエンコーダが許す最小表現であり、ROM の
//   支配的な符号化選択と一致することを 260 曲で確認済み。
//
// ── サイドカー (逸脱記録) ──────────────────────────────────────────
//   ROM が既定と異なる箇所のみを、トラック単位・イベント索引で記録する。
//   逸脱は既定ストリーム上の絶対索引に対するスプライス操作:
//       [start, deleteCount, ...replacementEvents]
//   ・音符表現の逸脱: [i, 1, <ROM の音符イベント>] (数・位置は不変)。
//   ・wait 分割の逸脱: [i, runLen, <ROM の wait トークン列>] (数が変わりうる)。
//   索引はすべて「既定ストリーム」を固定基準にとるため、置換で長さが変わっても
//   互いに安定 (ずれない)。逸脱ゼロのサイドカーはファイルごと省略する。
//
// ── 検証 (validator) ──────────────────────────────────────────────
//   .mid から既定ストリームを再計算し、サイドカーが記録するトラックごとの
//   既定イベント数とハッシュに一致するかを確認する。逸脱スパンが範囲外・重複・
//   非整列なら失敗する。.mid とサイドカーの不整合を大声で (throw で) 弾く。
//
// 使い方:  bun tools/midi_sequence.ts convert-all      # 旧 JSON から .mid+サイドカー生成
//          bun tools/midi_sequence.ts validate-all      # 追跡中の .mid+サイドカーを検証
//          bun tools/midi_sequence.ts --self-test

import { createHash } from "node:crypto";
import { readdirSync, readFileSync, writeFileSync, mkdirSync, existsSync } from "node:fs";
import { dirname, join } from "node:path";
import { sequenceToMidi, midiToSequence } from "./midi_roundtrip.ts";
import {
  build_sequence,
  type SequenceSource,
  type SequenceSegment,
  type SequenceEvent,
  type SequenceReport,
} from "./music_sequence.ts";

// エンジンのデュレーション表 (music_sequence.ts と同一)。
const DURATIONS = [
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
  20, 21, 22, 23, 24, 28, 30, 32, 36, 40, 42, 44, 48, 52, 54, 56, 60, 64,
  66, 68, 72, 76, 78, 80, 84, 88, 90, 92, 96,
];
const durationIndex = (ticks: number): number => DURATIONS.indexOf(ticks);

// 走行ステータスを設定するコントロール命令の opcode (>=0xbd)。
const CONTROL_OPCODES: Record<string, number> = {
  priority: 0xba, tempo: 0xbb, key_shift: 0xbc, voice: 0xbd, volume: 0xbe,
  pan: 0xbf, pitch_bend: 0xc0, pitch_bend_range: 0xc1, lfo_speed: 0xc2,
  lfo_delay: 0xc3, modulation_depth: 0xc4, modulation_type: 0xc5, tuning: 0xc8,
};

// ── サイドカーの型 ──────────────────────────────────────────────────
// スプライス操作: [start, deleteCount, ...replacementEvents]。
export type Deviation = [number, number, ...SequenceEvent[]];

export interface SidecarTrack {
  events: number; // 既定ストリームのイベント数 (ドリフト検出用)
  hash: string; // 既定ストリームのハッシュ (ドリフト検出用)
  deviations: Deviation[];
}

export interface Sidecar {
  format: 1;
  engine: "smsh-sequence-sidecar";
  name: string;
  tracks: Record<string, SidecarTrack>;
}

// ── 既定規則による音符表現の正準化 ──────────────────────────────────
// midiToSequence が返す「素の音符 (fresh note・key/vel 明示)・貪欲 wait」列を
// 受け取り、走行状態を追って最小表現に畳み込む。
export function greedyDefault(events: SequenceEvent[]): SequenceEvent[] {
  const out: SequenceEvent[] = [];
  let running: number | undefined; // 現在の走行ステータス opcode
  let runKey = 0;
  let runVel = 0;
  for (const event of events) {
    const kind = event[0];
    if (kind === "note" || kind === "note_running") {
      const duration = event[1] as number;
      const materialized = event.slice(2) as number[]; // [key, velocity] (materialize 済み)
      const effectiveKey = materialized[0]!;
      const effectiveVel = materialized[1]!;
      const opcode = 0xcf + durationIndex(duration);
      let params: number[];
      if (effectiveVel !== runVel) params = [effectiveKey, effectiveVel];
      else if (effectiveKey !== runKey) params = [effectiveKey];
      else params = [];
      if (running === opcode && params.length >= 1) {
        out.push(["note_running", duration, ...params]);
      } else {
        out.push(["note", duration, ...params]);
      }
      running = opcode;
      runKey = effectiveKey;
      runVel = effectiveVel;
    } else if (kind in CONTROL_OPCODES) {
      out.push(event);
      if (CONTROL_OPCODES[kind]! >= 0xbd) running = CONTROL_OPCODES[kind]!;
    } else if (kind === "control_running") {
      out.push(event);
      running = CONTROL_OPCODES[event[1] as string]!;
    } else if (kind === "note_end" || kind === "note_end_running") {
      out.push(event);
      running = 0xce;
    } else {
      // wait / label / fine / goto / pattern / pattern_end / repeat: 走行状態は不変。
      out.push(event);
    }
  }
  return out;
}

// ── ストリームを単位 (unit) 列に分ける: GAP(連続 wait) か EV(非 wait) ──
interface Unit {
  kind: "GAP" | "EV";
  start: number; // 既定ストリーム上の絶対開始索引
  length: number; // 消費するイベント数
  tokens?: number[]; // GAP: wait トークン列
  event?: SequenceEvent; // EV: そのイベント
}

function toUnits(events: SequenceEvent[]): Unit[] {
  const units: Unit[] = [];
  let index = 0;
  while (index < events.length) {
    if (events[index]![0] === "wait") {
      const tokens: number[] = [];
      let end = index;
      while (end < events.length && events[end]![0] === "wait") {
        tokens.push(events[end]![1] as number);
        end += 1;
      }
      units.push({ kind: "GAP", start: index, length: end - index, tokens });
      index = end;
    } else {
      units.push({ kind: "EV", start: index, length: 1, event: events[index]! });
      index += 1;
    }
  }
  return units;
}

const sum = (values: number[]): number => values.reduce((a, b) => a + b, 0);

// ── 既定ストリーム vs ROM ストリームの差分 → 逸脱スプライス列 ──────────
function diffStream(defaultStream: SequenceEvent[], romStream: SequenceEvent[]): Deviation[] {
  const defaultUnits = toUnits(defaultStream);
  const romUnits = toUnits(romStream);
  if (defaultUnits.length !== romUnits.length) {
    throw new Error("既定と ROM でストリームの単位構造が一致しない");
  }
  const deviations: Deviation[] = [];
  for (let i = 0; i < defaultUnits.length; i++) {
    const d = defaultUnits[i]!;
    const r = romUnits[i]!;
    if (d.kind !== r.kind) throw new Error("既定と ROM で単位種別が一致しない");
    if (d.kind === "GAP") {
      if (sum(d.tokens!) !== sum(r.tokens!)) throw new Error("wait 合計 tick が一致しない");
      if (JSON.stringify(d.tokens) !== JSON.stringify(r.tokens)) {
        deviations.push([d.start, d.length, ...r.tokens!.map((t): SequenceEvent => ["wait", t])]);
      }
    } else {
      if (JSON.stringify(d.event) !== JSON.stringify(r.event)) {
        deviations.push([d.start, 1, r.event!]);
      }
    }
  }
  return deviations;
}

// ── 逸脱スプライスを既定ストリームに適用 ────────────────────────────
function applyDeviations(defaultStream: SequenceEvent[], deviations: Deviation[]): SequenceEvent[] {
  const ordered = [...deviations].sort((a, b) => a[0] - b[0]);
  const out: SequenceEvent[] = [];
  let cursor = 0;
  for (const [start, deleteCount, ...replacement] of ordered) {
    while (cursor < start) out.push(defaultStream[cursor++]!);
    cursor += deleteCount;
    out.push(...replacement);
  }
  while (cursor < defaultStream.length) out.push(defaultStream[cursor++]!);
  return out;
}

// 既定ストリームのハッシュ (ドリフト検出用の指紋)。
function streamHash(events: SequenceEvent[]): string {
  return createHash("sha1").update(JSON.stringify(events)).digest("hex").slice(0, 16);
}

// .mid から各 stream セグメントの既定ストリームを再計算する。
function defaultStreams(midi: Buffer): Map<string, SequenceEvent[]> {
  const canonical = midiToSequence(midi);
  const streams = new Map<string, SequenceEvent[]>();
  for (const segment of canonical.layout) {
    if (segment.kind === "stream") streams.set(segment.label, greedyDefault(segment.events));
  }
  return streams;
}

// ── 検証: .mid とサイドカーの整合を確認 (不整合なら throw) ────────────
export function validate_midi_sidecar(midi: Buffer, sidecar: Sidecar | null): void {
  if (sidecar === null) return;
  if (sidecar.format !== 1 || sidecar.engine !== "smsh-sequence-sidecar") {
    throw new Error(`サイドカー ${sidecar?.name ?? "?"} の形式が不正`);
  }
  const streams = defaultStreams(midi);
  if (Object.keys(sidecar.tracks).length === 0) {
    throw new Error(`サイドカー ${sidecar.name} が空 (空なら省略すべき)`);
  }
  for (const [label, track] of Object.entries(sidecar.tracks)) {
    const stream = streams.get(label);
    if (stream === undefined) throw new Error(`サイドカー ${sidecar.name}: トラック ${label} が .mid に無い`);
    if (stream.length !== track.events) {
      throw new Error(`サイドカー ${sidecar.name}/${label}: 既定イベント数のドリフト (${track.events} != ${stream.length})`);
    }
    if (streamHash(stream) !== track.hash) {
      throw new Error(`サイドカー ${sidecar.name}/${label}: 既定ストリームのハッシュ不一致`);
    }
    if (track.deviations.length === 0) {
      throw new Error(`サイドカー ${sidecar.name}/${label}: 逸脱ゼロのトラックは省略すべき`);
    }
    let previousEnd = 0;
    for (const [start, deleteCount] of [...track.deviations].sort((a, b) => a[0] - b[0])) {
      if (!Number.isInteger(start) || !Number.isInteger(deleteCount) || start < 0 || deleteCount < 1) {
        throw new Error(`サイドカー ${sidecar.name}/${label}: 逸脱スパンが不正`);
      }
      if (start < previousEnd) throw new Error(`サイドカー ${sidecar.name}/${label}: 逸脱スパンが重複`);
      if (start + deleteCount > stream.length) {
        throw new Error(`サイドカー ${sidecar.name}/${label}: 逸脱スパンが既定ストリームの範囲外`);
      }
      previousEnd = start + deleteCount;
    }
  }
}

// ── 復号: .mid + サイドカー → SequenceSource ─────────────────────────
export function decode_midi_sidecar(midi: Buffer, sidecar: Sidecar | null): SequenceSource {
  validate_midi_sidecar(midi, sidecar);
  const canonical = midiToSequence(midi);
  const layout: SequenceSegment[] = canonical.layout.map((segment) => {
    if (segment.kind !== "stream") return segment;
    const stream = greedyDefault(segment.events);
    const track = sidecar?.tracks[segment.label];
    return { kind: "stream", label: segment.label, events: track ? applyDeviations(stream, track.deviations) : stream };
  });
  return { format: canonical.format, engine: canonical.engine, base: canonical.base, externals: canonical.externals, layout };
}

// ── 構築: .mid + サイドカー → エンジンバイト列 ──────────────────────
export function build_from_midi_sidecar(midi: Buffer, sidecar: Sidecar | null): [Buffer, SequenceReport] {
  return build_sequence(decode_midi_sidecar(midi, sidecar));
}

// ── 変換: SequenceSource → { .mid, サイドカー } ─────────────────────
// 逸脱ゼロなら sidecar は null (ファイルを省略)。
export function convert_sequence(source: SequenceSource, name: string): { midi: Buffer; sidecar: Sidecar | null } {
  const midi = sequenceToMidi(source);
  const streams = defaultStreams(midi);
  const romStreams = new Map<string, SequenceEvent[]>();
  for (const segment of source.layout) {
    if (segment.kind === "stream") romStreams.set(segment.label, segment.events);
  }
  const tracks: Record<string, SidecarTrack> = {};
  for (const [label, defaultStream] of streams) {
    const romStream = romStreams.get(label);
    if (romStream === undefined) throw new Error(`${name}: .mid のトラック ${label} が元 JSON に無い`);
    const deviations = diffStream(defaultStream, romStream);
    if (deviations.length > 0) {
      tracks[label] = { events: defaultStream.length, hash: streamHash(defaultStream), deviations };
    }
  }
  const sidecar: Sidecar | null =
    Object.keys(tracks).length === 0 ? null : { format: 1, engine: "smsh-sequence-sidecar", name, tracks };
  // 生成物が元とバイト完全一致することを常に確認する。
  const [expected] = build_sequence(source);
  const [rebuilt] = build_from_midi_sidecar(midi, sidecar);
  if (!expected.equals(rebuilt)) throw new Error(`${name}: .mid+サイドカーが元 JSON とバイト一致しない`);
  return { midi, sidecar };
}

// サイドカー JSON を安定した体裁で直列化する (逸脱 1 件を 1 行に)。
function serializeSidecar(sidecar: Sidecar): string {
  const lines: string[] = ["{"];
  lines.push(`  "format": ${sidecar.format},`);
  lines.push(`  "engine": ${JSON.stringify(sidecar.engine)},`);
  lines.push(`  "name": ${JSON.stringify(sidecar.name)},`);
  lines.push(`  "tracks": {`);
  const labels = Object.keys(sidecar.tracks);
  labels.forEach((label, li) => {
    const track = sidecar.tracks[label]!;
    lines.push(`    ${JSON.stringify(label)}: {`);
    lines.push(`      "events": ${track.events},`);
    lines.push(`      "hash": ${JSON.stringify(track.hash)},`);
    lines.push(`      "deviations": [`);
    track.deviations.forEach((deviation, di) => {
      const tail = di === track.deviations.length - 1 ? "" : ",";
      lines.push(`        ${JSON.stringify(deviation)}${tail}`);
    });
    lines.push(`      ]`);
    lines.push(`    }${li === labels.length - 1 ? "" : ","}`);
  });
  lines.push(`  }`);
  lines.push("}");
  return lines.join("\n") + "\n";
}

// ── CLI: 旧 JSON 群 → .mid + サイドカー + 新 index ───────────────────
const SEQUENCES_DIR = "assets/audio/sequences";
const MIDI_DIR = "assets/audio/midi";
const DATA_DIR = "assets/audio/data";

function convertAll(): void {
  // 一度きりの移行バッチ。冗長 sequence-JSON 群を .mid + サイドカー + 新 index に
  // 変換する。移行済み (旧ディレクトリ撤去済み) なら何もしない。再変換が要るときは
  // ROM から music_sequence.ts で JSON を再抽出してから実行する。
  if (!existsSync(join(SEQUENCES_DIR, "index.json"))) {
    console.log(`already-migrated (${SEQUENCES_DIR} は撤去済み; 変換対象なし)`);
    return;
  }
  const oldIndex = JSON.parse(readFileSync(join(SEQUENCES_DIR, "index.json"), "utf8"));
  mkdirSync(MIDI_DIR, { recursive: true });
  mkdirSync(DATA_DIR, { recursive: true });
  const sequences: Record<string, unknown>[] = [];
  let empty = 0;
  let nonempty = 0;
  const sidecarBytes: number[] = [];
  for (const entry of oldIndex.sequences) {
    const name = String(entry.name);
    const source = JSON.parse(readFileSync(join(SEQUENCES_DIR, String(entry.source)), "utf8")) as SequenceSource;
    const { midi, sidecar } = convert_sequence(source, name);
    writeFileSync(join(MIDI_DIR, `${name}.mid`), midi);
    const record: Record<string, unknown> = {
      name,
      address: entry.address,
      size: entry.size,
      midi: `${name}.mid`,
    };
    if (sidecar !== null) {
      const serialized = serializeSidecar(sidecar);
      writeFileSync(join(DATA_DIR, `${name}.json`), serialized);
      record.sidecar = `../data/${name}.json`;
      sidecarBytes.push(serialized.length);
      nonempty += 1;
    } else {
      empty += 1;
    }
    sequences.push(record);
  }
  const newIndex = { format: 1, engine: "smsh-sequence-series", sequences };
  writeFileSync(join(MIDI_DIR, "index.json"), JSON.stringify(newIndex, null, 2) + "\n");
  sidecarBytes.sort((a, b) => a - b);
  const median = sidecarBytes.length ? sidecarBytes[Math.floor(sidecarBytes.length / 2)] : 0;
  console.log(`converted=${sequences.length} empty=${empty} nonempty=${nonempty} sidecar-bytes(min/median/max)=${sidecarBytes[0] ?? 0}/${median}/${sidecarBytes.at(-1) ?? 0}`);
}

function validateAll(): void {
  const index = JSON.parse(readFileSync(join(MIDI_DIR, "index.json"), "utf8"));
  let checked = 0;
  for (const entry of index.sequences) {
    const name = String(entry.name);
    const midi = readFileSync(join(MIDI_DIR, String(entry.midi)));
    let sidecar: Sidecar | null = null;
    if (entry.sidecar !== undefined) {
      sidecar = JSON.parse(readFileSync(join(MIDI_DIR, String(entry.sidecar)), "utf8")) as Sidecar;
    }
    validate_midi_sidecar(midi, sidecar);
    // base が index の address と一致することも確認。
    const [, report] = build_from_midi_sidecar(midi, sidecar);
    if (report.base !== Number(entry.address)) throw new Error(`${name}: base が index の address と不一致`);
    checked += 1;
  }
  console.log(`validated=${checked}`);
}

// ── self-test ──────────────────────────────────────────────────────
export function self_test_midi_sequence(): void {
  // (1) 逸脱ゼロの曲: サイドカーは null で往復バイト完全。
  const clean: SequenceSource = {
    format: 1, engine: "smsh-sequence", base: "0x08000100", externals: { tone_bank: "0x08001000" },
    layout: [
      { kind: "stream", label: "track_1", events: [
        ["volume", 100], ["note", 23, 60, 127], ["wait", 23],
        ["note_running", 23, 62], ["wait", 96], ["wait", 4], ["fine"],
      ] },
      { kind: "align", boundary: 4, fill: 0 },
      { kind: "header", label: "sound_x", block_count: 0, priority: 0, reverb: 0, tone_bank: "tone_bank", tracks: ["track_1"] },
    ],
  };
  const cleanPair = convert_sequence(clean, "sound_clean");
  if (cleanPair.sidecar !== null) throw new Error("既定一致の曲はサイドカーを持たないはず");
  const [cleanExpected] = build_sequence(clean);
  const [cleanRebuilt] = build_from_midi_sidecar(cleanPair.midi, cleanPair.sidecar);
  if (!cleanExpected.equals(cleanRebuilt)) throw new Error("逸脱ゼロ曲の往復がバイト完全でない");

  // (2) 逸脱あり: 冗長 velocity・fresh 強制・非貪欲 wait を含む曲。
  const deviant: SequenceSource = {
    format: 1, engine: "smsh-sequence", base: "0x08000200", externals: { tone_bank: "0x08002000" },
    layout: [
      { kind: "stream", label: "track_1", events: [
        ["note", 23, 60, 100],
        ["wait", 24], ["wait", 24], ["wait", 24], ["wait", 23], // 非貪欲 (貪欲なら 92+3)
        ["note", 23, 62, 100], // fresh + 冗長 velocity (既定は note_running,np=1)
        ["wait", 23],
        ["fine"],
      ] },
      { kind: "align", boundary: 4, fill: 0 },
      { kind: "header", label: "sound_y", block_count: 0, priority: 0, reverb: 0, tone_bank: "tone_bank", tracks: ["track_1"] },
    ],
  };
  const deviantPair = convert_sequence(deviant, "sound_deviant");
  if (deviantPair.sidecar === null) throw new Error("逸脱ありの曲はサイドカーを持つはず");
  const [deviantExpected] = build_sequence(deviant);
  const [deviantRebuilt] = build_from_midi_sidecar(deviantPair.midi, deviantPair.sidecar);
  if (!deviantExpected.equals(deviantRebuilt)) throw new Error("逸脱あり曲の往復がバイト完全でない");

  // (3) 検証: ドリフトを大声で弾く。
  validate_midi_sidecar(deviantPair.midi, deviantPair.sidecar); // 正常
  const drifted = JSON.parse(JSON.stringify(deviantPair.sidecar)) as Sidecar;
  drifted.tracks.track_1!.events += 1;
  let threw = false;
  try { validate_midi_sidecar(deviantPair.midi, drifted); } catch { threw = true; }
  if (!threw) throw new Error("イベント数ドリフトを検出できなかった");
  const hashDrift = JSON.parse(JSON.stringify(deviantPair.sidecar)) as Sidecar;
  hashDrift.tracks.track_1!.hash = "0000000000000000";
  threw = false;
  try { validate_midi_sidecar(deviantPair.midi, hashDrift); } catch { threw = true; }
  if (!threw) throw new Error("ハッシュドリフトを検出できなかった");

  console.log("self-test=ok");
}

async function main(args: string[]): Promise<void> {
  if (args.includes("--self-test")) {
    self_test_midi_sequence();
    return;
  }
  if (args[0] === "convert-all") {
    convertAll();
    return;
  }
  if (args[0] === "validate-all") {
    validateAll();
    return;
  }
  console.log("usage: midi_sequence.ts convert-all | validate-all | --self-test");
}

if (import.meta.main) await main(Bun.argv.slice(2));
