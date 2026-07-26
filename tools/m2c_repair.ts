// Repair the two m2c output patterns that make an otherwise usable draft fail
// to compile under the GCC 2.96 fork. Both come from m2c committing to a guess
// it later contradicts, and both have a mechanical repair that loses no
// information. This only makes a draft compilable; it makes no claim that the
// draft is correct, and every candidate is still verified byte-for-byte.

// Split an argument list on commas that sit at nesting depth zero, so that
// nested calls, casts, and bracketed subscripts count as one argument each.
function topLevelArguments(text: string): number {
  const body = text.trim();
  if (body === "") return 0;
  let depth = 0;
  let count = 1;
  for (const character of body) {
    if (character === "(" || character === "[") depth++;
    else if (character === ")" || character === "]") depth--;
    else if (character === "," && depth === 0) count++;
  }
  return count;
}

// Read the argument text of the call that starts at the open paren `open`.
function callArguments(text: string, open: number): { body: string; end: number } | null {
  let depth = 0;
  for (let index = open; index < text.length; index++) {
    const character = text[index];
    if (character === "(") depth++;
    else if (character === ")") {
      depth--;
      if (depth === 0) return { body: text.slice(open + 1, index), end: index };
    }
  }
  return null;
}

// m2c infers a callee's arity from one call site. When another site in the
// same function passes a different number of arguments, the prototype it wrote
// rejects the call. The honest repair is the unprototyped declaration: C89
// accepts any argument count through it, the integer arguments these functions
// take are unaffected by the absence of a prototype, and m2c already emits this
// form itself for callees it could not type at all. Guessing a arity instead
// would be inventing a signature the evidence does not support.
export function repairArityConflicts(draft: string): string {
  const declaration = /^([A-Za-z_][A-Za-z0-9_ *]*?)\s*\b(Func_[0-9a-f]{8})\s*\(([^;]*)\)\s*;\s*$/gm;
  const declared = new Map<string, { params: number; text: string }>();
  for (const match of draft.matchAll(declaration)) {
    const parameters = match[3].trim();
    if (parameters === "") continue; // already unprototyped
    declared.set(match[2], {
      params: parameters === "void" ? 0 : topLevelArguments(parameters),
      text: match[0],
    });
  }
  if (declared.size === 0) return draft;
  // Scan for call sites in the body only. Blanking the declarations keeps every
  // offset intact while making sure a prototype never counts as a call to
  // itself, which would make every declared callee look self-consistent.
  const body = draft.replace(declaration, (whole) => " ".repeat(whole.length));
  const conflicted = new Set<string>();
  for (const [name, info] of declared) {
    const call = new RegExp(`\\b${name}\\s*\\(`, "g");
    for (const site of body.matchAll(call)) {
      const args = callArguments(body, site.index! + site[0].length - 1);
      if (args === null) continue;
      if (topLevelArguments(args.body) !== info.params) conflicted.add(name);
    }
  }
  let repaired = draft;
  for (const name of conflicted) {
    const info = declared.get(name)!;
    const relaxed = info.text.replace(/\(([^;]*)\)/, "()");
    repaired = repaired.replace(info.text, relaxed);
  }
  return repaired;
}

// m2c writes `*(void *)ADDR` when it could not decide a load's width. That is
// not dereferenceable, so the draft dies on the first such site. Every one of
// these is used in an integer context, and m2c has usually already typed the
// same address concretely somewhere else in the same draft -- reuse that type
// rather than inventing one, and fall back to the register-width `s32 *` only
// when the address appears nowhere else.
export function repairVoidDereferences(draft: string): string {
  const typed = new Map<string, string>();
  for (const match of draft.matchAll(/\*\(\s*(s8|u8|s16|u16|s32|u32)\s*\*\s*\)\s*(0x[0-9A-Fa-f]+)/g)) {
    const address = match[2].toLowerCase();
    if (!typed.has(address)) typed.set(address, match[1]);
  }
  return draft.replace(
    /\*\(\s*void\s*\*\s*\)\s*(0x[0-9A-Fa-f]+)/g,
    (_whole, address: string) => `*(${typed.get(address.toLowerCase()) ?? "s32"} *)${address}`,
  );
}

// m2c writes its diagnostics to stdout, interleaved with the translation
// itself, and callers capture that stream as the draft.  A line such as
// `Warning: missing "bx $lr" in last block of Func_02003a92 (initial).' then
// lands in the C file and the compiler stops at `parse error before ":"',
// which reads as a broken candidate rather than as the tool note it is.  Drop
// those lines so the draft is C, and so the real defect -- a body m2c could
// not reconstruct -- shows up as a byte mismatch that can be measured.
export function repairDiagnosticLines(draft: string): string {
  return draft.replace(/^(?:Warning|Error|Note):[^\n]*\n?/gm, "");
}

// m2c emits a call through a literal address as `(u8 (*)(s32))0x020089F5(x)',
// where the cast binds to the call's result instead of to the callee.  The
// compiler rejects it with `called object is not a function'.  Parenthesise
// the callee so the cast applies to the address.
export function repairCalleeCasts(draft: string): string {
  return draft.replace(
    /\((\s*[A-Za-z_][\w ]*\(\s*\*\s*\)\s*\([^()]*\)\s*)\)\s*(0x[0-9A-Fa-f]+)\s*\(/g,
    (_whole, cast: string, address: string) => `((${cast})${address})(`,
  );
}

export function repairM2cDraft(draft: string): string {
  return repairArityConflicts(
    repairCalleeCasts(repairVoidDereferences(repairDiagnosticLines(draft))),
  );
}
