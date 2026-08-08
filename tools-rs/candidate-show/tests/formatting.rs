//! Proves the float-formatting trap is INAPPLICABLE to this renderer rather
//! than assuming it.
//!
//! The trap: `serde_json`/ryu and `format!("{}")` do not agree with
//! ECMAScript's ToString(Number), so a ported document can grow a `1.0` where
//! JavaScript prints `1`, or an `1e21` where JavaScript prints something else.
//! `candidate_show.ts` emits numbers in exactly two shapes: the three header
//! counts (template-literal `${number}`) and the offset column
//! (`offset.toString(16).padStart(4, "0")`). This test walks EVERY numeric
//! token a rendered document can contain and pins that neither shape can ever
//! produce a fractional or exponential form.

use candidate_show::jsparse::{pad_start_zero, utf16_len};
use match_m2c::jsnum::to_js_number_string;

/// Every token in the header line and the offset column, for the whole range of
/// values a real document can hold.
fn header_line(candidate: f64, reference: f64, differing: f64) -> String {
    format!(
        "candidate={} reference={} differing_halfwords={}",
        to_js_number_string(candidate).unwrap(),
        to_js_number_string(reference).unwrap(),
        to_js_number_string(differing).unwrap(),
    )
}

#[test]
fn no_header_count_can_render_a_fractional_or_exponential_token() {
    // Byte lengths are non-negative integers bounded by the 32 MiB GBA address
    // space; halfword counts are bounded by half of that. Walk the whole
    // plausible range plus every power-of-two boundary where a naive `{}` or a
    // ryu shortest-representation would be tempted to switch notation.
    let mut probes: Vec<f64> = (0..4096).map(f64::from).collect();
    for exponent in 0..26u32 {
        let value = f64::from(1u32 << exponent);
        probes.push(value - 1.0);
        probes.push(value);
        probes.push(value + 1.0);
    }
    for probe in probes {
        if probe < 0.0 {
            continue;
        }
        let line = header_line(probe, probe, probe);
        for token in line.split(['=', ' ']) {
            if token.is_empty() || !token.starts_with(|c: char| c.is_ascii_digit()) {
                continue;
            }
            assert!(
                token.bytes().all(|b| b.is_ascii_digit()),
                "non-integral token {token:?} in {line:?}"
            );
        }
    }
}

#[test]
fn the_naive_formatter_would_have_been_wrong_at_least_once() {
    // If `{}` agreed with JavaScript everywhere, this whole module would be
    // theatre. Two witnesses so a later reader cannot "simplify"
    // `to_js_number_string` away.
    //
    // Past 1e21 ECMAScript switches to exponential notation and Rust does not.
    // The helper refuses the value rather than emitting the wrong token, which
    // is the guard: a naive `{}` would have printed a 22-digit run.
    let value = 1e21f64;
    assert!(to_js_number_string(value).is_err());
    assert_eq!(format!("{value}"), "1000000000000000000000");
    // A non-integral count cannot arise from a byte length, and the helper
    // refuses it rather than guessing at `toFixed` semantics.
    assert!(to_js_number_string(0.5).is_err());
}

#[test]
fn the_offset_column_is_integral_lowercase_hex_at_every_width() {
    for offset in (0..0x2_0000u64).step_by(2) {
        let rendered = pad_start_zero(&format!("{offset:x}"), 4);
        assert!(
            rendered.bytes().all(|b| b.is_ascii_digit() || (b'a'..=b'f').contains(&b)),
            "offset column {rendered:?} left the hex alphabet"
        );
        assert!(utf16_len(&rendered) >= 4, "padStart shrank {rendered:?}");
        if offset < 0x1_0000 {
            assert_eq!(utf16_len(&rendered), 4);
        } else {
            // Never truncates: past four digits the column widens.
            assert_eq!(utf16_len(&rendered), 5);
        }
    }
}
