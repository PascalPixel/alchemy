//! Siblings: one compiled function at several places across Golden Sun's
//! images. Psynergy finds relocation-masked twins; this module supplies the
//! TBS English main program and its code overlays, each place's production
//! status, source and unit, and binding equivalence, and applies the rules
//! that `alchemy check siblings` and the adoption commands share:
//! R1, an equivalent twin of exact C is exact C; R2, equivalent exact twins are
//! one member of one translation unit; R3, instanced sources and the headers
//! they include spell no address names.
use crate::compiler::overlay::{load, RESOURCE_BASE, RUNTIME_BASE};
use crate::compiler::source_inputs::quoted_include;
use crate::compiler::source_paths::{SourceOwner, SourcePaths};
use crate::compiler::translation_units::TranslationUnits;
use crate::coverage::model::{normalize, Span};
use crate::coverage::pipeline::{classify, BuildOptions};
use psynergy::compare::twins::{self, Body, Image, Index, Reference, Site};
use psynergy::decode::MAIN_BASE;
use serde_json::{json, Value};
use std::collections::{BTreeMap, BTreeSet, HashMap, HashSet};
use std::path::{Path, PathBuf};
use std::process::ExitCode;

const TARGET: &str = "tbs-en";
const SHIFT: u32 = RUNTIME_BASE - RESOURCE_BASE;
/// Other editions whose twins are listed as leads, never checked.
const LEADS: [&str; 2] = ["tbs-ja", "tla-en"];
const LEAD_LIMIT: usize = 400;
const INVENTORY: &str = "out/tbs-en/full/rebuilt.owner-inventory.json";
const USAGE: &str = "usage: alchemy check siblings [--report FILE]";

/// One ROM's main program and its code overlays as they execute: the
/// resources closing the directory whose decoded bytes open with a veneer,
/// loaded above the main program from 0x02008000.
struct Edition {
    target: String,
    names: Vec<String>,
    bytes: Vec<Vec<u8>>,
}
impl Edition {
    fn load(root: &Path, target: &str) -> Result<Self, String> {
        let path = root.join("roms").join(format!("{target}.gba"));
        let rom = std::fs::read(&path).map_err(|error| format!("{}: {error}", path.display()))?;
        Self::from_rom(target, rom)
    }
    fn from_rom(target: &str, rom: Vec<u8>) -> Result<Self, String> {
        let table = crate::overlay::rom::resource_table(&rom)?;
        let base = MAIN_BASE as usize;
        let count = (0..)
            .take_while(|resource: &usize| {
                rom.get(table + resource * 4..table + resource * 4 + 4)
                    .map(|word| u32::from_le_bytes(word.try_into().unwrap()) as usize)
                    .is_some_and(|address| address >= base && address - base < rom.len())
            })
            .count();
        let mut overlays = Vec::new();
        for resource in (0..count).rev() {
            let name = format!("resource_{resource:03x}");
            match crate::overlay::rom::decode_overlay(&rom, table, &name) {
                Ok(bytes) if psynergy::thumb::veneer_target(&bytes).is_some() => {
                    overlays.push((name, load(&bytes[..bytes.len() & !1], 0)?));
                }
                _ => break,
            }
        }
        let mut edition = Self {
            target: target.into(),
            names: vec!["main".into()],
            bytes: vec![rom],
        };
        for (name, bytes) in overlays.into_iter().rev() {
            edition.names.push(name);
            edition.bytes.push(bytes);
        }
        Ok(edition)
    }
    fn images(&self) -> Vec<Image<'_>> {
        self.bytes
            .iter()
            .enumerate()
            .map(|(index, bytes)| Image {
                bytes,
                base: if index == 0 { MAIN_BASE } else { RUNTIME_BASE },
                beneath: (index != 0).then_some(0),
            })
            .collect()
    }
    /// The runtime site of an owner in this edition.
    fn site(&self, owner: SourceOwner) -> Result<Site, String> {
        match owner.overlay_id() {
            None => Ok((0, owner.address())),
            Some(overlay) => self
                .names
                .iter()
                .position(|name| *name == overlay)
                .map(|image| (image, owner.address() + SHIFT))
                .ok_or_else(|| {
                    format!(
                        "{}: {overlay} is not a code overlay of {}",
                        owner.id(),
                        self.target
                    )
                }),
        }
    }
    /// A site's owner identity: main addresses, or overlay resource coordinates.
    fn id(&self, site: Site) -> String {
        match site.0 {
            0 => format!("main:{:08x}", site.1),
            image => format!("{}:{:08x}", self.names[image], site.1 - SHIFT),
        }
    }
}

/// A registered owner of the production build.
#[derive(Clone, Debug)]
struct Owner {
    site: Site,
    name: String,
    extent: usize,
    exact: bool,
    source: String,
}

/// One image's audited executable bytes as coverage classifies them, in
/// runtime addresses. Every list is sorted and disjoint.
#[derive(Clone, Debug, Default)]
struct Spans {
    executable: Vec<Span>,
    exact: Vec<Span>,
    credited: Vec<Span>,
    draft: Vec<Span>,
    withdrawn: Vec<Span>,
}

/// Where a place stands in production. Patterns and data are not function
/// sites: they lie inside or across exact C, off a word boundary, without
/// boundary evidence, or outside the audited executable bytes.
#[derive(Clone, Copy, Debug, PartialEq, Eq, PartialOrd, Ord)]
enum Status {
    Exact,
    Credited,
    Draft,
    Withdrawn,
    Unknown,
    Pattern,
    Data,
}
impl Status {
    fn label(self) -> &'static str {
        match self {
            Self::Exact => "exact-c",
            Self::Credited => "credited-asm",
            Self::Draft => "draft-c",
            Self::Withdrawn => "withdrawn-asm",
            Self::Unknown => "unknown",
            Self::Pattern => "pattern",
            Self::Data => "data",
        }
    }
    fn function(self) -> bool {
        !matches!(self, Self::Pattern | Self::Data)
    }
    fn recoverable(self) -> bool {
        self.function() && self != Self::Exact
    }
}

/// The TBS English images with everything the rules read about them.
struct Model<'a> {
    root: PathBuf,
    edition: &'a Edition,
    images: Vec<Image<'a>>,
    owners: Vec<Owner>,
    spans: Vec<Spans>,
    /// Starts the owner register records.
    registered: HashSet<Site>,
    /// Translation-unit members as the manifest declares them: unit and name.
    declared: HashMap<Site, (String, String)>,
    /// Reviewed complete extents of owners the inventory does not list.
    reviewed: HashMap<Site, usize>,
    at: HashMap<Site, usize>,
    lengths: HashMap<Site, usize>,
    ends: HashSet<Site>,
    members: HashMap<(String, usize), Vec<Site>>,
}

fn read_json(path: &Path) -> Result<Value, String> {
    crate::compiler::build_io::read_json(path)
}

fn text<'v>(value: &'v Value, key: &str) -> &'v str {
    value.get(key).and_then(Value::as_str).unwrap_or("")
}

/// Bytes of `spans` inside `span`; `spans` is sorted and disjoint.
fn overlap(spans: &[Span], span: Span) -> i64 {
    let first = spans.partition_point(|other| other.end <= span.start);
    spans[first..]
        .iter()
        .take_while(|other| other.start < span.end)
        .map(|other| other.end.min(span.end) - other.start.max(span.start))
        .sum()
}

impl<'a> Model<'a> {
    fn new(
        root: &Path,
        edition: &'a Edition,
        owners: Vec<Owner>,
        spans: Vec<Spans>,
        registered: HashSet<Site>,
        declared: HashMap<Site, (String, String)>,
        reviewed: HashMap<Site, usize>,
    ) -> Self {
        let images = edition.images();
        let at = owners
            .iter()
            .enumerate()
            .map(|(index, owner)| (owner.site, index))
            .collect();
        let mut lengths = HashMap::new();
        let mut ends = HashSet::new();
        for owner in owners.iter().filter(|owner| owner.exact) {
            if let Some(body) = Body::read(&images, owner.site.0, owner.site.1, owner.extent) {
                lengths.insert(owner.site, body.len());
            }
            let end = owner.site.1 + owner.extent as u32;
            ends.insert((owner.site.0, end));
            ends.insert((owner.site.0, (end + 3) & !3));
        }
        let mut members: HashMap<(String, usize), Vec<Site>> = HashMap::new();
        for (site, (unit, _)) in &declared {
            members
                .entry((unit.clone(), site.0))
                .or_default()
                .push(*site);
        }
        members.values_mut().for_each(|sites| sites.sort_unstable());
        Self {
            root: root.to_path_buf(),
            edition,
            images,
            owners,
            spans,
            registered,
            declared,
            reviewed,
            at,
            lengths,
            ends,
            members,
        }
    }

    /// The production model: the owner inventory of the last full build,
    /// coverage's classification, the owner register and the unit manifest.
    fn load(root: &Path, edition: &'a Edition) -> Result<Self, String> {
        let names = SourcePaths::load(root)?;
        let site = |owner: SourceOwner| edition.site(owner);
        let inventory = read_json(&root.join(INVENTORY))
            .map_err(|error| format!("{error}; run make build-full for the owner inventory"))?;
        let mut owners = Vec::new();
        for record in inventory["owners"]
            .as_array()
            .ok_or("owner inventory has no owners")?
        {
            let production = &record["production"];
            owners.push(Owner {
                site: site(SourceOwner::parse(text(record, "id"))?)?,
                name: text(record, "name").into(),
                extent: production["extent_bytes"].as_u64().unwrap_or(0) as usize,
                exact: text(production, "state") == "exact-c",
                source: text(production, "source").into(),
            });
        }
        let tree = crate::coverage::tree::work_tree_at(root.to_path_buf());
        let found = classify(&BuildOptions {
            target: TARGET.into(),
            exact: &tree,
            recon: Some(&tree),
        })?;
        let audited = found.overlay_exec.keys().cloned().collect::<Vec<_>>();
        if audited != edition.names[1..] {
            return Err(format!(
                "the ROM directory's {} code overlays are not the {} audited overlays",
                edition.names.len() - 1,
                audited.len()
            ));
        }
        let moved = |spans: &[Span], shift: u32| {
            normalize(
                &spans
                    .iter()
                    .map(|span| {
                        Span::new(span.start + i64::from(shift), span.end + i64::from(shift))
                    })
                    .collect::<Vec<_>>(),
            )
        };
        let mapped = |map: &BTreeMap<String, Vec<Span>>, name: &str| {
            moved(map.get(name).map(Vec::as_slice).unwrap_or(&[]), SHIFT)
        };
        let mut spans = vec![Spans {
            executable: moved(&found.main_exec, 0),
            exact: moved(&found.exact_main, 0),
            credited: moved(&found.retained_main, 0),
            draft: moved(&found.semantic_main, 0),
            withdrawn: moved(
                &[found.withdrawn_main, found.withdrawn_draft_main].concat(),
                0,
            ),
        }];
        for name in &audited {
            spans.push(Spans {
                executable: mapped(&found.overlay_exec, name),
                exact: mapped(&found.exact_overlay, name),
                credited: mapped(&found.retained_overlay, name),
                draft: mapped(&found.semantic_overlay, name),
                withdrawn: normalize(
                    &[
                        mapped(&found.withdrawn_overlay, name),
                        mapped(&found.withdrawn_draft_overlay, name),
                    ]
                    .concat(),
                ),
            });
        }
        let registered = names
            .registered_owners()
            .filter_map(|owner| site(owner).ok())
            .collect();
        let mut declared = HashMap::new();
        for unit in TranslationUnits::declared(root)?.units {
            if unit.game != "tbs" {
                continue;
            }
            for owner in &unit.owners {
                let owner = unit.source_owner(unit.image(), owner.address)?;
                let name = names.registered_name(owner).unwrap_or_default().to_string();
                declared.insert(site(owner)?, (unit.id.clone(), name));
            }
            for (image, instance) in &unit.instances {
                for (name, placed) in &instance.owners {
                    let owner = unit.source_owner(image, placed.address)?;
                    declared.insert(site(owner)?, (unit.id.clone(), name.clone()));
                }
            }
        }
        let reviewed = crate::overlay::owner_spans(root)?
            .into_iter()
            .filter_map(|(owner, extent)| Some((site(owner).ok()?, extent)))
            .collect();
        Ok(Self::new(
            root, edition, owners, spans, registered, declared, reviewed,
        ))
    }

    fn owner(&self, site: Site) -> Option<&Owner> {
        self.at.get(&site).map(|index| &self.owners[*index])
    }

    /// A place's production status when `length` bytes start there.
    fn status(&self, site: Site, length: usize) -> Status {
        if self.lengths.get(&site) == Some(&length) {
            return Status::Exact;
        }
        let spans = &self.spans[site.0];
        let span = Span::new(i64::from(site.1), i64::from(site.1) + length as i64);
        if overlap(&spans.executable, span) != span.bytes() {
            return Status::Data;
        }
        let image = &self.images[site.0];
        let prologue = image.bytes.get((site.1 - image.base) as usize + 1) == Some(&0xb5);
        if overlap(&spans.exact, span) > 0
            || site.1 % 4 != 0
            || !(prologue || self.registered.contains(&site) || self.ends.contains(&site))
        {
            return Status::Pattern;
        }
        [
            (&spans.credited, Status::Credited),
            (&spans.draft, Status::Draft),
            (&spans.withdrawn, Status::Withdrawn),
        ]
        .into_iter()
        .find(|(spans, _)| overlap(spans, span) > 0)
        .map_or(Status::Unknown, |(_, status)| status)
    }

    /// The source that builds a place today: its owner's, a draft beside the
    /// retained assembly, or the overlay listing that assembles it.
    fn source(&self, site: Site) -> String {
        if let Some(owner) = self.owner(site) {
            return owner.source.clone();
        }
        let owner = SourceOwner::parse(&self.edition.id(site)).expect("edition ids parse");
        let draft = match owner {
            SourceOwner::Main(address) => format!("en/main/{address:08x}.c"),
            _ => format!("en/overlays/{}.c", owner.legacy_stem()),
        };
        let draft = format!("recon/tbs/{draft}");
        match owner.overlay_id() {
            _ if self.root.join(&draft).is_file() => draft,
            Some(overlay) => format!("recon/tbs/raw/overlays/{overlay}_overlay.s"),
            None => String::new(),
        }
    }

    /// Whether two places start the same body: a known complete extent at
    /// one of them, read at the other.
    fn same_body(&self, left: Site, right: Site) -> bool {
        let extent = |site: Site| {
            self.owner(site)
                .map(|owner| owner.extent)
                .or_else(|| self.reviewed.get(&site).copied())
        };
        let (known, other, extent) = match (extent(left), extent(right)) {
            (Some(extent), _) => (left, right, extent),
            (None, Some(extent)) => (right, left, extent),
            _ => return false,
        };
        let body = Body::read(&self.images, known.0, known.1, extent)
            .or_else(|| Body::at(&self.images, known, extent));
        body.is_some_and(|body| {
            Body::at(&self.images, other, body.len())
                .is_some_and(|found| found.signature == body.signature)
        })
    }

    fn references(&self, site: Site, length: usize) -> Vec<Reference> {
        twins::references(&self.images, site, length)
    }

    /// Binding equivalence of two twins with their references.
    fn binds(&self, left: (Site, &[Reference]), right: (Site, &[Reference])) -> bool {
        twins::equivalent(
            &self.images,
            (left.0 .0, left.1),
            (right.0 .0, right.1),
            |x, y| self.same_body(x, y),
        )
    }

    fn same_member(&self, left: Site, right: Site) -> bool {
        matches!((self.declared.get(&left), self.declared.get(&right)), (Some(a), Some(b)) if a == b)
    }

    /// A variant module: each twin belongs to a different multi-owner unit,
    /// and the members beside each one are not twins at the same distance.
    fn variant(&self, left: Site, right: Site) -> bool {
        let divergent = |site: Site, other: Site| {
            let Some((unit, _)) = self.declared.get(&site) else {
                return false;
            };
            let Some(members) = self.members.get(&(unit.clone(), site.0)) else {
                return false;
            };
            let Some(position) = members.iter().position(|member| *member == site) else {
                return false;
            };
            let delta = i64::from(other.1) - i64::from(site.1);
            members.len() > 1
                && [position.checked_sub(1), Some(position + 1)]
                    .into_iter()
                    .flatten()
                    .filter_map(|index| members.get(index))
                    .all(|member| {
                        let moved = (other.0, (i64::from(member.1) + delta) as u32);
                        !self.same_body(*member, moved)
                    })
        };
        !self.same_member(left, right)
            && self.declared.get(&left).map(|found| &found.0)
                != self.declared.get(&right).map(|found| &found.0)
            && divergent(left, right)
            && divergent(right, left)
    }
}

/// Exact owners grouped by signature, with every place their body occurs.
struct Family {
    owners: Vec<usize>,
    body: Body,
    sites: Vec<(Site, Status)>,
}
impl Family {
    fn with(&self, status: impl Fn(Status) -> bool) -> Vec<Site> {
        self.sites
            .iter()
            .filter(|(_, found)| status(*found))
            .map(|(site, _)| *site)
            .collect()
    }
}

/// A near twin accepted as a lead for its closest family.
struct Near {
    family: usize,
    site: Site,
    differing: usize,
    status: Status,
}

struct Census {
    families: Vec<Family>,
    exact: usize,
    below_minimum: usize,
    self_matched: usize,
    near: Vec<Near>,
    near_exact_pairs: usize,
    near_complete: usize,
    near_partial: usize,
}

/// `work` over every item on all cores, results in item order.
fn parallel<T: Send, R: Send>(items: Vec<T>, work: impl Fn(T) -> R + Sync) -> Vec<R> {
    let threads = std::thread::available_parallelism().map_or(1, |count| count.get());
    let size = items.len().div_ceil(threads).max(1);
    let mut chunks = Vec::new();
    let mut items = items.into_iter();
    loop {
        let chunk = items.by_ref().take(size).collect::<Vec<_>>();
        if chunk.is_empty() {
            break;
        }
        chunks.push(chunk);
    }
    let work = &work;
    std::thread::scope(|scope| {
        chunks
            .into_iter()
            .map(|chunk| scope.spawn(move || chunk.into_iter().map(work).collect::<Vec<_>>()))
            .collect::<Vec<_>>()
            .into_iter()
            .flat_map(|handle| handle.join().expect("sibling search worker panicked"))
            .collect()
    })
}

impl Model<'_> {
    /// Every exact owner's twins, and with `near` the near tier.
    fn census(&self, near: bool) -> Census {
        let index = Index::new(&self.images);
        let mut families: Vec<Family> = Vec::new();
        let mut groups = HashMap::new();
        let mut exact = self
            .owners
            .iter()
            .enumerate()
            .filter(|(_, owner)| owner.exact)
            .collect::<Vec<_>>();
        exact.sort_unstable_by_key(|(_, owner)| owner.site);
        let mut below_minimum = 0;
        for (index, owner) in &exact {
            let Some(body) = Body::read(&self.images, owner.site.0, owner.site.1, owner.extent)
            else {
                below_minimum += 1;
                continue;
            };
            let family = *groups.entry(body.signature.clone()).or_insert_with(|| {
                families.push(Family {
                    owners: Vec::new(),
                    body,
                    sites: Vec::new(),
                });
                families.len() - 1
            });
            families[family].owners.push(*index);
        }
        let found = parallel(
            families.iter().map(|family| &family.body).collect(),
            |body| {
                index
                    .sites(&self.images, body)
                    .into_iter()
                    .map(|site| (site, self.status(site, body.len())))
                    .collect::<Vec<_>>()
            },
        );
        for (family, sites) in families.iter_mut().zip(found) {
            family.sites = sites;
        }
        let self_matched = families
            .iter()
            .flat_map(|family| {
                family.owners.iter().filter(|owner| {
                    family
                        .sites
                        .contains(&(self.owners[**owner].site, Status::Exact))
                })
            })
            .count();
        let mut census = Census {
            exact: exact.len(),
            below_minimum,
            self_matched,
            families,
            near: Vec::new(),
            near_exact_pairs: 0,
            near_complete: 0,
            near_partial: 0,
        };
        if near {
            self.near_tier(&index, &mut census);
        }
        census
    }

    fn near_tier(&self, index: &Index, census: &mut Census) {
        let found = parallel(
            census.families.iter().map(|family| &family.body).collect(),
            |body| index.near(&self.images, body),
        );
        let masked = census
            .families
            .iter()
            .flat_map(|family| family.sites.iter().map(|(site, _)| *site))
            .collect::<HashSet<_>>();
        let family_of = census
            .families
            .iter()
            .enumerate()
            .flat_map(|(family, found)| found.owners.iter().map(move |owner| (*owner, family)))
            .collect::<HashMap<_, _>>();
        let mut best: BTreeMap<Site, Near> = BTreeMap::new();
        let mut pairs = BTreeSet::new();
        for (family, (sites, complete)) in found.into_iter().enumerate() {
            let body = &census.families[family].body;
            if body.len() >= twins::NEAR_MINIMUM_BODY {
                let owners = census.families[family].owners.len();
                match complete {
                    true => census.near_complete += owners,
                    false => census.near_partial += owners,
                }
            }
            for (site, differing) in sites {
                let status = self.status(site, body.len());
                if status == Status::Exact {
                    if let Some(other) = family_of.get(&self.at[&site]) {
                        pairs.insert((family.min(*other), family.max(*other)));
                    }
                    continue;
                }
                if masked.contains(&site) {
                    continue;
                }
                let prologue =
                    |site: Site| Body::at(&self.images, site, 2).map(|found| found.signature);
                if !status.function()
                    || !self.registered.contains(&site)
                        && prologue(site) != prologue((body.image, body.start))
                {
                    continue;
                }
                if best
                    .get(&site)
                    .is_none_or(|near| differing < near.differing)
                {
                    best.insert(
                        site,
                        Near {
                            family,
                            site,
                            differing,
                            status,
                        },
                    );
                }
            }
        }
        census.near = best.into_values().collect();
        census.near_exact_pairs = pairs.len();
    }
}

/// How a family's places bind, pair by pair: all values equal; differences
/// only between two overlays, each pointing into its own; or anything else.
#[derive(Clone, Copy, Debug, PartialEq, Eq, PartialOrd, Ord)]
enum Merge {
    Identical,
    OverlayLocal,
    Different,
}
impl Merge {
    fn label(self) -> &'static str {
        match self {
            Self::Identical => "identical-bindings",
            Self::OverlayLocal => "overlay-local-bindings",
            Self::Different => "different-globals-or-callees",
        }
    }
}

/// What the rules find in one family.
struct Findings {
    /// Function places equivalent to another exact place of the family.
    equivalent: HashSet<Site>,
    /// R1: recoverable places equivalent to an exact twin.
    missed: Vec<Site>,
    /// R2: exact places equivalent to an exact twin of a different member.
    copies: Vec<Site>,
    merge: Merge,
}

impl Model<'_> {
    fn findings(&self, family: &Family) -> Findings {
        let length = family.body.len();
        let sites = family.with(Status::function);
        let exact = family.with(|status| status == Status::Exact);
        let references = sites
            .iter()
            .map(|site| (*site, self.references(*site, length)))
            .collect::<HashMap<_, _>>();
        let mut findings = Findings {
            equivalent: HashSet::new(),
            missed: Vec::new(),
            copies: Vec::new(),
            merge: self.merge(&sites, &references),
        };
        for site in &sites {
            let mut missed = false;
            let mut copied = false;
            for other in exact.iter().filter(|other| *other != site) {
                if !self.binds((*site, &references[site]), (*other, &references[other])) {
                    continue;
                }
                findings.equivalent.insert(*site);
                if !exact.contains(site) {
                    missed = true;
                } else if !self.same_member(*site, *other) && !self.variant(*site, *other) {
                    copied = true;
                }
            }
            if missed {
                findings.missed.push(*site);
            }
            if copied {
                findings.copies.push(*site);
            }
        }
        findings
    }

    fn merge(&self, sites: &[Site], references: &HashMap<Site, Vec<Reference>>) -> Merge {
        let value = |reference: &Reference| match reference {
            Reference::Call(_, value) | Reference::Literal(value) => *value,
        };
        let mut merge = Merge::Identical;
        for (index, left) in sites.iter().enumerate() {
            for right in &sites[index + 1..] {
                for (x, y) in references[left].iter().zip(&references[right]) {
                    let (x, y) = (value(x), value(y));
                    if x == y {
                        continue;
                    }
                    let local = left.0 != right.0
                        && left.0 != 0
                        && right.0 != 0
                        && self.images[left.0].contains(x)
                        && self.images[right.0].contains(y);
                    merge = merge.max(if local {
                        Merge::OverlayLocal
                    } else {
                        Merge::Different
                    });
                }
            }
        }
        merge
    }

    fn family_name(&self, family: &Family) -> String {
        let owner = &self.owners[family.owners[0]];
        format!("{}@{}", owner.name, self.edition.id(owner.site))
    }

    /// Every function twin of `body` with its status and binding equivalence.
    fn twins_of(&self, body: &Body) -> Vec<Twin> {
        let (site, length) = ((body.image, body.start), body.len());
        let references = self.references(site, length);
        twins::find(&self.images, body)
            .into_iter()
            .filter(|twin| *twin != site)
            .filter_map(|twin| {
                let status = self.status(twin, length);
                status.function().then(|| Twin {
                    site: twin,
                    status,
                    binds: self.binds((site, &references), (twin, &self.references(twin, length))),
                })
            })
            .collect()
    }

    /// The member an owner's equivalent twins join: the owner's own, else the
    /// first declared equivalent twin's.
    fn home(&self, owner: Site, twins: &[Twin]) -> Option<&(String, String)> {
        self.declared.get(&owner).or_else(|| {
            twins
                .iter()
                .filter(|twin| twin.binds)
                .find_map(|twin| self.declared.get(&twin.site))
        })
    }

    /// The instance entry that makes `place` the `home` member.
    fn needs(&self, home: Option<&(String, String)>, place: Site, length: usize) -> Option<String> {
        let Some((unit, name)) = home else {
            return Some("one translation unit linking every equivalent twin".into());
        };
        if self.declared.get(&place) == home {
            return None;
        }
        let image = &self.edition.names[place.0];
        let address = SourceOwner::parse(&self.edition.id(place)).map_or(0, SourceOwner::address);
        // An overlay's placeholder bounds the member there; only the main
        // image declares an extent.
        let extent = match image.as_str() {
            "main" => {
                let extent = self
                    .reviewed
                    .get(&place)
                    .copied()
                    .or_else(|| self.owner(place).map(|found| found.extent))
                    .unwrap_or(length);
                format!(",\"extent\":{extent}")
            }
            _ => String::new(),
        };
        Some(format!(
            "{unit} instances.{image}.owners.{name}={{\"address\":\"0x{address:08x}\"{extent}}}"
        ))
    }

    /// The adoption hook for owners about to become exact C, each with its
    /// complete extent: every twin is listed, and the hook refuses when an
    /// equivalent twin would stay outside exact C (R1) or an equivalent exact
    /// twin is another member (R2). Declared members and owners installed
    /// together count as exact.
    fn guard(&self, installing: &[(Site, usize)]) -> Result<Vec<String>, String> {
        let pending = installing
            .iter()
            .map(|(site, _)| *site)
            .collect::<HashSet<_>>();
        let mut lines = Vec::new();
        let mut refusals = Vec::new();
        for (site, extent) in installing {
            let Some(body) = Body::read(&self.images, site.0, site.1, *extent) else {
                continue;
            };
            let found = self.twins_of(&body);
            let home = self.home(*site, &found);
            let owner = self.edition.id(*site);
            for twin in &found {
                let other = self.edition.id(twin.site);
                lines.push(format!(
                    "siblings {owner} twin={other} status={} equivalent={}",
                    twin.status.label(),
                    if twin.binds { "yes" } else { "no" }
                ));
                let exact = twin.status == Status::Exact
                    || pending.contains(&twin.site)
                    || self.declared.contains_key(&twin.site);
                let needs = |place| self.needs(home, place, body.len()).unwrap_or_default();
                if !twin.binds {
                    continue;
                } else if !exact {
                    refusals.push(format!(
                        "R1: equivalent twin {other} would stay {}; needs {}",
                        twin.status.label(),
                        needs(twin.site)
                    ));
                } else if !self.same_member(*site, twin.site) && !self.variant(*site, twin.site) {
                    let place = if self.declared.get(site) == home {
                        twin.site
                    } else {
                        *site
                    };
                    refusals.push(format!(
                        "R2: {owner} and exact twin {other} would be two sources; needs {}",
                        needs(place)
                    ));
                }
            }
        }
        if refusals.is_empty() {
            return Ok(lines);
        }
        lines.extend(refusals);
        lines.push(format!(
            "adoption refused by the sibling rules; statuses come from {INVENTORY} (make build-full refreshes it)"
        ));
        Err(lines.join("\n"))
    }
}

/// A function twin of one owner.
struct Twin {
    site: Site,
    status: Status,
    binds: bool,
}

/// R3: the sources of instanced units, and every header they include, spell
/// no `Func_`, `Data_` or `Value_` address names, `#define` aliases included.
fn address_names(root: &Path, units: &TranslationUnits) -> Result<Vec<String>, String> {
    let names = regex::Regex::new(r"\b(?:Func|Data|Value)_[0-9A-Fa-f]{8}\b")
        .map_err(|error| error.to_string())?;
    let comments =
        regex::Regex::new(r"(?s)/\*.*?\*/|//[^\n]*").map_err(|error| error.to_string())?;
    let mut seen = BTreeSet::new();
    let mut found = Vec::new();
    let mut pending = units
        .units
        .iter()
        .filter(|unit| !unit.instances.is_empty())
        .map(|unit| {
            let game = crate::compiler::routing::game_directory(&unit.game);
            (
                root.join(&unit.source),
                root.join("games").join(game).join("INCLUDE"),
            )
        })
        .collect::<Vec<_>>();
    while let Some((path, include)) = pending.pop() {
        if !seen.insert(path.clone()) {
            continue;
        }
        let text = std::fs::read_to_string(&path)
            .map_err(|error| format!("{}: {error}", path.display()))?;
        let code = comments.replace_all(&text, |comment: &regex::Captures| {
            comment[0]
                .chars()
                .map(|c| if c == '\n' { '\n' } else { ' ' })
                .collect::<String>()
        });
        let shown = path
            .strip_prefix(root)
            .unwrap_or(&path)
            .display()
            .to_string();
        for (number, line) in code.lines().enumerate() {
            for name in names.find_iter(line) {
                found.push(format!("{shown}:{}: {}", number + 1, name.as_str()));
            }
            if let Some(header) = quoted_include(line) {
                let beside = path.parent().map(|directory| directory.join(header));
                let header = beside
                    .filter(|path| path.is_file())
                    .unwrap_or_else(|| include.join(header));
                if header.is_file() {
                    pending.push((header, include.clone()));
                }
            }
        }
    }
    found.sort();
    Ok(found)
}

/// Count one place of `bytes` under `key`.
fn tally<K: Ord>(map: &mut BTreeMap<K, (usize, usize)>, key: K, bytes: usize) {
    let entry = map.entry(key).or_default();
    *entry = (entry.0 + 1, entry.1 + bytes);
}

/// `label=places/bytes` for each key.
fn tallies<K>(map: &BTreeMap<K, (usize, usize)>, label: fn(&K) -> &'static str) -> String {
    map.iter()
        .map(|(key, (places, bytes))| format!(" {}={places}/{bytes}", label(key)))
        .collect()
}

fn tallies_json<K>(map: &BTreeMap<K, (usize, usize)>, label: fn(&K) -> &'static str) -> Value {
    map.iter()
        .map(|(key, (places, bytes))| {
            (
                label(key).to_string(),
                json!({"sites": places, "bytes": bytes}),
            )
        })
        .collect::<serde_json::Map<_, _>>()
        .into()
}

/// What `check siblings` prints and its report records about one census.
struct Totals {
    multi_site: usize,
    duplicates: Vec<usize>,
    candidates: Vec<usize>,
    near: BTreeMap<usize, Vec<usize>>,
    duplicate_sites: usize,
    redundant_bytes: usize,
    merges: BTreeMap<Merge, (usize, usize)>,
    recoverable: BTreeMap<Status, (usize, usize)>,
    near_statuses: BTreeMap<Status, (usize, usize)>,
    excluded: BTreeMap<Status, (usize, usize)>,
    /// R1 and R2 places, then families with any.
    missed: [usize; 2],
    copies: [usize; 2],
}
impl Totals {
    fn new(census: &Census, findings: &[Findings]) -> Self {
        let mut totals = Self {
            multi_site: 0,
            duplicates: Vec::new(),
            candidates: Vec::new(),
            near: BTreeMap::new(),
            duplicate_sites: 0,
            redundant_bytes: 0,
            merges: BTreeMap::new(),
            recoverable: BTreeMap::new(),
            near_statuses: BTreeMap::new(),
            excluded: BTreeMap::new(),
            missed: [0; 2],
            copies: [0; 2],
        };
        for (index, (family, found)) in census.families.iter().zip(findings).enumerate() {
            let length = family.body.len();
            for (_, status) in family.sites.iter().filter(|(_, status)| !status.function()) {
                tally(&mut totals.excluded, *status, length);
            }
            for (sum, places) in [
                (&mut totals.missed, &found.missed),
                (&mut totals.copies, &found.copies),
            ] {
                *sum = [
                    sum[0] + places.len(),
                    sum[1] + usize::from(!places.is_empty()),
                ];
            }
            let sites = family.with(Status::function).len();
            let exact = family.with(|status| status == Status::Exact).len();
            if sites < 2 {
                continue;
            }
            totals.multi_site += 1;
            if exact == sites {
                totals.duplicates.push(index);
                totals.duplicate_sites += sites;
                totals.redundant_bytes += (sites - 1) * length;
                let entry = totals.merges.entry(found.merge).or_default();
                *entry = (entry.0 + 1, entry.1 + (sites - 1) * length);
            } else if exact > 0 {
                totals.candidates.push(index);
                for (_, status) in family
                    .sites
                    .iter()
                    .filter(|(_, status)| status.recoverable())
                {
                    tally(&mut totals.recoverable, *status, length);
                }
            }
        }
        for (index, near) in census.near.iter().enumerate() {
            totals.near.entry(near.family).or_default().push(index);
            tally(
                &mut totals.near_statuses,
                near.status,
                census.families[near.family].body.len(),
            );
        }
        totals
    }

    fn lines(&self, census: &Census, near: bool) -> Vec<String> {
        let mut lines = vec![
            format!(
                "siblings owners exact={} below_minimum={} scanned={} self_matched={}",
                census.exact,
                census.below_minimum,
                census.exact - census.below_minimum,
                census.self_matched
            ),
            format!(
                "siblings families={} multi_site={} duplicates={} candidates={} excluded_sites{}",
                census.families.len(),
                self.multi_site,
                self.duplicates.len(),
                self.candidates.len(),
                tallies(&self.excluded, |status| status.label())
            ),
            format!(
                "siblings duplicates sites={} redundant_bytes={}{}",
                self.duplicate_sites,
                self.redundant_bytes,
                tallies(&self.merges, |merge| merge.label())
            ),
            format!(
                "siblings candidates{}",
                tallies(&self.recoverable, |status| status.label())
            ),
        ];
        if near {
            lines.push(format!(
                "siblings near families={}{} exact_family_pairs={} complete_owners={} partial_owners={}",
                self.near.len(),
                tallies(&self.near_statuses, |status| status.label()),
                census.near_exact_pairs,
                census.near_complete,
                census.near_partial
            ));
        }
        lines.push(format!(
            "siblings R1 report-only sites={} families={}",
            self.missed[0], self.missed[1]
        ));
        lines.push(format!(
            "siblings R2 report-only sites={} families={}",
            self.copies[0], self.copies[1]
        ));
        lines
    }
}

fn write_json(path: &Path, document: &Value) -> Result<(), String> {
    if let Some(parent) = path
        .parent()
        .filter(|parent| !parent.as_os_str().is_empty())
    {
        std::fs::create_dir_all(parent)
            .map_err(|error| format!("{}: {error}", parent.display()))?;
    }
    let mut text = serde_json::to_string_pretty(document).map_err(|error| error.to_string())?;
    text.push('\n');
    std::fs::write(path, text).map_err(|error| format!("{}: {error}", path.display()))
}

/// Report-only twins in the other editions: per edition, the families found
/// there among all families and among the listed ones, and each listed
/// family's sites.
fn leads(
    editions: &[(&str, Option<Edition>)],
    census: &Census,
    listed: &BTreeSet<usize>,
) -> (Value, BTreeMap<usize, serde_json::Map<String, Value>>) {
    let mut summary = serde_json::Map::new();
    let mut families = BTreeMap::<usize, serde_json::Map<String, Value>>::new();
    for (target, edition) in editions {
        let target = *target;
        let Some(edition) = edition else {
            summary.insert(target.into(), json!({"available": false}));
            println!("siblings leads {target} unavailable");
            continue;
        };
        let images = edition.images();
        let index = Index::new(&images);
        let found = parallel((0..census.families.len()).collect(), |family| {
            index.sites(&images, &census.families[family].body)
        });
        let mut counts = [[0usize; 3]; 2];
        for (family, sites) in found
            .iter()
            .enumerate()
            .filter(|(_, sites)| !sites.is_empty())
        {
            let same = sites.len() == census.families[family].with(Status::function).len();
            let listed = listed.contains(&family);
            let scopes = if listed {
                &mut counts[..]
            } else {
                &mut counts[..1]
            };
            for count in scopes {
                *count = [
                    count[0] + 1,
                    count[1] + usize::from(same),
                    count[2] + sites.len(),
                ];
            }
            let owners = sites.iter().take(LEAD_LIMIT).map(|site| edition.id(*site));
            let record = json!({"sites": sites.len(), "owners": owners.collect::<Vec<_>>()});
            families
                .entry(family)
                .or_default()
                .insert(target.into(), record);
        }
        let mut record = json!({"report_only": true, "code_overlays": edition.names.len() - 1});
        for (scope, [matched, same, sites], of) in [
            ("all", counts[0], census.families.len()),
            ("listed", counts[1], listed.len()),
        ] {
            println!(
                "siblings leads {target} {scope} report-only families={matched}/{of} same_site_count={same} sites={sites}"
            );
            record[scope] =
                json!({"families": matched, "of": of, "same_site_count": same, "sites": sites});
        }
        summary.insert(target.into(), record);
    }
    (Value::Object(summary), families)
}

fn run(root: &Path, report: Option<&Path>) -> Result<bool, String> {
    let edition = Edition::load(root, TARGET)?;
    let model = Model::load(root, &edition)?;
    let census = model.census(report.is_some());
    let findings = parallel(census.families.iter().collect(), |family| {
        model.findings(family)
    });
    let totals = Totals::new(&census, &findings);
    let units = TranslationUnits::declared(root)?;
    let spelling = address_names(root, &units)?;
    let instanced = units
        .units
        .iter()
        .filter(|unit| !unit.instances.is_empty())
        .count();
    for line in totals.lines(&census, report.is_some()) {
        println!("{line}");
    }
    for violation in &spelling {
        println!("siblings R3 address name: {violation}");
    }
    println!(
        "siblings R3 enforced instanced_units={instanced} violations={}",
        spelling.len()
    );
    let Some(path) = report else {
        return Ok(spelling.is_empty());
    };
    let listed = totals
        .duplicates
        .iter()
        .chain(&totals.candidates)
        .chain(totals.near.keys());
    let editions = LEADS.map(|target| (target, Edition::load(root, target).ok()));
    let (lead_summary, lead_families) = leads(&editions, &census, &listed.copied().collect());
    let place = |site: Site, status: Status, found: Option<&Findings>| {
        let member = model.declared.get(&site);
        json!({
            "owner": edition.id(site),
            "status": status.label(),
            "source": model.source(site),
            "unit": member.map(|(unit, _)| unit),
            "member": member.map(|(_, name)| name),
            "equivalent": found.map(|found| found.equivalent.contains(&site)),
            "r1": found.map(|found| found.missed.contains(&site)),
            "r2": found.map(|found| found.copies.contains(&site)),
        })
    };
    let family = |index: usize| {
        let family = &census.families[index];
        json!({
            "family": model.family_name(family),
            "body": family.body.len(),
            "merge": findings[index].merge.label(),
            "sites": family.sites.iter().filter(|(_, status)| status.function())
                .map(|(site, status)| place(*site, *status, Some(&findings[index]))).collect::<Vec<_>>(),
            "leads": lead_families.get(&index),
        })
    };
    let document = json!({
        "format": 1,
        "kind": "golden-sun-sibling-report",
        "target": TARGET,
        "minimum_body": twins::MINIMUM_BODY,
        "owners": {
            "exact": census.exact,
            "below_minimum": census.below_minimum,
            "self_matched": census.self_matched,
        },
        "families": {
            "masked_identical": census.families.len(),
            "multi_site": totals.multi_site,
            "duplicates": totals.duplicates.len(),
            "candidates": totals.candidates.len(),
            "near_candidates": totals.near.len(),
            "excluded_sites": tallies_json(&totals.excluded, |status| status.label()),
        },
        "duplicates": {
            "sites": totals.duplicate_sites,
            "redundant_bytes": totals.redundant_bytes,
            "merge": totals.merges.iter().map(|(merge, (families, bytes))| {
                (merge.label().to_string(), json!({"families": families, "bytes": bytes}))
            }).collect::<serde_json::Map<_, _>>(),
        },
        "candidates": tallies_json(&totals.recoverable, |status| status.label()),
        "near": {
            "candidates": tallies_json(&totals.near_statuses, |status| status.label()),
            "exact_family_pairs": census.near_exact_pairs,
            "complete_recall_owners": census.near_complete,
            "partial_recall_owners": census.near_partial,
        },
        "rules": {
            "R1": {"enforced": false, "sites": totals.missed[0], "families": totals.missed[1]},
            "R2": {"enforced": false, "sites": totals.copies[0], "families": totals.copies[1]},
            "R3": {"enforced": true, "instanced_units": instanced, "violations": spelling},
        },
        "leads": lead_summary,
        "tla_lead_families": lead_families.keys()
            .filter(|index| lead_families[index].contains_key("tla-en"))
            .map(|index| family(*index)).collect::<Vec<_>>(),
        "candidate_families": totals.candidates.iter().map(|index| family(*index)).collect::<Vec<_>>(),
        "near_families": totals.near.iter().map(|(index, near)| json!({
            "family": model.family_name(&census.families[*index]),
            "body": census.families[*index].body.len(),
            "sites": near.iter().map(|near| {
                let near = &census.near[*near];
                let mut record = place(near.site, near.status, None);
                record["differing_units"] = near.differing.into();
                record
            }).collect::<Vec<_>>(),
            "leads": lead_families.get(index),
        })).collect::<Vec<_>>(),
        "duplicate_families": totals.duplicates.iter().map(|index| family(*index)).collect::<Vec<_>>(),
    });
    write_json(path, &document)?;
    println!("siblings report: {}", path.display());
    Ok(spelling.is_empty())
}

/// `alchemy check siblings [--report FILE]`.
pub fn check(arguments: &[String]) -> ExitCode {
    let report = match arguments {
        [] => None,
        [flag, path] if flag == "--report" => Some(PathBuf::from(path)),
        [flag] if flag == "-h" || flag == "--help" => {
            println!("{USAGE}");
            return ExitCode::SUCCESS;
        }
        _ => {
            eprintln!("{USAGE}");
            return ExitCode::from(2);
        }
    };
    match run(crate::compiler::routing::root(), report.as_deref()) {
        Ok(true) => {
            println!("siblings check ok: R3 enforced; R1 and R2 report only");
            ExitCode::SUCCESS
        }
        Ok(false) => {
            eprintln!("siblings check failed: instanced sources spell address names (R3)");
            ExitCode::FAILURE
        }
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}

/// `alchemy inspect OWNER --siblings [--near] [--json FILE]`: every function
/// twin with its status, unit, source, equivalence and the instance entry it
/// needs; near twins on request; other editions as report-only leads.
pub fn inspect(root: &Path, owner: &str, near: bool, output: Option<&Path>) -> Result<(), String> {
    let owner = SourceOwner::parse_argument(owner)?;
    let edition = Edition::load(root, TARGET)?;
    let model = Model::load(root, &edition)?;
    let site = edition.site(owner)?;
    let extent = model
        .owner(site)
        .map(|found| found.extent)
        .or_else(|| model.reviewed.get(&site).copied())
        .ok_or_else(|| format!("{}: no registered or reviewed complete extent", owner.id()))?;
    let body = Body::read(&model.images, site.0, site.1, extent).ok_or_else(|| {
        format!(
            "{}: a {extent}-byte extent is below the {}-byte twin minimum",
            owner.id(),
            twins::MINIMUM_BODY
        )
    })?;
    let length = body.len();
    let found = model.twins_of(&body);
    let home = model.home(site, &found);
    println!(
        "owner={} name={} status={} body={length} unit={}{}",
        owner.id(),
        model.owner(site).map_or("-", |found| found.name.as_str()),
        model.status(site, length).label(),
        model
            .declared
            .get(&site)
            .map_or("-", |(unit, _)| unit.as_str()),
        model
            .needs(home, site, length)
            .filter(|_| found.iter().any(|twin| twin.binds))
            .map_or(String::new(), |needs| format!(" needs={needs}"))
    );
    let mut document =
        json!({"owner": owner.id(), "body": length, "twins": [], "near": [], "leads": {}});
    for twin in &found {
        let needs = twin
            .binds
            .then(|| model.needs(home, twin.site, length))
            .flatten();
        let unit = model
            .declared
            .get(&twin.site)
            .map(|(unit, _)| unit.as_str());
        println!(
            "twin {} status={} equivalent={} unit={} source={}{}",
            edition.id(twin.site),
            twin.status.label(),
            if twin.binds { "yes" } else { "no" },
            unit.unwrap_or("-"),
            model.source(twin.site),
            needs
                .as_ref()
                .map_or(String::new(), |needs| format!(" needs={needs}"))
        );
        document["twins"].as_array_mut().unwrap().push(json!({
            "owner": edition.id(twin.site), "status": twin.status.label(), "equivalent": twin.binds,
            "unit": unit, "source": model.source(twin.site), "needs": needs,
        }));
    }
    if near {
        let (found, complete) = Index::new(&model.images).near(&model.images, &body);
        for (twin, differing) in found {
            let status = model.status(twin, length);
            if status.function() {
                println!(
                    "near {} status={} differing_units={differing} source={}",
                    edition.id(twin),
                    status.label(),
                    model.source(twin)
                );
                document["near"].as_array_mut().unwrap().push(json!({
                    "owner": edition.id(twin), "status": status.label(), "differing_units": differing,
                }));
            }
        }
        println!(
            "near recall={}",
            if complete { "complete" } else { "partial" }
        );
    }
    for target in LEADS {
        let Ok(lead) = Edition::load(root, target) else {
            println!("lead {target} unavailable");
            continue;
        };
        let found = twins::find(&lead.images(), &body);
        let owners = found
            .iter()
            .take(LEAD_LIMIT)
            .map(|twin| lead.id(*twin))
            .collect::<Vec<_>>();
        for id in &owners {
            println!("lead {target}:{id} report-only");
        }
        document["leads"][target] = json!(owners);
    }
    println!(
        "twins={} equivalent={} near={}",
        found.len(),
        found.iter().filter(|twin| twin.binds).count(),
        document["near"].as_array().map_or(0, Vec::len)
    );
    output.map_or(Ok(()), |path| write_json(path, &document))
}

/// The adoption hook over the production model; see `Model::guard`.
pub fn guard(root: &Path, installing: &[(SourceOwner, usize)]) -> Result<Vec<String>, String> {
    let edition = Edition::load(root, TARGET)?;
    let model = Model::load(root, &edition)?;
    let installing = installing
        .iter()
        .map(|(owner, extent)| Ok((edition.site(*owner)?, *extent)))
        .collect::<Result<Vec<_>, String>>()?;
    model.guard(&installing)
}

/// Masked-identical twins of one owner's complete extent, for `alchemy score`.
pub fn count(root: &Path, owner: SourceOwner, extent: usize) -> Result<usize, String> {
    let edition = Edition::load(root, TARGET)?;
    let images = edition.images();
    let site = edition.site(owner)?;
    Ok(
        Body::read(&images, site.0, site.1, extent).map_or(0, |body| {
            twins::find(&images, &body).len().saturating_sub(1)
        }),
    )
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::compiler::translation_units::fixture::{staged_actor, Repository, STAGED_ACTOR};

    const TWIN: u32 = 0x0200_8100;
    const IMPORT: u32 = 0x0200_83f8;

    /// A `bl` pair at `site` reaching `target`.
    fn bl(site: u32, target: u32) -> [u8; 4] {
        let value = target.wrapping_sub(site + 4) as i32 >> 1;
        let high = 0xf000 | ((value >> 11) & 0x7ff) as u16;
        let low = 0xf800 | (value & 0x7ff) as u16;
        let [a, b] = high.to_le_bytes();
        let [c, d] = low.to_le_bytes();
        [a, b, c, d]
    }

    /// Twenty bytes at `address`: push, a call, a load of `value`, a return
    /// and its pool word.
    fn function(address: u32, callee: u32, value: u32) -> Vec<u8> {
        let mut bytes = vec![0x30, 0xb5];
        bytes.extend(bl(address + 2, callee));
        bytes.extend([0x02, 0x48, 0x05, 0x1c, 0x07, 0x28, 0x30, 0xbd, 0xc0, 0x46]);
        bytes.extend(value.to_le_bytes());
        bytes
    }

    /// Another shape of twenty bytes, never a twin of `function`; `step`
    /// changes one comparison.
    fn helper(address: u32, callee: u32, value: u32, step: u8) -> Vec<u8> {
        let mut bytes = function(address, callee, value);
        bytes[8] = 0x06;
        bytes[10] = step;
        bytes
    }

    /// resource_380, resource_381, ... from runtime-addressed pieces, each
    /// overlay importing the main function at 0x08000100 through `IMPORT`.
    fn edition(overlays: &[&[(u32, &[u8])]]) -> Edition {
        let mut main = vec![0u8; 0x200];
        main[0x100..0x104].copy_from_slice(&[0x30, 0xb5, 0x30, 0xbd]);
        let mut edition = Edition {
            target: TARGET.into(),
            names: vec!["main".into()],
            bytes: vec![main],
        };
        for (index, pieces) in overlays.iter().enumerate() {
            let mut bytes = vec![0u8; 0x400];
            bytes[0x3f8..].copy_from_slice(&[0x00, 0x4c, 0x20, 0x47, 0x01, 0x01, 0x00, 0x08]);
            for (address, piece) in *pieces {
                let at = (address - RUNTIME_BASE) as usize;
                bytes[at..at + piece.len()].copy_from_slice(piece);
            }
            edition
                .names
                .push(format!("resource_{:03x}", 0x380 + index));
            edition.bytes.push(bytes);
        }
        edition
    }

    /// Registered owners: site, extent, exact, and unit and member if declared.
    type Place<'a> = (Site, usize, bool, Option<(&'a str, &'a str)>);

    fn build<'a>(edition: &'a Edition, places: &[Place]) -> Model<'a> {
        let owners = places
            .iter()
            .map(|(site, extent, exact, _)| Owner {
                site: *site,
                name: "Twin".into(),
                extent: *extent,
                exact: *exact,
                source: String::new(),
            })
            .collect();
        let span = |address: u32, bytes: usize| {
            Span::new(i64::from(address), i64::from(address) + bytes as i64)
        };
        let spans = edition
            .images()
            .iter()
            .enumerate()
            .map(|(image, found)| Spans {
                executable: vec![span(found.base, found.bytes.len())],
                exact: normalize(
                    &places
                        .iter()
                        .filter(|place| place.2 && place.0 .0 == image)
                        .map(|place| span(place.0 .1, place.1))
                        .collect::<Vec<_>>(),
                ),
                ..Spans::default()
            })
            .collect();
        let declared = places
            .iter()
            .filter_map(|place| {
                let (unit, name) = place.3?;
                Some((place.0, (unit.to_string(), name.to_string())))
            })
            .collect();
        let registered = places.iter().map(|place| place.0).collect();
        Model::new(
            Path::new("."),
            edition,
            owners,
            spans,
            registered,
            declared,
            HashMap::new(),
        )
    }

    #[test]
    fn check_siblings_rejects_unadopted_equivalent_twin() {
        let one = function(TWIN, IMPORT, 0x0200_8300);
        let two = function(TWIN, IMPORT, 0x0200_8310);
        let global = function(TWIN, IMPORT, 0x0300_0000);
        let edition = edition(&[&[(TWIN, &one)], &[(TWIN, &two)], &[(TWIN, &global)]]);
        let model = build(&edition, &[((1, TWIN), 20, true, None)]);
        let census = model.census(false);
        assert_eq!(census.families.len(), 1);
        let found = model.findings(&census.families[0]);
        // Tables in its own image may differ; a global the exact owner does not
        // load is a different binding, listed but not missed.
        assert_eq!(found.missed, [(2, TWIN)]);
        assert_eq!(found.merge, Merge::Different);
        let totals = Totals::new(&census, &[found]);
        assert_eq!((totals.missed, totals.candidates.len()), ([1, 1], 1));
        let lines = totals.lines(&census, false);
        assert!(lines.contains(&"siblings R1 report-only sites=1 families=1".to_string()));
        assert!(lines.contains(&"siblings candidates unknown=2/40".to_string()));
    }

    #[test]
    fn places_need_audited_code_a_word_boundary_and_boundary_evidence() {
        let one = function(TWIN, IMPORT, 0x0200_8300);
        let mut headless = one.clone();
        headless[1] = 0xb4;
        let edition = edition(&[&[(TWIN, &one), (0x0200_8200, &headless)]]);
        let mut model = build(&edition, &[((1, TWIN), 20, true, None)]);
        assert_eq!(model.status((1, TWIN), 20), Status::Exact);
        assert_eq!(model.status((1, TWIN + 4), 12), Status::Pattern);
        assert_eq!(model.status((1, TWIN + 22), 12), Status::Pattern);
        // A registered start off a word boundary is still no function.
        model.registered.insert((1, 0x0200_8202));
        assert_eq!(model.status((1, 0x0200_8202), 12), Status::Pattern);
        assert_eq!(model.status((1, 0x0200_8200), 20), Status::Pattern);
        model.registered.insert((1, 0x0200_8200));
        assert_eq!(model.status((1, 0x0200_8200), 20), Status::Unknown);
        model.spans[1].withdrawn = vec![Span::new(0x0200_8200, 0x0200_8204)];
        assert_eq!(model.status((1, 0x0200_8200), 20), Status::Withdrawn);
        model.spans[1].executable = vec![Span::new(0x0200_8000, 0x0200_8210)];
        assert_eq!(model.status((1, 0x0200_8200), 20), Status::Data);
        // Exact C stays exact whatever audited intervals say around it.
        assert_eq!(model.status((1, TWIN), 20), Status::Exact);
    }

    #[test]
    fn check_siblings_rejects_equivalent_exact_twins_in_different_units() {
        let one = function(TWIN, IMPORT, 0x0200_8300);
        let two = function(TWIN, IMPORT, 0x0200_8310);
        let edition = edition(&[&[(TWIN, &one)], &[(TWIN, &two)]]);
        let copies = |left, right| {
            let model = build(
                &edition,
                &[((1, TWIN), 20, true, left), ((2, TWIN), 20, true, right)],
            );
            let census = model.census(false);
            model.findings(&census.families[0]).copies
        };
        let model = build(
            &edition,
            &[((1, TWIN), 20, true, None), ((2, TWIN), 20, true, None)],
        );
        let census = model.census(false);
        // Pool words into each overlay's own image differ only there.
        assert_eq!(
            model.findings(&census.families[0]).merge,
            Merge::OverlayLocal
        );
        let copied = [(1, TWIN), (2, TWIN)];
        assert_eq!(
            copies(Some(("left", "Twin")), Some(("right", "Twin"))),
            copied
        );
        assert_eq!(copies(None, None), copied);
        assert!(copies(Some(("module", "Twin")), Some(("module", "Twin"))).is_empty());
    }

    #[test]
    fn variant_module_twins_with_divergent_neighbours_are_allowed() {
        const NEIGHBOUR: u32 = 0x0200_8140;
        let one = function(TWIN, IMPORT, 0x0200_8300);
        let two = function(TWIN, IMPORT, 0x0200_8310);
        let beside = helper(NEIGHBOUR, IMPORT, 0x0200_8320, 7);
        let variant = helper(NEIGHBOUR, IMPORT, 0x0200_8320, 6);
        let copies = |neighbour: &[u8]| {
            let edition = edition(&[
                &[(TWIN, &one), (NEIGHBOUR, &beside)],
                &[(TWIN, &two), (NEIGHBOUR, neighbour)],
            ]);
            let model = build(
                &edition,
                &[
                    ((1, TWIN), 20, true, Some(("left", "Twin"))),
                    ((1, NEIGHBOUR), 20, true, Some(("left", "Beside"))),
                    ((2, TWIN), 20, true, Some(("right", "Twin"))),
                    ((2, NEIGHBOUR), 20, true, Some(("right", "Variant"))),
                ],
            );
            let census = model.census(false);
            let twin = census
                .families
                .iter()
                .find(|family| family.owners.contains(&0))
                .unwrap();
            model.findings(twin).copies
        };
        assert!(copies(&variant).is_empty());
        // The same neighbour at the same distance is one module copied twice.
        assert_eq!(copies(&beside), [(1, TWIN), (2, TWIN)]);
    }

    #[test]
    fn twins_calling_non_twin_local_functions_are_not_equivalent() {
        const HELPER: u32 = 0x0200_8180;
        let caller = function(TWIN, HELPER, 0x0200_8300);
        let called = helper(HELPER, IMPORT, 0x0200_8300, 7);
        let changed = helper(HELPER, IMPORT, 0x0200_8300, 6);
        let missed = |other: &[u8]| {
            let edition = edition(&[
                &[(TWIN, &caller), (HELPER, &called)],
                &[(TWIN, &caller), (HELPER, other)],
            ]);
            let model = build(
                &edition,
                &[((1, TWIN), 20, true, None), ((1, HELPER), 20, true, None)],
            );
            let census = model.census(false);
            let twin = census
                .families
                .iter()
                .find(|family| family.owners.contains(&0))
                .unwrap();
            model.findings(twin).missed
        };
        assert!(missed(&changed).is_empty());
        assert_eq!(missed(&called), [(2, TWIN)]);
    }

    #[test]
    fn address_names_in_instanced_sources_and_included_headers_are_rejected() {
        let repository = Repository::new();
        let root = repository.0.path();
        repository.write(
            &format!("games/THE BROKEN SEAL/SRC/{STAGED_ACTOR}"),
            "#include \"STAGED.H\"\n/* Func_02000000 is prose */\nint f(void) { return Value_0200abcd; }\n",
        );
        repository.write(
            "games/THE BROKEN SEAL/INCLUDE/STAGED.H",
            "#define StagedActor_Find Func_02001234 // Data_02000000\n",
        );
        repository.write(
            "games/THE BROKEN SEAL/SRC/FIELD/ALONE.C",
            "#define Old Func_02005678\n",
        );
        let alone = json!({
            "id": "alone", "source": "games/THE BROKEN SEAL/SRC/FIELD/ALONE.C",
            "overlay": "resource_394",
            "owners": [{"address": "0x02000100"}]
        });
        repository.units(json!([staged_actor(), alone]));
        let units = TranslationUnits::declared(root).unwrap();
        assert_eq!(
            address_names(root, &units).unwrap(),
            [
                "games/THE BROKEN SEAL/INCLUDE/STAGED.H:1: Func_02001234",
                "games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/STAGED_ACTOR.C:3: Value_0200abcd",
            ]
        );
    }

    #[test]
    fn adopt_refuses_when_equivalent_twin_is_left_behind() {
        let one = function(TWIN, IMPORT, 0x0200_8300);
        let two = function(TWIN, IMPORT, 0x0200_8310);
        let edition = edition(&[&[(TWIN, &one)], &[(TWIN, &two)]]);
        let error = build(&edition, &[]).guard(&[((1, TWIN), 20)]).unwrap_err();
        assert!(
            error.contains("R1: equivalent twin resource_381:02000100 would stay unknown; needs one translation unit"),
            "{error}"
        );
        // Declared as one member, either twin may install.
        let member = Some(("staged-actor", "Twin"));
        let declared = build(
            &edition,
            &[
                ((1, TWIN), 20, false, member),
                ((2, TWIN), 20, false, member),
            ],
        );
        assert_eq!(
            declared.guard(&[((1, TWIN), 20)]).unwrap(),
            ["siblings resource_380:02000100 twin=resource_381:02000100 status=unknown equivalent=yes"]
        );
        // A second source beside an exact twin is a copy, named with its entry.
        let exact = build(&edition, &[((2, TWIN), 20, true, member)]);
        let error = exact.guard(&[((1, TWIN), 20)]).unwrap_err();
        assert!(
            error.contains(r#"R2: resource_380:02000100 and exact twin resource_381:02000100 would be two sources; needs staged-actor instances.resource_380.owners.Twin={"address":"0x02000100"}"#),
            "{error}"
        );
    }

    #[test]
    fn report_lists_ja_and_tla_sites_as_report_only() {
        const ALONE: u32 = 0x0200_8200;
        let one = function(TWIN, IMPORT, 0x0200_8300);
        let two = function(TWIN, IMPORT, 0x0200_8310);
        let alone = helper(ALONE, IMPORT, 0x0200_8300, 7);
        let english = edition(&[&[(TWIN, &one), (ALONE, &alone)], &[(TWIN, &two)]]);
        let owners = [((1, TWIN), 20, true, None), ((1, ALONE), 20, true, None)];
        let model = build(&english, &owners);
        let census = model.census(false);
        let japanese = edition(&[&[(ALONE, &alone)], &[(TWIN, &two)]]);
        // Only the first family is listed; every family is counted.
        let (summary, families) = leads(
            &[("tbs-ja", Some(japanese)), ("tla-en", None)],
            &census,
            &BTreeSet::from([0]),
        );
        assert_eq!(summary["tbs-ja"]["report_only"], true);
        let listed = json!({"families": 1, "of": 1, "same_site_count": 0, "sites": 1});
        let all = json!({"families": 2, "of": 2, "same_site_count": 1, "sites": 2});
        assert_eq!(
            (&summary["tbs-ja"]["listed"], &summary["tbs-ja"]["all"]),
            (&listed, &all)
        );
        assert_eq!(summary["tla-en"], json!({"available": false}));
        assert_eq!(
            families[&0]["tbs-ja"]["owners"],
            json!(["resource_381:02000100"])
        );
        assert_eq!(
            families[&1]["tbs-ja"]["owners"],
            json!(["resource_380:02000200"])
        );
        // Other editions never enter the rules.
        assert_eq!(model.findings(&census.families[0]).missed, [(2, TWIN)]);
    }
}
