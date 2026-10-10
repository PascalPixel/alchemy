//! Aiming the search as a person would: rewrite where the code differs,
//! most of all at and just before the first divergence (allocation and
//! scheduling cascade from early differences), and try the kinds of
//! rewrite that have already paid on this function more often.

use super::ast::Function;
use super::mutate::{mutate_with, Kind, Rng, Step};
use super::types::Env;
use crate::diff::Heat;

/// Every site's weight starts here, so cold code stays reachable.
pub const FLOOR: u64 = 1;
/// Each differing instruction charged to a line.
pub const PER_ROW: u64 = 4;
/// The line where the comparison first diverges, and each of the
/// `BEFORE_LINES` printed lines just before it.
pub const FIRST: u64 = 24;
pub const BEFORE: u64 = 8;
const BEFORE_LINES: usize = 3;
/// Draws for one rewrite; the last is taken whatever its heat.
const TRIES: usize = 24;

fn squeeze(text: &str) -> Vec<String> {
    text.lines().map(|line| line.split_whitespace().collect::<Vec<_>>().join(" ")).collect()
}

/// The heat of the base function's printed lines, carried to any later
/// printing of it by their longest common subsequence.
pub struct Aim {
    base: Vec<String>,
    hot: Vec<u64>,
    cap: u64,
}

impl Aim {
    /// `printed` is the base function as the permuter prints it, and
    /// `first_line` the source line it starts on in the compiled text that
    /// `heat` was measured from.
    pub fn new(printed: &str, first_line: u32, heat: &Heat) -> Aim {
        let base = squeeze(printed);
        let line = |index: usize| first_line + index as u32;
        let first = heat.first.and_then(|first| first.checked_sub(first_line)).map(|first| first as usize);
        let hot: Vec<u64> = (0..base.len())
            .map(|index| {
                let rows = heat.lines.get(&line(index)).copied().unwrap_or(0) as u64;
                let bonus = match first {
                    Some(first) if index == first => FIRST,
                    Some(first) if index < first && index + BEFORE_LINES >= first => BEFORE,
                    _ => 0,
                };
                PER_ROW * rows + bonus
            })
            .collect();
        let cap = FLOOR + hot.iter().copied().max().unwrap_or(0);
        Aim { base, hot, cap }
    }

    /// Whether any line carries heat (else aiming is uniform).
    pub fn aims(&self) -> bool {
        self.cap > FLOOR
    }

    /// The heat of each line of a later printing: an aligned line keeps its
    /// base line's, a new line takes the line above it's.
    fn hot_of(&self, current: &[String]) -> Vec<u64> {
        let (_, script) = crate::diff::compare(current, &self.base);
        let (mut out, mut j) = (Vec::with_capacity(current.len()), 0);
        for entry in &script {
            match entry.as_bytes()[0] {
                b' ' => {
                    out.push(self.hot[j]);
                    j += 1;
                }
                b'+' => j += 1,
                _ => out.push(out.last().copied().unwrap_or(0)),
            }
        }
        out
    }

    /// A rewrite's weight: the floor plus the heat of the lines it changed
    /// (between the printings' common prefix and suffix), or of the lines
    /// on either side where it only inserted.
    fn weight(hot: &[u64], before: &[String], after: &[String]) -> u64 {
        let prefix = before.iter().zip(after).take_while(|(a, b)| a == b).count();
        let room = before.len().min(after.len()) - prefix;
        let suffix = before.iter().rev().zip(after.iter().rev()).take(room).take_while(|(a, b)| a == b).count();
        let span = prefix..before.len() - suffix;
        let heat = if span.is_empty() {
            let above = prefix.checked_sub(1).and_then(|at| hot.get(at)).copied().unwrap_or(0);
            above.max(hot.get(prefix).copied().unwrap_or(0))
        } else {
            hot[span].iter().sum()
        };
        FLOOR + heat
    }
}

/// One rewrite whose site is drawn with probability proportional to its
/// weight: draw a rewrite as `mutate_with` does, keep it with probability
/// weight / cap, else undo it and draw again. The kept step records the
/// generator as it chose its site, so replay and the minimizer repeat it.
pub fn aimed(function: &mut Function, env: &mut Env, rng: &mut Rng, kinds: &[(Kind, u64)], aim: &Aim) -> Option<Step> {
    let before = squeeze(&function.print());
    let hot = aim.hot_of(&before);
    let saved = function.clone();
    for attempt in 0..TRIES {
        let step = mutate_with(function, env, rng, kinds)?;
        let weight = Aim::weight(&hot, &before, &squeeze(&function.print()));
        if attempt + 1 == TRIES || rng.next() % aim.cap < weight {
            return Some(step);
        }
        *function = saved.clone();
        env.refresh(function);
    }
    None
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::collections::BTreeMap;

    fn heat(lines: &[(u32, usize)], first: Option<u32>) -> Heat {
        Heat { lines: lines.iter().copied().collect::<BTreeMap<_, _>>(), first }
    }

    #[test]
    fn heat_lands_on_printed_lines_and_the_lines_before_the_first_divergence() {
        let printed = "void f(void)\n{\n    a = 1;\n    b = 2;\n    c = 3;\n    d = 4;\n    e = 5;\n}\n";
        let aim = Aim::new(printed, 10, &heat(&[(16, 3), (15, 1)], Some(15)));
        // Lines 10.. are the printing's 0..; 15 is `d = 4;`, index 5.
        assert_eq!(aim.hot, [0, 0, BEFORE, BEFORE, BEFORE, PER_ROW + FIRST, 3 * PER_ROW, 0]);
        assert_eq!(aim.cap, FLOOR + PER_ROW + FIRST);
        assert!(aim.aims());
        assert!(!Aim::new(printed, 10, &Heat::default()).aims());
        // A later printing with a new line keeps each old line's heat.
        let later = squeeze("void f(void)\n{\n    int t;\n    a = 1;\n    b = 2;\n    c = 3;\n    d = 4;\n    e = 5;\n}\n");
        assert_eq!(aim.hot_of(&later), [0, 0, 0, BEFORE, BEFORE, BEFORE, PER_ROW + FIRST, 3 * PER_ROW, 0]);
    }

    #[test]
    fn a_rewrite_weighs_the_heat_of_the_lines_it_changes() {
        let before = squeeze("a;\nb;\nc;\nd;\n");
        let hot = [0, 5, 7, 0];
        assert_eq!(Aim::weight(&hot, &before, &squeeze("a;\nB;\nc;\nd;\n")), FLOOR + 5);
        assert_eq!(Aim::weight(&hot, &before, &squeeze("a;\nc;\nb;\nd;\n")), FLOOR + 12);
        assert_eq!(Aim::weight(&hot, &before, &squeeze("A;\nb;\nc;\nd;\n")), FLOOR);
        // An insertion weighs the hotter of its neighbours.
        assert_eq!(Aim::weight(&hot, &before, &squeeze("a;\nb;\nt;\nc;\nd;\n")), FLOOR + 7);
    }

    #[test]
    fn aimed_sites_follow_heat_deterministically() {
        use super::super::parse::{locate, scan_unit};
        let source = "int g0; int g1; int g2; int g3; int g4; int g5;\nint f(int a, int b)\n{\n    g0 = a + b;\n    g1 = a + b;\n    g2 = a + b;\n    g3 = a + b;\n    g4 = a + b;\n    g5 = a + b;\n    return a + b;\n}\n";
        let unit = scan_unit(source).unwrap();
        let located = locate(source, None, &unit.typedef_names()).unwrap();
        let printed = located.function.print();
        // All the heat on `g4 = a + b;`, the printing's sixth line.
        let first_line = 1;
        let hot_line = first_line + squeeze(&printed).iter().position(|line| line == "g4 = a + b;").unwrap() as u32;
        let aim = Aim::new(&printed, first_line, &heat(&[(hot_line, 6)], None));
        let swaps = [(Kind::SwapOperands, 1)];
        let mut hits = (0, 0);
        let mut trail = Vec::new();
        let mut rng = Rng::new(7);
        for _ in 0..200 {
            let mut function = located.function.clone();
            let mut env = Env::new(&unit, &function);
            let mut uniform = function.clone();
            let mut uniform_env = Env::new(&unit, &uniform);
            let step = aimed(&mut function, &mut env, &mut rng, &swaps, &aim).unwrap();
            trail.push(step.clone());
            hits.0 += usize::from(squeeze(&function.print()).contains(&"g4 = b + a;".to_string()));
            mutate_with(&mut uniform, &mut uniform_env, &mut rng, &swaps).unwrap();
            hits.1 += usize::from(squeeze(&uniform.print()).contains(&"g4 = b + a;".to_string()));
            // The kept step replays to the same rewrite.
            let mut replayed = located.function.clone();
            let replay_env = Env::new(&unit, &replayed);
            assert!(super::super::mutate::apply(step.kind, &mut replayed, &replay_env, &mut step.rng.clone()));
            assert_eq!(replayed.print(), function.print());
        }
        assert!(hits.0 > 3 * hits.1, "aimed {} against uniform {}", hits.0, hits.1);
        // The same seed draws the same trail.
        let mut again = Rng::new(7);
        for step in &trail {
            let mut function = located.function.clone();
            let mut env = Env::new(&unit, &function);
            assert_eq!(&aimed(&mut function, &mut env, &mut again, &swaps, &aim).unwrap(), step);
            let mut uniform = located.function.clone();
            let mut uniform_env = Env::new(&unit, &uniform);
            mutate_with(&mut uniform, &mut uniform_env, &mut again, &swaps).unwrap();
        }
    }

}
