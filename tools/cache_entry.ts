#!/usr/bin/env bun
// Writing a content-addressed cache entry so it can never be observed
// half-written.
//
// THE SECOND DEFECT BEHIND THE POISONED CACHE, and the one that actually
// explains the symptom. A hand-maintained key (see HANDOVER §5h) yields an
// entry that is WRONG BUT COMPLETE. It cannot explain one key holding 160
// bytes in one worktree and 164 in another: a content-addressed key has one
// correct content by construction, so two different LENGTHS mean one of them
// was never finished being written.
//
// `writeFileSync` to the final path is not atomic. A run killed mid-write, or
// two lanes building the same overlay at once, leaves a truncated file that the
// next `existsSync` check serves happily for ever. Nothing downstream re-reads
// or re-checks it, which is why the damage surfaced as a mysterious LZ
// round-trip failure in a completely different tool, recurred for several
// people, and survived across commits — `git checkout` does not touch `out/`.
//
// Writing to a unique temporary name in the SAME directory and renaming makes
// the entry appear whole or not at all; rename is atomic within a filesystem.
// A concurrent writer loses the race harmlessly, leaving a complete file.
//
//   bun tools/cache_entry.ts --self-test
import { existsSync, mkdtempSync, readFileSync, readdirSync, renameSync, rmSync, writeFileSync } from "node:fs";
import { join } from "node:path";

/** Write `data` to `finalPath` atomically. Never leaves a partial entry there. */
export function writeCacheEntryAtomically(finalPath: string, data: Uint8Array): void {
  // The suffix is deliberately NOT `.bin`: a temporary left behind by a killed
  // process must never be mistaken for an entry by a `*.bin` listing, and can
  // never be found by a key lookup because the key names the final path.
  const temporary = `${finalPath}.${process.pid}-${Math.random().toString(36).slice(2)}.partial`;
  try {
    writeFileSync(temporary, data);
    renameSync(temporary, finalPath);
  } catch (error) {
    try {
      rmSync(temporary, { force: true });
    } catch {
      // Best effort. A stray `.partial` is inert by construction.
    }
    throw error;
  }
}

function selfTest(): void {
  const work = mkdtempSync(join(Bun.env.TMPDIR ?? "/tmp", "alchemy-cache-entry-"));
  try {
    const target = join(work, "entry.bin");
    const payload = new Uint8Array([1, 2, 3, 4, 5]);
    writeCacheEntryAtomically(target, payload);
    if (!existsSync(target)) throw new Error("the entry must exist after writing");
    if (!Buffer.from(readFileSync(target)).equals(Buffer.from(payload))) {
      throw new Error("the entry must hold exactly the bytes written");
    }
    // No temporary may survive a successful write, or a later `*.partial` sweep
    // would have nothing to distinguish a leak from normal operation.
    if (readdirSync(work).some((name) => name.endsWith(".partial"))) {
      throw new Error("a successful write must leave no .partial behind");
    }
    // Overwriting with a SHORTER payload must not leave any of the longer one.
    // This is the 164-then-160 case that started the investigation.
    const shorter = new Uint8Array([9, 9]);
    writeCacheEntryAtomically(target, shorter);
    const after = readFileSync(target);
    if (after.byteLength !== 2) throw new Error(`a shorter rewrite must truncate cleanly, got ${after.byteLength} bytes`);
    // A temporary is never a valid entry name, in either direction.
    const temporaryName = "entry.bin.123-abc.partial";
    if (temporaryName.endsWith(".bin")) throw new Error("a temporary must not end in .bin");
    if (!temporaryName.endsWith(".partial")) throw new Error("a temporary must be identifiable");
    // And a failed write must not leave the temporary lying around: writing to
    // a path inside a file (not a directory) cannot succeed.
    const impossible = join(target, "nested", "entry.bin");
    let threw = false;
    try {
      writeCacheEntryAtomically(impossible, payload);
    } catch {
      threw = true;
    }
    if (!threw) throw new Error("an impossible write must throw rather than pass silently");
    if (readdirSync(work).some((name) => name.endsWith(".partial"))) {
      throw new Error("a failed write must clean up its .partial");
    }
    console.log("self-test=ok (atomic cache entry, both directions)");
  } finally {
    rmSync(work, { recursive: true, force: true });
  }
}

if (import.meta.main) {
  if (Bun.argv.includes("--self-test")) selfTest();
  else throw new Error("usage: cache_entry.ts --self-test");
}
