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
.music-player {
  position: relative;
  display: grid;
  grid-template-columns: minmax(0, 1fr);
  gap: 12px;
  margin: 0;
  padding: 17px 18px 14px;
  overflow: hidden;
  border: 1px solid #514627;
  border-radius: 10px;
  background: linear-gradient(105deg, #242019, #18171c 48%, #1c1b21);
  box-shadow: inset 4px 0 0 var(--gold), inset 0 1px rgb(255 255 255 / 5%);
}
.music-player::after {
  content: "";
  position: absolute;
  right: -42px;
  bottom: -55px;
  width: 210px;
  height: 120px;
  border: 1px solid rgb(244 201 93 / 10%);
  border-radius: 50%;
  box-shadow: 0 0 0 16px rgb(244 201 93 / 3%), 0 0 0 32px rgb(244 201 93 / 2%);
  pointer-events: none;
}
.music-label { color: var(--gold); font-size: 9px; font-weight: 800; letter-spacing: .16em; }
.music-heading { z-index: 1; display: flex; align-items: end; justify-content: space-between; gap: 18px; }
.music-heading > div { display: grid; gap: 5px; min-width: 0; }
.music-title { overflow: hidden; font: var(--weyard-font); font-size: 20px; line-height: 1.1; white-space: nowrap; text-overflow: ellipsis; }
.music-source { color: var(--muted); font-size: 9px; letter-spacing: .06em; text-transform: uppercase; }
.music-select {
  width: min(310px, 42vw);
  padding: 8px 32px 8px 10px;
  color: var(--ink);
  border: 1px solid var(--line);
  border-radius: 6px;
  background: #111015;
  font: inherit;
  font-size: 10px;
}
.music-wave { z-index: 1; display: block; width: 100%; height: 54px; border-block: 1px solid rgb(244 201 93 / 18%); }
.music-controls { z-index: 1; display: grid; grid-template-columns: 34px 42px 34px auto minmax(100px, 1fr) auto auto minmax(70px, 130px); align-items: center; gap: 9px; }
.music-controls button {
  height: 32px;
  padding: 0;
  color: var(--ink);
  border: 1px solid #514627;
  border-radius: 5px;
  background: #242019;
  font: inherit;
  cursor: pointer;
}
.music-controls button:hover { color: #111015; background: var(--gold); }
.music-controls .transport-play { height: 38px; color: #111015; border-color: var(--gold); background: var(--gold); font-size: 16px; }
.music-controls > span { color: var(--muted); font-size: 9px; font-variant-numeric: tabular-nums; }
.music-controls .volume-mark { color: var(--gold); font-size: 8px; font-weight: 800; letter-spacing: .12em; }
.music-controls input[type="range"] { width: 100%; accent-color: var(--gold); cursor: pointer; }
.music-note { z-index: 1; margin: 0; color: #77736a; font-size: 8px; line-height: 1.4; }
.music-player[aria-busy="true"] .music-wave { opacity: .35; }
.music-controls button:focus-visible, .music-select:focus-visible, .music-controls input:focus-visible { outline: 2px solid var(--cyan); outline-offset: 2px; }
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
  .music-heading { align-items: stretch; flex-direction: column; }
  .music-select { width: 100%; }
  .music-controls { grid-template-columns: 34px 42px 34px 1fr auto; }
  .music-seek { grid-column: 1 / -1; grid-row: 2; }
  .music-controls .volume-mark, .music-controls input[aria-label="Volume"] { display: none; }
}
@media (prefers-reduced-motion: reduce) {
  .music-player, .music-wave { scroll-behavior: auto; }
}
"###;
