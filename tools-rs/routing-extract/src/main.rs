//! Extract the per-source compiler-flag routing tables out of
//! tools/lib/alchemy_gcc.ts into a single ordered JSON document.
//!
//! The emission ORDER of `cflagsForSource`'s array spread is load-bearing
//! (gcc is later-flag-wins), so the output encodes rules as an ORDERED array,
//! never as a map keyed by table name.

use std::collections::BTreeMap;
use std::env;
use std::fs;

use serde_json::{json, Map, Value};

/// Replace every `//` and `/* */` comment with spaces, preserving byte offsets
/// and newlines. String literals are respected so `"http://x"` survives.
fn strip_comments(s: &str) -> String {
    let b = s.as_bytes().to_vec();
    let mut out = b.clone();
    let mut i = 0;
    let mut in_str: Option<u8> = None;
    while i < b.len() {
        let ch = b[i];
        if let Some(q) = in_str {
            if ch == b'\\' {
                i += 2;
                continue;
            }
            if ch == q {
                in_str = None;
            }
            i += 1;
            continue;
        }
        match ch {
            b'"' | b'\'' | b'`' => {
                in_str = Some(ch);
                i += 1;
            }
            b'/' if i + 1 < b.len() && b[i + 1] == b'/' => {
                while i < b.len() && b[i] != b'\n' {
                    out[i] = b' ';
                    i += 1;
                }
            }
            b'/' if i + 1 < b.len() && b[i + 1] == b'*' => {
                while i < b.len() && !(b[i] == b'*' && i + 1 < b.len() && b[i + 1] == b'/') {
                    if b[i] != b'\n' {
                        out[i] = b' ';
                    }
                    i += 1;
                }
                out[i] = b' ';
                out[i + 1] = b' ';
                i += 2;
            }
            _ => i += 1,
        }
    }
    String::from_utf8(out).expect("utf8")
}

/// Find the matching close paren/bracket for the opener at `open`.
fn match_delim(s: &[u8], open: usize) -> usize {
    let (o, c) = match s[open] {
        b'(' => (b'(', b')'),
        b'[' => (b'[', b']'),
        _ => panic!("not an opener at {open}"),
    };
    let mut depth = 0i32;
    let mut i = open;
    let mut in_str: Option<u8> = None;
    while i < s.len() {
        let ch = s[i];
        if let Some(q) = in_str {
            if ch == b'\\' {
                i += 2;
                continue;
            }
            if ch == q {
                in_str = None;
            }
            i += 1;
            continue;
        }
        match ch {
            b'"' | b'\'' | b'`' => in_str = Some(ch),
            x if x == o => depth += 1,
            x if x == c => {
                depth -= 1;
                if depth == 0 {
                    return i;
                }
            }
            _ => {}
        }
        i += 1;
    }
    panic!("unbalanced from {open}");
}

/// Every plain string literal inside `text`, in source order.
fn string_literals(text: &str) -> Vec<String> {
    let b = text.as_bytes();
    let mut out = Vec::new();
    let mut i = 0;
    while i < b.len() {
        if b[i] == b'"' {
            let start = i + 1;
            let mut j = start;
            while j < b.len() && b[j] != b'"' {
                if b[j] == b'\\' {
                    j += 1;
                }
                j += 1;
            }
            out.push(text[start..j].to_string());
            i = j + 1;
        } else {
            i += 1;
        }
    }
    out
}

fn key_kind(cond: &str) -> &'static str {
    if cond.contains("sourceKey(source)") {
        "path"
    } else if cond.contains("sourceStem(source)") {
        "basestem"
    } else {
        // `stem` local and `overlayStem(source)` are the same value.
        "stem"
    }
}

fn main() {
    let args: Vec<String> = env::args().collect();
    let src_path = args
        .get(1)
        .cloned()
        .unwrap_or_else(|| "tools/lib/alchemy_gcc.ts".to_string());
    let out_path = args
        .get(2)
        .cloned()
        .unwrap_or_else(|| "tools/data/routing.json".to_string());
    let raw = fs::read_to_string(&src_path).expect("read source");
    // Blank out comments in place (offsets preserved) so apostrophes in prose
    // cannot be mistaken for string delimiters.
    let src = strip_comments(&raw);
    let sb = src.as_bytes();

    // ---- 1. Tables -------------------------------------------------------
    let mut tables: BTreeMap<String, Value> = BTreeMap::new();
    let mut i = 0usize;
    while let Some(rel) = src[i..].find("const ") {
        let at = i + rel;
        i = at + 6;
        let rest = &src[i..];
        let name_end = match rest.find(|c: char| !(c.is_ascii_alphanumeric() || c == '_')) {
            Some(v) => v,
            None => break,
        };
        let name = &rest[..name_end];
        let after = &rest[name_end..];
        let is_set = after.starts_with(" = new Set");
        let is_map = after.starts_with(" = new Map");
        if !(is_set || is_map) {
            continue;
        }
        // Locate the `(` that opens the constructor argument list.
        let paren_rel = after.find('(').unwrap();
        let paren = i + name_end + paren_rel;
        let close = match_delim(sb, paren);
        let body = &src[paren + 1..close];
        if is_set {
            tables.insert(
                name.to_string(),
                json!({ "kind": "set", "entries": string_literals(body) }),
            );
        } else {
            let lits = string_literals(body);
            let mut m = Map::new();
            for pair in lits.chunks(2) {
                if pair.len() == 2 {
                    m.insert(pair[0].clone(), Value::String(pair[1].clone()));
                }
            }
            tables.insert(name.to_string(), json!({ "kind": "map", "entries": m }));
        }
        i = close;
    }

    // ---- 2. Parse one `...( cond ? [flags] : [] )` spread ----------------
    let parse_spread = |inner: &str| -> Value {
        let qmark = {
            // First top-level `?`.
            let b = inner.as_bytes();
            let mut depth = 0i32;
            let mut pos = None;
            let mut k = 0;
            let mut in_str: Option<u8> = None;
            while k < b.len() {
                let ch = b[k];
                if let Some(q) = in_str {
                    if ch == b'\\' {
                        k += 2;
                        continue;
                    }
                    if ch == q {
                        in_str = None;
                    }
                    k += 1;
                    continue;
                }
                match ch {
                    b'"' | b'\'' | b'`' => in_str = Some(ch),
                    b'(' | b'[' => depth += 1,
                    b')' | b']' => depth -= 1,
                    b'?' if depth == 0 => {
                        pos = Some(k);
                        break;
                    }
                    _ => {}
                }
                k += 1;
            }
            pos.expect("no ternary in spread")
        };
        let cond = inner[..qmark].trim().to_string();
        let rest = &inner[qmark + 1..];
        let lb = rest.find('[').expect("no then-branch array");
        let rb = match_delim(rest.as_bytes(), lb);
        let then_body = &rest[lb + 1..rb];

        // Condition -> ordered list of predicates (OR-joined).
        let mut when = Vec::new();
        for clause in cond.split("||") {
            let clause = clause.trim();
            if let Some(p) = clause.find(".has(") {
                let tname = clause[..p]
                    .rsplit(|c: char| !(c.is_ascii_alphanumeric() || c == '_'))
                    .next()
                    .unwrap()
                    .to_string();
                when.push(json!({"table": tname, "key": key_kind(clause)}));
            } else if let Some(p) = clause.find("===") {
                let lhs = clause[..p].trim();
                let val = string_literals(&clause[p..]).remove(0);
                when.push(json!({"eq": val, "key": key_kind(lhs)}));
            } else {
                panic!("unparsed condition: {clause}");
            }
        }

        // Flags, in emission order. Template literals become lookups.
        let mut flags = Vec::new();
        let tb = then_body.as_bytes();
        let mut k = 0;
        while k < tb.len() {
            match tb[k] {
                b'"' => {
                    let start = k + 1;
                    let mut j = start;
                    while tb[j] != b'"' {
                        if tb[j] == b'\\' {
                            j += 1;
                        }
                        j += 1;
                    }
                    flags.push(json!({ "lit": &then_body[start..j] }));
                    k = j + 1;
                }
                b'`' => {
                    let start = k + 1;
                    let mut j = start;
                    while tb[j] != b'`' {
                        j += 1;
                    }
                    let t = &then_body[start..j];
                    let dollar = t.find("${").expect("template without interpolation");
                    let prefix = &t[..dollar];
                    let expr = &t[dollar + 2..t.rfind('}').unwrap()];
                    let tname = expr[..expr.find(".get(").expect("template not a .get()")]
                        .trim()
                        .to_string();
                    flags.push(json!({"prefix": prefix, "from": tname, "key": key_kind(expr)}));
                    k = j + 1;
                }
                _ => k += 1,
            }
        }
        json!({ "when": when, "flags": flags })
    };

    // Collect the top-level `...(` spreads inside a function body slice.
    let collect_rules = |body: &str| -> Vec<Value> {
        let bb = body.as_bytes();
        let mut rules = Vec::new();
        let mut k = 0;
        while let Some(rel) = body[k..].find("...(") {
            let open = k + rel + 3;
            let close = match_delim(bb, open);
            rules.push(parse_spread(&body[open + 1..close]));
            k = close;
        }
        rules
    };

    // ---- 3. cflagsForSource ---------------------------------------------
    let fstart = src
        .find("export function cflagsForSource")
        .expect("cflagsForSource");
    let fend = src[fstart..].find("\n}\n").unwrap() + fstart;
    let fbody = &src[fstart..fend];

    // The two baseline subtractions, in the order they are applied.
    let mut base_filters = Vec::new();
    for stmt in ["const abiBase =", "const base ="] {
        let s = fbody.find(stmt).expect("base statement");
        let e = fbody[s..].find(";").unwrap() + s;
        let text = &fbody[s..e];
        let cond_end = text.find('?').unwrap();
        let mut when = Vec::new();
        for clause in text[..cond_end].split("||").skip(0) {
            if let Some(p) = clause.find(".has(") {
                let tname = clause[..p]
                    .rsplit(|c: char| !(c.is_ascii_alphanumeric() || c == '_'))
                    .next()
                    .unwrap()
                    .to_string();
                when.push(json!({"table": tname, "key": key_kind(clause)}));
            }
        }
        let removed = text
            .find("flag !== ")
            .map(|p| string_literals(&text[p..]).remove(0));
        base_filters.push(json!({ "when": when, "remove": removed }));
    }

    let ret = fbody.find("return [").unwrap();
    let rules = collect_rules(&fbody[ret..]);

    // ---- 4. cflagsForTargetSource (old_agbcc family) --------------------
    let astart = src
        .find("export function cflagsForTargetSource")
        .expect("cflagsForTargetSource");
    let aend = src[astart..].find("\n}\n").unwrap() + astart;
    let agbcc_rules = collect_rules(&src[astart..aend]);

    // ---- 5. Emit ---------------------------------------------------------
    let base = |name: &str| -> Vec<String> {
        let s = src.find(&format!("export const {name} = [")).unwrap();
        let lb = src[s..].find('[').unwrap() + s;
        let rb = match_delim(sb, lb);
        // `-I${join(ROOT, "include")}` yields a bare directory literal; turn it
        // back into a ROOT-relative sentinel the loader substitutes.
        let mut v: Vec<String> = string_literals(&src[lb + 1..rb])
            .into_iter()
            .map(|e| {
                if e.starts_with('-') {
                    e
                } else {
                    format!("-I${{ROOT}}/{e}")
                }
            })
            .collect();
        if name == "GS2_CFLAGS" {
            let mut c = base_cflags(&src, sb);
            c.append(&mut v);
            return c;
        }
        v
    };
    fn base_cflags(src: &str, sb: &[u8]) -> Vec<String> {
        let s = src.find("export const CFLAGS = [").unwrap();
        let lb = src[s..].find('[').unwrap() + s;
        let rb = match_delim(sb, lb);
        string_literals(&src[lb + 1..rb])
            .into_iter()
            .map(|e| {
                if e.starts_with('-') {
                    e
                } else {
                    format!("-I${{ROOT}}/{e}")
                }
            })
            .collect()
    }

    let doc = json!({
        "note": "Generated from tools/lib/alchemy_gcc.ts by tools-rs/routing-extract. \
                 `rules` is ORDERED: flags are emitted in array order and gcc is \
                 later-flag-wins. Never reorder, never key by table name.",
        "includeFlag": "-I<ROOT>/include",
        "base": {
            "gs1": base("CFLAGS"),
            "gs2": base("GS2_CFLAGS"),
            "agbcc": base("AGBCC_CFLAGS"),
        },
        "agbccSelect": {"table": "AGBCC_SOURCES", "key": "basestem"},
        "baseFilters": base_filters,
        "rules": rules,
        "agbccRules": agbcc_rules,
        "tables": tables,
    });

    if let Some(dir) = std::path::Path::new(&out_path).parent() {
        fs::create_dir_all(dir).ok();
    }
    fs::write(&out_path, serde_json::to_string_pretty(&doc).unwrap() + "\n").expect("write");
    eprintln!(
        "wrote {out_path}: {} tables, {} gs1 rules, {} agbcc rules",
        doc["tables"].as_object().unwrap().len(),
        doc["rules"].as_array().unwrap().len(),
        doc["agbccRules"].as_array().unwrap().len()
    );
}
