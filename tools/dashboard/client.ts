type Tree = "core" | "overlays" | "images" | "music";
type Child = Node | string | number | boolean | null | undefined | Child[];
type Attributes = Record<string, string | number | boolean | null | undefined>;

interface Snapshot {
  page: string;
  revision: string;
  generatedAt?: string;
  scanning: boolean;
  error?: string;
  trees: Record<Tree, string>;
  summary?: {
    executableBytes: number;
    exactBytes: number;
    exactPercent: number;
    semanticBytes: number;
    combinedBytes: number;
  };
}

function append(parent: Node, child: Child): void {
  if (Array.isArray(child)) {
    for (const item of child) append(parent, item);
  } else if (child instanceof Node) {
    parent.appendChild(child);
  } else if (child !== undefined && child !== null && child !== false) {
    parent.appendChild(document.createTextNode(String(child)));
  }
}

function h<K extends keyof HTMLElementTagNameMap>(
  tag: K,
  attributes: Attributes = {},
  ...children: Child[]
): HTMLElementTagNameMap[K] {
  const node = document.createElement(tag);
  for (const [key, value] of Object.entries(attributes)) {
    if (key === "className") node.className = String(value ?? "");
    else if (value !== undefined && value !== null && value !== false) {
      node.setAttribute(key, value === true ? "" : String(value));
    }
  }
  for (const child of children) append(node, child);
  return node;
}

const root = h("div", { id: "root" },
  h("div", { className: "loading", role: "status" }, "Reading the stones…"));
document.body.replaceChildren(root);
const tooltip = h("div", { className: "hover-tooltip", role: "tooltip", hidden: true });
document.body.append(tooltip);

let pageVersion = "";
let lastRevision = "";
let lastError = "";

async function requestSnapshot(): Promise<Snapshot> {
  const response = await fetch("/snapshot", { cache: "no-store" });
  if (!response.ok) throw new Error(`/snapshot returned ${response.status}`);
  return response.json() as Promise<Snapshot>;
}

function hideTooltip(): void {
  tooltip.hidden = true;
}

function showTooltip(event: PointerEvent): void {
  const target = event.target instanceof Element
    ? event.target.closest("[data-byte-leaf], [data-folder-depth]")
    : null;
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
  const top = below + bounds.height <= window.innerHeight - edge
    ? below
    : event.clientY - bounds.height - gap;
  tooltip.style.left = `${Math.max(edge, left)}px`;
  tooltip.style.top = `${Math.max(edge, top)}px`;
}

async function loadTree(
  section: HTMLElement,
  tree: Tree,
  title: string,
  revision: string,
): Promise<void> {
  const response = await fetch(`/svg/${tree}?v=${encodeURIComponent(revision)}`);
  if (!response.ok) throw new Error(`/svg/${tree} returned ${response.status}`);
  const parsed = new DOMParser().parseFromString(await response.text(), "image/svg+xml");
  const svg = parsed.documentElement;
  if (svg.localName !== "svg") throw new Error(`/svg/${tree} did not return an SVG`);
  svg.classList.add("tree-image");
  svg.setAttribute("aria-label", `${title} coverage graph`);
  svg.querySelectorAll("title").forEach((node) => node.remove());
  section.replaceChildren(svg);
}

function panel(tree: Tree, title: string, revision: string): HTMLElement {
  const section = h("section", { className: `panel p-${tree}` });
  void loadTree(section, tree, title, revision).catch((error) => {
    showError(error instanceof Error ? error.message : String(error));
  });
  return section;
}

function render(snapshot: Snapshot): void {
  hideTooltip();
  const trees = (Object.entries(snapshot.trees) as Array<[Tree, string]>)
    .map(([tree, title]) => panel(tree, title, snapshot.revision));
  const main = h("main", { className: "trees" }, trees);
  main.addEventListener("pointermove", showTooltip);
  main.addEventListener("pointerleave", hideTooltip);
  root.replaceChildren(main);
  lastRevision = snapshot.revision;
  lastError = "";
  if (snapshot.summary !== undefined) {
    document.title = `Alchemy — ${snapshot.summary.exactPercent.toFixed(2)}% exact C`;
  }
}

function showError(message: string): void {
  if (message === lastError) return;
  root.querySelector(".error")?.remove();
  root.prepend(h("div", { className: "error", role: "alert" },
    `Dashboard update failed: ${message}`));
  lastError = message;
}

function clearError(): void {
  root.querySelector(".error")?.remove();
  lastError = "";
}

function accept(snapshot: Snapshot): void {
  if (pageVersion !== "" && snapshot.page !== pageVersion) {
    location.reload();
    return;
  }
  pageVersion = snapshot.page;
  root.setAttribute("aria-busy", String(snapshot.scanning));
  if (snapshot.error !== undefined) showError(snapshot.error);
  else {
    clearError();
    if (snapshot.revision !== lastRevision) render(snapshot);
  }
}

async function refresh(): Promise<void> {
  try {
    accept(await requestSnapshot());
  } catch (error) {
    showError(error instanceof Error ? error.message : String(error));
  }
}

await refresh();

const events = new EventSource("/events");
events.addEventListener("update", (event) => {
  try {
    accept(JSON.parse((event as MessageEvent<string>).data) as Snapshot);
  } catch (error) {
    showError(error instanceof Error ? error.message : String(error));
  }
});

// EventSource reconnects on its own. This slow fallback also covers browsers or
// local proxies that suppress streaming responses.
setInterval(refresh, 5_000);
