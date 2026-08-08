// Oracle for the `compiler_signature` half of tests/parity.rs.
//
// `compilerSignature` in tools/lib/mode_sweep.ts cannot be called: it looks for
// five modules in tools/ that live in tools/lib/, so it throws ENOENT (see the
// PORT NOTE on signature_source_paths). A differential test against a function
// that always throws proves only that both sides throw, which is worthless.
//
// So this replays the same computation with the path CORRECTED, and the Rust
// side does the same, and the two are compared. Every other input --
// compilerBundleSignature, the hash construction, the file list and its order
// -- is taken verbatim from mode_sweep.ts lines 323-330 and 463-468. Nothing in
// tools/ is modified; the two lines are re-expressed here against the same
// imports.
import { compilerBundleSignature } from "../../../tools/lib/alchemy_gcc.ts";
import { createHash } from "node:crypto";
import { readFileSync } from "node:fs";
import { dirname, join } from "node:path";

// Four hops: tools-rs/mode-sweep/tests/<this file>. Miscounting this is
// exactly the regression b3ab4841b introduced in mode_sweep.ts, so the ROOT
// assertion below refuses to run against a directory that is not the repo.
const ROOT = dirname(dirname(dirname(dirname(Bun.fileURLToPath(import.meta.url)))));
if (!readFileSync(join(ROOT, "tools", "lib", "mode_sweep.ts"))) throw new Error("bad ROOT");

function hash(...parts: Array<string | Uint8Array>): string {
  const digest = createHash("sha256");
  for (const part of parts) {
    digest.update(part as never);
    digest.update("\0");
  }
  return digest.digest("hex");
}

console.log(
  hash(
    compilerBundleSignature(),
    ...["alchemy_gcc.ts", "match_m2c.ts", "integrate_matches.ts", "candidate_show.ts", "mode_sweep.ts"]
      // The one correction: "tools", "lib" instead of "tools".
      .map((name) => readFileSync(join(ROOT, "tools", "lib", name))),
  ),
);
