#!/usr/bin/env bun
// Tool role: entrypoint; invoked by PLAYBOOK.md.
// Adapt exact instruction-shape hits from Alchemy's verified C corpus to
// overlay-local symbols, then require fully linked byte equality.
import { mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { verifyCandidate } from "./match_m2c.ts";
import { assembleOverlay, OVERLAY_BASE } from "./overlay_disasm.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const DEFINITION = /\b(Func_[0-9a-f]{8})\s*\([^;{}]*\)\s*\{/i;
const DECLARATION = /^[\w][\w* ]*\b(Func_[0-9a-f]{8})\s*\([^;{}]*\)\s*;$/gim;

interface Options { templates: string; inventory: string; drafts: string; output: string }
interface FunctionRow { id: string; overlay: string; entry: number; offset: number; span_bytes: number }

function optionsOf(argv: string[]): Options {
  const options: Options = {
    templates: join(ROOT, "out/decomp/compiler-templates.json"),
    inventory: join(ROOT, "out/decomp/overlays.json"),
    drafts: join(ROOT, "out/decomp/overlay-matches"),
    output: join(ROOT, "out/decomp/overlay-template-matches"),
  };
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--templates") options.templates = argv[++index];
    else if (argument === "--inventory") options.inventory = argv[++index];
    else if (argument === "--drafts") options.drafts = argv[++index];
    else if (argument === "--output" || argument === "-o") options.output = argv[++index];
    else if (argument === "-h" || argument === "--help") {
      console.log("usage: overlay_template_match.ts [--templates FILE] [--inventory FILE] [--drafts DIR] [-o DIR]");
      process.exit(0);
    } else throw new Error(`unrecognized argument: ${argument}`);
  }
  return options;
}

function declaredFunctions(source: string): string[] {
  return [...source.matchAll(DECLARATION)].map((match) => match[1]);
}

function adapt(template: string, draft: string): { source: string; mappings: Record<string, string> } | null {
  const templateDefinition = DEFINITION.exec(template)?.[1];
  const draftDefinition = DEFINITION.exec(draft)?.[1];
  if (templateDefinition === undefined || draftDefinition === undefined) return null;
  const templateCalls = declaredFunctions(template).filter((name) => name !== templateDefinition);
  const draftCalls = declaredFunctions(draft).filter((name) => name !== draftDefinition);
  if (templateCalls.length !== draftCalls.length) return null;
  const mappings: Record<string, string> = { [templateDefinition]: draftDefinition };
  for (let index = 0; index < templateCalls.length; index++) mappings[templateCalls[index]] = draftCalls[index];
  let source = template;
  for (const [from, to] of Object.entries(mappings)) source = source.replaceAll(from, to);
  // m2c exposes signed comparison literals that may be supplied by an
  // adjacent overlay function rather than a private pool. Preserve the proven
  // source shape while adapting that literal with an explicit signed cast.
  const draftLowerBound = /if\s*\(\(s32\)\s*\w+\s*<\s*(0x[0-9a-f]+)\s*\)/i.exec(draft)?.[1];
  if (draftLowerBound !== undefined) {
    source = source
      .replace(/\bs16 var_r0;/, "s32 var_r0;")
      .replace(/var_r0 = temp - temp_r3;/, "var_r0 = (s16)(temp - temp_r3);")
      .replace(/<\s*-0x[0-9a-f]+/i, `< (s32)${draftLowerBound}`);
  }
  return { source, mappings };
}

async function main(): Promise<void> {
  const options = optionsOf(Bun.argv.slice(2));
  const templateReport = JSON.parse(readFileSync(options.templates, "utf8")) as {
    exact_hits: Array<{ id: string; exact_templates: string[] }>;
  };
  const inventory = JSON.parse(readFileSync(options.inventory, "utf8")) as { functions: FunctionRow[] };
  const functions = new Map(inventory.functions.map((fn) => [fn.id, fn]));
  const overlays = new Map<string, Buffer>();
  const results: Array<Record<string, unknown>> = [];
  for (const hit of templateReport.exact_hits) {
    const fn = functions.get(hit.id);
    if (fn === undefined) continue;
    if (!overlays.has(fn.overlay)) {
      overlays.set(fn.overlay, assembleOverlay(join(ROOT, "assets/code", fn.overlay, "overlay.s"), OVERLAY_BASE));
    }
    const stem = fn.entry.toString(16).padStart(8, "0");
    const draft = join(options.drafts, fn.overlay, fn.offset.toString(16).padStart(4, "0"), `${stem}.c`);
    let draftSource: string;
    try { draftSource = readFileSync(draft, "utf8"); }
    catch { results.push({ id: hit.id, matched: false, error: "missing draft" }); continue; }
    let exact = false;
    let bestMismatch = Number.MAX_SAFE_INTEGER;
    let bestSize: number | null = null;
    for (const templatePath of hit.exact_templates) {
      const adapted = adapt(readFileSync(join(ROOT, templatePath), "utf8"), draftSource);
      if (adapted === null) continue;
      const directory = join(options.output, fn.overlay, fn.offset.toString(16).padStart(4, "0"));
      const build = join(directory, "build");
      mkdirSync(build, { recursive: true });
      const candidate = join(directory, `${stem}.c`);
      writeFileSync(candidate, adapted.source);
      try {
        const verification = await verifyCandidate(candidate, overlays.get(fn.overlay)!, build, [], OVERLAY_BASE);
        const expected = overlays.get(fn.overlay)!.subarray(fn.offset, fn.offset + fn.span_bytes);
        let mismatch = Math.abs(verification.actual.length - expected.length);
        for (let byte = 0; byte < Math.min(verification.actual.length, expected.length); byte++) {
          if (verification.actual[byte] !== expected[byte]) mismatch++;
        }
        if (mismatch < bestMismatch) { bestMismatch = mismatch; bestSize = verification.size; }
        if (verification.size === fn.span_bytes && verification.actual.equals(expected)) {
          results.push({ id: hit.id, matched: true, size: verification.size, template: templatePath, source: candidate, mappings: adapted.mappings });
          console.log(`exact ${hit.id} template=${templatePath} bytes=${verification.size}`);
          exact = true;
          break;
        }
      } catch {}
    }
    if (!exact) results.push({
      id: hit.id,
      matched: false,
      mismatched_bytes: bestMismatch === Number.MAX_SAFE_INTEGER ? null : bestMismatch,
      size: bestSize,
    });
  }
  const matches = results.filter((result) => result.matched === true).length;
  mkdirSync(options.output, { recursive: true });
  writeFileSync(join(options.output, "report.json"), JSON.stringify({ format: 1, targets: templateReport.exact_hits.length, matches, results }, null, 2) + "\n");
  console.log(`targets=${templateReport.exact_hits.length} matches=${matches} report=${join(options.output, "report.json")}`);
}

if (import.meta.main) await main();
