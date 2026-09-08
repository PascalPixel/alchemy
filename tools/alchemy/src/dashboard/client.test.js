import { test, expect } from "bun:test";

async function coverageClient(writeText = async () => {}) {
  const source = await Bun.file(new URL("./client.js", import.meta.url)).text();
  class Element {
    constructor(attributes = {}, parent = null) { this.attributes = attributes; this.parent = parent; }
    getAttribute(name) { return this.attributes[name] ?? null; }
    setAttribute(name, value) { this.attributes[name] = value; }
    closest(selector) {
      const attribute = selector === "g[data-address]" ? "data-address" : "aria-label";
      return this.getAttribute(attribute) ? this : this.parent?.closest(selector) ?? null;
    }
    getBoundingClientRect() { return { left: 20, top: 30, width: 80, height: 50 }; }
  }
  const tile = new Element({ "aria-label": "Battle action: 6,332 bytes", "data-address": "0x080bbb0c" });
  const titles = [{ remove() { this.removed = true; } }, { remove() { this.removed = true; } }];
  const svg = new Element();
  svg.localName = "svg";
  svg.classList = { add() {} };
  svg.querySelectorAll = (selector) => selector === "title" ? titles : [tile];
  const tooltip = { hidden: true, style: {}, getBoundingClientRect: () => ({ width: 160, height: 30 }) };
  const requests = [];
  const functions = new Function("Element", "tooltip", "window", "navigator", "fetch", "DOMParser",
    source.slice(source.indexOf("function hideTooltip"), source.indexOf("function panel")) + ";return { showTooltip, copyTileAddress, loadTree };");
  return {
    ...functions(Element, tooltip, { innerWidth: 800, innerHeight: 600 }, { clipboard: { writeText } },
      async url => { requests.push(url); return { ok: true, text: async () => "fixture" }; }, class { parseFromString() { return { documentElement: svg }; } }),
    tile, titles, svg, tooltip, requests, child: new Element({}, tile), background: new Element(),
  };
}
test("coverage hover uses JS labels and removes every native SVG title", async () => {
  const ui = await coverageClient();
  let installed;
  await ui.loadTree({ querySelector: () => ({ replaceChildren(svg) { installed = svg; } }) }, "core", "Main game", "1");
  expect(installed).toBe(ui.svg);
  expect(ui.requests).toEqual(['/svg/core/540?v=1']);
  expect(ui.titles.every(title => title.removed)).toBe(true);
  expect(ui.svg.getAttribute("aria-label")).toBe("Main game coverage graph");
  expect(ui.tile.getAttribute("role")).toBe("button");
  expect(ui.tile.getAttribute("tabindex")).toBe("0");
  ui.showTooltip({ target: ui.child, clientX: 50, clientY: 60 });
  expect(ui.tooltip.textContent).toBe("Battle action: 6,332 bytes");
  expect(ui.tooltip.hidden).toBe(false);
  ui.showTooltip({ target: ui.background });
  expect(ui.tooltip.hidden).toBe(true);
});
test("resized charts request native pixel dimensions and reject stale-size responses", async () => {
  const ui = await coverageClient();
  let installed;
  const chart = { dataset: { width: '366' }, replaceChildren(svg) { installed = svg; } };
  const section = { querySelector: () => chart };
  await ui.loadTree(section, 'core', 'Main game', '2', 800);
  expect(installed).toBeUndefined();
  await ui.loadTree(section, 'core', 'Main game', '2', 366);
  expect(installed).toBe(ui.svg);
  expect(ui.requests).toEqual(['/svg/core/800?v=2', '/svg/core/366?v=2']);
});
test("coverage click and keyboard activation copy only the explicit start address", async () => {
  const copied = [];
  const ui = await coverageClient(async address => { copied.push(address); });
  for (const [type, key] of [["click"], ["keydown", "Enter"], ["keydown", " "]]) {
    let prevented = false;
    await ui.copyTileAddress({ type, key, target: ui.child, preventDefault() { prevented = true; } });
    expect(prevented).toBe(true);
    expect(ui.tooltip.textContent).toBe("Copied 0x080bbb0c");
    expect(ui.tooltip.style.left).not.toContain("NaN");
  }
  await ui.copyTileAddress({ type: "click", target: ui.background });
  await ui.copyTileAddress({ type: "keydown", key: "Tab", target: ui.child });
  expect(copied).toEqual(["0x080bbb0c", "0x080bbb0c", "0x080bbb0c"]);
});
test("coverage clipboard rejection never reports a successful copy", async () => {
  const ui = await coverageClient(async () => { throw new Error("denied"); });
  await ui.copyTileAddress({ type: "click", target: ui.child, preventDefault() {} });
  expect(ui.tooltip.textContent).toBe("Could not copy 0x080bbb0c");
});
test("nested coverage leaves copy their own address, not their container", async () => {
  const copied = [], ui = await coverageClient(async address => copied.push(address));
  ui.tile.parent = new ui.tile.constructor({ "aria-label": "Container", "data-address": "0x081a7020", "data-node": "container" });
  await ui.copyTileAddress({ type: "click", target: ui.child, preventDefault() {} });
  expect(copied).toEqual(["0x080bbb0c"]);
});
