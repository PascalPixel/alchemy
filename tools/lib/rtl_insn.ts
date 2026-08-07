// Typed extraction of RTL instructions and expressions from a gcc RTL dump,
// built on the generic S-expression parser in rtl_sexpr.ts. This is the
// layer candidate_explain.ts actually reasons over: instead of grepping dump
// text for a UID substring, it gets a real `RtlInsn` with a typed `RtlExpr`
// tree for its SET destination/source (when the pattern is a plain SET),
// falling back to an `unknown` node -- never a thrown error -- for anything
// this module does not model (PARALLEL bodies beyond a leading SET, inline
// asm, unrecognized rtx codes).
//
// Scope: only the rtx codes that matter for a Thumb -O2 ALU/load/store/call
// residual are modelled (reg, mem, const_int, symbol_ref, subreg, and the
// binary arithmetic/logic/shift codes). This is intentionally not a full RTL
// interpreter.
import { type SExpr, firstAtomDeep, head, parseAll, parseTag, render } from "./rtl_sexpr.ts";

export type RtlExpr =
  | { kind: "reg"; number: number; name: string }
  | { kind: "const_int"; value: number }
  | { kind: "symbol_ref"; name: string }
  | { kind: "label_ref"; target: string }
  | { kind: "mem"; address: RtlExpr }
  | { kind: "subreg"; inner: RtlExpr }
  | { kind: "binary"; op: BinaryOp; a: RtlExpr; b: RtlExpr }
  | { kind: "unary"; op: string; a: RtlExpr }
  | { kind: "call"; target: RtlExpr }
  | { kind: "unknown"; raw: string };

export type BinaryOp = "plus" | "minus" | "mult" | "and" | "ior" | "xor" | "ashift" | "ashiftrt" | "lshiftrt";
const BINARY_OPS = new Set<string>(["plus", "minus", "mult", "and", "ior", "xor", "ashift", "ashiftrt", "lshiftrt"]);

// LOG_LINKS: the insns THIS insn depends on (predecessors), each tagged with
// a dependency kind. gcc's dump leaves a true (data) dependency untagged --
// a bare `insn_list` -- and marks anti/output dependencies explicitly, e.g.
// `insn_list:REG_DEP_ANTI`. This mirrors REG_NOTE_KIND(link)==0 meaning
// "data dependence" in haifa-sched.c's rank_for_schedule (see rtl_schedule.ts).
export type DependencyKind = "true" | "anti" | "output";
export interface Dependency { uid: number; kind: DependencyKind; }

export interface RtlInsn {
  uid: number;
  kind: "insn" | "call_insn" | "jump_insn";
  code: string | null; // the pattern's own head: "set", "parallel", "use", "unspec_volatile", ...
  set: { dest: RtlExpr; src: RtlExpr } | null;
  callTarget: RtlExpr | null;
  dependencies: Dependency[];
  raw: string;
}

export function parseRtlExpr(expr: SExpr): RtlExpr {
  if (expr.kind === "atom") {
    const asNumber = Number(expr.value);
    if (Number.isFinite(asNumber) && /^-?\d+$/.test(expr.value)) return { kind: "const_int", value: asNumber };
    return { kind: "unknown", raw: expr.value };
  }
  const rawHead = head(expr);
  if (rawHead === null) return { kind: "unknown", raw: render(expr) };
  const { tag } = parseTag(rawHead);

  if (tag === "reg") {
    const number = Number(expr.items[1]?.kind === "atom" ? expr.items[1].value : Number.NaN);
    const name = expr.items[2]?.kind === "atom" ? expr.items[2].value : `r${number}`;
    if (Number.isFinite(number)) return { kind: "reg", number, name };
  }
  if (tag === "const_int") {
    const value = Number(expr.items[1]?.kind === "atom" ? expr.items[1].value : Number.NaN);
    if (Number.isFinite(value)) return { kind: "const_int", value };
  }
  if (tag === "symbol_ref") {
    const name = (firstAtomDeep(expr.items[1] ?? expr.items[0]) ?? "").replace(/^"|"$/g, "");
    return { kind: "symbol_ref", name };
  }
  if (tag === "label_ref") {
    const target = expr.items[1]?.kind === "atom" ? expr.items[1].value : (firstAtomDeep(expr) ?? "");
    return { kind: "label_ref", target };
  }
  if (tag === "mem") {
    return { kind: "mem", address: parseRtlExpr(expr.items[1]) };
  }
  if (tag === "subreg") {
    return { kind: "subreg", inner: parseRtlExpr(expr.items[1]) };
  }
  if (BINARY_OPS.has(tag) && expr.items.length >= 3) {
    return { kind: "binary", op: tag as BinaryOp, a: parseRtlExpr(expr.items[1]), b: parseRtlExpr(expr.items[2]) };
  }
  if (tag === "not" || tag === "neg" || tag === "sign_extend" || tag === "zero_extend") {
    return { kind: "unary", op: tag, a: parseRtlExpr(expr.items[1]) };
  }
  if (tag === "call") {
    return { kind: "call", target: parseRtlExpr(expr.items[1]) };
  }
  return { kind: "unknown", raw: render(expr) };
}

// A SET's destination is often a REG, but can be a MEM (a store: `(set (mem
// ...) (reg ...))`) or a SUBREG. Anything qualifies as long as it is not
// itself the src side -- extraction here never needs to reject a shape.
function extractSet(pattern: SExpr): { dest: RtlExpr; src: RtlExpr } | null {
  if (head(pattern) !== "set" || pattern.kind !== "list" || pattern.items.length < 3) return null;
  return { dest: parseRtlExpr(pattern.items[1]), src: parseRtlExpr(pattern.items[2]) };
}

// A PARALLEL's first vector member is almost always the operative SET (or a
// call-value SET) for the call/return patterns this tool cares about; the
// remaining members are USE/CLOBBER bookkeeping. Only the first is surfaced.
function extractFromPattern(pattern: SExpr): { code: string | null; set: RtlInsn["set"]; callTarget: RtlExpr | null } {
  const code = head(pattern);
  if (code === "set") {
    const set = extractSet(pattern);
    const callTarget = set !== null && set.src.kind === "call" ? set.src.target : null;
    return { code, set, callTarget };
  }
  if (code === "parallel" && pattern.kind === "list") {
    const body = pattern.items[1];
    const first = body?.kind === "list" ? body.items[0] : undefined;
    if (first !== undefined && head(first) === "set") {
      const set = extractSet(first);
      const callTarget = set !== null && set.src.kind === "call" ? set.src.target : null;
      return { code, set, callTarget };
    }
    return { code, set: null, callTarget: null };
  }
  if (code === "call") {
    // Only the list variant carries items; an atom here means the dump was not
    // the shape this branch assumes, which is worth failing on rather than
    // reading `undefined` off an atom.
    if (pattern.kind !== "list") throw new Error(`call pattern is an atom: ${pattern.value}`);
    return { code, set: null, callTarget: parseRtlExpr(pattern.items[1]) };
  }
  return { code, set: null, callTarget: null };
}

// LOG_LINKS is a hand-rolled linked list, not a normal S-expression list:
// `(insn_list[:KIND] UID (insn_list[:KIND] UID (nil)))`, printed as gcc's
// generic INSN_LIST/EXPR_LIST chain format. Walks it iteratively; stops at
// `(nil)` or anything that does not keep the chain shape, rather than
// throwing -- a REG_NOTES chain (`expr_list:REG_DEAD ...`) can appear in the
// same position on some insns and must not be misread as dependencies.
function parseLogLinks(chain: SExpr | undefined): Dependency[] {
  const dependencies: Dependency[] = [];
  let node = chain;
  while (node !== undefined && node.kind === "list") {
    const rawHead = head(node);
    if (rawHead === null) break;
    // The dependency kind rides in the MODE slot, not a `/flag`:
    // `insn_list:REG_DEP_ANTI` -> tag "insn_list", mode "REG_DEP_ANTI".
    const { tag, mode } = parseTag(rawHead);
    if (tag !== "insn_list") break;
    const uidAtom = node.items[1];
    const uid = uidAtom?.kind === "atom" ? Number(uidAtom.value) : Number.NaN;
    if (!Number.isFinite(uid)) break;
    const kind: DependencyKind = mode === "REG_DEP_ANTI" ? "anti" : mode === "REG_DEP_OUTPUT" ? "output" : "true";
    dependencies.push({ uid, kind });
    node = node.items[2];
  }
  return dependencies;
}

const INSN_KINDS = new Set(["insn", "call_insn", "jump_insn"]);

// Parses every top-level `(insn|call_insn|jump_insn UID PREV NEXT PATTERN
// ...)` statement in a dump, in the order they appear in the file. For a
// post-scheduling dump (the -dR / .sched2 file this tool reads) that order
// IS the final emitted instruction order: confirmed by inspection, not
// documented anywhere, since these forms only occur once in that file, all
// after the human-readable scheduling trace text (which parseAll already
// strips as `;;` comments).
export function parseInsns(dumpText: string): RtlInsn[] {
  const forms = parseAll(dumpText);
  const insns: RtlInsn[] = [];
  for (const form of forms) {
    if (form.kind !== "list" || form.items.length < 4) continue;
    const rawHead = head(form);
    if (rawHead === null || !INSN_KINDS.has(rawHead)) continue;
    const uid = Number(form.items[1]?.kind === "atom" ? form.items[1].value : Number.NaN);
    if (!Number.isFinite(uid)) continue;
    // Layout is (KIND UID PREV NEXT PATTERN ...): the pattern is the fifth
    // list member, index 4, not 3 -- easy to mis-count since UID/PREV/NEXT
    // read like a compact triple but the head atom occupies index 0 too.
    const pattern = form.items[4];
    const extracted = pattern === undefined ? { code: null, set: null, callTarget: null } : extractFromPattern(pattern);
    // Layout continues (KIND UID PREV NEXT PATTERN CODE {NAME} LOG_LINKS
    // NOTES...): LOG_LINKS is index 7. It is defensively re-found by shape
    // (head "insn_list", or the literal "nil") if index 7 is not it, since a
    // pattern with no matched template name can shift the field over by one.
    const expectedLinks = form.items[7];
    const logLinks = expectedLinks !== undefined && (head(expectedLinks) === "insn_list" || (expectedLinks.kind === "atom" && expectedLinks.value === "nil"))
      ? expectedLinks
      : form.items.slice(5).find((item) => head(item) === "insn_list");
    insns.push({
      uid,
      kind: rawHead as RtlInsn["kind"],
      code: extracted.code,
      set: extracted.set,
      callTarget: extracted.callTarget,
      dependencies: parseLogLinks(logLinks),
      raw: render(form),
    });
  }
  return insns;
}

// A CALL's operand is the memory location holding the callee
// (`(call (mem (symbol_ref "NAME")) args)`), so the readable name sits one
// MEM layer under `callTarget`. Returns null for indirect calls (register
// target) rather than guessing.
export function calleeSymbol(insn: RtlInsn): string | null {
  const target = insn.callTarget;
  if (target === null) return null;
  const callee = target.kind === "mem" ? target.address : target;
  return callee.kind === "symbol_ref" ? callee.name : null;
}

// The destination register an insn writes, if any -- the single strongest,
// cheapest correlation signal against a disassembled instruction's own
// destination operand (see rtl_align.ts).
export function destRegister(insn: RtlInsn): number | null {
  const dest = insn.set?.dest;
  if (dest === undefined || dest === null) return null;
  if (dest.kind === "reg") return dest.number;
  if (dest.kind === "subreg" && dest.inner.kind === "reg") return dest.inner.number;
  return null;
}

function selfTest(): void {
  // A trimmed but real excerpt: a plain ALU insn, a store, and a call insn,
  // exactly as gcc-2.96 prints them in a -dR/.sched2 dump (verified against
  // a real compile of resource_385:0314 during development).
  const dump = `
;; Ready list (t = 4): 19 28
;;		--> scheduling insn <<<28>>> on unit core

(insn 28 15 17 (set (reg:SI 3 r3)
        (plus:SI (reg/v:SI 3 r3)
            (reg/v:SI 2 r2))) 5 {*thumb_addsi3} (nil))

(insn 30 28 32 (set (mem:QI (plus:SI (reg/f:SI 0 r0)
                (const_int 9 [0x9])) [0 S1 A8])
        (reg:SI 3 r3)) 174 {*movqi_insn} (insn_list 28 (insn_list:REG_DEP_ANTI 17 (nil)))
    (expr_list:REG_DEAD (reg:SI 3 r3)
        (nil)))

(call_insn 67 58 71 (parallel [
            (set (reg:SI 0 r0)
                (call (mem:SI (symbol_ref:SI ("Func_02001508")) 0)
                    (const_int 0 [0x0])))
            (use (const_int 0 [0x0]))
            (clobber (reg:SI 14 lr))
        ] ) 245 {*call_value_insn} (nil))
`;
  const insns = parseInsns(dump);
  if (insns.length !== 3) throw new Error(`expected 3 insns, got ${insns.length}`);

  const [add, store, call] = insns;
  if (add.uid !== 28 || add.kind !== "insn") throw new Error(`wrong first insn: ${JSON.stringify(add)}`);
  if (destRegister(add) !== 3) throw new Error(`expected add to write r3, got ${destRegister(add)}`);
  if (add.set?.src.kind !== "binary" || add.set.src.op !== "plus") throw new Error("expected add src to be a plus");

  if (store.uid !== 30) throw new Error(`wrong second insn uid: ${store.uid}`);
  if (destRegister(store) !== null) throw new Error("a store's dest is a MEM, destRegister must be null");
  if (store.set?.dest.kind !== "mem") throw new Error(`expected store dest to be mem, got ${store.set?.dest.kind}`);
  if (store.dependencies.length !== 2) throw new Error(`expected 2 LOG_LINKS, got ${JSON.stringify(store.dependencies)}`);
  if (store.dependencies[0].uid !== 28 || store.dependencies[0].kind !== "true") {
    throw new Error(`expected an untagged (true) dependency on 28, got ${JSON.stringify(store.dependencies[0])}`);
  }
  if (store.dependencies[1].uid !== 17 || store.dependencies[1].kind !== "anti") {
    throw new Error(`expected a REG_DEP_ANTI dependency on 17, got ${JSON.stringify(store.dependencies[1])}`);
  }
  if (add.dependencies.length !== 0) throw new Error(`expected no dependencies on a (nil) LOG_LINKS, got ${JSON.stringify(add.dependencies)}`);

  if (call.uid !== 67 || call.kind !== "call_insn") throw new Error(`wrong third insn: ${JSON.stringify(call)}`);
  // A CALL's operand is the memory location holding the callee, so the
  // symbol sits one MEM layer under callTarget: `(call (mem (symbol_ref
  // "NAME")) args)`, not a bare symbol_ref -- this reflects real RTL shape.
  if (calleeSymbol(call) !== "Func_02001508") {
    throw new Error(`expected call target Func_02001508, got ${calleeSymbol(call)}`);
  }

  console.log("self-test=ok tool=rtl-insn");
}

if (import.meta.main && Bun.argv.includes("--self-test")) selfTest();
