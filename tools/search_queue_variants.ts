#!/usr/bin/env bun
// Apply the deterministic declaration/type variant library to queue-ranked
// C-debt candidates and retain only byte-identical matches below ignored out/.
import { mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { ROM_BASE, verifyCandidate } from "./match_m2c.ts";
import { variants } from "./search_variants.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const OUT = join(ROOT, "out/decomp/queue-variants");

interface Options { limit: number; variants: number; jobs: number }
interface Item { stem: string; candidate: string; diagnosis: { expected_size: number } }

function optionsOf(arguments_: string[]): Options {
  const options = { limit: 50, variants: 128, jobs: Math.min(18, navigator.hardwareConcurrency || 1) };
  for (let index = 0; index < arguments_.length; index++) {
    const argument = arguments_[index];
    if (argument === "--limit") options.limit = Number(arguments_[++index]);
    else if (argument === "--variants") options.variants = Number(arguments_[++index]);
    else if (argument === "--jobs") options.jobs = Number(arguments_[++index]);
    else if (argument === "-h" || argument === "--help") {
      console.log("usage: search_queue_variants.ts [--limit N] [--variants N] [--jobs N]");
      process.exit(0);
    } else throw new Error(`unrecognized argument: ${argument}`);
  }
  for (const value of Object.values(options)) {
    if (!Number.isInteger(value) || value < 1) throw new Error("numeric options must be positive integers");
  }
  return options;
}

async function main(): Promise<void> {
  const options = optionsOf(Bun.argv.slice(2));
  const queue = JSON.parse(readFileSync(join(ROOT, "out/decomp/queue.json"), "utf8")) as { items: Item[] };
  const rom = readFileSync(join(ROOT, "gs1-en.gba"));
  const items = queue.items.slice(0, options.limit);
  const tasks = items.flatMap((item) => {
    const source = readFileSync(join(ROOT, item.candidate), "utf8");
    return variants(source, options.variants).map(([label, body], index) => ({ item, label, body, index }));
  });
  mkdirSync(OUT, { recursive: true });
  const matches: Array<{ stem: string; label: string; source: string }> = [];
  let cursor = 0;
  let compiled = 0;
  async function worker(): Promise<void> {
    while (true) {
      const taskIndex = cursor++;
      if (taskIndex >= tasks.length) return;
      const { item, label, body, index } = tasks[taskIndex];
      const directory = join(OUT, "scratch", item.stem, index.toString());
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
  const report = { format: 1, targets: items.length, variants: tasks.length, compiled, matches };
  writeFileSync(join(OUT, "report.json"), JSON.stringify(report, null, 2) + "\n");
  console.log(`targets=${items.length} variants=${tasks.length} compiled=${compiled} matches=${matches.length}`);
}

if (import.meta.main) await main();
