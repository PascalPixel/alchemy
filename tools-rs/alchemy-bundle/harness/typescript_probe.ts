// Emits, from the live TypeScript, exactly the values `src/bin/bundle_corpus.rs`
// computes in Rust, so the two can be byte-compared.
//
// WHY a probe rather than a unit test: `compilerBundleSignature` is cache-key
// material in six call sites, and `compilerCommandForTargetSource` decides which
// compiler binary runs and with which flags. Neither can be checked by reading;
// the only honest check is running both implementations over every source the
// repository actually compiles and diffing the bytes.
//
// Reads a newline-separated corpus of source paths on stdin. Writes one record
// per line to stdout, tab-separated, in input order:
//
//   SIG\t<compilerBundleSignature()>
//   CMD\t<source>\t<target>\t<argv joined by U+001F>
//
// RETIREMENT: delete this file with the rest of the harness when
// tools/lib/alchemy_gcc.ts is deleted.

import {
  compilerBundleSignature,
  compilerCommandForTargetSource,
  cflagsForTargetSource,
  type CompilerTarget,
} from "../../../tools/lib/alchemy_gcc.ts";

const corpus = (await Bun.stdin.text())
  .split("\n")
  .map((line) => line.trim())
  .filter((line) => line.length > 0);

const out: string[] = [];
out.push(`SIG\t${compilerBundleSignature()}`);

const UNIT = String.fromCharCode(31);
const targets: CompilerTarget[] = ["gs1", "gs2"];
for (const source of corpus) {
  for (const target of targets) {
    // The flags come from `cflagsForTargetSource`, so every routed flag
    // combination the tree can produce reaches the command builder -- which is
    // the point: the array-spread order is later-flag-wins and load-bearing.
    const flags = [...cflagsForTargetSource(target, source), "-S", "-o", "-", source];
    const argv = compilerCommandForTargetSource(target, source, ...flags);
    out.push(`CMD\t${source}\t${target}\t${argv.join("")}`);
  }
}

process.stdout.write(out.join("\n") + "\n");
