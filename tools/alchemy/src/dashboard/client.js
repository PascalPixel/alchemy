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
function percent(value) { return `${Number(value ?? 0).toFixed(2)}%`; }

async function requestSnapshot() {
  const response = await fetch("/snapshot", { cache: "no-store" });
  if (!response.ok) throw new Error(`/snapshot returned ${response.status}`);
  return response.json();
}
function hideTooltip() { tooltip.hidden = true; }
function showTooltip(event, message) {
  const target = event.target instanceof Element ? event.target.closest("g[aria-label]") : null;
  const label = message ?? target?.getAttribute("aria-label")?.trim();
  if (!target || !label) { hideTooltip(); return; }
  tooltip.textContent = label;
  tooltip.hidden = false;
  const gap = 14;
  const edge = 8;
  const bounds = tooltip.getBoundingClientRect();
  const anchor = target.getBoundingClientRect();
  const x = event.clientX ?? anchor.left + anchor.width / 2;
  const y = event.clientY ?? anchor.top + anchor.height / 2;
  const left = Math.min(x + gap, window.innerWidth - bounds.width - edge);
  const below = y + gap;
  const top = below + bounds.height <= window.innerHeight - edge ? below : y - bounds.height - gap;
  tooltip.style.left = `${Math.max(edge, left)}px`;
  tooltip.style.top = `${Math.max(edge, top)}px`;
}
async function loadTree(section, tree, title, revision, width = 540, folder = "") {
  const height = section.querySelector(".chart")?.clientHeight ?? 0;
  const response = await fetch(`/svg/${tree}/${width}${height ? `x${height}` : ""}${folder ? `/${folder}` : ""}?v=${encodeURIComponent(revision)}`);
  if (!response.ok) throw new Error(`/svg/${tree} returned ${response.status}`);
  const parsed = new DOMParser().parseFromString(await response.text(), "image/svg+xml");
  const svg = parsed.documentElement;
  if (svg.localName !== "svg") throw new Error(`/svg/${tree} did not return an SVG`);
  // Keep accessible labels, but let only the dashboard's JS tooltip render.
  svg.querySelectorAll("title").forEach((title) => title.remove());
  svg.querySelectorAll("g[data-address], g[data-kind='folder']").forEach((tile) => {
    tile.setAttribute("role", "button");
    tile.setAttribute("tabindex", "0");
  });
  svg.classList.add("tree-image");
  svg.setAttribute("role", "group");
  svg.setAttribute("aria-label", `${title} coverage graph`);
  const chart = section.querySelector(".chart");
  if (chart?.dataset?.width && chart.dataset.width !== String(width)) return;
  if (height && chart.clientHeight !== height) return;
  if ((chart?.dataset?.folder ?? "") !== folder) return;
  chart?.replaceChildren(svg);
}
function panel(tree, title, revision) {
  const chart = h("div", { className: "chart", "data-tree": tree, "data-title": title, "data-revision": revision }, h("div", { className: "chart-loading" }, "Reading…"));
  const section = h("section", { className: `panel p-${tree}` },
    h("button", { className: "viewer-back", "data-action": "back", "aria-label": "Back to parent folder", hidden: true }, "‹"),
    chart,
    h("div", { className: "viewer-selection", hidden: true, "aria-live": "polite" }),
  );
  treeObserver.observe(chart);
  return section;
}
// Reflow at CSS-pixel dimensions; never scale the 16px bitmap font with a viewBox.
const treeObserver = new ResizeObserver(entries => {
  for (const { target: chart } of entries) {
    const width = chart.clientWidth;
    if (!width || (width === Number(chart.dataset.width) && chart.clientHeight === Number(chart.dataset.height))) continue;
    chart.dataset.width = String(width);
    chart.dataset.height = String(chart.clientHeight);
    loadTree(chart.parentElement, chart.dataset.tree, chart.dataset.title, chart.dataset.revision, width, chart.dataset.folder ?? "")
      .catch(error => showError(error instanceof Error ? error.message : String(error)));
  }
});
function render(snapshot) {
  hideTooltip();
  treeObserver.disconnect();
  const trees = Object.entries(snapshot.trees).map(([tree, title]) => panel(tree, title, snapshot.revision));
  root.replaceChildren(h("main", { className: "cards" }, ...trees));
  lastRevision = snapshot.revision;
  lastError = "";
}
function showError(message, prefix = "Dashboard update failed: ") {
  if (message === lastError) return;
  root.querySelector(".error")?.remove();
  root.prepend(h("div", { className: "error", role: "alert" }, `${prefix}${message}`));
  lastError = message;
}
function clearError() { root.querySelector(".error")?.remove(); lastError = ""; }
function accept(snapshot) {
  if (pageVersion !== "" && snapshot.page !== pageVersion) { location.reload(); return; }
  pageVersion = snapshot.page;
  root.setAttribute("aria-busy", String(snapshot.scanning));
  if (snapshot.hasCharts && snapshot.revision !== lastRevision) render(snapshot);
  document.title = snapshot.summary
    ? `Alchemy — 12 targets · ${percent(snapshot.summary.donePercent)} TBS EN done`
    : "Alchemy — published coverage";
  if (snapshot.published) {
    showError("Showing published charts. Live coverage is unavailable until the current build verifies.", "");
    root.querySelector(".error")?.setAttribute("title", snapshot.error ?? "Coverage refresh pending");
  } else if (snapshot.error !== undefined) showError(snapshot.error);
  else {
    clearError();
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
async function activateTile(event) {
  if (event.type === "keydown" && event.key !== "Enter" && event.key !== " ") return;
  const target = event.target instanceof Element ? event.target : null;
  const panel = target?.closest(".panel");
  const chart = panel?.querySelector(".chart");
  const selection = panel?.querySelector(".viewer-selection");
  const action = target?.closest("[data-action]")?.getAttribute("data-action");
  // Native buttons synthesize click for keyboard activation.
  if (action && action !== "shared" && event.type === "keydown") return;
  if (action === "shared") {
    event.preventDefault();
    const files = JSON.parse(chart.querySelector("metadata[data-shared]").getAttribute("data-shared"));
    selection.replaceChildren(h("div", {}, "Shared map files — stored once"),
      h("div", { className: "selection-controls" }, files.map(source =>
        h("button", { "data-action": "open-shared", "data-source": source }, source)),
        h("button", { "data-action": "close-selection" }, "Close")));
    selection.hidden = false;
    return;
  }
  if (action === "close-selection") {
    event.preventDefault();
    selection.hidden = true;
    return;
  }
  if (action === "copy-selection") {
    event.preventDefault();
    try {
      await navigator.clipboard.writeText(selection.dataset.address);
      selection.querySelector(".selection-feedback").textContent = "Address copied";
    } catch {
      selection.querySelector(".selection-feedback").textContent = "Could not copy address";
    }
    return;
  }
  if (action === "reveal-selection") {
    event.preventDefault();
    const button = target.closest("[data-action]");
    if (button.disabled) return;
    button.disabled = true;
    const feedback = selection.querySelector(".selection-feedback");
    try {
      const response = await fetch(`/reveal/${encodeURIComponent(selection.dataset.source)}`, {
        method: "POST", headers: { "X-Alchemy-Action": "1" },
      });
      if (!response.ok) throw Error("Reveal failed");
      feedback.textContent = "Shown in file browser";
    } catch {
      feedback.textContent = "Could not show this item — it may not be on disk";
    } finally { button.disabled = false; }
    return;
  }
  const tile = target?.closest("g[data-node]");
  const folder = tile?.getAttribute("data-kind") === "folder" ? tile : null;
  const back = target?.closest("[data-action='back']");
  if (!chart) return;
  const sharedSource = action === "open-shared" ? target.closest("[data-action]").getAttribute("data-source") : null;
  if (!folder && !back && !sharedSource) {
    if (!tile || !selection) return;
    event.preventDefault();
    showSelection(selection, tile);
    hideTooltip();
    return;
  }
  event.preventDefault();
  const path = sharedSource ? sharedSource.replace(/[^/]+$/, "") : back
    ? (chart.dataset.folder ?? "").replace(/[^/]+\/$/, "")
    : folder.getAttribute("data-source");
  chart.dataset.folder = path;
  if (selection) selection.hidden = true;
  panel.querySelector(".viewer-back").hidden = !path;
  hideTooltip();
  try {
    await loadTree(panel, chart.dataset.tree, chart.dataset.title, chart.dataset.revision, chart.clientWidth, path);
    if (folder && chart.dataset.folder === path) showSelection(selection, folder);
    if (sharedSource && chart.dataset.folder === path) {
      for (const file of chart.querySelectorAll("g[data-source]")) {
        if (file.getAttribute("data-source") !== sharedSource) continue;
        file.classList.add("shared-highlight");
        showSelection(selection, file);
      }
    }
  }
  catch (error) { showError(String(error)); }
}
function showSelection(selection, tile) {
  const source = tile.getAttribute("data-source");
  const address = tile.getAttribute("data-address");
  selection.dataset.address = address ?? "";
  selection.dataset.source = source ?? "";
  selection.replaceChildren(
    h("div", {}, source ?? "Unresolved source"),
    h("div", {}, tile.getAttribute("aria-label") ?? "No details available"),
    h("div", { className: "selection-controls" },
      address ? h("button", { "data-action": "copy-selection" }, "Copy address") : null,
      source ? h("button", { "data-action": "reveal-selection" },
        /Mac/.test(globalThis.navigator?.platform ?? "") ? "Show in Finder" :
        /Win/.test(globalThis.navigator?.platform ?? "") ? "Show in Explorer" : "Show in file manager") : null,
      h("button", { "data-action": "close-selection" }, "Close")),
    h("div", { className: "selection-feedback" }),
  );
  selection.hidden = false;
}
root.addEventListener("click", activateTile);
root.addEventListener("keydown", activateTile);
root.addEventListener("focusout", hideTooltip);
window.addEventListener("scroll", hideTooltip, { capture: true, passive: true });
