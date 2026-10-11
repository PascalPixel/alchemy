//! A unit's casts measured against its bytes: every cast the plan
//! proposes is written, the unit is compiled in its six editions, and each
//! cast the unit's functions do not need is taken out again. A function's
//! code depends only on its own calls, so one build measures one cast in
//! every function at once.

use super::objects::{difference, differing, first_error, Bench, Built};
use super::plan::{apply, cast_edits, Cast, Draft};
#[cfg(test)]
use super::plan::Form;
use std::collections::{BTreeMap, BTreeSet, VecDeque};
use std::path::Path;

const UNMEASURED: &str = "this is not measured yet";

#[derive(Default)]
pub(crate) struct Outcome {
    pub text: String,
    pub kept: usize,
    pub dropped: usize,
    /// The parts that differ from the unit's object before the pass, each
    /// with how; `compile` with the first message when it does not build.
    pub differing: Vec<(String, String)>,
    pub builds: usize,
}

fn reason(cast: &Cast, measured: &str) -> String {
    format!("as {}, its prototype, {measured}", cast.truth)
}

/// The text with the file's own edits and these casts, each under its reason.
pub(crate) fn render(old: &str, draft: &Draft, kept: &BTreeMap<usize, String>) -> String {
    let mut edits = draft.edits.clone();
    for (index, measured) in kept {
        edits.extend(cast_edits(old, &draft.casts[*index], &reason(&draft.casts[*index], measured)));
    }
    apply(old, &edits)
}

/// The functions a compiler's errors are in.
pub(crate) fn failed_functions(messages: &str) -> BTreeSet<String> {
    let mut found = BTreeSet::new();
    let mut current: Option<&str> = None;
    for line in messages.lines() {
        if let Some(at) = line.find("In function `") {
            let rest = &line[at + "In function `".len()..];
            current = rest.split('\'').next();
        } else if line.contains("At top level") {
            current = None;
        } else if !line.contains("warning:") && !line.contains("In file included") && !line.trim_start().starts_with("from ") && !line.trim().is_empty() {
            if let Some(name) = current {
                found.insert(name.to_string());
            }
        }
    }
    found
}

struct Trial<'a> {
    root: &'a Path,
    bench: &'a Bench,
    file: &'a str,
    old: &'a str,
    draft: &'a Draft,
    before: &'a Built,
    /// What differs with every cast in: no cast answers for it.
    bad: BTreeSet<String>,
    kept: BTreeMap<usize, String>,
    builds: usize,
}

impl Trial<'_> {
    fn build(&mut self, without: &BTreeSet<usize>) -> Built {
        let kept: BTreeMap<usize, String> = self.kept.iter().filter(|(index, _)| !without.contains(index)).map(|(index, measured)| (*index, measured.clone())).collect();
        self.builds += 1;
        self.bench.build(self.root, self.file, &render(self.old, self.draft, &kept))
    }

    /// Whether each function still gives its bytes without the casts
    /// taken from it; a function that does not gets the measured reason.
    fn judge(&mut self, mut taken: BTreeMap<String, Vec<usize>>) -> BTreeMap<String, Result<(), String>> {
        let mut verdicts = BTreeMap::new();
        let single = taken.len() == 1;
        while !taken.is_empty() {
            let without: BTreeSet<usize> = taken.values().flatten().copied().collect();
            let now = self.build(&without);
            if let Some(Err(messages)) = now.iter().find(|built| built.is_err()) {
                let mut failed: Vec<String> = failed_functions(messages).into_iter().filter(|name| taken.contains_key(name)).collect();
                if failed.is_empty() {
                    // An error no function owns: nothing of this round is known.
                    failed = taken.keys().cloned().collect();
                }
                for name in failed {
                    taken.remove(&name);
                    verdicts.insert(name.clone(), Err(format!("{name} does not compile")));
                }
                continue;
            }
            let worse: BTreeSet<String> = differing(self.before, &now).difference(&self.bad).cloned().collect();
            // A function that changes size changes the symbol table and
            // the section around it too: those follow, they do not accuse.
            let code = |name: &String| self.before.iter().chain(&now).flatten().any(|parts| parts.get(name).is_some_and(|part| part.code));
            let functions: Vec<&String> = worse.iter().filter(|name| code(name)).collect();
            let unowned = functions.iter().any(|name| !taken.contains_key(*name)) || (functions.is_empty() && !worse.is_empty());
            if taken.len() > 1 && unowned {
                // A change outside the functions under trial: one at a time.
                for (name, casts) in std::mem::take(&mut taken) {
                    verdicts.extend(self.judge(BTreeMap::from([(name, casts)])));
                }
                break;
            }
            for (name, _) in std::mem::take(&mut taken) {
                // Alone, a function answers for whatever its casts changed.
                let part = if worse.contains(&name) { Some(&name) } else { worse.iter().next().filter(|_| single) };
                let verdict = match part {
                    Some(part) => Err(format!("{part} {}", difference(self.before, &now, part).unwrap_or_else(|| "differs".to_string()))),
                    None => Ok(()),
                };
                verdicts.insert(name, verdict);
            }
        }
        verdicts
    }
}

pub(crate) fn settle(root: &Path, bench: &Bench, file: &str, old: &str, before: &Built, draft: &Draft) -> Outcome {
    let all: BTreeMap<usize, String> = (0..draft.casts.len()).map(|index| (index, UNMEASURED.to_string())).collect();
    let mut trial = Trial { root, bench, file, old, draft, before, bad: BTreeSet::new(), kept: all, builds: 0 };
    let full = trial.build(&BTreeSet::new());
    let report = |text: String, kept: usize, now: &Built, builds: usize| {
        let mut found: Vec<(String, String)> = Vec::new();
        if let Some(Err(messages)) = now.iter().find(|built| built.is_err()) {
            found.push(("compile".to_string(), first_error(messages)));
        }
        for name in differing(before, now) {
            if name != "compile" {
                found.push((name.clone(), difference(before, now, &name).unwrap_or_else(|| "is there on one side only".to_string())));
            }
        }
        Outcome { text, kept, dropped: draft.casts.len() - kept, differing: found, builds }
    };
    if full.iter().any(|built| built.is_err()) || draft.casts.is_empty() {
        return report(render(old, draft, &trial.kept), draft.casts.len(), &full, trial.builds);
    }
    trial.bad = differing(before, &full);
    // A function that differs with every cast in may be right with none:
    // then the header's declaration is what it read all along. A function
    // that does not compile without its casts keeps them for the rounds.
    let mut theirs: BTreeSet<usize> = draft.casts.iter().enumerate().filter(|(_, cast)| trial.bad.contains(&cast.function)).map(|(index, _)| index).collect();
    for _ in 0..2 {
        if theirs.is_empty() {
            break;
        }
        let none = trial.build(&theirs);
        if let Some(Err(messages)) = none.iter().find(|built| built.is_err()) {
            let failed = failed_functions(messages);
            let fewer: BTreeSet<usize> = theirs.iter().copied().filter(|index| !failed.contains(&draft.casts[*index].function)).collect();
            if fewer.len() == theirs.len() {
                break;
            }
            theirs = fewer;
            continue;
        }
        let still = differing(before, &none);
        let healed: BTreeSet<usize> = theirs.iter().copied().filter(|index| !still.contains(&draft.casts[*index].function)).collect();
        if !healed.is_empty() {
            trial.kept.retain(|index, _| !healed.contains(index));
            let now = trial.build(&BTreeSet::new());
            if now.iter().all(|built| built.is_ok()) {
                trial.bad = differing(before, &now);
            }
        }
        break;
    }
    // Each function's casts: first all of them; where the function needs
    // some, those of each name and shape together; then one at a time.
    let measurable = |name: &str| before.iter().flatten().any(|parts| parts.get(name).is_some_and(|part| part.code));
    let mut open: BTreeMap<String, VecDeque<Vec<usize>>> = BTreeMap::new();
    let mut alone: Vec<usize> = Vec::new();
    for (index, cast) in draft.casts.iter().enumerate() {
        if trial.bad.contains(&cast.function) || !trial.kept.contains_key(&index) {
            continue;
        }
        if measurable(&cast.function) {
            let queue = open.entry(cast.function.clone()).or_insert_with(|| VecDeque::from([Vec::new()]));
            queue[0].push(index);
        } else {
            alone.push(index);
        }
    }
    while !open.is_empty() {
        let taken: BTreeMap<String, Vec<usize>> = open.iter().map(|(name, queue)| (name.clone(), queue[0].clone())).collect();
        for (name, verdict) in trial.judge(taken) {
            let Some(queue) = open.get_mut(&name) else {
                continue;
            };
            let Some(tried) = queue.pop_front() else {
                continue;
            };
            match verdict {
                Ok(()) => tried.iter().for_each(|index| {
                    trial.kept.remove(index);
                }),
                Err(measured) if tried.len() == 1 => {
                    trial.kept.insert(tried[0], measured);
                }
                Err(_) => {
                    let mut groups: BTreeMap<(&str, &str), Vec<usize>> = BTreeMap::new();
                    for index in &tried {
                        groups.entry((&draft.casts[*index].name, &draft.casts[*index].old)).or_default().push(*index);
                    }
                    let parts: Vec<Vec<usize>> = if groups.len() > 1 { groups.into_values().collect() } else { tried.iter().map(|index| vec![*index]).collect() };
                    parts.into_iter().rev().for_each(|part| queue.push_front(part));
                }
            }
            if queue.is_empty() {
                open.remove(&name);
            }
        }
    }
    // A cast in code that has no bytes of its own (an inline function)
    // answers for every function that expands it.
    for index in alone {
        let name = draft.casts[index].function.clone();
        match trial.judge(BTreeMap::from([(name, vec![index])])).into_values().next() {
            Some(Ok(())) => {
                trial.kept.remove(&index);
            }
            Some(Err(measured)) => {
                trial.kept.insert(index, measured);
            }
            None => {}
        }
    }
    let text = render(old, draft, &trial.kept);
    let last = trial.build(&BTreeSet::new());
    if differing(before, &last).is_subset(&trial.bad) && last.iter().all(|built| built.is_ok()) {
        report(text, trial.kept.len(), &last, trial.builds)
    } else {
        // The casts act on each other in a way the rounds did not see.
        let all: BTreeMap<usize, String> = (0..draft.casts.len()).map(|index| (index, UNMEASURED.to_string())).collect();
        report(render(old, draft, &all), draft.casts.len(), &full, trial.builds)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn errors_name_the_functions_they_are_in() {
        let messages = "In file included from a.c:3:\nb.h:4: warning: x\na.c: In function `Run':\na.c:10: warning: implicit declaration of function `Draw'\na.c: In function `Walk':\na.c:20: too many arguments to function `Step'\na.c: At top level:\na.c:30: parse error\na.c: In function `Stop':\na.c:40: void value not ignored as it ought to be\n";
        assert_eq!(failed_functions(messages).into_iter().collect::<Vec<_>>(), ["Stop", "Walk"]);
    }

    #[test]
    fn kept_casts_are_written_with_their_reasons() {
        let old = "s32 Get();\nvoid Run(void)\n{\n\tGet(1);\n\tGet(2);\n}\n";
        let cast = |needle: &str| Cast { at: old.find(needle).unwrap(), name: "Get".into(), old: "s32 (*)()".into(), truth: "u8 (*)(u8)".into(), function: "Run".into(), form: Form::Shape };
        let draft = Draft { edits: vec![super::super::plan::removal(old, 0, 10)], casts: vec![cast("Get(1)"), cast("Get(2)")] };
        let kept = BTreeMap::from([(1, "Run is 4 bytes longer".to_string())]);
        assert_eq!(
            render(old, &draft, &kept),
            "void Run(void)\n{\n\tGet(1);\n\t/* FAKEMATCH: Get is called as s32 (*)(); as u8 (*)(u8), its prototype, Run is 4 bytes longer */\n\t((s32 (*)())Get)(2);\n}\n"
        );
    }
}
