// Parity harness for the mutation operators.
//
// The CLI only exercises inferAssemblyConstraints. This example runs every
// operator over real semantic/*.c bodies with a fixed LCG and prints one
// checksum line per (fixture, operator, seed). It is a test harness, not a tool.

use std::fs;

use decomp_constraints::CONSTRAINT_OPERATORS;

/// The same LCG as the TypeScript driver: state advances mod 2^32 and the draw
/// is state / 2^32, so both sides walk an identical stream of doubles.
fn lcg(seed: u32) -> impl FnMut() -> f64 {
    let mut state = seed;
    move || {
        state = state.wrapping_mul(1664525).wrapping_add(1013904223);
        f64::from(state) / 4294967296.0
    }
}

fn fnv(text: &str) -> String {
    let mut hash: u32 = 0x811c_9dc5;
    for byte in text.as_bytes() {
        hash = (hash ^ u32::from(*byte)).wrapping_mul(0x0100_0193);
    }
    format!("{hash:08x}")
}

fn main() {
    for path in std::env::args().skip(1) {
        let body = String::from_utf8_lossy(&fs::read(&path).expect("fixture readable")).into_owned();
        let name = path.rsplit('/').next().unwrap_or(&path).to_string();
        for (label, operator) in CONSTRAINT_OPERATORS {
            for seed in 1..=40u32 {
                let mut random = lcg(seed.wrapping_mul(2654435761));
                let out = operator(&body, &mut random);
                match out {
                    // PORT NOTE: JS `String.length` counts UTF-16 code units,
                    // so the length column is computed the same way here
                    // rather than as a byte or char count.
                    Some(text) => println!(
                        "{name} {label} {seed} {} {}",
                        fnv(&text),
                        text.chars().map(char::len_utf16).sum::<usize>()
                    ),
                    None => println!("{name} {label} {seed} null"),
                }
            }
            let mut chained = body.clone();
            let mut random = lcg(12345);
            for step in 0..12 {
                let Some(next) = operator(&chained, &mut random) else {
                    break;
                };
                chained = next;
                println!(
                    "{name} {label} chain{step} {} {}",
                    fnv(&chained),
                    chained.chars().map(char::len_utf16).sum::<usize>()
                );
            }
        }
    }
}
