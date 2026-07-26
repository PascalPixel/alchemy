#!/usr/bin/env bun
// Expand one hand-derived C template across every member of an overlay shape
// cohort: rename its entry symbol per member, byte-verify, and adopt the exact
// ones through tools/overlay_adopt.ts.
//
// This is the lever the overlay work actually turns. Cohort expansion converted
// 131 of the 154 overlay functions recovered on 2026-07-25, against 19 from
// one-at-a-time hand work and none at all from the permuter and variant sweeps,
// which produced zero exact overlay matches across thousands of candidates.
// Deriving one template and expanding it is worth roughly an order of magnitude
// more than drafting members individually.
//
// A member whose constants differ from the template's simply fails to verify
// and is reported, not adopted: byte equality remains the only gate, and this
// tool never weakens it.
import { mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { basename, dirname, join } from "node:path";
import { verifyCandidate } from "./match_m2c.ts";
import { assembleOverlay, OVERLAY_BASE } from "./overlay_disasm.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const ENTRY_SYMBOL = /Func_[0-9a-f]{8}/;

interface Member { overlay: string; offset: number }
interface Options { shapes: string; index: number; template: string; apply: boolean; work: string }

function optionsOf(argv: string[]): Options {
  const options: Options = {
    shapes: join(ROOT, "out/decomp/overlay-shapes.json"),
    index: -1,
    template: "",
    apply: false,
    work: join(ROOT, "work/overlay-cohort"),
  };
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--shapes") options.shapes = argv[++index];
    else if (argument === "--cohort") options.index = Number(argv[++index]);
    else if (argument === "--template" || argument === "-t") options.template = argv[++index];
    else if (argument === "--apply") options.apply = true;
    else if (argument === "--work") options.work = argv[++index];
    else if (argument === "-h" || argument === "--help") {
      console.log("usage: overlay_cohort.ts --cohort N --template FILE [--shapes FILE] [--apply]");
      process.exit(0);
    } else throw new Error(`unrecognized argument: ${argument}`);
  }
  if (options.template === "") throw new Error("--template is required");
  if (!Number.isInteger(options.index) || options.index < 0) throw new Error("--cohort must be a non-negative index into the shapes report");
  return options;
}

// 雛形の入口シンボルだけを差し替える。定数は雛形のまま置くので、値が違う
// 個体は検証で落ちる。それが正しい: 一致は常にバイト比較で決める。
export function retarget(template: string, entry: number): string {
  const symbol = `Func_${entry.toString(16).padStart(8, "0")}`;
  if (!ENTRY_SYMBOL.test(template)) throw new Error("template has no Func_XXXXXXXX entry symbol");
  return template.replace(ENTRY_SYMBOL, symbol);
}

function selfTest(): void {
  const source = "void Func_02000030(void) { }\n";
  const out = retarget(source, 0x02001234);
  if (!out.includes("Func_02001234") || out.includes("Func_02000030")) {
    throw new Error("overlay cohort self-test: retarget failed");
  }
  let threw = false;
  try { retarget("void nothing(void) { }", 0x02000000); } catch { threw = true; }
  if (!threw) throw new Error("overlay cohort self-test: missing entry symbol must throw");
  console.log("overlay cohort self-test passed");
}

async function main(): Promise<void> {
  const argv = Bun.argv.slice(2);
  if (argv.includes("--self-test")) {
    selfTest();
    return;
  }
  const options = optionsOf(argv);
  const report = JSON.parse(readFileSync(options.shapes, "utf8")) as
    { cohorts: Array<{ count: number; code_bytes: number; members: Member[] }> };
  const cohort = report.cohorts[options.index];
  if (cohort === undefined) throw new Error(`no cohort at index ${options.index} (report has ${report.cohorts.length})`);
  const template = readFileSync(options.template, "utf8");

  const overlays = new Map<string, Buffer>();
  const exact: Array<{ overlay: string; offset: number; path: string; span: number }> = [];
  const missed: string[] = [];

  for (const member of cohort.members) {
    const entry = OVERLAY_BASE + member.offset;
    const stem = entry.toString(16).padStart(8, "0");
    if (!overlays.has(member.overlay)) {
      overlays.set(member.overlay, assembleOverlay(join(ROOT, "assets/code", `${member.overlay}_overlay.s`), OVERLAY_BASE));
    }
    const directory = join(options.work, member.overlay);
    mkdirSync(directory, { recursive: true });
    // 検証器は候補の語幹から番地を取るので、ファイル名は番地そのものにする。
    const candidate = join(directory, `${stem}.c`);
    writeFileSync(candidate, retarget(template, entry));
    const scratch = join(directory, stem);
    mkdirSync(scratch, { recursive: true });
    try {
      const verification = await verifyCandidate(candidate, overlays.get(member.overlay)!, scratch, [], OVERLAY_BASE);
      let mismatch = Math.abs(verification.actual.length - verification.expected.length);
      const shared = Math.min(verification.actual.length, verification.expected.length);
      for (let index = 0; index < shared; index++) {
        if (verification.actual[index] !== verification.expected[index]) mismatch++;
      }
      if (mismatch === 0) exact.push({ overlay: member.overlay, offset: member.offset, path: candidate, span: verification.actual.length });
      else missed.push(`${member.overlay}:${member.offset.toString(16).padStart(4, "0")} mismatched=${mismatch}`);
    } catch (error) {
      missed.push(`${member.overlay}:${member.offset.toString(16).padStart(4, "0")} ${(error as Error).message.slice(0, 60)}`);
    }
  }

  console.log(`cohort=${options.index} members=${cohort.members.length} exact=${exact.length} missed=${missed.length} template=${basename(options.template)}`);
  for (const line of missed) console.log(`  miss ${line}`);

  if (!options.apply) {
    console.log(`(pass --apply to adopt the ${exact.length} exact members)`);
    return;
  }
  let adopted = 0;
  for (const member of exact) {
    const id = `${member.overlay}:${member.offset.toString(16).padStart(4, "0")}`;
    const result = Bun.spawnSync([
      "bun", "tools/overlay_adopt.ts", id, "--source", member.path, "--span", String(member.span), "--apply",
    ], { cwd: ROOT, stdout: "pipe", stderr: "pipe" });
    const output = result.stdout.toString() + result.stderr.toString();
    if (output.includes("adopt=applied")) adopted++;
    else console.log(`  adopt-refused ${id}: ${output.split("\n").filter((line) => /error/i.test(line)).slice(-1)[0] ?? ""}`.slice(0, 160));
  }
  console.log(`adopted=${adopted}`);
}

if (import.meta.main) await main();
