// Local Alchemy dashboard: tracked coverage maps.
// Run: bun tools/dashboard_server.ts
import { existsSync, statSync } from "node:fs";
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

const FONT = join(ROOT, "assets", "fonts", "weyard.otf");
const PAGE_FILES = [SOURCE, join(DASHBOARD, "index.html"), join(DASHBOARD, "styles.css"), join(DASHBOARD, "client.js")];

const svgFile = (tree: Tree) => join(ROOT, "assets", "readme", `gs1-en-${tree}.svg`);
const mtime = (file: string) => existsSync(file) ? statSync(file).mtimeMs : 0;
const pageVersion = () => PAGE_FILES.map(mtime).join(":");

function headers(contentType?: string, cacheControl = "no-store"): HeadersInit {
  return {
    ...(contentType ? { "content-type": contentType } : {}),
    "cache-control": cacheControl,
  };
}

function mtimes(): Record<string, number | string> {
  return {
    ...Object.fromEntries(Object.keys(TREES).map((tree) => [tree, mtime(svgFile(tree as Tree))])),
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
    if (path === "/config") return Response.json({ page: pageVersion(), trees: TREES }, { headers: headers() });
    if (path === "/mtimes") return Response.json(mtimes(), { headers: headers() });
    if (path === "/weyard.otf") return new Response(Bun.file(FONT), { headers: headers("font/otf", "public, max-age=300") });

    const tree = /^\/svg\/(core|overlays|assets)$/.exec(path)?.[1] as Tree | undefined;
    if (tree) return new Response(Bun.file(svgFile(tree)), { headers: headers("image/svg+xml") });

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
