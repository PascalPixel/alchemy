// Emits, from the live TypeScript, exactly the values `src/bin/plan_corpus.rs`
// computes in Rust, so the two can be byte-compared.
//
// WHY a probe rather than a unit test: `sourceToAssemblyPlan` decides which
// compiler binary compiles a source, whether a separate preprocessing pass runs,
// and the exact argv of each step -- and gcc is later-flag-wins, so argv order
// is machine code. The routing tables that feed it have thousands of entries
// with per-source overlays. No amount of reading proves the two implementations
// agree; running both over every source the repository compiles does.
//
// Failures are recorded, not swallowed: a throw becomes an `err` record
// carrying its message, so throw-for-throw parity is measured too. A family
// whose bundle is not installed on this machine still contributes a comparison.
//
// Reads a newline-separated corpus of source paths on stdin. Writes one record
// per line to stdout, tab-separated, in input order:
//
//   PLAN\t<source>\t<target>\t<family>\t<ok|err>\t<payload>
//   MUT \t<source>\t<payload>          (routed gs1, with flag mutations)
//   DPP \t<source>\t<ok|err>\t<argv>   (directPreprocessorCommand)
//   DCC \t<source>\t<ok|err>\t<argv>   (directCompilerCommand)
//   DCCS\t<source>\t<ok|err>\t<argv>   (directCompilerCommandForSource)
//
// argv items are joined by U+001F; the plan payload's sections by U+001E.
//
// RETIREMENT: delete this file with the rest of the harness when
// tools/lib/alchemy_gcc.ts is deleted.

import {
  sourceToAssemblyPlan,
  directPreprocessorCommand,
  directCompilerCommand,
  directCompilerCommandForSource,
  type CompilerFamily,
  type CompilerTarget,
  type SourceToAssemblyPlan,
} from "../../../tools/lib/alchemy_gcc.ts";

const corpus = (await Bun.stdin.text())
  .split("\n")
  .map((line) => line.trim())
  .filter((line) => line.length > 0);

const UNIT = String.fromCharCode(31);
const SECTION = String.fromCharCode(30);

const targets: CompilerTarget[] = ["gs1", "gs2"];
// Declaration order of the `CompilerFamily` union. The Rust side sweeps the
// same order; `drift.rs` proves the two orders are the same one.
const families: CompilerFamily[] = [
  "routed",
  "gcc296",
  "old-agbcc",
  "pret-early-thumb",
  "gcc2951",
  "gcc3",
];

// Deliberately inside this crate's own scratch directory: nothing is written,
// but a path that reads like a real build output invites someone to wire one up.
const FIXTURES = "tools-rs/alchemy-plan/target/fixtures";

function payload(plan: SourceToAssemblyPlan): string {
  return [
    plan.target,
    plan.requestedFamily,
    plan.family,
    plan.routingSource,
    plan.input,
    plan.output,
    plan.compilerInput,
    plan.flags.join(UNIT),
    plan.steps.map((step) => [step.kind, ...step.command].join(UNIT)).join(SECTION),
  ].join(SECTION);
}

function attempt(body: () => string): string {
  try {
    return `ok\t${body()}`;
  } catch (error) {
    return `err\t${error instanceof Error ? error.message : String(error)}`;
  }
}

const out: string[] = [];
for (const source of corpus) {
  const input = source;
  const output = `${FIXTURES}/${source}.s`;

  for (const target of targets) {
    for (const family of families) {
      const record = attempt(() =>
        payload(
          sourceToAssemblyPlan({
            target,
            routingSource: source,
            input,
            output,
            // "routed" is a real member of the union, so it is passed through
            // rather than turned back into an absent property: the two are
            // equivalent by `??` and the sweep should exercise the explicit form.
            family,
          }),
        )
      );
      out.push(`PLAN\t${source}\t${target}\t${family}\t${record}`);
    }
  }

  // Every optional property populated at once, so defaulting is not the only
  // path measured: an explicit dumpbase, an explicit intermediate, and a flag
  // mutation whose addition must land last to win.
  out.push(
    `MUT\t${source}\t` +
      attempt(() =>
        payload(
          sourceToAssemblyPlan({
            target: "gs1",
            routingSource: source,
            input,
            output,
            flags: { addFlags: ["-fno-inline"], removeFlags: ["-O2"] },
            preprocessedOutput: `${FIXTURES}/${source}.i`,
            dumpbase: "explicit.c",
          }),
        )
      ),
  );

  out.push(
    `DPP\t${source}\t` +
      attempt(() => directPreprocessorCommand(input, `${FIXTURES}/${source}.i`).join(UNIT)),
  );
  out.push(
    `DCC\t${source}\t` +
      attempt(() => directCompilerCommand(input, output, "explicit.c", source).join(UNIT)),
  );
  out.push(
    `DCCS\t${source}\t` +
      attempt(() =>
        directCompilerCommandForSource(source, input, output, "explicit.c").join(UNIT)
      ),
  );
}

process.stdout.write(out.join("\n") + "\n");
