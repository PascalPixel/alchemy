let config;
let lastVersion = "";

function h(tag, attributes = {}, ...children) {
  const node = document.createElement(tag);
  for (const [key, value] of Object.entries(attributes)) {
    if (key === "className") node.className = value;
    else if (value !== undefined && value !== null) node.setAttribute(key, String(value));
  }
  for (const child of children.flat(Infinity)) {
    if (child === undefined || child === null) continue;
    node.appendChild(child instanceof Node ? child : document.createTextNode(String(child)));
  }
  return node;
}

async function request(path, type) {
  const response = await fetch(path, { cache: "no-store" });
  if (!response.ok) throw new Error(`${path} returned ${response.status}`);
  return type === "json" ? response.json() : response.text();
}

function panel(kind, title, content) {
  return h("section", { className: `panel p-${kind}` },
    h("div", { className: "titlebar", title }, title), content);
}

function render(mtimes) {
  const graphs = Object.entries(config.trees).map(([tree, label]) => panel(tree, label,
    h("img", { className: "tree-image", src: `/svg/${tree}?v=${mtimes[tree]}`, alt: `${label} coverage graph` })));
  document.getElementById("root").replaceChildren(h("main", { className: "trees" }, graphs));
}

function showError(error) {
  document.querySelector(".error")?.remove();
  document.getElementById("root").prepend(h("div", { className: "error", role: "alert" },
    `Dashboard update failed: ${error.message}`));
}

async function tick() {
  try {
    const mtimes = await request("/mtimes", "json");
    if (mtimes.page !== config.page) {
      location.reload();
      return;
    }
    const version = JSON.stringify(mtimes);
    if (version !== lastVersion) {
      render(mtimes);
      lastVersion = version;
    }
  } catch (error) {
    showError(error);
  } finally {
    setTimeout(tick, 2000);
  }
}

async function start() {
  try {
    config = await request("/config", "json");
    await tick();
  } catch (error) {
    showError(error);
  }
}

start();
