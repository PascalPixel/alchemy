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
.music-panel { aspect-ratio: 540 / 304; border: 0; border-radius: 0; background: transparent; }
.music-card { display: block; width: 100%; height: 100%; font-family: Weyard; font-style: italic; shape-rendering: crispEdges; }
.music-card text { font-family: Weyard; font-style: italic; }
.sound-shell { fill: #151b25; stroke: #73d0dc; stroke-width: 2; }
.sound-head { fill: #167b89; stroke: #73d0dc; stroke-width: 2; }
.sound-label { fill: #f4efe2; font-size: 15px; }
.sound-title { fill: #f7d35c; font-size: 18px; }
.sound-source, .sound-track, .sound-time, .sound-volume, .sound-foot { fill: #73d0dc; font-size: 12px; }
.sound-screen { fill: #09141b; stroke: #245a63; stroke-width: 2; }
.sound-playhead { stroke: #f4efe2; stroke-width: 2; }
.sound-seek { fill: transparent; cursor: crosshair; }
.sound-button { cursor: pointer; }
.sound-button rect { fill: #245a63; stroke: #73d0dc; stroke-width: 2; }
.sound-button text { fill: #f4efe2; font-size: 20px; pointer-events: none; }
.sound-button.sound-primary rect { fill: #f7d35c; stroke: #f4efe2; }
.sound-button.sound-primary text { fill: #151b25; }
.sound-button:hover rect, .sound-button:focus-visible rect { fill: #167b89; stroke: #f4efe2; }
.sound-button.sound-primary:hover rect, .sound-button.sound-primary:focus-visible rect { fill: #f4efe2; }
.sound-button:focus { outline: none; }
.music-panel[aria-busy="true"] .sound-screen { opacity: .55; }
.panel { min-width: 0; overflow: hidden; border: 1px solid var(--line); border-radius: 10px; background: var(--raised); }
.panel-title { display: flex; justify-content: space-between; padding: 11px 14px 9px; border-bottom: 1px solid var(--line); font-size: 11px; font-weight: 800; letter-spacing: .08em; text-transform: uppercase; }
.panel-title span:last-child { color: var(--muted); font-size: 9px; font-weight: 400; }
.chart { aspect-ratio: 540 / 304; min-height: 180px; }
.tree-image { display: block; width: 100%; height: 100%; object-fit: contain; }
.chart-loading, .loading { display: grid; place-items: center; min-height: 180px; color: var(--muted); font: var(--weyard-font); }
.loading { min-height: 100vh; }
.error { position: fixed; z-index: 3; top: 12px; left: 50%; max-width: calc(100% - 24px); transform: translateX(-50%); padding: 9px 12px; color: #ffc1bb; background: #371b1a; border: 1px solid #a94b42; border-radius: 6px; font: var(--weyard-font); }
.hover-tooltip { position: fixed; z-index: 4; max-width: calc(100vw - 16px); padding: 7px 9px; overflow: hidden; color: #fff; background: rgb(10 10 13 / 96%); border: 1px solid rgb(255 255 255 / 24%); border-radius: 4px; box-shadow: 0 5px 18px rgb(0 0 0 / 45%); font: var(--weyard-font); line-height: 1.2; pointer-events: none; white-space: nowrap; text-overflow: ellipsis; }
.hover-tooltip[hidden] { display: none; }
@media (max-width: 1000px) {
  .cards { grid-template-columns: 1fr; }
}
@media (max-width: 560px) {
  .cards { padding: 18px 12px; }
}
"###;
