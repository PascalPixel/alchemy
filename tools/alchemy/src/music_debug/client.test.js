import { test, expect } from "bun:test";
test("music is one matching card with a fixed pixel font and repeat, not volume", async () => {
  const css = await Bun.file(new URL("./style.css", import.meta.url)).text(), js = await Bun.file(new URL("./client.js", import.meta.url)).text();
  expect(css).toContain('.music-player, .music-player button, .music-player output { font: var(--weyard-font); }');
  expect(css.split('\n').filter(line => line.startsWith('.music')).join('\n')).not.toContain('font-size:');
  expect(css.match(/\.music-player \{[^}]+/)[0]).not.toContain('grid-column');
  expect(css).toContain('--raised: #1f7f93;'); expect(css).toContain('--backdrop: #103840;');
  expect(css).toContain('--music-edge: var(--raised);'); expect(css).not.toContain('radial-gradient');
  expect(css).toContain('text-shadow: 1px 1px 0 #000;');
  expect(css).toContain('border-color: #c9e1dc #103840 #103840 #c9e1dc;');
  expect(js).toContain('volume: 1,'); expect(js).toContain('Repeat this track'); expect(js).not.toContain('music-volume');
});
test("PCM playback excludes the ROM lookahead byte and preserves native loop pitch", async () => {
  const source = await Bun.file(new URL("./client.js", import.meta.url)).text();
  const context = { createBuffer: (_, length, sampleRate) => ({ length, sampleRate, duration: length / sampleRate, copyToChannel(data) { this.data = data; } }) };
  const [decode, loopEnd] = new Function("ensureAudio", source.slice(source.indexOf("function pcmBuffer"), source.indexOf("async function prepareTrackSamples")) + ";return [pcmBuffer, pcmLoopEnd];")(() => context);
  for (const game of ["gs1", "gs2"]) {
    const base = new URL(`../../../../games/${game}/assets/sound/`, import.meta.url);
    const rows = (await Bun.file(new URL("samples.tsv", base)).text()).trim().split("\n").filter(line => !line.startsWith("#")).slice(1);
    for (const row of rows) {
      const [, , frequency, loop, count, path] = row.split("\t");
      const wav = await Bun.file(new URL(path, base)).arrayBuffer();
      const buffer = decode(wav);
      expect(buffer.length).toBe(Number(count));
      expect(buffer.sampleRate).toBe(Math.round(Number(frequency) / 1024));
      expect(buffer.data[0]).toBe((new Uint8Array(wav)[44] - 128) / 128);
      if (loop) expect(loopEnd(buffer, {}) - Number(loop) / buffer.sampleRate).toBeCloseTo((Number(count) - 1 - Number(loop)) / buffer.sampleRate, 12);
      if (game === "gs1" && path.endsWith("wave_10.pcm8.wav")) expect(loopEnd(buffer, {}) * buffer.sampleRate - Number(loop)).toBe(42);
    }
  }
  expect(() => decode(new ArrayBuffer(0))).toThrow("canonical mono PCM8");
});
