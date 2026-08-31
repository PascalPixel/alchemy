pub const STYLES: &str = r###"
@font-face {
  font-family: Weyard;
  src: url("/weyard.otf") format("opentype");
  font-weight: 400;
  font-style: italic;
  font-display: block;
}
:root {
  color-scheme: dark;
  --ink: #f4efe2;
  --muted: #aaa69b;
  --line: #343238;
  --raised: #1c1b21;
  --gold: #f4c95d;
  --violet: #8b6cdf;
  --cyan: #7dd3fc;
  --green: #82d7a0;
  --weyard-font: italic 400 16px/15px Weyard;
}
* { box-sizing: border-box; }
html, body { min-width: 320px; min-height: 100%; margin: 0; background: #0f0e12; }
body {
  color: var(--ink);
  font-family: ui-monospace, SFMono-Regular, Menlo, Consolas, monospace;
  background: radial-gradient(circle at 10% -10%, rgb(139 108 223 / 16%), transparent 32rem), radial-gradient(circle at 95% 0%, rgb(244 201 93 / 10%), transparent 28rem), #0f0e12;
}
#root { min-height: 100vh; }
.cards { display: grid; grid-template-columns: repeat(2, minmax(0, 1fr)); gap: 14px; width: min(1600px, 100%); margin: 0 auto; padding: 28px; }
.music-player { grid-column: 1 / -1; min-width: 0; overflow: hidden; border: 1px solid #73d0dc; border-radius: 10px; background: #123437; font-family: Weyard; font-style: italic; }
.music-player, .music-player button, .music-player output { font: var(--weyard-font); }
.music-titlebar { display: flex; align-items: center; justify-content: space-between; min-height: 42px; padding: 8px 16px; color: #f4efe2; background: #167b89; border-bottom: 2px solid #f4efe2; }
.music-list { height: min(430px, 52vh); overflow-y: auto; scrollbar-color: #73d0dc #123437; }
.music-row { display: grid; grid-template-columns: 68px minmax(0, 1fr) minmax(160px, .7fr); align-items: center; width: 100%; min-height: 42px; padding: 0 16px; color: #f4efe2; background: transparent; border: 0; border-bottom: 1px solid rgb(115 208 220 / 28%); text-align: left; cursor: pointer; }
.music-row:hover { background: rgb(115 208 220 / 10%); }
.music-row[aria-current="true"] { color: #151b25; background: #f7d35c; }
.music-row:focus-visible { position: relative; z-index: 1; outline: 2px solid #f4efe2; outline-offset: -3px; }
.music-row-request { color: #73d0dc; letter-spacing: .08em; }
.music-row[aria-current="true"] .music-row-request, .music-row[aria-current="true"] .music-row-source { color: #245a63; }
.music-row-title { overflow: hidden; white-space: nowrap; text-overflow: ellipsis; }
.music-row-source { overflow: hidden; color: #73d0dc; text-align: right; white-space: nowrap; text-overflow: ellipsis; }
.music-chin { display: grid; grid-template-columns: minmax(190px, .75fr) auto minmax(300px, 1.4fr); gap: 16px; align-items: center; min-height: 96px; padding: 14px 16px 16px; background: #167b89; border-top: 2px solid #f4efe2; }
.music-now { min-width: 0; }
.music-now-title, .music-now-source { overflow: hidden; white-space: nowrap; text-overflow: ellipsis; }
.music-now-title { color: #f7d35c; }
.music-now-source { margin-top: 5px; color: #b7edf2; }
.music-controls { display: flex; gap: 7px; }
.music-control { display: grid; width: 42px; height: 42px; padding: 0; place-items: center; color: #f4efe2; background: #245a63; border: 2px solid #73d0dc; cursor: pointer; }
.music-control svg { width: 20px; height: 20px; fill: currentColor; stroke: currentColor; stroke-width: 2; }
.music-control-primary { color: #151b25; background: #f7d35c; border-color: #f4efe2; }
.music-control[aria-pressed="true"] { color: #151b25; background: #f7d35c; border-color: #f4efe2; }
.music-control:hover, .music-control:focus-visible { color: #151b25; background: #f4efe2; outline: 0; }
.music-control[aria-disabled="true"] { opacity: .45; cursor: wait; }
.music-scrubber-shell { display: grid; grid-template-columns: 42px minmax(0, 1fr) 42px; gap: 8px; align-items: center; min-width: 0; color: #f4efe2; }
.music-wave { position: relative; display: flex; align-items: center; gap: 2px; height: 50px; overflow: hidden; }
.music-wave-bar { flex: 1 1 0; min-width: 1px; height: var(--bar, 8%); background: rgb(183 237 242 / 36%); }
.music-wave-bar.passed { background: #f7d35c; }
.music-range { position: absolute; inset: 0; width: 100%; height: 100%; margin: 0; appearance: none; background: transparent; cursor: pointer; }
.music-range::-webkit-slider-runnable-track { height: 100%; background: transparent; }
.music-range::-webkit-slider-thumb { width: 3px; height: 50px; margin: 0; appearance: none; background: #f4efe2; box-shadow: 0 0 0 1px #151b25; }
.music-player[aria-busy="true"] .music-chin { opacity: .72; }
.sr-only { position: absolute; width: 1px; height: 1px; padding: 0; overflow: hidden; clip: rect(0, 0, 0, 0); white-space: nowrap; border: 0; }
.panel { min-width: 0; overflow: hidden; border: 1px solid var(--line); border-radius: 10px; background: var(--raised); }
.chart { aspect-ratio: 540 / 304; min-height: 180px; }
.tree-image { display: block; width: 100%; height: 100%; object-fit: contain; }
.chart-loading, .loading { display: grid; place-items: center; min-height: 180px; color: var(--muted); font: var(--weyard-font); }
.loading { min-height: 100vh; }
.error { position: fixed; z-index: 3; top: 12px; left: 50%; max-width: calc(100% - 24px); transform: translateX(-50%); padding: 9px 12px; color: #ffc1bb; background: #371b1a; border: 1px solid #a94b42; border-radius: 6px; font: var(--weyard-font); }
.hover-tooltip { position: fixed; z-index: 4; max-width: calc(100vw - 16px); padding: 7px 9px; overflow: hidden; color: #fff; background: rgb(10 10 13 / 96%); border: 1px solid rgb(255 255 255 / 24%); border-radius: 4px; box-shadow: 0 5px 18px rgb(0 0 0 / 45%); font: var(--weyard-font); line-height: 1.2; pointer-events: none; white-space: nowrap; text-overflow: ellipsis; }
.hover-tooltip[hidden] { display: none; }
@media (max-width: 1000px) {
  .cards { grid-template-columns: 1fr; }
  .music-chin { grid-template-columns: minmax(180px, 1fr) auto minmax(280px, 1.4fr); }
}
@media (max-width: 560px) {
  .cards { padding: 18px 12px; }
  .music-row { grid-template-columns: 54px minmax(0, 1fr); padding: 0 12px; }
  .music-row-source { display: none; }
  .music-chin { grid-template-columns: minmax(0, 1fr) auto; gap: 12px; }
  .music-scrubber-shell { grid-column: 1 / -1; }
}
"###;
