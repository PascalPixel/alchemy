//! GCC 2.96's post-reload list scheduler decides the order of the argument
//! loads before each call. Within a block every load is ready at once and
//! `rank_for_schedule` picks by priority (longest path to the block end),
//! then by the number of forward dependents, then by original order. A
//! load of rK gains a dependent for every later call that clobbers rK and
//! for the next set of rK; a call that returns a value sets r0 instead of
//! clobbering it, which ends r0's chain. That single bit per call is what
//! this module infers from the reference bytes.

use crate::decode::{Ins, Kind, Offset};
use std::collections::BTreeSet;

/// One instruction that sets an argument register before a call.
#[derive(Debug, Clone)]
struct Load {
    reg: u8,
    /// Instruction index in the decoded window.
    at: usize,
    /// Position in the chain that builds this register (0 = first).
    link: usize,
    /// Chain length.
    chain: usize,
    /// A constant the compiler precomputes into a pseudo for a direct call:
    /// anything beyond a single `movs`.
    costly: bool,
    /// An earlier instruction that still reads the register's old value:
    /// the load cannot pass it, and loses the tie right after it.
    blocked: bool,
}

#[derive(Debug, Clone)]
struct Site {
    /// Instruction index of the `bl`.
    at: usize,
    loads: Vec<Load>,
    /// Registers written anywhere between the previous call and this one.
    sets: BTreeSet<u8>,
}

/// The register written by an instruction that only builds a value.
fn written(kind: &Kind) -> Option<u8> {
    match *kind {
        Kind::MovImm { rd, .. }
        | Kind::LdrPool { rd, .. }
        | Kind::Movs { rd, .. }
        | Kind::MovHi { rd, .. }
        | Kind::AddImm3 { rd, .. }
        | Kind::AddImm8 { rd, .. }
        | Kind::AddReg { rd, .. }
        | Kind::AddHi { rd, .. }
        | Kind::SubImm3 { rd, .. }
        | Kind::SubImm8 { rd, .. }
        | Kind::SubReg { rd, .. }
        | Kind::ShiftImm { rd, .. }
        | Kind::Alu { rd, .. }
        | Kind::LdrSp { rd, .. }
        | Kind::AddSp { rd, .. }
        | Kind::AddPc { rd, .. }
        | Kind::Load { rd, .. } => (rd <= 3).then_some(rd),
        _ => None,
    }
}

fn reads_only_self(kind: &Kind, rd: u8) -> bool {
    match *kind {
        Kind::ShiftImm { rm, .. } => rm == rd,
        Kind::AddImm8 { .. } | Kind::SubImm8 { .. } => true,
        Kind::AddImm3 { rn, .. } | Kind::SubImm3 { rn, .. } => rn == rd,
        Kind::Alu { rm, .. } => rm != rd,
        Kind::MovImm { .. } | Kind::LdrPool { .. } => false,
        _ => false,
    }
}

/// Registers an instruction reads.
pub(crate) fn reads(kind: &Kind) -> Vec<u8> {
    match *kind {
        Kind::Store { rd, rn, offset, .. } => {
            let mut r = vec![rd, rn];
            if let Offset::Reg(rm) = offset {
                r.push(rm);
            }
            r
        }
        Kind::Load { rn, offset, .. } => {
            let mut r = vec![rn];
            if let Offset::Reg(rm) = offset {
                r.push(rm);
            }
            r
        }
        Kind::StrSp { rd, .. } => vec![rd],
        Kind::CmpImm { rn, .. } => vec![rn],
        Kind::CmpReg { rn, rm } => vec![rn, rm],
        Kind::Movs { rm, .. } | Kind::MovHi { rm, .. } | Kind::ShiftImm { rm, .. } => vec![rm],
        Kind::AddImm3 { rn, .. } | Kind::SubImm3 { rn, .. } => vec![rn],
        Kind::AddReg { rn, rm, .. } | Kind::SubReg { rn, rm, .. } => vec![rn, rm],
        Kind::AddHi { rd, rm } => vec![rd, rm],
        Kind::Alu { rd, rm, .. } => vec![rd, rm],
        Kind::AddImm8 { rd, .. } | Kind::SubImm8 { rd, .. } => vec![rd],
        Kind::Stmia { rn, list } => {
            let mut r = vec![rn];
            r.extend((0..8u8).filter(|b| list & (1 << b) != 0));
            r
        }
        Kind::Ldmia { rn, .. } => vec![rn],
        _ => vec![],
    }
}

fn is_block_end(kind: &Kind) -> bool {
    matches!(
        kind,
        Kind::B { .. } | Kind::Bcond { .. } | Kind::Bx(_) | Kind::Pop { pc: true, .. }
    )
}

fn uses_r0(kind: &Kind) -> bool {
    match *kind {
        Kind::Store { rd, rn, offset, .. } => {
            rd == 0 || rn == 0 || matches!(offset, Offset::Reg(0))
        }
        Kind::StrSp { rd, .. } => rd == 0,
        Kind::CmpImm { rn, .. } => rn == 0,
        Kind::CmpReg { rn, rm } => rn == 0 || rm == 0,
        Kind::Movs { rm, .. } | Kind::MovHi { rm, .. } | Kind::ShiftImm { rm, .. } => rm == 0,
        Kind::AddImm3 { rn, .. } | Kind::SubImm3 { rn, .. } => rn == 0,
        Kind::AddReg { rn, rm, .. } | Kind::SubReg { rn, rm, .. } => rn == 0 || rm == 0,
        Kind::AddHi { rd, rm } => rd == 0 || rm == 0,
        Kind::Alu { rd, rm, .. } => rd == 0 || rm == 0,
        Kind::AddImm8 { rd, .. } | Kind::SubImm8 { rd, .. } => rd == 0,
        Kind::Load { rn, offset, .. } => rn == 0 || matches!(offset, Offset::Reg(0)),
        _ => false,
    }
}

/// Splits one basic block into call sites with their argument loads.
#[allow(clippy::needless_range_loop)]
fn sites_in(ins: &[Ins], start: usize, end: usize, targets: &BTreeSet<u32>) -> Vec<Site> {
    let mut sites = Vec::new();
    let mut pending: Vec<Load> = Vec::new();
    let mut sets = BTreeSet::new();
    for i in start..end {
        if i > start && targets.contains(&ins[i].addr) {
            break;
        }
        let kind = &ins[i].kind;
        if let Kind::B { target } = kind {
            if ins.get(i + 1).is_some_and(|next| next.addr == *target) {
                continue;
            }
        }
        if let Kind::Bl { .. } = kind {
            // A register read by anything but its own chain before the call
            // is a temporary, not an argument load.
            let mut loads = std::mem::take(&mut pending);
            let temporaries: Vec<u8> = loads
                .iter()
                .filter(|l| l.link + 1 == l.chain)
                .filter(|l| {
                    (l.at + 1..i).any(|k| {
                        reads(&ins[k].kind).contains(&l.reg) && written(&ins[k].kind) != Some(l.reg)
                    })
                })
                .map(|l| l.reg)
                .collect();
            loads.retain(|l| !temporaries.contains(&l.reg));
            sites.push(Site {
                at: i,
                loads,
                sets: std::mem::take(&mut sets),
            });
            continue;
        }
        if is_block_end(kind) {
            break;
        }
        let Some(rd) = written(kind) else {
            continue;
        };
        sets.insert(rd);
        if reads_only_self(kind, rd) {
            if let Some(previous) = pending.iter_mut().rev().find(|l| l.reg == rd) {
                let chain = previous.chain + 1;
                let link = previous.link + 1;
                let head = previous.at;
                for load in pending.iter_mut().filter(|l| l.reg == rd && l.at >= head) {
                    load.chain = chain;
                }
                for load in pending.iter_mut().filter(|l| l.reg == rd && l.at >= head) {
                    load.costly = true;
                }
                pending.push(Load {
                    reg: rd,
                    at: i,
                    link,
                    chain,
                    costly: true,
                    blocked: false,
                });
                continue;
            }
        }
        pending.retain(|l| l.reg != rd);
        // A store or compare between the previous call and this load that
        // reads the old value of the register is an anti-dependence.
        let site_start = sites.last().map(|s: &Site| s.at + 1).unwrap_or(start);
        let blocked = (site_start..i)
            .rev()
            .take_while(|k| written(&ins[*k].kind) != Some(rd))
            .any(|k| written(&ins[k].kind).is_none() && reads(&ins[k].kind).contains(&rd));
        pending.push(Load {
            reg: rd,
            at: i,
            link: 0,
            chain: 1,
            costly: matches!(kind, Kind::LdrPool { .. }),
            blocked,
        });
    }
    sites
}

/// Forward dependents of the last link of a chain for `reg` at site `k`:
/// later calls that clobber the register until the next set of it.
fn dependents(sites: &[Site], k: usize, reg: u8, value: &[bool]) -> usize {
    let mut count = 1;
    if reg == 0 && value[k] {
        return count;
    }
    for (j, site) in sites.iter().enumerate().skip(k + 1) {
        if site.sets.contains(&reg) {
            return count + 1;
        }
        count += 1;
        if reg == 0 && value[j] {
            return count;
        }
    }
    count
}

/// Simulates the scheduler over one call site and returns the instruction
/// indices in emitted order.
fn schedule(sites: &[Site], k: usize, value: &[bool], direct: &[bool]) -> Vec<usize> {
    let site = &sites[k];
    let mut ready: Vec<usize> = (0..site.loads.len())
        .filter(|l| site.loads[*l].link == 0 && !site.loads[*l].blocked)
        .collect();
    let mut waiting: Vec<usize> = (0..site.loads.len())
        .filter(|l| site.loads[*l].link == 0 && site.loads[*l].blocked)
        .collect();
    let mut penalized: Vec<usize> = Vec::new();
    let mut order = Vec::new();
    let mut done = vec![false; site.loads.len()];
    if ready.is_empty() {
        ready.append(&mut waiting);
    }
    while !ready.is_empty() {
        let rank = |l: &usize| {
            let load = &site.loads[*l];
            let priority = load.chain - load.link;
            let unpenalized = !penalized.contains(l);
            let deps = if load.link + 1 == load.chain {
                dependents(sites, k, load.reg, value)
            } else {
                1
            };
            // Original order breaks the remaining ties: a direct call's costly
            // arguments were precomputed ahead of its cheap ones.
            let early = u8::from(direct[k] && load.costly);
            (
                priority,
                unpenalized,
                deps,
                early,
                u8::MAX - load.reg,
                usize::MAX - load.link,
            )
        };
        ready.sort_by_key(rank);
        let chosen = ready.pop().unwrap();
        done[chosen] = true;
        // The reader that blocked a load goes right after the first load, so
        // the blocked load is ready next and loses that one tie as a
        // dependent of the last scheduled instruction.
        penalized.clear();
        if !waiting.is_empty() {
            penalized.extend(waiting.iter().copied());
            ready.append(&mut waiting);
        }
        order.push(site.loads[chosen].at);
        let load = &site.loads[chosen];
        if let Some(next) = site
            .loads
            .iter()
            .position(|l| l.reg == load.reg && l.link == load.link + 1 && l.at > load.at)
        {
            ready.push(next);
        }
    }
    order
}

fn observed(site: &Site) -> Vec<usize> {
    let mut order: Vec<usize> = site.loads.iter().map(|l| l.at).collect();
    order.sort_unstable();
    order
}

/// Infers which calls return a value: the set of `bl` instruction indices
/// whose r0 set, rather than clobber, makes the predicted load order of
/// every call in its block match the reference.
pub struct Sites {
    pub value: BTreeSet<usize>,
    pub direct: BTreeSet<usize>,
}

pub fn value_calls(ins: &[Ins]) -> Sites {
    // A branch over a literal pool is not a block boundary: the scheduler
    // ran before the pool was placed.
    let pool_jump = |k: usize| match ins[k].kind {
        Kind::B { target } => ins.get(k + 1).is_some_and(|next| next.addr == target),
        _ => false,
    };
    let targets: BTreeSet<u32> = ins
        .iter()
        .enumerate()
        .filter(|(k, _)| !pool_jump(*k))
        .filter_map(|(_, x)| match x.kind {
            Kind::B { target } | Kind::Bcond { target, .. } => Some(target),
            _ => None,
        })
        .collect();
    let mut result = Sites {
        value: BTreeSet::new(),
        direct: BTreeSet::new(),
    };
    let mut start = 0;
    while start < ins.len() {
        let mut end = start;
        while end < ins.len() {
            if end > start && targets.contains(&ins[end].addr) {
                break;
            }
            end += 1;
            if is_block_end(&ins[end - 1].kind) && !pool_jump(end - 1) {
                break;
            }
        }
        let sites = sites_in(ins, start, end, &targets);
        // A call whose result is read right after it returns a value.
        let mut value: Vec<bool> = sites
            .iter()
            .map(|site| {
                ins.get(site.at + 1)
                    .is_some_and(|next| uses_r0(&next.kind) && written(&next.kind) != Some(0))
                    || ins.get(site.at + 1).is_some_and(|next| {
                        matches!(
                            next.kind,
                            Kind::Movs { rm: 0, .. } | Kind::MovHi { rm: 0, .. }
                        )
                    })
            })
            .collect();
        // Wrapper spelling by default; a direct call is inferred from its order.
        let mut direct = vec![false; sites.len()];
        let mut matched = vec![false; sites.len()];
        for k in 0..sites.len() {
            if schedule(&sites, k, &value, &direct) == observed(&sites[k]) {
                matched[k] = true;
                continue;
            }
            // Flip the direct bit of this call, then the value bit of this or a
            // later call, with or without the direct bit, until the order
            // matches without breaking a call already matched.
            // The value bit is the cheaper explanation: a direct call also changes
            // how its constants are shared, so it is tried after.
            let mut choices: Vec<(usize, bool, bool)> = Vec::new();
            for j in k..sites.len() {
                choices.push((j, true, false));
            }
            choices.push((k, false, true));
            for j in k..sites.len() {
                choices.push((j, true, true));
            }
            for (j, flip_value, flip_direct) in choices {
                if flip_value {
                    value[j] = !value[j];
                }
                if flip_direct {
                    direct[k] = !direct[k];
                }
                let fixed = schedule(&sites, k, &value, &direct) == observed(&sites[k])
                    && (0..k).all(|m| {
                        !matched[m] || schedule(&sites, m, &value, &direct) == observed(&sites[m])
                    });
                if fixed {
                    matched[k] = true;
                    break;
                }
                if flip_value {
                    value[j] = !value[j];
                }
                if flip_direct {
                    direct[k] = !direct[k];
                }
            }
        }
        if std::env::var_os("LIFTER_DEBUG").is_some() {
            for (k, site) in sites.iter().enumerate() {
                let predicted = schedule(&sites, k, &value, &direct);
                let seen = observed(site);
                eprintln!(
                    "site {} {:08x} value={} direct={} {} predicted={predicted:?} observed={seen:?}",
                    site.at,
                    ins[site.at].addr,
                    value[k],
                    direct[k],
                    if predicted == seen { "ok" } else { "MISMATCH" }
                );
            }
        }
        for (k, site) in sites.iter().enumerate() {
            if value[k] {
                result.value.insert(site.at);
            }
            if direct[k] {
                result.direct.insert(site.at);
            }
        }
        start = end.max(start + 1);
    }
    result
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::decode::{decode_window, OVERLAY_BASE};

    fn image(halves: &[u16]) -> Vec<u8> {
        halves.iter().flat_map(|h| h.to_le_bytes()).collect()
    }

    /// `movs r1,#0; movs r0,#12; bl A; movs r0,#7; bl B; bx lr` needs A void,
    /// while the same with B taking two arguments keeps r0 first.
    #[test]
    fn next_call_arity_sets_the_order() {
        // bl encodings: target offsets are irrelevant to the schedule.
        let block = image(&[
            0x2100, 0x200c, 0xf000, 0xf810, 0x2007, 0xf000, 0xf810, 0x4770,
        ]);
        let ins = decode_window(&block, OVERLAY_BASE, block.len() as u32);
        assert_eq!(value_calls(&ins).value, BTreeSet::from([2]));
        let mismatch = image(&[
            0x200c, 0x2100, 0xf000, 0xf810, 0x2007, 0xf000, 0xf810, 0x4770,
        ]);
        let ins = decode_window(&mismatch, OVERLAY_BASE, mismatch.len() as u32);
        assert!(value_calls(&ins).value.is_empty());
    }
}

#[cfg(test)]
mod direct_tests {
    use super::*;
    use crate::decode::{decode_window, OVERLAY_BASE};

    fn image(halves: &[u16]) -> Vec<u8> {
        halves.iter().flat_map(|h| h.to_le_bytes()).collect()
    }

    /// `movs r2, #128; lsls r2, r2, #8; movs r0, #1; movs r1, #0; bl A;
    /// movs r0, #1; movs r1, #2; movs r2, #3; bl B; bx lr`: the costly
    /// argument came first, so A was a direct call; with the chain split
    /// around the cheap loads it was a wrapper.
    #[test]
    fn costly_first_means_direct() {
        let direct = image(&[
            0x2280, 0x0212, 0x2001, 0x2100, 0xf000, 0xf80a, 0x2001, 0x2102, 0x2203, 0xf000, 0xf80a,
            0x4770,
        ]);
        let ins = decode_window(&direct, OVERLAY_BASE, direct.len() as u32);
        assert_eq!(value_calls(&ins).direct, BTreeSet::from([4]));
        let wrapper = image(&[
            0x2280, 0x2001, 0x2100, 0x0212, 0xf000, 0xf80a, 0x2001, 0x2102, 0x2203, 0xf000, 0xf80a,
            0x4770,
        ]);
        let ins = decode_window(&wrapper, OVERLAY_BASE, wrapper.len() as u32);
        assert!(value_calls(&ins).direct.is_empty());
    }
}
