#!/usr/bin/env bun
// Draft the C for straight-line overlay wrapper functions.
//
// Why this exists: the overlay strict queue has a long tail of functions whose
// whole body is constant materialisation plus direct calls — setup wrappers,
// dispatch stubs, forwarders. They convert on the first probe once the argument
// count, the argument-setter order and the constant spelling are right, and all
// three are readable straight off the disassembly. Writing them by hand costs a
// few minutes each and reading them wrong costs a probe; deriving them costs
// neither. 53 of the 96 rows this recognised on 2026-07-30 were byte-exact on
// the first probe.
//
// This only *drafts*. `overlay_adopt.ts` remains the oracle — run the draft
// through a dry run and keep the zeros. A rejected draft means the shape needed
// a lever this does not model, not that the row is unconvertible.
//
// Three rules from HANDOVER §4 are encoded here, and they are the whole reason
// the hit rate is what it is:
//
//   * Argument-setter order fixes the callee's declared return type. A reference
//     that sets r1 before r0 came from an `s32`-returning callee; r0 first came
//     from a `void` one. Getting this backwards changes the emitted order and
//     costs the match.
//   * A pool-loaded constant that also factorises as `k << n` with `k <= 255`
//     must be forced back into the pool with `(s32)&Value_xxxxxxxx`, because a
//     plain literal would be built with `movs`/`lsls` instead. A pool-loaded
//     constant that does *not* factorise pools by itself and wants the literal.
//   * `pop {rN}` for N != 0 in the epilogue means r0 is live across the return,
//     so the function returns a value; `pop {r0}` means it does not.
import { existsSync, mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const OVERLAY_BASE = 0x02000000;

type Operand = { kind: "literal" | "pooled"; value: number };
interface Call {
  target: string;
  args: Operand[];
  r1BeforeR0: boolean;
}
export interface Wrapper {
  returnsValue: boolean;
  calls: Call[];
  trailingConstant: number | null;
}

// A constant gcc can build in two instructions (`movs rN,#k` then `lsls rN,#n`)
// is never pooled from a plain literal, so a reference that pools it needs the
// symbol spelling instead.
export function factorises(value: number): boolean {
  if (value === 0) return true;
  for (let shift = 0; shift < 32; shift++) {
    if (value % (1 << shift) !== 0) continue;
    const base = Math.floor(value / (1 << shift));
    if (base >= 0 && base <= 255) return true;
  }
  return false;
}

function hex8(value: number): string {
  return (value >>> 0).toString(16).padStart(8, "0");
}

// Parse the decoded body. Returns null the moment anything outside the modelled
// vocabulary appears — a memory access, a branch, a saved register — because a
// half-understood body produces a draft that wastes a probe and, worse, reads as
// authoritative to whoever picks it up next.
export function readWrapper(
  instructions: readonly string[],
  pool: ReadonlyMap<number, number>,
): Wrapper | null {
  if (instructions.length === 0) return null;
  const opening = instructions[0].replace(/\s+/g, " ");
  if (!opening.startsWith("push")) return null;
  // Any callee-saved register in the prologue means live state across a call,
  // which is exactly what this translator does not model.
  if (/r[4-9]|sl|fp|r1[01]/.test(opening)) return null;

  let body = instructions.slice(1);
  let epilogue: string[] = [];
  for (let index = 0; index < body.length; index++) {
    if (body[index].replace(/\s+/g, " ").startsWith("pop")) {
      epilogue = body.slice(index, index + 2);
      body = body.slice(0, index);
      break;
    }
  }
  if (epilogue.length !== 2) return null;
  const popped = epilogue[0].replace(/\s+/g, " ");
  if (!epilogue[1].replace(/\s+/g, " ").startsWith("bx")) return null;
  const returnsValue = !/\{\s*r0\s*\}/.test(popped);

  const registers = new Map<string, Operand>();
  let order: string[] = [];
  const calls: Call[] = [];

  for (const raw of body) {
    const text = raw.replace(/\s+/g, " ").trim();
    let found = /^movs? (r[0-3]), #(\d+)$/.exec(text);
    if (found !== null) {
      registers.set(found[1], { kind: "literal", value: Number(found[2]) });
      if (!order.includes(found[1])) order.push(found[1]);
      continue;
    }
    found = /^lsls (r[0-3]), (r[0-3]), #(\d+)$/.exec(text);
    if (found !== null && found[1] === found[2]) {
      const held = registers.get(found[1]);
      if (held === undefined || held.kind !== "literal") return null;
      registers.set(found[1], { kind: "literal", value: held.value * (1 << Number(found[3])) });
      continue;
    }
    found = /^negs (r[0-3]), (r[0-3])$/.exec(text);
    if (found !== null && found[1] === found[2]) {
      const held = registers.get(found[1]);
      if (held === undefined || held.kind !== "literal") return null;
      registers.set(found[1], { kind: "literal", value: -held.value });
      continue;
    }
    found = /^ldr (r[0-3]), \[pc, #\d+\] @ \(0x([0-9a-f]+)\)$/.exec(text);
    if (found !== null) {
      const word = pool.get(Number.parseInt(found[2], 16));
      if (word === undefined) return null;
      registers.set(found[1], { kind: "pooled", value: word });
      if (!order.includes(found[1])) order.push(found[1]);
      continue;
    }
    found = /^bl 0x([0-9a-f]+)$/.exec(text);
    if (found !== null) {
      const present: number[] = [];
      for (let index = 0; index < 4; index++) {
        if (registers.has(`r${index}`)) present.push(index);
      }
      // The arguments must be a contiguous run from r0; a gap means a register
      // carried a value this translator never saw set.
      const arity = present.length === 0 ? 0 : present[present.length - 1] + 1;
      for (let index = 0; index < arity; index++) {
        if (!present.includes(index)) return null;
      }
      const args: Operand[] = [];
      for (let index = 0; index < arity; index++) args.push(registers.get(`r${index}`)!);
      const zero = order.indexOf("r0");
      const one = order.indexOf("r1");
      calls.push({
        target: found[1],
        args,
        r1BeforeR0: one !== -1 && (zero === -1 || one < zero),
      });
      registers.clear();
      order = [];
      continue;
    }
    return null;
  }

  if (calls.length === 0) return null;
  let trailingConstant: number | null = null;
  if (registers.size > 0) {
    // Anything still live after the last call can only be the returned constant.
    const held = registers.get("r0");
    if (registers.size !== 1 || held === undefined || held.kind !== "literal") return null;
    trailingConstant = held.value;
  }
  return { returnsValue, calls, trailingConstant };
}

export function renderWrapper(address: number, wrapper: Wrapper): string {
  const { returnsValue, calls, trailingConstant } = wrapper;
  const symbols: number[] = [];
  for (const call of calls) {
    for (const arg of call.args) {
      if (arg.kind === "pooled" && arg.value >= 0 && factorises(arg.value) &&
          !symbols.includes(arg.value)) {
        symbols.push(arg.value);
      }
    }
  }
  const lastTarget = calls[calls.length - 1].target;
  const declared = new Map<string, string>();
  for (const call of calls) {
    const tail = call.target === lastTarget && returnsValue && trailingConstant === null;
    const type = call.r1BeforeR0 || tail ? "s32" : "void";
    if (!declared.has(call.target)) declared.set(call.target, type);
  }

  const lines = ["typedef signed int s32;"];
  if (symbols.length > 0) lines.push("typedef unsigned char u8;");
  lines.push("");
  for (const value of symbols) lines.push(`extern u8 Value_${hex8(value)};`);
  if (symbols.length > 0) lines.push("");
  for (const [target, type] of declared) {
    const call = calls.find((entry) => entry.target === target)!;
    const parameters = call.args.length === 0 ? "void" : call.args.map(() => "s32").join(", ");
    lines.push(`extern ${type} Func_0${target}(${parameters});`);
  }
  lines.push("");
  lines.push(`${returnsValue ? "s32" : "void"} Func_${hex8(address)}(void)`);
  lines.push("{");
  calls.forEach((call, index) => {
    const rendered = call.args.map((arg) => {
      if (arg.kind === "pooled" && arg.value >= 0 && factorises(arg.value)) {
        return `(s32)&Value_${hex8(arg.value)}`;
      }
      // Small counts and indices read as decimal the way the reference sources
      // spell them; anything wider is an address or a packed word and reads as
      // hex. The choice is cosmetic — both spell the same constant.
      if (arg.value >= -128 && arg.value <= 255) return String(arg.value);
      return `0x${(arg.value >>> 0).toString(16).toUpperCase()}`;
    });
    const last = index === calls.length - 1;
    const lead = returnsValue && last && trailingConstant === null ? "    return " : "    ";
    lines.push(`${lead}Func_0${call.target}(${rendered.join(", ")});`);
  });
  if (trailingConstant !== null) lines.push(`    return ${trailingConstant};`);
  lines.push("}");
  return lines.join("\n") + "\n";
}

const LISTING = /^\s*[0-9a-f]+:\t[0-9a-f ]+\t(.*)$/;
const POOL_ENTRY = /0x([0-9a-f]+) = 0x([0-9a-f]+)/g;

export function parseOverlayShow(output: string): {
  instructions: string[];
  pool: Map<number, number>;
} {
  const instructions: string[] = [];
  for (const line of output.split(/\r?\n/)) {
    const found = LISTING.exec(line);
    if (found !== null) instructions.push(found[1].trim());
  }
  const pool = new Map<number, number>();
  for (const match of output.matchAll(POOL_ENTRY)) {
    pool.set(Number.parseInt(match[1], 16), Number.parseInt(match[2], 16));
  }
  return { instructions, pool };
}

interface Row {
  overlay: string;
  offset: number;
  entry: number;
  span_bytes: number;
  contained_by?: unknown[];
  starts_with_prologue?: boolean;
  structural_veneer?: boolean;
  data_walk?: boolean;
  returns?: number;
}

async function disassemble(overlay: string, offset: number, span: number): Promise<string> {
  const shown = Bun.spawnSync([
    process.execPath, join(ROOT, "tools/overlay_show.ts"),
    overlay, offset.toString(16).padStart(4, "0"), "-n", String(span),
  ], { cwd: ROOT, stdout: "pipe", stderr: "pipe" });
  return Buffer.from(shown.stdout).toString("utf8");
}

function selfTest(): void {
  const instructions = [
    "push\t{lr}",
    "bl\t0x2001344",
    "ldr\tr0, [pc, #20]\t@ (0x20002a4)",
    "bl\t0x20013c2",
    "movs\tr1, #0",
    "movs\tr0, #12",
    "bl\t0x20013e2",
    "bl\t0x200135e",
    "pop\t{r0}",
    "bx\tr0",
    "asrs\tr3, r6, #14",
  ];
  const pool = new Map([[0x020002a4, 0x13b3]]);
  const wrapper = readWrapper(instructions, pool);
  if (wrapper === null) throw new Error("wrapper self-test failed to parse the reference shape");
  if (wrapper.returnsValue || wrapper.calls.length !== 4) {
    throw new Error("wrapper self-test read the wrong call shape");
  }
  // 0x13b3 is odd, so it pools by itself and must stay a literal.
  const rendered = renderWrapper(0x02000288, wrapper);
  if (!rendered.includes("Func_020013c2(0x13B3);") ||
      !rendered.includes("extern s32 Func_020013e2(s32, s32);") ||
      !rendered.includes("Func_020013e2(12, 0);") ||
      rendered.includes("Value_")) {
    throw new Error("wrapper self-test rendered the wrong source");
  }
  // A pooled constant that factorises must come back as the forced symbol.
  const forced = readWrapper([
    "push\t{lr}", "ldr\tr0, [pc, #8]\t@ (0x2000010)", "bl\t0x2001000", "pop\t{r0}", "bx\tr0",
  ], new Map([[0x02000010, 0x1420]]));
  if (forced === null) throw new Error("wrapper self-test failed on the pooled-symbol shape");
  if (!renderWrapper(0x02000000, forced).includes("(s32)&Value_00001420")) {
    throw new Error("wrapper self-test did not force a factorising pool word");
  }
  // A body carrying a load or a branch is out of scope and must be refused.
  if (readWrapper(["push\t{lr}", "ldrh\tr3, [r0, #6]", "bl\t0x2001000", "pop\t{r0}", "bx\tr0"],
                  new Map()) !== null) {
    throw new Error("wrapper self-test accepted an unmodelled body");
  }
  if (!factorises(0x1420) || !factorises(0x13c0) || factorises(0x13b3) || factorises(0x953)) {
    throw new Error("wrapper self-test factorisation is wrong");
  }
  console.log("self-test=ok overlay-wrapper-draft");
}

async function main(): Promise<void> {
  const argv = Bun.argv.slice(2);
  if (argv.includes("--self-test")) {
    selfTest();
    return;
  }
  if (argv.includes("-h") || argv.includes("--help")) {
    console.log("usage: overlay_wrapper_draft.ts [--inventory FILE] [--out DIR] [--max-span N]");
    return;
  }
  const inventoryPath = argv.includes("--inventory")
    ? argv[argv.indexOf("--inventory") + 1]
    : join(ROOT, "out/decomp/overlays.json");
  const outputDirectory = argv.includes("--out")
    ? argv[argv.indexOf("--out") + 1]
    : join(ROOT, "work/overlay-wrappers");
  const maxSpan = argv.includes("--max-span")
    ? Number(argv[argv.indexOf("--max-span") + 1])
    : 256;
  if (!existsSync(inventoryPath)) {
    throw new Error(`missing ${inventoryPath}; run tools/overlay_inventory.ts first`);
  }
  const inventory = JSON.parse(readFileSync(inventoryPath, "utf8")) as { functions: Row[] };
  mkdirSync(outputDirectory, { recursive: true });

  let drafted = 0;
  let skipped = 0;
  for (const row of inventory.functions) {
    if ((row.contained_by ?? []).length > 0) continue;
    if (row.starts_with_prologue !== true) continue;
    if (row.structural_veneer === true || row.data_walk === true) continue;
    if ((row.returns ?? 0) === 0) continue;
    if (row.span_bytes > maxSpan) continue;
    const installed = join(ROOT, `exact/${row.overlay}_c_${hex8(row.entry)}.c`);
    if (existsSync(installed)) continue;
    const { instructions, pool } = parseOverlayShow(
      await disassemble(row.overlay, row.offset, row.span_bytes),
    );
    const wrapper = readWrapper(instructions, pool);
    if (wrapper === null) {
      skipped++;
      continue;
    }
    const stem = hex8(row.entry);
    writeFileSync(join(outputDirectory, `${stem}.c`), renderWrapper(row.entry, wrapper));
    console.log(`${row.overlay}:${row.offset.toString(16).padStart(4, "0")} span=${row.span_bytes} draft=${stem}.c`);
    drafted++;
  }
  console.log(`drafted=${drafted} unmodelled=${skipped} out=${outputDirectory}`);
}

if (import.meta.main) {
  await main();
}
