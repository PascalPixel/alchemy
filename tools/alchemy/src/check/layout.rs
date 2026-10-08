use ags::assets::image::{
    indexed_bitmap_png, sprite_runs, sprite_runs_score, tile_sheet_layout, tile_sheet_score,
    GbaBpp, SpriteRun, TileLayout,
};
use std::collections::BTreeSet;
use std::path::{Path, PathBuf};
use std::process::ExitCode;

const USAGE: &str = "usage: alchemy check layout [--report]\n\
Refuses every tile-sheet PNG whose finder layout beats the drawn one clearly, unless\n\
it is the best arrangement of the whole sprites its recipe names;\n\
--report lists every sheet with both scores instead.";

pub const MARGIN: f64 = 0.1;

#[derive(Clone, Debug, PartialEq, Eq, PartialOrd, Ord)]
struct Recipe {
    png: PathBuf,
    form: String,
}

fn tile_form(form: &str) -> Option<(GbaBpp, (usize, usize))> {
    let bpp = match form.get(..4)? {
        "4bpp" => GbaBpp::Bpp4,
        "8bpp" => GbaBpp::Bpp8,
        _ => return None,
    };
    let shape = &form[4..];
    if shape.is_empty() {
        return Some((bpp, (1, 1)));
    }
    let (wide, high) = shape.split_once('x')?;
    Some((
        bpp,
        (
            wide.parse::<usize>().ok()? / 8,
            high.parse::<usize>().ok()? / 8,
        ),
    ))
}

fn judged_form(form: &str) -> bool {
    tile_form(form).is_some() || form == BITMAP
}

const BITMAP: &str = "bitmap";

fn form_of(built: &str) -> Option<&str> {
    let file = built.rsplit('/').next()?;
    file.split('.').nth(1)
}

fn recipes_in(
    source_root: &Path,
    text: &str,
    found: &mut BTreeSet<Recipe>,
    problems: &mut Vec<String>,
) {
    for line in text.lines() {
        let Some(rest) = line.trim().strip_prefix(".incbin") else {
            continue;
        };
        let Some(built) = rest
            .trim()
            .strip_prefix('"')
            .and_then(|r| r.split('"').next())
        else {
            continue;
        };
        let Some(form) = form_of(built) else {
            continue;
        };
        let (root, built) = match built.strip_prefix("COMMON/") {
            Some(shared) => match source_root.parent().and_then(Path::parent) {
                Some(games) => (games.join("COMMON/SRC"), shared),
                None => continue,
            },
            None => (source_root.to_path_buf(), built),
        };
        let Ok(input) = ags::resource::input_name(built) else {
            continue;
        };
        let input = root.join(input);
        if judged_form(form) {
            found.insert(Recipe {
                png: input,
                form: form.to_owned(),
            });
        } else if form == "parts" {
            let Ok(list) = std::fs::read_to_string(&input) else {
                continue;
            };
            let sibling = |name: &str| {
                let path = ags::resource::sibling_path(&input, name)?;
                std::fs::read(path).map_err(|error| format!("{name}: {error}"))
            };
            problems.extend(
                tilemap_problems(&list, &sibling)
                    .into_iter()
                    .map(|problem| format!("{}: {problem}", input.display())),
            );
            for line in list.lines() {
                let fields: Vec<&str> = line.split('\t').map(str::trim).collect();
                if let [part, form] = fields.as_slice() {
                    let png = ags::resource::sibling_path(&input, &format!("{part}.PNG"));
                    if let (true, Ok(png)) = (judged_form(form), png) {
                        found.insert(Recipe {
                            png,
                            form: (*form).to_owned(),
                        });
                    }
                }
            }
        }
    }
}

fn tilemap_problems(list: &str, sibling: &dyn Fn(&str) -> Result<Vec<u8>, String>) -> Vec<String> {
    let lines: Vec<Vec<&str>> = list
        .lines()
        .filter(|line| !line.trim().is_empty())
        .map(|line| line.split('\t').map(str::trim).collect())
        .collect();
    let maps: Vec<&str> = lines
        .iter()
        .filter(|fields| fields.get(1) == Some(&"table"))
        .filter(|fields| {
            sibling(&format!("{}.TSV", fields[0]))
                .is_ok_and(|text| ags::resource::tilemap(fields[0], &text).is_ok())
        })
        .map(|fields| fields[0])
        .collect();
    if maps.is_empty() {
        return Vec::new();
    }
    let mut problems = Vec::new();
    for fields in &lines {
        let [part, form, drawing @ ..] = fields.as_slice() else {
            continue;
        };
        if tile_form(form).is_none() {
            continue;
        }
        if drawing.is_empty() {
            problems.push(format!(
                "{part}.PNG is cut plainly beside the tilemap {}; draw it as the picture the map shows \
                 and the tiles the map never shows as frames (PART FORM MAP FIRST COUNT [frames])",
                maps.join(", ")
            ));
            continue;
        }
        let built = format!("{part}.{form}");
        let result = ags::resource::MapPart::parse(&built, drawing).and_then(|drawn| {
            if !maps.contains(&drawn.map.as_str()) {
                return Err(format!(
                    "{built} is drawn by {}, not this list's tilemap",
                    drawn.map
                ));
            }
            drawn.layout(&built, sibling)?;
            Ok(())
        });
        if let Err(error) = result {
            problems.push(error);
        }
    }
    problems
}

fn recipes(root: &Path) -> Result<(BTreeSet<Recipe>, Vec<String>), String> {
    let mut found = BTreeSet::new();
    let mut problems = Vec::new();
    for entry in walkdir::WalkDir::new(root.join("games")).sort_by_file_name() {
        let entry = entry.map_err(|error| format!("games: {error}"))?;
        let path = entry.path();
        if path.extension().is_none_or(|ext| ext != "S") {
            continue;
        }
        let text = path.to_string_lossy();
        let Some(at) = text.find("/SRC/") else {
            continue;
        };
        let source_root = PathBuf::from(&text[..at + 4]);
        let source = std::fs::read_to_string(path)
            .map_err(|error| format!("{}: {error}", path.display()))?;
        recipes_in(&source_root, &source, &mut found, &mut problems);
    }
    Ok((found, problems))
}

#[derive(Clone, Debug, PartialEq)]
pub struct Judgement {
    pub tiles: usize,
    pub drawn: TileLayout,
    pub drawn_score: f64,
    pub best: TileLayout,
    pub best_score: f64,
    pub runs: Vec<SpriteRun>,
    pub runs_score: f64,
    /// The best score of any arrangement of whole sprites, when the recipe
    /// names a sprite shape (A4).
    pub sprites_score: Option<f64>,
    pub bitmap: bool,
}

impl Judgement {
    fn animation(&self) -> bool {
        self.drawn.meta == (1, 1)
            && self.drawn.tiles_wide <= ANIMATION_TILES
            && self.best.meta == (1, 1)
            && self.best.tiles_wide.is_multiple_of(self.drawn.tiles_wide)
            && self.tiles / self.best.tiles_wide <= self.drawn.tiles_wide
    }

    fn mixed(&self) -> bool {
        self.drawn.tiles_wide == 1
            && (self.bitmap || self.runs.iter().any(|run| run.layout.meta != (1, 1)))
    }

    pub fn found_score(&self) -> f64 {
        let layout = if self.animation() {
            self.drawn_score
        } else {
            self.best_score
        };
        if self.mixed() {
            layout.max(self.runs_score)
        } else {
            layout
        }
    }

    /// A sheet whose recipe names a sprite shape can only be drawn as whole
    /// sprites; drawn as their best arrangement, a finder layout that cuts
    /// them is no drawing A2 allows. Glyph strips need this: letters stacked
    /// without leading join across every row, so the finder's cut columns
    /// outscore any honest arrangement of the words.
    pub fn best_of_its_sprites(&self) -> bool {
        self.sprites_score
            .is_some_and(|best| self.drawn_score >= best)
    }

    pub fn banked(&self) -> bool {
        self.found_score() - self.drawn_score > MARGIN && !self.best_of_its_sprites()
    }
}

const ANIMATION_TILES: usize = 4;

pub fn judge(png: &[u8], form: &str) -> Result<Option<Judgement>, String> {
    let image = indexed_bitmap_png(png).map_err(|error| error.0)?;
    let (bpp, meta) = match tile_form(form) {
        Some(tiles) => tiles,
        None if form == BITMAP && image.width == 8 => (GbaBpp::Bpp8, (1, 1)),
        None if form == BITMAP => return Ok(None),
        None => return Err(format!(".{form} is not a tile form")),
    };
    let mut tiles = ags::resource::build_file(&format!("SHEET.{form}"), png)?;
    let count = tiles.len() / bpp.tile_bytes();
    tiles.truncate(count * bpp.tile_bytes());
    if count < 2 {
        return Ok(None);
    }
    let drawn = TileLayout {
        meta,
        tiles_wide: image.width as usize / 8,
    };
    if drawn.tiles_wide == 0
        || !drawn.tiles_wide.is_multiple_of(meta.0)
        || count % (drawn.tiles_wide * meta.1) != 0
    {
        return Err(format!(
            "{}x{} pixels is not whole rows of {}x{} metatiles",
            image.width,
            image.height,
            meta.0 * 8,
            meta.1 * 8
        ));
    }
    let best = tile_sheet_layout(&tiles, bpp);
    let runs = if drawn.tiles_wide == 1 {
        sprite_runs(&tiles, bpp)
    } else {
        Vec::new()
    };
    let sprites_score = (meta != (1, 1)).then(|| {
        let sprites = count / (meta.0 * meta.1);
        (1..=sprites)
            .filter(|wide| sprites.is_multiple_of(*wide))
            .map(|wide| {
                let layout = TileLayout {
                    meta,
                    tiles_wide: wide * meta.0,
                };
                tile_sheet_score(&tiles, bpp, layout)
            })
            .fold(0.0, f64::max)
    });
    Ok(Some(Judgement {
        tiles: count,
        drawn,
        drawn_score: tile_sheet_score(&tiles, bpp, drawn),
        best,
        best_score: tile_sheet_score(&tiles, bpp, best),
        runs_score: sprite_runs_score(&tiles, bpp, &runs),
        runs,
        sprites_score,
        bitmap: form == BITMAP,
    }))
}

fn describe(layout: TileLayout) -> String {
    format!(
        "{} tiles wide, metatiles {}x{}",
        layout.tiles_wide,
        layout.meta.0 * 8,
        layout.meta.1 * 8
    )
}

fn run(root: &Path, report: bool) -> Result<(), String> {
    let (recipes, tilemaps) = recipes(root)?;
    let recipes = recipes.into_iter().collect::<Vec<_>>();
    let workers = std::thread::available_parallelism().map_or(1, |count| count.get());
    let chunk = recipes.len().div_ceil(workers).max(1);
    let judgements = std::thread::scope(|scope| {
        let handles = recipes
            .chunks(chunk)
            .map(|part| {
                scope.spawn(move || {
                    part.iter()
                        .map(|recipe| {
                            let name = recipe
                                .png
                                .strip_prefix(root)
                                .unwrap_or(&recipe.png)
                                .display()
                                .to_string();
                            let Ok(png) = std::fs::read(&recipe.png) else {
                                return Ok(None);
                            };
                            judge(&png, &recipe.form)
                                .map(|judgement| {
                                    judgement.map(|judgement| (name.clone(), judgement))
                                })
                                .map_err(|error| format!("{name}: {error}"))
                        })
                        .collect::<Vec<_>>()
                })
            })
            .collect::<Vec<_>>();
        handles
            .into_iter()
            .flat_map(|handle| handle.join().expect("layout worker"))
            .collect::<Vec<_>>()
    });
    let mut banked = Vec::new();
    let mut judged = 0;
    for (recipe, judgement) in recipes.iter().zip(judgements) {
        let Some((name, judgement)) = judgement? else {
            continue;
        };
        judged += 1;
        let runs = judgement
            .runs
            .iter()
            .map(|run| {
                format!(
                    "{}x{}@{}",
                    run.layout.meta.0 * 8,
                    run.layout.meta.1 * 8,
                    run.count
                )
            })
            .collect::<Vec<_>>()
            .join(" ");
        let sprites = judgement
            .sprites_score
            .map(|best| format!(", best whole sprites ({best:.3})"))
            .unwrap_or_default();
        let line = format!(
            "{name} .{}: drawn {} ({:.3}), finder {} ({:.3}), sprite runs {runs} ({:.3}){sprites}",
            recipe.form,
            describe(judgement.drawn),
            judgement.drawn_score,
            describe(judgement.best),
            judgement.best_score,
            judgement.runs_score
        );
        if report {
            println!(
                "{:+.3}\t{}{line}",
                judgement.found_score() - judgement.drawn_score,
                if judgement.banked() { "BANKED " } else { "" }
            );
        }
        if judgement.banked() {
            banked.push(line);
        }
    }
    if judged == 0 {
        return Err("layout check judged no tile sheet".into());
    }
    if report || (banked.is_empty() && tilemaps.is_empty()) {
        for problem in &tilemaps {
            println!("TILEMAP {problem}");
        }
        println!(
            "layout judged={judged} banked={} tilemap={}",
            banked.len(),
            tilemaps.len()
        );
        return Ok(());
    }
    if !tilemaps.is_empty() {
        return Err(format!(
            "{} tilemap part list problem(s):\n{}",
            tilemaps.len(),
            tilemaps.join("\n")
        ));
    }
    Err(format!(
        "{} banked tile sheet(s); redraw each at the finder's layout (agsgfx X.4bpp Y.png), or a mixed\n\
         column as the part list of its sprite runs (agsgfx X.4bpp Y.png --sprites):\n{}",
        banked.len(),
        banked.join("\n")
    ))
}

pub(super) fn entry(arguments: &[String]) -> ExitCode {
    let root = crate::compiler::routing::root();
    let result = match arguments {
        [] => run(root, false),
        [flag] if flag == "--report" => run(root, true),
        _ => {
            eprintln!("{USAGE}");
            return ExitCode::from(2);
        }
    };
    crate::result(result)
}

#[cfg(test)]
mod tests {
    use super::*;
    use ags::assets::image::png_from_bitmap;

    fn picture() -> Vec<u8> {
        let (wide, high) = (80usize, 16usize);
        (0..wide * high)
            .map(|index| u8::from((4..12).contains(&(index / wide))))
            .collect()
    }

    fn banked(pixels: &[u8], wide: usize, width: usize) -> Vec<u8> {
        let tiles: Vec<Vec<u8>> = (0..pixels.len() / 64)
            .map(|tile| {
                let (tx, ty) = (tile % (wide / 8), tile / (wide / 8));
                (0..64)
                    .map(|i| pixels[(ty * 8 + i / 8) * wide + tx * 8 + i % 8])
                    .collect()
            })
            .collect();
        let per_row = width / 8;
        let rows = tiles.len() / per_row;
        let mut sheet = vec![0u8; tiles.len() * 64];
        for (index, tile) in tiles.iter().enumerate() {
            let (tx, ty) = (index % per_row, index / per_row);
            for i in 0..64 {
                sheet[(ty * 8 + i / 8) * width + tx * 8 + i % 8] = tile[i];
            }
        }
        assert_eq!(sheet.len(), rows * 8 * width);
        sheet
    }

    #[test]
    fn a_sheet_drawn_as_its_picture_passes() {
        let palette = [0u8; 32];
        let png = png_from_bitmap(&picture(), &palette, 80).unwrap();
        let judgement = judge(&png, "4bpp").unwrap().unwrap();
        assert!(!judgement.banked(), "{judgement:?}");
    }

    #[test]
    fn a_banked_sheet_is_refused() {
        let palette = [0u8; 32];
        let png = png_from_bitmap(&banked(&picture(), 80, 40), &palette, 40).unwrap();
        let judgement = judge(&png, "4bpp").unwrap().unwrap();
        assert!(judgement.banked(), "{judgement:?}");
        assert_eq!(judgement.best.tiles_wide, 10);
    }

    #[test]
    fn a_bitmap_one_tile_wide_is_judged_as_its_column_of_tiles() {
        let palette = [0u8; 32];
        let mut column = banked(&picture(), 80, 8);
        column.extend([0u8; 32]);
        let png = png_from_bitmap(&column, &palette, 8).unwrap();
        let judgement = judge(&png, "bitmap").unwrap().unwrap();
        assert_eq!(judgement.tiles, 20);
        assert!(judgement.banked(), "{judgement:?}");
        let png = png_from_bitmap(&picture(), &palette, 80).unwrap();
        assert!(judge(&png, "bitmap").unwrap().is_none());
    }

    #[test]
    fn an_animation_strip_one_frame_wide_passes() {
        let pixels: Vec<u8> = (0..4 * 64)
            .map(|index| u8::from(index % 64 / 8 == 3 || index % 8 == 3))
            .collect();
        let png = png_from_bitmap(&pixels, &[0u8; 32], 8).unwrap();
        let judgement = judge(&png, "4bpp").unwrap().unwrap();
        assert!(!judgement.banked(), "{judgement:?}");
    }

    #[test]
    fn a_column_of_several_obj_shapes_is_refused() {
        let mut tiles: Vec<Vec<u8>> = Vec::new();
        for (wide, high) in [(2usize, 2usize), (4, 1)] {
            for tile in 0..wide * high {
                let (tx, ty) = (tile % wide, tile / wide);
                tiles.push(
                    (0..64)
                        .map(|i| {
                            let (x, y) = (tx * 8 + i % 8, ty * 8 + i / 8);
                            u8::from(x > 0 && y > 0 && x < wide * 8 - 1 && y < high * 8 - 1)
                        })
                        .collect(),
                );
            }
        }
        let pixels: Vec<u8> = tiles.concat();
        let png = png_from_bitmap(&pixels, &[0u8; 32], 8).unwrap();
        let judgement = judge(&png, "4bpp").unwrap().unwrap();
        assert!(judgement.banked(), "{judgement:?}");
    }

    /// A 32x8 sprite of four glyph tiles, or a blank one: each glyph a box
    /// with a shadow row beneath and an empty column after, as a font draws
    /// letters without leading.
    fn glyph_strip(sprites: &[bool], sprites_wide: usize) -> Vec<u8> {
        let glyph = |x: usize, y: usize| {
            if x < 7 && (y == 0 || y == 6 || x == 0 || x == 6) {
                15
            } else {
                u8::from(y == 7 && (1..7).contains(&x))
            }
        };
        let width = sprites_wide * 32;
        let mut pixels = vec![0u8; sprites.len() * 32 * 8];
        for (sprite, &drawn) in sprites.iter().enumerate() {
            let (left, top) = (sprite % sprites_wide * 32, sprite / sprites_wide * 8);
            for y in 0..8 {
                for x in 0..32 {
                    if drawn {
                        pixels[(top + y) * width + left + x] = glyph(x % 8, y);
                    }
                }
            }
        }
        pixels
    }

    #[test]
    fn a_glyph_strip_drawn_as_its_best_whole_sprites_passes() {
        let palette = [0u8; 32];
        let sprites = [false, false, false, true, true, true];
        let png = png_from_bitmap(&glyph_strip(&sprites, 1), &palette, 32).unwrap();
        let judgement = judge(&png, "4bpp32x8").unwrap().unwrap();
        assert!(
            judgement.found_score() - judgement.drawn_score > MARGIN,
            "the finder's cut columns beat the words: {judgement:?}"
        );
        assert_ne!(judgement.best.meta, (4, 1), "{judgement:?}");
        assert!(judgement.best_of_its_sprites(), "{judgement:?}");
        assert!(!judgement.banked(), "{judgement:?}");
        let png = png_from_bitmap(&glyph_strip(&sprites, 2), &palette, 64).unwrap();
        let worse = judge(&png, "4bpp32x8").unwrap().unwrap();
        assert!(!worse.best_of_its_sprites(), "{worse:?}");
        assert!(
            worse.banked(),
            "a worse arrangement of the sprites: {worse:?}"
        );
        let png = png_from_bitmap(&glyph_strip(&sprites, 1), &palette, 32).unwrap();
        let plain = judge(&png, "4bpp").unwrap().unwrap();
        assert_eq!(plain.sprites_score, None);
        assert!(plain.banked(), "no sprite shape named: {plain:?}");
    }

    fn map_sibling(name: &str) -> Result<Vec<u8>, String> {
        match name {
            "M.TSV" => Ok(b"a:u16\tb:u16\tc:u16\td:u16\n0\t0\t0\t0\n0\t1\t2\t0\n".to_vec()),
            "T.TSV" => Ok(b"a:u8\n1\n".to_vec()),
            other => Err(format!("{other} is missing")),
        }
    }

    #[test]
    fn a_tile_part_cut_plainly_beside_its_tilemap_is_refused() {
        let problems = tilemap_problems("P\tgbapal\nM\ttable\nP\t8bpp\n", &map_sibling);
        assert_eq!(problems.len(), 1, "{problems:?}");
        assert!(problems[0].contains("P.PNG is cut plainly"), "{problems:?}");
    }

    #[test]
    fn tile_parts_drawn_by_their_tilemap_pass() {
        let list = "P\tgbapal\nM\ttable\nP\t8bpp\tM\t0\t3\nF\t8bpp\tM\t1\t3\tframes\n";
        assert!(tilemap_problems(list, &map_sibling).is_empty());
        assert_eq!(
            tilemap_problems("M\ttable\nP\t8bpp\tN\t0\t3\n", &map_sibling).len(),
            1
        );
        assert_eq!(
            tilemap_problems("M\ttable\nP\t8bpp\tM\t5\t3\n", &map_sibling).len(),
            1
        );
    }

    #[test]
    fn part_lists_without_a_tilemap_are_not_judged_by_one() {
        assert!(tilemap_problems("T\ttable\nP\t8bpp\n", &map_sibling).is_empty());
    }

    #[test]
    fn tile_forms_name_their_depth_and_obj_shape() {
        assert_eq!(tile_form("4bpp"), Some((GbaBpp::Bpp4, (1, 1))));
        assert_eq!(tile_form("8bpp32x16"), Some((GbaBpp::Bpp8, (4, 2))));
        assert_eq!(tile_form("frames"), None);
        assert_eq!(tile_form("bitmap"), None);
        assert!(judged_form("bitmap") && judged_form("8bpp32x16") && !judged_form("frames"));
    }

    #[test]
    fn a_common_recipe_reads_its_input_from_common() {
        let directory = tempfile::tempdir().unwrap();
        let source_root = directory.path().join("games/GAME/SRC");
        let mut found = BTreeSet::new();
        let mut problems = Vec::new();
        recipes_in(
            &source_root,
            "\t.incbin \"COMMON/GRAPHICS/SET/TILES.4bpp.mtf\"\n",
            &mut found,
            &mut problems,
        );
        let found: Vec<_> = found.into_iter().map(|recipe| recipe.png).collect();
        assert_eq!(
            found,
            [directory
                .path()
                .join("games/COMMON/SRC/GRAPHICS/SET/TILES.PNG")]
        );
    }

    #[test]
    fn sources_name_tile_recipes_directly_and_through_part_lists() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        std::fs::create_dir_all(root.join("A")).unwrap();
        std::fs::write(
            root.join("A/LOGO.TSV"),
            "LOGO_TILES\tgbapal\nLOGO_TILES\t8bpp\n",
        )
        .unwrap();
        let mut found = BTreeSet::new();
        let mut problems = Vec::new();
        recipes_in(
            root,
            "\t.incbin \"A/LOGO.parts.lz\"\n\t.incbin \"A/WORD.4bpp32x16.lz\"\n\t.incbin \"A/HERO.frames\"\n\t.incbin \"A/SPARK.bitmap.lz\"\n",
            &mut found,
            &mut problems,
        );
        assert!(problems.is_empty(), "{problems:?}");
        let found: Vec<_> = found
            .into_iter()
            .map(|recipe| {
                (
                    recipe.png.strip_prefix(root).unwrap().display().to_string(),
                    recipe.form,
                )
            })
            .collect();
        assert_eq!(
            found,
            [
                ("A/LOGO_TILES.PNG".to_owned(), "8bpp".to_owned()),
                ("A/SPARK.PNG".to_owned(), "bitmap".to_owned()),
                ("A/WORD.PNG".to_owned(), "4bpp32x16".to_owned()),
            ]
        );
    }
}
