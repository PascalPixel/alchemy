//! Static dashboard assets owned by the Rust server.
//!
//! Keeping the stylesheet here makes the dashboard reproducible from the Rust
//! crate alone. The HTTP route still uses `/styles.css` for browser semantics,
//! but no standalone web source file is needed at runtime or in the repository.

pub const STYLES: &str = r###"@font-face {
  font-family: Weyard;
  src: url("/weyard.otf") format("opentype");
  font-weight: 400;
  font-style: italic;
  font-display: block;
}

:root {
  --weyard-font: italic 400 16px/15px Weyard;
}

* {
  box-sizing: border-box;
  font-smooth: never;
  -webkit-font-smoothing: none;
  font-synthesis: none;
}

html,
body {
  width: 100vw;
  height: 100%;
  margin: 0;
  overflow: hidden;
  background: #fff;
}

#root {
  position: relative;
  width: 100vw;
  height: 100vh;
  height: 100dvh;
  overflow: hidden;
}

.trees {
  position: absolute;
  inset: 0;
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  grid-template-rows: repeat(2, minmax(0, 1fr));
  overflow: hidden;
}

.tree-image {
  width: 100%;
  height: 100%;
  max-width: 100%;
  max-height: 100%;
  display: block;
  object-fit: contain;
  object-position: center;
}

.panel {
  min-width: 0;
  min-height: 0;
  overflow: hidden;
}

.error {
  position: absolute;
  z-index: 1;
  top: 8px;
  left: 50%;
  max-width: calc(100% - 16px);
  transform: translateX(-50%);
  padding: 8px 10px;
  color: #7d211b;
  background: #fff1ef;
  border: 2px solid #7c2626;
  border-radius: 6px;
  font: var(--weyard-font);
}

.loading {
  position: absolute;
  inset: 0;
  height: 100%;
  display: grid;
  place-items: center;
  color: #555;
  font: var(--weyard-font);
}

.hover-tooltip {
  position: fixed;
  z-index: 2;
  max-width: calc(100vw - 16px);
  padding: 7px 9px;
  overflow: hidden;
  color: #fff;
  background: rgb(25 25 31 / 94%);
  border: 1px solid rgb(255 255 255 / 35%);
  border-radius: 4px;
  box-shadow: 0 3px 12px rgb(0 0 0 / 35%);
  font: var(--weyard-font);
  line-height: 1.2;
  pointer-events: none;
  white-space: nowrap;
  text-overflow: ellipsis;
}

.hover-tooltip[hidden] {
  display: none;
}

@media (prefers-color-scheme: dark) {
  html, body { background: #131318; }
  .error { color: #ffc1bb; background: #371b1a; }
}

/* Each chart is 540:304. These cutovers select the packing that gives every
 * chart the largest possible contained rectangle for the viewport shape. */
@media (min-aspect-ratio: 135 / 38) {
  .trees {
    grid-template-columns: repeat(4, minmax(0, 1fr));
    grid-template-rows: minmax(0, 1fr);
  }
}

@media (max-aspect-ratio: 135 / 152) {
  .trees {
    grid-template-columns: minmax(0, 1fr);
    grid-template-rows: repeat(4, minmax(0, 1fr));
  }
}
"###;
