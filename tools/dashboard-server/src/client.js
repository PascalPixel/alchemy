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
const root = h("div", { id: "root" }, h("div", { className: "loading", role: "status" }, "Reading the stones…"));
document.body.replaceChildren(root);
const tooltip = h("div", { className: "hover-tooltip", role: "tooltip", hidden: true });
document.body.append(tooltip);
let pageVersion = "";
let lastRevision = "";
let lastError = "";
const number = new Intl.NumberFormat("en");
function bytes(value) { return number.format(Math.round(value ?? 0)); }
function percent(value) { return `${Number(value ?? 0).toFixed(2)}%`; }
function upper(value) { return String(value ?? "").toUpperCase(); }

const music = {
  context: null, master: null, analyser: null, tracks: [], notes: [], duration: 0,
  index: 0, cursor: 0, position: 0, startedAt: 0, playing: false, timer: 0,
  voices: new Set(), ui: null,
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
      if (status & 0x80) { state.at += 1; running = status; }
      else status = running;
      if (status === 0xff) {
        const type = view.getUint8(state.at++);
        const length = midiVlq(view, state);
        if (type === 0x51 && length === 3) {
          events.push({ tick, kind: "tempo", value: (view.getUint8(state.at) << 16) | (view.getUint8(state.at + 1) << 8) | view.getUint8(state.at + 2) });
        }
        state.at += length;
      } else if (status === 0xf0 || status === 0xf7) {
        state.at += midiVlq(view, state);
      } else {
        const kind = status >> 4;
        const channel = status & 15;
        const a = view.getUint8(state.at++);
        const b = kind === 0xc || kind === 0xd ? 0 : view.getUint8(state.at++);
        if (kind === 0x9 && b) events.push({ tick, kind: "on", channel, note: a, velocity: b });
        else if (kind === 0x8 || (kind === 0x9 && !b)) events.push({ tick, kind: "off", channel, note: a });
        else if (kind === 0xc) events.push({ tick, kind: "program", channel, value: a });
      }
    }
    at = end;
  }
  events.sort((a, b) => a.tick - b.tick || (a.kind === "tempo" ? -1 : 0));
  const programs = Array(16).fill(0);
  const active = new Map();
  const notes = [];
  let tick = 0;
  let seconds = 0;
  let tempo = 500000;
  for (const event of events) {
    seconds += (event.tick - tick) * tempo / division / 1000000;
    tick = event.tick;
    if (event.kind === "tempo") tempo = event.value;
    else if (event.kind === "program") programs[event.channel] = event.value;
    else if (event.kind === "on") {
      const key = `${event.channel}:${event.note}`;
      const queue = active.get(key) ?? [];
      queue.push({ start: seconds, note: event.note, velocity: event.velocity, channel: event.channel, program: programs[event.channel] });
      active.set(key, queue);
    } else if (event.kind === "off") {
      const key = `${event.channel}:${event.note}`;
      const start = active.get(key)?.shift();
      if (start) notes.push({ ...start, end: Math.max(seconds, start.start + 0.03) });
    }
  }
  notes.sort((a, b) => a.start - b.start);
  return { notes, duration: Math.max(seconds, ...notes.map((note) => note.end), 0) };
}
function musicTime(value) {
  const seconds = Math.max(0, Math.floor(value || 0));
  return `${Math.floor(seconds / 60)}:${String(seconds % 60).padStart(2, "0")}`;
}
function currentMusicPosition() {
  return music.playing && music.context ? Math.min(music.duration, music.context.currentTime - music.startedAt) : music.position;
}
function stopVoices() {
  for (const voice of music.voices) { try { voice.stop(); } catch {} }
  music.voices.clear();
}
function updateMusicUi() {
  if (!music.ui) return;
  const position = currentMusicPosition();
  music.ui.seek.value = String(position);
  music.ui.elapsed.textContent = musicTime(position);
  music.ui.duration.textContent = musicTime(music.duration);
  music.ui.play.textContent = music.playing ? "Ⅱ" : "▶";
  music.ui.play.setAttribute("aria-label", music.playing ? "Pause" : "Play");
  const canvas = music.ui.canvas;
  const scale = window.devicePixelRatio || 1;
  const width = Math.max(1, canvas.clientWidth);
  const height = Math.max(1, canvas.clientHeight);
  if (canvas.width !== width * scale || canvas.height !== height * scale) { canvas.width = width * scale; canvas.height = height * scale; }
  const context = canvas.getContext("2d");
  context.setTransform(scale, 0, 0, scale, 0, 0);
  context.clearRect(0, 0, width, height);
  context.fillStyle = "#343238";
  const bins = Math.max(24, Math.floor(width / 5));
  const energy = Array(bins).fill(0);
  for (const note of music.notes) {
    const bin = Math.min(bins - 1, Math.floor(note.start / Math.max(music.duration, 1) * bins));
    energy[bin] = Math.max(energy[bin], note.velocity / 127);
  }
  for (let i = 0; i < bins; i += 1) {
    const bar = 3 + energy[i] * (height - 7);
    context.fillStyle = i / bins <= position / Math.max(music.duration, 1) ? "#f4c95d" : "#4a4652";
    context.fillRect(i * width / bins, (height - bar) / 2, Math.max(1, width / bins - 2), bar);
  }
  if (music.playing && position >= music.duration) pauseMusic(true);
}
function voiceType(note) {
  if (note.channel === 9) return "square";
  return ["triangle", "square", "sawtooth", "sine"][Math.floor(note.program / 16) % 4];
}
function scheduleMusic() {
  if (!music.playing || !music.context) return;
  const position = currentMusicPosition();
  const horizon = position + 0.45;
  while (music.cursor < music.notes.length && music.notes[music.cursor].start < horizon) {
    const note = music.notes[music.cursor++];
    if (note.end <= position) continue;
    const start = music.startedAt + Math.max(note.start, position);
    const end = music.startedAt + note.end;
    const oscillator = music.context.createOscillator();
    const gain = music.context.createGain();
    oscillator.type = voiceType(note);
    oscillator.frequency.value = 440 * 2 ** ((note.note - 69) / 12);
    const level = (note.velocity / 127) * (note.channel === 9 ? 0.025 : 0.045);
    gain.gain.setValueAtTime(0.0001, start);
    gain.gain.exponentialRampToValueAtTime(Math.max(level, 0.0002), start + 0.008);
    gain.gain.setValueAtTime(Math.max(level, 0.0002), Math.max(start + 0.009, end - 0.04));
    gain.gain.exponentialRampToValueAtTime(0.0001, end);
    oscillator.connect(gain).connect(music.master);
    oscillator.start(start);
    oscillator.stop(end + 0.01);
    music.voices.add(oscillator);
    oscillator.addEventListener("ended", () => music.voices.delete(oscillator));
  }
  updateMusicUi();
}
function cursorAt(position) {
  let low = 0, high = music.notes.length;
  while (low < high) { const middle = (low + high) >> 1; if (music.notes[middle].start < position) low = middle + 1; else high = middle; }
  return low;
}
async function playMusic() {
  if (!music.notes.length) return;
  if (!music.context) {
    music.context = new AudioContext();
    music.master = music.context.createGain();
    music.master.gain.value = Number(music.ui.volume.value);
    music.master.connect(music.context.destination);
  }
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
  music.index = (index + music.tracks.length) % music.tracks.length;
  const track = music.tracks[music.index];
  music.ui.title.textContent = track.title;
  music.ui.source.textContent = `${track.source} · extracted MIDI`;
  music.ui.select.value = String(music.index);
  music.ui.card.setAttribute("aria-busy", "true");
  try {
    const response = await fetch(`/music/${track.file}`);
    if (!response.ok) throw new Error(`track returned ${response.status}`);
    const parsed = parseMidi(await response.arrayBuffer());
    music.notes = parsed.notes;
    music.duration = parsed.duration;
    music.position = 0;
    music.ui.seek.max = String(music.duration);
    updateMusicUi();
    if (autoplay) await playMusic();
  } catch (error) {
    music.ui.source.textContent = `Could not read track · ${error instanceof Error ? error.message : error}`;
  } finally { music.ui.card.setAttribute("aria-busy", "false"); }
}
function musicPlayer() {
  if (music.ui?.card) return music.ui.card;
  pauseMusic(true);
  const title = h("strong", { className: "music-title" }, "Reading recovered sequences…");
  const source = h("span", { className: "music-source" }, "Golden Sun sound archive");
  const canvas = h("canvas", { className: "music-wave", width: 640, height: 54, "aria-hidden": "true" });
  const play = h("button", { className: "transport-play", type: "button", "aria-label": "Play" }, "▶");
  const previous = h("button", { type: "button", "aria-label": "Previous track" }, "←");
  const next = h("button", { type: "button", "aria-label": "Next track" }, "→");
  const seek = h("input", { className: "music-seek", type: "range", min: 0, max: 0, step: .01, value: 0, "aria-label": "Track position" });
  const elapsed = h("span", {}, "0:00");
  const duration = h("span", {}, "0:00");
  const volume = h("input", { type: "range", min: 0, max: .8, step: .01, value: .32, "aria-label": "Volume" });
  const select = h("select", { className: "music-select", "aria-label": "Choose a recovered sequence" }, h("option", {}, "Loading tracks…"));
  const card = h("section", { className: "music-player" },
    h("div", { className: "music-label" }, "CARTRIDGE AUDIO · GS1 EN"),
    h("div", { className: "music-heading" }, h("div", {}, title, source), select),
    canvas,
    h("div", { className: "music-controls" }, previous, play, next, elapsed, seek, duration, h("span", { className: "volume-mark", "aria-hidden": "true" }, "VOL"), volume),
    h("p", { className: "music-note" }, "Browser synthesis of the recovered sequence data. Timing and notes come from the ROM; instrument timbres are a lightweight preview."),
  );
  music.ui = { card, title, source, canvas, play, previous, next, seek, elapsed, duration, volume, select };
  play.addEventListener("click", () => music.playing ? pauseMusic() : playMusic());
  previous.addEventListener("click", () => loadMusic(music.index - 1, music.playing));
  next.addEventListener("click", () => loadMusic(music.index + 1, music.playing));
  seek.addEventListener("input", () => { const wasPlaying = music.playing; pauseMusic(); music.position = Number(seek.value); updateMusicUi(); if (wasPlaying) playMusic(); });
  volume.addEventListener("input", () => { if (music.master) music.master.gain.value = Number(volume.value); });
  select.addEventListener("change", () => loadMusic(Number(select.value), music.playing));
  fetch("/music/catalog").then((response) => {
    if (!response.ok) throw new Error(`catalog returned ${response.status}`);
    return response.json();
  }).then((tracks) => {
    music.tracks = tracks;
    select.replaceChildren(...tracks.map((track, index) => h("option", { value: index }, `${String(track.id).padStart(3, "0")} · ${track.title}`)));
    return loadMusic(0);
  }).catch((error) => { source.textContent = `Music catalog unavailable · ${error.message}`; });
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
    h("header", { className: "panel-title" }, h("span", {}, title), h("span", {}, tree === "core" || tree === "overlays" ? "EN build" : "shared assets")),
    chart,
  );
  loadTree(section, tree, title, revision).catch((error) => showError(error instanceof Error ? error.message : String(error)));
  return section;
}
function metric(label, value, detail, tone = "") {
  return h("article", { className: `metric ${tone}` },
    h("div", { className: "metric-label" }, label),
    h("div", { className: "metric-value" }, value),
    h("div", { className: "metric-detail" }, detail),
  );
}
function edition(code, role, tone = "") {
  return h("div", { className: `edition ${tone}` },
    h("strong", {}, upper(code)), h("span", {}, role));
}
function historicalProduct(name, editions, fullTarget) {
  const game = name.toLowerCase();
  return h("section", { className: "product" },
    h("div", { className: "product-name" }, name),
    h("div", { className: "edition-row" },
      ...editions.split(" · ").map((code) => {
        const target = `${game}-${code}`;
        if (target === fullTarget) return edition(code, "full build", "full");
        if (code === "ja") return edition(code, "canonical", "base");
        return edition(code, "compile-only");
      }),
    ),
  );
}
function render(snapshot) {
  hideTooltip();
  const summary = snapshot.summary;
  const project = snapshot.project;
  const header = h("header", { className: "masthead" },
    h("div", { className: "identity" },
      h("div", { className: "kicker" }, "ALCHEMY · RECONSTRUCTION LEDGER"),
      h("h1", {}, project.title),
      h("p", {}, "Two shared historical source trees, twelve edition targets, and one separate reintegration product. Japanese is canonical within each game; every derived ROM must still be proved independently."),
    ),
    h("div", { className: "products", "aria-label": "Historical and integration products" },
      historicalProduct("GS1", project.gs1, project.fullTarget),
      historicalProduct("GS2", project.gs2, project.fullTarget),
      h("section", { className: "product integration" },
        h("div", { className: "product-name" }, project.integration),
        h("div", { className: "integration-copy" }, "GS1 + GS2 reintegration · no reference ROM"),
      ),
    ),
  );
  const correspondenceMetric = summary.correspondenceAvailable
    ? metric("GS1 corpus ×6", `${bytes(summary.correspondenceMatched)} / ${bytes(summary.correspondenceTotal)}`, `${bytes(summary.correspondenceShared)} shared-core · ${bytes(summary.correspondenceRegional)} regional candidates · ${bytes(summary.correspondenceUnresolved)} unresolved`, "cross")
    : metric("GS1 corpus ×6", "Unavailable", "Run make reports to refresh cross-edition reports", "cross");
  const metrics = h("section", { className: "metrics", "aria-label": "Project metrics" },
    metric("EN DONE", percent(summary.donePercent), `${bytes(summary.doneBytes)} proven bytes`, "done"),
    metric("EN Proven C", percent(summary.provenCPercent), `${bytes(summary.provenCBytes)} linked bytes`, "proven-c"),
    metric("Draft C", percent(summary.draftCPercent), `${bytes(summary.draftCBytes)} visible bytes · never counted as DONE`, "draft-c"),
    correspondenceMetric,
    metric("Canonical JA", bytes(summary.gs1JaSources + summary.gs2JaSources), `GS1 ${bytes(summary.gs1JaSources)} · GS2 ${bytes(summary.gs2JaSources)} tracked source owners`, "base"),
    metric("Historical targets", bytes(summary.historicalTargets), `${bytes(summary.fullTargets)} full · ${bytes(summary.compileOnlyTargets)} compile-only · Alchemy separate`, "derived"),
  );
  const legend = h("div", { className: "legend" },
    h("span", {}, h("i", { className: "swatch unknown" }), "Unknown"),
    h("span", {}, h("i", { className: "swatch draft-asm" }), "Draft ASM"),
    h("span", {}, h("i", { className: "swatch draft-c" }), "Draft C"),
    h("span", {}, h("i", { className: "swatch proven-asm" }), "Proven ASM"),
    h("span", {}, h("i", { className: "swatch proven-c" }), "Proven C"),
    h("span", { id: "scan-state", className: "scan-state" }, snapshot.scanning ? "Scanning…" : "Live"),
  );
  const trees = Object.entries(snapshot.trees).map(([tree, title]) => panel(tree, title, snapshot.revision));
  root.replaceChildren(h("div", { className: "shell" }, header, metrics, musicPlayer(), legend, h("main", { className: "trees" }, trees)));
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
    else {
      const state = root.querySelector("#scan-state");
      if (state) state.textContent = snapshot.scanning ? "Scanning…" : "Live";
    }
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
