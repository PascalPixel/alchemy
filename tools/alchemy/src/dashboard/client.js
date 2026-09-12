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
async function copyTileAddress(event) {
  if (event.type === "keydown" && event.key !== "Enter" && event.key !== " ") return;
  const target = event.target instanceof Element ? event.target.closest("g[data-address]") : null;
  const address = target?.getAttribute("data-address");
  if (!address) return;
  event.preventDefault();
  try {
    await navigator.clipboard.writeText(address);
    showTooltip(event, `Copied ${address}`);
  } catch {
    showTooltip(event, `Could not copy ${address}`);
  }
}
async function loadTree(section, tree, title, revision, width = 540) {
  const response = await fetch(`/svg/${tree}/${width}?v=${encodeURIComponent(revision)}`);
  if (!response.ok) throw new Error(`/svg/${tree} returned ${response.status}`);
  const parsed = new DOMParser().parseFromString(await response.text(), "image/svg+xml");
  const svg = parsed.documentElement;
  if (svg.localName !== "svg") throw new Error(`/svg/${tree} did not return an SVG`);
  // Keep accessible labels, but let only the dashboard's JS tooltip render.
  svg.querySelectorAll("title").forEach((title) => title.remove());
  svg.querySelectorAll("g[data-address]").forEach((tile) => {
    tile.setAttribute("role", tile.getAttribute("data-node") === "container" ? "group" : "button");
    tile.setAttribute("tabindex", "0");
  });
  svg.classList.add("tree-image");
  svg.setAttribute("role", "group");
  svg.setAttribute("aria-label", `${title} coverage graph`);
  const chart = section.querySelector(".chart");
  if (chart?.dataset?.width && chart.dataset.width !== String(width)) return;
  chart?.replaceChildren(svg);
}
function panel(tree, title, revision) {
  const chart = h("div", { className: "chart", "data-tree": tree, "data-title": title, "data-revision": revision }, h("div", { className: "chart-loading" }, "Reading…"));
  const section = h("section", { className: `panel p-${tree}` },
    chart,
  );
  treeObserver.observe(chart);
  return section;
}
// Reflow at CSS-pixel dimensions; never scale the 16px bitmap font with a viewBox.
const treeObserver = new ResizeObserver(entries => {
  for (const { target: chart } of entries) {
    const width = chart.clientWidth;
    if (!width || width === Number(chart.dataset.width)) continue;
    chart.dataset.width = String(width);
    loadTree(chart.parentElement, chart.dataset.tree, chart.dataset.title, chart.dataset.revision, width)
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
    ? `Alchemy — 12 targets · ${percent(snapshot.summary.donePercent)} GS1 EN done`
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
root.addEventListener("click", copyTileAddress);
root.addEventListener("keydown", copyTileAddress);
root.addEventListener("focusout", hideTooltip);
window.addEventListener("scroll", hideTooltip, { capture: true, passive: true });
