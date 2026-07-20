// midi_roundtrip.ts — 試作 (prototype)
//
// 目的: エンジン固有の sequence-JSON (tools/music_sequence.ts のコーデック) を
//       標準 MIDI ファイル (SMF) 経由で往復させ、MIDI を「バイト完全な正典
//       (byte-exact canonical source)」として使えるかを実証的に検証する。
//
// ── 対応表 (sequence-JSON → SMF) ────────────────────────────────────────
//   ・トラック構成: SMF format 1。
//       - トラック0 = コンダクタ。曲全体の骨組み (format / engine / base /
//         externals / layout の並び順と align・header メタデータ) を
//         メタイベント 0x01 (text) に JSON で格納する。
//       - stream セグメントごとに MTrk を 1 本割り当てる (MTrk #1..#N)。
//   ・音符 (note / note_running): MIDI の note-on / note-off で表現する。
//       - note-on の tick   = 現在の時間カーソル (wait の累積)。
//       - note-off の tick  = note-on + ゲート長 (= 命令のデュレーション値)。
//       - key / velocity は「実効値」を書き出す。JSON 側で省略されていても
//         直前の走行値を材質化 (materialize) して note-on に載せる。
//   ・待機 (wait): MIDI のデルタタイム (時間カーソルの前進) で表現する。
//       MIDI 上には独立したイベントを残さない。1 tick = 1 エンジン tick。
//   ・それ以外のエンジン命令 (control 各種 / fine / label / pattern /
//     pattern_end / goto / repeat / note_end / *_running control) は、
//     現在の tick 位置にマーカ用メタ 0x06 を置き、ペイロードに
//     イベント配列そのものを JSON で逐語格納する (可逆)。
//
// ── 逆写像 (SMF → sequence-JSON) の正準化方針 ──────────────────────────
//   MIDI の note-on/off とデルタタイムには、下記の「格納選択 (storage
//   choice)」を表す情報が存在しない。よって逆写像は各々を一意な正準形に
//   畳み込む。これが往復で失われる (= MIDI が表現できない) 情報である。
//     1. 音符は常に fresh な "note" として再構成する ("note_running" にしない)。
//     2. key / velocity は常に明示する (省略形に戻さない)。
//     3. wait のトークン分割は「大きい順の貪欲法」で正準化する
//        (元の 24+24+24+23 のような分割は復元しない)。
//   control / 構造命令はマーカに逐語格納しているため無損失で戻る。
//   すなわち本実験は「音符と待機を MIDI の音符とデルタタイムで表す」層の
//   可逆性だけを問う (task の指定どおり)。
//
// 使い方:  bun tools/midi_roundtrip.ts            # 2曲を往復し結果を out/ に出力
//          bun tools/midi_roundtrip.ts --self-test

import {
  midi_events,
  make_midi_chunk,
  type MidiEvent,
} from "./import_asset.ts";
import {
  build_sequence,
  type SequenceSource,
  type SequenceSegment,
  type SequenceEvent,
  type ControlName,
} from "./music_sequence.ts";
import { decode_midi_sidecar, type Sidecar } from "./midi_sequence.ts";

// エンジンのデュレーション表 (music_sequence.ts と同一。wait 分割に使う)。
const DURATIONS = [
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
  20, 21, 22, 23, 24, 28, 30, 32, 36, 40, 42, 44, 48, 52, 54, 56, 60, 64,
  66, 68, 72, 76, 78, 80, 84, 88, 90, 92, 96,
];
const DURATIONS_DESC = [...DURATIONS].filter((v) => v > 0).sort((a, b) => b - a);

// note 系か wait かを判定する補助。
const isNote = (e: SequenceEvent): boolean =>
  e[0] === "note" || e[0] === "note_running";
const isWait = (e: SequenceEvent): boolean => e[0] === "wait";

// ── MIDI 低レベル出力 ───────────────────────────────────────────────
// 可変長数量 (delta-time / meta size)。
function vlqEncode(value: number): number[] {
  if (value < 0) throw new Error("負のデルタタイムは表現できない");
  const bytes = [value & 0x7f];
  value >>= 7;
  while (value > 0) {
    bytes.unshift((value & 0x7f) | 0x80);
    value >>= 7;
  }
  return bytes;
}

interface RawMidiEvent {
  tick: number;
  seq: number; // 同一 tick 内の順序保持用
  bytes: number[]; // ステータス以降の実バイト (デルタは後で付与)
}

function metaText(metaType: number, text: string): number[] {
  const payload = [...Buffer.from(text, "utf8")];
  return [0xff, metaType, ...vlqEncode(payload.length), ...payload];
}

function buildMTrk(events: RawMidiEvent[]): Buffer {
  // tick 昇順 → 同一 tick は seq 昇順で安定ソート。
  const ordered = [...events].sort((a, b) => a.tick - b.tick || a.seq - b.seq);
  const out: number[] = [];
  let prev = 0;
  for (const e of ordered) {
    out.push(...vlqEncode(e.tick - prev), ...e.bytes);
    prev = e.tick;
  }
  out.push(...vlqEncode(0), 0xff, 0x2f, 0x00); // End of Track
  return make_midi_chunk(Buffer.from("MTrk"), Buffer.from(out));
}

// ── 走行状態を追って note の実効 key/velocity を材質化する ──────────
// music_sequence.ts のコーデックでは note のパラメータは [key, velocity]。
// 省略時は直前の走行値を引き継ぐ。往復のため、その実効値を復元する。
function effectiveNoteParams(
  event: SequenceEvent,
  state: { key: number; vel: number },
): { dur: number; key: number; vel: number } {
  // event = ["note", dur, key?, vel?] または ["note_running", dur, key?, vel?]
  const dur = event[1] as number;
  const params = (event.slice(2) as number[]);
  if (params.length >= 1) state.key = params[0];
  if (params.length >= 2) state.vel = params[1];
  return { dur, key: state.key, vel: state.vel };
}

// ── sequence-JSON → SMF バイト列 ───────────────────────────────────
export function sequenceToMidi(source: SequenceSource): Buffer {
  const PPQN = 96; // 1 エンジン tick = 1 MIDI tick。
  const streamSegments = source.layout.filter(
    (s): s is Extract<SequenceSegment, { kind: "stream" }> => s.kind === "stream",
  );

  // コンダクタトラック: 骨組みを逐語 JSON で格納。
  const skeleton = {
    format: source.format,
    engine: source.engine,
    base: source.base,
    externals: source.externals,
    layout: source.layout.map((seg) => {
      if (seg.kind === "stream") return { kind: "stream", label: seg.label };
      return seg; // align / header はメタデータをそのまま
    }),
  };
  const conductor = buildMTrk([
    { tick: 0, seq: 0, bytes: metaText(0x01, JSON.stringify(skeleton)) },
  ]);

  const trackChunks: Buffer[] = [conductor];

  streamSegments.forEach((segment, index) => {
    const channel = index % 16;
    const raw: RawMidiEvent[] = [];
    let cursor = 0; // 時間カーソル (tick)
    let seq = 0;
    const noteState = { key: 0, vel: 0 };
    // ゲート満了イベントは大きな seq を与えて同一 tick の grid イベントの後に置く。
    let offSeqBase = 1_000_000;

    for (const event of segment.events) {
      const kind = event[0];
      if (isNote(event)) {
        const { dur, key, vel } = effectiveNoteParams(event, noteState);
        raw.push({
          tick: cursor,
          seq: seq++,
          bytes: [0x90 | channel, key & 0x7f, vel & 0x7f],
        });
        raw.push({
          tick: cursor + dur,
          seq: offSeqBase++,
          bytes: [0x80 | channel, key & 0x7f, 0x40],
        });
      } else if (isWait(event)) {
        cursor += event[1] as number;
      } else {
        // control / fine / label / pattern / pattern_end / goto / repeat /
        // note_end / *_running — 逐語マーカとして現 tick に置く。
        raw.push({
          tick: cursor,
          seq: seq++,
          bytes: metaText(0x06, JSON.stringify(event)),
        });
      }
    }
    trackChunks.push(buildMTrk(raw));
  });

  const header = Buffer.alloc(6);
  header.writeUInt16BE(1, 0); // format 1
  header.writeUInt16BE(trackChunks.length, 2);
  header.writeUInt16BE(PPQN, 4);
  return Buffer.concat([
    make_midi_chunk(Buffer.from("MThd"), header),
    ...trackChunks,
  ]);
}

// ── wait のギャップを正準トークン列に分解 (大きい順・貪欲) ──────────
function tokenizeWait(gap: number): SequenceEvent[] {
  const out: SequenceEvent[] = [];
  let remaining = gap;
  while (remaining > 0) {
    const chunk = DURATIONS_DESC.find((v) => v <= remaining);
    if (chunk === undefined) throw new Error("wait を分解できない");
    out.push(["wait", chunk]);
    remaining -= chunk;
  }
  return out;
}

// ── SMF バイト列 → sequence-JSON ───────────────────────────────────
export function midiToSequence(midi: Buffer): SequenceSource {
  const report = midi_events(midi);
  // トラックごとにイベントを仕分け。
  const byTrack = new Map<number, MidiEvent[]>();
  for (const e of report.events) {
    if (!byTrack.has(e.track)) byTrack.set(e.track, []);
    byTrack.get(e.track)!.push(e);
  }
  for (const list of byTrack.values()) {
    list.sort((a, b) => a.tick - b.tick || a.order - b.order);
  }

  // コンダクタ (track 0) から骨組みを復元。
  const conductorEvents = byTrack.get(0) ?? [];
  const skelEvent = conductorEvents.find(
    (e) => e.type === "meta" && e.meta === 0x01,
  );
  if (!skelEvent || typeof skelEvent.data !== "string") {
    throw new Error("コンダクタ骨組みが見つからない");
  }
  const skeleton = JSON.parse(Buffer.from(skelEvent.data, "hex").toString("utf8"));

  // stream セグメントを順に MTrk (1-based) と対応付ける。
  let streamIndex = 0;
  const layout: SequenceSegment[] = skeleton.layout.map((seg: any) => {
    if (seg.kind !== "stream") return seg as SequenceSegment;
    streamIndex += 1;
    const events = reconstructStream(byTrack.get(streamIndex) ?? []);
    return { kind: "stream", label: seg.label, events };
  });

  return {
    format: skeleton.format,
    engine: skeleton.engine,
    base: skeleton.base,
    externals: skeleton.externals,
    layout,
  };
}

// 1 本の MTrk のイベント列から stream の event 配列を再構成する。
function reconstructStream(events: MidiEvent[]): SequenceEvent[] {
  // grid イベント (note-on / マーカ) と note-off を分離。
  interface Grid {
    tick: number;
    order: number;
    event: SequenceEvent;
  }
  const grid: Grid[] = [];
  // ピッチごとの note-on 待ち行列 (FIFO) で note-off とゲートを対応付ける。
  const pending = new Map<number, { tick: number; ref: Grid }[]>();

  for (const e of events) {
    if (e.type === "meta" && e.meta === 0x2f) continue; // EOT
    if (e.type === "meta" && e.meta === 0x06) {
      const parsed = JSON.parse(
        Buffer.from(e.data as string, "hex").toString("utf8"),
      ) as SequenceEvent;
      grid.push({ tick: e.tick, order: e.order, event: parsed });
      continue;
    }
    if (e.type === "channel" && e.status !== undefined) {
      const kind = e.status & 0xf0;
      const [d0, d1] = e.data as number[];
      if (kind === 0x90 && d1 > 0) {
        // note-on。ゲートは後で note-off から確定 (仮に 0)。
        const node: Grid = { tick: e.tick, order: e.order, event: ["note", 0, d0, d1] };
        grid.push(node);
        if (!pending.has(d0)) pending.set(d0, []);
        pending.get(d0)!.push({ tick: e.tick, ref: node });
      } else if (kind === 0x80 || (kind === 0x90 && d1 === 0)) {
        // note-off。対応する最古の note-on にゲートを書き込む。
        const queue = pending.get(d0);
        if (!queue || queue.length === 0) throw new Error("対応しない note-off");
        const on = queue.shift()!;
        const dur = e.tick - on.tick;
        (on.ref.event as [string, number, number, number])[1] = dur;
      }
    }
  }
  if ([...pending.values()].some((q) => q.length > 0)) {
    throw new Error("閉じていない note-on が残っている");
  }

  // grid を tick・order でソートし、隣接 tick 間のギャップを wait に分解して挿入。
  grid.sort((a, b) => a.tick - b.tick || a.order - b.order);
  const out: SequenceEvent[] = [];
  let cursor = 0;
  for (const g of grid) {
    if (g.tick > cursor) {
      out.push(...tokenizeWait(g.tick - cursor));
      cursor = g.tick;
    }
    out.push(g.event);
  }
  return out;
}

// ── 構造的等価判定 (差分を列挙) ────────────────────────────────────
function diffValue(a: unknown, b: unknown, path: string, diffs: string[]): void {
  if (Array.isArray(a) && Array.isArray(b)) {
    if (a.length !== b.length) {
      diffs.push(`${path}: 配列長 ${a.length} != ${b.length}`);
      const n = Math.min(a.length, b.length);
      for (let i = 0; i < n; i++) diffValue(a[i], b[i], `${path}[${i}]`, diffs);
      return;
    }
    for (let i = 0; i < a.length; i++) diffValue(a[i], b[i], `${path}[${i}]`, diffs);
    return;
  }
  if (a && b && typeof a === "object" && typeof b === "object") {
    const keys = new Set([...Object.keys(a), ...Object.keys(b)]);
    for (const k of keys) {
      diffValue((a as any)[k], (b as any)[k], `${path}.${k}`, diffs);
    }
    return;
  }
  if (a !== b) diffs.push(`${path}: ${JSON.stringify(a)} != ${JSON.stringify(b)}`);
}

// 差分を意味カテゴリに分類する (レポート用)。
function classifyDiff(line: string): string {
  if (/\[0\]: "note" != "note_running"|"note_running" != "note"/.test(line)) {
    return "running-status (note vs note_running)";
  }
  if (/events\]: 配列長/.test(line)) return "param 省略 / wait 分割 (要素数)";
  return "その他";
}

interface SongResult {
  name: string;
  fieldClean: boolean;
  byteClean: boolean;
  originalBytes: number;
  rebuiltBytes: number;
  firstByteDiff: number;
  diffs: string[];
}

async function roundTripSong(name: string, source: SequenceSource, outDir: string): Promise<SongResult> {
  const midi = sequenceToMidi(source);
  await Bun.write(`${outDir}/${name}.mid`, midi);

  const rebuilt = midiToSequence(midi);
  await Bun.write(`${outDir}/${name}.roundtrip.json`, JSON.stringify(rebuilt, null, 2) + "\n");

  const diffs: string[] = [];
  diffValue(source, rebuilt, "$", diffs);
  const fieldClean = diffs.length === 0;

  // コーデックで両者を ROM バイト列に符号化して比較。
  const [origRom] = build_sequence(source);
  let rebuiltRom: Buffer;
  let byteClean = false;
  let firstByteDiff = -1;
  try {
    [rebuiltRom] = build_sequence(rebuilt);
    byteClean = origRom.equals(rebuiltRom);
    if (!byteClean) {
      const n = Math.min(origRom.length, rebuiltRom.length);
      for (let i = 0; i < n; i++) if (origRom[i] !== rebuiltRom[i]) { firstByteDiff = i; break; }
      if (firstByteDiff < 0) firstByteDiff = n;
    }
    await Bun.write(`${outDir}/${name}.rebuilt.rom`, rebuiltRom);
  } catch (err) {
    diffs.push(`build_sequence(rebuilt) 失敗: ${(err as Error).message}`);
    rebuiltRom = Buffer.alloc(0);
  }
  await Bun.write(`${outDir}/${name}.original.rom`, origRom);

  return {
    name,
    fieldClean,
    byteClean,
    originalBytes: origRom.length,
    rebuiltBytes: rebuiltRom.length,
    firstByteDiff,
    diffs,
  };
}

// ── 非単射性の直接証明 ─────────────────────────────────────────────
// 音楽的に同一だがバイト表現の異なる 2 つの sequence-JSON が、同一の
// SMF に写ることを示す (= MIDI は格納選択を保持できない)。
function nonInjectivityProof(): { pair: string; sameMidi: boolean }[] {
  const mk = (events: SequenceEvent[]): SequenceSource => ({
    format: 1,
    engine: "smsh-sequence",
    base: "0x08000100",
    externals: { tone_bank: "0x08001000" },
    layout: [
      { kind: "stream", label: "track_1", events },
      { kind: "align", boundary: 4, fill: 0 },
      {
        kind: "header", label: "sound_x", block_count: 0, priority: 0,
        reverb: 0, tone_bank: "tone_bank", tracks: ["track_1"],
      },
    ],
  });
  const results: { pair: string; sameMidi: boolean }[] = [];

  // (1) note vs note_running: バイトは異なるが音は同一。
  const a1 = mk([["note", 23, 60, 100], ["wait", 24], ["note", 23, 62], ["wait", 24], ["fine"]]);
  const b1 = mk([["note", 23, 60, 100], ["wait", 24], ["note_running", 23, 62], ["wait", 24], ["fine"]]);
  results.push({
    pair: "note vs note_running",
    sameMidi: sequenceToMidi(a1).equals(sequenceToMidi(b1)),
  });

  // (2) velocity 明示 vs 省略: バイトは異なるが音は同一。
  const a2 = mk([["note", 23, 60, 100], ["wait", 24], ["note", 23, 62, 100], ["wait", 24], ["fine"]]);
  const b2 = mk([["note", 23, 60, 100], ["wait", 24], ["note", 23, 62], ["wait", 24], ["fine"]]);
  results.push({
    pair: "velocity 明示 vs 省略",
    sameMidi: sequenceToMidi(a2).equals(sequenceToMidi(b2)),
  });

  // (3) wait 分割の違い: 95 tick を 92+3 と 24*3+23 で。
  const a3 = mk([["note", 92, 60, 100], ["wait", 92], ["wait", 3], ["fine"]]);
  const b3 = mk([["note", 92, 60, 100], ["wait", 24], ["wait", 24], ["wait", 24], ["wait", 23], ["fine"]]);
  results.push({
    pair: "wait 分割 (92+3 vs 24+24+24+23)",
    sameMidi: sequenceToMidi(a3).equals(sequenceToMidi(b3)),
  });

  return results;
}

export function self_test(): void {
  // 格納選択を使わない最小曲は往復でバイト完全になるはず。
  const clean: SequenceSource = {
    format: 1,
    engine: "smsh-sequence",
    base: "0x08000100",
    externals: { tone_bank: "0x08001000" },
    layout: [
      { kind: "stream", label: "track_1", events: [["volume", 100], ["note", 23, 60, 127], ["wait", 23], ["fine"]] },
      { kind: "align", boundary: 4, fill: 0 },
      { kind: "header", label: "sound_x", block_count: 0, priority: 0, reverb: 0, tone_bank: "tone_bank", tracks: ["track_1"] },
    ],
  };
  const rebuilt = midiToSequence(sequenceToMidi(clean));
  const [a] = build_sequence(clean);
  const [b] = build_sequence(rebuilt);
  if (!a.equals(b)) throw new Error("最小曲の往復がバイト完全でない");
  const proofs = nonInjectivityProof();
  if (!proofs.every((p) => p.sameMidi)) throw new Error("非単射性の証明が不成立");
  console.log("self-test=ok");
}

async function main(): Promise<void> {
  if (Bun.argv.includes("--self-test")) {
    self_test();
    return;
  }
  const outDir = "out/audio_midi";
  const { mkdirSync } = await import("node:fs");
  mkdirSync(outDir, { recursive: true });

  // デモ曲は追跡中の .mid + サイドカーから正典ソースを復元して往復させる
  // (冗長な旧 sequence-JSON は撤去済み)。
  const songs = ["sound_075", "sound_106"] as const;

  const results: SongResult[] = [];
  for (const name of songs) {
    const midiSource = await Bun.file(`assets/audio/midi/${name}.mid`).arrayBuffer();
    const sidecarPath = `assets/audio/data/${name}.json`;
    const sidecar = (await Bun.file(sidecarPath).exists())
      ? (JSON.parse(await Bun.file(sidecarPath).text()) as Sidecar)
      : null;
    const source = decode_midi_sidecar(Buffer.from(midiSource), sidecar);
    results.push(await roundTripSong(name, source, outDir));
  }

  const proofs = nonInjectivityProof();

  const lines: string[] = [];
  lines.push("# MIDI ↔ sequence-JSON 往復レポート\n");
  for (const r of results) {
    lines.push(`## ${r.name}`);
    lines.push(`field-equal: ${r.fieldClean ? "CLEAN" : "FAIL"}`);
    lines.push(`byte-equal : ${r.byteClean ? "CLEAN" : "FAIL"} (orig ${r.originalBytes}B / rebuilt ${r.rebuiltBytes}B, 最初の相違 offset ${r.firstByteDiff})`);
    if (r.diffs.length) {
      const cats = new Map<string, number>();
      for (const d of r.diffs) {
        const c = classifyDiff(d);
        cats.set(c, (cats.get(c) ?? 0) + 1);
      }
      lines.push(`差分カテゴリ:`);
      for (const [c, n] of cats) lines.push(`  - ${c}: ${n}`);
      lines.push(`差分 (先頭20件):`);
      for (const d of r.diffs.slice(0, 20)) lines.push(`  ${d}`);
    }
    lines.push("");
  }
  lines.push("## 非単射性の証明 (音は同一・バイトは相違 → 同一 MIDI か)");
  for (const p of proofs) lines.push(`  - ${p.pair}: 同一MIDI=${p.sameMidi}`);
  lines.push("");

  const report = lines.join("\n");
  await Bun.write(`${outDir}/report.txt`, report + "\n");
  console.log(report);
}

if (import.meta.main) await main();
