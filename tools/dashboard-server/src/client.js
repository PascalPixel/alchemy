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
const number = new Intl.NumberFormat("en");
function bytes(value) { return number.format(Math.round(value ?? 0)); }
function percent(value) { return `${Number(value ?? 0).toFixed(2)}%`; }
function upper(value) { return String(value ?? "").toUpperCase(); }

async function requestSnapshot() {
  const response = await fetch("/snapshot", { cache: "no-store" });
  if (!response.ok) throw new Error(`/snapshot returned ${response.status}`);
  return response.json();
}
function hideTooltip() { tooltip.hidden = true; }
function showTooltip(event) {
  const target = event.target instanceof Element ? event.target.closest("g[aria-label]") : null;
  const label = target?.getAttribute("aria-label")?.trim();
  if (!label) { hideTooltip(); return; }
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
  if (!response.ok) throw new Error(`/svg/${tree} returned ${response.status}`);
  const parsed = new DOMParser().parseFromString(await response.text(), "image/svg+xml");
  const svg = parsed.documentElement;
  if (svg.localName !== "svg") throw new Error(`/svg/${tree} did not return an SVG`);
  svg.classList.add("tree-image");
  svg.setAttribute("aria-label", `${title} coverage graph`);
  section.querySelector(".chart")?.replaceChildren(svg);
}
function panel(tree, title, revision) {
  const chart = h("div", { className: "chart" }, h("div", { className: "chart-loading" }, "Reading…"));
  const section = h("section", { className: `panel p-${tree}` },
    h("header", { className: "panel-title" }, h("span", {}, title), h("span", {}, tree === "core" || tree === "overlays" ? "EN build" : "shared assets")),
    chart,
  );
  loadTree(section, tree, title, revision).catch((error) => showError(error instanceof Error ? error.message : String(error)));
  return section;
}
function metric(label, value, detail, tone = "") {
  return h("article", { className: `metric ${tone}` },
    h("div", { className: "metric-label" }, label),
    h("div", { className: "metric-value" }, value),
    h("div", { className: "metric-detail" }, detail),
  );
}
function edition(code, role, tone = "") {
  return h("div", { className: `edition ${tone}` },
    h("strong", {}, upper(code)), h("span", {}, role));
}
function historicalProduct(name, editions, fullTarget) {
  const game = name.toLowerCase();
  return h("section", { className: "product" },
    h("div", { className: "product-name" }, name),
    h("div", { className: "edition-row" },
      ...editions.split(" · ").map((code) => {
        const target = `${game}-${code}`;
        if (target === fullTarget) return edition(code, "full build", "full");
        if (code === "ja") return edition(code, "canonical", "base");
        return edition(code, "compile-only");
      }),
    ),
  );
}
function render(snapshot) {
  hideTooltip();
  const summary = snapshot.summary;
  const project = snapshot.project;
  const header = h("header", { className: "masthead" },
    h("div", { className: "identity" },
      h("div", { className: "kicker" }, "ALCHEMY · RECONSTRUCTION LEDGER"),
      h("h1", {}, project.title),
      h("p", {}, "Two shared historical source trees, twelve edition targets, and one separate reintegration product. Japanese is canonical within each game; every derived ROM must still be proved independently."),
    ),
    h("div", { className: "products", "aria-label": "Historical and integration products" },
      historicalProduct("GS1", project.gs1, project.fullTarget),
      historicalProduct("GS2", project.gs2, project.fullTarget),
      h("section", { className: "product integration" },
        h("div", { className: "product-name" }, project.integration),
        h("div", { className: "integration-copy" }, "GS1 + GS2 reintegration · no reference ROM"),
      ),
    ),
  );
  const metrics = h("section", { className: "metrics", "aria-label": "Project metrics" },
    metric("EN DONE", percent(summary.donePercent), `${bytes(summary.doneBytes)} exact or permanent bytes`, "done"),
    metric("EN exact C", percent(summary.exactPercent), `${bytes(summary.exactBytes)} linked bytes`, "exact"),
    metric("Tracked C", percent(summary.trackedPercent), `${bytes(summary.trackedBytes)} visible bytes · never counted as DONE`, "tracked"),
    metric("GS1 corpus ×6", `${bytes(summary.correspondenceMatched)} / ${bytes(summary.correspondenceTotal)}`, `${bytes(summary.correspondenceShared)} shared-core · ${bytes(summary.correspondenceRegional)} regional candidates · ${bytes(summary.correspondenceUnresolved)} unresolved`, "cross"),
    metric("Canonical JA", bytes(summary.gs1JaSources + summary.gs2JaSources), `GS1 ${bytes(summary.gs1JaSources)} · GS2 ${bytes(summary.gs2JaSources)} tracked source owners`, "base"),
    metric("Historical targets", bytes(summary.historicalTargets), `${bytes(summary.fullTargets)} full · ${bytes(summary.compileOnlyTargets)} compile-only · Alchemy separate`, "derived"),
  );
  const legend = h("div", { className: "legend" },
    h("span", {}, h("i", { className: "swatch exact" }), "Exact C"),
    h("span", {}, h("i", { className: "swatch tracked" }), "Tracked C"),
    h("span", {}, h("i", { className: "swatch retained" }), "Permanent ASM"),
    h("span", {}, h("i", { className: "swatch open" }), "Open"),
    h("span", { id: "scan-state", className: "scan-state" }, snapshot.scanning ? "Scanning…" : "Live"),
  );
  const trees = Object.entries(snapshot.trees).map(([tree, title]) => panel(tree, title, snapshot.revision));
  root.replaceChildren(h("div", { className: "shell" }, header, metrics, legend, h("main", { className: "trees" }, trees)));
  lastRevision = snapshot.revision;
  lastError = "";
  document.title = `Alchemy — 12 targets · ${percent(summary.donePercent)} GS1 EN done`;
}
function showError(message) {
  if (message === lastError) return;
  root.querySelector(".error")?.remove();
  root.prepend(h("div", { className: "error", role: "alert" }, `Dashboard update failed: ${message}`));
  lastError = message;
}
function clearError() { root.querySelector(".error")?.remove(); lastError = ""; }
function accept(snapshot) {
  if (pageVersion !== "" && snapshot.page !== pageVersion) { location.reload(); return; }
  pageVersion = snapshot.page;
  root.setAttribute("aria-busy", String(snapshot.scanning));
  if (snapshot.error !== undefined) showError(snapshot.error);
  else {
    clearError();
    if (snapshot.revision !== lastRevision) render(snapshot);
    else {
      const state = root.querySelector("#scan-state");
      if (state) state.textContent = snapshot.scanning ? "Scanning…" : "Live";
    }
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
