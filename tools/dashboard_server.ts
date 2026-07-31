// Local Alchemy dashboard: coverage maps, the shared kanban, and team chat.
// Run: bun tools/dashboard_server.ts
import { existsSync, readFileSync, statSync } from "node:fs";
import { dirname, join } from "node:path";

const SOURCE = Bun.fileURLToPath(import.meta.url);
const ROOT = join(dirname(SOURCE), "..");
const DASHBOARD = join(dirname(SOURCE), "dashboard");
const SOURCE_MTIME = statSync(SOURCE).mtimeMs;
const PORT = 4649;

const TREES = {
  core: "Core ROM",
  overlays: "Overlays",
  assets: "Assets",
} as const;
type Tree = keyof typeof TREES;

const CAST = {
  venus: { name: "Isaac", portrait: "resource_f0_images_image_00.png" },
  mars: { name: "Garet", portrait: "resource_f0_images_image_01.png" },
  jupiter: { name: "Ivan", portrait: "resource_f0_images_image_02.png" },
  mercury: { name: "Mia", portrait: "resource_f0_images_image_03.png" },
  vale: { name: "Kraden", portrait: "resource_f0_images_image_12.png" },
  "wise-one": { name: "The Wise One", portrait: "battle_characters_chr_0fc_koma_003.png" },
} as const;

const CHAT = "/tmp/ALCHEMY_GROUP_CHAT.csv";
const KANBAN = "/tmp/ALCHEMY_KANBAN.md";
const FONT = join(ROOT, "assets", "fonts", "weyard.otf");
const PAGE_FILES = [SOURCE, join(DASHBOARD, "index.html"), join(DASHBOARD, "styles.css"), join(DASHBOARD, "client.js")];

const svgFile = (tree: Tree) => join(ROOT, "assets", "readme", `gs1-en-${tree}.svg`);
const portraitFile = (handle: keyof typeof CAST) => join(ROOT, "assets", "graphics", CAST[handle].portrait);
const mtime = (file: string) => existsSync(file) ? statSync(file).mtimeMs : 0;
const pageVersion = () => PAGE_FILES.map(mtime).join(":");
const clientCast = () => Object.fromEntries(Object.entries(CAST).map(([handle, member]) => [handle, member.name]));

function headers(contentType?: string, cacheControl = "no-store"): HeadersInit {
  return {
    ...(contentType ? { "content-type": contentType } : {}),
    "cache-control": cacheControl,
  };
}

function readChatCsv(): { time: string; who: string; text: string }[] {
  if (!existsSync(CHAT)) return [];
  const rows: string[][] = [];
  let field = "", row: string[] = [], quoted = false;
  const raw = readFileSync(CHAT, "utf8");

  for (let index = 0; index < raw.length; index++) {
    const character = raw[index];
    if (quoted) {
      if (character === '"') {
        if (raw[index + 1] === '"') { field += '"'; index++; }
        else quoted = false;
      } else field += character;
    } else if (character === '"') quoted = true;
    else if (character === ",") { row.push(field); field = ""; }
    else if (character === "\n") {
      row.push(field); field = "";
      if (row.some(Boolean)) rows.push(row);
      row = [];
    } else if (character !== "\r") field += character;
  }

  if (field !== "" || row.length) {
    row.push(field);
    if (row.some(Boolean)) rows.push(row);
  }
  return rows.slice(1).map(([time, who, text]) => ({
    time: time ?? "",
    who: (who ?? "").toLowerCase(),
    text: text ?? "",
  }));
}

function mtimes(): Record<string, number | string> {
  return {
    ...Object.fromEntries(Object.keys(TREES).map((tree) => [tree, mtime(svgFile(tree as Tree))])),
    kanban: mtime(KANBAN),
    chat: mtime(CHAT),
    avatars: Object.keys(CAST).reduce((sum, handle) => sum + mtime(portraitFile(handle as keyof typeof CAST)), 0),
    page: pageVersion(),
  };
}

const server = Bun.serve({
  port: PORT,
  fetch(request) {
    const path = new URL(request.url).pathname;

    if (path === "/") return new Response(Bun.file(join(DASHBOARD, "index.html")), { headers: headers("text/html; charset=utf-8") });
    if (path === "/styles.css") return new Response(Bun.file(join(DASHBOARD, "styles.css")), { headers: headers("text/css; charset=utf-8") });
    if (path === "/client.js") return new Response(Bun.file(join(DASHBOARD, "client.js")), { headers: headers("text/javascript; charset=utf-8") });
    if (path === "/config") return Response.json({ page: pageVersion(), trees: TREES, cast: clientCast() }, { headers: headers() });
    if (path === "/chat") return Response.json(readChatCsv(), { headers: headers() });
    if (path === "/kanban") {
      const body = existsSync(KANBAN) ? Bun.file(KANBAN) : "# Work queue\n\nNo kanban is available.";
      return new Response(body, { headers: headers("text/plain; charset=utf-8") });
    }
    if (path === "/mtimes") return Response.json(mtimes(), { headers: headers() });
    if (path === "/weyard.otf") return new Response(Bun.file(FONT), { headers: headers("font/otf", "public, max-age=300") });

    const tree = /^\/svg\/(core|overlays|assets)$/.exec(path)?.[1] as Tree | undefined;
    if (tree) return new Response(Bun.file(svgFile(tree)), { headers: headers("image/svg+xml") });

    const handle = /^\/avatar\/([a-z-]+)\.png$/.exec(path)?.[1] as keyof typeof CAST | undefined;
    if (handle && CAST[handle]) return new Response(Bun.file(portraitFile(handle)), { headers: headers("image/png") });

    return new Response("Not found", { status: 404, headers: headers("text/plain; charset=utf-8") });
  },
});

// WatchPaths starts a stopped launchd job but does not replace a running one.
setInterval(() => {
  try {
    if (statSync(SOURCE).mtimeMs !== SOURCE_MTIME) process.exit(0);
  } catch {
    process.exit(0);
  }
}, 1000);

console.log(`Alchemy dashboard on ${server.url}`);
