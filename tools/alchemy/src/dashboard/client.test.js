import { test, expect } from "bun:test";

test("shared map files open their real folder and highlight without copying", async () => {
  const source = await Bun.file(new URL("./client.js", import.meta.url)).text();
  const path = "games/tbs/GRAPHICS/TILE/SHARED.PNG";
  const selected = [], loads = [], classes = [];
  const file = { getAttribute: () => path, classList: { add: name => classes.push(name) } };
  const selection = { hidden: true, replaceChildren(...children) { this.children = children; } };
  const chart = { dataset: { tree: "rom", folder: "games/tbs/SRC/FIELD/XIAN/" }, clientWidth: 540,
    querySelector: () => ({ getAttribute: () => JSON.stringify([path]) }), querySelectorAll: () => [file] };
  const panel = { querySelector: name => name === ".chart" ? chart : name === ".viewer-selection" ? selection : {} };
  class Element {
    constructor(action) { this.action = action; }
    closest(name) { return name === ".panel" ? panel : name === "[data-action]" ? this : null; }
    getAttribute(name) { return name === "data-action" ? this.action : path; }
  }
  const activate = new Function("Element", "h", "showSelection", "loadTree", "hideTooltip", "showError",
    source.slice(source.indexOf("async function activateTile"), source.indexOf("function showSelection")) + ";return activateTile;")(
      Element, (...args) => args, (_, tile) => selected.push(tile), async (...args) => loads.push(args), () => {}, error => { throw error; });
  await activate({ type: "keydown", key: "Enter", target: new Element("shared"), preventDefault() {} });
  expect(selection.hidden).toBe(false);
  expect(JSON.stringify(selection.children)).toContain(path);
  expect(loads).toHaveLength(0);
  await activate({ type: "keydown", key: "Enter", target: new Element("open-shared"), preventDefault() {} });
  expect(loads).toHaveLength(0);
  await activate({ type: "click", target: new Element("open-shared"), preventDefault() {} });
  expect(chart.dataset.folder).toBe("games/tbs/GRAPHICS/TILE/");
  expect(loads).toHaveLength(1);
  expect(selected).toEqual([file]);
  expect(classes).toEqual(["shared-highlight"]);
});

async function coverageClient() {
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
  const functions = new Function("Element", "tooltip", "window", "fetch", "DOMParser",
    source.slice(source.indexOf("function hideTooltip"), source.indexOf("function panel")) + ";return { showTooltip, loadTree };");
  return {
    ...functions(Element, tooltip, { innerWidth: 800, innerHeight: 600 },
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

test("folder requests preserve the selected folder and discard stale responses", async () => {
  const ui = await coverageClient();
  let installed;
  const chart = {
    dataset: { folder: "games/tbs/SRC/FIELD/XIAN/", width: "540" },
    replaceChildren(svg) { installed = svg; },
  };
  const section = { querySelector: () => chart };
  await ui.loadTree(section, "rom", "ROM contents", "1", 540, chart.dataset.folder);
  expect(installed).toBe(ui.svg);
  expect(ui.requests.at(-1)).toBe("/svg/rom/540/games/tbs/SRC/FIELD/XIAN/?v=1");
  installed = undefined;
  await ui.loadTree(section, "rom", "ROM contents", "1", 540, "games/tbs/");
  expect(installed).toBeUndefined();
});
test("resized charts request native pixel dimensions and reject stale-size responses", async () => {
  const ui = await coverageClient();
  let installed;
  const chart = { dataset: { width: '366' }, clientHeight: 600, replaceChildren(svg) { installed = svg; } };
  const section = { querySelector: () => chart };
  await ui.loadTree(section, 'core', 'Main game', '2', 800);
  expect(installed).toBeUndefined();
  await ui.loadTree(section, 'core', 'Main game', '2', 366);
  expect(installed).toBe(ui.svg);
  expect(ui.requests).toEqual(['/svg/core/800x600?v=2', '/svg/core/366x600?v=2']);
});

test("file activation does not navigate its ancestor folder", async () => {
  const source = await Bun.file(new URL("./client.js", import.meta.url)).text();
  const chart = { dataset: { folder: "games/tbs/", tree: "rom", title: "ROM", revision: "1" }, clientWidth: 540 };
  const back = { hidden: true };
  const selection = { hidden: true };
  const panel = { querySelector: selector => selector === ".chart" ? chart : selector === ".viewer-selection" ? selection : back };
  class Element {
    constructor(kind, parent = null) { this.kind = kind; this.parent = parent; }
    getAttribute(name) { return name === "data-kind" ? this.kind : "games/tbs/SRC/"; }
    closest(selector) {
      if (selector === ".panel") return panel;
      if (selector === "[data-action]") return null;
      if (selector === "[data-action='back']") return null;
      if (selector === "g[data-node]") return this.kind ? this : this.parent?.closest(selector);
      if (selector === "g[data-kind='folder'][data-source]") return this.kind === "folder" ? this : this.parent?.closest(selector);
      throw Error(selector);
    }
  }
  let selections = 0, loads = 0;
  const activate = new Function("Element", "showSelection", "loadTree", "hideTooltip", "showError",
    source.slice(source.indexOf("async function activateTile"), source.indexOf('function showSelection')) + ";return activateTile;")(
      Element, () => { selections++; selection.hidden = false; }, async () => loads++, () => {}, error => { throw Error(error); });
  const folder = new Element("folder"), file = new Element("gba-4bpp-tiles", folder);
  for (const [type, key] of [["click"], ["keydown", "Enter"], ["keydown", " "]]) {
    await activate({ type, key, target: new Element(null, file), preventDefault() {} });
  }
  expect(selections).toBe(3);
  expect(selection.hidden).toBe(false);
  expect(loads).toBe(0);
  expect(chart.dataset.folder).toBe("games/tbs/");
  await activate({ type: "click", target: new Element(null, folder), preventDefault() {} });
  expect(loads).toBe(1);
  expect(chart.dataset.folder).toBe("games/tbs/SRC/");
  expect(back.hidden).toBe(false);
  expect(selection.hidden).toBe(true);
});

test("selection shows source details without copying and tolerates missing addresses", async () => {
  const source = await Bun.file(new URL("./client.js", import.meta.url)).text();
  const show = new Function("h", source.slice(source.indexOf("function showSelection"), source.indexOf('root.addEventListener("click"')) + ";return showSelection;")(
    (tag, attributes, ...children) => ({ tag, attributes, children }));
  const selection = { dataset: {}, hidden: true, replaceChildren(...children) { this.children = children; } };
  const attributes = { "data-source": "games/tbs/SRC/BATTLE/MAIN.C", "data-address": "0x080bbb0c", "aria-label": "Battle action: 6,332 bytes" };
  show(selection, { getAttribute: name => attributes[name] ?? null });
  expect(selection.hidden).toBe(false);
  expect(selection.dataset.address).toBe("0x080bbb0c");
  expect(selection.children[0].children[0]).toBe(attributes["data-source"]);
  expect(selection.children[2].children[0].attributes["data-action"]).toBe("copy-selection");
  show(selection, { getAttribute: () => null });
  expect(selection.dataset.address).toBe("");
  expect(selection.children[0].children[0]).toBe("Unresolved source");
  expect(selection.children[2].children[0]).toBeNull();
});

test("selection controls copy once, report rejection, and close without navigating", async () => {
  const source = await Bun.file(new URL("./client.js", import.meta.url)).text();
  const feedback = { textContent: "" };
  const selection = { hidden: false, dataset: { address: "0x080bbb0c" }, querySelector: () => feedback };
  const panel = { querySelector: name => name === ".viewer-selection" ? selection : {} };
  class Element {
    constructor(action) { this.action = action; }
    closest(selector) { return selector === ".panel" ? panel : selector === "[data-action]" ? this : null; }
    getAttribute() { return this.action; }
  }
  const copied = [];
  let reject = false;
  const activate = new Function("Element", "navigator",
    source.slice(source.indexOf("async function activateTile"), source.indexOf("function showSelection")) + ";return activateTile;")(
      Element, { clipboard: { async writeText(value) { if (reject) throw Error("denied"); copied.push(value); } } });
  const event = { type: "keydown", key: "Enter", target: new Element("copy-selection"), preventDefault() {} };
  await activate(event);
  expect(copied).toEqual([]);
  event.type = "click";
  await activate(event);
  expect(copied).toEqual(["0x080bbb0c"]);
  expect(feedback.textContent).toBe("Address copied");
  reject = true;
  await activate(event);
  expect(feedback.textContent).toBe("Could not copy address");
  event.target = new Element("close-selection");
  await activate(event);
  expect(selection.hidden).toBe(true);
});
