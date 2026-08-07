#!/usr/bin/env bun
// Every command must start without crashing.
//
// A self-test proves a tool's units work. It does not prove the tool RUNS: 92 of
// 130 dispatcher subcommands once pointed at files that no longer existed, and
// shape_sweep could not touch overlay owners at all, while every gate was green.
// This runs each command and requires that it either does its job or explains
// its usage -- never that it dies with an unhandled exception.
//
// A usage exit is a pass. A stack trace, a missing module, or an unhandled
// rejection is a failure, because those are the shapes a moved file produces.
//
//   bun tools/check/commands.ts            # every subcommand of every tool
//   bun tools/check/commands.ts --scripts  # package.json scripts that are safe to probe
//   bun tools/check/commands.ts --self-test
import { existsSync, readdirSync } from "node:fs";
import { dirname, join } from "node:path";
import { resolveJobs } from "../lib/jobs.ts";

const ROOT = dirname(dirname(dirname(Bun.fileURLToPath(import.meta.url))));
const TOOLS = join(ROOT, "tools");

// A crash leaves one of these in the output. Exit codes alone are not enough:
// several tools exit 1 to report a legitimate finding.
const CRASH = [
  "error: Cannot find module",
  "error: Cannot find package",
  "ENOENT: no such file or directory, open '--",
  "is not a function",
  "undefined is not an object",
  "SyntaxError",
  "TypeError:",
  "ReferenceError:",
];

export type Outcome = { command: string; ok: boolean; detail: string };

export function crashed(output: string): string | null {
  for (const marker of CRASH) {
    if (output.includes(marker)) return marker;
  }
  return null;
}

// Tools that must not be probed: they mutate the tree, take minutes, or expect a
// long-running server. Each is named with the reason, so the exclusion is a
// decision on the page rather than a silent gap.
const UNPROBED = new Map<string, string>([
  ["make/build_rom", "rebuilds the whole ROM"],
  ["make/build_full", "rebuilds every overlay"],
  ["make/build_assets", "re-encodes 2,431 asset regions"],
  ["make/build_asm", "regenerates the disassembly tree"],
  ["make/build_claimed", "rebuilds the claimed image"],
  ["make/build_semantic", "rewrites the semantic tree"],
  ["metrics/dashboard_server", "starts a server and waits"],
  ["overlay/overlay_adopt", "installs a source into an overlay"],
]);

export function commandsUnder(tools: readonly string[], list: (tool: string) => string[]): string[] {
  const commands: string[] = [];
  for (const tool of tools) {
    for (const file of list(tool)) {
      if (!file.endsWith(".ts") || file === "index.ts") continue;
      const name = `${tool}/${file.slice(0, -3)}`;
      if (!UNPROBED.has(name)) commands.push(name);
    }
  }
  return commands.sort();
}

async function probe(command: string): Promise<Outcome> {
  const [tool, sub] = command.split("/");
  const child = Bun.spawn(["bun", join(TOOLS, tool!, `${sub}.ts`), "--help"], {
    cwd: ROOT,
    stdout: "pipe",
    stderr: "pipe",
    env: { ...process.env, ALCHEMY_PROBE: "1" },
  });
  // A command that ignores --help and starts real work would otherwise hold a
  // job slot forever. Killing it is not a failure: this gate asks whether the
  // process got off the ground, and one that is still running plainly did.
  const deadline = setTimeout(() => child.kill(), 20_000);
  const [, stdout, stderr] = await Promise.all([
    child.exited,
    new Response(child.stdout).text(),
    new Response(child.stderr).text(),
  ]);
  clearTimeout(deadline);
  const marker = crashed(`${stdout}\n${stderr}`);
  return {
    command,
    ok: marker === null,
    detail: marker ?? "",
  };
}

async function runAll(commands: readonly string[], jobs: number): Promise<Outcome[]> {
  const results: Outcome[] = [];
  let next = 0;
  await Promise.all(Array.from({ length: Math.min(jobs, commands.length) }, async () => {
    for (let index = next++; index < commands.length; index = next++) {
      results.push(await probe(commands[index]!));
    }
  }));
  return results;
}

function selfTest(): void {
  if (crashed("usage: overlay <subcommand>") !== null) throw new Error("a usage message must pass");
  if (crashed("error: Cannot find module './gone.ts'") === null) throw new Error("a missing module must fail");
  if (crashed("TypeError: x is not a function") === null) throw new Error("a TypeError must fail");
  const commands = commandsUnder(["overlay", "make"], (tool) =>
    tool === "overlay" ? ["index.ts", "overlay_twins.ts", "notes.md"] : ["build_rom.ts", "cache_entry.ts"]);
  if (commands.join(",") !== "make/cache_entry,overlay/overlay_twins") {
    throw new Error(`commandsUnder gave ${commands.join(",")}`);
  }
  if (!UNPROBED.has("make/build_rom")) throw new Error("destructive commands must stay unprobed");
  console.log("commands self-test ok");
}

async function main(): Promise<void> {
  const args = Bun.argv.slice(2);
  if (args.includes("--self-test")) return selfTest();
  const tools = readdirSync(TOOLS, { withFileTypes: true })
    .filter((entry) => entry.isDirectory() && entry.name !== "lib" && entry.name !== "scratch")
    .filter((entry) => existsSync(join(TOOLS, entry.name, "index.ts")))
    .map((entry) => entry.name);
  const commands = commandsUnder(tools, (tool) => readdirSync(join(TOOLS, tool)));
  const started = performance.now();
  const results = await runAll(commands, resolveJobs());
  const failed = results.filter((result) => !result.ok).sort((a, b) => a.command.localeCompare(b.command));
  for (const result of failed) console.error(`FAIL ${result.command}: ${result.detail}`);
  const seconds = ((performance.now() - started) / 1000).toFixed(1);
  console.log(
    `commands: ${results.length - failed.length}/${results.length} start cleanly in ${seconds}s ` +
      `(${UNPROBED.size} unprobed by name)`,
  );
  if (failed.length > 0) process.exit(1);
}

if (import.meta.main) await main();
