//! The boundary model: what one GCC 2.96 translation unit can and cannot
//! produce between two adjacent functions, and the scored evidence for the
//! gaps it leaves open. Pure: the loader fills an `Image`, this judges it.

use std::collections::{BTreeMap, BTreeSet};

#[derive(Clone, Copy, Debug, PartialEq, Eq, PartialOrd, Ord)]
pub enum Lang {
    /// Game C, compiled by the game's compiler.
    C,
    /// Handwritten assembly under games/.
    Assembly,
    /// Nintendo library or compiler runtime: another compiler, another file.
    Library,
    /// Not-yet-C listing or scaffold; its source language is still open.
    Listing,
    /// An 8-byte far-call or overlay veneer.
    Veneer,
}

impl Lang {
    pub fn label(self) -> &'static str {
        match self {
            Lang::C => "c",
            Lang::Assembly => "assembly",
            Lang::Library => "library",
            Lang::Listing => "listing",
            Lang::Veneer => "veneer",
        }
    }
}

/// What fills the bytes between one function's body and the next function.
#[derive(Clone, Copy, Debug, PartialEq, Eq, PartialOrd, Ord)]
pub enum Pad {
    None,
    Zero,
    Nop,
    Other,
}

impl Pad {
    pub fn of(bytes: &[u8]) -> Pad {
        if bytes.is_empty() {
            Pad::None
        } else if bytes.iter().all(|&b| b == 0) {
            Pad::Zero
        } else if bytes.len().is_multiple_of(2) && bytes.chunks(2).all(|h| h == [0xc0, 0x46]) {
            Pad::Nop
        } else {
            Pad::Other
        }
    }

    pub fn label(self) -> &'static str {
        match self {
            Pad::None => "none",
            Pad::Zero => "zero",
            Pad::Nop => "nop",
            Pad::Other => "other",
        }
    }
}

#[derive(Clone, Debug)]
pub struct Function {
    pub name: String,
    pub start: u32,
    /// End of the body: the symbol's size, literal pool included.
    pub end: u32,
    /// Today's object, relative to the build's obj directory.
    pub object: String,
    pub lang: Lang,
    pub arm: bool,
    /// A local symbol in today's object (our reconstruction's `static`).
    pub is_static: bool,
    pub region: usize,
    /// Everything the relocations and literal pools reach, callees excluded.
    pub refs: BTreeSet<String>,
    /// Functions of this image it calls, through veneers.
    pub callees: BTreeSet<usize>,
    /// Functions of this image it reaches by a plain branch (not a call).
    pub branches: BTreeSet<usize>,
    /// Data blocks of this image it references.
    pub data_refs: BTreeSet<usize>,
    /// Data blocks of this image reached through a local symbol.
    pub static_data: BTreeSet<usize>,
    /// Referenced from outside this image (an overlay's import, a far call).
    pub external: bool,
    /// Bytes between the body's end and the next function, data excluded.
    pub pad: Vec<u8>,
    /// Hash of the bytes with relocated places blanked; 0 for veneers.
    pub shape: u64,
    /// Other images holding the same shape (functions of MIN_SHAPE bytes or more).
    pub copies: usize,
    /// Other images where this shape is followed by the next function's.
    pub pair_copies: usize,
}

#[derive(Clone, Debug)]
pub struct Data {
    pub start: u32,
    pub end: u32,
    pub object: String,
    pub region: usize,
    pub names: Vec<String>,
    /// Functions of this image the block points at.
    pub targets: BTreeSet<usize>,
}

#[derive(Clone, Debug, Default)]
pub struct Image {
    pub name: String,
    pub functions: Vec<Function>,
    pub data: Vec<Data>,
    /// Static inline helpers each of today's C files defines itself.
    pub inline_helpers: BTreeMap<String, BTreeSet<String>>,
    /// Objects whose data the linker script places away from their text.
    pub displaced_data: BTreeSet<String>,
}

#[derive(Clone, Copy, Debug, PartialEq, Eq, PartialOrd, Ord)]
pub enum Verdict {
    ForcedBoundary,
    ForcedJoin,
    Conflict,
    JudgedBoundary,
    JudgedJoin,
}

impl Verdict {
    pub fn label(self) -> &'static str {
        match self {
            Verdict::ForcedBoundary => "forced-boundary",
            Verdict::ForcedJoin => "forced-join",
            Verdict::Conflict => "conflict",
            Verdict::JudgedBoundary => "judged-boundary",
            Verdict::JudgedJoin => "judged-join",
        }
    }

    pub fn is_boundary(self) -> bool {
        matches!(self, Verdict::ForcedBoundary | Verdict::Conflict | Verdict::JudgedBoundary)
    }

    pub fn is_forced(self) -> bool {
        !matches!(self, Verdict::JudgedBoundary | Verdict::JudgedJoin)
    }
}

/// The gap after function `index`.
#[derive(Clone, Debug)]
pub struct Gap {
    pub index: usize,
    pub forced_boundary: Vec<String>,
    pub forced_join: Vec<String>,
    /// Join evidence from our own source (statics, inline helpers).
    pub source_join: Vec<String>,
    pub signals: Vec<String>,
    /// Boundary probability from every signal.
    pub probability: f64,
    /// Boundary probability from the binary alone: no names, folders or statics.
    pub binary: f64,
    pub verdict: Verdict,
    pub similarity: f64,
    pub data_bytes: u32,
    /// The scored signals by name, for measuring which ones separate.
    pub features: Vec<(&'static str, f64)>,
}

/// The widest run of functions a helper's referrers may span and still be
/// read as one file's private helper.
pub const LOCAL_SPAN: usize = 4;
/// Beyond this span a run of referrers says nothing.
pub const NEAR_SPAN: usize = 12;

/// The subsystem prefix of one of our names: `Title_Run` gives `Title`,
/// `BtlTgtExec` gives `Btl`; placeholders give none.
pub fn prefix(name: &str) -> Option<String> {
    if crate::ld_map::is_placeholder(name) || name.starts_with("sub_") {
        return None;
    }
    let name = name.trim_start_matches('_');
    if let Some((head, _)) = name.split_once('_') {
        return (!head.is_empty()).then(|| head.to_string());
    }
    let mut chars = name.char_indices();
    let (_, first) = chars.next()?;
    if !first.is_ascii_alphabetic() {
        return None;
    }
    let end = chars.find(|(_, c)| c.is_ascii_uppercase() || c.is_ascii_digit()).map_or(name.len(), |(i, _)| i);
    Some(name[..end].to_string())
}

/// Today's folder of an object path.
pub fn folder(object: &str) -> &str {
    object.rsplit_once('/').map_or("", |(dir, _)| dir)
}

/// Weighted Jaccard similarity of two reference sets.
pub fn similarity(a: &BTreeSet<String>, b: &BTreeSet<String>, weights: &BTreeMap<String, f64>) -> f64 {
    let weight = |k: &String| weights.get(k).copied().unwrap_or(1.0);
    let shared: f64 = a.intersection(b).map(weight).sum();
    let union: f64 = a.union(b).map(weight).sum();
    if union == 0.0 {
        0.0
    } else {
        shared / union
    }
}

fn logistic(z: f64) -> f64 {
    1.0 / (1.0 + (-z).exp())
}

/// The data blocks placed after function `index` and before the next one.
pub fn data_after(image: &Image, index: usize) -> Vec<usize> {
    let f = &image.functions[index];
    let next = image.functions.get(index + 1);
    image
        .data
        .iter()
        .enumerate()
        .filter(|(_, d)| d.region == f.region && d.start >= f.end && next.is_some_and(|n| d.end <= n.start && n.region == f.region))
        .map(|(i, _)| i)
        .collect()
}

/// The position of each data block: the function it follows, if any.
/// Only a block between two functions of one region has one; a block after
/// a region's last function (the trailing data area of a bank or overlay)
/// belongs to its users' unit.
pub fn data_owner(image: &Image) -> Vec<Option<usize>> {
    image
        .data
        .iter()
        .map(|d| {
            let at = image.functions.partition_point(|f| f.start < d.start);
            let before = at.checked_sub(1).filter(|&i| image.functions[i].region == d.region)?;
            image.functions.get(at).filter(|next| next.region == d.region).map(|_| before)
        })
        .collect()
}

/// The functions of a data block's region that reference it.
pub fn data_users(image: &Image) -> Vec<BTreeSet<usize>> {
    let mut users: Vec<BTreeSet<usize>> = vec![BTreeSet::new(); image.data.len()];
    for (i, f) in image.functions.iter().enumerate() {
        for &d in &f.data_refs {
            if image.data[d].region == f.region {
                users[d].insert(i);
            }
        }
    }
    users
}

/// Log-odds of a boundary before any scored evidence: about one gap in twenty.
pub const PRIOR: f64 = -3.0;
/// Log-odds weights of the binary signals (positive: boundary).
pub const BINARY_WEIGHTS: &[(&str, f64)] = &[
    ("depth", 4.5),
    ("cohesion", -3.0),
    ("pair", -2.0),
    ("calls", -1.5),
    ("near", -1.0),
    ("order", -2.0),
    ("shared-edge", 5.0),
    ("shared-pair", -2.0),
];
/// Weights of what our own source says: names, folders, statics.
pub const SOURCE_WEIGHTS: &[(&str, f64)] = &[("prefix", 0.5), ("folder", 1.0), ("displaced-data", 1.0), ("source-join", -3.0)];

/// How far the cohesion window reaches on each side of a gap.
pub const WINDOW: usize = 3;

/// Every referrer of every function: callers, branches and the data blocks
/// that point at it (placed after the function they follow), and whether
/// anything outside a short run reaches it.
fn referrers(image: &Image, owner: &[Option<usize>], users: &[BTreeSet<usize>]) -> (Vec<BTreeSet<usize>>, Vec<bool>) {
    let n = image.functions.len();
    let mut referrers: Vec<BTreeSet<usize>> = vec![BTreeSet::new(); n];
    let mut far: Vec<bool> = image.functions.iter().map(|f| f.external).collect();
    for (i, f) in image.functions.iter().enumerate() {
        for &c in f.callees.iter().chain(&f.branches) {
            if c != i {
                referrers[c].insert(i);
                if f.lang == Lang::Veneer {
                    far[c] = true;
                }
            }
        }
    }
    for (d, data) in image.data.iter().enumerate() {
        for &t in &data.targets {
            match owner[d] {
                Some(o) => {
                    referrers[t].insert(o);
                }
                None if users[d].is_empty() => far[t] = true,
                None => referrers[t].extend(users[d].iter().copied()),
            }
        }
    }
    (referrers, far)
}

/// The entity set of every function: what it references, what it calls and
/// who calls it. Two functions of one file share callers as often as callees.
fn entities(image: &Image, referrers: &[BTreeSet<usize>]) -> Vec<BTreeSet<String>> {
    image
        .functions
        .iter()
        .enumerate()
        .map(|(i, f)| {
            let mut set: BTreeSet<String> = f.refs.clone();
            set.extend(f.callees.iter().map(|c| format!("fn:{:08x}", image.functions[*c].start)));
            set.extend(referrers[i].iter().map(|r| format!("by:{:08x}", image.functions[*r].start)));
            set
        })
        .collect()
}

fn union(sets: &[BTreeSet<String>], range: std::ops::RangeInclusive<usize>) -> BTreeSet<String> {
    let mut out = BTreeSet::new();
    for i in range {
        out.extend(sets[i].iter().cloned());
    }
    out
}

/// Every gap of the image judged.
pub fn judge(image: &Image) -> Vec<Gap> {
    let n = image.functions.len();
    if n < 2 {
        return Vec::new();
    }
    let owner = data_owner(image);
    let users = data_users(image);
    let (referrers, far) = referrers(image, &owner, &users);
    let sets = entities(image, &referrers);
    let mut frequency: BTreeMap<&str, usize> = BTreeMap::new();
    for set in &sets {
        for e in set {
            *frequency.entry(e).or_default() += 1;
        }
    }
    let weights: BTreeMap<String, f64> = frequency.into_iter().map(|(k, c)| (k.to_string(), (n as f64 / c as f64).ln().max(0.05))).collect();

    // A helper reached only from a short run around it, function or data.
    let mut local_join: Vec<Vec<String>> = vec![Vec::new(); n - 1];
    let mut near_join: Vec<Vec<String>> = vec![Vec::new(); n - 1];
    let mut mark = |low: usize, high: usize, region_ok: bool, label: String| {
        if !region_ok || high == low {
            return;
        }
        let target = if high - low <= LOCAL_SPAN {
            &mut local_join
        } else if high - low <= NEAR_SPAN {
            &mut near_join
        } else {
            return;
        };
        for gap in target.iter_mut().take(high).skip(low) {
            gap.push(label.clone());
        }
    };
    for (i, f) in image.functions.iter().enumerate() {
        if far[i] || referrers[i].is_empty() || f.lang == Lang::Veneer {
            continue;
        }
        let low = referrers[i].iter().copied().min().unwrap_or(i).min(i);
        let high = referrers[i].iter().copied().max().unwrap_or(i).max(i);
        let same = referrers[i].iter().all(|&r| image.functions[r].region == f.region && image.functions[r].lang == f.lang);
        mark(low, high, same, format!("local:{}", f.name));
    }
    for (d, used) in users.iter().enumerate() {
        let (low, o) = match owner[d] {
            Some(o) if !used.is_empty() && used.iter().all(|&u| u <= o) => (used.iter().copied().min().unwrap_or(o), o),
            None if used.len() > 1 => (used.iter().copied().min().unwrap_or(0), used.iter().copied().max().unwrap_or(0)),
            _ => continue,
        };
        let lang = image.functions[o].lang;
        let same = used.iter().all(|&u| image.functions[u].region == image.data[d].region && image.functions[u].lang == lang);
        let name = image.data[d].names.first().cloned().unwrap_or_else(|| format!("{:08x}", image.data[d].start));
        mark(low, o, same, format!("rodata:{name}"));
    }
    // A plain branch into another function: no compiler emits one across files.
    let mut branch_join: Vec<Vec<String>> = vec![Vec::new(); n - 1];
    for (j, f) in image.functions.iter().enumerate() {
        for &t in &f.branches {
            for gap in branch_join.iter_mut().take(j.max(t)).skip(j.min(t)) {
                gap.push(format!("branch:{}", image.functions[t].name));
            }
        }
    }
    // Joins our source asserts: a static used across the gap.
    let mut source_join: Vec<Vec<String>> = vec![Vec::new(); n - 1];
    for (i, f) in image.functions.iter().enumerate() {
        if f.is_static {
            for &r in &referrers[i] {
                for gap in source_join.iter_mut().take(r.max(i)).skip(r.min(i)) {
                    gap.push(format!("static:{}", f.name));
                }
            }
        }
    }
    let mut static_users: BTreeMap<usize, BTreeSet<usize>> = BTreeMap::new();
    for (i, f) in image.functions.iter().enumerate() {
        for &d in &f.static_data {
            let entry = static_users.entry(d).or_default();
            entry.insert(i);
            if let Some(o) = owner[d] {
                entry.insert(o);
            }
        }
    }
    for (d, used) in &static_users {
        let (Some(&low), Some(&high)) = (used.first(), used.last()) else {
            continue;
        };
        let name = image.data[*d].names.first().cloned().unwrap_or_else(|| format!("{:08x}", image.data[*d].start));
        for gap in source_join.iter_mut().take(high).skip(low) {
            gap.push(format!("static-data:{name}"));
        }
    }
    // Link order: a bank's data follows its code object by object, so a data
    // block placed before another but used after it ties the two users' runs
    // into one file (unless a data-only file or a global table intervenes).
    let mut order_join: Vec<Vec<String>> = vec![Vec::new(); n - 1];
    let trailing: Vec<usize> = (0..image.data.len()).filter(|&d| owner[d].is_none() && !users[d].is_empty() && users[d].len() <= 3).collect();
    for (x, &j) in trailing.iter().enumerate() {
        for &k in &trailing[x + 1..] {
            if image.data[k].region != image.data[j].region {
                continue;
            }
            let (Some(&low), Some(&high)) = (users[k].first(), users[j].last()) else {
                continue;
            };
            if low < high && high - low <= NEAR_SPAN {
                let name = image.data[j].names.first().cloned().unwrap_or_else(|| format!("{:08x}", image.data[j].start));
                for gap in order_join.iter_mut().take(high).skip(low) {
                    gap.push(format!("order:{name}"));
                }
            }
        }
    }

    // Forced boundaries first: the cohesion windows stop at them.
    let mut forced: Vec<Vec<String>> = Vec::with_capacity(n - 1);
    let mut data_bytes = Vec::with_capacity(n - 1);
    for i in 0..n - 1 {
        let (a, b) = (&image.functions[i], &image.functions[i + 1]);
        let mut reasons = Vec::new();
        let data = data_after(image, i);
        let bytes: u32 = data.iter().map(|&d| image.data[d].end - image.data[d].start).sum();
        if a.region != b.region {
            reasons.push("region".to_string());
        }
        let compiled = |l: Lang| matches!(l, Lang::C | Lang::Library | Lang::Listing);
        if !data.is_empty() && (compiled(a.lang) || compiled(b.lang)) {
            let objects: BTreeSet<&str> = data.iter().map(|&d| image.data[d].object.as_str()).collect();
            reasons.push(format!("data:{}B:{}", bytes, objects.into_iter().collect::<Vec<_>>().join(",")));
        }
        if a.lang != b.lang && !(matches!(a.lang, Lang::C | Lang::Listing) && matches!(b.lang, Lang::C | Lang::Listing)) {
            reasons.push(format!("language:{}|{}", a.lang.label(), b.lang.label()));
        } else if a.arm != b.arm && a.lang != Lang::Veneer {
            reasons.push(format!("isa:{}|{}", if a.arm { "arm" } else { "thumb" }, if b.arm { "arm" } else { "thumb" }));
        }
        if a.lang == Lang::C && b.lang == Lang::C && data.is_empty() && a.region == b.region && b.start > ((a.end + 3) & !3) {
            reasons.push(format!("pad:{}B", b.start - a.end));
        }
        // An archive member is a file of its own.
        if a.object != b.object && (a.object.contains(".a(") || b.object.contains(".a(")) {
            reasons.push("archive-member".to_string());
        }
        forced.push(reasons);
        data_bytes.push(bytes);
    }
    // Cohesion across each gap: the window on each side, cut at forced boundaries.
    let cohesion: Vec<f64> = (0..n - 1)
        .map(|i| {
            let mut low = i;
            while low > 0 && i - low + 1 < WINDOW && forced[low - 1].is_empty() {
                low -= 1;
            }
            let mut high = i + 1;
            while high + 1 < n && high - i < WINDOW && forced[high].is_empty() {
                high += 1;
            }
            similarity(&union(&sets, low..=i), &union(&sets, i + 1..=high), &weights)
        })
        .collect();
    // Depth of each gap's valley: how far cohesion climbs on both sides before falling.
    let depth: Vec<f64> = (0..n - 1)
        .map(|i| {
            let mut left = cohesion[i];
            let mut k = i;
            while k > 0 && forced[k - 1].is_empty() && cohesion[k - 1] >= left {
                k -= 1;
                left = cohesion[k];
            }
            let mut right = cohesion[i];
            let mut k = i;
            while k + 1 < n - 1 && forced[k + 1].is_empty() && cohesion[k + 1] >= right {
                k += 1;
                right = cohesion[k];
            }
            (left - cohesion[i]) + (right - cohesion[i])
        })
        .collect();

    let mut gaps = Vec::with_capacity(n - 1);
    for i in 0..n - 1 {
        let (a, b) = (&image.functions[i], &image.functions[i + 1]);
        let forced_boundary = forced[i].clone();
        let mut forced_join = branch_join[i].clone();
        forced_join.sort();
        forced_join.dedup();
        // A private helper joins only where nothing else divides the two sides.
        if a.lang == b.lang {
            forced_join.extend(local_join[i].iter().cloned());
        }
        let mut signals = Vec::new();
        let mut source = source_join[i].clone();
        if a.object != b.object && a.lang == Lang::C && b.lang == Lang::C {
            if let (Some(left), Some(right)) = (image.inline_helpers.get(&a.object), image.inline_helpers.get(&b.object)) {
                for name in left.intersection(right) {
                    source.push(format!("inline:{name}"));
                }
            }
        }
        source.sort();
        source.dedup();

        // Scored evidence, in log-odds of a boundary.
        let pair = similarity(&sets[i], &sets[i + 1], &weights);
        let calls = a.callees.contains(&(i + 1)) || b.callees.contains(&i);
        let near = !near_join[i].is_empty();
        let shared = a.copies > 0 || b.copies > 0;
        let shared_edge = shared && a.pair_copies == 0;
        let shared_pair = shared && a.pair_copies > 0;
        let prefix_change = match (prefix(&a.name), prefix(&b.name)) {
            (Some(x), Some(y)) if x == y => -1.0,
            (Some(_), Some(_)) => 1.0,
            _ => 0.0,
        };
        let folder_change = folder(&a.object) != folder(&b.object);
        let displaced = a.object != b.object && image.displaced_data.contains(&a.object);
        let flag = |b: bool| if b { 1.0 } else { 0.0 };
        let features = vec![
            ("pair", pair),
            ("cohesion", cohesion[i]),
            ("depth", depth[i]),
            ("calls", flag(calls)),
            ("near", flag(near)),
            ("shared-edge", flag(shared_edge)),
            ("shared-pair", flag(shared_pair)),
            ("order", flag(!order_join[i].is_empty())),
            ("prefix", prefix_change),
            ("folder", flag(folder_change)),
            ("displaced-data", flag(displaced)),
            ("source-join", flag(!source.is_empty())),
        ];
        let value = |name: &str| features.iter().find(|(k, _)| *k == name).map_or(0.0, |(_, v)| *v);
        let mut binary = PRIOR + BINARY_WEIGHTS.iter().map(|(k, w)| w * value(k)).sum::<f64>();
        signals.push(format!("pair:{pair:.2}"));
        signals.push(format!("cohesion:{:.2}", cohesion[i]));
        signals.push(format!("depth:{:.2}", depth[i]));
        if calls {
            signals.push("calls".into());
        }
        if near {
            signals.push(format!("near:{}", near_join[i].len()));
        }
        if shared_edge {
            signals.push(format!("shared-edge:{}|{}", a.copies, b.copies));
        }
        if shared_pair {
            signals.push(format!("shared-pair:{}", a.pair_copies));
        }
        if !local_join[i].is_empty() && a.lang != b.lang {
            signals.push(format!("local:{}", local_join[i].len()));
        }
        if !order_join[i].is_empty() {
            signals.push(format!("order:{}", order_join[i].len()));
        }
        if a.lang == Lang::Veneer && b.lang == Lang::Veneer {
            binary = PRIOR;
            signals.push("veneers".into());
        }
        let z = binary + SOURCE_WEIGHTS.iter().map(|(k, w)| w * value(k)).sum::<f64>();
        if prefix_change != 0.0 {
            signals.push(format!("prefix:{}", if prefix_change > 0.0 { "change" } else { "same" }));
        }
        if folder_change {
            signals.push("folder".into());
        }
        if displaced {
            signals.push("displaced-data".into());
        }
        let verdict = match (!forced_boundary.is_empty(), !forced_join.is_empty()) {
            (true, true) => Verdict::Conflict,
            (true, false) => Verdict::ForcedBoundary,
            (false, true) => Verdict::ForcedJoin,
            (false, false) if logistic(z) >= 0.5 => Verdict::JudgedBoundary,
            _ => Verdict::JudgedJoin,
        };
        let (probability, binary) = match verdict {
            Verdict::ForcedBoundary | Verdict::Conflict => (1.0, 1.0),
            Verdict::ForcedJoin => (0.0, 0.0),
            _ => (logistic(z), logistic(binary)),
        };
        gaps.push(Gap {
            index: i,
            forced_boundary,
            forced_join,
            source_join: source,
            signals,
            probability,
            binary,
            verdict,
            similarity: cohesion[i],
            features,
            data_bytes: data_bytes[i],
        });
    }
    gaps
}

#[derive(Clone, Debug)]
pub struct Unit {
    pub first: usize,
    pub last: usize,
    pub data: Vec<usize>,
    pub confidence: f64,
    pub name: String,
}

impl Unit {
    pub fn objects<'a>(&self, image: &'a Image) -> Vec<&'a str> {
        let mut seen = Vec::new();
        for f in &image.functions[self.first..=self.last] {
            if seen.last() != Some(&f.object.as_str()) && !seen.contains(&f.object.as_str()) {
                seen.push(f.object.as_str());
            }
        }
        seen
    }
}

/// A neutral placeholder: today's object holding most of the unit's code,
/// with the count of the others it absorbs.
pub fn placeholder(image: &Image, first: usize, last: usize) -> String {
    let mut bytes: BTreeMap<&str, u32> = BTreeMap::new();
    let mut order: Vec<&str> = Vec::new();
    for f in &image.functions[first..=last] {
        if !order.contains(&f.object.as_str()) {
            order.push(&f.object);
        }
        *bytes.entry(&f.object).or_default() += f.end - f.start;
    }
    let main = order.iter().max_by_key(|o| (bytes[*o], std::cmp::Reverse(order.iter().position(|x| x == *o)))).copied().unwrap_or("");
    let stem = main.trim_end_matches(".o");
    let stem = stem.split_once("/SRC/").map_or(stem, |(_, tail)| tail);
    if order.len() > 1 {
        format!("{stem}+{}", order.len() - 1)
    } else {
        stem.to_string()
    }
}

/// Functions grouped at every boundary the gaps judge.
pub fn group(image: &Image, gaps: &[Gap], use_source: bool) -> Vec<Unit> {
    let n = image.functions.len();
    if n == 0 {
        return Vec::new();
    }
    let cut = |g: &Gap| {
        if use_source {
            g.verdict.is_boundary()
        } else {
            match g.verdict {
                Verdict::JudgedBoundary | Verdict::JudgedJoin => g.binary >= 0.5,
                v => v.is_boundary(),
            }
        }
    };
    let p = |g: &Gap| if use_source { g.probability } else { g.binary };
    let mut units = Vec::new();
    let mut first = 0;
    for i in 0..n {
        let end = i + 1 == n || cut(&gaps[i]);
        if !end {
            continue;
        }
        let mut confidence: f64 = 1.0;
        if first > 0 {
            confidence = confidence.min(p(&gaps[first - 1]));
        }
        if i + 1 < n {
            confidence = confidence.min(p(&gaps[i]));
        }
        for g in &gaps[first..i] {
            confidence = confidence.min(1.0 - p(g));
        }
        let data = data_after(image, i);
        units.push(Unit { first, last: i, data, confidence, name: placeholder(image, first, i) });
        first = i + 1;
    }
    // Trailing data goes to the unit holding most of its users.
    let owner = data_owner(image);
    for (d, used) in data_users(image).iter().enumerate() {
        if owner[d].is_some() || used.is_empty() {
            continue;
        }
        let mut votes: BTreeMap<usize, usize> = BTreeMap::new();
        for &u in used {
            if let Some(k) = units.iter().position(|unit| unit.first <= u && u <= unit.last) {
                *votes.entry(k).or_default() += 1;
            }
        }
        if let Some((&k, _)) = votes.iter().max_by_key(|(k, v)| (**v, std::cmp::Reverse(**k))) {
            units[k].data.push(d);
        }
    }
    units
}

/// Data blocks no function of their region references: tables reached only
/// through other tables, ids or other images.
pub fn unclaimed(image: &Image) -> Vec<usize> {
    let owner = data_owner(image);
    data_users(image).iter().enumerate().filter(|(d, used)| owner[*d].is_none() && used.is_empty()).map(|(d, _)| d).collect()
}

#[cfg(test)]
mod tests {
    use super::*;

    fn function(name: &str, start: u32, size: u32, object: &str, refs: &[&str]) -> Function {
        Function {
            name: name.into(),
            start,
            end: start + size,
            object: object.into(),
            lang: Lang::C,
            arm: false,
            is_static: false,
            region: 0,
            refs: refs.iter().map(|r| r.to_string()).collect(),
            callees: BTreeSet::new(),
            branches: BTreeSet::new(),
            data_refs: BTreeSet::new(),
            static_data: BTreeSet::new(),
            external: false,
            pad: Vec::new(),
            shape: 0,
            copies: 0,
            pair_copies: 0,
        }
    }

    fn image() -> Image {
        let mut image = Image { name: "test".into(), ..Image::default() };
        image.functions = vec![
            function("Menu_Open", 0x100, 0x20, "games/G/SRC/MENU/OPEN.o", &["ram:gMenu", "rom:MenuTable"]),
            function("Menu_Close", 0x120, 0x20, "games/G/SRC/MENU/CLOSE.o", &["ram:gMenu", "rom:MenuTable"]),
            function("Menu_Draw", 0x140, 0x1e, "games/G/SRC/MENU/DRAW.o", &["ram:gMenu"]),
            function("Battle_Run", 0x170, 0x20, "games/G/SRC/BATTLE/RUN.o", &["ram:gBattle", "io:04000000"]),
            function("Battle_End", 0x190, 0x20, "games/G/SRC/BATTLE/RUN.o", &["ram:gBattle", "io:04000000"]),
            function("Sound_Play", 0x1b0, 0x20, "games/G/SRC/SOUND/PLAY.o", &["ram:gSound"]),
        ];
        image.data = vec![Data {
            start: 0x160,
            end: 0x170,
            object: "games/G/SRC/MENU/DRAW.o".into(),
            region: 0,
            names: vec!["MenuStrings".into()],
            targets: BTreeSet::new(),
        }];
        image
    }

    #[test]
    fn data_between_functions_is_a_forced_boundary() {
        let gaps = judge(&image());
        assert_eq!(gaps[2].verdict, Verdict::ForcedBoundary);
        assert!(gaps[2].forced_boundary[0].starts_with("data:16B:games/G/SRC/MENU/DRAW.o"));
    }

    #[test]
    fn shared_references_join_and_unrelated_neighbours_split() {
        let gaps = judge(&image());
        assert_eq!(gaps[0].verdict, Verdict::JudgedJoin, "{:?}", gaps[0]);
        assert_eq!(gaps[3].verdict, Verdict::JudgedJoin, "{:?}", gaps[3]);
        assert_eq!(gaps[4].verdict, Verdict::JudgedBoundary, "{:?}", gaps[4]);
        let units = group(&image(), &gaps, true);
        let spans: Vec<(usize, usize)> = units.iter().map(|u| (u.first, u.last)).collect();
        assert_eq!(spans, [(0, 2), (3, 4), (5, 5)]);
        assert_eq!(units[0].data, [0]);
        assert_eq!(units[0].name, "MENU/OPEN+2");
        assert_eq!(units[1].name, "BATTLE/RUN");
    }

    #[test]
    fn a_helper_called_only_by_its_neighbour_joins_and_a_far_one_does_not() {
        let mut image = image();
        image.functions[5].callees.insert(4);
        image.functions[3].refs.clear();
        let gaps = judge(&image);
        assert_eq!(gaps[4].verdict, Verdict::ForcedJoin);
        assert_eq!(gaps[4].forced_join, ["local:Battle_End"]);
        image.functions[4].external = true;
        assert_eq!(judge(&image)[4].forced_join, Vec::<String>::new());
    }

    #[test]
    fn a_plain_branch_between_functions_joins_and_conflicts_with_data() {
        let mut image = image();
        image.functions[0].branches.insert(1);
        image.functions[1].branches.insert(3);
        let gaps = judge(&image);
        assert_eq!(gaps[0].verdict, Verdict::ForcedJoin);
        assert_eq!(gaps[2].verdict, Verdict::Conflict);
    }

    #[test]
    fn extra_padding_and_a_change_of_language_or_instruction_set_split() {
        let mut image = image();
        image.functions[1].start = 0x128;
        image.functions[4].arm = true;
        image.functions[5].lang = Lang::Assembly;
        let gaps = judge(&image);
        assert_eq!(gaps[0].forced_boundary, ["pad:8B"]);
        assert_eq!(gaps[3].forced_boundary, ["isa:thumb|arm"]);
        assert_eq!(gaps[4].forced_boundary, ["language:c|assembly"]);
    }

    #[test]
    fn statics_join_only_with_the_source_and_names_read_as_prefixes() {
        let mut image = image();
        image.functions[3].refs = ["ram:gSound".to_string()].into();
        image.functions[4].refs = ["ram:gOther".to_string()].into();
        image.functions[4].is_static = true;
        image.functions[3].callees.insert(4);
        image.functions[4].external = true;
        let gaps = judge(&image);
        assert_eq!(gaps[3].source_join, ["static:Battle_End"]);
        assert!(gaps[3].probability < gaps[3].binary);
        assert_eq!(prefix("Title_Run").as_deref(), Some("Title"));
        assert_eq!(prefix("BtlTgtExec").as_deref(), Some("Btl"));
        assert_eq!(prefix("Func_08001234"), None);
        assert_eq!(Pad::of(&[0xc0, 0x46]), Pad::Nop);
        assert_eq!(Pad::of(&[0, 0]), Pad::Zero);
    }
}
