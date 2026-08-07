// A small, general-purpose parser for the Lisp-like S-expression syntax gcc's
// RTL dumps (-d<letter>, -fdump-rtl-*) print. This module knows nothing about
// RTL semantics -- it only turns the text into a typed tree so consumers
// (rtl_insn.ts) can pattern-match structurally instead of scraping strings.
//
// gcc's RTL print format is close to Lisp but not quite: tags carry a
// `/flag` and/or `:MODE` suffix glued directly onto the head atom with no
// space (`reg/v:SI`, `plus:SI`), `[...]` brackets group PARALLEL bodies and
// bracketed value hints (`(const_int 3 [0x3])`), and quoted strings hold
// symbol names. `;;` lines are human-readable trace/commentary, never part
// of the S-expression content, and are stripped before tokenizing.

export type SExpr =
  | { kind: "atom"; value: string }
  | { kind: "list"; bracket: boolean; items: SExpr[] };

export function atom(value: string): SExpr {
  return { kind: "atom", value };
}

export function tokenize(text: string): string[] {
  const stripped = text
    .split("\n")
    .filter((line) => !line.trim().startsWith(";;"))
    .join("\n");
  const tokens: string[] = [];
  let index = 0;
  while (index < stripped.length) {
    const char = stripped[index];
    if (/\s/.test(char)) { index++; continue; }
    if (char === "(" || char === ")" || char === "[" || char === "]") {
      tokens.push(char);
      index++;
      continue;
    }
    if (char === '"') {
      let end = index + 1;
      while (end < stripped.length && stripped[end] !== '"') end++;
      tokens.push(stripped.slice(index, end + 1));
      index = end + 1;
      continue;
    }
    let end = index;
    while (end < stripped.length && !/[\s()[\]]/.test(stripped[end])) end++;
    tokens.push(stripped.slice(index, end));
    index = end;
  }
  return tokens;
}

// Parses every top-level form in the token stream, tolerating an unbalanced
// trailing fragment (a dump file can be truncated by a compiler crash or a
// mid-write read) by returning what parsed cleanly rather than throwing.
export function parseSExprs(tokens: string[]): SExpr[] {
  let position = 0;
  function parseOne(): SExpr {
    const token = tokens[position];
    if (token === "(" || token === "[") {
      const bracket = token === "[";
      position++;
      const items: SExpr[] = [];
      const close = bracket ? "]" : ")";
      while (position < tokens.length && tokens[position] !== close) {
        items.push(parseOne());
      }
      if (tokens[position] === close) position++;
      return { kind: "list", bracket, items };
    }
    position++;
    return { kind: "atom", value: token };
  }
  const results: SExpr[] = [];
  while (position < tokens.length) {
    if (tokens[position] === ")" || tokens[position] === "]") { position++; continue; }
    results.push(parseOne());
  }
  return results;
}

export function parseAll(text: string): SExpr[] {
  return parseSExprs(tokenize(text));
}

// The head of a list, e.g. `(plus:SI (reg ...) (reg ...))` -> "plus:SI". A
// non-list or empty list has no head.
export function head(expr: SExpr): string | null {
  if (expr.kind !== "list" || expr.items.length === 0) return null;
  const first = expr.items[0];
  return first.kind === "atom" ? first.value : null;
}

// gcc glues `/flags` and `:MODE` onto the head atom with no separator:
// `reg/v:SI`, `insn_list:REG_DEP_ANTI`, `mem/f/c:SI`. Split it back apart.
export function parseTag(rawHead: string): { tag: string; flags: string[]; mode: string | null } {
  const [beforeMode, mode] = rawHead.includes(":")
    ? [rawHead.slice(0, rawHead.indexOf(":")), rawHead.slice(rawHead.indexOf(":") + 1)]
    : [rawHead, null];
  const [tag, ...flags] = beforeMode.split("/");
  return { tag, flags, mode };
}

// Descends into nested single-child lists to find the first real atom --
// handles gcc's occasional extra wrapping paren, e.g. the symbol name in
// `(symbol_ref:SI ("Func_02001508"))` is one list deep from where a bare
// `(symbol_ref:SI "Func_02001508")` would put it.
export function firstAtomDeep(expr: SExpr): string | null {
  if (expr.kind === "atom") return expr.value;
  for (const item of expr.items) {
    const found = firstAtomDeep(item);
    if (found !== null) return found;
  }
  return null;
}

export function render(expr: SExpr): string {
  if (expr.kind === "atom") return expr.value;
  const open = expr.bracket ? "[" : "(";
  const close = expr.bracket ? "]" : ")";
  return `${open}${expr.items.map(render).join(" ")}${close}`;
}

function selfTest(): void {
  const parsed = parseAll(
    ';; a trace comment, not content\n' +
    '(insn 15 10 17 (set (reg/v:SI 3 r3)\n' +
    '        (plus:SI (reg:SI 3 r3)\n' +
    '            (const_int 3 [0x3]))) 5 {*thumb_addsi3} (nil))\n',
  );
  if (parsed.length !== 1 || parsed[0].kind !== "list") throw new Error("expected one top-level list");
  if (head(parsed[0]) !== "insn") throw new Error(`expected head "insn", got ${head(parsed[0])}`);
  const pattern = parsed[0].items[4];
  if (head(pattern) !== "set") throw new Error(`expected pattern head "set", got ${head(pattern)}`);
  const dest = pattern.kind === "list" ? pattern.items[1] : undefined;
  const destHead = dest === undefined ? null : head(dest);
  if (destHead !== "reg/v:SI") throw new Error(`expected dest head "reg/v:SI", got ${destHead}`);
  const { tag, flags, mode } = parseTag(destHead);
  if (tag !== "reg" || flags.join(",") !== "v" || mode !== "SI") {
    throw new Error(`parseTag mis-split "${destHead}": tag=${tag} flags=${flags} mode=${mode}`);
  }
  const symbolWrapped = parseAll('(symbol_ref:SI ("Func_02001508"))')[0];
  const symbolArgument = symbolWrapped.kind === "list" ? symbolWrapped.items[1] : undefined;
  if (symbolArgument === undefined || firstAtomDeep(symbolArgument) !== '"Func_02001508"') {
    throw new Error(`firstAtomDeep did not descend through the extra wrapping paren: ${symbolArgument && firstAtomDeep(symbolArgument)}`);
  }
  const truncated = parseAll("(insn 1 0 2 (set (reg:SI 3");
  if (truncated.length !== 1) throw new Error("truncated input should still yield the partial form, not throw");
  console.log("self-test=ok tool=rtl-sexpr");
}

if (import.meta.main && Bun.argv.includes("--self-test")) selfTest();
