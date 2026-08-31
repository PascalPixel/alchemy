// Browser client for the Rust dashboard server.
function append(parent, child) {
  if (Array.isArray(child)) {
    for (const item of child) append(parent, item);
  } else if (child && typeof child === "object" && child.nodeType) {
    parent.appendChild(child);
  } else if (child !== undefined && child !== null && child !== false) {
    parent.appendChild(document.createTextNode(String(child)));
  }
}
function h(tag, attributes = {}, ...children) {
  const node = document.createElement(tag);
  for (const [key, value] of Object.entries(attributes)) {
    if (key === "className") node.className = String(value ?? "");
    else if (value !== undefined && value !== null && value !== false)
      node.setAttribute(key, value === true ? "" : String(value));
  }
  for (const child of children) append(node, child);
  return node;
}
function s(tag, attributes = {}, ...children) {
  const node = document.createElementNS("http://www.w3.org/2000/svg", tag);
  for (const [key, value] of Object.entries(attributes)) {
    if (value !== undefined && value !== null && value !== false)
      node.setAttribute(key, value === true ? "" : String(value));
  }
  for (const child of children) append(node, child);
  return node;
}
const root = h("div", { id: "root" }, h("div", { className: "loading", role: "status" }, "Reading the stones…"));
document.body.replaceChildren(root);
const tooltip = h("div", { className: "hover-tooltip", role: "tooltip", hidden: true });
document.body.append(tooltip);
let pageVersion = "";
let lastRevision = "";
let lastError = "";
function percent(value) { return `${Number(value ?? 0).toFixed(2)}%`; }

const music = {
  context: null, master: null, analyser: null, tracks: [], notes: [], duration: 0,
  index: 0, cursor: 0, position: 0, startedAt: 0, playing: false, timer: 0,
  voices: new Set(), ui: null, soundfont: null, soundfonts: new Map(), sampleBuffers: new Map(), generatorBuffers: new Map(), ready: false, volume: 1,
  repeatOne: localStorage.getItem("alchemy.music.repeatOne") === "true",
};
function midiVlq(view, state) {
  let value = 0;
  for (let i = 0; i < 4; i += 1) {
    const byte = view.getUint8(state.at++);
    value = (value << 7) | (byte & 0x7f);
    if (!(byte & 0x80)) return value;
  }
  throw new Error("Invalid MIDI variable-length number");
}
// SMSH advances tempo against the GBA display clock, not MIDI's nominal
// 120-BPM default. Proven by the native/meta pairs embedded in GS1 sources.
function smshTempo(value) {
  return value > 0 ? Math.round(120547500 / value) : 500000;
}
function parseMidi(buffer) {
  const view = new DataView(buffer);
  const text = (at, length) => String.fromCharCode(...new Uint8Array(buffer, at, length));
  if (text(0, 4) !== "MThd") throw new Error("Track is not a Standard MIDI file");
  const tracks = view.getUint16(10);
  const division = view.getUint16(12);
  if (division & 0x8000) throw new Error("SMPTE-timed MIDI is not supported");
  const events = [];
  let at = 8 + view.getUint32(4);
  for (let track = 0; track < tracks; track += 1) {
    if (text(at, 4) !== "MTrk") throw new Error("MIDI track header is missing");
    const end = at + 8 + view.getUint32(at + 4);
    const state = { at: at + 8 };
    let tick = 0;
    let running = 0;
    while (state.at < end) {
      tick += midiVlq(view, state);
      let status = view.getUint8(state.at);
      if (status & 0x80) {
        state.at += 1;
        if (status < 0xf0) running = status;
      } else {
        if (running < 0x80 || running >= 0xf0) throw new Error("MIDI running status has no channel event");
        status = running;
      }
      if (status === 0xff) {
        const type = view.getUint8(state.at++);
        const length = midiVlq(view, state);
        if (type === 0x51 && length === 3) {
          events.push({ tick, track, kind: "tempo", value: (view.getUint8(state.at) << 16) | (view.getUint8(state.at + 1) << 8) | view.getUint8(state.at + 2) });
        } else if (type === 0x06) {
          try {
            const marker = JSON.parse(new TextDecoder().decode(new Uint8Array(buffer, state.at, length)));
            if (Array.isArray(marker)) events.push({ tick, track, kind: "engine", marker });
          } catch {}
        }
        state.at += length;
      } else if (status === 0xf0 || status === 0xf7) {
        state.at += midiVlq(view, state);
      } else {
        const kind = status >> 4;
        const channel = status & 15;
        const a = view.getUint8(state.at++);
        const b = kind === 0xc || kind === 0xd ? 0 : view.getUint8(state.at++);
        if (kind === 0x9 && b) events.push({ tick, track, kind: "on", channel, note: a, velocity: b });
        else if (kind === 0x8 || (kind === 0x9 && !b)) events.push({ tick, track, kind: "off", channel, note: a });
        else if (kind === 0xc) events.push({ tick, track, kind: "program", channel, value: a });
        else if (kind === 0xb) events.push({ tick, track, kind: "control", channel, control: a, value: b });
        else if (kind === 0xe) events.push({ tick, track, kind: "bend", channel, value: ((b << 7) | a) - 8192 });
      }
    }
    at = end;
  }
  events.sort((a, b) => a.tick - b.tick || (a.kind === "tempo" ? -1 : 0));
  const programs = Array(16).fill(0);
  const volumes = Array(16).fill(100);
  const expressions = Array(16).fill(127);
  const pans = Array(16).fill(64);
  const bends = Array(16).fill(0);
  const keyShifts = Array(16).fill(0);
  const tunings = Array(16).fill(64);
  const active = new Map();
  const notes = [];
  let tick = 0;
  let seconds = 0;
  let tempo = 500000;
  for (const event of events) {
    seconds += (event.tick - tick) * tempo / division / 1000000;
    tick = event.tick;
    const voice = event.track;
    if (event.kind === "tempo") tempo = event.value;
    else if (event.kind === "engine") {
      const [command, value] = event.marker;
      if (command === "voice") programs[voice] = value;
      else if (command === "tempo") tempo = smshTempo(value);
      else if (command === "volume") volumes[voice] = value;
      else if (command === "pan") pans[voice] = value;
      else if (command === "pitch_bend") bends[voice] = value * 128;
      else if (command === "key_shift") keyShifts[voice] = value;
      else if (command === "tuning") tunings[voice] = value;
    }
    else if (event.kind === "program") programs[voice] = event.value;
    else if (event.kind === "control") {
      if (event.control === 7) volumes[voice] = event.value;
      else if (event.control === 10) pans[voice] = event.value;
      else if (event.control === 11) expressions[voice] = event.value;
    } else if (event.kind === "bend") bends[voice] = event.value;
    else if (event.kind === "on") {
      const key = `${voice}:${event.note}`;
      const queue = active.get(key) ?? [];
      queue.push({ start: seconds, note: event.note + (keyShifts[voice] ?? 0), velocity: event.velocity, channel: voice, program: programs[voice] ?? 0, volume: volumes[voice] ?? 100, expression: expressions[voice] ?? 127, pan: pans[voice] ?? 64, bend: (bends[voice] ?? 0) + ((tunings[voice] ?? 64) - 64) * 64 });
      active.set(key, queue);
    } else if (event.kind === "off") {
      const key = `${voice}:${event.note}`;
      const start = active.get(key)?.shift();
      if (start) notes.push({ ...start, end: Math.max(seconds, start.start + 0.03) });
    }
  }
  notes.sort((a, b) => a.start - b.start);
  return { notes, duration: Math.max(seconds, ...notes.map((note) => note.end), 0) };
}
function currentMusicPosition() {
  return music.playing && music.context ? Math.min(music.duration, music.context.currentTime - music.startedAt) : music.position;
}
function musicTime(value) {
  const seconds = Math.max(0, Math.floor(value || 0));
  return `${Math.floor(seconds / 60)}:${String(seconds % 60).padStart(2, "0")}`;
}
function stopVoices() {
  for (const voice of music.voices) { try { voice.stop(); } catch {} }
  music.voices.clear();
}
function updateMusicUi() {
  if (!music.ui) return;
  const position = currentMusicPosition();
  music.ui.play.replaceChildren(musicIcon(music.playing ? "pause" : "play"));
  music.ui.play.setAttribute("aria-label", music.playing ? "Pause" : "Play track (A)");
  if (music.playing && position >= music.duration) {
    pauseMusic(true);
    if (music.repeatOne) void playMusic();
    return;
  }
  music.ui.elapsed.textContent = musicTime(position);
  music.ui.duration.textContent = musicTime(music.duration);
  music.ui.scrubber.max = String(Math.max(music.duration, 0.01));
  music.ui.scrubber.value = String(position);
  const progress = position / Math.max(music.duration, 0.01);
  music.ui.waveBars.forEach((bar, index) => bar.classList.toggle("passed", index / music.ui.waveBars.length <= progress));
}
function ensureAudio() {
  if (!music.context) {
    music.context = new AudioContext();
    music.master = music.context.createGain();
    music.master.gain.value = music.volume;
    music.master.connect(music.context.destination);
  }
  return music.context;
}
async function loadSoundfont(game) {
  if (music.soundfonts.has(game)) {
    music.soundfont = music.soundfonts.get(game);
    return music.soundfont;
  }
  const response = await fetch(game === "gs1" ? "/music/soundfont" : "/music/gs2/soundfont");
  if (!response.ok) throw new Error(`soundfont returned ${response.status}`);
  const soundfont = await response.json();
  const expectedVoices = game === "gs1" ? 144 : 145;
  if (soundfont.engine !== "golden-sun-rom-audio-bank" || soundfont.bank.length !== expectedVoices) throw new Error(`recovered ${game.toUpperCase()} music bank is incomplete`);
  soundfont.game = game;
  soundfont.sampleByAddress = new Map(soundfont.samples.map((sample) => [sample.address, sample]));
  soundfont.embeddedByAddress = new Map(soundfont.embedded_samples.map((sample) => [sample.address, sample]));
  for (const sample of soundfont.embedded_samples) {
    soundfont.sampleByAddress.set(sample.address, { ...sample, loop_start: sample.control & 0xc0000000 ? sample.loop_start : null, embedded: true });
  }
  soundfont.waveformByName = new Map(soundfont.waveforms.map((waveform) => [waveform.name, waveform]));
  music.soundfonts.set(game, soundfont);
  music.soundfont = soundfont;
  return soundfont;
}
function resolveTone(note) {
  let tone = music.soundfont.bank[note.program];
  let rhythm = false;
  if (tone?.kind === "rhythm") {
    const base = Number(tone.tones.slice(-3));
    tone = music.soundfont.bank[base + note.note];
    rhythm = true;
  }
  if (!tone) throw new Error(`program ${note.program} / key ${note.note} is absent from the recovered bank`);
  return { tone, rhythm };
}
async function loadSample(game, address) {
  const cacheKey = `${game}:${address}`;
  if (music.sampleBuffers.has(cacheKey)) return music.sampleBuffers.get(cacheKey);
  const sample = music.soundfont.sampleByAddress.get(address);
  if (!sample) throw new Error(`PCM sample ${address} is absent from the recovered wave catalog`);
  if (sample.embedded) {
    const rate = Math.max(3000, sample.frequency / 1024);
    const buffer = ensureAudio().createBuffer(1, sample.samples.length, rate);
    buffer.copyToChannel(Float32Array.from(sample.samples, (value) => (value > 127 ? value - 256 : value) / 128), 0);
    music.sampleBuffers.set(cacheKey, buffer);
    return buffer;
  }
  const response = await fetch(`/music/${game === "gs1" ? "" : "gs2/"}samples/${sample.source}`);
  if (!response.ok) throw new Error(`${sample.source} returned ${response.status}`);
  const buffer = await ensureAudio().decodeAudioData(await response.arrayBuffer());
  music.sampleBuffers.set(cacheKey, buffer);
  return buffer;
}
async function prepareTrackSamples(game) {
  await loadSoundfont(game);
  const addresses = new Set();
  for (const note of music.notes) {
    const { tone } = resolveTone(note);
    if (tone.kind === "pcm") addresses.add(tone.sample);
  }
  await Promise.all([...addresses].map((address) => loadSample(game, address)));
}
function generatorBuffer(tone) {
  const key = `${music.soundfont.game ?? "gs1"}:${tone.kind}:${tone.generator ?? tone.waveform}`;
  if (music.generatorBuffers.has(key)) return music.generatorBuffers.get(key);
  const context = ensureAudio();
  let samples;
  if (tone.kind === "wave") {
    const waveform = music.soundfont.waveformByName.get(tone.waveform);
    if (!waveform) throw new Error(`CGB waveform ${tone.waveform} is absent`);
    samples = Float32Array.from(waveform.samples, (sample) => (sample - 7.5) / 7.5);
  } else if (tone.kind === "noise") {
    samples = new Float32Array(32767);
    let state = 0x7fff;
    for (let i = 0; i < samples.length; i += 1) {
      const feedback = (state ^ (state >> 1)) & 1;
      state = (state >> 1) | (feedback << 14);
      samples[i] = state & 1 ? .7 : -.7;
    }
  } else {
    const duty = [.125, .25, .5, .75][tone.generator ?? 2];
    samples = Float32Array.from({ length: 256 }, (_, index) => index / 256 < duty ? .72 : -.72);
  }
  const buffer = context.createBuffer(1, samples.length, context.sampleRate);
  buffer.copyToChannel(samples, 0);
  music.generatorBuffers.set(key, buffer);
  return buffer;
}
function envelopeShape(tone) {
  const [attack, decay, sustain, release] = tone.envelope;
  if (tone.kind === "pcm") {
    return {
      attack: attack >= 250 ? .005 : Math.max(.01, (256 - attack) / 90),
      decay: decay === 0 ? .01 : Math.max(.02, (256 - decay) / 90),
      sustain: sustain / 255,
      release: release === 0 ? .03 : Math.max(.04, (256 - release) / 90),
    };
  }
  return { attack: .004, decay: .02, sustain: Math.min(1, sustain / 15), release: release ? Math.max(.03, release / 30) : .03 };
}
function scheduleTone(note, position) {
  const context = music.context;
  const { tone, rhythm } = resolveTone(note);
  const source = context.createBufferSource();
  let baseRate = 1;
  const playedKey = tone.fixed_pitch || rhythm ? tone.key : note.note + note.bend / 8192 * 2;
  if (tone.kind === "pcm") {
    const track = music.tracks[music.index];
    const sample = music.soundfont.sampleByAddress.get(tone.sample);
    source.buffer = music.sampleBuffers.get(`${track.game}:${tone.sample}`);
    if (!source.buffer || !sample) throw new Error(`PCM sample ${tone.sample} was not prepared`);
    if (sample.loop_start !== null) {
      source.loop = true;
      source.loopStart = sample.loop_start / Math.max(3000, sample.frequency / 1024);
      source.loopEnd = source.buffer.duration;
    }
  } else {
    source.buffer = generatorBuffer(tone);
    source.loop = true;
    baseRate = 440 * 2 ** ((tone.key - 69) / 12) / (context.sampleRate / source.buffer.length);
  }
  source.playbackRate.value = baseRate * 2 ** ((playedKey - tone.key) / 12);
  const start = music.startedAt + Math.max(note.start, position);
  const noteEnd = music.startedAt + note.end;
  const envelope = envelopeShape(tone);
  const stop = noteEnd + envelope.release;
  const gain = context.createGain();
  const velocity = note.velocity / 127;
  const channelLevel = note.volume / 127 * note.expression / 127;
  const level = Math.max(.0002, velocity * channelLevel * .22);
  const sustain = Math.max(.0001, level * envelope.sustain);
  const attackEnd = Math.min(noteEnd, start + envelope.attack);
  const decayEnd = Math.min(noteEnd, attackEnd + envelope.decay);
  gain.gain.setValueAtTime(.0001, start);
  gain.gain.exponentialRampToValueAtTime(level, attackEnd);
  gain.gain.exponentialRampToValueAtTime(sustain, decayEnd);
  gain.gain.setValueAtTime(sustain, noteEnd);
  gain.gain.exponentialRampToValueAtTime(.0001, stop);
  let output = gain;
  if (typeof context.createStereoPanner === "function") {
    const panner = context.createStereoPanner();
    const rhythmPan = rhythm ? (tone.pan_sweep > 127 ? tone.pan_sweep - 256 : tone.pan_sweep) / 128 : 0;
    panner.pan.value = Math.max(-1, Math.min(1, (note.pan - 64) / 64 + rhythmPan));
    gain.connect(panner);
    output = panner;
  }
  source.connect(gain);
  output.connect(music.master);
  source.start(start);
  source.stop(stop + .01);
  music.voices.add(source);
  source.addEventListener("ended", () => music.voices.delete(source));
}
function scheduleMusic() {
  if (!music.playing || !music.context) return;
  const position = currentMusicPosition();
  const horizon = position + 0.45;
  while (music.cursor < music.notes.length && music.notes[music.cursor].start < horizon) {
    const note = music.notes[music.cursor++];
    if (note.end <= position) continue;
    scheduleTone(note, position);
  }
  updateMusicUi();
}
function cursorAt(position) {
  let low = 0, high = music.notes.length;
  while (low < high) { const middle = (low + high) >> 1; if (music.notes[middle].start < position) low = middle + 1; else high = middle; }
  return low;
}
function drawMusicWave() {
  const count = 96;
  const energy = Array(count).fill(0.08);
  for (const note of music.notes) {
    const index = Math.min(count - 1, Math.floor(note.start / Math.max(music.duration, 0.01) * count));
    energy[index] = Math.max(energy[index], note.velocity / 127);
  }
  music.ui.waveBars = energy.map((level) => h("span", { className: "music-wave-bar", style: `--bar:${Math.round(14 + level * 82)}%` }));
  music.ui.wave.replaceChildren(...music.ui.waveBars, music.ui.scrubber);
}
function seekMusic(position) {
  const next = Math.max(0, Math.min(music.duration, Number(position) || 0));
  const wasPlaying = music.playing;
  clearInterval(music.timer);
  stopVoices();
  music.position = next;
  music.cursor = cursorAt(next);
  if (wasPlaying && music.context) {
    music.startedAt = music.context.currentTime - next;
    music.timer = setInterval(scheduleMusic, 100);
    scheduleMusic();
  } else {
    updateMusicUi();
  }
}
async function playMusic() {
  if (!music.notes.length || !music.ready) return;
  ensureAudio();
  await music.context.resume();
  music.startedAt = music.context.currentTime - music.position;
  music.cursor = cursorAt(music.position);
  music.playing = true;
  clearInterval(music.timer);
  music.timer = setInterval(scheduleMusic, 100);
  scheduleMusic();
}
function pauseMusic(rewind = false) {
  music.position = rewind ? 0 : currentMusicPosition();
  music.playing = false;
  clearInterval(music.timer);
  stopVoices();
  updateMusicUi();
}
async function loadMusic(index, autoplay = false) {
  pauseMusic(true);
  music.ready = false;
  music.index = (index + music.tracks.length) % music.tracks.length;
  const track = music.tracks[music.index];
  music.ui.title.textContent = track.title;
  music.ui.source.textContent = track.source;
  music.ui.rows.forEach((row, rowIndex) => row.setAttribute("aria-current", String(rowIndex === music.index)));
  music.ui.rows[music.index]?.scrollIntoView({ block: "nearest" });
  music.ui.card.setAttribute("aria-busy", "true");
  music.ui.play.setAttribute("aria-disabled", "true");
  music.notes = [];
  music.duration = 0;
  drawMusicWave();
  updateMusicUi();
  try {
    if (!track.available || !track.file) {
      music.notes = [];
      music.duration = 0;
      music.position = 0;
      music.ui.source.textContent = `${track.source} · not yet recovered (${track.status})`;
      updateMusicUi();
      return;
    }
    await loadSoundfont(track.game);
    const response = await fetch(track.path);
    if (!response.ok) throw new Error(`track returned ${response.status}`);
    const parsed = parseMidi(await response.arrayBuffer());
    music.notes = parsed.notes;
    music.duration = parsed.duration;
    music.position = 0;
    drawMusicWave();
    music.ui.source.textContent = `${track.source} · loading recovered instrument bank`;
    await prepareTrackSamples(track.game);
    music.ready = true;
    music.ui.source.textContent = `${track.source} · ${track.gameTitle} ROM tone bank`;
    updateMusicUi();
    if (autoplay) await playMusic();
  } catch (error) {
    music.ui.source.textContent = `Could not read track · ${error instanceof Error ? error.message : error}`;
  } finally {
    music.ui.card.setAttribute("aria-busy", "false");
    music.ui.play.setAttribute("aria-disabled", String(!music.ready));
  }
}
function musicIcon(kind) {
  const svg = s("svg", { viewBox: "0 0 24 24", "aria-hidden": "true" });
  const paths = {
    previous: "M5 5h2v14H5zM19 5v14L8 12z",
    play: "M7 4v16l13-8z",
    pause: "M6 4h4v16H6zM14 4h4v16h-4z",
    next: "M17 5h2v14h-2zM5 5v14l11-7z",
    repeat: "M7 7h9l-2.5-2.5L15 3l5 5-5 5-1.5-1.5L16 9H7a3 3 0 00-3 3H2a5 5 0 015-5zm10 8H8l2.5 2.5L9 19l-5-5 5-5 1.5 1.5L8 13h9a3 3 0 003-3h2a5 5 0 01-5 5z",
  };
  svg.append(s("path", { d: paths[kind] }));
  return svg;
}
function musicPlayer() {
  if (music.ui?.card) return music.ui.card;
  pauseMusic(true);
  const count = h("span", {}, "Reading tracks…");
  const list = h("div", { className: "music-list", role: "listbox", "aria-label": "Golden Sun music" });
  const title = h("div", { className: "music-now-title" }, "Reading recovered sequences…");
  const source = h("div", { className: "music-now-source" }, "Golden Sun sound archive");
  const control = (label, icon, primary = false) => h("button", {
    className: `music-control${primary ? " music-control-primary" : ""}`,
    type: "button", "aria-label": label,
  }, musicIcon(icon));
  const previous = control("Previous track", "previous");
  const play = control("Play track (A)", "play", true);
  const next = control("Next track", "next");
  const repeat = control("Repeat this track", "repeat");
  repeat.setAttribute("aria-pressed", String(music.repeatOne));
  const elapsed = h("span", {}, "0:00");
  const duration = h("span", {}, "0:00");
  const scrubber = h("input", { className: "music-range", type: "range", min: 0, max: 0.01, step: 0.01, value: 0, "aria-label": "Track position" });
  const wave = h("div", { className: "music-wave" }, scrubber);
  const card = h("section", { className: "music-player", "aria-label": "Music", "aria-busy": "true" },
    h("header", { className: "music-titlebar" }, h("span", {}, "Music"), count),
    list,
    h("footer", { className: "music-chin" },
      h("div", { className: "music-now" }, title, source),
      h("div", { className: "music-controls" }, previous, play, next, repeat),
      h("div", { className: "music-scrubber-shell" }, elapsed, wave, duration),
    ),
  );
  music.ui = { card, count, list, rows: [], title, source, play, previous, next, repeat, elapsed, duration, scrubber, wave, waveBars: [] };
  play.addEventListener("click", () => music.playing ? pauseMusic() : playMusic());
  previous.addEventListener("click", () => loadMusic(music.index - 1, music.playing));
  next.addEventListener("click", () => loadMusic(music.index + 1, music.playing));
  repeat.addEventListener("click", () => {
    music.repeatOne = !music.repeatOne;
    localStorage.setItem("alchemy.music.repeatOne", String(music.repeatOne));
    repeat.setAttribute("aria-pressed", String(music.repeatOne));
  });
  scrubber.addEventListener("input", () => seekMusic(scrubber.value));
  fetch("/music/catalog").then((response) => {
    if (!response.ok) throw new Error(`catalog returned ${response.status}`);
    return response.json();
  }).then((tracks) => {
    music.tracks = tracks;
    count.textContent = `${tracks.length} tracks`;
    music.ui.rows = tracks.map((track, index) => {
      const row = h("button", {
        className: "music-row", type: "button", role: "option", "aria-current": "false",
        "aria-label": `${track.gameTitle}, ${track.title}, ROM sequence ${track.soundId}`,
      },
      h("span", { className: "music-row-request" }, track.game.toUpperCase()),
      h("span", { className: "music-row-title" }, track.title),
      h("span", { className: "music-row-source" }, track.gameTitle),
      );
      row.addEventListener("click", () => {
        if (index === music.index && music.ready) music.playing ? pauseMusic() : playMusic();
        else loadMusic(index, true);
      });
      return row;
    });
    list.replaceChildren(...music.ui.rows);
    return loadMusic(0);
  }).catch((error) => {
    source.textContent = `Music catalog unavailable · ${error.message}`;
    card.setAttribute("aria-busy", "false");
  });
  return card;
}

async function requestSnapshot() {
  const response = await fetch("/snapshot", { cache: "no-store" });
  if (!response.ok) throw new Error(`/snapshot returned ${response.status}`);
  return response.json();
}
function hideTooltip() { tooltip.hidden = true; }
function showTooltip(event) {
  const target = event.target instanceof Element ? event.target.closest("g[aria-label]") : null;
  const label = target?.getAttribute("aria-label")?.trim();
  if (!label) { hideTooltip(); return; }
  tooltip.textContent = label;
  tooltip.hidden = false;
  const gap = 14;
  const edge = 8;
  const bounds = tooltip.getBoundingClientRect();
  const left = Math.min(event.clientX + gap, window.innerWidth - bounds.width - edge);
  const below = event.clientY + gap;
  const top = below + bounds.height <= window.innerHeight - edge ? below : event.clientY - bounds.height - gap;
  tooltip.style.left = `${Math.max(edge, left)}px`;
  tooltip.style.top = `${Math.max(edge, top)}px`;
}
async function loadTree(section, tree, title, revision) {
  const response = await fetch(`/svg/${tree}?v=${encodeURIComponent(revision)}`);
  if (!response.ok) throw new Error(`/svg/${tree} returned ${response.status}`);
  const parsed = new DOMParser().parseFromString(await response.text(), "image/svg+xml");
  const svg = parsed.documentElement;
  if (svg.localName !== "svg") throw new Error(`/svg/${tree} did not return an SVG`);
  svg.classList.add("tree-image");
  svg.setAttribute("aria-label", `${title} coverage graph`);
  section.querySelector(".chart")?.replaceChildren(svg);
}
function panel(tree, title, revision) {
  const chart = h("div", { className: "chart" }, h("div", { className: "chart-loading" }, "Reading…"));
  const section = h("section", { className: `panel p-${tree}` },
    chart,
  );
  loadTree(section, tree, title, revision).catch((error) => showError(error instanceof Error ? error.message : String(error)));
  return section;
}
function render(snapshot) {
  hideTooltip();
  const summary = snapshot.summary;
  const trees = Object.entries(snapshot.trees).map(([tree, title]) => panel(tree, title, snapshot.revision));
  root.replaceChildren(h("main", { className: "cards" }, ...trees, musicPlayer()));
  lastRevision = snapshot.revision;
  lastError = "";
  document.title = `Alchemy — 12 targets · ${percent(summary.donePercent)} GS1 EN done`;
}
function showError(message) {
  if (message === lastError) return;
  root.querySelector(".error")?.remove();
  root.prepend(h("div", { className: "error", role: "alert" }, `Dashboard update failed: ${message}`));
  lastError = message;
}
function clearError() { root.querySelector(".error")?.remove(); lastError = ""; }
function accept(snapshot) {
  if (pageVersion !== "" && snapshot.page !== pageVersion) { location.reload(); return; }
  pageVersion = snapshot.page;
  root.setAttribute("aria-busy", String(snapshot.scanning));
  if (snapshot.error !== undefined) showError(snapshot.error);
  else {
    clearError();
    if (snapshot.revision !== lastRevision) render(snapshot);
  }
}
async function refresh() {
  try { accept(await requestSnapshot()); }
  catch (error) { showError(error instanceof Error ? error.message : String(error)); }
}
await refresh();
const events = new EventSource("/events");
events.addEventListener("update", (event) => {
  try { accept(JSON.parse(event.data)); }
  catch (error) { showError(error instanceof Error ? error.message : String(error)); }
});
root.addEventListener("pointermove", showTooltip);
root.addEventListener("pointerleave", hideTooltip);
window.addEventListener("scroll", hideTooltip, { capture: true, passive: true });
