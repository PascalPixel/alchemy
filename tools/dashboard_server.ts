// Live Alchemy dashboard. The browser is a projection of the current worktree:
// no generated coverage file has to be refreshed before it changes.
// Run: bun tools/dashboard_server.ts
import { existsSync, statSync, watch, type FSWatcher } from "node:fs";
import { dirname, join } from "node:path";
import {
  BOX_TREES,
  buildCoverageMap,
  renderBoxTrees,
  svgCacheVersion,
  workTree,
  type BoxTreeId,
  type CoverageMap,
} from "./coverage_map.ts";

const SOURCE = Bun.fileURLToPath(import.meta.url);
const ROOT = join(dirname(SOURCE), "..");
const DASHBOARD = join(dirname(SOURCE), "dashboard");
const CLIENT = join(DASHBOARD, "client.ts");
const STYLES = join(DASHBOARD, "styles.css");
const FONT = join(ROOT, "assets", "fonts", "weyard.otf");
const PORT = Number(Bun.env.ALCHEMY_DASHBOARD_PORT ?? 4649);
const COVERAGE_DIRECTORIES = ["asm", "assets", "metrics", "semantic", "src"] as const;
const COVERAGE_BUILD_FILES = [join(ROOT, "out", "full", "asm", "manifest.json")];
const RESTART_FILES = [SOURCE, join(dirname(SOURCE), "coverage_map.ts")];

const TREE_LABELS: Record<BoxTreeId, string> = {
  core: "Main image",
  overlays: "Code overlays",
  assets: "Data / assets",
};
const PAGE_FILES = [CLIENT, STYLES];
const encoder = new TextEncoder();

interface LiveCoverage {
  revision: string;
  generatedAt: string;
  map: CoverageMap;
  trees: Record<BoxTreeId, string>;
}

interface ClientBundle {
  version: string;
  source: string;
}

let coverage: LiveCoverage | undefined;
let scanError: string | undefined;
let scanning = false;
let scanQueued = false;
let scanTimer: ReturnType<typeof setTimeout> | undefined;
let clientBundle: ClientBundle | undefined;
const eventClients = new Set<ReadableStreamDefaultController<Uint8Array>>();

const mtime = (file: string): number => existsSync(file) ? statSync(file).mtimeMs : 0;
const pageVersion = (): string => PAGE_FILES.map(mtime).join(":");

function headers(contentType?: string, cacheControl = "no-store"): HeadersInit {
  return {
    ...(contentType ? { "content-type": contentType } : {}),
    "cache-control": cacheControl,
  };
}

function documentShell(): string {
  const version = encodeURIComponent(pageVersion());
  return "<!doctype html>" +
    '<meta charset="utf-8">' +
    '<meta name="viewport" content="width=device-width,initial-scale=1">' +
    "<title>Alchemy</title>" +
    `<link rel="stylesheet" href="/styles.css?v=${version}">` +
    `<script type="module" src="/client.js?v=${version}"></script>`;
}

async function bundledClient(): Promise<string> {
  const version = String(mtime(CLIENT));
  if (clientBundle?.version === version) return clientBundle.source;
  const result = await Bun.build({
    entrypoints: [CLIENT],
    target: "browser",
    format: "esm",
    minify: false,
    sourcemap: "none",
    write: false,
  });
  if (!result.success || result.outputs.length !== 1) {
    throw new Error(result.logs.map(String).join("\n") || "dashboard client build failed");
  }
  clientBundle = { version, source: await result.outputs[0].text() };
  return clientBundle.source;
}

function snapshot() {
  const map = coverage?.map;
  const exact = map?.categories.exact_c.bytes ?? 0;
  const semantic = map?.categories.semantic_c.bytes ?? 0;
  return {
    page: pageVersion(),
    revision: coverage?.revision ?? "starting",
    generatedAt: coverage?.generatedAt,
    scanning,
    error: scanError,
    trees: TREE_LABELS,
    summary: map && {
      executableBytes: map.executable_bytes,
      exactBytes: exact,
      exactPercent: map.categories.exact_c.percent_of_executable,
      semanticBytes: semantic,
      combinedBytes: exact + semantic,
    },
  };
}

function sendEvent(event: string, value: unknown): void {
  const message = encoder.encode(`event: ${event}\ndata: ${JSON.stringify(value)}\n\n`);
  for (const client of eventClients) {
    try {
      client.enqueue(message);
    } catch {
      eventClients.delete(client);
    }
  }
}

function notify(): void {
  sendEvent("update", snapshot());
}

function rebuildCoverage(): void {
  if (scanning) {
    scanQueued = true;
    return;
  }
  scanning = true;
  notify();
  try {
    const tree = workTree(ROOT);
    const map = buildCoverageMap({
      target: "gs1-en",
      exact: tree,
      semantic: tree,
      validateTrackedProgress: false,
    });
    const trees = renderBoxTrees(map, tree);
    const revision = BOX_TREES.map((name) => svgCacheVersion(trees[name])).join("-");
    coverage = { revision, generatedAt: new Date().toISOString(), map, trees };
    scanError = undefined;
  } catch (error) {
    scanError = error instanceof Error ? error.message : String(error);
  } finally {
    scanning = false;
    notify();
    if (scanQueued) {
      scanQueued = false;
      queueMicrotask(rebuildCoverage);
    }
  }
}

function scheduleCoverageRebuild(): void {
  if (scanTimer !== undefined) clearTimeout(scanTimer);
  scanTimer = setTimeout(() => {
    scanTimer = undefined;
    rebuildCoverage();
  }, 250);
}

export function affectsCoverage(path: string): boolean {
  const normalized = path.replaceAll("\\", "/").replace(/^\.\//, "");
  return /^(?:asm|assets|metrics|semantic|src)\//.test(normalized);
}

function watchRepository(): FSWatcher[] {
  const watchers = COVERAGE_DIRECTORIES
    .filter((directory) => existsSync(join(ROOT, directory)))
    .map((directory) => {
      const watcher = watch(join(ROOT, directory), { recursive: true }, scheduleCoverageRebuild);
      watcher.on("error", (error) => {
        scanError = `${directory} watcher: ${error.message}`;
        notify();
      });
      return watcher;
    });
  const pageWatcher = watch(DASHBOARD, { recursive: true }, (_event, filename) => {
    const path = String(filename ?? "").replaceAll("\\", "/");
    if (path === "client.ts" || path === "styles.css") {
      clientBundle = undefined;
      notify();
    }
  });
  pageWatcher.on("error", (error) => {
    scanError = `dashboard watcher: ${error.message}`;
    notify();
  });
  watchers.push(pageWatcher);
  // Exact and semantic ownership are tracked-tree projections. Orange retained
  // ownership comes from the last verified assembly manifest, so a completed
  // full build must wake the live dashboard too.
  for (const file of COVERAGE_BUILD_FILES.filter(existsSync)) {
    const watcher = watch(file, scheduleCoverageRebuild);
    watcher.on("error", (error) => {
      scanError = `build-evidence watcher: ${error.message}`;
      notify();
    });
    watchers.push(watcher);
  }
  return watchers;
}

function eventResponse(): Response {
  let held: ReadableStreamDefaultController<Uint8Array> | undefined;
  const stream = new ReadableStream<Uint8Array>({
    start(controller) {
      held = controller;
      eventClients.add(controller);
      controller.enqueue(encoder.encode(`event: update\ndata: ${JSON.stringify(snapshot())}\n\n`));
    },
    cancel() {
      if (held !== undefined) eventClients.delete(held);
    },
  });
  return new Response(stream, {
    headers: {
      ...headers("text/event-stream; charset=utf-8"),
      connection: "keep-alive",
    },
  });
}

async function selfTest(): Promise<void> {
  if (!affectsCoverage("assets/code/resource_373_overlay.s") ||
      !affectsCoverage("semantic/overlays/example.c") ||
      affectsCoverage("tools/dashboard/styles.css")) {
    throw new Error("dashboard coverage-path filter failed");
  }
  const tree = workTree(ROOT);
  const map = buildCoverageMap({
    target: "gs1-en",
    exact: tree,
    semantic: tree,
    validateTrackedProgress: false,
  });
  const trees = renderBoxTrees(map, tree);
  if (map.categories.exact_c.bytes <= 0 || BOX_TREES.some((name) => !trees[name].startsWith("<svg "))) {
    throw new Error("dashboard live coverage generation failed");
  }
  const client = await bundledClient();
  if (!client.includes("EventSource") || !client.includes("createElement")) {
    throw new Error("dashboard client is not bundled hyperscript with live events");
  }
  if (documentShell().includes('<div id="root"')) {
    throw new Error("dashboard document shell contains authored view markup");
  }
  console.log(`self-test=ok dashboard exact=${map.categories.exact_c.bytes}`);
}

async function main(): Promise<void> {
  rebuildCoverage();
  const server = Bun.serve({
    port: PORT,
    async fetch(request) {
      const path = new URL(request.url).pathname;
      if (path === "/") {
        return new Response(documentShell(), { headers: headers("text/html; charset=utf-8") });
      }
      if (path === "/styles.css") {
        return new Response(Bun.file(STYLES), { headers: headers("text/css; charset=utf-8") });
      }
      if (path === "/client.js") {
        try {
          return new Response(await bundledClient(), { headers: headers("text/javascript; charset=utf-8") });
        } catch (error) {
          return new Response(error instanceof Error ? error.message : String(error), {
            status: 500,
            headers: headers("text/plain; charset=utf-8"),
          });
        }
      }
      if (path === "/snapshot") return Response.json(snapshot(), { headers: headers() });
      if (path === "/events") return eventResponse();
      if (path === "/weyard.otf") {
        return new Response(Bun.file(FONT), { headers: headers("font/otf", "public, max-age=300") });
      }
      const tree = /^\/svg\/(core|overlays|assets)$/.exec(path)?.[1] as BoxTreeId | undefined;
      if (tree !== undefined) {
        const svg = coverage?.trees[tree];
        return svg === undefined
          ? new Response(scanError ?? "Coverage is still being read", { status: 503, headers: headers("text/plain; charset=utf-8") })
          : new Response(svg, { headers: headers("image/svg+xml; charset=utf-8") });
      }
      return new Response("Not found", { status: 404, headers: headers("text/plain; charset=utf-8") });
    },
  });
  const repositoryWatchers = watchRepository();
  const restartMtimes = RESTART_FILES.map(mtime);
  const restartMonitor = setInterval(() => {
    if (RESTART_FILES.some((file, index) => mtime(file) !== restartMtimes[index])) process.exit(0);
  }, 1_000);
  const keepalive = setInterval(() => {
    const message = encoder.encode(": keepalive\n\n");
    for (const client of eventClients) {
      try { client.enqueue(message); } catch { eventClients.delete(client); }
    }
  }, 5_000);
  const stop = (): void => {
    clearInterval(restartMonitor);
    clearInterval(keepalive);
    for (const watcher of repositoryWatchers) watcher.close();
    for (const client of eventClients) {
      try { client.close(); } catch {}
    }
    server.stop();
  };
  process.once("SIGINT", stop);
  process.once("SIGTERM", stop);
  console.log(`Alchemy dashboard on ${server.url} (live worktree coverage)`);
}

if (import.meta.main) {
  if (Bun.argv.includes("--self-test")) await selfTest();
  else await main();
}
