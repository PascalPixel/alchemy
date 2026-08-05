#!/usr/bin/env bun
// Mechanical fix for the "wrong call symbol" bug found repeatedly today
// (resource_38b:cb4, resource_3b1:63ac, and "nearly every file" in
// resource_3c8 per an earlier harvesting report): semantic sources often
// declare a call target by its shared, veneer-resolved main-image name where
// the raw .s container's assembly actually calls a distinct LOCAL veneer at
// that specific site. overlay_call_targets.ts's resolvedCallNames() already
// knows the ground-truth per-site symbol; this rewrites each owner's call
// sites to match it POSITIONALLY (Nth call in the C source <-> Nth call site
// in the assembly, by textual/address order) and keeps the edit only if
// alchemist.ts's byte-diff proves it helps -- same gate, same discipline as
// mechanical_unsign_sweep.ts. Skips (does not guess) whenever the call count
// doesn't line up 1:1, since that means the correlation is ambiguous.
import { execFileSync } from "node:child_process";
import { readFileSync, readdirSync, unlinkSync, writeFileSync } from "node:fs";
import { basename, dirname, join } from "node:path";
import { resolveSpan } from "./alchemist.ts";
import { assembleOverlay } from "./overlay_disasm.ts";
import { blSiteSymbols, symbolName } from "./bl_site_symbols.ts";

const ROOT = dirname(Bun.fileURLToPath(import.meta.url).replace(/^file:\/\//, "")).replace(/\/tools$/, "");
const OVERLAY_BASE = 0x02000000;

const CALL_SITE = /\bFunc_([0-9a-f]{8})\s*\(/gi;
const EXTERN_DECL = /extern\s+([\w\s*]+?)\s+Func_([0-9a-f]{8})\s*\(([^)]*)\)\s*;/gi;

interface ExternInfo { returnType: string; params: string }

function rewriteCallSymbols(text: string, correctNames: string[], selfAddress: string): { text: string; changed: boolean } | null {
  // Collect existing extern declarations by name, to clone signatures for renamed sites.
  const externs = new Map<string, ExternInfo>();
  EXTERN_DECL.lastIndex = 0;
  let externMatch: RegExpExecArray | null;
  while ((externMatch = EXTERN_DECL.exec(text)) !== null) {
    externs.set(`Func_${externMatch[2].toLowerCase()}`, { returnType: externMatch[1].trim(), params: externMatch[3] });
  }

  // Ranges to exclude from call-site scanning: every extern declaration
  // (a `Func_X(` inside one is a parameter type/name, not a call) and the
  // function's own definition signature (self-reference, not a call site;
  // matching by this file's own address, not merely "the first Func_ token
  // that looks like a definition", since a definition-shaped occurrence
  // elsewhere would otherwise be silently excluded too).
  const excluded: { start: number; end: number }[] = [];
  EXTERN_DECL.lastIndex = 0;
  while ((externMatch = EXTERN_DECL.exec(text)) !== null) {
    excluded.push({ start: externMatch.index, end: externMatch.index + externMatch[0].length });
  }
  const selfDefinition = new RegExp(`\\b(?:void|u\\d+|s\\d+)\\s*\\*?\\s*Func_${selfAddress}\\s*\\([^)]*\\)\\s*\\{`, "i");
  const selfMatch = selfDefinition.exec(text);
  if (selfMatch !== null) excluded.push({ start: selfMatch.index, end: selfMatch.index + selfMatch[0].length });
  const isExcluded = (index: number): boolean => excluded.some((range) => index >= range.start && index < range.end);

  // Collect call sites in textual order with their exact source span.
  CALL_SITE.lastIndex = 0;
  const occurrences: { start: number; end: number; name: string }[] = [];
  let callMatch: RegExpExecArray | null;
  while ((callMatch = CALL_SITE.exec(text)) !== null) {
    if (isExcluded(callMatch.index)) continue;
    const name = `Func_${callMatch[1].toLowerCase()}`;
    occurrences.push({ start: callMatch.index, end: callMatch.index + `Func_${callMatch[1]}`.length, name });
  }

  if (occurrences.length !== correctNames.length || occurrences.length === 0) return null;

  const neededNew = new Map<string, ExternInfo>();
  const renames: { start: number; end: number; from: string; to: string }[] = [];
  for (let index = 0; index < occurrences.length; index++) {
    const occurrence = occurrences[index];
    const correct = correctNames[index];
    if (occurrence.name === correct) continue;
    const signature = externs.get(occurrence.name);
    if (signature === undefined) return null; // can't safely fabricate a signature
    // If the corrected name already has its own extern (reused from the
    // original file, or from an earlier renamed site), its signature MUST
    // match this site's -- otherwise this call's own argument list won't
    // match the declaration everyone else at that name is sharing, which
    // caused real "too few/too many arguments" compile failures (safely
    // caught and reverted, but this makes the generation itself safe rather
    // than relying on the gate for something this checkable up front).
    const existing = externs.get(correct) ?? neededNew.get(correct);
    if (existing !== undefined && (existing.returnType !== signature.returnType || existing.params !== signature.params)) {
      return null;
    }
    if (!externs.has(correct)) neededNew.set(correct, signature);
    renames.push({ start: occurrence.start, end: occurrence.end, from: occurrence.name, to: correct });
  }
  if (renames.length === 0) return { text, changed: false };

  // Apply text renames back-to-front so earlier offsets stay valid.
  let output = text;
  for (const rename of [...renames].sort((a, b) => b.start - a.start)) {
    output = output.slice(0, rename.start) + rename.to + output.slice(rename.end);
  }

  // Rebuild the extern block: drop declarations for names no longer called
  // at all, keep ones still in use, add ones newly needed.
  const stillUsed = new Set(correctNames);
  output = output.replace(EXTERN_DECL, (full, returnType: string, addr: string) => {
    const name = `Func_${addr.toLowerCase()}`;
    return stillUsed.has(name) ? full : "";
  });
  const insertionPoint = output.search(/\n(?:void|u\d+|s\d+|extern)\s+Func_[0-9a-f]{8}\s*\(/i);
  const declLines = [...neededNew.entries()]
    .map(([name, info]) => `extern ${info.returnType} ${name}(${info.params});`)
    .join("\n");
  if (insertionPoint === -1) {
    output = `${declLines}\n${output}`;
  } else {
    output = `${output.slice(0, insertionPoint)}\n${declLines}${output.slice(insertionPoint)}`;
  }
  return { text: output, changed: true };
}


const overlayImages = new Map<string, Uint8Array>();
function overlayImageFor(overlay: string): Uint8Array {
  let image = overlayImages.get(overlay);
  if (image === undefined) {
    image = assembleOverlay(join(ROOT, "assets", "code", `${overlay}_overlay.s`), OVERLAY_BASE);
    overlayImages.set(overlay, image);
  }
  return image;
}

interface AlchemistVerdict {
  verdict: "exact" | "improved" | "refused" | "exhausted" | "unnecessary";
  baseline_differing_bytes: number;
  differing_bytes: number;
}

function runAlchemist(id: string, write: boolean): AlchemistVerdict | null {
  try {
    const args = ["tools/alchemist.ts", id, "--json"];
    if (write) args.push("--write");
    const output = execFileSync("bun", args, { cwd: ROOT, encoding: "utf8", timeout: 90_000 });
    return JSON.parse(output) as AlchemistVerdict;
  } catch {
    return null;
  }
}

function runAdopt(id: string, source: string): boolean {
  try {
    const [overlay, offsetText] = id.split(":");
    const offset = Number.parseInt(offsetText, 16);
    const span = resolveSpan(overlay, offset);
    const args = ["tools/overlay_adopt.ts", id, "--source", source, "--apply"];
    if (span !== undefined) args.push("--span", String(span));
    execFileSync("bun", args, { cwd: ROOT, encoding: "utf8", timeout: 60_000 });
    return true;
  } catch {
    return false;
  }
}

async function main(): Promise<void> {
  const limitArg = Bun.argv.find((arg) => arg.startsWith("--limit="));
  const limit = limitArg !== undefined ? Number.parseInt(limitArg.split("=")[1], 10) : Infinity;
  const onlyArg = Bun.argv.find((arg) => arg.startsWith("--only="));
  const only = onlyArg !== undefined ? new Set(onlyArg.split("=")[1].split(",")) : null;
  const semanticDir = join(ROOT, "semantic");
  const files = readdirSync(semanticDir)
    .filter((name) => /^resource_[0-9a-f]+_c_[0-9a-f]{8}\.c$/i.test(name))
    .filter((name) => only === null || only.has(basename(name, ".c")))
    .map((name) => join(semanticDir, name))
    .slice(0, limit);

  let closed = 0;
  let improved = 0;
  let reverted = 0;
  let skipped = 0;

  for (const path of files) {
    const base = basename(path, ".c");
    const match = /^(resource_[0-9a-f]+)_c_([0-9a-f]{8})$/i.exec(base);
    if (match === null) { skipped++; continue; }
    const overlay = match[1];
    const address = Number.parseInt(match[2], 16);
    const offset = address - OVERLAY_BASE;
    const id = `${overlay}:${offset.toString(16)}`;
    const span = resolveSpan(overlay, offset);
    if (span === undefined) { skipped++; continue; }

    const original = readFileSync(path, "utf8");
    // Names come from DECODING the reference bl encodings, not from
    // resolvedCallNames(). That was this tool's original bug: an overlay bl
    // does not store the assembler's PC-relative displacement, so the symbol
    // must be named at the decoded address, which differs per call site even
    // for the same callee. resolvedCallNames() reports where the veneer
    // physically sits, which fixes the displacement's high halfword and
    // leaves the low one wrong -- a near-miss that reads like an external
    // toolchain problem. Three owners were written off that way before the
    // rule was worked out (see tools/bl_site_symbols.ts).
    let image: Uint8Array;
    try {
      image = overlayImageFor(overlay);
    } catch { skipped++; continue; }
    const sites = blSiteSymbols(image, OVERLAY_BASE + offset, span);
    if (sites.length === 0) { skipped++; continue; }
    const correctNames = sites.map(({ symbol }) => symbolName(symbol));

    const result = rewriteCallSymbols(original, correctNames, match[2].toLowerCase());
    if (result === null || !result.changed) { skipped++; continue; }

    const before = runAlchemist(id, false);
    if (before === null) { skipped++; continue; }
    const preBytes = before.baseline_differing_bytes;
    if (preBytes === 0) { skipped++; continue; }

    writeFileSync(path, result.text);
    const after = runAlchemist(id, true);
    if (after === null) {
      writeFileSync(path, original);
      reverted++;
      console.log(`${id} REVERTED (alchemist error after transform)`);
      continue;
    }

    const exactVerdict = after.verdict === "exact" || after.verdict === "unnecessary";
    const better = after.differing_bytes < preBytes;
    if (!exactVerdict && !better) {
      writeFileSync(path, original);
      reverted++;
      continue;
    }

    // Try to adopt on ANY improvement, not just on an "exact" verdict.
    // overlay_adopt.ts self-gates: it splices the candidate into the overlay's
    // assembly, rebuilds, and refuses unless the WHOLE overlay comes back
    // byte-identical -- which is the project's actual definition of exact, and
    // strictly stronger than the windowed byte-diff alchemist reports. The two
    // disagree in the near-exact band: 12 owners this sweep had left open at
    // "2 differing bytes" rebuilt the overlay byte-for-byte and closed on the
    // spot (resource_39b:e6c, resource_3a6:cd0, resource_385:4b4, ...). Gating
    // adoption on the weaker signal was leaving finished work on the floor.
    // A failed adopt costs one rebuild and reverts both files itself, so this
    // is safe to attempt unconditionally.
    if (runAdopt(id, path)) {
      // overlay_adopt.ts --apply installs the placeholder but does not
      // remove the now-superseded draft; leaving it behind means the owner
      // exists in both trees at once.
      unlinkSync(path);
      closed++;
      console.log(`${id} CLOSED via mechanical call-symbol fix, was ${preBytes} differing bytes`);
      continue;
    }

    if (exactVerdict) {
      writeFileSync(path, original);
      reverted++;
      console.log(`${id} REVERTED (adopt failed after exact verdict)`);
      continue;
    }

    improved++;
    console.log(`${id} IMPROVED via mechanical call-symbol fix: ${preBytes} -> ${after.differing_bytes} differing bytes`);
  }

  console.log(`closed=${closed} improved=${improved} reverted=${reverted} skipped=${skipped} total=${files.length}`);
}

if (import.meta.main) await main();
