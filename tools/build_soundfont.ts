#!/usr/bin/env bun
// Tool role: dead; no tracked importer or invocation; audit before use.
// build_soundfont.ts — 回収済み音声データから SoundFont (SF2 v2.01) を決定論的に生成する。
//
// ── 目的 ────────────────────────────────────────────────────────────────
//   リポジトリ内で回収済みの素材だけを使い、out/audio_midi/golden-sun.sf2 を
//   生成する。外部プロジェクト (goldensunvst / agbplay / GBA Mus Ripper 等) は
//   一切参照しない。SF2 形式そのものは公開仕様であり、その仕様に従って実装する。
//
// ── 入力 (すべてリポジトリ内・追跡対象) ─────────────────────────────────
//   ・assets/audio/waves/index.json + wave_NN.pcm8.wav
//       32 個の署名付き PCM 波形。各波形の周波数 (frequency = サンプルレート×1024)
//       とループ開始位置を保持する。コーデックは tools/audio_wave.ts。
//   ・assets/audio/engine/onshoku.json  … トーンバンク (225 トーンレコード)。
//       mp2k 系の 12 バイトトーン形式。各レコードは種別 (pcm / pulse_1 / pulse_2 /
//       wave(CGB ch3) / noise / rhythm)、ルートキー、パン、ADSR エンベロープ、
//       および PCM サンプルアドレスまたは PSG パラメータを持つ。
//   ・assets/audio/engine/hakei.json    … 18 個の CGB プログラマブル波形 (32 nibble)。
//
// ── プログラム番号の対応 (最重要) ───────────────────────────────────────
//   エクスポートされた MIDI (tools/midi_roundtrip.ts) は音色選択を標準 MIDI の
//   プログラムチェンジ (0xC0) ではなく、0x06 マーカーメタイベントに JSON
//   ["voice", N] として格納する。N はソング先頭 (コンダクタトラックの 0x01 メタ)
//   の externals.tone_bank が指すボイスグループ内のトーンレコード番号である。
//   実測: N は 0..127。ソングは 2 つのボイスグループのいずれかを参照する。
//     ・tone_bank = 0x080fba78  → bank_0 (144 レコード) …… 58 ソング
//     ・tone_bank = 0x080fc138  → bank_1 (81 レコード)  …… 202 ソング
//   したがって SF2 側は次の規約で番号付けする。下流のマーカー→プログラム変換器が
//   この規約で (bank, program) を選べば正しい音色に解決する。
//     ・SF2 bank 0, preset N  = bank_0 のトーンレコード N   (N = 0..127)
//     ・SF2 bank 1, preset N  = bank_1 のトーンレコード N   (N = 0..80)
//   MIDI 側の対応: tone_bank 0x080fba78 → Bank Select 0、0x080fc138 → Bank Select 1。
//
// ── サンプル変換 ────────────────────────────────────────────────────────
//   GBA の PCM は 8-bit 署名付き (tools/audio_wave.ts が WAV の +128 バイアスを反転)。
//   SF2 は 16-bit 署名付きのため int8 を <<8 (×256) して格納する。サンプルレートは
//   frequency/1024。ループは wave メタの loop_start (null なら非ループ)。ルートキーは
//   トーンレコードの key フィールド (bank_0 では 55/60/65 が出現する)。
//
// ── エンベロープ (ROM 実測で確定) ───────────────────────────────────────
//   トーンレコードのバイト 8..11 = [attack, decay, sustain, release]。この意味は
//   混合器の音量更新ループ Func_080f9674 (asm/080f9674.s) の逐次解析で確定した。
//   ボイス構造体 r4 のフェーズ処理 (毎サウンドフレーム 1 回):
//     Attack  (.L0300714a): envVol += attack   ; ldrb [r4,#4]→adds r5,r5,r0、255 で飽和
//                           → 立ち上がりは「加算」。到達フレーム数 = ceil(255/attack)。
//     Decay   (.L03007128): envVol  = envVol*decay>>8 ; ldrb [r4,#5]→muls,lsrs #8
//                           sustain ([r4,#6]) 以下で保持 → 指数減衰。
//     Sustain (.L0300711a): sustain レベル ([r4,#6], 0..255) を保持。
//     Release (.L03007108): envVol  = envVol*release>>8 ; ldrb [r4,#7]→muls,lsrs #8
//                           → 指数減衰。0x40 (stop) フラグ成立で突入。
//   すなわち Attack は線形加算、Decay/Release は 256 を分母とする乗算指数減衰。
//   これを SF2 の timecents (時間、1200×log2(秒)) / centibels (減衰量、-200×log10(比))
//   に変換する (下記 dsEnvelope)。フレームレートは GBA VBlank ≒ 59.7275Hz だが、
//   従来どおり 60Hz 近似 (FRAME_HZ) を用いる (差は 0.5% で可聴閾以下)。
//   PSG (CGB) トーンの 4 バイトは CGB ハードウェア寄りの意味 (0..15 のカウンタ/レベル)
//   であり、正確な再現規則は本リポジトリ内で完全には回収できていない。よって PSG は
//   「合成サンプル近似」として簡易 CGB エンベロープ (cgbEnvelope) を当て、
//   ヘッダーに近似である旨を明記する。
//
// ── ビブラート (LFO → ピッチ変調) ───────────────────────────────────────
//   エンジンのトラック処理 Func_080f9c90 (asm/080f9c90.s) は各ボイスに三角波 LFO を
//   持ち、既定 LFO 速度 22 を [r5,#25] に書き込む (movs r0,#22)。LFO 位相 [r5,#26] は
//   毎フレーム速度分だけ進み、256 で 1 周する。したがって既定ビブラート周波数 =
//   22 × VBlank(≒59.7275Hz) / 256 ≒ 5.13Hz。これを SF2 の freqVibLfo (絶対セント、
//   基準 8.176Hz) に変換して全メロディゾーンへ与える (VIB_LFO_CENTS)。
//   変調の深さはシーケンスの modulation_depth コマンドが決める。これを
//   midi_roundtrip.ts の書き出しで標準 MIDI の CC1 (モジュレーションホイール) に写す。
//   SF2 仕様 8.4.2 の既定モジュレータ集合には「CC1 → vibLfoToPitch (+50 セント)」が
//   含まれ、fluidsynth はこれを自動適用する。よって CC1 が 0..127 を動くと最大 ±50
//   セント (半音幅 p-p) のビブラートが掛かり、深さがトラックの MOD 値に追従する。
//   (エンジンの MOD→セント厳密係数は本リポジトリ内で数値化されていないため、深さは
//    SF2 既定モジュレータ由来の musical な値を用い、速度のみエンジン実測に合わせる。)
//   modulation_type は 260 曲すべてで 0 (ピッチ=ビブラート) のため、種別分岐は不要。
//
// ── リバーブ ────────────────────────────────────────────────────────────
//   リバーブ量は曲ごとにソングヘッダのバイトで数値化されている (bit7=有効、下位7bit=量)。
//   実測分布: 0 → 186 曲、128(=有効/量0) → 16 曲、178(=有効/量50) → 58 曲。曲ごとに
//   異なるため、全曲共有の SF2 では単一の reverbEffectsSend ジェネレータで表せない。
//   よって曲固有値として midi_roundtrip.ts の書き出しで CC91 (リバーブ送り) に写し、
//   プレイヤ (fluidsynth 既定リバーブ) 側で曲ごとに掛ける。SF2 側では静的量を付けない。
//
// ── PSG チャンネル (トーンバンクが参照するため合成近似を同梱する) ───────
//   ・pulse_1 / pulse_2 … デューティ矩形波 (generator 0/1/2/3 = 12.5/25/50/75%)。
//   ・noise             … 15-bit / 7-bit LFSR ノイズ (generator 0/1)。ピッチ非追従の近似。
//   ・wave (CGB ch3)    … hakei.json の 32 nibble を 1 周期波形として合成。
//   いずれも 1 種につき 1 サンプルを共有し、各インストゥルメントで overridingRootKey を
//   レコードの key に設定してピッチを合わせる。5 個のゼロ長シンセ記述子
//   (0x0811dac8..0x0811db24) を指す pcm レコードは実サンプルを持たないため無音に落とす。
//
// ── 出力 / 検証 ─────────────────────────────────────────────────────────
//   out/audio_midi/golden-sun.sf2 (out/ は .gitignore 済み)。生成後に構造自己検査を
//   行う (チャンクサイズ整合、全 sampleID / instrument インデックスの妥当性)。
//   fluidsynth が存在すれば MIDI を 1 本レンダリングしてスモークテストする。
//
// 使い方: bun tools/build_soundfont.ts            # 生成 + 自己検査 (+ 可能ならスモーク)
//         bun tools/build_soundfont.ts --self-test

import { existsSync, mkdirSync, readFileSync, readdirSync, writeFileSync } from "fs";
import { execFileSync } from "child_process";
import { dirname, join, resolve } from "path";
import { signedPcmFromWav } from "./audio_wave.ts";

// ── リポジトリ内パス ────────────────────────────────────────────────────
const REPO = resolve(dirname(new URL(import.meta.url).pathname), "..");
const AUDIO = join(REPO, "assets", "audio");
const WAVES_DIR = join(AUDIO, "waves");
const ENGINE_DIR = join(AUDIO, "engine");
const MIDI_DIR = join(AUDIO, "midi");
const OUT_DIR = join(REPO, "out", "audio_midi");
const OUT_SF2 = join(OUT_DIR, "golden-sun.sf2");

// ── ボイスグループ定義 (audio_engine_data.ts と一致) ────────────────────
const BANK_0_ADDRESS = 0x080fba78;
const BANK_1_ADDRESS = 0x080fc138;
const BANK_0_COUNT = 144;
const BANK_1_COUNT = 81;

// ── エンジン想定値 ──────────────────────────────────────────────────────
const FRAME_HZ = 60; // mp2k のエンベロープ更新フレームレート (仮定)。
const VBLANK_HZ = 16777216 / 280896; // GBA VBlank ≒ 59.7275Hz (LFO 周波数の導出に使用)。
const MP2K_LFO_SPEED = 22; // 既定 LFO 速度 (Func_080f9c90 が [r5,#25] に書く値)。
// 既定ビブラート周波数 = speed × VBlank / 256。SF2 freqVibLfo は 8.176Hz 基準の絶対セント。
const VIB_LFO_HZ = (MP2K_LFO_SPEED * VBLANK_HZ) / 256; // ≒ 5.13Hz
const VIB_LFO_CENTS = Math.round(1200 * Math.log2(VIB_LFO_HZ / 8.176)); // ≒ -807

// ── 合成 PSG のパラメータ ───────────────────────────────────────────────
const SQUARE_PERIOD = 64; // 1 周期のサンプル数 (デューティ分解能)。
const NOISE_LEN = 32768; // ノイズループ長。
const PSG_AMP = 10000; // PSG 振幅 (16-bit フルスケール手前)。
const CGB_ROOT = 60; // 合成 PSG の shdr 基準キー (実ピッチは各ゾーンで上書き)。

// ── SF2 ジェネレータ番号 (仕様 8.1.3) ───────────────────────────────────
const GEN = {
  keyRange: 43,
  pan: 17,
  attackVolEnv: 34,
  decayVolEnv: 36,
  sustainVolEnv: 37,
  releaseVolEnv: 38,
  freqVibLfo: 24, // SF2 仕様 8.1.3: ビブラート LFO 周波数 (絶対セント)。8=initialFilterFc なので厳禁。
  initialAttenuation: 48,
  scaleTuning: 56,
  overridingRootKey: 58,
  sampleModes: 54,
  sampleID: 53,
  instrument: 41,
} as const;

// ── 汎用ヘルパ ──────────────────────────────────────────────────────────
function readJson(path: string): any {
  return JSON.parse(readFileSync(path, "utf8"));
}

function freqOfKey(key: number): number {
  return 440 * Math.pow(2, (key - 69) / 12);
}

function clamp(value: number, low: number, high: number): number {
  return Math.max(low, Math.min(high, value));
}

// 秒 → timecents。0 秒付近は下限に丸める。
function secToTimecents(seconds: number): number {
  const s = Math.max(seconds, 1e-6);
  return Math.round(clamp(1200 * Math.log2(s), -12000, 8000));
}

// ── DirectSound ADSR → SF2 変換 ─────────────────────────────────────────
function dsEnvelope(adsr: number[]): {
  attack: number; decay: number; sustain: number; release: number;
} {
  const [A, D, S, R] = adsr;
  // Attack: 0→255 まで A ずつ加算。A=0 は事実上「立ち上がらない」ため最小時間扱い。
  const attackFrames = A <= 0 ? 1 : Math.ceil(255 / A);
  const attack = secToTimecents(attackFrames / FRAME_HZ);
  // Decay: 255→sustain へ *D/256 で指数減衰。頂点が既に sustain 以下、または D=0 は即時。
  let decayFrames = 1;
  if (D > 0 && D < 256 && S < 255 && S > 0) {
    decayFrames = Math.max(1, Math.log(S / 255) / Math.log(D / 256));
  } else if (D > 0 && D < 256 && S === 0) {
    // sustain=0 は -96dB 相当まで減衰させる。
    decayFrames = Math.max(1, Math.log(Math.pow(2, -16)) / Math.log(D / 256));
  }
  const decay = secToTimecents(decayFrames / FRAME_HZ);
  // Sustain: 振幅比 S/255 の減衰量を centibels で。0dB=0, 無音=1440(上限)。
  const sustain = S <= 0 ? 1440 : Math.round(clamp(-200 * Math.log10(S / 255), 0, 1440));
  // Release: *R/256 で 2^-16 (≈-96dB) に達するまでのフレーム数。
  let releaseFrames = 1;
  if (R > 0 && R < 256) {
    releaseFrames = Math.max(1, Math.log(Math.pow(2, -16)) / Math.log(R / 256));
  }
  const release = secToTimecents(releaseFrames / FRAME_HZ);
  return { attack, decay, sustain, release };
}

// ── CGB (PSG) エンベロープ近似 ──────────────────────────────────────────
//   4 バイトを [attack, decay, sustain(0..15), release] のカウンタ/レベルとして扱う簡易近似。
//   本リポジトリ内では正確な CGB 更新規則が完全には回収できていないため近似である。
function cgbEnvelope(adsr: number[]): {
  attack: number; decay: number; sustain: number; release: number;
} {
  const [A, D, S, R] = adsr;
  // カウンタ 1 段 ≈ 1/60 秒、全 16 段を目安に線形換算。
  const attack = secToTimecents(Math.max(A, 0) * 16 / FRAME_HZ || 1e-3);
  const sustain = S >= 15 ? 0 : Math.round(clamp(-200 * Math.log10(Math.max(S, 1) / 15), 0, 1440));
  const decay = secToTimecents(Math.max(D, 0) * 16 / FRAME_HZ || 1e-3);
  const release = secToTimecents(Math.max(R, 1) * 16 / FRAME_HZ);
  return { attack, decay, sustain, release };
}

// ── DirectSound パン (byte3) → SF2 pan (±500) ───────────────────────────
function dsPan(panSweep: number): number | null {
  if ((panSweep & 0x80) === 0) return null; // 強制パン無効 → 中央 (ゾーンに書かない)。
  const pan7 = panSweep & 0x7f; // 64 = 中央。
  return Math.round(clamp((pan7 - 64) / 64 * 500, -500, 500));
}

// ── サンプル生成 ────────────────────────────────────────────────────────
interface SampleDef {
  name: string;
  data: Int16Array; // 16-bit PCM 本体 (ガードは smpl 組み立て時に付与)。
  rate: number;
  rootKey: number;
  loopStart: number; // 絶対サンプル index (本体内)。
  loopEnd: number;   // 絶対サンプル index (本体内・ループ終端)。
  looped: boolean;
}

// 署名付き 8-bit WAV → 16-bit Int16Array。
function pcm16FromWav(wav: Buffer, rate: number): Int16Array {
  const signed = signedPcmFromWav(wav, rate); // 各バイト = 二の補数署名付きサンプル。
  const out = new Int16Array(signed.length);
  for (let i = 0; i < signed.length; i++) {
    const s8 = (signed[i] << 24) >> 24; // int8 に符号拡張。
    out[i] = s8 * 256;
  }
  return out;
}

// 矩形波 1 周期を生成 (duty 0/1/2/3 = 12.5/25/50/75%)。
function squareSample(duty: number): SampleDef {
  const highFraction = [0.125, 0.25, 0.5, 0.75][duty];
  const highCount = Math.round(SQUARE_PERIOD * highFraction);
  const data = new Int16Array(SQUARE_PERIOD);
  for (let i = 0; i < SQUARE_PERIOD; i++) data[i] = i < highCount ? PSG_AMP : -PSG_AMP;
  const rate = Math.round(SQUARE_PERIOD * freqOfKey(CGB_ROOT));
  return {
    name: `psg_square_${duty}`, data, rate, rootKey: CGB_ROOT,
    loopStart: 0, loopEnd: SQUARE_PERIOD, looped: true,
  };
}

// LFSR ノイズ (mode 0 = 15-bit, mode 1 = 7-bit)。
function noiseSample(mode: number): SampleDef {
  const data = new Int16Array(NOISE_LEN);
  let reg = 0x7fff;
  const width = mode === 1 ? 6 : 14; // 7-bit モードは bit6 へも戻す。
  for (let i = 0; i < NOISE_LEN; i++) {
    const bit = (reg ^ (reg >> 1)) & 1;
    reg = (reg >> 1) | (bit << 14);
    if (mode === 1) reg = (reg & ~(1 << width)) | (bit << width);
    data[i] = (reg & 1) ? -PSG_AMP : PSG_AMP;
  }
  return {
    name: `psg_noise_${mode}`, data, rate: 44100, rootKey: CGB_ROOT,
    loopStart: 0, loopEnd: NOISE_LEN, looped: true,
  };
}

// CGB プログラマブル波形 (32 nibble) を 1 周期波形に。
function cgbWaveSample(index: number, nibbles: number[]): SampleDef {
  const data = new Int16Array(32);
  for (let i = 0; i < 32; i++) data[i] = Math.round((nibbles[i] - 7.5) / 7.5 * PSG_AMP);
  const rate = Math.round(32 * freqOfKey(CGB_ROOT));
  return {
    name: `cgb_wave_${index.toString().padStart(2, "0")}`, data, rate, rootKey: CGB_ROOT,
    loopStart: 0, loopEnd: 32, looped: true,
  };
}

// 無音サンプル (rhythm キットの空ゾーン用の受け皿)。
function silenceSample(): SampleDef {
  return {
    name: "silence", data: new Int16Array(48), rate: 22050, rootKey: 60,
    loopStart: 0, loopEnd: 48, looped: true,
  };
}

// GS シンセ楽器の近似波形 (種別ごとに帯域制限の別波形)。
//   ゴールデンサンの「シンセ」音色 (bank_0 のプログラム 80..99、bank_1 の 60..69) は
//   ROM 上ではゼロ長 WaveData 記述子 (0x0811dac8..0x0811db24 の 5 個) を指し、実サンプルを
//   持たない (WaveData ヘッダの length=0。実 PCM 例 wave_00 は length=666 とサイズ整合し、
//   このゼロ長を確定)。実機ではソフトウェアシンセが波形を手続き的に生成する。
//   ── 記述子の判別 (ROM 実測) ───────────────────────────────────────────
//   各記述子はヘッダ 16 バイトの直後に 8 バイトのシンセパラメータを持ち、先頭バイトは
//   すべて 0x80 (シンセ標識)、続く第2バイトが波形種別を選ぶ。5 記述子の実測値:
//     0x0811dac8: 80 00 10 f0 e0 80 00 00  … 種別 0
//     0x0811dae0: 80 00 60 20 40 80 00 00  … 種別 0
//     0x0811daf8: 80 00 80 40 40 40 00 00  … 種別 0
//     0x0811db10: 80 01 00 00 00 00 00 40  … 種別 1
//     0x0811db24: 80 02 00 00 00 00 00 00  … 種別 2
//   第2バイトが 0/1/2 の 3 系統に分かれる (種別 0 のみ整形バイトを伴う)。従来はこの 5 個を
//   すべて 1 本のノコギリ波 (gs_synth) に潰していたため、種別の異なる音色 (例: sound_000 の
//   最後まで鳴る鋸→矩形系リード) が同じ音になっていた。ここで種別ごとに波形を分ける:
//     種別 0 → 帯域制限ノコギリ波 (従来と同一。多数の種別0曲を回帰させない)。
//     種別 1 → 帯域制限三角波 (柔らかいリード)。
//     種別 2 → 帯域制限矩形波 50% (中空のリード)。
//   (エンジンの厳密な合成規則は本リポジトリ内で完全には回収できていないため音色は近似。
//    種別の分離のみ ROM バイトに基づく確定事実。)
// いずれも DirectSound (kind=pcm) 音色として実 PCM と同じミキサ経路
// (Func_080f9674: 標本値 × envVol × chanVol) を通る。したがって同一エンベロープ・
// 速度で実 PCM 音色と釣り合うよう、波形の RMS を実 PCM サンプル群の中央値 RMS に
// 合わせる (targetRms)。
type SynthFamily = 0 | 1 | 2;
const SYNTH_NAME: Record<SynthFamily, string> = {
  0: "gs_synth_saw",
  1: "gs_synth_tri",
  2: "gs_synth_pulse",
};

// ゼロ長シンセ記述子アドレス → 波形種別 (第2バイト)。ROM 実測 (上表)。
const SYNTH_DESCRIPTORS: Record<string, SynthFamily> = {
  "0x0811dac8": 0,
  "0x0811dae0": 0,
  "0x0811daf8": 0,
  "0x0811db10": 1,
  "0x0811db24": 2,
};

function synthLeadSample(family: SynthFamily, targetRms: number): SampleDef {
  const N = 128; // 1 周期のサンプル数
  const H = 24; // 加算する倍音数 (エイリアス抑制のための帯域制限)
  const raw = new Float64Array(N);
  let sumSq = 0;
  for (let i = 0; i < N; i++) {
    let v = 0;
    for (let k = 1; k <= H; k++) {
      const phase = (2 * Math.PI * k * i) / N;
      if (family === 0) {
        v += Math.sin(phase) / k; // 鋸波: 全倍音、振幅 1/k。
      } else if (family === 1) {
        if (k % 2 === 1) v += (((k - 1) / 2) % 2 === 0 ? 1 : -1) * Math.sin(phase) / (k * k); // 三角波: 奇数倍音、1/k^2 交番。
      } else {
        if (k % 2 === 1) v += Math.sin(phase) / k; // 矩形波 50%: 奇数倍音、1/k。
      }
    }
    raw[i] = v;
    sumSq += v * v;
  }
  const rawRms = Math.sqrt(sumSq / N);
  const gain = targetRms / rawRms; // RMS を実 PCM 中央値に合わせる
  const data = new Int16Array(N);
  for (let i = 0; i < N; i++) data[i] = Math.round(clamp(raw[i] * gain, -32767, 32767));
  const rate = Math.round(N * freqOfKey(60)); // 基準キー60で中央ハ
  return {
    name: SYNTH_NAME[family], data, rate, rootKey: 60,
    loopStart: 0, loopEnd: N, looped: true,
  };
}

// ── SF2 バイナリ組み立て ────────────────────────────────────────────────
function riffChunk(id: string, data: Buffer): Buffer {
  const head = Buffer.alloc(8);
  head.write(id, 0, "ascii");
  head.writeUInt32LE(data.length, 4);
  const pad = data.length % 2 === 1 ? Buffer.alloc(1) : Buffer.alloc(0); // ワード整列。
  return Buffer.concat([head, data, pad]);
}

function listChunk(listType: string, subs: Buffer[]): Buffer {
  const body = Buffer.concat([Buffer.from(listType, "ascii"), ...subs]);
  const head = Buffer.alloc(8);
  head.write("LIST", 0, "ascii");
  head.writeUInt32LE(body.length, 4);
  return Buffer.concat([head, body]);
}

function zstring(text: string, size: number): Buffer {
  const buf = Buffer.alloc(size); // ゼロ埋め (NUL 終端込み)。
  Buffer.from(text, "ascii").copy(buf, 0, 0, Math.min(text.length, size - 1));
  return buf;
}

interface Zone { gens: Array<[number, number | [number, number]]>; }
interface Instrument { name: string; zones: Zone[]; }
interface Preset { name: string; bank: number; preset: number; instrumentIndex: number; }

function writeGen(oper: number, amount: number | [number, number]): Buffer {
  const buf = Buffer.alloc(4);
  buf.writeUInt16LE(oper, 0);
  if (Array.isArray(amount)) {
    buf.writeUInt8(amount[0], 2);
    buf.writeUInt8(amount[1], 3);
  } else {
    buf.writeInt16LE(amount, 2);
  }
  return buf;
}

// pdta の inst/ibag/igen を構築。
function buildInstChunks(instruments: Instrument[]): { inst: Buffer; ibag: Buffer; igen: Buffer } {
  const instRecs: Buffer[] = [];
  const ibagRecs: Buffer[] = [];
  const igenRecs: Buffer[] = [];
  for (const instrument of instruments) {
    const rec = Buffer.alloc(22);
    zstring(instrument.name, 20).copy(rec, 0);
    rec.writeUInt16LE(ibagRecs.length, 20);
    instRecs.push(rec);
    for (const zone of instrument.zones) {
      const bag = Buffer.alloc(4);
      bag.writeUInt16LE(igenRecs.length, 0);
      bag.writeUInt16LE(0, 2); // modNdx (モジュレータ未使用)。
      ibagRecs.push(bag);
      for (const [oper, amount] of zone.gens) igenRecs.push(writeGen(oper, amount));
    }
  }
  // 終端レコード。
  const term = Buffer.alloc(22);
  term.writeUInt16LE(ibagRecs.length, 20);
  instRecs.push(term);
  const bagTerm = Buffer.alloc(4);
  bagTerm.writeUInt16LE(igenRecs.length, 0);
  ibagRecs.push(bagTerm);
  igenRecs.push(Buffer.alloc(4)); // igen 終端。
  return {
    inst: Buffer.concat(instRecs),
    ibag: Buffer.concat(ibagRecs),
    igen: Buffer.concat(igenRecs),
  };
}

// pdta の phdr/pbag/pgen を構築。各プリセットは 1 ゾーン (instrument ジェネレータのみ)。
function buildPresetChunks(presets: Preset[]): { phdr: Buffer; pbag: Buffer; pgen: Buffer } {
  const phdrRecs: Buffer[] = [];
  const pbagRecs: Buffer[] = [];
  const pgenRecs: Buffer[] = [];
  for (const preset of presets) {
    const rec = Buffer.alloc(38);
    zstring(preset.name, 20).copy(rec, 0);
    rec.writeUInt16LE(preset.preset, 20);
    rec.writeUInt16LE(preset.bank, 22);
    rec.writeUInt16LE(pbagRecs.length, 24);
    phdrRecs.push(rec);
    const bag = Buffer.alloc(4);
    bag.writeUInt16LE(pgenRecs.length, 0);
    bag.writeUInt16LE(0, 2);
    pbagRecs.push(bag);
    pgenRecs.push(writeGen(GEN.instrument, preset.instrumentIndex));
  }
  const term = Buffer.alloc(38);
  zstring("EOP", 20).copy(term, 0);
  term.writeUInt16LE(pbagRecs.length, 24);
  phdrRecs.push(term);
  const bagTerm = Buffer.alloc(4);
  bagTerm.writeUInt16LE(pgenRecs.length, 0);
  pbagRecs.push(bagTerm);
  pgenRecs.push(Buffer.alloc(4));
  return {
    phdr: Buffer.concat(phdrRecs),
    pbag: Buffer.concat(pbagRecs),
    pgen: Buffer.concat(pgenRecs),
  };
}

// smpl プール + shdr を構築。各サンプルの後に 46 のゼロサンプル (補間ガード) を挿入。
function buildSampleData(samples: SampleDef[]): { smpl: Buffer; shdr: Buffer } {
  const GUARD = 46;
  const pool: number[] = [];
  const starts: number[] = [];
  for (const sample of samples) {
    starts.push(pool.length);
    for (let i = 0; i < sample.data.length; i++) pool.push(sample.data[i]);
    for (let i = 0; i < GUARD; i++) pool.push(0);
  }
  const smpl = Buffer.alloc(pool.length * 2);
  for (let i = 0; i < pool.length; i++) smpl.writeInt16LE(pool[i], i * 2);

  const shdrRecs: Buffer[] = [];
  samples.forEach((sample, index) => {
    const start = starts[index];
    const end = start + sample.data.length;
    const rec = Buffer.alloc(46);
    zstring(sample.name, 20).copy(rec, 0);
    rec.writeUInt32LE(start, 20);                  // dwStart
    rec.writeUInt32LE(end, 24);                    // dwEnd
    rec.writeUInt32LE(start + sample.loopStart, 28); // dwStartloop
    rec.writeUInt32LE(start + sample.loopEnd, 32);   // dwEndloop
    rec.writeUInt32LE(sample.rate, 36);            // dwSampleRate
    rec.writeUInt8(sample.rootKey, 40);            // byOriginalPitch
    rec.writeInt8(0, 41);                          // chPitchCorrection
    rec.writeUInt16LE(0, 42);                      // wSampleLink
    rec.writeUInt16LE(1, 44);                      // sfSampleType = 1 (monoSample)
    shdrRecs.push(rec);
  });
  shdrRecs.push(Buffer.alloc(46)); // EOS 終端。
  return { smpl, shdr: Buffer.concat(shdrRecs) };
}

// ── トーンレコード → インストゥルメントゾーンのジェネレータ列 ───────────
function toneZoneGens(
  record: any,
  sampleIndexOf: (name: string) => number,
  keyRange: [number, number] | null,
): Array<[number, number | [number, number]]> {
  const gens: Array<[number, number | [number, number]]> = [];
  if (keyRange) gens.push([GEN.keyRange, keyRange]); // keyRange は先頭に置く (仕様)。

  const kind = record.kind;
  const root = record.key;
  let sampleName: string;
  let envelope: number[] = record.envelope ?? [0, 0, 15, 0];
  let isPsg = false;

  if (kind === "pcm") {
    sampleName = pcmSampleName(record.sample);
  } else if (kind === "wave") {
    const wi = Number(record.waveform.slice(5)); // "wave_NN" → NN (hakei インデックス)。
    sampleName = `cgb_wave_${wi.toString().padStart(2, "0")}`;
    isPsg = true;
  } else if (kind === "pulse_1" || kind === "pulse_2") {
    sampleName = `psg_square_${record.generator}`;
    isPsg = true;
  } else if (kind === "noise") {
    sampleName = `psg_noise_${record.generator}`;
    isPsg = true;
  } else {
    sampleName = "silence";
  }

  const env = isPsg ? cgbEnvelope(envelope) : dsEnvelope(envelope);
  gens.push([GEN.overridingRootKey, root]);
  if (record.fixed_pitch) gens.push([GEN.scaleTuning, 0]); // 固定ピッチ: キーで音程を変えない。
  if (env.attack > -12000) gens.push([GEN.attackVolEnv, env.attack]);
  if (env.decay > -12000) gens.push([GEN.decayVolEnv, env.decay]);
  if (env.sustain > 0) gens.push([GEN.sustainVolEnv, env.sustain]);
  if (env.release > -12000) gens.push([GEN.releaseVolEnv, env.release]);

  // ビブラート LFO 速度をエンジン既定 (≒5.13Hz) に設定する。深さは CC1 → vibLfoToPitch の
  // SF2 既定モジュレータが与える (シーケンスの modulation_depth が CC1 を動かす)。
  gens.push([GEN.freqVibLfo, VIB_LFO_CENTS]);

  if (kind === "pcm") {
    const pan = dsPan(record.pan_sweep);
    if (pan !== null) gens.push([GEN.pan, pan]);
  }
  if (isPsg) gens.push([GEN.initialAttenuation, 60]); // PSG は控えめに。

  const sampleIndex = sampleIndexOf(sampleName);
  const looped = sampleLooped(sampleName);
  gens.push([GEN.sampleModes, looped ? 1 : 0]);
  gens.push([GEN.sampleID, sampleIndex]); // sampleID はゾーン最後に置く (仕様)。
  return gens;
}

// pcm レコードの sample アドレスからサンプル名を解決。未登録アドレスは実サンプルを
// 持たない GS シンセ記述子 (ゼロ長 WaveData) なので、無音ではなく種別別の合成リード波形
// (gs_synth_saw/tri/pulse) に写す (音程・リズム・エンベロープは正しく鳴らし音色のみ近似)。
let pcmAddrToName: Map<string, string>;
let sampleLoopedByName: Map<string, boolean>;
function pcmSampleName(addr: string): string {
  const key = addr.toLowerCase();
  const known = pcmAddrToName.get(key);
  if (known !== undefined) return known;
  const family = SYNTH_DESCRIPTORS[key] ?? 0; // 未知アドレスは安全側で鋸波。
  return SYNTH_NAME[family];
}
// gs_synth_* (合成リード) 系サンプル名か。
function isSynthName(name: string): boolean {
  return name === SYNTH_NAME[0] || name === SYNTH_NAME[1] || name === SYNTH_NAME[2];
}
function sampleLooped(name: string): boolean {
  return sampleLoopedByName.get(name) ?? false;
}

// ── SoundFont 全体を組み立てる ──────────────────────────────────────────
interface BuildResult {
  sf2: Buffer;
  stats: {
    pcmSamples: number; psgSamples: number; totalSamples: number;
    instruments: number; presets: number; synthLeadRecords: number; rhythmZones: number;
  };
}

function buildSoundFont(): BuildResult {
  const waveIndex = readJson(join(WAVES_DIR, "index.json"));
  const onshoku = readJson(join(ENGINE_DIR, "onshoku.json"));
  const hakei = readJson(join(ENGINE_DIR, "hakei.json"));

  // 1) PCM サンプル (32 波形)。
  const samples: SampleDef[] = [];
  pcmAddrToName = new Map();
  sampleLoopedByName = new Map();
  for (const wave of waveIndex.waves) {
    const rate = Math.round(Number(wave.frequency) / 1024);
    const wav = readFileSync(join(WAVES_DIR, wave.source));
    const data = pcm16FromWav(wav, rate);
    const looped = wave.loop_start !== null;
    const def: SampleDef = {
      name: wave.name, data, rate, rootKey: 60,
      loopStart: looped ? wave.loop_start : 0,
      loopEnd: data.length, looped,
    };
    samples.push(def);
    pcmAddrToName.set(String(wave.address).toLowerCase(), wave.name);
    sampleLoopedByName.set(wave.name, looped);
  }
  const pcmCount = samples.length;

  // 実 PCM サンプルの RMS 中央値。gs_synth (同じ DirectSound ミキサ経路) の音量を
  // これに合わせ、シンセ音色が実 PCM 音色と釣り合うようにする。
  const rmsOf = (d: Int16Array): number => {
    let s = 0;
    for (let i = 0; i < d.length; i++) s += d[i] * d[i];
    return Math.sqrt(s / Math.max(1, d.length));
  };
  const pcmRms = samples.map((s) => rmsOf(s.data)).sort((a, b) => a - b);
  const medianPcmRms = pcmRms[Math.floor(pcmRms.length / 2)] ?? 8000;

  // 2) 参照される PSG サンプルだけを合成する。
  const usedSquares = new Set<number>();
  const usedNoise = new Set<number>();
  const usedCgb = new Set<number>();
  for (const bank of onshoku.banks) {
    for (const r of bank.records) {
      if (r.kind === "pulse_1" || r.kind === "pulse_2") usedSquares.add(r.generator);
      else if (r.kind === "noise") usedNoise.add(r.generator);
      else if (r.kind === "wave") usedCgb.add(Number(r.waveform.slice(5)));
    }
  }
  for (const duty of [...usedSquares].sort()) {
    const s = squareSample(duty);
    samples.push(s); sampleLoopedByName.set(s.name, true);
  }
  for (const mode of [...usedNoise].sort()) {
    const s = noiseSample(mode);
    samples.push(s); sampleLoopedByName.set(s.name, true);
  }
  for (const wi of [...usedCgb].sort((a, b) => a - b)) {
    const s = cgbWaveSample(wi, hakei.waveforms[wi].samples);
    samples.push(s); sampleLoopedByName.set(s.name, true);
  }
  const silence = silenceSample();
  samples.push(silence); sampleLoopedByName.set("silence", true);
  // GS シンセ 3 系統 (鋸/三角/矩形) を種別ごとに合成する。
  for (const family of [0, 1, 2] as SynthFamily[]) {
    const s = synthLeadSample(family, medianPcmRms);
    samples.push(s); sampleLoopedByName.set(s.name, true);
  }
  const psgCount = samples.length - pcmCount - 4; // silence と gs_synth 3 系統を除く

  const sampleIndexByName = new Map<string, number>();
  samples.forEach((s, i) => sampleIndexByName.set(s.name, i));
  const sampleIndexOf = (name: string): number => {
    const i = sampleIndexByName.get(name);
    if (i === undefined) throw new Error(`サンプルが見つからない: ${name}`);
    return i;
  };

  // 3) インストゥルメント: プリセットから到達可能なレコードを対象にする。
  //    bank_0 は preset 0..127、bank_1 は preset 0..80。
  const instruments: Instrument[] = [];
  const instrumentIndexByKey = new Map<string, number>();
  let synthLeadRecords = 0;
  let rhythmZones = 0;

  const bankRecords = [onshoku.banks[0].records, onshoku.banks[1].records];

  function ensureInstrument(bankNo: number, recordIndex: number): number {
    const key = `${bankNo}_${recordIndex}`;
    const cached = instrumentIndexByKey.get(key);
    if (cached !== undefined) return cached;
    const record = bankRecords[bankNo][recordIndex];
    const name = `b${bankNo}_${recordIndex.toString().padStart(3, "0")}_${record.kind}`;
    const zones: Zone[] = [];

    if (record.kind === "rhythm") {
      // ドラムキット: key_map=null なので MIDI ノート K がサブテーブルの K 番目を選ぶ。
      // サブテーブルは bank_0 のレコード 92 起点。ノート K → bank_0 レコード (92+K)。
      const subMatch = /^bank_0_(\d+)$/.exec(record.tones);
      const subBase = subMatch ? Number(subMatch[1]) : 92;
      for (let note = 0; note < 128; note++) {
        const target = subBase + note;
        if (target >= BANK_0_COUNT) break;
        const sub = bankRecords[0][target];
        if (sub.kind === "rhythm") continue; // 入れ子は展開しない。
        // ドラムのサブ音がシンセ記述子 (gs_synth) を指す場合は従来どおり鳴らさない
        // (打楽器ゾーンを合成リードで埋めない)。
        if (sub.kind === "pcm" && isSynthName(pcmSampleName(sub.sample))) continue;
        zones.push({ gens: toneZoneGens(sub, sampleIndexOf, [note, note]) });
        rhythmZones++;
      }
      if (zones.length === 0) {
        zones.push({ gens: silentZone(sampleIndexOf) });
      }
    } else {
      // 旋律レコード。GS シンセ記述子は gs_synth 波形で鳴らす (無音にしない)。
      if (record.kind === "pcm" && isSynthName(pcmSampleName(record.sample))) synthLeadRecords++;
      zones.push({ gens: toneZoneGens(record, sampleIndexOf, null) });
    }

    const idx = instruments.length;
    instruments.push({ name, zones });
    instrumentIndexByKey.set(key, idx);
    return idx;
  }

  const presets: Preset[] = [];
  for (let n = 0; n < Math.min(128, BANK_0_COUNT); n++) {
    presets.push({
      name: `GS b0 ${n}`, bank: 0, preset: n,
      instrumentIndex: ensureInstrument(0, n),
    });
  }
  for (let n = 0; n < Math.min(128, BANK_1_COUNT); n++) {
    presets.push({
      name: `GS b1 ${n}`, bank: 1, preset: n,
      instrumentIndex: ensureInstrument(1, n),
    });
  }

  // 4) チャンク組み立て。
  const { smpl, shdr } = buildSampleData(samples);
  const { inst, ibag, igen } = buildInstChunks(instruments);
  const { phdr, pbag, pgen } = buildPresetChunks(presets);
  const pmod = Buffer.alloc(10); // pmod 終端 (モジュレータ未使用)。
  const imod = Buffer.alloc(10); // imod 終端。

  const info = listChunk("INFO", [
    (() => { const b = Buffer.alloc(4); b.writeUInt16LE(2, 0); b.writeUInt16LE(1, 2); return riffChunk("ifil", b); })(),
    riffChunk("isng", zstring("Golden Sun mp2k engine", 24)),
    riffChunk("INAM", zstring("Golden Sun (recovered)", 24)),
    riffChunk("ISFT", zstring("build_soundfont.ts", 20)),
  ]);
  const sdta = listChunk("sdta", [riffChunk("smpl", smpl)]);
  const pdta = listChunk("pdta", [
    riffChunk("phdr", phdr), riffChunk("pbag", pbag), riffChunk("pmod", pmod), riffChunk("pgen", pgen),
    riffChunk("inst", inst), riffChunk("ibag", ibag), riffChunk("imod", imod), riffChunk("igen", igen),
    riffChunk("shdr", shdr),
  ]);

  const body = Buffer.concat([Buffer.from("sfbk", "ascii"), info, sdta, pdta]);
  const head = Buffer.alloc(8);
  head.write("RIFF", 0, "ascii");
  head.writeUInt32LE(body.length, 4);
  const sf2 = Buffer.concat([head, body]);

  return {
    sf2,
    stats: {
      pcmSamples: pcmCount, psgSamples: psgCount, totalSamples: samples.length,
      instruments: instruments.length, presets: presets.length, synthLeadRecords, rhythmZones,
    },
  };
}

function silentZone(sampleIndexOf: (name: string) => number): Array<[number, number | [number, number]]> {
  return [
    [GEN.initialAttenuation, 1440],
    [GEN.sampleModes, 0],
    [GEN.sampleID, sampleIndexOf("silence")],
  ];
}

// ── 構造自己検査 ────────────────────────────────────────────────────────
//   RIFF を再パースし、チャンクサイズ整合と全 sampleID / instrument インデックスの
//   妥当性、バッグインデックスの単調性・終端整合を確認する。
function selfCheck(sf2: Buffer): void {
  if (sf2.toString("ascii", 0, 4) !== "RIFF") throw new Error("RIFF 署名がない");
  if (sf2.readUInt32LE(4) !== sf2.length - 8) throw new Error("RIFF サイズ不整合");
  if (sf2.toString("ascii", 8, 12) !== "sfbk") throw new Error("sfbk 形式でない");

  // トップレベル LIST を走査。
  const lists = new Map<string, { start: number; end: number }>();
  let p = 12;
  while (p < sf2.length) {
    const id = sf2.toString("ascii", p, p + 4);
    const size = sf2.readUInt32LE(p + 4);
    if (id !== "LIST") throw new Error(`予期しないトップチャンク: ${id}`);
    const listType = sf2.toString("ascii", p + 8, p + 12);
    lists.set(listType, { start: p + 12, end: p + 8 + size });
    p += 8 + size + (size % 2);
  }
  for (const need of ["INFO", "sdta", "pdta"]) {
    if (!lists.has(need)) throw new Error(`${need} LIST が無い`);
  }

  // pdta のサブチャンクを収集。
  const sub = new Map<string, Buffer>();
  const pdta = lists.get("pdta")!;
  let q = pdta.start;
  while (q < pdta.end) {
    const id = sf2.toString("ascii", q, q + 4);
    const size = sf2.readUInt32LE(q + 4);
    sub.set(id, sf2.subarray(q + 8, q + 8 + size));
    q += 8 + size + (size % 2);
  }
  for (const need of ["phdr", "pbag", "pgen", "inst", "ibag", "igen", "shdr"]) {
    if (!sub.has(need)) throw new Error(`pdta に ${need} が無い`);
  }

  const shdrCount = sub.get("shdr")!.length / 46 - 1; // 終端を除くサンプル数。
  const instCount = sub.get("inst")!.length / 22 - 1;
  if (shdrCount < 1 || instCount < 1) throw new Error("サンプル/インストゥルメントが空");

  // レコード数の整数性。
  const checks: Array<[string, number]> = [
    ["phdr", 38], ["pbag", 4], ["pgen", 4], ["inst", 22], ["ibag", 4], ["igen", 4], ["shdr", 46],
  ];
  for (const [id, unit] of checks) {
    if (sub.get(id)!.length % unit !== 0) throw new Error(`${id} のサイズが ${unit} の倍数でない`);
  }

  // ibag.genNdx / inst.bagNdx の単調性と終端整合。
  const inst = sub.get("inst")!;
  const ibag = sub.get("ibag")!;
  const igen = sub.get("igen")!;
  const igenCount = igen.length / 4;
  let prevBag = -1;
  for (let i = 0; i <= instCount; i++) {
    const bagNdx = inst.readUInt16LE(i * 22 + 20);
    if (bagNdx < prevBag) throw new Error("inst.bagNdx が単調でない");
    prevBag = bagNdx;
  }
  const ibagCount = ibag.length / 4 - 1;
  if (inst.readUInt16LE(instCount * 22 + 20) !== ibagCount) throw new Error("inst 終端が ibag 総数と不一致");

  // 全 igen の sampleID (gen 53) が shdr 範囲内か検査。
  let sampleIdChecks = 0;
  for (let g = 0; g < igenCount - 1; g++) {
    const oper = igen.readUInt16LE(g * 4);
    if (oper === GEN.sampleID) {
      const id = igen.readUInt16LE(g * 4 + 2);
      if (id < 0 || id >= shdrCount) throw new Error(`igen sampleID ${id} が範囲外 (0..${shdrCount - 1})`);
      sampleIdChecks++;
    }
  }

  // 全 pgen の instrument (gen 41) が inst 範囲内か検査。
  const pgen = sub.get("pgen")!;
  const pgenCount = pgen.length / 4;
  let instRefChecks = 0;
  for (let g = 0; g < pgenCount - 1; g++) {
    const oper = pgen.readUInt16LE(g * 4);
    if (oper === GEN.instrument) {
      const id = pgen.readUInt16LE(g * 4 + 2);
      if (id < 0 || id >= instCount) throw new Error(`pgen instrument ${id} が範囲外 (0..${instCount - 1})`);
      instRefChecks++;
    }
  }

  // shdr の各ループ/範囲が smpl プール内か検査。
  const smplBytes = (() => {
    const sdta = lists.get("sdta")!;
    let r = sdta.start;
    while (r < sdta.end) {
      const id = sf2.toString("ascii", r, r + 4);
      const size = sf2.readUInt32LE(r + 4);
      if (id === "smpl") return size;
      r += 8 + size + (size % 2);
    }
    throw new Error("smpl チャンクが無い");
  })();
  const totalPoints = smplBytes / 2;
  const shdr = sub.get("shdr")!;
  for (let s = 0; s < shdrCount; s++) {
    const start = shdr.readUInt32LE(s * 46 + 20);
    const end = shdr.readUInt32LE(s * 46 + 24);
    const ls = shdr.readUInt32LE(s * 46 + 28);
    const le = shdr.readUInt32LE(s * 46 + 32);
    if (!(start <= end && end <= totalPoints)) throw new Error(`shdr ${s} の start/end が範囲外`);
    if (!(start <= ls && ls <= le && le <= end)) throw new Error(`shdr ${s} のループ点が範囲外`);
    if (shdr.readUInt32LE(s * 46 + 36) === 0) throw new Error(`shdr ${s} のサンプルレートが 0`);
  }

  console.log(`self-check=ok samples=${shdrCount} instruments=${instCount} presets=${sub.get("phdr")!.length / 38 - 1} sampleID_refs=${sampleIdChecks} instrument_refs=${instRefChecks}`);
}

// ── fluidsynth スモークテスト (存在すれば) ──────────────────────────────
function fluidsynthSmoke(sf2Path: string): void {
  let has = false;
  try { execFileSync("which", ["fluidsynth"], { stdio: "ignore" }); has = true; } catch { /* 無し */ }
  if (!has) {
    console.log("smoke=skipped (fluidsynth 未検出。構造自己検査で代替)");
    return;
  }
  const mids = existsSync(MIDI_DIR) ? readdirSync(MIDI_DIR).filter((f) => f.endsWith(".mid")).sort() : [];
  if (mids.length === 0) { console.log("smoke=skipped (MIDI 無し)"); return; }
  const midi = join(MIDI_DIR, mids[0]);
  const outWav = join(OUT_DIR, "golden-sun.smoke.wav");
  try {
    execFileSync("fluidsynth", ["-ni", "-F", outWav, "-r", "44100", sf2Path, midi], { stdio: "ignore" });
    const size = existsSync(outWav) ? readFileSync(outWav).length : 0;
    console.log(`smoke=ok rendered=${mids[0]} wav_bytes=${size}`);
  } catch (error) {
    console.log(`smoke=failed (${(error as Error).message})`);
  }
}

// ── 自己テスト (決定論性 + 変換関数) ────────────────────────────────────
function selfTest(): void {
  // dsEnvelope の代表値。
  const e = dsEnvelope([255, 0, 255, 165]);
  if (!(e.sustain === 0 && e.attack < -3000 && e.release > -3000)) {
    throw new Error(`dsEnvelope 自己テスト失敗: ${JSON.stringify(e)}`);
  }
  // dsPan。
  if (dsPan(0) !== null || dsPan(0xc0) !== 0 || dsPan(0xa0)! >= 0 || dsPan(0xe0)! <= 0) {
    throw new Error("dsPan 自己テスト失敗");
  }
  // 決定論性: 2 回生成してバイト一致。
  const a = buildSoundFont().sf2;
  const b = buildSoundFont().sf2;
  if (!a.equals(b)) throw new Error("生成が決定論的でない");
  selfCheck(a);
  console.log("self-test=ok deterministic=true");
}

function main(args: string[]): void {
  if (args.includes("--self-test")) { selfTest(); return; }
  const { sf2, stats } = buildSoundFont();
  selfCheck(sf2);
  mkdirSync(OUT_DIR, { recursive: true });
  writeFileSync(OUT_SF2, sf2);
  console.log(
    `wrote ${OUT_SF2} bytes=${sf2.length}\n` +
    `pcm_samples=${stats.pcmSamples} psg_samples=${stats.psgSamples} total_samples=${stats.totalSamples}\n` +
    `instruments=${stats.instruments} presets=${stats.presets} ` +
    `synth_lead_records=${stats.synthLeadRecords} rhythm_zones=${stats.rhythmZones}`,
  );
  fluidsynthSmoke(OUT_SF2);
}

if (import.meta.main) main(Bun.argv.slice(2));
