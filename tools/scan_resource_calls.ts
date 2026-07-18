#!/usr/bin/env bun

export interface DirectResourceCall {
  resource_id: number;
  function: string;
  call_address: string;
  source: "literal" | "immediate";
}

function privateOutput(path: string): boolean {
  const normalized = path.replaceAll("\\", "/");
  return normalized.startsWith("out/") || normalized.includes("/out/") ||
    normalized.startsWith("/tmp/") || normalized.startsWith("/private/tmp/");
}

function option(args: string[], name: string): string {
  const index = args.indexOf(name);
  if (index < 0 || args[index + 1] === undefined) throw new Error(`${name} is required`);
  return args[index + 1];
}

function writesR0(line: string): boolean {
  return /:\s+[0-9a-f]+\s+.*\b(?:adds|subs|mov|movs|ldr|ldrb|ldrh|pop)\s+r0\b/.test(line);
}

export function directCalls(
  text: string,
  rom: Uint8Array,
  functionName: string,
  resourceCount: number,
): { calls: DirectResourceCall[]; unresolved: number } {
  const lines = text.split(/\r?\n/);
  const view = new DataView(rom.buffer, rom.byteOffset, rom.byteLength);
  const calls: DirectResourceCall[] = [];
  let unresolved = 0;
  for (let index = 0; index < lines.length; index++) {
    if (!/\bbl\s+[0-9a-f]+ <Func_08002f40>/.test(lines[index])) continue;
    let resourceId: number | undefined;
    let source: DirectResourceCall["source"] | undefined;
    for (let previous = index - 1; previous >= Math.max(0, index - 16); previous--) {
      const literal = lines[previous].match(/\bldr\s+r0, \[pc, #[^\]]+\].*\(([0-9a-f]+) /);
      if (literal) {
        const address = Number.parseInt(literal[1], 16);
        const offset = address - 0x08000000;
        if (offset >= 0 && offset + 4 <= rom.length) {
          resourceId = view.getUint32(offset, true);
          source = "literal";
        }
        break;
      }
      const immediate = lines[previous].match(/\bmovs\s+r0, #(\d+)/);
      if (immediate) {
        resourceId = Number(immediate[1]);
        source = "immediate";
        break;
      }
      if (writesR0(lines[previous])) break;
    }
    if (resourceId === undefined || source === undefined || resourceId < 0 || resourceId >= resourceCount) {
      unresolved++;
      continue;
    }
    const address = lines[index].match(/^\s*([0-9a-f]+):/i)?.[1];
    calls.push({
      resource_id: resourceId,
      function: functionName,
      call_address: `0x${address ?? "0"}`,
      source,
    });
  }
  return { calls, unresolved };
}

export function selfTest(): void {
  const rom = new Uint8Array(0x100);
  new DataView(rom.buffer).setUint32(0x20, 0x2a, true);
  const text = `
08000000: 4807 ldr r0, [pc, #28] @ (8000020 <x>)
08000002: f002 bl 8002f40 <Func_08002f40>
08000006: 2011 movs r0, #17
08000008: f002 bl 8002f40 <Func_08002f40>
0800000c: 1c28 adds r0, r5, #0
0800000e: f002 bl 8002f40 <Func_08002f40>`;
  const result = directCalls(text, rom, "Func_test", 0x100);
  if (result.calls.length !== 2 || result.unresolved !== 1 ||
      result.calls[0].resource_id !== 0x2a || result.calls[0].source !== "literal" ||
      result.calls[1].resource_id !== 17 || result.calls[1].source !== "immediate") {
    throw new Error("resource-call scanner self-test failed");
  }
  console.log("self-test=ok");
}

async function main(args: string[]): Promise<void> {
  if (args.includes("--self-test")) {
    selfTest();
    return;
  }
  const positional = args.filter((item, index) => !item.startsWith("-") && args[index - 1] !== "-o" &&
    args[index - 1] !== "--resource-count");
  if (positional.length < 2) {
    console.log("usage: scan_resource_calls.ts ROM ASM_BUILD_DIR -o OUT [--resource-count COUNT]");
    return;
  }
  const output = option(args, "-o");
  if (!privateOutput(output)) throw new Error("resource-call reports must stay under out/ or a temporary directory");
  const resourceCount = args.includes("--resource-count") ? Number(option(args, "--resource-count")) : 0x3ce;
  const rom = new Uint8Array(await Bun.file(positional[0]).arrayBuffer());
  const directory = positional[1].replace(/\/$/, "");
  const calls: DirectResourceCall[] = [];
  let unresolved = 0;
  let functions = 0;
  for (const leaf of new Bun.Glob("*.elf").scanSync(directory)) {
    const process = Bun.spawnSync(["arm-none-eabi-objdump", "-d", `${directory}/${leaf}`], {
      stdout: "pipe",
      stderr: "pipe",
    });
    if (process.exitCode !== 0) throw new Error(process.stderr.toString().trim());
    const functionName = `Func_${leaf.replace(/\.elf$/, "")}`;
    const result = directCalls(process.stdout.toString(), rom, functionName, resourceCount);
    calls.push(...result.calls);
    unresolved += result.unresolved;
    functions++;
  }
  calls.sort((left, right) => left.resource_id - right.resource_id ||
    left.function.localeCompare(right.function) || left.call_address.localeCompare(right.call_address));
  const resources = new Map<number, DirectResourceCall[]>();
  for (const call of calls) resources.set(call.resource_id, [...(resources.get(call.resource_id) ?? []), call]);
  await Bun.write(output, JSON.stringify({
    format: 1,
    kind: "direct-resource-call-scan",
    functions,
    resolved_calls: calls.length,
    unresolved_calls: unresolved,
    resources: [...resources].map(([resource_id, direct_calls]) => ({ resource_id, direct_calls })),
  }, null, 2) + "\n");
  console.log(`functions=${functions} resolved=${calls.length} unresolved=${unresolved} resources=${resources.size}`);
}

if (import.meta.main) await main(Bun.argv.slice(2));
