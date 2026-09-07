//! Format conversions have no game, resource, or ROM-address defaults.
use std::io::Write;

const FORMATS: &str =
    "words2bin | pairs2bin | tilemap2bin | png2bpp4 | bpp42png | png2bpp8 | bpp82png | png2bgr555 | wav2pcm8 | pcm82wav";
const USAGE: &str = "usage: alchemy convert FORMAT INPUT OUTPUT [options]\n\
formats: words2bin | pairs2bin | tilemap2bin | png2bpp4 | png2bpp8 | png2bgr555 | wav2pcm8\n\
         bpp42png | bpp82png INPUT OUTPUT --palette FILE --tiles-wide N\n\
         pcm82wav INPUT OUTPUT --rate HZ\n\
OUTPUT must not exist.";

#[derive(Default)]
struct Options {
    palette: Option<String>,
    tiles_wide: Option<usize>,
    rate: Option<u32>,
}

fn encode(format: &str, input: &[u8]) -> Result<Vec<u8>, String> {
    let text = || std::str::from_utf8(input).map_err(|error| error.to_string());
    match format {
        "words2bin" => import_asset::import_words(text()?),
        "pairs2bin" => import_asset::import_pairs(text()?),
        "tilemap2bin" => import_asset::import_tilemap(text()?),
        "png2bpp4" => import_asset::gba_tiles_from_png(input, import_asset::GbaBpp::Bpp4)
            .map_err(|error| error.to_string()),
        "png2bpp8" => import_asset::gba_tiles_from_png(input, import_asset::GbaBpp::Bpp8)
            .map_err(|error| error.to_string()),
        "png2bgr555" => {
            import_asset::bgr555_palette_from_png(input).map_err(|error| error.to_string())
        }
        "wav2pcm8" => import_asset::wav_pcm8(input)
            .map(|(_, samples)| samples)
            .map_err(|error| error.to_string()),
        _ => return Err(format!("unknown conversion {format}; expected {FORMATS}")),
    }
}

fn options(args: &[String]) -> Result<Options, String> {
    let mut options = Options::default();
    let mut at = 0;
    while at < args.len() {
        let flag = &args[at];
        let value = args
            .get(at + 1)
            .ok_or_else(|| format!("{flag} requires a value"))?;
        at += 2;
        match flag.as_str() {
            "--palette" => {
                if options.palette.is_some() {
                    return Err("--palette may be supplied only once".into());
                }
                options.palette = Some(value.clone());
            }
            "--tiles-wide" => {
                if options.tiles_wide.is_some() {
                    return Err("--tiles-wide may be supplied only once".into());
                }
                options.tiles_wide = Some(
                    value
                        .parse()
                        .map_err(|_| "--tiles-wide must be a decimal integer".to_string())?,
                );
            }
            "--rate" => {
                if options.rate.is_some() {
                    return Err("--rate may be supplied only once".into());
                }
                options.rate = Some(
                    value
                        .parse()
                        .map_err(|_| "--rate must be a decimal integer".to_string())?,
                );
            }
            _ => return Err(format!("unknown conversion option {flag}")),
        }
    }
    Ok(options)
}

fn require_reverse_options(options: Options) -> Result<(String, usize), String> {
    let Some(palette) = options.palette else {
        return Err("bpp42png and bpp82png require --palette FILE".into());
    };
    let Some(tiles_wide) = options.tiles_wide else {
        return Err("bpp42png and bpp82png require --tiles-wide N".into());
    };
    if options.rate.is_some() {
        return Err("bpp42png and bpp82png do not accept --rate".into());
    }
    Ok((palette, tiles_wide))
}

fn require_rate(options: Options) -> Result<u32, String> {
    if options.palette.is_some() || options.tiles_wide.is_some() {
        return Err("pcm82wav accepts only --rate HZ".into());
    }
    options
        .rate
        .ok_or_else(|| "pcm82wav requires --rate HZ".into())
}

fn require_no_options(options: Options, format: &str) -> Result<(), String> {
    if options.palette.is_some() || options.tiles_wide.is_some() || options.rate.is_some() {
        return Err(format!("{format} does not accept conversion options"));
    }
    Ok(())
}

fn convert(format: &str, input: &[u8], options: Options) -> Result<Vec<u8>, String> {
    match format {
        "bpp42png" | "bpp82png" => {
            let (palette_path, tiles_wide) = require_reverse_options(options)?;
            let palette =
                std::fs::read(&palette_path).map_err(|error| format!("{palette_path}: {error}"))?;
            let bpp = if format == "bpp42png" {
                import_asset::GbaBpp::Bpp4
            } else {
                import_asset::GbaBpp::Bpp8
            };
            import_asset::png_from_gba_tiles(input, &palette, bpp, tiles_wide)
                .map_err(|error| error.to_string())
        }
        "pcm82wav" => {
            import_asset::pcm8_wav(input, require_rate(options)?).map_err(|error| error.to_string())
        }
        _ => {
            require_no_options(options, format)?;
            encode(format, input)
        }
    }
}

pub fn run(args: &[String]) -> Result<(), String> {
    if args == ["--help"] || args == ["-h"] {
        println!("{USAGE}\nformats: {FORMATS}");
        return Ok(());
    }
    if args.len() < 3 {
        return Err(USAGE.into());
    }
    let (format, input, output) = (&args[0], &args[1], &args[2]);
    let options = options(&args[3..])?;
    let input_data = std::fs::read(input).map_err(|error| format!("{input}: {error}"))?;
    let output_data = convert(format, &input_data, options)?;
    let mut file = std::fs::OpenOptions::new()
        .write(true)
        .create_new(true)
        .open(output)
        .map_err(|error| format!("{output}: {error}"))?;
    file.write_all(&output_data)
        .map_err(|error| format!("{output}: {error}"))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn conversion_does_not_overwrite_output() {
        let dir = tempfile::tempdir().unwrap();
        let input = dir.path().join("input");
        let output = dir.path().join("output");
        std::fs::write(&input, "0x1234").unwrap();
        let args = [
            "words2bin",
            input.to_str().unwrap(),
            output.to_str().unwrap(),
        ]
        .into_iter()
        .map(str::to_string)
        .collect::<Vec<_>>();
        run(&args).unwrap();
        assert_eq!(std::fs::read(&output).unwrap(), [0x34, 0x12]);
        std::fs::write(&input, "0xffff").unwrap();
        assert!(run(&args).is_err());
        assert_eq!(std::fs::read(&output).unwrap(), [0x34, 0x12]);
    }

    #[test]
    fn canonical_converters_preserve_words_and_reject_old_names() {
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
        for legacy in ["png2gba4bpp", "png2gba8bpp", "png2gbapal"] {
            assert!(encode(legacy, b"").is_err());
        }
        assert!(encode("absent", b"").is_err());
        assert!(encode("png2bpp4", b"not a PNG").is_err());
    }

    #[test]
    fn tile_converters_round_trip_and_require_explicit_context() {
        let dir = tempfile::tempdir().unwrap();
        let tiles = dir.path().join("tiles.4bpp");
        let palette = dir.path().join("palette.bgr555");
        let png = dir.path().join("tiles.png");
        let rebuilt_tiles = dir.path().join("rebuilt.4bpp");
        let rebuilt_palette = dir.path().join("rebuilt.bgr555");
        let raw_tiles = vec![0x10; 32];
        let raw_palette = vec![0, 0, 31, 0];
        std::fs::write(&tiles, &raw_tiles).unwrap();
        std::fs::write(&palette, &raw_palette).unwrap();

        let missing_context = [
            "bpp42png".to_string(),
            tiles.to_string_lossy().into_owned(),
            png.to_string_lossy().into_owned(),
        ];
        assert!(run(&missing_context).is_err());

        let args = [
            "bpp42png".to_string(),
            tiles.to_string_lossy().into_owned(),
            png.to_string_lossy().into_owned(),
            "--palette".to_string(),
            palette.to_string_lossy().into_owned(),
            "--tiles-wide".to_string(),
            "1".to_string(),
        ];
        run(&args).unwrap();
        let to_tiles = [
            "png2bpp4".to_string(),
            png.to_string_lossy().into_owned(),
            rebuilt_tiles.to_string_lossy().into_owned(),
        ];
        let to_palette = [
            "png2bgr555".to_string(),
            png.to_string_lossy().into_owned(),
            rebuilt_palette.to_string_lossy().into_owned(),
        ];
        run(&to_tiles).unwrap();
        run(&to_palette).unwrap();
        assert_eq!(std::fs::read(rebuilt_tiles).unwrap(), raw_tiles);
        assert_eq!(std::fs::read(rebuilt_palette).unwrap(), raw_palette);

        let tiles8 = dir.path().join("tiles.8bpp");
        let png8 = dir.path().join("tiles8.png");
        let rebuilt8 = dir.path().join("rebuilt.8bpp");
        let raw_tiles8 = vec![1; 64];
        std::fs::write(&tiles8, &raw_tiles8).unwrap();
        let to_png8 = [
            "bpp82png".to_string(),
            tiles8.to_string_lossy().into_owned(),
            png8.to_string_lossy().into_owned(),
            "--palette".to_string(),
            palette.to_string_lossy().into_owned(),
            "--tiles-wide".to_string(),
            "1".to_string(),
        ];
        let to_tiles8 = [
            "png2bpp8".to_string(),
            png8.to_string_lossy().into_owned(),
            rebuilt8.to_string_lossy().into_owned(),
        ];
        run(&to_png8).unwrap();
        run(&to_tiles8).unwrap();
        assert_eq!(std::fs::read(rebuilt8).unwrap(), raw_tiles8);
    }

    #[test]
    fn pcm_converters_round_trip_and_require_rate() {
        let dir = tempfile::tempdir().unwrap();
        let pcm = dir.path().join("input.pcm8");
        let wav = dir.path().join("output.wav");
        let rebuilt = dir.path().join("rebuilt.pcm8");
        let samples = [128, 0, 127];
        std::fs::write(&pcm, samples).unwrap();
        let missing_rate = [
            "pcm82wav".to_string(),
            pcm.to_string_lossy().into_owned(),
            wav.to_string_lossy().into_owned(),
        ];
        assert!(run(&missing_rate).is_err());
        let to_wav = [
            "pcm82wav".to_string(),
            pcm.to_string_lossy().into_owned(),
            wav.to_string_lossy().into_owned(),
            "--rate".to_string(),
            "8000".to_string(),
        ];
        let to_pcm = [
            "wav2pcm8".to_string(),
            wav.to_string_lossy().into_owned(),
            rebuilt.to_string_lossy().into_owned(),
        ];
        run(&to_wav).unwrap();
        run(&to_pcm).unwrap();
        assert_eq!(std::fs::read(rebuilt).unwrap(), samples);
    }
}
