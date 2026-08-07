#!/usr/bin/env bun
// Run every tool's --self-test, discovered rather than listed.
//
// The chain this replaces named 107 tools by hand in one 4,858-character
// package.json string. 138 tools actually had a self-test, so 31 safety nets
// ran nowhere and nothing could detect that: adding a tool and forgetting the
// chain is invisible, and the omission looks exactly like a tool without tests.
// Discovery removes the list, and with it the drift.
//
//   bun tools/self_test.ts             # run all, fail on first failing tool
//   bun tools/self_test.ts --list      # names only
//   bun tools/self_test.ts --jobs 4
//   bun tools/self_test.ts --self-test
import { readdirSync, readFileSync } from "node:fs";
import { availableParallelism } from "node:os";
import { dirname, join } from "node:path";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const TOOLS = join(ROOT, "tools");

// A tool opts in by handling the flag. Matching the literal keeps the contract
// visible in the tool itself rather than in a registry that can disagree.
// Names may be nested (`assets/music.ts`): folders are an ownership boundary,
// not a reason to stop testing, and a non-recursive walk would silently drop
// every self-test the moment a tool moved into one.
export function discover(names: readonly string[], read: (name: string) => string): string[] {
  return names
    .filter((name) => name.endsWith(".ts"))
    .filter((name) => read(name).includes('"--self-test"'))
    .sort();
}

function walk(directory: string, prefix = ""): string[] {
  const names: string[] = [];
  for (const entry of readdirSync(join(TOOLS, directory), { withFileTypes: true })) {
    const relative = prefix === "" ? entry.name : `${prefix}/${entry.name}`;
    if (entry.isDirectory()) names.push(...walk(join(directory, entry.name), relative));
    else names.push(relative);
  }
  return names;
}

// This runner is excluded from its own discovery: it would recurse.
const EXCLUDED = new Set(["self_test.ts"]);

type Result = { tool: string; ok: boolean; output: string; ms: number };

async function runOne(tool: string): Promise<Result> {
  const started = performance.now();
  const child = Bun.spawn(["bun", join(TOOLS, tool), "--self-test"], {
    cwd: ROOT,
    stdout: "pipe",
    stderr: "pipe",
  });
  const [code, stdout, stderr] = await Promise.all([
    child.exited,
    new Response(child.stdout).text(),
    new Response(child.stderr).text(),
  ]);
  return {
    tool,
    ok: code === 0,
    output: (stderr || stdout).trim(),
    ms: Math.round(performance.now() - started),
  };
}

// Bounded concurrency: the self-tests compile C and assemble overlays, so an
// unbounded fan-out over 138 tools thrashes rather than finishing sooner.
async function runAll(tools: readonly string[], jobs: number): Promise<Result[]> {
  const results: Result[] = [];
  let next = 0;
  const workers = Array.from({ length: Math.min(jobs, tools.length) }, async () => {
    for (let index = next++; index < tools.length; index = next++) {
      results.push(await runOne(tools[index]!));
    }
  });
  await Promise.all(workers);
  return results;
}

function selfTest(): void {
  const read = (name: string) =>
    ({
      "a.ts": 'if (args.includes("--self-test")) return selfTest();',
      "b.ts": "no tests here",
      "c.ts": 'const x = "--self-test";',
      "d.txt": '"--self-test"',
    })[name] ?? "";
  const found = discover(["a.ts", "b.ts", "c.ts", "d.txt"], read);
  if (found.join(",") !== "a.ts,c.ts") throw new Error(`discover picked ${found.join(",")}`);
  if (discover([], read).length !== 0) throw new Error("empty input must discover nothing");
  if (!EXCLUDED.has("self_test.ts")) throw new Error("the runner must exclude itself");
  if (discover(["assets/music.ts"], () => '"--self-test"').length !== 1) {
    throw new Error("discover must accept nested names");
  }
  console.log("self_test self-test ok");
}

async function main(): Promise<void> {
  const args = Bun.argv.slice(2);
  if (args.includes("--self-test")) return selfTest();
  const jobs = Number(args[args.indexOf("--jobs") + 1]) || Math.max(2, availableParallelism() - 2);
  const tools = discover(walk(""), (name) =>
    readFileSync(join(TOOLS, name), "utf8")).filter((name) => !EXCLUDED.has(name));
  if (args.includes("--list")) {
    for (const tool of tools) console.log(`  ${tool}`);
    console.log(`${tools.length} tools expose a self-test`);
    return;
  }
  const started = performance.now();
  const results = await runAll(tools, jobs);
  const failed = results.filter((result) => !result.ok).sort((a, b) => a.tool.localeCompare(b.tool));
  for (const result of failed) {
    console.error(`FAIL ${result.tool}`);
    for (const line of result.output.split("\n").slice(-6)) console.error(`     ${line}`);
  }
  const seconds = ((performance.now() - started) / 1000).toFixed(1);
  console.log(`self-tests: ${results.length - failed.length}/${results.length} passed in ${seconds}s (jobs=${jobs})`);
  if (failed.length > 0) process.exit(1);
}

if (import.meta.main) await main();
