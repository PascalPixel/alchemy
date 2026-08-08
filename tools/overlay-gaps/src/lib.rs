use std::collections::BTreeMap;

use overlay_call_targets::overlay_image;
use overlay_published::{
    exact_spans, is_prologue_shape, manual_regions, overlay_names, saves_link_register,
};

pub const ALIGNMENT_SLACK: i64 = 2;

#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Gap {
    pub start: i64,
    pub end: i64,
    pub after: i64,
    pub before: i64,
    pub bytes: i64,
    pub returns: Vec<i64>,
    pub padding: bool,
    pub verdict: &'static str,
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Tail {
    pub start: i64,
    pub end: i64,
    pub bytes: i64,
    pub veneers: i64,
    pub call_via: i64,
    pub prologues: Vec<i64>,
    pub returns: Vec<i64>,
    pub verdict: &'static str,
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Overlap {
    pub owner: i64,
    pub next: i64,
    pub bytes: i64,
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Duplicate {
    pub start: i64,
    pub recorded: i64,
    pub compiled: i64,
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub struct SpansResult {
    pub spans: Vec<(i64, i64)>,
    pub duplicates: Vec<Duplicate>,
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub struct GapReport {
    pub gaps: Vec<Gap>,
    pub overlaps: Vec<Overlap>,
    pub duplicates: Vec<Duplicate>,
    pub owners: i64,
    pub tail: Tail,
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub struct RankedGap {
    pub overlay: String,
    pub gap: Gap,
}

pub fn rank_gaps(mut items: Vec<RankedGap>) -> Vec<RankedGap> {
    items.sort_by(|left, right| {
        let lt = if left.gap.returns.len() == 1 { 0 } else { 1 };
        let rt = if right.gap.returns.len() == 1 { 0 } else { 1 };
        lt.cmp(&rt)
            .then_with(|| right.gap.bytes.cmp(&left.gap.bytes))
            .then_with(|| left.overlay.cmp(&right.overlay))
            .then_with(|| left.gap.start.cmp(&right.gap.start))
    });
    items
}

pub fn is_return_shape(halfword: u16) -> bool {
    (halfword & 0xff87) == 0x4700 || (halfword & 0xff00) == 0xbd00
}

fn u16le(image: &[u8], at: i64) -> u16 {
    if at < 0 || at + 1 >= image.len() as i64 {
        return 0;
    }
    let at = at as usize;
    image[at] as u16 | ((image[at + 1] as u16) << 8)
}

pub fn mask_banks(image: &[u8], from: i64, to: i64) -> (Vec<bool>, i64, i64) {
    let from = from.max(0).min(image.len() as i64);
    let to = to.max(from).min(image.len() as i64);
    let mut covered = vec![false; (to - from) as usize];
    let mut veneers = 0;
    let mut call_via = 0;
    let aligned = from + ((4 - from.rem_euclid(4)) % 4);
    let mut at = aligned;
    while at + 8 <= to {
        let first = u16le(image, at);
        let second = u16le(image, at + 2);
        if (first & 0xf800) == 0x4800
            && (second & 0xff87) == 0x4700
            && ((second >> 3) & 0xf) == ((first >> 8) & 7)
        {
            veneers += 1;
            for k in at..(at + 8).min(to) {
                covered[(k - from) as usize] = true;
            }
        }
        at += 4;
    }
    at = from;
    while at + 4 <= to {
        if !covered[(at - from) as usize]
            && (u16le(image, at) & 0xff87) == 0x4700
            && u16le(image, at + 2) == 0x46c0
        {
            call_via += 1;
            for k in at..(at + 4).min(to) {
                covered[(k - from) as usize] = true;
            }
        }
        at += 2;
    }
    (covered, veneers, call_via)
}

pub fn owner_spans(overlay: &str) -> Result<SpansResult, String> {
    let mut by_start = BTreeMap::<i64, i64>::new();
    for span in manual_regions(overlay) {
        if span.end > span.start {
            by_start
                .entry(span.start)
                .and_modify(|end| *end = (*end).max(span.end))
                .or_insert(span.end);
        }
    }
    let mut duplicates = Vec::new();
    for span in exact_spans(overlay) {
        if span.end <= span.start {
            continue;
        }
        if let Some(recorded_end) = by_start.get(&span.start).copied() {
            if recorded_end != span.end {
                duplicates.push(Duplicate {
                    start: span.start,
                    recorded: recorded_end - span.start,
                    compiled: span.end - span.start,
                });
            }
        }
        by_start
            .entry(span.start)
            .and_modify(|end| *end = (*end).max(span.end))
            .or_insert(span.end);
    }
    Ok(SpansResult {
        spans: by_start
            .into_iter()
            .map(|(start, end)| (start, end))
            .collect(),
        duplicates,
    })
}

pub fn gaps_between(image: &[u8], spans: &[(i64, i64)]) -> (Vec<Gap>, Vec<Overlap>) {
    let mut gaps = Vec::new();
    let mut overlaps = Vec::new();
    let mut rule = |start: i64, end: i64, after: i64, before: i64| {
        let (covered, _, _) = mask_banks(image, start, end);
        let mut padding = true;
        for at in start.max(0)..end.min(image.len() as i64) {
            if image[at as usize] != 0 {
                padding = false;
                break;
            }
        }
        let mut returns = Vec::new();
        let mut at = start;
        while at + 1 < end {
            if at >= 0
                && at - start >= 0
                && (at - start) < covered.len() as i64
                && !covered[(at - start) as usize]
                && is_return_shape(u16le(image, at))
            {
                returns.push(at);
            }
            at += 2;
        }
        let verdict = if padding {
            "PADDING"
        } else if !returns.is_empty() {
            "CODE-SUSPECT"
        } else {
            "POOL-OR-DATA"
        };
        gaps.push(Gap {
            start,
            end,
            after,
            before,
            bytes: end - start,
            returns,
            padding,
            verdict,
        });
    };

    if let Some(&(first, _)) = spans.first() {
        if first > ALIGNMENT_SLACK {
            rule(0, first, -1, first);
        }
    }
    for pair in spans.windows(2) {
        let (current_start, current_end) = pair[0];
        let (next_start, _) = pair[1];
        let size = next_start - current_end;
        if size < 0 {
            overlaps.push(Overlap {
                owner: current_start,
                next: next_start,
                bytes: -size,
            });
            continue;
        }
        let padding =
            (current_end..next_start).all(|at| image.get(at as usize).copied().unwrap_or(0) == 0);
        if size <= ALIGNMENT_SLACK && padding {
            continue;
        }
        rule(current_end, next_start, current_start, next_start);
    }
    (gaps, overlaps)
}

pub fn rule_tail(image: &[u8], from: i64) -> Tail {
    let from = from.max(0).min(image.len() as i64);
    let end = image.len() as i64;
    if from >= end {
        return Tail {
            start: from,
            end,
            bytes: 0,
            veneers: 0,
            call_via: 0,
            prologues: vec![],
            returns: vec![],
            verdict: "EMPTY",
        };
    }
    let (covered, veneers, call_via) = mask_banks(image, from, end);
    let mut prologues = Vec::new();
    let mut returns = Vec::new();
    let mut at = from;
    while at + 1 < end {
        if !covered[(at - from) as usize] {
            let halfword = u16le(image, at);
            if is_return_shape(halfword) {
                returns.push(at);
            }
            if is_prologue_shape(halfword) && saves_link_register(halfword) {
                prologues.push(at);
            }
        }
        at += 2;
    }
    let verdict = if !prologues.is_empty() {
        "PROLOGUE-SUSPECT"
    } else if !returns.is_empty() {
        "RETURN-SUSPECT"
    } else {
        "VENEER-AND-DATA"
    };
    Tail {
        start: from,
        end,
        bytes: end - from,
        veneers,
        call_via,
        prologues,
        returns,
        verdict,
    }
}

pub fn gaps_of(overlay: &str) -> Result<GapReport, String> {
    let image = overlay_image(overlay)?;
    let spans = owner_spans(overlay)?;
    let (gaps, overlaps) = gaps_between(&image, &spans.spans);
    let last = spans.spans.last().map(|(_, end)| *end).unwrap_or(0);
    Ok(GapReport {
        gaps,
        overlaps,
        duplicates: spans.duplicates,
        owners: spans.spans.len() as i64,
        tail: rule_tail(&image, last),
    })
}

pub fn self_test() -> Result<(), String> {
    if !is_return_shape(0x4770)
        || !is_return_shape(0x4700)
        || !is_return_shape(0xbd78)
        || is_return_shape(0xb5e0)
        || is_return_shape(0)
    {
        return Err("sweep D self-test: return shapes".into());
    }
    let image = vec![0u8; 0x40];
    let (_, overlaps) = gaps_between(&image, &[(0, 0x18), (0x10, 0x20)]);
    if overlaps.len() != 1 || overlaps[0].bytes != 8 {
        return Err("sweep D self-test: over-measure not caught".into());
    }
    let mut leaf = vec![0u8; 0x40];
    let words = [
        0x4b03u16, 0x21bf, 0x681b, 0x0049, 0x185a, 0x4b02, 0x8013, 0x4770,
    ];
    for (i, word) in words.iter().enumerate() {
        leaf[0x10 + i * 2] = *word as u8;
        leaf[0x11 + i * 2] = (*word >> 8) as u8;
    }
    let (gaps, _) = gaps_between(&leaf, &[(0, 0x10), (0x28, 0x40)]);
    if gaps.len() != 1 || gaps[0].verdict != "CODE-SUSPECT" || gaps[0].returns != vec![0x1e] {
        return Err("sweep D self-test: leaf gap".into());
    }
    let mut tail = vec![0u8; 0x20];
    for at in [0x10, 0x18] {
        tail[at] = 0;
        tail[at + 1] = 0x48;
        tail[at + 2] = 0;
        tail[at + 3] = 0x47;
    }
    let clean = rule_tail(&tail, 0x10);
    if clean.veneers != 2 || clean.verdict != "VENEER-AND-DATA" {
        return Err("sweep D self-test: veneer tail".into());
    }
    let mut getter = vec![0u8; 8];
    getter[0] = 0;
    getter[1] = 0x48;
    getter[2] = 0x70;
    getter[3] = 0x47;
    if mask_banks(&getter, 0, 8).1 != 0 {
        return Err("sweep D self-test: getter is not a veneer".into());
    }
    println!("sweep D self-test passed (return shapes, leaf, undercount, over-measure, tail ruling,\n  tail leaf, call_via/interior-bank mask, head region, empty-sweep refusal)");
    Ok(())
}

fn json_escape(s: &str) -> String {
    format!("\"{}\"", s.replace('\\', "\\\\").replace('"', "\\\""))
}
fn int_array(values: &[i64], level: usize) -> String {
    if values.is_empty() {
        return "[]".into();
    }
    let pad = "  ".repeat(level);
    let item = "  ".repeat(level + 1);
    format!(
        "[\n{}\n{}]",
        values
            .iter()
            .map(|v| format!("{item}{v}"))
            .collect::<Vec<_>>()
            .join(",\n"),
        pad
    )
}
fn gap_json(gap: &Gap, level: usize) -> String {
    let p = "  ".repeat(level);
    let f = "  ".repeat(level + 1);
    format!("{p}{{\n{f}\"start\": {},\n{f}\"end\": {},\n{f}\"after\": {},\n{f}\"before\": {},\n{f}\"bytes\": {},\n{f}\"returns\": {},\n{f}\"padding\": {},\n{f}\"verdict\": {}\n{p}}}", gap.start, gap.end, gap.after, gap.before, gap.bytes, int_array(&gap.returns, level + 1), gap.padding, json_escape(gap.verdict))
}
fn tail_json(tail: &Tail, level: usize) -> String {
    let p = "  ".repeat(level);
    let f = "  ".repeat(level + 1);
    format!("{p}{{\n{f}\"start\": {},\n{f}\"end\": {},\n{f}\"bytes\": {},\n{f}\"veneers\": {},\n{f}\"callVia\": {},\n{f}\"prologues\": {},\n{f}\"returns\": {},\n{f}\"verdict\": {}\n{p}}}", tail.start, tail.end, tail.bytes, tail.veneers, tail.call_via, int_array(&tail.prologues, level + 1), int_array(&tail.returns, level + 1), json_escape(tail.verdict))
}
fn report_json(report: &GapReport, level: usize) -> String {
    let p = "  ".repeat(level);
    let f = "  ".repeat(level + 1);
    let gaps = if report.gaps.is_empty() {
        "[]".into()
    } else {
        format!(
            "[\n{}\n{}]",
            report
                .gaps
                .iter()
                .map(|g| gap_json(g, level + 2))
                .collect::<Vec<_>>()
                .join(",\n"),
            "  ".repeat(level + 1)
        )
    };
    let overlaps = if report.overlaps.is_empty() {
        "[]".into()
    } else {
        format!(
            "[\n{}\n{}]",
            report
                .overlaps
                .iter()
                .map(|o| format!(
                    "{}{{\"owner\": {}, \"next\": {}, \"bytes\": {}}}",
                    "  ".repeat(level + 2),
                    o.owner,
                    o.next,
                    o.bytes
                ))
                .collect::<Vec<_>>()
                .join(",\n"),
            "  ".repeat(level + 1)
        )
    };
    let duplicates = if report.duplicates.is_empty() {
        "[]".into()
    } else {
        format!(
            "[\n{}\n{}]",
            report
                .duplicates
                .iter()
                .map(|d| format!(
                    "{}{{\"start\": {}, \"recorded\": {}, \"compiled\": {}}}",
                    "  ".repeat(level + 2),
                    d.start,
                    d.recorded,
                    d.compiled
                ))
                .collect::<Vec<_>>()
                .join(",\n"),
            "  ".repeat(level + 1)
        )
    };
    format!("{p}{{\n{f}\"gaps\": {gaps},\n{f}\"overlaps\": {overlaps},\n{f}\"duplicates\": {duplicates},\n{f}\"owners\": {},\n{f}\"tail\": {}\n{p}}}", report.owners, tail_json(&report.tail, level + 1))
}

pub fn reports_json(reports: &[(String, GapReport)]) -> String {
    let body = reports
        .iter()
        .map(|(name, report)| format!("  {}: {}", json_escape(name), report_json(report, 1)))
        .collect::<Vec<_>>()
        .join(",\n");
    format!("{{\n{body}\n}}")
}

pub fn run(args: &[String]) -> Result<(), String> {
    if args.iter().any(|a| a == "--self-test") {
        return self_test();
    }
    let json = args.iter().any(|a| a == "--json");
    let ranked = args.iter().any(|a| a == "--ranked");
    if json && ranked {
        return Err("choose either --json or --ranked".into());
    }
    let names = args
        .iter()
        .filter(|a| !a.starts_with("--"))
        .cloned()
        .collect::<Vec<_>>();
    let overlays = if names.is_empty() {
        overlay_names()?
    } else {
        names
    };
    let mut reports = Vec::new();
    for overlay in overlays {
        if let Ok(report) = gaps_of(&overlay) {
            reports.push((overlay, report));
        }
    }
    let mut suspects = 0;
    let mut overlapping = 0;
    let mut tails = 0;
    let mut return_tails = 0;
    for (_, report) in &reports {
        suspects += report
            .gaps
            .iter()
            .filter(|g| g.verdict == "CODE-SUSPECT")
            .count();
        overlapping += report.overlaps.len();
        if report.tail.verdict == "PROLOGUE-SUSPECT" {
            tails += 1;
        }
        if report.tail.verdict == "RETURN-SUSPECT" {
            return_tails += 1;
        }
    }
    if json {
        if reports.is_empty() {
            std::process::exit(1);
        }
        println!("{}", reports_json(&reports));
        return Ok(());
    }
    if ranked {
        let candidates = rank_gaps(
            reports
                .iter()
                .flat_map(|(name, report)| {
                    report
                        .gaps
                        .iter()
                        .filter(|g| g.verdict == "CODE-SUSPECT")
                        .map(|g| RankedGap {
                            overlay: name.clone(),
                            gap: g.clone(),
                        })
                })
                .collect(),
        );
        for item in &candidates {
            let shape = if item.gap.returns.len() == 1 {
                "ONE-RETURN".into()
            } else {
                format!("{}-RETURN", item.gap.returns.len())
            };
            println!(
                "{:>6}B  {:<10}  {} 0x{:x}-0x{:x}",
                item.gap.bytes, shape, item.overlay, item.gap.start, item.gap.end
            );
        }
        println!("\nranked_code_suspect_gaps={}", candidates.len());
    } else {
        for (overlay, report) in &reports {
            let interesting = report
                .gaps
                .iter()
                .filter(|g| g.verdict == "CODE-SUSPECT")
                .collect::<Vec<_>>();
            if interesting.is_empty()
                && report.overlaps.is_empty()
                && report.duplicates.is_empty()
                && !["PROLOGUE-SUSPECT", "RETURN-SUSPECT"].contains(&report.tail.verdict)
            {
                continue;
            }
            println!("{overlay}");
            if ["PROLOGUE-SUSPECT", "RETURN-SUSPECT"].contains(&report.tail.verdict) {
                println!(
                    "  TAIL         0x{:x}-0x{:x} {}B, {} veneers, {} call_via",
                    report.tail.start,
                    report.tail.end - 1,
                    report.tail.bytes,
                    report.tail.veneers,
                    report.tail.call_via
                );
                if !report.tail.prologues.is_empty() {
                    println!(
                        "               {} lr-saving prologues at {}",
                        report.tail.prologues.len(),
                        report
                            .tail
                            .prologues
                            .iter()
                            .take(8)
                            .map(|x| format!("0x{x:x}"))
                            .collect::<Vec<_>>()
                            .join(" ")
                    );
                }
                if !report.tail.returns.is_empty() {
                    println!(
                        "               {} returns outside the banks at {}",
                        report.tail.returns.len(),
                        report
                            .tail
                            .returns
                            .iter()
                            .take(8)
                            .map(|x| format!("0x{x:x}"))
                            .collect::<Vec<_>>()
                            .join(" ")
                    );
                }
            }
            for overlap in &report.overlaps {
                println!(
                    "  OVERLAP      0x{:x} runs {} bytes past 0x{:x}",
                    overlap.owner, overlap.bytes, overlap.next
                );
            }
            for duplicate in &report.duplicates {
                println!(
                    "  STALE-SPAN   0x{:x} recorded {}B, compiles to {}B",
                    duplicate.start, duplicate.recorded, duplicate.compiled
                );
            }
            for gap in interesting {
                println!(
                    "  CODE-SUSPECT 0x{:x}-0x{:x} {}B after 0x{:x}, returns at {}",
                    gap.start,
                    gap.end - 1,
                    gap.bytes,
                    gap.after,
                    gap.returns
                        .iter()
                        .map(|x| format!("0x{x:x}"))
                        .collect::<Vec<_>>()
                        .join(" ")
                );
            }
        }
    }
    println!("\noverlays={} code_suspect_gaps={} overlaps={} prologue_suspect_tails={} return_suspect_tails={}", reports.len(), suspects, overlapping, tails, return_tails);
    if reports.is_empty() {
        println!("NOTHING SWEPT — this is a FAILURE, not a pass.\n  No code overlay matched. Check the name against assets/code/*_overlay.s.");
        std::process::exit(1);
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn synthetic_gap_and_mask_behaviour() {
        self_test().unwrap();
    }
    #[test]
    fn ranking_prefers_one_return_then_bytes() {
        let make = |name: &str, bytes: i64, returns: Vec<i64>| RankedGap {
            overlay: name.into(),
            gap: Gap {
                start: 0,
                end: bytes,
                after: 0,
                before: bytes,
                bytes,
                returns,
                padding: false,
                verdict: "CODE-SUSPECT",
            },
        };
        let result = rank_gaps(vec![
            make("resource_b", 256, vec![1, 2]),
            make("resource_c", 128, vec![1]),
            make("resource_a", 192, vec![1]),
        ]);
        assert_eq!(
            result.into_iter().map(|x| x.overlay).collect::<Vec<_>>(),
            vec!["resource_a", "resource_c", "resource_b"]
        );
    }
}
