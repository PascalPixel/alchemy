// Differential-parity probe for `tools-rs/alchemy-selftest`.
//
// Reads one key per line on stdin, calls the LIVE TypeScript primitive the key
// names, and writes `key<U+001D>value` on stdout in the same order. The Rust
// side records the identical keys and values as it runs the ported `selfTest`
// body, so a byte comparison of the two streams is the parity check.
//
// This file is new and additive. It imports `alchemy_gcc.ts` and does not
// modify it.

import {
  cflagsForTargetSource,
  directCompilerCommand,
  sourceToAssemblyPlan,
  usesAgbccCompiler,
} from "../../../tools/lib/alchemy_gcc.ts";

const UNIT = "\u001f";
const SECTION = "\u001e";
const PAIR = "\u001d";

function dashed(value: string): string[] {
  return value === "-" ? [] : value.split(UNIT);
}

function planPayload(plan: any): string {
  const steps = plan.steps
    .map((step: any) => [step.kind, ...step.command].join(UNIT))
    .join(SECTION);
  return [
    plan.target,
    plan.requestedFamily,
    plan.family,
    plan.routingSource,
    plan.input,
    plan.output,
    plan.compilerInput,
    plan.flags.join(UNIT),
    steps,
  ].join(SECTION);
}

function evaluate(key: string): string {
  const parts = key.split("\t");
  const kind = parts[0];
  if (kind === "CF") {
    return cflagsForTargetSource(parts[1] as any, parts[2]).join(UNIT);
  }
  if (kind === "UA") {
    return String(usesAgbccCompiler(parts[1] as any, parts[2]));
  }
  if (kind === "DCC") {
    try {
      const argv = directCompilerCommand(
        parts[1], parts[2], parts[3], parts[4] === "-" ? undefined : parts[4],
      );
      return `ok\t${argv.join(UNIT)}`;
    } catch (error) {
      return `err\t${(error as Error).message}`;
    }
  }
  if (kind === "PLAN") {
    const options: Record<string, unknown> = {
      target: parts[1],
      routingSource: parts[2],
      input: parts[3],
      output: parts[4],
    };
    if (parts[5] !== "-") options.family = parts[5];
    if (parts[6] !== "-" || parts[7] !== "-") {
      options.flags = { addFlags: dashed(parts[6]), removeFlags: dashed(parts[7]) };
    }
    if (parts[8] !== "-") options.preprocessedOutput = parts[8];
    if (parts[9] !== "-") options.dumpbase = parts[9];
    try {
      return `ok\t${planPayload(sourceToAssemblyPlan(options as any))}`;
    } catch (error) {
      return `err\t${(error as Error).message}`;
    }
  }
  throw new Error(`unknown probe key kind: ${kind}`);
}

const input = await Bun.stdin.text();
const lines = input.split("\n").filter((line) => line.length > 0);
if (lines.length === 0) {
  // Scanning nothing is not passing.
  throw new Error("probe received no keys on stdin");
}
const out: string[] = [];
for (const line of lines) {
  out.push(`${line}${PAIR}${evaluate(line)}`);
}
process.stdout.write(`${out.join("\n")}\n`);
