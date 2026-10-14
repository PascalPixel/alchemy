use ags::graphics::metatiles;
use ags::lz::{
    compress_general, compress_mtf4, compress_tagged, compress_tagged_palette, LzMachine,
};
use ags::assets::image::{bgr555_palette_from_png, indexed_png, GbaBpp};
use std::fs;
use std::process::ExitCode;

const USAGE: &str = "usage: agsgfx INPUT OUTPUT [options]
  X.png  -> Y.4bpp | Y.8bpp    tiles, row-major or by metatile (-mwidth N -mheight N)
  X.png  -> Y.gbapal           the PNG's palette as little-endian BGR555
  X.png  -> Y.bitmap[.lz|.mtf] any build recipe, as the build makes it (see ags::resource)
  X.tsv  -> Y.delta1.lz        a data recipe from an editable table
  X      -> Y.lz               compress (--lz general|palette|tagged|mtf4; the LZ kinds
                               take --machine WINDOW,READ_AHEAD,MAX_DISTANCE,PALETTE_READ_AHEAD)
  X.png  -> Y.png              redraw X's pixels, read as --from FORM, as the OBJ sprites of
                               --to FORM, --wide N to a row (all in one row by default), in
                               X's palette; both forms are tile forms of one depth
  X.tsv  -> Y.png              redraw a part list of tile parts of --to's depth the same way,
                               in the parts' shared palette or --palette P.png's; each is
                               refused unless --to builds the same tiles from the drawing
                               and the drawing gives back every palette index of its source";

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

fn bpp(extension: &str) -> GbaBpp {
    if extension == "4bpp" {
        GbaBpp::Bpp4
    } else {
        GbaBpp::Bpp8
    }
}

/// The value after the option `name`, when it is given: an option given last,
/// with no value after it, is refused rather than taken as not given.
fn given(args: &[String], name: &str) -> Result<Option<String>, String> {
    match args.iter().position(|arg| arg == name) {
        None => Ok(None),
        Some(index) => match args.get(index + 1) {
            Some(value) => Ok(Some(value.clone())),
            None => Err(format!("{name} is given no value")),
        },
    }
}

/// A sheet's tiles drawn again as --to's sprites: a PNG built as --from, or a
/// part list of tile parts (ags::resource::redraw and redraw_parts).
fn redraw(input: &str, data: &[u8], options: &[String]) -> Result<Vec<u8>, String> {
    let to = given(options, "--to")?.ok_or("redrawing needs --to FORM")?;
    let wide = given(options, "--wide")?
        .map(|wide| wide.parse().map_err(|_| "--wide takes a number"))
        .transpose()?;
    let palette = given(options, "--palette")?;
    let from = given(options, "--from")?;
    if extension(input) != "tsv" {
        if palette.is_some() {
            return Err("--palette names a part list's palette; a PNG keeps its own".into());
        }
        let from = from.ok_or("redrawing a PNG needs --from FORM")?;
        return ags::resource::redraw(data, &from, &to, wide);
    }
    if from.is_some() {
        return Err("--from names a PNG's form; a part list names each part's".into());
    }
    let palette = palette
        .map(|path| fs::read(&path).map_err(|error| format!("{path}: {error}")))
        .transpose()?;
    ags::resource::redraw_parts(data, &to, wide, palette.as_deref(), &|name| {
        let path = ags::resource::sibling_path(std::path::Path::new(input), name)?;
        fs::read(&path).map_err(|error| format!("{}: {error}", path.display()))
    })
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
        ("png" | "tsv", "png") => redraw(input, &data, options)?,
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
        ("png" | "tsv", _) if from == "png" || option(options, "--lz").is_none() => {
            ags::resource::build_file_with(output, &data, &|name| {
                let path = ags::resource::sibling_path(std::path::Path::new(input), name)?;
                fs::read(&path).map_err(|error| format!("{}: {error}", path.display()))
            })?
        }
        (_, "lz") => match option(options, "--lz").as_deref() {
            Some("general") => compress_general(&data, &machine(options)?)?,
            Some("palette") => compress_tagged_palette(&data, &machine(options)?)?,
            Some("tagged") => compress_tagged(&data, &machine(options)?)?,
            Some("mtf4") => compress_mtf4(&data)?,
            _ => return Err("compressing needs --lz general|palette|tagged|mtf4".into()),
        },
        _ => return Err(format!("no conversion from .{from} to .{to}\n{USAGE}")),
    };
    ags::write_whole(output, &bytes)
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

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn a_redraw_option_given_last_without_its_value_is_refused() {
        let folder = tempfile::tempdir().unwrap();
        let (input, output) = (folder.path().join("sheet.png"), folder.path().join("drawn.png"));
        let png = ags::assets::image::png_from_bitmap(&[0; 64], &[0, 0], 8).unwrap();
        fs::write(&input, &png).unwrap();
        let args = |options: &[&str]| -> Vec<String> {
            [input.to_str().unwrap(), output.to_str().unwrap()]
                .iter()
                .chain(options)
                .map(|arg| arg.to_string())
                .collect()
        };
        let refused = |options: &[&str]| run(&args(options)).unwrap_err();
        assert_eq!(refused(&["--from", "4bpp", "--to", "4bpp", "--wide"]), "--wide is given no value");
        assert_eq!(refused(&["--to", "4bpp", "--from"]), "--from is given no value");
        assert_eq!(refused(&["--from", "4bpp", "--to"]), "--to is given no value");
        assert_eq!(refused(&["--from", "4bpp", "--to", "4bpp", "--wide", "a"]), "--wide takes a number");
        assert!(!output.exists());
        run(&args(&["--from", "4bpp", "--to", "4bpp", "--wide", "1"])).unwrap();
        assert_eq!(fs::read(&output).unwrap(), png);
    }
}
