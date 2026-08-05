#!/usr/bin/env bun
// alchemist.ts only ever WRITES to the real source on an "exact" verdict; an
// "improved" verdict's saved variant sits in a scratch work/ directory and
// is meant for a human to inspect as a lead, per its own `next` field. This
// session found real, repeated value in just applying that saved variant
// directly and re-running alchemist -- it keeps hill-climbing (a fresh
// "improved" with a further move) until it either reaches "exact" or
// "exhausted". Every step is gated by alchemist's own byte-diff, so this
// never installs anything that isn't a genuine measured improvement.
import { execFileSync } from "node:child_process";
import { copyFileSync, readdirSync, unlinkSync } from "node:fs";
import { basename, dirname, join } from "node:path";
import { resolveSpan } from "./alchemist.ts";

const ROOT = dirname(Bun.fileURLToPath(import.meta.url).replace(/^file:\/\//, "")).replace(/\/tools$/, "");
const OVERLAY_BASE = 0x02000000;

interface AlchemistVerdict {
  verdict: "exact" | "improved" | "refused" | "exhausted" | "unnecessary";
  baseline_differing_bytes: number;
  differing_bytes: number;
  saved: string | null;
}

function runAlchemist(id: string, write: boolean): AlchemistVerdict | null {
  try {
    const args = ["tools/alchemist.ts", id, "--json"];
    if (write) args.push("--write");
    const output = execFileSync("bun", args, { cwd: ROOT, encoding: "utf8", timeout: 90_000 });
    return JSON.parse(output) as AlchemistVerdict;
  } catch {
    return null;
  }
}

function runAdopt(id: string, source: string, span: number): boolean {
  try {
    execFileSync("bun", ["tools/overlay_adopt.ts", id, "--source", source, "--span", String(span), "--apply"], {
      cwd: ROOT, encoding: "utf8", timeout: 60_000,
    });
    return true;
  } catch {
    return false;
  }
}

async function main(): Promise<void> {
  const limitArg = Bun.argv.find((arg) => arg.startsWith("--limit="));
  const limit = limitArg !== undefined ? Number.parseInt(limitArg.split("=")[1], 10) : Infinity;
  const semanticDir = join(ROOT, "semantic");
  const files = readdirSync(semanticDir)
    .filter((name) => /^resource_[0-9a-f]+_c_[0-9a-f]{8}\.c$/i.test(name))
    .slice(0, limit);

  let closed = 0;
  let improved = 0;
  let unchanged = 0;

  for (const name of files) {
    const path = join(semanticDir, name);
    const match = /^(resource_[0-9a-f]+)_c_([0-9a-f]{8})$/i.exec(basename(name, ".c"));
    if (match === null) continue;
    const overlay = match[1];
    const address = Number.parseInt(match[2], 16);
    const id = `${overlay}:${(address - OVERLAY_BASE).toString(16)}`;

    const initial = runAlchemist(id, false);
    if (initial === null || initial.baseline_differing_bytes === 0) continue;
    const startBytes = initial.baseline_differing_bytes;

    let steps = 0;
    let lastBytes = startBytes;
    while (steps < 15) {
      const result = runAlchemist(id, true);
      if (result === null) break;
      if (result.verdict === "exact" || result.verdict === "unnecessary") {
        const span = resolveSpan(overlay, address - OVERLAY_BASE);
        if (span !== undefined && runAdopt(id, path, span)) {
          unlinkSync(path); // overlay_adopt.ts --apply does not do this itself
          closed++;
          console.log(`${id}: CLOSED (was ${startBytes} differing bytes)`);
        }
        break;
      }
      if (result.verdict !== "improved" || result.saved === null) break;
      copyFileSync(result.saved, path);
      lastBytes = result.differing_bytes;
      steps++;
    }
    if (lastBytes < startBytes) {
      console.log(`${id}: ${startBytes} -> ${lastBytes} differing bytes`);
      improved++;
    } else {
      unchanged++;
    }
  }

  console.log(`closed=${closed} improved=${improved} unchanged=${unchanged} total=${files.length}`);
}

if (import.meta.main) await main();
