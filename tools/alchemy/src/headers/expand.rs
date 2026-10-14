//! What each call saw: an edition's preprocessed text of a unit, read in
//! order as the compiler reads it, and matched back to the source's text.

use super::scan::{arms, body_of, bodies, cast_shape, is_function, prototyped, shape, view_shape, Body, How};
use crate::c_items::{declared, flat, Kind};
use std::collections::{BTreeMap, BTreeSet, HashMap};

/// The type of a function called with no declaration in sight.
pub(crate) const IMPLICIT: &str = "s32 (*)()";

/// One use of a wanted name inside a function body of the expansion.
#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) struct Sight {
    pub file: String,
    pub line: usize,
    pub function: String,
    /// Where the body's opening brace comes from.
    pub open: (String, usize),
    pub name: String,
    pub how: How,
    /// The declaration in sight, as a shape; none is an implicit one.
    pub seen: Option<String>,
    /// Whether the name's own definition stands above this use.
    pub defined: bool,
}

#[derive(Default)]
pub(crate) struct Expanded {
    /// The functions and objects this text defines for other code, as
    /// shapes.
    pub defined: BTreeMap<String, String>,
    /// What the headers on the search path declare of the wanted names:
    /// each header and the shape it gives.
    pub declared: BTreeMap<String, BTreeSet<(String, String)>>,
    /// The functions it defines `static`.
    pub hidden: BTreeSet<String>,
    pub sights: Vec<Sight>,
    /// The wanted names its code and initializers use at all.
    pub named: BTreeSet<String>,
    /// The lines of each file that reached the compiler.
    pub active: BTreeMap<String, BTreeSet<usize>>,
}

fn normalize(path: &str) -> String {
    super::model::normalize(path)
}

/// The file and line each line of an expansion comes from.
pub(crate) fn origins(root: &str, text: &str, unit: &str) -> Vec<(String, usize)> {
    let mut file = unit.to_string();
    let mut line = 1;
    text.lines()
        .map(|row| {
            if let Some(rest) = row.strip_prefix("# ") {
                if let Some((number, quoted)) = rest.split_once(' ') {
                    if let (Ok(next), Some(quoted)) = (number.parse::<usize>(), quoted.strip_prefix('"')) {
                        if let Some((path, _)) = quoted.split_once('"') {
                            file = normalize(path.strip_prefix(root).unwrap_or(path).trim_start_matches('/'));
                            line = next;
                            return (String::new(), 0);
                        }
                    }
                }
            }
            line += 1;
            (file.clone(), line - 1)
        })
        .collect()
}

fn words(text: &str) -> impl Iterator<Item = &str> {
    text.split(|c: char| !(c.is_ascii_alphanumeric() || c == '_')).filter(|w| !w.is_empty())
}

pub(crate) fn read(root: &str, text: &str, unit: &str, wanted: &dyn Fn(&str) -> bool) -> Expanded {
    let origins = origins(root, text, unit);
    let starts: Vec<usize> = std::iter::once(0).chain(text.match_indices('\n').map(|(at, _)| at + 1)).collect();
    let origin = |offset: usize| -> (String, usize) {
        let row = starts.partition_point(|&start| start <= offset).saturating_sub(1);
        origins.get(row).cloned().unwrap_or_default()
    };
    let mut out = Expanded::default();
    for (row, (file, line)) in text.lines().zip(&origins) {
        if *line > 0 && !row.trim().is_empty() && file.starts_with("games/tbs/") {
            out.active.entry(file.clone()).or_default().insert(*line);
        }
    }
    let mask = crate::camelotize::source::mask(text);
    let mut global: HashMap<String, String> = HashMap::new();
    // The functions whose bodies the text has reached, their own included.
    let mut bodied: BTreeSet<String> = BTreeSet::new();
    // A later declaration adds to an earlier one only a parameter list.
    let declare = |global: &mut HashMap<String, String>, name: &str, new: String| match global.get_mut(name) {
        Some(old) if is_function(old) && is_function(&new) && !prototyped(old) && prototyped(&new) => *old = new,
        Some(_) => {}
        None => {
            global.insert(name.to_string(), new);
        }
    };
    for piece in flat(text) {
        match piece.item.kind {
            Kind::Declaration => {
                let all = declared(&piece.item);
                let from = origin(piece.item.start).0;
                for d in &all {
                    if d.typedef || !wanted(&d.name) {
                        continue;
                    }
                    let shape = shape(&d.signature, &d.name);
                    let defines = !d.function && (!d.external || d.initialized);
                    if defines && !d.local && crate::c_items::c_text_file(&std::path::Path::new(root).join(&from)) {
                        out.defined.insert(d.name.clone(), shape.clone());
                    } else if !defines && super::model::spelling(&from).is_some() {
                        out.declared.entry(d.name.clone()).or_default().insert((from.clone(), shape.clone()));
                    }
                    declare(&mut global, &d.name, shape);
                }
                if let Some(at) = piece.item.text.find('=') {
                    let code = &mask[piece.item.start + at..piece.item.end];
                    out.named.extend(words(&String::from_utf8_lossy(code)).filter(|w| wanted(w)).map(str::to_string));
                }
            }
            Kind::Function { .. } => {
                let Some(body) = body_of(text, &mask, &piece.item, wanted) else {
                    continue;
                };
                declare(&mut global, &body.name, shape(&body.signature, &body.name));
                bodied.insert(body.name.clone());
                if body.local {
                    out.hidden.insert(body.name.clone());
                } else {
                    out.defined.insert(body.name.clone(), shape(&body.signature, &body.name));
                }
                let open = origin(body.open);
                for used in &body.uses {
                    out.named.insert(used.name.clone());
                    let local = body
                        .locals
                        .iter()
                        .filter(|l| l.name == used.name && l.end <= used.at && used.at < l.scope)
                        .max_by_key(|l| l.start)
                        .map(|l| shape(&l.signature, &l.name));
                    let seen = local.or_else(|| global.get(&used.name).cloned());
                    let (file, line) = origin(used.at);
                    let defined = bodied.contains(&used.name);
                    out.sights.push(Sight { file, line, function: body.name.clone(), open: open.clone(), name: used.name.clone(), how: used.how.clone(), seen, defined });
                }
            }
            _ => {}
        }
    }
    out
}

/// A source file as the expansions are matched to it.
pub(crate) struct Source {
    pub bodies: Vec<Body>,
    starts: Vec<usize>,
    arms: Vec<(usize, usize)>,
}

impl Source {
    pub(crate) fn new(text: &str, wanted: &dyn Fn(&str) -> bool) -> Source {
        let starts = std::iter::once(0).chain(text.match_indices('\n').map(|(at, _)| at + 1)).collect();
        Source { bodies: bodies(text, wanted), starts, arms: arms(text) }
    }

    /// The line of an offset, counted from one.
    pub(crate) fn line(&self, offset: usize) -> usize {
        self.starts.partition_point(|&start| start <= offset)
    }

    /// Whether a line is outside every conditional branch the expansion
    /// left out: its innermost branch has a line that reached the compiler.
    fn active(&self, line: usize, reached: Option<&BTreeSet<usize>>) -> bool {
        match self.arms.iter().filter(|(start, end)| *start < line && line < *end).max_by_key(|(start, _)| *start) {
            None => true,
            Some((start, end)) => reached.is_some_and(|lines| lines.range(start + 1..*end).next().is_some()),
        }
    }
}

/// A use of a name in the source and the declaration it had in sight.
#[derive(Clone, Debug, PartialEq, Eq, PartialOrd, Ord)]
pub(crate) struct Seen {
    pub file: String,
    pub at: usize,
    pub name: String,
    pub old: String,
    pub call: bool,
}

/// A use the expansion makes where the source's text does not write the
/// name: one a macro makes.
#[derive(Clone, Debug, PartialEq, Eq, PartialOrd, Ord)]
pub(crate) struct Stray {
    pub file: String,
    pub line: usize,
    pub name: String,
    pub old: String,
    pub call: bool,
}

fn old(sight: &Sight) -> Option<String> {
    match &sight.how {
        How::Call => Some(sight.seen.clone().unwrap_or_else(|| IMPLICIT.to_string())),
        How::Cast(text) => cast_shape(text),
        How::View { cast, deref } => view_shape(cast, *deref),
        How::Value => sight.seen.clone(),
    }
}

/// Each call of the expansion as a place in the source, when the source
/// writes the name there; otherwise a stray.
pub(crate) fn pair(sources: &BTreeMap<String, Source>, expanded: &Expanded) -> (Vec<Seen>, Vec<Stray>) {
    let (mut seen, mut strays) = (Vec::new(), Vec::new());
    let mut groups: Vec<(&(String, usize), &str, Vec<&Sight>)> = Vec::new();
    for sight in &expanded.sights {
        match groups.last_mut() {
            Some((open, function, list)) if **open == sight.open && *function == sight.function => list.push(sight),
            _ => groups.push((&sight.open, &sight.function, vec![sight])),
        }
    }
    for (open, function, sights) in groups {
        let stray = |sight: &Sight, strays: &mut Vec<Stray>| {
            if let Some(old) = old(sight) {
                strays.push(Stray { file: sight.file.clone(), line: sight.line, name: sight.name.clone(), old, call: sight.how != How::Value });
            }
        };
        let source = sources.get(&open.0);
        let body = source.and_then(|s| s.bodies.iter().find(|b| b.name == function && s.line(b.start) <= open.1 && open.1 <= s.line(b.close)));
        let (Some(source), Some(body)) = (source, body) else {
            sights.iter().for_each(|sight| stray(sight, &mut strays));
            continue;
        };
        let reached = expanded.active.get(&open.0);
        let names: BTreeSet<&str> = sights.iter().map(|s| s.name.as_str()).collect();
        for name in names {
            let there: Vec<&&Sight> = sights.iter().filter(|s| s.name == name).collect();
            let here: Vec<&super::scan::Use> = body.uses.iter().filter(|u| u.name == name && source.active(source.line(u.at), reached)).collect();
            let mut matched: Vec<(&Sight, &super::scan::Use)> = Vec::new();
            if there.len() == here.len() && there.iter().all(|s| s.file == open.0) {
                matched.extend(there.iter().map(|s| **s).zip(here.iter().copied()));
            } else {
                // Line by line, where both write the name as often.
                let mut lines: BTreeMap<usize, (Vec<&Sight>, Vec<&super::scan::Use>)> = BTreeMap::new();
                for sight in &there {
                    if sight.file == open.0 {
                        lines.entry(sight.line).or_default().0.push(sight);
                    } else {
                        stray(sight, &mut strays);
                    }
                }
                for used in &here {
                    lines.entry(source.line(used.at)).or_default().1.push(used);
                }
                for (_, (sights, uses)) in lines {
                    if sights.len() == uses.len() {
                        matched.extend(sights.into_iter().zip(uses));
                    } else {
                        sights.iter().for_each(|sight| stray(sight, &mut strays));
                    }
                }
            }
            for (sight, used) in matched {
                // A cast or view the source writes itself stays as it is;
                // a macro's cast or view stands where the source writes a
                // call or the bare name. A call pairs with no other use.
                let call = used.how == How::Call;
                if matches!(used.how, How::Cast(_) | How::View { .. }) || matches!((&used.how, &sight.how), (How::Call, How::Value) | (How::Value, How::Call | How::Cast(_))) {
                    continue;
                }
                if let Some(old) = old(sight) {
                    seen.push(Seen { file: open.0.clone(), at: used.at, name: name.to_string(), old, call });
                }
            }
        }
    }
    (seen, strays)
}

#[cfg(test)]
mod tests {
    use super::*;

    const EXPANSION: &str = "# 1 \"games/tbs/src/a.c\"\n# 1 \"/root/games/tbs/include/h.h\" 1\nvoid Far(s32 a, s32 b);\ns32 Known();\nstatic void Quiet(void) { }\n# 2 \"games/tbs/src/a.c\" 2\ns32 Known(u8 *p);\n\n\n\nvoid Run(s32 a)\n{\n\ts32 Local(s16);\n\tLocal(Known(0)); ((void (*)(s32))Far)(a);\n\n\n\n\tOther(a); Quiet(); Other(1);\n}\nvoid (*table[])(void) = { Other };\n";

    const SOURCE: &str = "#include \"h.h\"\ns32 Known(u8 *p);\n#define Far ((void (*)(s32))Far)\n#define TWICE(x) Other(x); Quiet(); Other(1)\n\nvoid Run(s32 a)\n{\n\ts32 Local(s16);\n\tLocal(Known(0)); Far(a);\n#if JA\n\tOther(a);\n#endif\n\tTWICE(a);\n}\nvoid (*table[])(void) = { Other };\n";

    fn wanted(name: &str) -> bool {
        ["Far", "Known", "Local", "Other", "Quiet", "Run"].contains(&name)
    }

    #[test]
    fn an_expansion_gives_each_use_the_declaration_in_sight() {
        let found = read("/root", EXPANSION, "games/tbs/src/a.c", &wanted);
        assert_eq!(found.defined.keys().collect::<Vec<_>>(), ["Run"]);
        assert_eq!(found.hidden.iter().collect::<Vec<_>>(), ["Quiet"]);
        assert_eq!(found.named.iter().map(String::as_str).collect::<Vec<_>>(), ["Far", "Known", "Local", "Other", "Quiet"]);
        let seen: Vec<(&str, usize, Option<&str>)> = found.sights.iter().map(|s| (s.name.as_str(), s.line, s.seen.as_deref())).collect();
        assert_eq!(
            seen,
            vec![
                ("Local", 9, Some("s32 (*)(s16)")),
                ("Known", 9, Some("s32 (*)(u8 *)")),
                ("Far", 9, Some("void (*)(s32, s32)")),
                ("Other", 13, None),
                ("Quiet", 13, Some("void (*)(void)")),
                ("Other", 13, None),
            ]
        );
        assert_eq!(found.sights[2].how, How::Cast("void (*)(s32)".into()));
        assert_eq!(found.sights[0].open, ("games/tbs/src/a.c".to_string(), 7));
        assert!(found.active["games/tbs/src/a.c"].contains(&9) && !found.active["games/tbs/src/a.c"].contains(&11));
        assert!(found.active.contains_key("games/tbs/include/h.h"));
    }

    #[test]
    fn a_use_knows_whether_its_definition_stands_above() {
        let text = "# 1 \"games/tbs/src/a.c\"\nvoid Run(void)\n{\n\tLate(1); Run();\n}\nvoid Late(s32 a)\n{\n}\nvoid After(void)\n{\n\tLate(2); Far();\n}\n";
        let found = read("/root", text, "games/tbs/src/a.c", &|name| ["Run", "Late", "Far"].contains(&name));
        let seen: Vec<(&str, usize, bool)> = found.sights.iter().map(|sight| (sight.name.as_str(), sight.line, sight.defined)).collect();
        assert_eq!(seen, [("Late", 3, false), ("Run", 3, true), ("Late", 10, true), ("Far", 10, false)]);
    }

    #[test]
    fn calls_pair_with_the_source_where_it_writes_them() {
        let found = read("/root", EXPANSION, "games/tbs/src/a.c", &wanted);
        let mut sources = BTreeMap::new();
        sources.insert("games/tbs/src/a.c".to_string(), Source::new(SOURCE, &wanted));
        let (seen, strays) = pair(&sources, &found);
        let at = |needle: &str| SOURCE.find(needle).unwrap();
        assert_eq!(
            seen,
            vec![
                Seen { file: "games/tbs/src/a.c".into(), at: at("Far(a)"), name: "Far".into(), old: "void (*)(s32)".into(), call: true },
                Seen { file: "games/tbs/src/a.c".into(), at: at("Known(0)"), name: "Known".into(), old: "s32 (*)(u8 *)".into(), call: true },
                Seen { file: "games/tbs/src/a.c".into(), at: at("Local(Known"), name: "Local".into(), old: "s32 (*)(s16)".into(), call: true },
            ]
        );
        // The macro's calls are nowhere in the body's text; the branch the
        // edition leaves out is not counted against them.
        assert_eq!(strays.iter().map(|s| (s.name.as_str(), s.line, s.old.as_str())).collect::<Vec<_>>(), [("Other", 13, IMPLICIT), ("Other", 13, IMPLICIT), ("Quiet", 13, "void (*)(void)")]);
    }
}
