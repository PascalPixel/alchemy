//! Format conversions have no game, resource, or ROM-address defaults.
use std::io::Write;

const FORMATS: &str =
    "words2bin | pairs2bin | tilemap2bin | png2gba4bpp | png2gba8bpp | png2gbapal";

fn encode(format: &str, input: &[u8]) -> Result<Vec<u8>, String> {
    let text = || std::str::from_utf8(input).map_err(|e| e.to_string());
    match format {
        "words2bin" => import_asset::import_words(text()?),
        "pairs2bin" => import_asset::import_pairs(text()?),
        "tilemap2bin" => import_asset::import_tilemap(text()?),
        "png2gba4bpp" | "png2gba8bpp" | "png2gbapal" => {
            let (pixels, palette, _) =
                import_asset::gba_graphics(input, if format == "png2gba4bpp" { 4.0 } else { 8.0 })
                    .map_err(|e| e.to_string())?;
            Ok(if format == "png2gbapal" {
                palette
            } else {
                pixels
            })
        }
        _ => Err(format!("unknown conversion {format}; expected {FORMATS}")),
    }
}

pub fn run(args: &[String]) -> Result<(), String> {
    if args == ["--help"] || args == ["-h"] {
        println!("usage: alchemy convert FORMAT INPUT OUTPUT\nformats: {FORMATS}\nOUTPUT must not exist.");
        return Ok(());
    }
    let [format, input, output] = args else {
        return Err("usage: alchemy convert FORMAT INPUT OUTPUT".into());
    };
    let data = std::fs::read(input).map_err(|e| format!("{input}: {e}"))?;
    let data = encode(format, &data)?;
    let mut file = std::fs::OpenOptions::new()
        .write(true)
        .create_new(true)
        .open(output)
        .map_err(|e| format!("{output}: {e}"))?;
    file.write_all(&data).map_err(|e| format!("{output}: {e}"))
}

#[test]
fn conversion_does_not_overwrite_output() {
    let dir = tempfile::tempdir().unwrap();
    let input = dir.path().join("input");
    let output = dir.path().join("output");
    std::fs::write(&input, "0x1234").unwrap();
    let args = vec![
        "words2bin".into(),
        input.to_string_lossy().into_owned(),
        output.to_string_lossy().into_owned(),
    ];
    run(&args).unwrap();
    assert_eq!(std::fs::read(&output).unwrap(), [0x34, 0x12]);
    std::fs::write(&input, "0xffff").unwrap();
    assert!(run(&args).is_err());
    assert_eq!(std::fs::read(&output).unwrap(), [0x34, 0x12]);
}

#[test]
fn converters_preserve_words_and_reject_wrong_formats() {
    for (format, input) in [
        ("words2bin", "0x1234\n0xabcd"),
        ("pairs2bin", "0x1234 0xabcd"),
        ("tilemap2bin", "1234 abcd"),
    ] {
        assert_eq!(
            encode(format, input.as_bytes()).unwrap(),
            [0x34, 0x12, 0xcd, 0xab]
        );
        assert!(encode(format, b"invalid").is_err());
    }
    assert!(encode("absent", b"").is_err());
    assert!(encode("png2gba4bpp", b"not a PNG").is_err());
}
