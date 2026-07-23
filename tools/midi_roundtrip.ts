// Tool role: both; imported by tools/midi_sequence.ts; invoked by CONVENTIONS.md, assets/audio/README.md, tools/build_soundfont.ts.
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
// ── 再生用チャンネルコントローラ (GBA エンジンの発音を再現する追加出力) ──
//   往復の可逆性は上記 0x06 マーカが担保する。それとは別に、実プレイヤ
//   (fluidsynth 等) が GBA エンジンらしく鳴らせるよう、下記の標準 MIDI
//   コントロールチェンジ (0xB0) を「再生専用」に重ねて出力する。これらは
//   midiToSequence の逆写像では読み飛ばす (set-tempo 0x51・キュー 0x07 と同様)
//   ため、ROM バイト列の復元には一切影響しない。
//     ・CC1  (モジュレーションホイール) ← modulation_depth コマンド。
//         エンジンのトラック LFO 深さ。SF2 側の既定モジュレータ
//         (CC1 → vibLfoToPitch, +50 セント) がこれを受けてビブラートを掛ける。
//         modulation_type は全 260 曲で 0 (ピッチ変調) のため常にビブラートに写す。
//         MOD 値 (0..127) をそのまま CC1 値にする (mp2k MOD と MIDI CC1 は同域)。
//     ・CC91 (エフェクト1深度 = リバーブ送り) ← ソングヘッダの reverb バイト。
//         bit7=有効・下位7bit=量。有効なら量を、無効/0 なら 0 を tick 0 で送る。
//         曲ごとに異なる (0/有効0/50) ため全曲共有の SF2 では表せず、曲固有値として
//         ここで CC91 に写す。0 を明示送出して既定リバーブの誤混入も防ぐ。
//     ・プログラムチェンジ (0xC0) ← voice コマンド。エンジンの音色選択。SF2 は
//         「バンク × プログラム」で音色を並べる (bank 0 = トーンバンク bank_0 の
//         レコード N、bank 1 = bank_1 のレコード N)。voice の値 N (全曲 0..127) を
//         そのままプログラム番号にする。これを出さないと全チャンネルが既定の
//         bank0/program0 で鳴り「楽器が総取り違え」になる (本修正の主眼)。
//     ・バンクセレクト MSB (CC0) ← ソングの externals.tone_bank。
//         0x080fba78 → bank 0、0x080fc138 → bank 1。曲頭で一度送る (曲内で不変)。
//         これで各曲が正しいトーンバンクのプリセット群を引く。
//     ・ピッチホイール (0xE0) ← pitch_bend コマンド (走行形含む)。エンジンの実ベンド量
//         (引数 − 64) を正規化した 14bit ホイールに写す。曲頭で RPN0 = 既定幅 2 半音を
//         設定し、pitch_bend_range が来ればその値で RPN0 を上書きする。asm 実測
//         (Func_080fac44/080f9bcc/080f9be0) から幅と写像を導いた (本ファイル下部の定数)。
//     ・チャンネルボリューム (CC7) ← volume コマンド (エンジン音量 #18、0..127 直値)。
//     ・パン (CC10) ← pan コマンド (エンジン定位 #20、中立 64)。
//     ・微調律 RPN1 (CC101/100/6/38) ← tuning コマンド (引数 − 64、±1 半音域)。
//         Func_080f9c18 が #12 に格納しピッチ端数へ寄与する微調律を写す。
//   写せない命令 (標準 MIDI・現行 SF2 で確実に鳴る対応が無い): priority (発音優先度)・
//   key_shift (全 260 曲で 0)・lfo_speed/lfo_delay (SF2 は CC1→ビブラート幅のモジュレータ
//   のみ配線、CC76/78 は未配線)・modulation_type (全曲 0 のエンジン内部ルーティング旗)。
//   これらはマーカ 0x06 に逐語保持され続け、ROM 復元には元から使われる。
//   チャンネル割当: GM のドラムチャンネル (ch9) は打楽器バンクに固定されるため
//   旋律トラックが載らないよう回避する (ch0..8, 10..15 を使う)。
//   これらの CC/プログラム追加で .mid のバイト列は変わるが、逆写像・サイドカー・
//   ROM 復元は不変 (これらは読み飛ばされ、既定ストリームのハッシュも変わらない)。
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
import { canonicalJson } from "./canonical_json.ts";

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

// 生バイトのメタイベント (テンポ 0x51 など、非テキストのペイロード用)。
function metaBytes(metaType: number, payload: number[]): number[] {
  return [0xff, metaType, ...vlqEncode(payload.length), ...payload];
}

// ── 再生用メタの型 (再構築では無視される) ──────────────────────────
//   ・0x07 (cue point): 構造キュー。ペイロードのテキストで種別を区別する。
//       "loopStart" / "loopEnd"  : ループ区間の目印 (RPG 系の慣習)。
//       "pattern+"  / "pattern-" : パターン展開ブラケットの開閉。
//         ブラケット内の note-on と 0x06 マーカは「展開複製」なので、
//         midiToSequence は再構築時にこれらを捨て、ブラケットの tick 長を
//         コンパクト時間軸から差し引く (= 逆写像が元の compact stream を復元)。
//   ・0x51 (set tempo): 実テンポ。プレイヤが 120BPM 既定にならないよう出す。
//       エンジンの tempo 制御値 v から BPM = 2v (このエンジン系の慣習)。
const CUE_META = 0x07;
const TEMPO_META = 0x51;

// ── 再生用チャンネルコントローラ番号 (逆写像では読み飛ばす) ──────────
const CC_BANK_MSB = 0; // バンクセレクト MSB ← externals.tone_bank
const CC_MODWHEEL = 1; // モジュレーションホイール ← modulation_depth (ビブラート深さ)
const CC_VOLUME = 7; // チャンネルボリューム ← volume コマンド (エンジンの音量 #18)
const CC_PAN = 10; // パン ← pan コマンド (エンジンの定位 #20)
const CC_REVERB = 91; // エフェクト1深度 = リバーブ送り ← ヘッダ reverb バイト
// RPN (登録済みパラメータ番号) 用データエントリ。ピッチベンド幅・微調律に使う。
const CC_DATA_MSB = 6; // データエントリ MSB
const CC_DATA_LSB = 38; // データエントリ LSB
const CC_RPN_LSB = 100; // RPN 選択 LSB
const CC_RPN_MSB = 101; // RPN 選択 MSB
const RPN_BEND_RANGE = 0; // RPN 0x0000 = ピッチベンド幅 (半音)
const RPN_FINE_TUNE = 1; // RPN 0x0001 = チャンネル微調律 (±100 セント = ±1 半音)
const DRUM_CHANNEL = 9; // GM 打楽器チャンネル (旋律トラックを載せない)

// ── エンジンのピッチ合成から導いた定数 (asm 実測) ─────────────────────
//   Func_080fac44 のピッチ合成: r1 = (tuning#12 + bend#14×bendRange#15)<<2
//     + keyShift#10<<8 + … 、[#8]=r1>>8 が整数半音・[#9]=端数 (1/256 半音単位)。
//   ・pitch_bend ハンドラ Func_080f9bcc は「引数バイト − 64」を #14 に格納するので
//     エンジンの実ベンド量 = (引数 − 64) ∈ [-64,63] (中立 = 64)。
//   ・ピッチ寄与 = bend×range<<2 / 256 = bend×range/64 半音。よって最大偏移
//     (bend=±64) = ±range 半音 ⇒ MIDI ピッチホイール全振り = range 半音に一致し、
//     RPN0 に range をそのまま設定すれば良い (ホイール値は range に依らず正規化)。
//   ・pitch_bend_range ハンドラ Func_080f9be0 は #15 に直値を格納 (半音数)。
//     トラック初期化 (Func_080f9c90) が #15 = 2 を既定にするので既定幅は 2 半音。
//   ・tuning ハンドラ Func_080f9c18 は「引数 − 64」を #12 に格納し、寄与 = (引数−64)/64
//     半音の微調律 (中立 = 64)。RPN1 (±1 半音 = 14bit) に写す。
//   ・volume ハンドラ Func_080f9ba4 は #18 に直値 (0..127)、pan ハンドラ Func_080f9bb8 は
//     「引数 − 64」を #20 に格納 (中立 = 64、生の引数 = MIDI パン域)。
const BEND_CENTER = 64; // 引数バイトの中立点 (asm の subs #64)
const DEFAULT_BEND_RANGE = 2; // #15 の既定値 (Func_080f9c90 のトラック初期化)
const MIDI_BEND_CENTER = 0x2000; // 14bit ピッチホイールの中立 (8192)
// エンジンの中立差 1 単位 = 全振り (8192) の 1/64 ⇒ 128 を掛ける。
const BEND_UNIT_TO_WHEEL = MIDI_BEND_CENTER / BEND_CENTER; // = 128

// 制御命令名の集合 (explicit 形の kind 判定に使う)。
const CONTROL_NAMES = new Set<ControlName>([
  "priority", "tempo", "key_shift", "voice", "volume", "pan",
  "pitch_bend", "pitch_bend_range", "lfo_speed", "lfo_delay",
  "modulation_depth", "modulation_type", "tuning",
]);

// トーンバンクアドレス → SF2 バンク番号 (build_soundfont.ts と一致)。
const TONE_BANK_TO_SF2: Record<string, number> = {
  "0x080fba78": 0, // bank_0 (144 レコード)
  "0x080fc138": 1, // bank_1 (81 レコード)
};

// コントロールチェンジのバイト列 (ステータス 0xB0 | channel)。
function controlChange(channel: number, controller: number, value: number): number[] {
  return [0xb0 | channel, controller & 0x7f, value & 0x7f];
}

// プログラムチェンジのバイト列 (ステータス 0xC0 | channel)。
function programChange(channel: number, program: number): number[] {
  return [0xc0 | channel, program & 0x7f];
}

// ピッチホイールのバイト列 (ステータス 0xE0 | channel、14bit を LSB→MSB で載せる)。
function pitchWheel(channel: number, value14: number): number[] {
  const v = Math.max(0, Math.min(0x3fff, value14));
  return [0xe0 | channel, v & 0x7f, (v >> 7) & 0x7f];
}

// エンジンの中立差 (引数 − 64) → 14bit ピッチホイール値 (中立 8192)。
function bendToWheel(controlValue: number): number {
  return MIDI_BEND_CENTER + (controlValue - BEND_CENTER) * BEND_UNIT_TO_WHEEL;
}

// RPN を選択してデータエントリを書き込むコントロールチェンジ列を返す
// (選択 MSB/LSB → データ MSB[→LSB])。tick が同じでも順序保持のため配列で返す。
function rpnWrite(
  channel: number, rpnMsb: number, rpnLsb: number, dataMsb: number, dataLsb?: number,
): number[][] {
  const seq = [
    controlChange(channel, CC_RPN_MSB, rpnMsb),
    controlChange(channel, CC_RPN_LSB, rpnLsb),
    controlChange(channel, CC_DATA_MSB, dataMsb),
  ];
  if (dataLsb !== undefined) seq.push(controlChange(channel, CC_DATA_LSB, dataLsb));
  return seq;
}

// externals.tone_bank → SF2 バンク番号 (未知は 0)。
function bankSelectFor(toneBank: string | undefined): number {
  return TONE_BANK_TO_SF2[String(toneBank ?? "").toLowerCase()] ?? 0;
}

// ストリーム番号 → MIDI チャンネル (ドラムチャンネル 9 を飛ばす)。
function streamChannel(index: number): number {
  return (index < DRUM_CHANNEL ? index : index + 1) % 16;
}

// ヘッダの reverb バイト → CC91 送り量。bit7 有効なら下位7bit、無効/0 なら 0。
function reverbSend(reverbByte: number): number {
  return (reverbByte & 0x80) !== 0 ? reverbByte & 0x7f : 0;
}

// 1 エンジン tick = 1 MIDI tick。四分音符あたり 96 tick (wait 表の最大値 = 全音符)。
const PPQN = 96;

// ── エンジンの実 tick 速度 (ROM 実測から導出) ──────────────────────
//   ・毎フレーム (VBlank) 累算器 += tempoI、150 を超えるたびに 1 tick 進む
//     (Func_080f9c90: accumulator[+34] += tempoI[+32]; while >=150 subtract 150)。
//   ・tempo コマンドは制御値を 2 倍して tempoI にする
//     (Func_080f9b4c: lsls #1 → ×scale(0x100)>>8; 既定 scale=256 なので tempoI = 2×value。
//      Func_080faa58 の初期化も既定値 75×2 = 150 = 1 tick/frame と整合)。
//   ・VBlank 周波数 = GBA のフレーム周期 280896 サイクル / 16.78MHz。
//   ⇒ ticks/sec = fps × 2 × value / 150。壁時計 = 総 tick / この値。
const FRAMES_PER_SECOND = 16777216 / 280896; // ≈ 59.7275 Hz (GBA VBlank)
const TEMPO_THRESHOLD = 150; // 累算器のしきい値 (Func_080f9c90)
const TEMPO_MULTIPLIER = 2; // tempo コマンドの 2 倍 (Func_080f9b4c, 既定 scale)

export function ticksPerSecond(controlValue: number): number {
  return (FRAMES_PER_SECOND * TEMPO_MULTIPLIER * controlValue) / TEMPO_THRESHOLD;
}

// テンポ制御値 → MIDI set-tempo ペイロード (1拍あたりマイクロ秒, 3バイト BE)。
// 四分音符 = PPQN tick なので microsPerQuarter = PPQN / ticksPerSecond。
function tempoPayload(controlValue: number): number[] {
  const microsPerQuarter = Math.round((PPQN * 1_000_000) / ticksPerSecond(controlValue));
  return [
    (microsPerQuarter >> 16) & 0xff,
    (microsPerQuarter >> 8) & 0xff,
    microsPerQuarter & 0xff,
  ];
}

// ── パターン定義の本体を索引化 ──────────────────────────────────────
// このデータでは pattern 呼び出しはすべて後方参照・入れ子なし・repeat なし。
// 定義は `["label", P]` から最初の `["pattern_end"]` までの区間。呼び出し時に
// この本体の音符と wait を現在 tick へインライン展開する (再生を時間正確に)。
function patternBodies(events: SequenceEvent[]): Map<string, SequenceEvent[]> {
  const targets = new Set(
    events.filter((e) => e[0] === "pattern").map((e) => e[1] as string),
  );
  const bodies = new Map<string, SequenceEvent[]>();
  for (let i = 0; i < events.length; i++) {
    const event = events[i]!;
    if (event[0] === "label" && targets.has(event[1] as string) && !bodies.has(event[1] as string)) {
      const body: SequenceEvent[] = [];
      for (let k = i + 1; k < events.length; k++) {
        if (events[k]![0] === "pattern_end") break;
        body.push(events[k]!);
      }
      bodies.set(event[1] as string, body);
    }
  }
  return bodies;
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

  // ソングヘッダのリバーブ量 (曲全体に適用)。ヘッダが無い試作曲は 0 (ドライ)。
  const headerSegment = source.layout.find(
    (s): s is Extract<SequenceSegment, { kind: "header" }> => s.kind === "header",
  );
  const reverbCC = reverbSend(Number(headerSegment?.reverb ?? 0));
  // 曲がどのトーンバンクを参照するか (全チャンネル共通)。
  const bankMSB = bankSelectFor(source.externals?.tone_bank as string | undefined);

  streamSegments.forEach((segment, index) => {
    const channel = streamChannel(index);
    const raw: RawMidiEvent[] = [];
    let cursor = 0; // 時間カーソル (tick)
    let seq = 0;
    const noteState = { key: 0, vel: 0 };
    // ゲート満了イベントは大きな seq を与えて同一 tick の grid イベントの後に置く。
    let offSeqBase = 1_000_000;

    const bodies = patternBodies(segment.events);
    // goto の飛び先ラベル (= ループ開始点) に loopStart キューを添える。
    const loopTargets = new Set(
      segment.events.filter((e) => e[0] === "goto").map((e) => e[1] as string),
    );

    // 曲頭で (1) バンクセレクト → (2) リバーブ送りを送る。バンクはプログラム
    // チェンジより前に置く必要がある (プレイヤがバンクを確定してから音色を引く)。再生専用。
    raw.push({ tick: 0, seq: seq++, bytes: controlChange(channel, CC_BANK_MSB, bankMSB) });
    raw.push({ tick: 0, seq: seq++, bytes: controlChange(channel, CC_REVERB, reverbCC) });
    // 既定のピッチベンド幅 (エンジンのトラック初期値 = 2 半音) を RPN0 で確定する。
    // pitch_bend_range コマンドが来ればその都度上書きされる。再生専用。
    for (const cc of rpnWrite(channel, 0, RPN_BEND_RANGE, DEFAULT_BEND_RANGE, 0)) {
      raw.push({ tick: 0, seq: seq++, bytes: cc });
    }

    // 現在 tick に note-on / note-off を積む (再生用の実音)。
    const emitNote = (event: SequenceEvent, state: { key: number; vel: number }): void => {
      const { dur, key, vel } = effectiveNoteParams(event, state);
      raw.push({ tick: cursor, seq: seq++, bytes: [0x90 | channel, key & 0x7f, vel & 0x7f] });
      raw.push({ tick: cursor + dur, seq: offSeqBase++, bytes: [0x80 | channel, key & 0x7f, 0x40] });
    };

    for (const event of segment.events) {
      const kind = event[0];
      if (isNote(event)) {
        emitNote(event, noteState);
      } else if (isWait(event)) {
        cursor += event[1] as number;
      } else if (kind === "label") {
        raw.push({ tick: cursor, seq: seq++, bytes: metaText(0x06, JSON.stringify(event)) });
        if (loopTargets.has(event[1] as string)) {
          raw.push({ tick: cursor, seq: seq++, bytes: metaText(CUE_META, "loopStart") });
        }
      } else if (kind === "pattern") {
        // 逐語マーカ (再構築で保持) を先に置き、その後に展開ブラケットを開く。
        raw.push({ tick: cursor, seq: seq++, bytes: metaText(0x06, JSON.stringify(event)) });
        raw.push({ tick: cursor, seq: seq++, bytes: metaText(CUE_META, "pattern+") });
        // 本体の音符を現在 tick へインライン (走行状態は複製して外側を汚さない)。
        const clone = { ...noteState };
        for (const bodyEvent of bodies.get(event[1] as string) ?? []) {
          if (isNote(bodyEvent)) emitNote(bodyEvent, clone);
          else if (isWait(bodyEvent)) cursor += bodyEvent[1] as number;
          // 本体内の control 等は再生に寄与せず再構築でも捨てるので出さない。
        }
        raw.push({ tick: cursor, seq: seq++, bytes: metaText(CUE_META, "pattern-") });
      } else if (kind === "goto") {
        // 上位ループは一度だけ本体を鳴らして goto では飛ばない (fine で止める)。
        raw.push({ tick: cursor, seq: seq++, bytes: metaText(CUE_META, "loopEnd") });
        raw.push({ tick: cursor, seq: seq++, bytes: metaText(0x06, JSON.stringify(event)) });
      } else {
        // control / fine / pattern_end / note_end / *_running — 逐語マーカ。
        raw.push({ tick: cursor, seq: seq++, bytes: metaText(0x06, JSON.stringify(event)) });
        // tempo 制御には実テンポメタも重ねる (プレイヤの 120BPM 既定を防ぐ)。
        if (kind === "tempo") {
          raw.push({ tick: cursor, seq: seq++, bytes: metaBytes(TEMPO_META, tempoPayload(event[1] as number)) });
        }

        // ── 再生専用チャンネルイベントへの写像 ──────────────────────
        // explicit 形 (["name", v]) と走行形 (["control_running","name", v]) を
        // 制御名・値に正規化してから写す。写せない命令はマーカだけ残る。
        let ctrlName: ControlName | null = null;
        let ctrlValue = 0;
        if (kind === "control_running") {
          ctrlName = event[1] as ControlName;
          ctrlValue = event[2] as number;
        } else if (CONTROL_NAMES.has(kind as ControlName)) {
          ctrlName = kind as ControlName;
          ctrlValue = event[1] as number;
        }
        const emit = (bytes: number[]): void => {
          raw.push({ tick: cursor, seq: seq++, bytes });
        };
        switch (ctrlName) {
          case "voice":
            // 音色選択 → プログラムチェンジ。
            emit(programChange(channel, ctrlValue));
            break;
          case "volume":
            // エンジンの音量 (#18、0..127 直値) → チャンネルボリューム CC7。
            emit(controlChange(channel, CC_VOLUME, ctrlValue & 0x7f));
            break;
          case "pan":
            // エンジンの定位 (中立 64、生の引数が MIDI パン域) → パン CC10。
            emit(controlChange(channel, CC_PAN, ((ctrlValue % 256) + 256) & 0x7f));
            break;
          case "modulation_depth":
            // トラック LFO 深さ → CC1 (SF2 の既定モジュレータがビブラートを掛ける)。
            emit(controlChange(channel, CC_MODWHEEL, ctrlValue & 0x7f));
            break;
          case "pitch_bend":
            // 実ベンド量 (引数 − 64) を正規化した 14bit ホイールに写す。RPN0 に幅を
            // 設定済みなので、プレイヤは range 半音を全振りとして鳴らす。
            emit(pitchWheel(channel, bendToWheel(ctrlValue)));
            break;
          case "pitch_bend_range":
            // ベンド幅 (半音) を RPN0 に設定する。以後のホイールがこの幅で鳴る。
            for (const cc of rpnWrite(channel, 0, RPN_BEND_RANGE, ctrlValue & 0x7f, 0)) emit(cc);
            break;
          case "tuning":
            // 微調律 (引数 − 64、±1 半音域) を RPN1 (14bit) に写す。
            {
              const value14 = Math.max(0, Math.min(0x3fff, bendToWheel(ctrlValue)));
              for (const cc of rpnWrite(channel, 0, RPN_FINE_TUNE, (value14 >> 7) & 0x7f, value14 & 0x7f)) {
                emit(cc);
              }
            }
            break;
          default:
            // priority / key_shift(全曲 0) / lfo_speed / lfo_delay / modulation_type:
            // 標準 MIDI・現行 SF2 で確実に鳴る対応が無いためマーカのみ (下記コメント参照)。
            break;
        }
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
//
// 逆写像の要点: sequenceToMidi はパターン呼び出しの本体を展開複製し、上位ループを
// 一度だけ鳴らす。これらは 0x07 キューで括られる。ここではブラケット内の note-on と
// 0x06 マーカを捨て、ブラケットの tick 長を「コンパクト時間軸」から差し引くことで、
// 展開前の compact stream (= 元のエンジンイベント列) をバイト完全に復元する。
// loopStart/loopEnd と set-tempo(0x51) は再生専用なので無視する。
function reconstructStream(events: MidiEvent[]): SequenceEvent[] {
  interface Node {
    compactTick: number; // ブラケット長を除いた時間軸 (元の compact 位置)
    rawTick: number; // 展開後の実 tick (ゲート算出に使う)
    order: number;
    event: SequenceEvent;
    dropped: boolean; // ブラケット内 (展開複製) なら true
  }
  const grid: Node[] = []; // 保持ノード (dropped でないもの) のみ
  // ピッチごとの note-on 待ち行列 (FIFO)。展開複製も含めゲート対応に使う。
  const pending = new Map<number, Node[]>();

  let depth = 0; // パターン展開ブラケットの入れ子深さ
  let bracketStart = 0; // 現ブラケットの開始 tick
  let removed = 0; // これまでに閉じたブラケット長の総和

  for (const e of events) {
    if (e.type === "meta" && e.meta === 0x2f) continue; // EOT
    if (e.type === "meta" && e.meta === TEMPO_META) continue; // 再生用テンポは無視
    if (e.type === "meta" && e.meta === CUE_META) {
      const text = Buffer.from(e.data as string, "hex").toString("utf8");
      if (text === "pattern+") {
        if (depth === 0) bracketStart = e.tick;
        depth += 1;
      } else if (text === "pattern-") {
        depth -= 1;
        if (depth === 0) removed += e.tick - bracketStart;
      }
      // loopStart / loopEnd: 目印のみ。構造は 0x06 の label/goto が担うので無視。
      continue;
    }
    if (e.type === "meta" && e.meta === 0x06) {
      if (depth > 0) continue; // 展開複製の control マーカ → 捨てる
      const parsed = JSON.parse(
        Buffer.from(e.data as string, "hex").toString("utf8"),
      ) as SequenceEvent;
      grid.push({ compactTick: e.tick - removed, rawTick: e.tick, order: e.order, event: parsed, dropped: false });
      continue;
    }
    if (e.type === "channel" && e.status !== undefined) {
      const kind = e.status & 0xf0;
      // note-on/off 以外の再生専用チャンネルイベント (バンクセレクト/CC1/CC7/CC10/
      // CC91/プログラムチェンジ/ピッチホイール 0xE0/RPN 各 CC 等) は逆写像で無視する。
      if (kind !== 0x90 && kind !== 0x80) continue;
      const [d0, d1] = e.data as number[];
      if (kind === 0x90 && d1 > 0) {
        // note-on。ゲートは後で note-off から確定 (仮に 0)。
        const node: Node = {
          compactTick: e.tick - removed, rawTick: e.tick, order: e.order,
          event: ["note", 0, d0, d1], dropped: depth > 0,
        };
        if (!node.dropped) grid.push(node);
        if (!pending.has(d0)) pending.set(d0, []);
        pending.get(d0)!.push(node);
      } else if (kind === 0x80 || (kind === 0x90 && d1 === 0)) {
        // note-off。対応する最古の note-on にゲート (展開後 tick 差) を書き込む。
        const queue = pending.get(d0);
        if (!queue || queue.length === 0) throw new Error("対応しない note-off");
        const on = queue.shift()!;
        (on.event as [string, number, number, number])[1] = e.tick - on.rawTick;
      }
    }
  }
  if ([...pending.values()].some((q) => q.length > 0)) {
    throw new Error("閉じていない note-on が残っている");
  }
  if (depth !== 0) throw new Error("パターン展開ブラケットが閉じていない");

  // 保持ノードを compact 時間軸で並べ、隣接ギャップを wait に分解して挿入。
  grid.sort((a, b) => a.compactTick - b.compactTick || a.order - b.order);
  const out: SequenceEvent[] = [];
  let cursor = 0;
  for (const g of grid) {
    if (g.compactTick > cursor) {
      out.push(...tokenizeWait(g.compactTick - cursor));
      cursor = g.compactTick;
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
  await Bun.write(`${outDir}/${name}.roundtrip.json`, canonicalJson(rebuilt) + "\n");

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
