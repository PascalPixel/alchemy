//! `psynergy convert`: gbagfx's file conversions between editable assets and
//! the GBA's binary forms. Every input, palette, width and rate is explicit;
//! there is no game, resource or ROM-address default.
use psynergy::assets::image::{self, GbaBpp};
use psynergy::assets::{text, wav};
use std::io::Write;

pub const USAGE: &str = "usage: psynergy convert FORMAT INPUT OUTPUT [options]\n\
formats: png2bpp4 | png2bpp8 | png2bgr555 | wav2pcm8 | words2bin | pairs2bin | tilemap2bin\n\
bpp42png | bpp82png take --palette FILE --tiles-wide N\n\
pcm82wav takes --rate HZ\n\
OUTPUT must not exist.\n";
const FORMATS: &str = "png2bpp4 | bpp42png | png2bpp8 | bpp82png | png2bgr555 | wav2pcm8 | pcm82wav | words2bin | pairs2bin | tilemap2bin";

#[derive(Default)]
struct Options {
    palette: Option<String>,
    tiles_wide: Option<usize>,
    rate: Option<u32>,
}

fn options(args: &[String]) -> Result<Options, String> {
    let mut options = Options::default();
    let mut rest = args.iter();
    while let Some(flag) = rest.next() {
        let value = rest
            .next()
            .ok_or_else(|| format!("{flag} requires a value"))?;
        let decimal = |name: &str| format!("{name} must be a decimal integer");
        let duplicate = match flag.as_str() {
            "--palette" => options.palette.replace(value.clone()).is_some(),
            "--tiles-wide" => options
                .tiles_wide
                .replace(value.parse().map_err(|_| decimal(flag))?)
                .is_some(),
            "--rate" => options
                .rate
                .replace(value.parse().map_err(|_| decimal(flag))?)
                .is_some(),
            _ => return Err(format!("unknown conversion option {flag}")),
        };
        if duplicate {
            return Err(format!("{flag} may be supplied only once"));
        }
    }
    Ok(options)
}

/// The conversions that take only their input.
fn encode(format: &str, input: &[u8]) -> Result<Vec<u8>, String> {
    let utf8 = || std::str::from_utf8(input).map_err(|error| error.to_string());
    let asset = |error: psynergy::assets::AssetError| error.to_string();
    match format {
        "words2bin" => text::import_words(utf8()?),
        "pairs2bin" => text::import_pairs(utf8()?),
        "tilemap2bin" => text::import_tilemap(utf8()?),
        "png2bpp4" => image::gba_tiles_from_png(input, GbaBpp::Bpp4).map_err(asset),
        "png2bpp8" => image::gba_tiles_from_png(input, GbaBpp::Bpp8).map_err(asset),
        "png2bgr555" => image::bgr555_palette_from_png(input).map_err(asset),
        "wav2pcm8" => wav::wav_pcm8(input)
            .map(|(_, samples)| samples)
            .map_err(asset),
        _ => Err(format!("unknown conversion {format}; expected {FORMATS}")),
    }
}

fn convert(format: &str, input: &[u8], options: Options) -> Result<Vec<u8>, String> {
    let asset = |error: psynergy::assets::AssetError| error.to_string();
    match format {
        "bpp42png" | "bpp82png" => {
            let (Some(palette), Some(tiles_wide), None) =
                (options.palette, options.tiles_wide, options.rate)
            else {
                return Err(format!(
                    "{format} takes exactly --palette FILE and --tiles-wide N"
                ));
            };
            let palette = std::fs::read(&palette).map_err(|error| format!("{palette}: {error}"))?;
            let bpp = if format == "bpp42png" {
                GbaBpp::Bpp4
            } else {
                GbaBpp::Bpp8
            };
            image::png_from_gba_tiles(input, &palette, bpp, tiles_wide).map_err(asset)
        }
        "pcm82wav" => {
            let (None, None, Some(rate)) = (options.palette, options.tiles_wide, options.rate)
            else {
                return Err("pcm82wav takes exactly --rate HZ".into());
            };
            wav::pcm8_wav(input, rate).map_err(asset)
        }
        _ => {
            if options.palette.is_some() || options.tiles_wide.is_some() || options.rate.is_some() {
                return Err(format!("{format} does not accept conversion options"));
            }
            encode(format, input)
        }
    }
}

pub fn run(args: &[String]) -> Result<String, String> {
    let [format, input, output, rest @ ..] = args else {
        return Err(USAGE.into());
    };
    let options = options(rest)?;
    let input_data = std::fs::read(input).map_err(|error| format!("{input}: {error}"))?;
    let output_data = convert(format, &input_data, options)?;
    let mut file = std::fs::OpenOptions::new()
        .write(true)
        .create_new(true)
        .open(output)
        .map_err(|error| format!("{output}: {error}"))?;
    file.write_all(&output_data)
        .map_err(|error| format!("{output}: {error}"))?;
    Ok(String::new())
}

#[cfg(test)]
mod tests {
    use super::*;

    fn args(values: &[&str]) -> Vec<String> {
        values.iter().map(|value| value.to_string()).collect()
    }

    #[test]
    fn conversion_does_not_overwrite_output() {
        let dir = tempfile::tempdir().unwrap();
        let input = dir.path().join("input");
        let output = dir.path().join("output");
        std::fs::write(&input, "0x1234").unwrap();
        let args = args(&[
            "words2bin",
            input.to_str().unwrap(),
            output.to_str().unwrap(),
        ]);
        run(&args).unwrap();
        assert_eq!(std::fs::read(&output).unwrap(), [0x34, 0x12]);
        std::fs::write(&input, "0xffff").unwrap();
        assert!(run(&args).is_err());
        assert_eq!(std::fs::read(&output).unwrap(), [0x34, 0x12]);
    }

    #[test]
    fn table_converters_preserve_words_and_reject_old_names() {
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
        for legacy in ["png2gba4bpp", "png2gba8bpp", "png2gbapal", "absent"] {
            assert!(encode(legacy, b"").is_err());
        }
        assert!(encode("png2bpp4", b"not a PNG").is_err());
    }

    #[test]
    fn options_are_explicit_and_single() {
        assert!(options(&args(&["--rate"])).is_err());
        assert!(options(&args(&["--rate", "8000", "--rate", "8000"])).is_err());
        assert!(options(&args(&["--tiles-wide", "0x10"])).is_err());
        assert!(options(&args(&["--offset", "0"])).is_err());
        let rate = || Options {
            rate: Some(8000),
            ..Options::default()
        };
        assert!(convert("words2bin", b"0x1234", rate()).is_err());
        assert!(convert("bpp42png", &[0; 32], rate()).is_err());
        assert!(convert("pcm82wav", &[0], Options::default()).is_err());
    }

    #[test]
    fn tile_converters_round_trip_and_require_explicit_context() {
        let dir = tempfile::tempdir().unwrap();
        let path = |name: &str| dir.path().join(name).to_string_lossy().into_owned();
        let raw_tiles = vec![0x10; 32];
        let raw_palette = vec![0, 0, 31, 0];
        std::fs::write(path("tiles.4bpp"), &raw_tiles).unwrap();
        std::fs::write(path("palette.bgr555"), &raw_palette).unwrap();
        assert!(run(&args(&[
            "bpp42png",
            &path("tiles.4bpp"),
            &path("tiles.png")
        ]))
        .is_err());
        let palette = path("palette.bgr555");
        let context = ["--palette", palette.as_str(), "--tiles-wide", "1"];
        let to_png = [
            &["bpp42png", &path("tiles.4bpp"), &path("tiles.png")],
            &context[..],
        ];
        run(&args(&to_png.concat())).unwrap();
        run(&args(&[
            "png2bpp4",
            &path("tiles.png"),
            &path("rebuilt.4bpp"),
        ]))
        .unwrap();
        run(&args(&[
            "png2bgr555",
            &path("tiles.png"),
            &path("rebuilt.bgr555"),
        ]))
        .unwrap();
        assert_eq!(std::fs::read(path("rebuilt.4bpp")).unwrap(), raw_tiles);
        assert_eq!(std::fs::read(path("rebuilt.bgr555")).unwrap(), raw_palette);

        let raw_tiles8 = vec![1; 64];
        std::fs::write(path("tiles.8bpp"), &raw_tiles8).unwrap();
        let to_png8 = [
            &["bpp82png", &path("tiles.8bpp"), &path("tiles8.png")],
            &context[..],
        ];
        run(&args(&to_png8.concat())).unwrap();
        run(&args(&[
            "png2bpp8",
            &path("tiles8.png"),
            &path("rebuilt.8bpp"),
        ]))
        .unwrap();
        assert_eq!(std::fs::read(path("rebuilt.8bpp")).unwrap(), raw_tiles8);
    }

    #[test]
    fn pcm_converters_round_trip_and_require_rate() {
        let dir = tempfile::tempdir().unwrap();
        let path = |name: &str| dir.path().join(name).to_string_lossy().into_owned();
        let samples = [128, 0, 127];
        std::fs::write(path("input.pcm8"), samples).unwrap();
        assert!(run(&args(&[
            "pcm82wav",
            &path("input.pcm8"),
            &path("output.wav")
        ]))
        .is_err());
        run(&args(&[
            "pcm82wav",
            &path("input.pcm8"),
            &path("output.wav"),
            "--rate",
            "8000",
        ]))
        .unwrap();
        run(&args(&[
            "wav2pcm8",
            &path("output.wav"),
            &path("rebuilt.pcm8"),
        ]))
        .unwrap();
        assert_eq!(std::fs::read(path("rebuilt.pcm8")).unwrap(), samples);
    }
}
