//! agsgfx: Golden Sun graphics conversion, as pret's gbagfx is for Pokémon.
//! The conversion follows from the file extensions, and every encoder
//! setting is an option on the command line, kept beside the asset.
use ags::graphics::metatiles;
use ags::lz::{
    compress_general, compress_mtf4, compress_tagged, compress_tagged_palette, LzMachine,
};
use psynergy::assets::image::{
    bgr555_palette_from_png, indexed_png, png_from_bitmap, png_from_gba_tiles, GbaBpp,
};
use std::fs;
use std::process::ExitCode;

const USAGE: &str = "usage: agsgfx INPUT OUTPUT [options]
  X.png  -> Y.4bpp | Y.8bpp    tiles, row-major or by metatile (-mwidth N -mheight N)
  X.png  -> Y.gbapal           the PNG's palette as little-endian BGR555
  X.png  -> Y.bitmap[.lz|.mtf] any build recipe, as the build makes it (see ags::resource)
  X.4bpp | X.8bpp -> Y.png     tiles back to an indexed PNG (--palette P.gbapal|P.png --width TILES)
  X.bitmap -> Y.png            a linear 8-bit bitmap to an indexed PNG (--palette P --width PIXELS)
  X      -> Y.lz               compress (--lz general|palette|tagged|mtf4; the LZ kinds
                               take --machine WINDOW,READ_AHEAD,MAX_DISTANCE,PALETTE_READ_AHEAD)
  X.lz   -> Y                  decompress a tagged stream (tag 0 general, 1 palette, 2 tile)";

fn extension(path: &str) -> String {
    path.rsplit('.').next().unwrap_or("").to_ascii_lowercase()
}

fn option(args: &[String], name: &str) -> Option<String> {
    args.iter()
        .position(|arg| arg == name)
        .and_then(|index| args.get(index + 1).cloned())
}

fn number(args: &[String], name: &str, default: usize) -> Result<usize, String> {
    option(args, name).map_or(Ok(default), |value| {
        value.parse().map_err(|_| format!("{name} takes a number"))
    })
}

fn machine(args: &[String]) -> Result<LzMachine, String> {
    let text = option(args, "--machine").ok_or("LZ needs --machine W,R,D,P")?;
    let values: Vec<usize> = text
        .split(',')
        .map(|part| {
            part.trim()
                .parse()
                .map_err(|_| "--machine takes four numbers")
        })
        .collect::<Result<_, _>>()?;
    match values.as_slice() {
        [window, read_ahead, distance, palette] => {
            Ok(LzMachine::new(*window, *read_ahead, *distance, *palette))
        }
        _ => Err("--machine takes four numbers".into()),
    }
}

fn decompress(stream: &[u8]) -> Result<Vec<u8>, String> {
    const LIMIT: u64 = 1 << 24;
    // The decoders read ahead in whole words, as the game does; in the ROM
    // the next resource follows, so a lone stream gets that slack as zeros.
    let mut data = stream.to_vec();
    data.extend([0; 4]);
    let data = &data[..];
    let tag = *data.first().ok_or("empty stream")?;
    let decoded = match tag {
        0 => psynergy::assets::lz::decode_general(data, 0, data.len(), LIMIT),
        1 => psynergy::assets::lz::decode_palette(data, 1, data.len(), LIMIT),
        2 => psynergy::assets::lz::decode_mtf4_lz(data, 0, data.len(), LIMIT),
        other => return Err(format!("unknown stream tag {other}")),
    };
    decoded.map(|(bytes, _)| bytes).map_err(|error| error.0)
}

fn bpp(extension: &str) -> GbaBpp {
    if extension == "4bpp" {
        GbaBpp::Bpp4
    } else {
        GbaBpp::Bpp8
    }
}

fn run(args: &[String]) -> Result<(), String> {
    if args.len() < 2 || args.iter().any(|arg| arg == "-h" || arg == "--help") {
        return Err(USAGE.into());
    }
    let (input, output) = (&args[0], &args[1]);
    let options = &args[2..];
    let data = fs::read(input).map_err(|error| format!("{input}: {error}"))?;
    let (from, to) = (extension(input), extension(output));
    let bytes = match (from.as_str(), to.as_str()) {
        ("png", "4bpp" | "8bpp") => {
            let image = indexed_png(&data).map_err(|error| error.0)?;
            metatiles(
                &ags::graphics::indices(&image),
                image.width as usize,
                image.height as usize,
                bpp(&to),
                number(options, "-mwidth", 1)?,
                number(options, "-mheight", 1)?,
            )?
        }
        ("png", "gbapal") => bgr555_palette_from_png(&data).map_err(|error| error.0)?,
        ("png", _) => ags::resource::build_file(output, &data)?,
        ("4bpp" | "8bpp", "png") => {
            let path = option(options, "--palette").ok_or("needs --palette")?;
            let palette = fs::read(&path).map_err(|error| format!("{path}: {error}"))?;
            let palette = if extension(&path) == "png" {
                bgr555_palette_from_png(&palette).map_err(|error| error.0)?
            } else {
                palette
            };
            png_from_gba_tiles(&data, &palette, bpp(&from), number(options, "--width", 16)?)
                .map_err(|error| error.0)?
        }
        ("bitmap", "png") => {
            let path = option(options, "--palette").ok_or("needs --palette")?;
            let palette = fs::read(&path).map_err(|error| format!("{path}: {error}"))?;
            let width = option(options, "--width").ok_or("needs --width")?;
            let width = width.parse().map_err(|_| "--width takes a number")?;
            png_from_bitmap(&data, &palette, width).map_err(|error| error.0)?
        }
        ("lz", _) => decompress(&data)?,
        (_, "lz") => match option(options, "--lz").as_deref() {
            Some("general") => compress_general(&data, &machine(options)?)?,
            Some("palette") => compress_tagged_palette(&data, &machine(options)?)?,
            Some("tagged") => compress_tagged(&data, &machine(options)?)?,
            Some("mtf4") => compress_mtf4(&data)?,
            _ => return Err("compressing needs --lz general|palette|tagged|mtf4".into()),
        },
        _ => return Err(format!("no conversion from .{from} to .{to}\n{USAGE}")),
    };
    fs::write(output, bytes).map_err(|error| format!("{output}: {error}"))
}

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    match run(&args) {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("{error}");
            ExitCode::FAILURE
        }
    }
}
