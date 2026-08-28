use crate::{cli::Options, render::align_streams};
use candidate_compiler::verify::{assemble, compile_source, copy_text, run};
use regex::Regex;
use std::path::Path;

type Var = (String, Option<u8>, Option<i32>);

#[derive(Clone, Debug)]
pub struct Report {
    pub text: String,
    pub dimensions: Vec<&'static str>,
    pub repair: Option<RepairPlan>,
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub enum Repair {
    SwapDeclarations {
        left: String,
        right: String,
    },
    SplitLifetime {
        name: String,
    },
    MergeZeroCarrier,
    ReciprocalRoleSwap {
        name: String,
    },
    PreloadAdjacentHalfwords {
        first_destination: String,
        first_source: String,
        second_destination: String,
        second_source: String,
        carrier: String,
    },
    MaterializeMessageAndMergeCount {
        indexed_value: String,
        message: String,
        coordinate: String,
        count: String,
    },
    SplitOppositeSideAndScaledOffset {
        side: String,
        opposite: String,
    },
    MergeCarrierPhases {
        earlier: String,
        later: String,
    },
    /// Structural repair: the reference's comparison branches are the
    /// mirror of the candidate's, so some relational guard is spelled with
    /// its operands in the other order. The permuter enumerates one mirror
    /// per relational guard site; the byte score selects.
    MirrorRelationalGuards,
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct RepairPlan {
    repairs: Vec<Repair>,
}

impl From<Repair> for RepairPlan {
    fn from(repair: Repair) -> Self {
        Self::one(repair)
    }
}

impl RepairPlan {
    pub fn one(repair: Repair) -> Self {
        Self {
            repairs: vec![repair],
        }
    }

    pub fn two(first: Repair, second: Repair) -> Self {
        Self {
            repairs: vec![first, second],
        }
    }

    pub fn try_from_repairs(repairs: Vec<Repair>) -> Result<Self, String> {
        if !(1..=2).contains(&repairs.len()) {
            return Err(format!(
                "allocator repair plan must name one or two repairs, got {}",
                repairs.len()
            ));
        }
        Ok(Self { repairs })
    }

    pub fn repairs(&self) -> &[Repair] {
        &self.repairs
    }

    pub fn dimensions(&self) -> Vec<&'static str> {
        let mut dimensions = Vec::new();
        for repair in &self.repairs {
            for dimension in repair.dimensions() {
                if !dimensions.contains(dimension) {
                    dimensions.push(*dimension);
                }
            }
        }
        dimensions
    }

    pub fn label(&self) -> String {
        if self.repairs.len() == 1 {
            self.repairs[0].label()
        } else {
            format!(
                "compose({})",
                self.repairs
                    .iter()
                    .map(Repair::label)
                    .collect::<Vec<_>>()
                    .join(",")
            )
        }
    }
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
            Self::PreloadAdjacentHalfwords { .. } => {
                &["temporary", "evaluation_order", "type_width"]
            }
            Self::MaterializeMessageAndMergeCount { .. } => {
                &["temporary", "evaluation_order", "block_lifetime"]
            }
            Self::SplitOppositeSideAndScaledOffset { .. } => &["temporary", "evaluation_order"],
            Self::MergeCarrierPhases { .. } => &["temporary", "block_lifetime"],
            Self::MirrorRelationalGuards => &["evaluation_order", "commutative_order"],
        }
    }
    pub fn label(&self) -> String {
        match self {
            Self::SwapDeclarations { left, right } => format!("swap_declarations({left},{right})"),
            Self::SplitLifetime { name } => format!("split_lifetime({name})"),
            Self::MergeZeroCarrier => "merge_lifetime(zero_carrier)".into(),
            Self::ReciprocalRoleSwap { name } => format!("reciprocal_register_role_swap({name})"),
            Self::PreloadAdjacentHalfwords {
                first_destination,
                first_source,
                second_destination,
                second_source,
                carrier,
            } => format!(
                "preload_adjacent_halfwords({first_destination},{first_source},{second_destination},{second_source},{carrier})"
            ),
            Self::MaterializeMessageAndMergeCount {
                indexed_value,
                message,
                coordinate,
                count,
            } => format!(
                "materialize_message_and_merge_count({indexed_value},{message},{coordinate},{count})"
            ),
            Self::SplitOppositeSideAndScaledOffset { side, opposite } => {
                format!("split_opposite_side_and_scaled_offset({side},{opposite})")
            }
            Self::MergeCarrierPhases { earlier, later } => {
                format!("merge_carrier_phases({earlier},{later})")
            }
            Self::MirrorRelationalGuards => "mirror_relational_guards".into(),
        }
    }
}

impl Report {
    fn new(text: impl Into<String>, repair: Option<RepairPlan>) -> Self {
        let dimensions = repair
            .as_ref()
            .map_or_else(Vec::new, RepairPlan::dimensions);
        Self {
            text: text.into(),
            dimensions,
            repair,
        }
    }

    fn undecoded(reason: &str) -> Self {
        Self::new(format!("allocator_order=undecoded reason={reason}\n"), None)
    }

    fn edit(mut text: String, value: &str, repair: Option<RepairPlan>) -> Self {
        text += &format!("edit={value}\n");
        Self::new(text, repair)
    }
}

fn text_section(object: &Path, output: &Path) -> Result<Vec<u8>, String> {
    copy_text(&object.to_string_lossy(), &output.to_string_lossy())?;
    std::fs::read(output).map_err(|e| e.to_string())
}

pub(crate) fn decode(
    root: &Path,
    options: &Options,
    routing: &Path,
    stem: &str,
    source: &str,
    work: &Path,
    candidate: &[String],
    reference: &[String],
) -> Result<Report, String> {
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
    let normal_object = options
        .precompiled_object
        .as_deref()
        .map(Path::new)
        .map(Path::to_path_buf)
        .unwrap_or_else(|| work.join(format!("{stem}.o")));
    if text_section(&normal_object, &dir.join("normal.text"))?
        != text_section(&object, &dir.join("diagnostic.text"))?
    {
        return Ok(Report::undecoded("debug-text-drift"));
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
        return Ok(Report::undecoded("missing-owner-uids"));
    }
    let (mut left, mut right) = (candidate.to_vec(), reference.to_vec());
    if left.len() < uids.len() || right.len() < uids.len() {
        return Ok(Report::undecoded("instruction-stream-shape"));
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
        return Report::edit(out, "undecoded reason=conflicting-targets", None);
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
    if let Some(plan) = paired_phase_plan(source, &vars, &roles) {
        let label = plan.label();
        return Report::edit(
            out,
            &format!("{label} detail=two-nonoverlapping-register-phases"),
            Some(plan),
        );
    }
    if dwarf.contains("DW_OP_reg13") {
        if let Some((x, y)) = reciprocal(stacks.iter().copied()).and_then(|(a, b)| {
            Some((
                unique(&vars, |v| v.2 == Some(a))?,
                unique(&vars, |v| v.2 == Some(b))?,
            ))
        }) {
            return Report::edit(
                out,
                &format!("swap_declarations({},{})", x.0, y.0),
                Some(
                    Repair::SwapDeclarations {
                        left: x.0.clone(),
                        right: y.0.clone(),
                    }
                    .into(),
                ),
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
            return Report::edit(
                out,
                &format!(
                    "split_lifetime({}) detail=recreate-at-guard-and-backedge",
                    var.0
                ),
                Some(
                    Repair::SplitLifetime {
                        name: var.0.clone(),
                    }
                    .into(),
                ),
            );
        }
    }
    if candidate.contains("movs\tr3, #223") && reference.contains("subs\tr3, #33") {
        return Report::edit(
            out,
            "merge_lifetime(zero_carrier) detail=keep-zero-live-through-clears-and-mask",
            Some(Repair::MergeZeroCarrier.into()),
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
        return Report::edit(
            out,
            "undecoded reason=no-unique-source-shape",
            repair.map(Into::into),
        );
    }
    Report::edit(out, "undecoded reason=no-unique-source-shape", None)
}

fn word_positions(source: &str, name: &str) -> Vec<usize> {
    Regex::new(&format!(r"\b{}\b", regex::escape(name)))
        .unwrap()
        .find_iter(source)
        .map(|value| value.start())
        .collect()
}

fn first_write(source: &str, name: &str) -> Option<usize> {
    Regex::new(&format!(r"\b{}\b\s*(?:=|--|\+\+)", regex::escape(name)))
        .ok()?
        .find(source)
        .map(|value| value.start())
}

fn scalar_names(source: &str) -> Vec<String> {
    let mut names = Regex::new(r"\bs32\s+([A-Za-z_][A-Za-z0-9_]*)\b")
        .unwrap()
        .captures_iter(source)
        .map(|capture| capture[1].to_string())
        .collect::<Vec<_>>();
    names.sort();
    names.dedup();
    names
}

/// A reciprocal allocation floor can expose only the first displaced value,
/// while the reference register trace shows two complete, non-overlapping
/// carrier phases. Name both merges only when the debug allocation identifies
/// the later occupant of the target register and the adjacent callee-saved
/// register has exactly one nearest later scalar phase. The permuter repeats
/// the lifetime and source-shape guards before it emits any candidate.
fn paired_phase_plan(source: &str, vars: &[Var], roles: &[(u32, u8, u8)]) -> Option<RepairPlan> {
    let (_, actual, target) = *roles.first()?;
    if roles
        .iter()
        .any(|&(_, left, right)| left != actual || right != target)
        || target < 5
    {
        return None;
    }
    let earlier = unique(vars, |var| var.1 == Some(actual))?.0.clone();
    let earlier_end = word_positions(source, &earlier).last().copied()?;
    let mut later_candidates = vars
        .iter()
        .filter(|var| var.1 == Some(target))
        .filter_map(|var| {
            first_write(source, &var.0)
                .filter(|write| *write > earlier_end)
                .map(|write| (write, var.0.clone()))
        })
        .collect::<Vec<_>>();
    later_candidates.sort();
    let (later_write, later) = later_candidates.first()?.clone();
    if later_candidates
        .get(1)
        .is_some_and(|next| next.0 == later_write)
    {
        return None;
    }

    let companion_register = target.checked_sub(1)?;
    let mut companion_candidates = vars
        .iter()
        .filter(|var| var.1 == Some(companion_register))
        .filter_map(|var| {
            first_write(source, &var.0)
                .filter(|write| *write < earlier_end)
                .map(|write| (write, var.0.clone()))
        })
        .collect::<Vec<_>>();
    companion_candidates.sort();
    let companion = companion_candidates.first()?.1.clone();
    let companion_end = word_positions(source, &companion).last().copied()?;
    let excluded = [&earlier, &later, &companion];
    let mut following = scalar_names(source)
        .into_iter()
        .filter(|name| !excluded.contains(&name))
        .filter_map(|name| {
            first_write(source, &name)
                .filter(|write| *write > companion_end)
                .map(|write| (write, name))
        })
        .collect::<Vec<_>>();
    following.sort();
    let (write, companion_later) = following.first()?.clone();
    if following.get(1).is_some_and(|next| next.0 == write) {
        return None;
    }
    Some(RepairPlan::two(
        Repair::MergeCarrierPhases {
            earlier: companion,
            later: companion_later,
        },
        Repair::MergeCarrierPhases { earlier, later },
    ))
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
