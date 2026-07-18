import { mkdirSync, readFileSync, readdirSync, writeFileSync } from "node:fs";
import { basename, join, resolve } from "node:path";
import { Discovery } from "./discover.ts";
import { emitDiscovery } from "./emit_function.ts";
import { M2C } from "./alchemy_gcc.ts";

const ROOT = resolve(import.meta.dir, "..");

interface Options {
  rom: string;
  maxBytes: number;
  limit?: number;
  jobs: number;
  context?: string;
  output: string;
}

function usage(): never {
  console.log("usage: decompile_batch.ts [-h] [--max-bytes MAX_BYTES] [--limit LIMIT] [--jobs JOBS] [--context CONTEXT] [-o OUTPUT] rom");
  process.exit(0);
}

function options(argv: string[]): Options {
  if (argv.includes("-h") || argv.includes("--help")) usage();
  const result: Options = { rom: "", maxBytes: 64, jobs: Math.min(16, navigator.hardwareConcurrency || 1), output: join(ROOT, "work/m2c") };
  for (let index = 0; index < argv.length; index++) {
    const arg = argv[index];
    if (arg === "--max-bytes") result.maxBytes = Number(argv[++index]);
    else if (arg === "--limit") result.limit = Number(argv[++index]);
    else if (arg === "--jobs") result.jobs = Number(argv[++index]);
    else if (arg === "--context") result.context = argv[++index];
    else if (arg === "-o" || arg === "--output") result.output = argv[++index];
    else if (!arg.startsWith("-") && !result.rom) result.rom = arg;
    else throw new Error(`unrecognized arguments: ${arg}`);
  }
  if (!result.rom) throw new Error("the following arguments are required: rom");
  return result;
}

async function parallelMap<T, U>(items: T[], jobs: number, worker: (item: T) => Promise<U>): Promise<U[]> {
  const results = new Array<U>(items.length);
  let cursor = 0;
  async function run(): Promise<void> {
    while (true) {
      const index = cursor++;
      if (index >= items.length) return;
      results[index] = await worker(items[index]);
    }
  }
  await Promise.all(Array.from({ length: Math.max(1, jobs) }, run));
  return results;
}

export async function main(argv = process.argv.slice(2)): Promise<void> {
  const args = options(argv);
  const rom = readFileSync(args.rom);
  const discovery = new Discovery(rom);
  discovery.run();
  mkdirSync(args.output, { recursive: true });
  const tracked = new Set(readdirSync(join(ROOT, "src")).filter((name) => /^[0-9a-f]+\.c$/.test(name)).map((name) => Number.parseInt(basename(name, ".c"), 16)));
  let candidates: [number, number][] = [];
  for (const [address, fn] of [...discovery.functions].sort((a, b) => a[0] - b[0])) {
    if (tracked.has(address) || fn.mode !== "thumb" || fn.unresolved.size || !fn.instructions.size) continue;
    const instructions = [...fn.instructions];
    const start = Math.min(...instructions);
    const end = Math.max(...instructions.map((item) => item + discovery.instructions.get(item)!.size));
    if (start === address && end - start <= args.maxBytes) candidates.push([address, end - start]);
  }
  if (args.limit !== undefined) candidates = candidates.slice(0, args.limit);
  const results = await parallelMap(candidates, args.jobs, async ([address, size]) => {
    const assembly = join(args.output, `${address.toString(16).padStart(8, "0")}.s`);
    const source = join(args.output, `${address.toString(16).padStart(8, "0")}.c`);
    let success = false;
    try {
      emitDiscovery(rom, discovery, address, assembly);
      const command = [String(M2C), "-t", "gba", "--valid-syntax", "--comment-style", "none", assembly];
      if (args.context) command.splice(1, 0, "--context", args.context, "--no-cache");
      const process = Bun.spawn(command, { stdout: "pipe", stderr: "pipe" });
      const [stdout, , exitCode] = await Promise.all([
        new Response(process.stdout).arrayBuffer(),
        new Response(process.stderr).arrayBuffer(),
        process.exited,
      ]);
      success = exitCode === 0;
      if (success) writeFileSync(source, Buffer.from(stdout));
    } catch {}
    return { entry: address, size, success };
  });
  writeFileSync(join(args.output, "report.json"), `${JSON.stringify(results, null, 2)}\n`);
  const success = results.filter((item) => item.success).length;
  console.log(`candidates=${results.length} success=${success} failures=${results.length - success}`);
}

if (import.meta.main) await main();
