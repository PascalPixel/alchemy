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

function timeAgo(value) {
  const date = new Date(value);
  if (Number.isNaN(date.getTime())) return "unstamped";
  const seconds = Math.max(0, (Date.now() - date.getTime()) / 1000);
  if (seconds < 60) return "just now";
  if (seconds < 3600) return `${Math.floor(seconds / 60)}m ago`;
  if (seconds < 86400) return `${Math.floor(seconds / 3600)}h ago`;
  return `${Math.floor(seconds / 86400)}d ago`;
}

function parseKanban(text) {
  const lines = text.split("\n");
  const heading = (lines.find((line) => line.startsWith("#")) || "Work queue").replace(/^#+\s*/, "");
  const rows = lines
    .filter((line) => line.trim().startsWith("|") && !/^\|[\s:|-]+\|$/.test(line.trim()))
    .map((line) => line.split("|").slice(1, -1).map((cell) => cell.trim()));
  if (rows.length === 0) return { heading, columns: [] };
  return {
    heading,
    columns: rows[0].map((title, index) => ({
      title,
      cards: rows.slice(1).map((row) => row[index] || "").filter((card) => card && card !== "—" && card !== "-"),
    })),
  };
}

function portrait(handle, label) {
  return h("img", {
    className: "sprite",
    src: `/avatar/${handle}.png?v=${window.avatarVersion || 0}`,
    alt: label,
    loading: "lazy",
  });
}

function postit(text) {
  const handles = [...text.matchAll(/@([a-z-]+)/g)]
    .map((match) => match[1])
    .filter((handle) => config.cast[handle]);
  const clean = text
    .replace(/`@[a-z-]+`/g, "")
    .replace(/@[a-z-]+/g, "")
    .replace(/`/g, "")
    .replace(/\s{2,}/g, " ")
    .trim();
  const card = h("article", { className: "postit" }, clean);
  if (handles.length) {
    card.appendChild(h("div", { className: "owners" },
      [...new Set(handles)].map((handle) => portrait(handle, config.cast[handle]))));
  }
  return card;
}

function kanban(board) {
  const columns = board.columns.map((column) => h("section", { className: "column" },
    h("div", { className: "column-name", title: column.title }, column.title),
    h("div", { className: "card-stack" }, column.cards.map(postit))));
  return h("div", { className: "board" }, h("div", { className: "columns" }, columns));
}

function message(item) {
  const handle = item.who || "unknown";
  const name = config.cast[handle] || handle;
  const stamp = new Date(item.time).getTime();
  const valid = !Number.isNaN(stamp) && stamp < Date.now() + 120000;
  return h("article", { className: "message" },
    h("div", { className: "avatar" }, portrait(handle, name)),
    h("div", {},
      h("div", { className: "message-head" },
        h("strong", {}, name),
        h("span", { className: "handle" }, ` · @${handle}`),
        h("span", { className: "time", "data-time": valid ? item.time : "", title: item.time || "" },
          valid ? timeAgo(item.time) : "unstamped")),
      h("div", { className: "message-body" }, item.text || "")));
}

function panel(kind, title, content) {
  return h("section", { className: `panel p-${kind}` },
    h("div", { className: "titlebar", title }, title), content);
}

function render(mtimes, board, messages) {
  window.avatarVersion = mtimes.avatars || 0;
  const graphs = Object.entries(config.trees).map(([tree, label]) => panel(tree, label,
    h("img", { className: "tree-image", src: `/svg/${tree}?v=${mtimes[tree]}`, alt: `${label} coverage graph` })));
  const chat = h("div", { className: "chat", "aria-live": "polite" }, messages.map(message));
  document.getElementById("root").replaceChildren(
    h("div", { className: "trees" }, graphs),
    h("main", { className: "main" },
      panel("board", board.heading, kanban(board)),
      panel("chat", "Team chat · #alchemy", chat)),
  );
  chat.scrollTop = chat.scrollHeight;
}

function showError(error) {
  document.querySelector(".error")?.remove();
  document.getElementById("root").prepend(h("div", { className: "error", role: "alert" },
    `Dashboard update failed: ${error.message}`));
}

function refreshTimes() {
  document.querySelectorAll("[data-time]").forEach((node) => {
    const value = node.getAttribute("data-time");
    if (value) node.textContent = timeAgo(value);
  });
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
      const [board, chat] = await Promise.all([
        request("/kanban", "text").then(parseKanban),
        request("/chat", "json"),
      ]);
      render(mtimes, board, chat);
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
    setInterval(refreshTimes, 30000);
  } catch (error) {
    showError(error);
  }
}

start();
