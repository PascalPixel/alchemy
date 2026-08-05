#!/usr/bin/env python3
"""Structural oracle over one reconstructed C owner, for alchemist.ts.

alchemist.ts models a function body as an array of source LINES and decides
which reorderings are legal with regular expressions. That model cannot see
C's actual structure, and every place it guessed wrong produced source that
still compiled to fewer differing bytes -- so the byte-diff gate accepted it
-- while meaning something different from what was written. Four separate
instances were found by reading its diffs:

  * a `} else if (x) {` line is one line playing two structural roles, and
    the renderer duplicated it (resource_371:28e8);
  * a split-store temp was declared `s32` and silently narrowed a `u8 *`
    (resource_3ae:e40);
  * a sink moved an assignment past a read of the same variable, reusing a
    stale value (resource_375:be0, resource_3c5:28a0);
  * a braceless `if (...)` was hoisted away from the statement it governs,
    making that statement conditional and the intended body unconditional
    (resource_3ba:31c0, resource_3bd:bc8).

This tool answers those questions from a real parse instead. It deliberately
does NOT rewrite or re-print the source: pycparser discards comments, and in
this corpus the header comments carry the reverse-engineering evidence (span
derivations, pool maps, call-site tables), so regenerating from the tree
would destroy the documentation. The tree is used as an ORACLE over source
line numbers; alchemist.ts keeps applying line-level edits, now checked
against real structure.

Requires pycparser (the same parser m2c uses for its C context files).

Output is one JSON object on stdout:
  {"function": name,
   "statements": [{"line", "end_line", "kind", "anchor", "governed_by",
                   "writes", "reads"}],
   "declarations": {name: type-string}}

`anchor` marks a construct whose position defines control flow -- a label, a
jump, or a braceless header -- which may never be moved or crossed.
`governed_by` is the line of the braceless header that owns this statement,
which may never be separated from it.
"""
import json
import re
import sys

try:
    from pycparser import c_ast, c_parser
except ImportError:  # pragma: no cover - environment guard
    # A fresh clone should not fail the aggregate suite over an uninstalled
    # optional parser, but a caller asking for real structure must never be
    # told "no anchors here" when the truth is "could not look" -- that would
    # silently disable the safety checks this tool exists to provide. So the
    # self-test skips and every other invocation errors loudly.
    if "--self-test" in sys.argv:
        print("self-test=skipped tool=c_structure (pycparser not installed; see requirements.txt)")
        sys.exit(0)
    print(json.dumps({"error": "pycparser is not installed (pip install -r requirements.txt)"}))
    sys.exit(2)

# The corpus's fixed type vocabulary, taken from include/types.h. Parsing the
# real preprocessor output would also work and is what the compiler sees, but
# these owners include exactly one header and it is this one, so inlining its
# typedefs keeps the tool a pure function of the source file.
PREAMBLE = """
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
"""
PREAMBLE_LINES = PREAMBLE.count("\n")

ANCHOR_NODES = (c_ast.Label, c_ast.Goto, c_ast.Return, c_ast.Break, c_ast.Continue,
                c_ast.Case, c_ast.Default)
BRACELESS_OWNERS = (c_ast.If, c_ast.For, c_ast.While, c_ast.DoWhile, c_ast.Switch)

# Every attribute under which a node can hang a nested statement list. Missing
# one silently drops that whole region from the report, and a caller cannot
# tell "no anchors here" from "never looked" -- which is exactly how a
# `switch` body went unanalysed and let a global assignment be hoisted above
# two `if (...) { break; }` blocks on resource_371:6ec, making it run on the
# break paths where it previously did not.
BODY_ATTRS = ("stmt", "iftrue", "iffalse", "body")


def preprocess(source: str) -> str:
    """Strip directives so pycparser sees a translation unit it can parse.

    Comments are replaced by blank lines rather than removed, so every
    remaining line keeps its original number -- the whole point of this tool
    is to report line numbers that map back onto the untouched file.
    """
    source = re.sub(r"/\*.*?\*/", lambda m: "\n" * m.group(0).count("\n"), source, flags=re.S)
    source = re.sub(r"//[^\n]*", "", source)
    return re.sub(r"^[ \t]*#[^\n]*", "", source, flags=re.M)


def type_of(node) -> str:
    """Render a declaration's type, so a split temp can copy it exactly."""
    pointers = ""
    while isinstance(node, (c_ast.PtrDecl, c_ast.ArrayDecl)):
        if isinstance(node, c_ast.PtrDecl):
            pointers += "*"
        node = node.type
    if isinstance(node, c_ast.TypeDecl):
        node = node.type
    if isinstance(node, c_ast.IdentifierType):
        base = " ".join(node.names)
    elif isinstance(node, (c_ast.Struct, c_ast.Union)):
        keyword = "struct" if isinstance(node, c_ast.Struct) else "union"
        base = f"{keyword} {node.name}" if node.name else keyword
    else:
        return ""
    return f"{base} {pointers}".strip() if pointers else base


class Effects(c_ast.NodeVisitor):
    """Names a statement writes and reads.

    Only a plain `name = ...` defines a NAME; a store through a pointer or
    into an array element targets memory, and its base is a read. That
    distinction is what the regex model kept getting wrong.
    """

    def __init__(self):
        self.writes, self.reads = set(), set()

    def visit_Assignment(self, node):
        if isinstance(node.lvalue, c_ast.ID):
            self.writes.add(node.lvalue.name)
            if node.op != "=":  # `x += 1` reads x as well
                self.reads.add(node.lvalue.name)
        else:
            self.visit(node.lvalue)
        self.visit(node.rvalue)

    def visit_UnaryOp(self, node):
        if node.op in ("p++", "p--", "++", "--") and isinstance(node.expr, c_ast.ID):
            self.writes.add(node.expr.name)
            self.reads.add(node.expr.name)
            return
        self.visit(node.expr)

    def visit_ID(self, node):
        self.reads.add(node.name)

    def visit_Decl(self, node):
        if node.name:
            self.writes.add(node.name)
        if node.init:
            self.visit(node.init)


def effects_of(node):
    walker = Effects()
    walker.visit(node)
    return sorted(walker.writes), sorted(walker.reads)


def line_of(node) -> int:
    return node.coord.line - PREAMBLE_LINES if node.coord else 0


def last_line(node) -> int:
    """Deepest line any part of this node reaches, for multi-line statements."""
    deepest = line_of(node)
    for _, child in node.children():
        deepest = max(deepest, last_line(child))
    return deepest


def statements_in(block):
    """The statement list a node holds, whatever shape it uses to hold it."""
    if isinstance(block, c_ast.Compound):
        return block.block_items or []
    if isinstance(block, (c_ast.Case, c_ast.Default)):
        return block.stmts or []
    return [block]


def collect(block, governed_by, out):
    """Walk one statement list, recording each statement and its structure."""
    for item in statements_in(block):
        if item is None:
            continue
        entry = {
            "line": line_of(item),
            "end_line": last_line(item),
            "kind": type(item).__name__,
            "anchor": isinstance(item, ANCHOR_NODES),
            "governed_by": governed_by,
        }
        writes, reads = effects_of(item)
        entry["writes"], entry["reads"] = writes, reads
        out.append(entry)

        # A `case`/`default` is a jump target holding its own statement list.
        if isinstance(item, (c_ast.Case, c_ast.Default)):
            collect(item, None, out)
            continue

        # A control construct whose body is NOT a Compound has a braceless
        # body: the header and that statement are one indivisible construct,
        # which is exactly what the line model could not represent.
        if isinstance(item, BRACELESS_OWNERS):
            entry["anchor"] = True
            for name in BODY_ATTRS:
                body = getattr(item, name, None)
                if body is None:
                    continue
                collect(body, None if isinstance(body, c_ast.Compound) else entry["line"], out)


def analyse(path: str, function: str):
    source = preprocess(open(path, encoding="utf8", errors="ignore").read())
    unit = c_parser.CParser().parse(PREAMBLE + source, filename=path)
    for node in unit.ext:
        if isinstance(node, c_ast.FuncDef) and node.decl.name == function:
            statements, declarations = [], {}
            for _, child in node.body.children():
                if isinstance(child, c_ast.Decl) and child.name:
                    declarations[child.name] = type_of(child.type)
            collect(node.body, None, statements)
            return {"function": function, "statements": statements, "declarations": declarations}
    return {"error": f"function not found: {function}"}


def self_test() -> None:
    import tempfile, os

    fixture = """#include "types.h"
void Func_02000500(void)
{
    s32 a;
    u8 *p;

    a = 1;
    if (a != 0)
        a = 2;
    p = 0;
    if (a != 9) return;
    a = 3;
}
"""
    handle, path = tempfile.mkstemp(suffix=".c")
    os.write(handle, fixture.encode())
    os.close(handle)
    try:
        result = analyse(path, "Func_02000500")
    finally:
        os.unlink(path)
    assert "error" not in result, result
    statements = result["statements"]

    # The braceless `if` is an anchor and the statement it governs is bound to it.
    header = next(s for s in statements if s["kind"] == "If" and s["line"] == 8)
    assert header["anchor"], header
    governed = [s for s in statements if s["governed_by"] == 8]
    assert len(governed) == 1 and governed[0]["line"] == 9, governed

    # A `return` is an anchor even when written inline after a condition.
    assert any(s["anchor"] and s["kind"] == "If" and s["line"] == 11 for s in statements), statements

    # A plain assignment defines a NAME; the pointer's type is recovered exactly.
    assign = next(s for s in statements if s["line"] == 7)
    assert assign["writes"] == ["a"] and not assign["anchor"], assign
    assert result["declarations"]["p"] == "u8 *", result["declarations"]
    assert result["declarations"]["a"] == "s32", result["declarations"]

    # Regression: statements nested in a `switch` must be reported. Missing a
    # statement container makes the whole region invisible, and a caller
    # cannot distinguish "no anchors here" from "never looked" -- which is how
    # a global assignment came to be hoisted above two `if (...) { break; }`
    # blocks on resource_371:6ec, running it on the break paths.
    switch_fixture = """#include "types.h"
void Func_02000600(s32 mode)
{
    s32 a;

    switch (mode) {
    case 33:
        a = 1;
        if (a != 0) {
            break;
        }
        a = 2;
        break;
    default:
        a = 3;
    }
}
"""
    handle, path = tempfile.mkstemp(suffix=".c")
    os.write(handle, switch_fixture.encode())
    os.close(handle)
    try:
        switched = analyse(path, "Func_02000600")
    finally:
        os.unlink(path)
    assert "error" not in switched, switched
    reported = {s["line"] for s in switched["statements"]}
    for line in (8, 12, 13, 15):  # the case, both assignments inside it, the default arm
        assert line in reported, f"line {line} missing from switch body: {sorted(reported)}"
    cases = [s for s in switched["statements"] if s["kind"] in ("Case", "Default")]
    assert len(cases) == 2 and all(s["anchor"] for s in cases), cases
    print("self-test=ok tool=c_structure")


if __name__ == "__main__":
    if "--self-test" in sys.argv:
        self_test()
    else:
        source_path, function_name = sys.argv[1], sys.argv[2]
        print(json.dumps(analyse(source_path, function_name)))
