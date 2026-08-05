#!/usr/bin/env bun
// Report the symbol name each `bl` in an overlay owner must be given.
//
// An overlay `bl` does not store the assembler's PC-relative displacement.
// Decoding the reference encoding as if it did yields the address the symbol
// must carry for the assembler to reproduce those exact bytes -- and that
// address depends on the CALL SITE, not just the callee. This is the
// mechanical reason behind the project's rule that call symbols are per-site
// rather than per-import, and why the raw .s spells every call as its own
// `sub_02000xxx`.
//
// Three owners sat blocked on this for a long time, each with a note
// concluding "toolchain/link-time veneer-placement quirk, not attempted
// further" -- one of them recording that three separate symbol strategies had
// been tried. All three were reasoning about the encoding rather than
// decoding it, and all three closed byte-exact once decoded:
// resource_382:3ac (+108), resource_3b3:1174 (+80), resource_380:4328 (+148).
// This tool exists so the decode is one command instead of a rediscovery.
//
// Naming the veneer's PHYSICAL address (what overlay_call_targets.ts reports
// as the target) is the trap: it fixes the displacement's high halfword and
// leaves the low halfword wrong, which reads like a near-miss caused by
// something external. Two other dead ends, both ruled out empirically: adding
// the thumb bit changes nothing (the resolver masks it), and the per-overlay
// `_call_via_rN` linking delta from callViaBankBase makes it worse (that
// constant applies to the call-via bank, not to ordinary local callees).
import { assembleOverlay, OVERLAY_BASE } from "./overlay_disasm.ts";
import { resolveSpan } from "./alchemist.ts";
import { dirname, join } from "node:path";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

export interface CallSite {
  site: number;    // address of the bl
  symbol: number;  // address the symbol must be named at
}

/**
 * Decode every `bl` in [start, start+span).
 *
 * A literal pool word can happen to match the two-halfword `bl` shape, so a
 * caller should cross-check the count against the number of calls its source
 * actually makes rather than trusting this blindly -- which is also why this
 * only ever reports, and never edits a source.
 */
export function blSiteSymbols(image: Uint8Array, start: number, span: number, base = OVERLAY_BASE): CallSite[] {
  const halfword = (address: number): number => {
    const offset = address - base;
    return image[offset] | (image[offset + 1] << 8);
  };
  const sites: CallSite[] = [];
  for (let address = start; address + 3 < start + span; address += 2) {
    const high = halfword(address);
    const low = halfword(address + 2);
    if ((high & 0xf800) !== 0xf000 || (low & 0xf800) !== 0xf800) continue;
    let offset = ((high & 0x7ff) << 12) | ((low & 0x7ff) << 1);
    if (offset & 0x400000) offset -= 0x800000; // sign-extend the 23-bit displacement
    sites.push({ site: address, symbol: address + 4 + offset });
  }
  return sites;
}

export function symbolName(address: number): string {
  return `Func_${address.toString(16).padStart(8, "0")}`;
}

function selfTest(): void {
  // Encode a known bl and check it round-trips to the address the assembler
  // would need. Built by hand rather than taken from a ROM so the fixture
  // carries no game bytes.
  const base = 0x02000000;
  const image = new Uint8Array(0x40);
  const site = 0x02000010;
  const target = 0x02000030;
  const offset = target - (site + 4);
  const high = 0xf000 | ((offset >> 12) & 0x7ff);
  const low = 0xf800 | ((offset >> 1) & 0x7ff);
  image[site - base] = high & 0xff;
  image[site - base + 1] = high >> 8;
  image[site - base + 2] = low & 0xff;
  image[site - base + 3] = low >> 8;

  const found = blSiteSymbols(image, 0x02000000, 0x40, base);
  if (found.length !== 1) throw new Error(`expected one call site, got ${found.length}`);
  if (found[0].site !== site) throw new Error(`wrong site: ${found[0].site.toString(16)}`);
  if (found[0].symbol !== target) throw new Error(`wrong symbol address: ${found[0].symbol.toString(16)}`);
  if (symbolName(target) !== "Func_02000030") throw new Error(`wrong name: ${symbolName(target)}`);

  // A negative (backward) displacement must sign-extend rather than wrap.
  const backSite = 0x02000020;
  const backTarget = 0x02000004;
  const backOffset = backTarget - (backSite + 4);
  const backHigh = 0xf000 | ((backOffset >> 12) & 0x7ff);
  const backLow = 0xf800 | ((backOffset >> 1) & 0x7ff);
  const backImage = new Uint8Array(0x40);
  backImage[backSite - base] = backHigh & 0xff;
  backImage[backSite - base + 1] = backHigh >> 8;
  backImage[backSite - base + 2] = backLow & 0xff;
  backImage[backSite - base + 3] = backLow >> 8;
  const backFound = blSiteSymbols(backImage, 0x02000000, 0x40, base);
  if (backFound.length !== 1 || backFound[0].symbol !== backTarget) {
    throw new Error(`backward displacement did not sign-extend: ${JSON.stringify(backFound)}`);
  }
  console.log("self-test=ok tool=bl_site_symbols");
}

async function main(): Promise<void> {
  if (Bun.argv.includes("--self-test")) return selfTest();
  const id = Bun.argv[2];
  const matched = /^(resource_[0-9a-f]+):(?:0x)?([0-9a-f]+)$/i.exec(id ?? "");
  if (matched === null) {
    console.log("usage: bl_site_symbols.ts <overlay:offsetHex> [--span BYTES]");
    process.exit(1);
  }
  const overlay = matched[1];
  const offset = Number.parseInt(matched[2], 16);
  const spanArgument = Bun.argv.indexOf("--span");
  const span = spanArgument === -1 ? resolveSpan(overlay, offset) : Number(Bun.argv[spanArgument + 1]);
  if (span === undefined) throw new Error(`no registered span for ${id}; pass --span BYTES`);

  const image = assembleOverlay(join(ROOT, "assets", "code", `${overlay}_overlay.s`), OVERLAY_BASE);
  const sites = blSiteSymbols(image, OVERLAY_BASE + offset, span);
  console.log(`${id} span=${span} call_sites=${sites.length}`);
  for (const { site, symbol } of sites) {
    console.log(`  0x${site.toString(16)} -> ${symbolName(symbol)}`);
  }
  console.log("name the i-th call in source order after the i-th site above;");
  console.log("a repeated callee still needs a DIFFERENT name at each of its sites.");
}

if (import.meta.main) await main();
