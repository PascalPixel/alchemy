// `/client.js`.
//
// The browser client is deliberately plain JavaScript. It is embedded in the
// Rust binary and served verbatim, so starting the dashboard never requires a
// frontend build step. Tests guard the behaviours the dashboard relies on and
// reject source annotations in the checked-in browser bundle.

/// The browser client embedded in the native server.
const BUNDLE: &str = r###"// Browser client for the Rust dashboard server.
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
"###;

/// `bundledClient()`.
///
/// The bundle is compiled into the executable, so this returns its bytes
/// directly. The `Result` keeps the empty-client failure explicit rather than
/// allowing a silent empty 200 response.
pub fn bundled_client() -> Result<String, String> {
    if BUNDLE.is_empty() {
        return Err("dashboard client build failed".to_string());
    }
    Ok(BUNDLE.to_string())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn the_embedded_bundle_contains_the_dashboard_behaviours() {
        let client = bundled_client().unwrap();
        assert!(client.contains("EventSource"));
        assert!(client.contains("createElement"));
        assert!(client.contains("closest(\"g[aria-label]\")"));
        assert!(client.contains("hover-tooltip"));
        assert!(!client.contains("data-byte-leaf"));
        assert!(!client.contains("data-folder-depth"));
        assert!(!client.contains("legendbar"));
        assert!(!client.contains("titlebar"));
    }

    #[test]
    fn the_bundle_carries_no_source_annotations() {
        // If a future update accidentally embeds typed source instead of the
        // browser program, these markers would appear.
        assert!(!BUNDLE.contains("type Snapshot"));
        assert!(!BUNDLE.contains(": Promise<"));
        assert!(!BUNDLE.contains("as Snapshot"));
    }

    #[test]
    fn the_checked_in_bundle_is_a_real_program() {
        assert!(BUNDLE.len() > 4_000);
        let digest = coverage_map::sha1::sha1_hex(BUNDLE.as_bytes());
        assert_eq!(digest.len(), 40);
        assert_ne!(digest, coverage_map::sha1::sha1_hex(b"not the client"));
    }
}
