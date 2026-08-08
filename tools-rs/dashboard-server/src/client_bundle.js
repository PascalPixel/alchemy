// Browser client for the Rust dashboard server.
function append(parent, child) {
  if (Array.isArray(child)) {
    for (const item of child)
      append(parent, item);
  } else if (child instanceof Node) {
    parent.appendChild(child);
  } else if (child !== undefined && child !== null && child !== false) {
    parent.appendChild(document.createTextNode(String(child)));
  }
}
function h(tag, attributes = {}, ...children) {
  const node = document.createElement(tag);
  for (const [key, value] of Object.entries(attributes)) {
    if (key === "className")
      node.className = String(value ?? "");
    else if (value !== undefined && value !== null && value !== false) {
      node.setAttribute(key, value === true ? "" : String(value));
    }
  }
  for (const child of children)
    append(node, child);
  return node;
}
var root = h("div", { id: "root" }, h("div", { className: "loading", role: "status" }, "Reading the stones…"));
document.body.replaceChildren(root);
var tooltip = h("div", { className: "hover-tooltip", role: "tooltip", hidden: true });
document.body.append(tooltip);
var pageVersion = "";
var lastRevision = "";
var lastError = "";
async function requestSnapshot() {
  const response = await fetch("/snapshot", { cache: "no-store" });
  if (!response.ok)
    throw new Error(`/snapshot returned ${response.status}`);
  return response.json();
}
function hideTooltip() {
  tooltip.hidden = true;
}
function showTooltip(event) {
  const target = event.target instanceof Element ? event.target.closest("g[aria-label]") : null;
  const label = target?.getAttribute("aria-label")?.trim();
  if (!label) {
    hideTooltip();
    return;
  }
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
  if (!response.ok)
    throw new Error(`/svg/${tree} returned ${response.status}`);
  const parsed = new DOMParser().parseFromString(await response.text(), "image/svg+xml");
  const svg = parsed.documentElement;
  if (svg.localName !== "svg")
    throw new Error(`/svg/${tree} did not return an SVG`);
  svg.classList.add("tree-image");
  svg.setAttribute("aria-label", `${title} coverage graph`);
  section.replaceChildren(svg);
}
function panel(tree, title, revision) {
  const section = h("section", { className: `panel p-${tree}` });
  loadTree(section, tree, title, revision).catch((error) => {
    showError(error instanceof Error ? error.message : String(error));
  });
  return section;
}
function render(snapshot) {
  hideTooltip();
  const trees = Object.entries(snapshot.trees).map(([tree, title]) => panel(tree, title, snapshot.revision));
  const main = h("main", { className: "trees" }, trees);
  root.replaceChildren(main);
  lastRevision = snapshot.revision;
  lastError = "";
  if (snapshot.summary !== undefined) {
    document.title = `Alchemy — ${snapshot.summary.exactPercent.toFixed(2)}% exact C`;
  }
}
function showError(message) {
  if (message === lastError)
    return;
  root.querySelector(".error")?.remove();
  root.prepend(h("div", { className: "error", role: "alert" }, `Dashboard update failed: ${message}`));
  lastError = message;
}
function clearError() {
  root.querySelector(".error")?.remove();
  lastError = "";
}
function accept(snapshot) {
  if (pageVersion !== "" && snapshot.page !== pageVersion) {
    location.reload();
    return;
  }
  pageVersion = snapshot.page;
  root.setAttribute("aria-busy", String(snapshot.scanning));
  if (snapshot.error !== undefined)
    showError(snapshot.error);
  else {
    clearError();
    if (snapshot.revision !== lastRevision)
      render(snapshot);
  }
}
async function refresh() {
  try {
    accept(await requestSnapshot());
  } catch (error) {
    showError(error instanceof Error ? error.message : String(error));
  }
}
await refresh();
var events = new EventSource("/events");
events.addEventListener("update", (event) => {
  try {
    accept(JSON.parse(event.data));
  } catch (error) {
    showError(error instanceof Error ? error.message : String(error));
  }
});
root.addEventListener("pointermove", showTooltip);
root.addEventListener("pointerleave", hideTooltip);
