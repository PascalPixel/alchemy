//! The lettering of the README figures: a small pixel face defined here, in
//! tool source, like the marks below it. Nothing is read from a ROM, and no
//! glyph sheet or font file is tracked.
use std::collections::BTreeMap;

/// Device pixels per game pixel in the README figures, drawn for retina
/// displays and shown at half their width.
pub(crate) const FIGURE_SCALE: u32 = 2;
/// A text line, in game pixels: the box the face's seven-pixel capitals and
/// two-pixel descenders sit in.
pub(crate) const LINE: u32 = 16;

/// Printable ASCII, one glyph per character: rows from the cap line down,
/// `/` between rows, `#` ink. Capitals and digits stand seven rows high on
/// the baseline under row seven; descenders take two rows more. A glyph
/// advances its width and one pixel of spacing.
const FACE: [(char, &str); 95] = [
    (' ', "..."),
    ('!', "#/#/#/#/#/./#"),
    ('"', "#.#/#.#"),
    ('#', ".#.#./.#.#./#####/.#.#./#####/.#.#./.#.#."),
    ('$', "..#../.####/#.#../.###./..#.#/####./..#.."),
    ('%', "##..#/##..#/...#./..#../.#.../#..##/#..##"),
    ('&', ".##../#..#./#.#../.#.../#.#.#/#..#./.##.#"),
    ('\'', "#/#"),
    ('(', "..#/.#./#../#../#../.#./..#"),
    (')', "#../.#./..#/..#/..#/.#./#.."),
    ('*', "...../..#../#.#.#/.###./#.#.#/..#.."),
    ('+', "...../..#../..#../#####/..#../..#.."),
    (',', "../../../../../.#/.#/#."),
    ('-', "..../..../..../####"),
    ('.', "././././././#"),
    ('/', "....#/....#/...#./..#../.#.../#..../#...."),
    ('0', ".###./#...#/#..##/#.#.#/##..#/#...#/.###."),
    ('1', "..#../.##../..#../..#../..#../..#../.###."),
    ('2', ".###./#...#/....#/...#./..#../.#.../#####"),
    ('3', "#####/...#./..#../...#./....#/#...#/.###."),
    ('4', "...#./..##./.#.#./#..#./#####/...#./...#."),
    ('5', "#####/#..../####./....#/....#/#...#/.###."),
    ('6', "..##./.#.../#..../####./#...#/#...#/.###."),
    ('7', "#####/....#/...#./..#../.#.../.#.../.#..."),
    ('8', ".###./#...#/#...#/.###./#...#/#...#/.###."),
    ('9', ".###./#...#/#...#/.####/....#/...#./.##.."),
    (':', "././#/./././#"),
    (';', "../../.#/../../../.#/#."),
    ('<', "...#/..#./.#../#.../.#../..#./...#"),
    ('=', "..../..../####/..../####"),
    ('>', "#.../.#../..#./...#/..#./.#../#..."),
    ('?', ".###./#...#/....#/...#./..#../...../..#.."),
    ('@', ".###./#...#/#.###/#.#.#/#.###/#..../.###."),
    ('A', ".###./#...#/#...#/#####/#...#/#...#/#...#"),
    ('B', "####./#...#/#...#/####./#...#/#...#/####."),
    ('C', ".###./#...#/#..../#..../#..../#...#/.###."),
    ('D', "###../#..#./#...#/#...#/#...#/#..#./###.."),
    ('E', "#####/#..../#..../####./#..../#..../#####"),
    ('F', "#####/#..../#..../####./#..../#..../#...."),
    ('G', ".###./#...#/#..../#.###/#...#/#...#/.####"),
    ('H', "#...#/#...#/#...#/#####/#...#/#...#/#...#"),
    ('I', "###/.#./.#./.#./.#./.#./###"),
    ('J', "..###/...#./...#./...#./...#./#..#./.##.."),
    ('K', "#...#/#..#./#.#../##.../#.#../#..#./#...#"),
    ('L', "#..../#..../#..../#..../#..../#..../#####"),
    ('M', "#...#/##.##/#.#.#/#.#.#/#...#/#...#/#...#"),
    ('N', "#...#/#...#/##..#/#.#.#/#..##/#...#/#...#"),
    ('O', ".###./#...#/#...#/#...#/#...#/#...#/.###."),
    ('P', "####./#...#/#...#/####./#..../#..../#...."),
    ('Q', ".###./#...#/#...#/#...#/#.#.#/#..#./.##.#"),
    ('R', "####./#...#/#...#/####./#.#../#..#./#...#"),
    ('S', ".####/#..../#..../.###./....#/....#/####."),
    ('T', "#####/..#../..#../..#../..#../..#../..#.."),
    ('U', "#...#/#...#/#...#/#...#/#...#/#...#/.###."),
    ('V', "#...#/#...#/#...#/#...#/#...#/.#.#./..#.."),
    ('W', "#...#/#...#/#...#/#.#.#/#.#.#/#.#.#/.#.#."),
    ('X', "#...#/#...#/.#.#./..#../.#.#./#...#/#...#"),
    ('Y', "#...#/#...#/.#.#./..#../..#../..#../..#.."),
    ('Z', "#####/....#/...#./..#../.#.../#..../#####"),
    ('[', "###/#../#../#../#../#../###"),
    ('\\', "#..../#..../.#.../..#../...#./....#/....#"),
    (']', "###/..#/..#/..#/..#/..#/###"),
    ('^', "..#../.#.#./#...#"),
    ('_', "...../...../...../...../...../...../...../#####"),
    ('`', "#./.#"),
    ('a', "...../...../.###./....#/.####/#...#/.####"),
    ('b', "#..../#..../####./#...#/#...#/#...#/####."),
    ('c', "..../..../.###/#.../#.../#.../.###"),
    ('d', "....#/....#/.####/#...#/#...#/#...#/.####"),
    ('e', "...../...../.###./#...#/#####/#..../.###."),
    ('f', "..##/.#../####/.#../.#../.#../.#.."),
    ('g', "...../...../.####/#...#/#...#/#...#/.####/....#/.###."),
    ('h', "#..../#..../####./#...#/#...#/#...#/#...#"),
    ('i', "#/./#/#/#/#/#"),
    ('j', "..#/.../..#/..#/..#/..#/..#/#.#/.#."),
    ('k', "#.../#.../#..#/#.#./##../#.#./#..#"),
    ('l', "#/#/#/#/#/#/#"),
    ('m', "...../...../##.#./#.#.#/#.#.#/#.#.#/#.#.#"),
    ('n', "...../...../####./#...#/#...#/#...#/#...#"),
    ('o', "...../...../.###./#...#/#...#/#...#/.###."),
    ('p', "...../...../####./#...#/#...#/#...#/####./#..../#...."),
    ('q', "...../...../.####/#...#/#...#/#...#/.####/....#/....#"),
    ('r', "..../..../#.##/##../#.../#.../#..."),
    ('s', "...../...../.####/#..../.###./....#/####."),
    ('t', ".#../.#../####/.#../.#../.#../..##"),
    ('u', "...../...../#...#/#...#/#...#/#...#/.####"),
    ('v', "...../...../#...#/#...#/#...#/.#.#./..#.."),
    ('w', "...../...../#...#/#...#/#.#.#/#.#.#/.#.#."),
    ('x', "...../...../#...#/.#.#./..#../.#.#./#...#"),
    ('y', "...../...../#...#/#...#/#...#/#...#/.####/....#/.###."),
    ('z', "...../...../#####/...#./..#../.#.../#####"),
    ('{', "..#/.#./.#./#../.#./.#./..#"),
    ('|', "#/#/#/#/#/#/#"),
    ('}', "#../.#./.#./..#/.#./.#./#.."),
    ('~', "...../...../.#.../#.#.#/...#."),
];

/// One face: frames of `cell` game pixels (rows most significant bit
/// leftmost), each frame's advance, and the characters it draws.
#[derive(Clone, PartialEq, Debug)]
pub(crate) struct Letters {
    pub cell: (u32, u32),
    /// Rows from the top of a `LINE` box to the top of a cell.
    pub top: u32,
    pub rows: Vec<Vec<u16>>,
    pub advance: Vec<u32>,
    pub map: BTreeMap<char, usize>,
}

impl Letters {
    /// The figures' face: every printable ASCII character of `FACE`.
    pub(crate) fn face() -> Self {
        let mut letters = Self {
            cell: (8, 9),
            top: 4,
            rows: Vec::with_capacity(FACE.len()),
            advance: Vec::with_capacity(FACE.len()),
            map: BTreeMap::new(),
        };
        for (frame, (character, glyph)) in FACE.iter().enumerate() {
            let mut rows = vec![0u16; letters.cell.1 as usize];
            let mut width = 0;
            for (y, row) in glyph.split('/').enumerate() {
                width = row.len() as u32;
                for (x, pixel) in row.bytes().enumerate() {
                    if pixel == b'#' {
                        rows[y] |= 0x8000 >> x;
                    }
                }
            }
            letters.rows.push(rows);
            letters.advance.push(width + 1);
            letters.map.insert(*character, frame);
        }
        letters
    }
    /// A character's frame, when the face draws it.
    pub(crate) fn frame(&self, character: char) -> Option<usize> {
        self.map.get(&character).copied()
    }
    /// A string's width in game pixels; a character the face lacks takes a
    /// question mark's place.
    pub(crate) fn width(&self, text: &str) -> u32 {
        text.chars().map(|c| self.advance[self.stand_in(c)]).sum()
    }
    pub(crate) fn stand_in(&self, character: char) -> usize {
        self.frame(character)
            .or_else(|| self.frame('?'))
            .unwrap_or(0)
    }
    pub(crate) fn ink(&self, frame: usize, x: u32, y: u32) -> bool {
        x < self.cell.0 && y < self.cell.1 && self.rows[frame][y as usize] & (0x8000 >> x) != 0
    }
}

/// Weyard UI's three logo marks: Alchemy's mountain, The Broken Seal's sun
/// and The Lost Age's anchor. No tracked item, Psynergy or status icon draws
/// them cleanly, so they are drawn here as one-colour masks on a `LINE` box,
/// `#` ink, and take the labels' one-pixel shadow wherever they are shown.
pub(crate) const MARKS: [(&str, [&str; LINE as usize]); 3] = [
    (
        "alchemy",
        [
            "................",
            "................",
            "......#.........",
            "......#.........",
            ".....###........",
            ".....###........",
            "....#.#.#.......",
            "....#####.......",
            "...#######.#....",
            "...##########...",
            "..############..",
            "..#############.",
            ".##############.",
            ".##############.",
            "###############.",
            "................",
        ],
    ),
    (
        "tbs",
        [
            "................",
            "................",
            "......#.........",
            ".#....#....#....",
            "..#.......#.....",
            ".....###........",
            "....#####.......",
            "...#######......",
            "##.#######.##...",
            "...#######......",
            "....#####.......",
            ".....###........",
            "..#.......#.....",
            ".#....#....#....",
            "......#.........",
            "................",
        ],
    ),
    (
        "tla",
        [
            "................",
            ".....###........",
            "....#...#.......",
            ".....###........",
            "......#.........",
            "..#########.....",
            "......#.........",
            "......#.........",
            "......#.........",
            "......#.........",
            "#.....#.....#...",
            "##....#....##...",
            ".##...#...##....",
            "..###.#.###.....",
            "....#####.......",
            "................",
        ],
    ),
];
/// A mark's rows, by name.
pub(crate) fn mark(name: &str) -> &'static [&'static str; LINE as usize] {
    &MARKS
        .iter()
        .find(|(key, _)| *key == name)
        .expect("a Weyard UI mark")
        .1
}
/// A mark's width: its rightmost ink column and one more.
pub(crate) fn mark_width(name: &str) -> u32 {
    mark(name)
        .iter()
        .filter_map(|row| row.rfind('#'))
        .max()
        .map_or(0, |column| column as u32 + 1)
}

#[cfg(test)]
pub(crate) fn fixture() -> Letters {
    // A three-pixel square for every Latin-1 code but the C1 controls,
    // advancing four.
    let mut rows = vec![0u16; 16];
    for row in &mut rows[8..11] {
        *row = 0xe000;
    }
    Letters {
        cell: (16, 16),
        top: 0,
        rows: vec![rows; 224],
        advance: vec![4; 224],
        map: (0x20..0x100u32)
            .filter(|code| !(0x7f..0xa0).contains(code))
            .filter_map(|code| Some((char::from_u32(code)?, code as usize - 0x20)))
            .collect(),
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn the_face_draws_printable_ascii_inside_its_cell() {
        let face = Letters::face();
        assert_eq!(
            face.map.keys().copied().collect::<String>(),
            (' '..='~').collect::<String>()
        );
        for (character, glyph) in FACE {
            let rows = glyph.split('/').collect::<Vec<_>>();
            assert!(rows.len() <= face.cell.1 as usize, "{character:?}");
            assert!(
                rows.iter().all(|row| row.len() == rows[0].len()
                    && row.len() < face.cell.0 as usize
                    && row.bytes().all(|pixel| pixel == b'.' || pixel == b'#')),
                "{character:?}"
            );
            // Only a descender reaches below the baseline.
            assert!(
                rows.len() <= 7 || "gjpqy,;_".contains(character),
                "{character:?}"
            );
        }
        let h = face.frame('H').unwrap();
        assert!((0..7).all(|y| face.ink(h, 0, y) && face.ink(h, 4, y)));
        assert!(!face.ink(h, 5, 3) && !face.ink(h, 0, 7));
        assert_eq!(face.width("Hi"), 6 + 2);
        assert_eq!(face.width(" "), 4);
        assert_eq!(face.frame('é'), None);
        assert_eq!(face.width("—"), face.width("?"));
    }
    #[test]
    fn the_marks_are_one_colour_masks_on_a_line_box() {
        for (name, rows) in MARKS {
            assert!(
                rows.iter()
                    .all(|row| row.len() == LINE as usize
                        && row.chars().all(|c| c == '.' || c == '#')),
                "{name}"
            );
            // Each keeps its last column clear for the shadow.
            assert!(mark_width(name) < LINE, "{name}");
        }
        assert_eq!(
            (mark_width("alchemy"), mark_width("tbs"), mark_width("tla")),
            (15, 13, 13)
        );
    }
}
