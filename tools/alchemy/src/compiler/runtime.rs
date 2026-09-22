//! Compiler runtime routines the original images linked from the toolchain
//! library, as pret links libgcc. This repository carries no license, so it
//! holds no runtime code: the registry names library members and the addresses
//! at which the original link placed them, and every byte is built here from
//! the licensed `agscc` container with the approved compiler, following the
//! container's own library build rules.
use crate::compiler::build_io::{read, read_json, text, write};
use crate::compiler::overlay::{encode, RESOURCE_BASE, RUNTIME_BASE};
use crate::compiler::routing::{bundle, runtime_library_cflags, CompilerTarget};
use crate::compiler::sha256;
use fs2::FileExt;
use psynergy::cache::SqliteCache;
use psynergy::process::run;
use serde::Deserialize;
use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};
use std::sync::{Mutex, OnceLock};

/// Members and link placements; the only runtime facts the repository keeps.
pub const REGISTRY: &str = "games/THE BROKEN SEAL/recon/compiler-runtime.json";
/// The listing label of a runtime window in a retained overlay listing.
pub const LABEL: &str = "AlchemyRuntime_";
const CONTAINER: &str = "agscc";
/// The container's target makefile fragment: generated sources and flags.
const TARGET_RULES: &str = "gcc/config/arm/t-arm-elf";
/// `LIBGCC2_CFLAGS` and `INTERNAL_CFLAGS` defines of the container's library build.
const LIBRARY_DEFINES: &[&str] = &[
    "-DCROSS_COMPILE",
    "-DIN_GCC",
    "-DIN_LIBGCC2",
    "-D__GCC_FLOAT_NOT_NEEDED",
];
/// The options `agscc/build.sh` configures the container with.
const CONFIGURE: &[&str] = &[
    "--build=i686-unknown-linux-gnu",
    "--host=i686-unknown-linux-gnu",
    "--target=arm-elf",
    "--with-cpu=arm7tdmi",
    "--enable-multilib",
    "--enable-interwork",
    "--enable-languages=c",
    "--without-headers",
    "--disable-shared",
    "--disable-threads",
    "--disable-nls",
    "--with-gnu-as",
    "--with-gnu-ld",
    "--disable-checking",
];
const CONFIGURE_CFLAGS: &str =
    "-O2 -std=gnu17 -Wno-error -Wno-implicit-int -Wno-implicit-function-declaration";
const LINK_BINUTILS: [&str; 3] = [
    "arm-none-eabi-ld",
    "arm-none-eabi-nm",
    "arm-none-eabi-objcopy",
];
const CACHE_VERSION: &str = "compiler-runtime-v1";

#[derive(Deserialize)]
#[serde(deny_unknown_fields)]
struct Document {
    format: u32,
    kind: String,
    container: String,
    members: BTreeMap<String, String>,
    links: Vec<LinkRecord>,
}
#[derive(Deserialize)]
#[serde(deny_unknown_fields)]
struct LinkRecord {
    image: String,
    text: String,
    #[serde(default)]
    rodata: Option<String>,
    members: Vec<String>,
}

/// One link of library members, in archive-scan order, at a recorded address.
/// Overlay addresses are resource coordinates, as in the retained listings.
#[derive(Clone, Debug, PartialEq, Eq, PartialOrd, Ord)]
pub struct Link {
    pub image: String,
    pub text: u32,
    pub rodata: Option<u32>,
    pub members: Vec<String>,
}
impl Link {
    pub fn overlay(&self) -> bool {
        self.image != "main"
    }
    fn runtime(&self, address: u32) -> u32 {
        if self.overlay() {
            address - RESOURCE_BASE + RUNTIME_BASE
        } else {
            address
        }
    }
}
/// A link's placed bytes, serialized as the image stores them.
#[derive(Clone, Debug)]
pub struct Linked {
    pub text: Vec<u8>,
    pub rodata: Vec<u8>,
}
pub struct Registry {
    members: BTreeMap<String, String>,
    pub links: Vec<Link>,
}

fn address(value: &str, field: &str) -> Result<u32, String> {
    value
        .strip_prefix("0x")
        .filter(|digits| digits.len() == 8)
        .and_then(|digits| u32::from_str_radix(digits, 16).ok())
        .ok_or_else(|| format!("{REGISTRY}: invalid {field} address {value}"))
}
fn member_name(name: &str) -> bool {
    name.starts_with('_')
        && name.len() > 1
        && name
            .bytes()
            .all(|byte| byte.is_ascii_alphanumeric() || byte == b'_')
}
impl Registry {
    pub fn load(root: &Path) -> Result<Self, String> {
        Self::parse(read_json(root.join(REGISTRY))?)
    }
    fn parse(document: Document) -> Result<Self, String> {
        if document.format != 1
            || document.kind != "compiler-runtime-links"
            || document.container != CONTAINER
        {
            return Err(format!("{REGISTRY}: unsupported format"));
        }
        for (name, source) in &document.members {
            if !member_name(name) || source.is_empty() || source.contains("..") {
                return Err(format!("{REGISTRY}: invalid member {name}"));
            }
        }
        let mut links: Vec<Link> = Vec::new();
        let mut windows = BTreeSet::new();
        for record in &document.links {
            let link = Link {
                image: record.image.clone(),
                text: address(&record.text, "text")?,
                rodata: record
                    .rodata
                    .as_deref()
                    .map(|value| address(value, "rodata"))
                    .transpose()?,
                members: record.members.clone(),
            };
            let valid_image =
                link.image == "main" || crate::overlay::rom::resource_id(&link.image).is_ok();
            let range = if link.overlay() {
                RESOURCE_BASE..RUNTIME_BASE
            } else {
                0x0800_0000..0x0a00_0000
            };
            let distinct: BTreeSet<_> = link.members.iter().collect();
            if !valid_image
                || !range.contains(&link.text)
                || link.rodata.is_some_and(|rodata| !range.contains(&rodata))
                || link.text & 3 != 0
                || link.members.is_empty()
                || distinct.len() != link.members.len()
                || link
                    .members
                    .iter()
                    .any(|member| !document.members.contains_key(member))
            {
                return Err(format!(
                    "{REGISTRY}: invalid link {} 0x{:08x}",
                    link.image, link.text
                ));
            }
            for start in std::iter::once(link.text).chain(link.rodata) {
                if !windows.insert((link.image.clone(), start)) {
                    return Err(format!(
                        "{REGISTRY}: duplicate window {} 0x{start:08x}",
                        link.image
                    ));
                }
            }
            links.push(link);
        }
        let used: BTreeSet<_> = links.iter().flat_map(|link| &link.members).collect();
        if let Some(unused) = document.members.keys().find(|name| !used.contains(name)) {
            return Err(format!("{REGISTRY}: member {unused} is never linked"));
        }
        Ok(Self {
            members: document.members,
            links,
        })
    }
    pub fn links_for<'a>(&'a self, image: &'a str) -> impl Iterator<Item = &'a Link> + 'a {
        self.links.iter().filter(move |link| link.image == image)
    }
    /// The link whose text or data window starts at `start` in `image`.
    pub fn window(&self, image: &str, start: u32) -> Option<(&Link, bool)> {
        self.links.iter().find_map(|link| {
            if link.image != image {
                None
            } else if link.text == start {
                Some((link, true))
            } else if link.rodata == Some(start) {
                Some((link, false))
            } else {
                None
            }
        })
    }
}

/// A generated library source and the container rule that writes it:
/// `name:` followed by `echo '...' > name` and `cat $(srcdir)/path >> name`.
fn generated_source(container: &Path, rules: &str, name: &str) -> Result<String, String> {
    let mut lines = rules.lines().skip_while(|line| {
        line.split_once(':')
            .is_none_or(|(target, _)| target.trim() != name)
    });
    if lines.next().is_none() {
        return Err(format!("{TARGET_RULES} has no rule for {name}"));
    }
    let mut output = String::new();
    for line in lines.take_while(|line| line.starts_with('\t')) {
        let command = line.trim();
        let redirect = [format!(" >> {name}"), format!(" > {name}")]
            .into_iter()
            .find_map(|suffix| command.strip_suffix(suffix.as_str()))
            .ok_or_else(|| format!("{TARGET_RULES}: unsupported {name} command: {command}"))?;
        if let Some(echoed) = redirect
            .strip_prefix("echo '")
            .and_then(|rest| rest.strip_suffix('\''))
        {
            output.push_str(echoed);
            output.push('\n');
        } else if let Some(path) = redirect.strip_prefix("cat $(srcdir)/") {
            let file = container.join("gcc").join(path);
            output.push_str(&String::from_utf8_lossy(&read(&file)?));
        } else {
            return Err(format!(
                "{TARGET_RULES}: unsupported {name} command: {command}"
            ));
        }
    }
    if output.is_empty() {
        return Err(format!("{TARGET_RULES}: empty rule for {name}"));
    }
    Ok(output)
}
/// A makefile variable's words, as the target fragment assigns them.
fn rule_variable(rules: &str, name: &str) -> Result<Vec<String>, String> {
    rules
        .lines()
        .find_map(|line| {
            let (key, value) = line.split_once('=')?;
            (key.trim() == name).then(|| value.split_whitespace().map(str::to_string).collect())
        })
        .ok_or_else(|| format!("{TARGET_RULES} does not assign {name}"))
}

enum Source {
    /// `-xassembler-with-cpp` source in the container.
    Assembly(PathBuf),
    /// A fine-grained source a container rule generates.
    Generated(String),
    /// C that includes the configured `tconfig.h`.
    Configured(PathBuf),
}
fn source_of(container: &Path, source: &str) -> Source {
    if !source.contains('/') {
        Source::Generated(source.to_string())
    } else if source.ends_with(".asm") {
        Source::Assembly(container.join(source))
    } else {
        Source::Configured(container.join(source))
    }
}

/// The configured container headers (`tconfig.h`, `tm.h`), produced once by
/// the container's own `configure` under ignored output.
fn configured(root: &Path) -> Result<PathBuf, String> {
    static LOCK: Mutex<()> = Mutex::new(());
    let container = root.join(CONTAINER);
    let script = container.join("gcc/configure");
    let identity = sha256::hex(
        &[
            read(&script)?,
            CONFIGURE.join("\0").into_bytes(),
            CONFIGURE_CFLAGS.as_bytes().to_vec(),
        ]
        .concat(),
    );
    let parent = root.join("tools/out/compiler-build/runtime");
    let directory = parent.join(format!("configure-{}", &identity[..16]));
    if directory.join("tconfig.h").is_file() {
        return Ok(directory);
    }
    let _guard = LOCK.lock().map_err(|_| "runtime configure lock poisoned")?;
    std::fs::create_dir_all(&parent).map_err(|error| format!("{}: {error}", parent.display()))?;
    let lock_path = parent.join(".configure.lock");
    let lock = std::fs::File::create(&lock_path)
        .map_err(|error| format!("{}: {error}", lock_path.display()))?;
    lock.lock_exclusive()
        .map_err(|error| format!("{}: {error}", lock_path.display()))?;
    if directory.join("tconfig.h").is_file() {
        return Ok(directory);
    }
    let work = tempfile::Builder::new()
        .prefix(".configure-")
        .tempdir_in(&parent)
        .map_err(|error| error.to_string())?;
    let output = std::process::Command::new("sh")
        .arg(&script)
        .arg(format!("--srcdir={}", container.join("gcc").display()))
        .args(CONFIGURE)
        .env("CFLAGS", CONFIGURE_CFLAGS)
        .current_dir(work.path())
        .output()
        .map_err(|error| format!("runtime configure: {error}"))?;
    if !output.status.success() || !work.path().join("tconfig.h").is_file() {
        let log = String::from_utf8_lossy(&output.stderr);
        let tail: Vec<_> = log.lines().rev().take(12).collect();
        return Err(format!(
            "runtime configure of {CONTAINER} failed: {}",
            tail.into_iter().rev().collect::<Vec<_>>().join("\n")
        ));
    }
    let staged = work.keep();
    std::fs::rename(&staged, &directory)
        .map_err(|error| format!("{}: {error}", directory.display()))?;
    Ok(directory)
}

/// The compile command of one member, without its output option.
fn member_command(
    root: &Path,
    registry: &Registry,
    member: &str,
    work: &Path,
) -> Result<Vec<String>, String> {
    let container = root.join(CONTAINER);
    let rules = String::from_utf8_lossy(&read(container.join(TARGET_RULES))?).into_owned();
    let recorded = registry
        .members
        .get(member)
        .ok_or_else(|| format!("{REGISTRY}: unknown member {member}"))?;
    let mut command = vec![
        text(bundle().join("xgcc")),
        format!("-B{}/", bundle().display()),
    ];
    command.extend(runtime_library_cflags());
    command.extend(LIBRARY_DEFINES.iter().map(|flag| (*flag).to_string()));
    command.extend(rule_variable(&rules, "TARGET_LIBGCC2_CFLAGS")?);
    command.extend(["-isystem".to_string(), text(container.join("gcc/ginclude"))]);
    let includes = [
        container.join("gcc"),
        container.join("gcc/config"),
        container.join("include"),
    ];
    match source_of(&container, recorded) {
        Source::Assembly(path) => {
            command.extend(includes.iter().map(|path| format!("-I{}", path.display())));
            command.extend([
                format!("-DL{member}"),
                "-xassembler-with-cpp".into(),
                text(path),
            ]);
        }
        Source::Generated(name) => {
            let generated = work.join(&name);
            if !generated.is_file() {
                write(&generated, generated_source(&container, &rules, &name)?)?;
            }
            command.extend(includes.iter().map(|path| format!("-I{}", path.display())));
            command.extend([
                "-DFINE_GRAINED_LIBRARIES".into(),
                format!("-DL{member}"),
                text(generated),
            ]);
        }
        Source::Configured(path) => {
            command.push(format!("-I{}", configured(root)?.display()));
            command.extend(includes.iter().map(|path| format!("-I{}", path.display())));
            command.extend([format!("-DL{member}"), text(path)]);
        }
    }
    Ok(command)
}
/// Links built by this process, keyed by checkout.
fn memo() -> &'static Mutex<BTreeMap<(String, Link), Linked>> {
    static BUILT: OnceLock<Mutex<BTreeMap<(String, Link), Linked>>> = OnceLock::new();
    BUILT.get_or_init(|| Mutex::new(BTreeMap::new()))
}
fn section(elf: &Path, name: &str, work: &Path) -> Result<Vec<u8>, String> {
    let output = work.join(format!("{}.bin", name.trim_start_matches('.')));
    run(
        &[
            "arm-none-eabi-objcopy".into(),
            "-O".into(),
            "binary".into(),
            "-j".into(),
            name.into(),
            text(elf),
            text(&output),
        ],
        work,
    )?;
    Ok(if output.is_file() {
        read(&output)?
    } else {
        Vec::new()
    })
}

/// Build one link from the container, or return its cached bytes. The cache
/// identity is the preprocessed member sources, flags, compiler bundle and
/// binutils, so a container change rebuilds.
pub fn build(root: &Path, link: &Link) -> Result<Linked, String> {
    crate::compiler::bundle::validate_bundle(CompilerTarget::Tbs)?;
    let registry = Registry::load(root)?;
    if !registry.links.contains(link) {
        return Err(format!(
            "{} 0x{:08x} is not a registered runtime link",
            link.image, link.text
        ));
    }
    let memo_key = (text(root), link.clone());
    if let Some(linked) = memo().lock().unwrap().get(&memo_key) {
        return Ok(linked.clone());
    }
    let parent = root.join("tools/out/compiler-build/runtime");
    std::fs::create_dir_all(&parent).map_err(|error| format!("{}: {error}", parent.display()))?;
    let work = tempfile::Builder::new()
        .prefix(".link-")
        .tempdir_in(&parent)
        .map_err(|error| error.to_string())?;
    let mut identity = vec![
        CACHE_VERSION.to_string(),
        crate::compiler::bundle::compiler_bundle_signature_checked()?,
        crate::compiler::bundle::host_executable_signature(&LINK_BINUTILS)?,
        format!("{link:?}"),
    ];
    let mut commands = Vec::new();
    for member in &link.members {
        let command = member_command(root, &registry, member, work.path())?;
        let mut preprocess = command.clone();
        preprocess.extend(["-E".into(), "-P".into()]);
        let expanded = run(&preprocess, work.path())
            .map_err(|error| format!("runtime member {member}: {error}"))?;
        let portable = |value: &String| value.replace(&text(root), "<root>");
        identity.push(member.clone());
        identity.extend(command.iter().map(portable));
        identity.push(sha256::hex(expanded.as_bytes()));
        commands.push((member.clone(), command));
    }
    let key = sha256::hex(identity.join("\0").as_bytes());
    let store = SqliteCache::open(&root.join("out/cache/compiler-runtime.sqlite3"))?;
    let cached = store.get(&key).ok().flatten().and_then(|entries| {
        let find = |name: &str| {
            entries
                .iter()
                .find(|(kind, _)| kind == name)
                .map(|(_, data)| data.clone())
        };
        Some(Linked {
            text: find("text")?,
            rodata: find("rodata")?,
        })
    });
    let linked = match cached {
        Some(linked) => linked,
        None => {
            let linked = link_members(link, &commands, work.path())?;
            store.put(&key, &[("text", &linked.text), ("rodata", &linked.rodata)])?;
            linked
        }
    };
    memo().lock().unwrap().insert(memo_key, linked.clone());
    Ok(linked)
}
fn link_members(
    link: &Link,
    commands: &[(String, Vec<String>)],
    work: &Path,
) -> Result<Linked, String> {
    let mut objects = Vec::new();
    for (member, command) in commands {
        let object = work.join(format!("{member}.o"));
        let mut compile = command.clone();
        compile.extend(["-c".into(), "-o".into(), text(&object)]);
        run(&compile, work).map_err(|error| format!("runtime member {member}: {error}"))?;
        objects.push(object);
    }
    let inputs = objects
        .iter()
        .map(|object| format!("\"{}\"(.text)", object.display()))
        .collect::<Vec<_>>()
        .join(" ");
    let rodata = link.rodata.map_or_else(String::new, |address| {
        format!(
            "  .rodata 0x{:08x} : {{ *(.rodata) }}\n",
            link.runtime(address)
        )
    });
    let script = work.join("runtime.ld");
    write(
        &script,
        format!(
            "SECTIONS\n{{\n  .text 0x{:08x} : {{ {inputs} }}\n{rodata}}}\n",
            link.runtime(link.text)
        ),
    )?;
    let elf = work.join("runtime.elf");
    let mut command = vec![
        "arm-none-eabi-ld".to_string(),
        "--no-warn-mismatch".into(),
        "-e".into(),
        "0".into(),
        "-T".into(),
        text(&script),
        "-o".into(),
        text(&elf),
    ];
    command.extend(objects.iter().map(text));
    run(&command, work)?;
    let undefined = run(&["arm-none-eabi-nm".into(), "-u".into(), text(&elf)], work)?;
    if !undefined.trim().is_empty() {
        return Err(format!(
            "runtime link {} 0x{:08x} leaves undefined symbols: {}",
            link.image,
            link.text,
            undefined.split_whitespace().collect::<Vec<_>>().join(" ")
        ));
    }
    let headers = run(
        &["arm-none-eabi-objdump".into(), "-h".into(), text(&elf)],
        work,
    )?;
    for line in headers.lines() {
        let fields: Vec<_> = line.split_whitespace().collect();
        if let [_, name, size, ..] = fields.as_slice() {
            let allowed = *name == ".text" || (*name == ".rodata" && link.rodata.is_some());
            if name.starts_with('.')
                && !allowed
                && u64::from_str_radix(size, 16).is_ok_and(|size| size > 0)
            {
                return Err(format!(
                    "runtime link {} 0x{:08x} has unplaced section {name}",
                    link.image, link.text
                ));
            }
        }
    }
    let mut text_bytes = section(&elf, ".text", work)?;
    let rodata_bytes = if link.rodata.is_some() {
        section(&elf, ".rodata", work)?
    } else {
        Vec::new()
    };
    if text_bytes.is_empty() || (link.rodata.is_some() && rodata_bytes.is_empty()) {
        return Err(format!(
            "runtime link {} 0x{:08x} produced an empty window",
            link.image, link.text
        ));
    }
    if link.overlay() {
        text_bytes = encode(&text_bytes, (link.text - RESOURCE_BASE) as usize)?;
    }
    Ok(Linked {
        text: text_bytes,
        rodata: rodata_bytes,
    })
}

/// `AlchemyRuntime_<address>:` windows of a retained overlay listing, with
/// the byte span of the `.space` directives that reserve each one.
pub fn listing_windows(listing: &str) -> Result<Vec<(u32, usize)>, String> {
    let lines: Vec<&str> = listing.lines().map(str::trim).collect();
    let mut windows = Vec::new();
    for (index, line) in lines.iter().enumerate() {
        let Some(label) = line.strip_prefix(LABEL) else {
            continue;
        };
        let start = label
            .strip_suffix(':')
            .filter(|digits| digits.len() == 8)
            .and_then(|digits| u32::from_str_radix(digits, 16).ok())
            .ok_or_else(|| format!("invalid runtime window label {line}"))?;
        let mut span = 0usize;
        for next in &lines[index + 1..] {
            match crate::compiler::overlay::space_size(next) {
                Some(size) => span += size as usize,
                None => break,
            }
        }
        if span == 0 {
            return Err(format!("{LABEL}{start:08x} reserves no bytes"));
        }
        windows.push((start, span));
    }
    Ok(windows)
}
/// The container-built bytes for each runtime window of one overlay listing.
pub fn overlay_fill(
    root: &Path,
    game: CompilerTarget,
    overlay: &str,
    listing: &str,
) -> Result<Vec<(u32, Vec<u8>)>, String> {
    let windows = listing_windows(listing)?;
    if windows.is_empty() {
        return Ok(Vec::new());
    }
    if game != CompilerTarget::Tbs {
        return Err(format!(
            "{overlay}: runtime windows are registered only for {}",
            CompilerTarget::Tbs.directory()
        ));
    }
    let registry = Registry::load(root)?;
    let mut fills = Vec::new();
    for (start, span) in windows {
        let (link, text_window) = registry.window(overlay, start).ok_or_else(|| {
            format!("{overlay}: {LABEL}{start:08x} is not a registered runtime link")
        })?;
        let linked = build(root, link)?;
        let data = if text_window {
            linked.text
        } else {
            linked.rodata
        };
        if data.len() != span {
            return Err(format!(
                "{overlay}: runtime window 0x{start:08x} reserves {span} bytes; the container built {}",
                data.len()
            ));
        }
        fills.push((start, data));
    }
    Ok(fills)
}
/// Identity of every registered link's built bytes, for stage stamps. A tree
/// without a registry links no runtime.
pub fn signature(root: &Path) -> Result<String, String> {
    if !root.join(REGISTRY).is_file() {
        return Ok("no-compiler-runtime".into());
    }
    let registry = Registry::load(root)?;
    let mut stream = read(root.join(REGISTRY))?;
    for link in &registry.links {
        let linked = build(root, link)?;
        stream.extend(sha256::hex(&linked.text).into_bytes());
        stream.extend(sha256::hex(&linked.rodata).into_bytes());
    }
    Ok(sha256::hex(&stream))
}

#[cfg(test)]
mod tests {
    use super::*;
    fn document(links: serde_json::Value) -> Document {
        serde_json::from_value(serde_json::json!({
            "format": 1,
            "kind": "compiler-runtime-links",
            "container": "agscc",
            "members": {"_first": "gcc/config/arm/first.asm", "_second": "second.c"},
            "links": links,
        }))
        .unwrap()
    }
    #[test]
    fn registry_rejects_invalid_duplicate_and_unused_links() {
        let valid = Registry::parse(document(serde_json::json!([
            {"image": "main", "text": "0x080072e4", "members": ["_first"]},
            {"image": "resource_3bf", "text": "0x020057b0", "rodata": "0x02005f90", "members": ["_first", "_second"]},
        ])))
        .unwrap();
        assert_eq!(valid.window("resource_3bf", 0x0200_5f90).unwrap().1, false);
        assert!(valid.window("resource_3bf", 0x0200_57b0).unwrap().1);
        assert!(valid.window("resource_3a7", 0x0200_57b0).is_none());
        for links in [
            serde_json::json!([{"image": "main", "text": "0x080072e4", "members": ["_first", "_second"]},
                               {"image": "main", "text": "0x080072e4", "members": ["_first"]}]),
            serde_json::json!([{"image": "main", "text": "0x020057b0", "members": ["_first", "_second"]}]),
            serde_json::json!([{"image": "resource_3bf", "text": "0x080072e4", "members": ["_first", "_second"]}]),
            serde_json::json!([{"image": "main", "text": "0x080072e6", "members": ["_first", "_second"]}]),
            serde_json::json!([{"image": "main", "text": "0x080072e4", "members": ["_first", "_first", "_second"]}]),
            serde_json::json!([{"image": "main", "text": "0x080072e4", "members": ["_first", "_third"]}]),
            serde_json::json!([{"image": "main", "text": "0x080072e4", "members": ["_first"]}]),
            serde_json::json!([{"image": "atlas", "text": "0x080072e4", "members": ["_first", "_second"]}]),
        ] {
            assert!(Registry::parse(document(links)).is_err());
        }
    }
    #[test]
    fn generated_sources_follow_the_container_rule() {
        let container = tempfile::tempdir().unwrap();
        std::fs::create_dir_all(container.path().join("gcc/config")).unwrap();
        std::fs::write(container.path().join("gcc/config/body.c"), "int body;\n").unwrap();
        let rules = "X = 1\n\nwide.c: $(srcdir)/config/body.c\n\techo '#define WIDE' > wide.c\n\techo '#define ORDER' >> wide.c\n\tcat $(srcdir)/config/body.c >> wide.c\n\nother.c:\n\techo x > other.c\n";
        assert_eq!(
            generated_source(container.path(), rules, "wide.c").unwrap(),
            "#define WIDE\n#define ORDER\nint body;\n"
        );
        assert!(generated_source(container.path(), rules, "other.c").is_err());
        assert!(generated_source(container.path(), rules, "absent.c").is_err());
        assert_eq!(
            rule_variable("FLAGS = -Da -fb\n", "FLAGS").unwrap(),
            ["-Da", "-fb"]
        );
        assert!(rule_variable("FLAGS = -Da\n", "OTHER").is_err());
    }
    #[test]
    fn listing_windows_measure_reserved_spans() {
        let listing = "\t.4byte 0\nAlchemyRuntime_020057b0:\n\t.space 0x3c\n\t.space 4\n\t.4byte 1\nAlchemyRuntime_02005f90:\n\t.space 0x14\n";
        assert_eq!(
            listing_windows(listing).unwrap(),
            [(0x0200_57b0, 64), (0x0200_5f90, 20)]
        );
        assert!(listing_windows("AlchemyRuntime_020057b0:\n\t.4byte 0\n").is_err());
        assert!(listing_windows("AlchemyRuntime_57b0:\n\t.space 4\n").is_err());
    }
    /// The tracked registry names only members and addresses, and every
    /// overlay link has a reserved window in its tracked listing.
    #[test]
    fn tracked_links_have_reserved_listing_windows() {
        let root = crate::compiler::routing::root();
        let registry = Registry::load(root).unwrap();
        for link in registry.links.iter().filter(|link| link.overlay()) {
            let path = root.join(format!(
                "games/THE BROKEN SEAL/raw/overlays/{}_overlay.s",
                link.image
            ));
            let listing = std::fs::read_to_string(&path).unwrap();
            let windows = listing_windows(&listing).unwrap();
            for start in std::iter::once(link.text).chain(link.rodata) {
                assert!(
                    windows.iter().any(|(window, _)| *window == start),
                    "{} has no window at 0x{start:08x}",
                    link.image
                );
            }
        }
    }
}
