//! Raw integer data in overlay listings is retained matching material, not
//! source: its total may fall as data becomes typed tables or private inputs,
//! but never grow past the number the Makefile records.

use std::path::Path;
use std::process::ExitCode;

const USAGE: &str = "usage: alchemy check overlay-data --max COUNT\n\nCount the raw integer data values in tracked games/*/asm/overlays/*_overlay.s listings and fail when the total exceeds COUNT.";
/// GNU as integer data directives on ARM, aliases included, so a respelling
/// cannot hide growth.
const DIRECTIVES: [&str; 9] = [
    ".byte", ".2byte", ".4byte", ".8byte", ".word", ".short", ".hword", ".long", ".int",
];

fn overlay_listing(path: &str) -> bool {
    let parts: Vec<&str> = path.split('/').collect();
    matches!(parts.as_slice(), ["games", _, "asm", "overlays", name] if name.ends_with("_overlay.s"))
}

/// The values a data directive statement emits: one per top-level operand.
fn operands(text: &str) -> usize {
    let (mut count, mut depth, mut pending, mut quoted) = (0, 0i32, false, false);
    for character in text.chars() {
        match character {
            '"' => quoted = !quoted,
            '(' if !quoted => depth += 1,
            ')' if !quoted => depth -= 1,
            ',' if !quoted && depth == 0 => {
                count += usize::from(pending);
                pending = false;
                continue;
            }
            _ => {}
        }
        pending |= !character.is_whitespace();
    }
    count + usize::from(pending)
}

/// The raw integer data values a listing emits, skipping comments, labels
/// and statement separators the way the assembler reads them.
fn data_values(listing: &str) -> usize {
    let mut total = 0;
    let mut in_block = false;
    for line in listing.lines() {
        let mut code = String::new();
        let mut rest = line;
        while !rest.is_empty() {
            if in_block {
                let Some(end) = rest.find("*/") else { break };
                rest = &rest[end + 2..];
                in_block = false;
            } else if let Some(start) = rest.find("/*") {
                code.push_str(&rest[..start]);
                rest = &rest[start + 2..];
                in_block = true;
            } else {
                code.push_str(rest);
                break;
            }
        }
        let code = code.split('@').next().unwrap_or("");
        for statement in code.split(';') {
            let mut statement = statement.trim_start();
            while let Some((label, after)) = statement.split_once(':') {
                if label.is_empty() || label.contains(char::is_whitespace) {
                    break;
                }
                statement = after.trim_start();
            }
            let (directive, arguments) = statement
                .split_once(char::is_whitespace)
                .unwrap_or((statement, ""));
            if DIRECTIVES.contains(&directive.to_ascii_lowercase().as_str()) {
                total += operands(arguments);
            }
        }
    }
    total
}

fn count(root: &Path) -> Result<usize, String> {
    Ok(super::tracked_game_files(root, overlay_listing)?
        .iter()
        .map(|(_, data)| data_values(&String::from_utf8_lossy(data)))
        .sum())
}

fn ratchet(total: usize, max: usize) -> Result<String, String> {
    if total > max {
        return Err(format!(
            "overlay listings hold {total} raw data values, above the ratchet of {max}: \
             type the data from its consumers or register it as a private input instead"
        ));
    }
    Ok(if total < max {
        format!("overlay data ok: {total} / {max} raw values; lower OVERLAY_DATA_DIRECTIVES_MAX to {total}")
    } else {
        format!("overlay data ok: {total} / {max} raw values")
    })
}

pub(super) fn entry(arguments: &[String]) -> ExitCode {
    let max = match arguments {
        [flag, value] if flag == "--max" => value.parse::<usize>().ok(),
        _ => None,
    };
    let Some(max) = max else {
        eprintln!("{USAGE}");
        return ExitCode::from(2);
    };
    super::report(
        count(crate::compiler::routing::root())
            .and_then(|total| ratchet(total, max))
            .map(|message| println!("{message}")),
    )
}

#[test]
fn every_emitted_integer_counts_however_it_is_spelled() {
    let listing = [
        ".syntax unified",
        "\t.4byte 0x47704800",
        "\t.2byte 0x96dc @ .4byte 1, 2",
        "label: .byte 1, (2, 3), \"a,b\"",
        "a: b: .LONG 1; .short 2, 3",
        "\t.word",
        "/* .4byte 1",
        ".4byte 2 */ .hword 4, 5 /* .int 6 */",
        "\t.space 0x96",
        "\t.set sub_020013c0, 0x020013c0",
        "\t.ascii \"1, 2\"",
        "\toverlay_veneer 0x02008329",
        "\t.4bytes 7",
        "labelled.4byte: .8byte 8",
    ]
    .join("\n");
    assert_eq!(data_values(&listing), 1 + 1 + 3 + 3 + 2 + 1);
}

#[test]
fn only_tracked_overlay_listings_count_and_the_total_may_only_fall() {
    let data = "\t.4byte 1\n\t.2byte 2, 3\n";
    let root = super::fixture_repository(&[
        ("games/X/asm/overlays/resource_1_overlay.s", data, true),
        ("games/Y/asm/overlays/resource_2_overlay.s", data, true),
        ("games/X/asm/overlays/resource_3_overlay.s", data, false),
        (
            "games/X/asm/overlays/nested/resource_4_overlay.s",
            data,
            true,
        ),
        ("games/X/asm/executable_gaps/08000404.s", data, true),
    ]);
    assert_eq!(count(root.path()), Ok(6));
    assert!(ratchet(6, 6).is_ok());
    assert!(ratchet(5, 6)
        .unwrap()
        .contains("lower OVERLAY_DATA_DIRECTIVES_MAX to 5"));
    assert!(ratchet(7, 6)
        .unwrap_err()
        .contains("above the ratchet of 6"));
}
