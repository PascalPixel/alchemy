//! The five source rewrites and the `candidates()` fan-out.
//!
//! These are the pure half of `permute_m2c.ts`: string in, string out, no
//! filesystem and no subprocess. Everything here is exercised directly by the
//! differential parity harness against the live TypeScript.

use crate::jsstring::{code_unit_cmp, js_trim, replace_all, replace_first};
use crate::patterns;

/// `stateBlock(body, swap)`.
///
/// Rewrites every `M2C_FIELD` access into an explicit two-term address
/// computation, with the base and the scaled index in one order or the other.
pub fn state_block(body: &str, swap: bool) -> String {
    let matches = patterns::statement_matches(body);
    if matches.is_empty() {
        return body.to_string();
    }
    let mut out = String::with_capacity(body.len());
    let mut cursor = 0usize;
    for found in &matches {
        out.push_str(&body[cursor..found.start]);
        let terms: [&str; 2] = if swap {
            ["(s32)_mo", "(s32)_mb"]
        } else {
            ["(s32)_mb", "(s32)_mo"]
        };
        let total = format!("{} + {} + (s32){}", terms[0], terms[1], found.offset);
        out.push_str(&format!(
            "{}{{ s32 _mb = *(s32 *){}; s32 _mo = {} * {}; {}*({})({}); }}",
            found.indent, found.address, found.index, found.stride, found.target, found.kind, total
        ));
        cursor = found.end;
    }
    out.push_str(&body[cursor..]);
    out
}

/// `liftConstants(body)`.
///
/// PORT NOTE: the constant set is collected from the **whole** body, including
/// the signature that precedes the opening brace, but the substitution is
/// applied only to the text after it. A ROM address appearing in the signature
/// therefore gets a `_cN` declaration that nothing uses. Reproduced.
pub fn lift_constants(body: &str) -> String {
    let Some((_, cut)) = patterns::opening_brace(body) else {
        return body.to_string();
    };

    // PORT NOTE (trap: `Set` deduplication and default `sort()` order).
    // `[...new Set(matches)]` keeps first-seen order and then `sort()` reorders
    // by UTF-16 code unit, not by numeric value and not by `localeCompare`. A
    // `HashSet` would lose the intermediate order that the sort is stable over,
    // and `str::cmp` is a third order again.
    let mut constants: Vec<&str> = Vec::new();
    for found in patterns::address_matches(body) {
        if !constants.contains(&found) {
            constants.push(found);
        }
    }
    constants.sort_by(|left, right| code_unit_cmp(left, right));

    let mut declarations = String::new();
    for (index, value) in constants.iter().enumerate() {
        declarations.push_str(&format!("\n    s32 _c{index} = {value};"));
    }
    let mut rest = body[cut..].to_string();
    for (index, value) in constants.iter().enumerate() {
        rest = replace_all(&rest, value, &format!("_c{index}"));
    }
    format!("{}{}{}", &body[..cut], declarations, rest)
}

/// `threadLeadingArgument(body)`.
///
/// m2c drops an unused leading parameter, so this tries the shifted signature,
/// forwarding `arg0` to a known zero-argument callee.
pub fn thread_leading_argument(body: &str) -> String {
    let Some(signature) = patterns::signature_match(body) else {
        return body.to_string();
    };
    if patterns::mentions_arg0(&signature.parameters) {
        return body.to_string();
    }
    let trimmed = js_trim(&signature.parameters);
    let parameters = if trimmed.is_empty() || trimmed == "void" {
        "s32 arg0".to_string()
    } else {
        format!("s32 arg0, {}", signature.parameters)
    };

    // `body.replace(signature, "$1$2(<parameters>) {")`: a non-global regular
    // expression replaces its first match only.
    let replacement = format!("{}{}({}) {{", signature.leading, signature.name, parameters);
    let mut result = String::with_capacity(body.len() + replacement.len());
    result.push_str(&body[..signature.start]);
    result.push_str(&replacement);
    result.push_str(&body[signature.end..]);

    let Some(callee) = patterns::bare_call_match(&result) else {
        return result;
    };
    // PORT NOTE: the TypeScript builds these two needles by string
    // concatenation and calls `String#replace` (first occurrence) then
    // `String#replaceAll`. The order matters: the declaration is rewritten to
    // take an `s32`, and only then does every remaining bare call become
    // `(arg0)`. Collapsing the two into one pass changes the output.
    let declaration = format!("{}{}();", callee.leading, callee.name);
    let replaced = replace_first(
        &result,
        &declaration,
        &format!("{}{}(s32);", callee.leading, callee.name),
    );
    replace_all(
        &replaced,
        &format!("{}();", callee.name),
        &format!("{}(arg0);", callee.name),
    )
}

/// `volatileHardware(body)`.
pub fn volatile_hardware(body: &str) -> String {
    let matches = patterns::hardware_matches(body);
    if matches.is_empty() {
        return body.to_string();
    }
    let mut out = String::with_capacity(body.len());
    let mut cursor = 0usize;
    for found in &matches {
        out.push_str(&body[cursor..found.start]);
        out.push_str(&format!("*(volatile {} *){}", found.kind, found.address));
        cursor = found.end;
    }
    out.push_str(&body[cursor..]);
    out
}

/// `postIncrementWalk(body)`.
pub fn post_increment_walk(body: &str) -> String {
    let matches = patterns::post_increment_matches(body);
    if matches.is_empty() {
        return body.to_string();
    }
    let mut out = String::with_capacity(body.len());
    let mut cursor = 0usize;
    for found in &matches {
        out.push_str(&body[cursor..found.start]);
        // The callback folds only when the loaded pointer is the one being
        // advanced; otherwise it returns the match untouched.
        if found.load == format!("*{}", found.pointer) {
            out.push_str(&format!("{} = *{}++;", found.target, found.pointer));
        } else {
            out.push_str(&body[found.start..found.end]);
        }
        cursor = found.end;
    }
    out.push_str(&body[cursor..]);
    out
}

/// `candidates(body)`: eleven variants, in source order.
///
/// PORT NOTE: `[...direct, threadLeadingArgument(body), ...direct.map(...)]`.
/// The plain `threadLeadingArgument(body)` sits **between** the five direct
/// variants and their threaded versions, not after them, and duplicates are not
/// removed. Order is load-bearing: `main` takes the first variant that
/// verifies, so reordering or de-duplicating changes which source file lands.
pub fn candidates(body: &str) -> Vec<String> {
    let direct = [
        state_block(body, false),
        state_block(body, true),
        lift_constants(body),
        volatile_hardware(body),
        post_increment_walk(body),
    ];
    let mut out: Vec<String> = direct.to_vec();
    out.push(thread_leading_argument(body));
    for variant in &direct {
        out.push(thread_leading_argument(variant));
    }
    out
}

#[cfg(test)]
mod tests {
    use super::*;

    const FIELD: &str =
        "s32 Func_08001234(void) {\n    return M2C_FIELD((*(s32 *)0x08002000 + (i * 4)), s32 *, 0x10);\n}\n";

    #[test]
    fn state_block_swaps_the_two_terms() {
        let plain = state_block(FIELD, false);
        let swapped = state_block(FIELD, true);
        assert!(plain.contains("(s32)_mb + (s32)_mo + (s32)0x10"));
        assert!(swapped.contains("(s32)_mo + (s32)_mb + (s32)0x10"));
        assert_ne!(plain, swapped);
    }

    #[test]
    fn candidates_has_eleven_entries_in_a_fixed_order() {
        let all = candidates(FIELD);
        assert_eq!(all.len(), 11);
        assert_eq!(all[0], state_block(FIELD, false));
        assert_eq!(all[5], thread_leading_argument(FIELD));
        assert_eq!(all[6], thread_leading_argument(&state_block(FIELD, false)));
        // Duplicates are kept, not collapsed.
        let unchanged = all.iter().filter(|body| *body == &all[3]).count();
        assert!(unchanged >= 1);
    }

    #[test]
    fn lift_constants_sorts_by_code_unit_and_dedupes_in_place() {
        let body = "f() {\n a = 0x08002000; b = 0x03001000; c = 0x08002000;\n}";
        let lifted = lift_constants(body);
        // 0x03.. sorts before 0x08.. by code unit, and the repeat is dropped.
        assert!(lifted.contains("s32 _c0 = 0x03001000;"));
        assert!(lifted.contains("s32 _c1 = 0x08002000;"));
        assert!(!lifted.contains("s32 _c2"));
        assert!(lifted.contains("a = _c1; b = _c0; c = _c1;"));
    }

    #[test]
    fn lift_constants_declares_signature_constants_it_never_substitutes() {
        // Reproduced defect: the address in the signature is counted but not
        // rewritten, so `_c0` is declared and unused.
        let body = "f(0x08002000) {\n a = 1;\n}";
        let lifted = lift_constants(body);
        assert!(lifted.contains("s32 _c0 = 0x08002000;"));
        assert!(lifted.starts_with("f(0x08002000) {"));
    }

    #[test]
    fn lift_constants_without_an_opening_brace_is_the_identity() {
        assert_eq!(lift_constants("no brace 0x08002000"), "no brace 0x08002000");
    }

    #[test]
    fn thread_leading_argument_is_a_no_op_when_arg0_is_present() {
        let body = "s32 Func_08001234(s32 arg0) {\n}\n";
        assert_eq!(thread_leading_argument(body), body);
    }

    #[test]
    fn thread_leading_argument_forwards_to_a_bare_callee() {
        let body = "void Func_08001234(void) {\n    s32 Func_08005678();\n    Func_08005678();\n}\n";
        let threaded = thread_leading_argument(body);
        assert!(threaded.contains("Func_08001234(s32 arg0) {"));
        assert!(threaded.contains("s32 Func_08005678(s32);"));
        assert!(threaded.contains("    Func_08005678(arg0);"));
    }

    #[test]
    fn post_increment_only_folds_the_matching_pointer() {
        let folded = post_increment_walk("a = *p;\n    p += 4;\n");
        assert_eq!(folded, "a = *p++;\n");
        let untouched = "a = *q;\n    p += 4;\n";
        assert_eq!(post_increment_walk(untouched), untouched);
    }

    #[test]
    fn volatile_hardware_normalises_the_whitespace_it_ate() {
        assert_eq!(
            volatile_hardware("x = * (u16 *) 0x04000000;"),
            "x = *(volatile u16 *)0x04000000;"
        );
    }
}
