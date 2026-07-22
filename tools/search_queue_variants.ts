#!/usr/bin/env bun
// Tool role: entrypoint; invoked by PLAYBOOK.md.
// Apply the deterministic declaration/type variant library to queue-ranked
// C-debt candidates and retain only byte-identical matches below ignored out/.
import { existsSync, mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { dirname, isAbsolute, join } from "node:path";
import { ROM_BASE, verifyCandidate } from "./match_m2c.ts";
import { variants } from "./search_variants.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const OUT = join(ROOT, "out/decomp/queue-variants");

interface Options {
  limit: number;
  variants: number;
  jobs: number;
  queue: string;
  rom: string;
  seeds: string | null;
  targets: Set<string> | null;
}
interface Item { stem: string; candidate: string; diagnosis: { expected_size: number } }
interface SeedManifest { format: number; seeds: Record<string, string | string[]> }

function optionsOf(arguments_: string[]): Options {
  const options: Options = {
    limit: 50,
    variants: 128,
    jobs: Math.min(18, navigator.hardwareConcurrency || 1),
    queue: join(ROOT, "out/decomp/queue.json"),
    rom: join(ROOT, "roms", "gs1-en.gba"),
    seeds: join(ROOT, "work/decomp-seeds.json"),
    targets: null,
  };
  for (let index = 0; index < arguments_.length; index++) {
    const argument = arguments_[index];
    if (argument === "--limit") options.limit = Number(arguments_[++index]);
    else if (argument === "--variants") options.variants = Number(arguments_[++index]);
    else if (argument === "--jobs") options.jobs = Number(arguments_[++index]);
    else if (argument === "--queue") options.queue = arguments_[++index];
    else if (argument === "--rom") options.rom = arguments_[++index];
    else if (argument === "--seeds") options.seeds = arguments_[++index];
    else if (argument === "--no-seeds") options.seeds = null;
    else if (argument === "--targets") {
      options.targets = new Set(arguments_[++index].split(",").filter(Boolean));
    }
    else if (argument === "-h" || argument === "--help") {
      console.log("usage: search_queue_variants.ts [--limit N] [--variants N] [--jobs N] [--queue FILE] [--rom FILE] [--seeds FILE|--no-seeds] [--targets STEM,...]");
      process.exit(0);
    } else throw new Error(`unrecognized argument: ${argument}`);
  }
  for (const value of [options.limit, options.variants, options.jobs]) {
    if (!Number.isInteger(value) || value < 1) throw new Error("numeric options must be positive integers");
  }
  return options;
}

async function main(): Promise<void> {
  const options = optionsOf(Bun.argv.slice(2));
  const queue = JSON.parse(readFileSync(options.queue, "utf8")) as { items: Item[] };
  const seedManifest = options.seeds !== null && existsSync(options.seeds)
    ? JSON.parse(readFileSync(options.seeds, "utf8")) as SeedManifest
    : null;
  if (seedManifest !== null && seedManifest.format !== 1) throw new Error("unsupported seed manifest format");
  const rom = readFileSync(options.rom);
  const items = queue.items
    .filter((item) => options.targets === null || options.targets.has(item.stem))
    .slice(0, options.limit);
  let seedSources = 0;
  const tasks = items.flatMap((item) => {
    const configured = seedManifest?.seeds[item.stem] ?? [];
    const seeds = Array.isArray(configured) ? configured : [configured];
    const paths = [item.candidate, ...seeds];
    const seen = new Set<string>();
    return paths.flatMap((path, sourceIndex) => {
      const resolved = isAbsolute(path) ? path : join(ROOT, path);
      if (!existsSync(resolved)) throw new Error(`missing candidate seed: ${path}`);
      const source = readFileSync(resolved, "utf8");
      const digest = new Bun.CryptoHasher("sha256").update(source).digest("hex");
      if (seen.has(digest)) return [];
      seen.add(digest);
      if (sourceIndex > 0) seedSources++;
      const origin = sourceIndex === 0 ? "queue" : `seed${sourceIndex}`;
      return variants(source, options.variants).map(([label, body], index) => ({
        item, label: `${origin}:${label}`, body, index, sourceIndex,
      }));
    });
  });
  mkdirSync(OUT, { recursive: true });
  const matches: Array<{ stem: string; label: string; source: string }> = [];
  let cursor = 0;
  let compiled = 0;
  async function worker(): Promise<void> {
    while (true) {
      const taskIndex = cursor++;
      if (taskIndex >= tasks.length) return;
      const { item, label, body, index, sourceIndex } = tasks[taskIndex];
      const directory = join(OUT, "scratch", item.stem, sourceIndex.toString(), index.toString());
      mkdirSync(directory, { recursive: true });
      const source = join(directory, `${item.stem}.c`);
      const work = join(directory, "work");
      mkdirSync(work, { recursive: true });
      writeFileSync(source, body);
      try {
        const verification = await verifyCandidate(source, rom, work);
        compiled++;
        const address = Number.parseInt(item.stem, 16);
        const expected = rom.subarray(address - ROM_BASE, address - ROM_BASE + item.diagnosis.expected_size);
        if (verification.size === expected.length && verification.actual.equals(expected)) {
          const destination = join(OUT, "matches", `${item.stem}.c`);
          mkdirSync(dirname(destination), { recursive: true });
          writeFileSync(destination, body);
          matches.push({ stem: item.stem, label, source: destination });
          console.log(`exact ${item.stem} label=${label}`);
        }
      } catch {}
    }
  }
  await Promise.all(Array.from({ length: Math.min(options.jobs, tasks.length) }, worker));
  const report = { format: 1, targets: items.length, seed_sources: seedSources, variants: tasks.length, compiled, matches };
  writeFileSync(join(OUT, "report.json"), JSON.stringify(report, null, 2) + "\n");
  console.log(`targets=${items.length} seed_sources=${seedSources} variants=${tasks.length} compiled=${compiled} matches=${matches.length}`);
}

if (import.meta.main) await main();
