use crate::{
    cli::Options,
    disasm::disassemble,
    render::{align_streams, ordered_lines},
};
use candidate_compiler::verify::{assemble, compile_source, copy_text, run};
use regex::Regex;
use std::path::Path;

type Var = (String, Option<u8>, Option<i32>);

#[derive(Clone, Debug)]
pub struct Report {
    pub text: String,
    pub dimensions: Vec<&'static str>,
    pub repair: Option<Repair>,
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub enum Repair {
    SwapDeclarations { left: String, right: String },
    SplitLifetime { name: String },
    MergeZeroCarrier,
    ReciprocalRoleSwap { name: String },
}

impl Repair {
    pub fn dimensions(&self) -> &'static [&'static str] {
        match self {
            Self::SwapDeclarations { .. } => &["declaration_order"],
            Self::SplitLifetime { .. } => &["block_lifetime", "loop_spelling"],
            Self::MergeZeroCarrier => &["temporary"],
            Self::ReciprocalRoleSwap { .. } => {
                &["temporary", "evaluation_order", "commutative_order"]
            }
        }
    }
    pub fn label(&self) -> String {
        match self {
            Self::SwapDeclarations { left, right } => format!("swap_declarations({left},{right})"),
            Self::SplitLifetime { name } => format!("split_lifetime({name})"),
            Self::MergeZeroCarrier => "merge_lifetime(zero_carrier)".into(),
            Self::ReciprocalRoleSwap { name } => format!("reciprocal_register_role_swap({name})"),
        }
    }
}

fn report(text: impl Into<String>, repair: Option<Repair>) -> Report {
    let dimensions = repair
        .as_ref()
        .map_or_else(Vec::new, |repair| repair.dimensions().to_vec());
    Report {
        text: text.into(),
        dimensions,
        repair,
    }
}

fn text_section(object: &Path, output: &Path) -> Result<Vec<u8>, String> {
    copy_text(&object.to_string_lossy(), &output.to_string_lossy())?;
    std::fs::read(output).map_err(|e| e.to_string())
}

pub fn decode(
    root: &Path,
    options: &Options,
    routing: &Path,
    stem: &str,
    source: &str,
    work: &Path,
) -> Result<Report, String> {
    if options.precompiled_object.is_some() {
        return Ok(report(
            "allocator_order=undecoded reason=standalone-owner-only\n",
            None,
        ));
    }
    let dir = work.join("allocator-order");
    std::fs::create_dir_all(&dir).map_err(|e| format!("{}: {e}", dir.display()))?;
    let input = root
        .join(source)
        .canonicalize()
        .map_err(|e| format!("{source}: {e}"))?;
    let assembly = dir.join("candidate.s");
    let object = dir.join("candidate.o");
    let mut flags = options.flags.clone();
    flags.extend(["-g", "-dp", "-dr", "-dl", "-dg"].map(str::to_string));
    compile_source(
        &input.to_string_lossy(),
        &routing.to_string_lossy(),
        &assembly.to_string_lossy(),
        &flags,
        options.target,
        &options.configuration,
        &dir,
    )?;
    assemble(&assembly.to_string_lossy(), &object.to_string_lossy())?;
    if text_section(&work.join(format!("{stem}.o")), &dir.join("normal.text"))?
        != text_section(&object, &dir.join("diagnostic.text"))?
    {
        return Ok(report(
            "allocator_order=undecoded reason=debug-text-drift\n",
            None,
        ));
    }
    let file = input
        .file_name()
        .ok_or("source has no file name")?
        .to_string_lossy();
    let read = |suffix: &str| {
        std::fs::read_to_string(dir.join(format!("{file}{suffix}"))).map_err(|e| e.to_string())
    };
    let dwarf = run(
        &[
            "arm-none-eabi-readelf",
            "--debug-dump=info",
            &object.to_string_lossy(),
        ],
        &dir,
    )?;
    let asm = std::fs::read_to_string(assembly).map_err(|e| e.to_string())?;
    let dumps = [read(".00.rtl")?, read(".17.lreg")?, read(".18.greg")?];
    let uids = asm_uids(&asm, stem);
    if uids.is_empty() {
        return Ok(report(
            "allocator_order=undecoded reason=missing-owner-uids\n",
            None,
        ));
    }
    let lines = |name: &str| -> Result<Vec<String>, String> {
        Ok(ordered_lines(&disassemble(
            &work.join(name).to_string_lossy(),
            0.0,
        )?))
    };
    let (mut left, mut right) = (lines("candidate.bin")?, lines("reference.bin")?);
    if left.len() < uids.len() || right.len() < uids.len() {
        return Ok(report(
            "allocator_order=undecoded reason=instruction-stream-shape\n",
            None,
        ));
    }
    left.truncate(uids.len());
    right.truncate(uids.len());
    Ok(analyze(
        &std::fs::read_to_string(input).map_err(|e| e.to_string())?,
        &dwarf,
        [&dumps[0], &dumps[1], &dumps[2]],
        &uids,
        &align_streams(&left, &right),
        stem,
    ))
}

fn analyze(
    source: &str,
    dwarf: &str,
    dumps: [&str; 3],
    uids: &[u32],
    pairs: &[(Option<String>, Option<String>)],
    stem: &str,
) -> Report {
    let (vars, hard) = (variables(dwarf), dispositions(dumps[2], stem));
    let (mut roles, mut stacks) = (Vec::new(), Vec::new());
    let (mut index, mut candidate, mut reference) = (0usize, String::new(), String::new());
    for (left, right) in pairs {
        let uid = left.as_ref().and_then(|_| {
            let value = uids.get(index).copied();
            index += 1;
            value
        });
        if left == right {
            continue;
        }
        candidate += left.as_deref().unwrap_or("");
        reference += right.as_deref().unwrap_or("");
        let (Some(left), Some(right)) = (left, right) else {
            continue;
        };
        let (actual, target) = (regs(left), regs(right));
        if let (Some(uid), Some(&actual), Some(&target)) = (uid, actual.first(), target.first()) {
            if actual != target {
                if let Some(pseudo) = pseudo_at(dumps[1], stem, uid)
                    .filter(|pseudo| hard.contains(&(*pseudo, actual)))
                {
                    roles.push((pseudo, actual, target));
                }
            }
        }
        if let (Some(a), Some(b)) = (stack_offset(left), stack_offset(right)) {
            if a != b {
                stacks.push((a, b));
            }
        }
    }
    let mut out = format!("allocator_order=constraint owner={stem}\n");
    roles.sort_unstable();
    roles.dedup();
    if roles
        .windows(2)
        .any(|window| window[0].0 == window[1].0 && window[0].2 != window[1].2)
    {
        return edit(out, "undecoded reason=conflicting-targets", None);
    }
    for &(pseudo, actual, target) in &roles {
        let name = unique(&vars, |v| v.1 == Some(actual)).map_or("expression", |v| &v.0);
        let rank = creation_rank(dumps[0], stem, pseudo).map_or("?".into(), |n| n.to_string());
        let life = dump(dumps[1], stem)
            .lines()
            .find(|line| {
                line.trim_start()
                    .starts_with(&format!("Register {pseudo} used "))
            })
            .map(str::trim)
            .unwrap_or("lifetime unavailable");
        out += &format!(
            "role={name} pseudo={pseudo} creation_rank={rank} actual=r{actual} target=r{target} lifetime={life}\n"
        );
    }
    if dwarf.contains("DW_OP_reg13") {
        if let Some((x, y)) = reciprocal(stacks.iter().copied()).and_then(|(a, b)| {
            Some((
                unique(&vars, |v| v.2 == Some(a))?,
                unique(&vars, |v| v.2 == Some(b))?,
            ))
        }) {
            return edit(
                out,
                &format!("swap_declarations({},{})", x.0, y.0),
                Some(Repair::SwapDeclarations {
                    left: x.0.clone(),
                    right: y.0.clone(),
                }),
            );
        }
    }
    if source.contains("do {") && source.contains("while") {
        if let Some(var) = unique(&vars, |v| {
            v.1.is_some_and(|reg| {
                source.matches(&v.0).count() >= 3
                    && candidate.matches(&format!("[r{reg}")).count() >= 2
            })
        }) {
            return edit(
                out,
                &format!(
                    "split_lifetime({}) detail=recreate-at-guard-and-backedge",
                    var.0
                ),
                Some(Repair::SplitLifetime {
                    name: var.0.clone(),
                }),
            );
        }
    }
    if candidate.contains("movs\tr3, #223") && reference.contains("subs\tr3, #33") {
        return edit(
            out,
            "merge_lifetime(zero_carrier) detail=keep-zero-live-through-clears-and-mask",
            Some(Repair::MergeZeroCarrier),
        );
    }
    if reciprocal(roles.iter().map(|&(_, a, b)| (a, b))).is_some() {
        out += "constraint=reciprocal-register-role-swap\n";
        let names = roles
            .iter()
            .filter_map(|&(_, actual, _)| unique(&vars, |var| var.1 == Some(actual)))
            .map(|var| var.0.clone())
            .collect::<std::collections::BTreeSet<_>>();
        let repair = (names.len() == 1).then(|| Repair::ReciprocalRoleSwap {
            name: names.into_iter().next().unwrap(),
        });
        return edit(out, "undecoded reason=no-unique-source-shape", repair);
    }
    edit(out, "undecoded reason=no-unique-source-shape", None)
}

fn variables(text: &str) -> Vec<Var> {
    text.split("Abbrev Number:")
        .filter(|entry| {
            entry.contains("DW_TAG_variable") || entry.contains("DW_TAG_formal_parameter")
        })
        .filter_map(|entry| {
            let name = attribute(entry, "DW_AT_name")?
                .rsplit_once(':')?
                .1
                .trim()
                .into();
            let location = attribute(entry, "DW_AT_location").unwrap_or("");
            Some((
                name,
                number(location, r"DW_OP_reg([0-9]+)"),
                number(location, r"DW_OP_fbreg: (-?[0-9]+)"),
            ))
        })
        .collect()
}

fn attribute<'a>(entry: &'a str, key: &str) -> Option<&'a str> {
    entry.lines().find(|line| line.contains(key))
}

fn asm_uids(text: &str, stem: &str) -> Vec<u32> {
    let Some((_, body)) = text.split_once(&format!("Func_{stem}:")) else {
        return Vec::new();
    };
    let mut last = None;
    body.lines()
        .map(str::trim)
        .take_while(|line| !line.starts_with(".size"))
        .filter_map(|line| {
            last = line
                .rsplit_once('@')
                .and_then(|(_, tail)| tail.split_whitespace().next()?.parse().ok())
                .or(last);
            line.chars()
                .next()
                .is_some_and(char::is_alphabetic)
                .then_some(last.unwrap_or(0))
        })
        .collect()
}

fn dump<'a>(text: &'a str, stem: &str) -> &'a str {
    text.split_once(&format!(";; Function Func_{stem}"))
        .map_or(text, |(_, body)| body)
        .split("\n;; Function ")
        .next()
        .unwrap_or(text)
}

fn pseudo_at(text: &str, stem: &str, uid: u32) -> Option<u32> {
    dump(text, stem).split("\n(").find_map(|record| {
        let head = ["insn ", "jump_insn ", "call_insn "]
            .iter()
            .find_map(|prefix| record.strip_prefix(prefix))?;
        (head.split_whitespace().next()?.parse::<u32>().ok()? == uid)
            .then(|| number(record, r"\(set \(reg(?:/v)?:[A-Z]+ ([0-9]+)"))
            .flatten()
    })
}

fn creation_rank(text: &str, stem: &str, pseudo: u32) -> Option<usize> {
    let regex = Regex::new(r"\(set \(reg(?:/v)?:[A-Z]+ ([0-9]+)").unwrap();
    let mut seen = std::collections::BTreeSet::new();
    let rank = regex
        .captures_iter(dump(text, stem))
        .filter_map(|capture| capture[1].parse::<u32>().ok())
        .filter(|value| *value >= 32 && seen.insert(*value))
        .position(|value| value == pseudo);
    rank
}

fn dispositions(text: &str, stem: &str) -> Vec<(u32, u8)> {
    let body = dump(text, stem)
        .split_once(";; Register dispositions:")
        .map_or("", |(_, body)| body)
        .split("\n\n")
        .next()
        .unwrap_or("");
    Regex::new(r"([0-9]+) in ([0-9]+)")
        .unwrap()
        .captures_iter(body)
        .filter_map(|capture| Some((capture[1].parse().ok()?, capture[2].parse().ok()?)))
        .collect()
}

fn regs(text: &str) -> Vec<u8> {
    Regex::new(r"\b(?:r(?:1[0-4]|[0-9])|sl|fp|ip|lr)\b")
        .unwrap()
        .find_iter(text)
        .filter_map(|value| match value.as_str() {
            "sl" => Some(10),
            "fp" => Some(11),
            "ip" => Some(12),
            "lr" => Some(14),
            value => value.strip_prefix('r')?.parse().ok(),
        })
        .collect()
}

fn number<T: std::str::FromStr>(text: &str, pattern: &str) -> Option<T> {
    Regex::new(pattern).ok()?.captures(text)?[1].parse().ok()
}
fn stack_offset(text: &str) -> Option<i32> {
    text.split_once("[sp, #")?.1.split_once(']')?.0.parse().ok()
}
fn unique(vars: &[Var], predicate: impl Fn(&Var) -> bool) -> Option<&Var> {
    let mut found = vars.iter().filter(|var| predicate(var));
    let value = found.next()?;
    found.next().is_none().then_some(value)
}
fn reciprocal<T: Ord + Copy>(pairs: impl IntoIterator<Item = (T, T)>) -> Option<(T, T)> {
    let pairs: std::collections::BTreeSet<_> = pairs.into_iter().collect();
    let mut found = pairs
        .iter()
        .filter(|&&(a, b)| a < b && pairs.contains(&(b, a)));
    let value = found.next().copied()?;
    found.next().is_none().then_some(value)
}
fn edit(mut out: String, value: &str, repair: Option<Repair>) -> Report {
    out += &format!("edit={value}\n");
    report(out, repair)
}
