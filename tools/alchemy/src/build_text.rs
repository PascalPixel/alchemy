//! The message archive's build rule. Each game's `TEXT/MESSAGES.S` includes
//! `text/messages.inc`, which `build rom` writes from the edition's PO catalog
//! as pret's mapjson writes the assembly its data files include: the Huffman
//! archive, whose address words name its own labels, and an absolute symbol
//! `Msg<Name>` for each message the code names, set to that message's number
//! in this edition. The linker places the archive and fills every literal
//! pool that loads a message number.
use crate::targets::DecompTarget;
#[cfg(test)]
use crate::targets::TARGET_IDS;
use ags::text::{self, ARCHIVES};
#[cfg(test)]
use std::collections::BTreeSet;
use std::fs;
use std::path::Path;

/// The generated include, under the build directory the assembler searches.
pub(crate) const INCLUDE: &str = "text/messages.inc";

/// C sources explicitly import canonical message names as integer constants.
pub(crate) const C_INCLUDE: &str = "text/MSG_IDS.H";

/// The archive's labels: `Text_MessageModels`, `Text_MessageContexts` (read
/// by the symbol decoder) and `Text_MessageBanks` (read by the lookup).
const LABEL: &str = "Text_Message";

/// Write `output/text/messages.inc` for `target`, leaving an unchanged file
/// untouched.
pub(crate) fn build(root: &Path, target: DecompTarget, output: &Path) -> Result<(), String> {
    let spec = ARCHIVES
        .iter()
        .find(|spec| spec.target == target.id.as_str())
        .ok_or_else(|| format!("{} has no message catalog", target.id))?;
    let source = text::read_source(&root.join(spec.output))?;
    if source.target != spec.target {
        return Err(format!("{} is the {} catalog", spec.output, source.target));
    }
    let assembly = text::messages_include(&source, LABEL, spec.output)?;
    write_generated(&output.join(INCLUDE), &assembly)?;
    write_c_imports(output, &c_imports(&source.names))
}

fn write_generated(path: &Path, contents: &str) -> Result<(), String> {
    if fs::read_to_string(path).ok().as_deref() == Some(contents) {
        return Ok(());
    }
    fs::create_dir_all(path.parent().expect("include directory"))
        .map_err(|error| error.to_string())?;
    fs::write(path, contents).map_err(|error| format!("{}: {error}", path.display()))
}

/// Lookup macros leave the C identifier namespace alone until source requests
/// TEXT_MESSAGE_ENUM(MsgCanonicalName), which declares just that enum name.
fn c_imports(names: &[(String, usize)]) -> String {
    let mut header = String::from(
        "/* Message numbers from the edition's editable PO catalog. */\n\
         #ifndef ALCHEMY_MESSAGE_IDS_H\n\
         #define ALCHEMY_MESSAGE_IDS_H\n\
         #define TEXT_MESSAGE_ENUM(name) enum { name = TEXT_MESSAGE_NUMBER_ ## name }\n",
    );
    for (name, number) in names {
        header.push_str(&format!("#define TEXT_MESSAGE_NUMBER_{name} {number}\n"));
    }
    header.push_str("#endif\n");
    header
}

pub(crate) fn current_c_imports(root: &Path, target: DecompTarget) -> Result<String, String> {
    let spec = ARCHIVES
        .iter()
        .find(|spec| spec.target == target.id.as_str())
        .ok_or_else(|| format!("{} has no message catalog", target.id))?;
    let catalog = text::read_catalog(&root.join(spec.output))?;
    let actual = catalog.headers.get("X-Alchemy-Target").map(String::as_str);
    if actual != Some(spec.target) {
        return Err(format!("{} is not the {} catalog", spec.output, target.id));
    }
    let names =
        text::catalog_names(&catalog).map_err(|error| format!("{}: {error}", spec.output))?;
    Ok(c_imports(&names))
}

pub(crate) fn write_c_imports(output: &Path, header: &str) -> Result<(), String> {
    write_generated(&output.join(C_INCLUDE), header)
}

/// Audit metadata is generated from current source in a temporary directory;
/// old generated headers under out/ are never read by fresh source checks.
/// Standalone compiler fixtures without a catalog need no message imports.
pub(crate) fn fresh_c_imports(
    root: &Path,
    target: DecompTarget,
) -> Result<Option<tempfile::TempDir>, String> {
    let spec = ARCHIVES
        .iter()
        .find(|spec| spec.target == target.id.as_str())
        .ok_or_else(|| format!("{} has no message catalog", target.id))?;
    if !root.join(spec.output).is_file() {
        return Ok(None);
    }
    let directory = tempfile::Builder::new()
        .prefix("alchemy-message-imports-")
        .tempdir()
        .map_err(|error| error.to_string())?;
    write_c_imports(directory.path(), &current_c_imports(root, target)?)?;
    Ok(Some(directory))
}

/// Every `Msg<Name>` active in an edition's source is the `msgctxt` of exactly
/// one entry in that edition's catalog, and no linker script under
/// `games/` assigns one a number by hand.
#[cfg(test)]
pub(crate) fn check_names(root: &Path) -> Result<(), String> {
    let mut problems = Vec::new();
    for id in TARGET_IDS {
        let target = crate::targets::target_for(id);
        let mut used = BTreeSet::new();
        for directory in [target.game_dir(), "games/COMMON"] {
            used_names(
                root,
                target,
                &root.join(directory),
                &mut used,
                &mut problems,
            )?;
        }
        let spec = ARCHIVES
            .iter()
            .find(|spec| spec.target == id.as_str())
            .ok_or_else(|| format!("{id} has no catalog"))?;
        let names = text::catalog_names(&text::read_catalog(&root.join(spec.output))?)
            .map_err(|error| format!("{}: {error}", spec.output))?;
        let named = names
            .iter()
            .map(|(name, _)| name.as_str())
            .collect::<BTreeSet<_>>();
        problems.extend(missing_names(spec.output, &used, &named));
    }
    if problems.is_empty() {
        Ok(())
    } else {
        Err(problems.join("\n"))
    }
}

/// Collect the message names in `directory`'s C, headers and assembly, and
/// refuse any linker script that sets one.
#[cfg(test)]
fn used_names(
    root: &Path,
    target: DecompTarget,
    directory: &Path,
    used: &mut BTreeSet<String>,
    problems: &mut Vec<String>,
) -> Result<(), String> {
    let assignment =
        regex::Regex::new(r"(?m)^\s*(Msg[A-Z][A-Za-z0-9]*)\s*=").expect("static pattern");
    let mut sources = Vec::new();
    for entry in walkdir::WalkDir::new(directory) {
        let entry = entry.map_err(|error| error.to_string())?;
        let path = entry.path();
        let extension = path
            .extension()
            .and_then(|extension| extension.to_str())
            .unwrap_or_default()
            .to_ascii_lowercase();
        if !entry.file_type().is_file()
            || !matches!(extension.as_str(), "c" | "h" | "s" | "inc" | "ld")
        {
            continue;
        }
        if extension == "ld" {
            let source =
                fs::read_to_string(path).map_err(|error| format!("{}: {error}", path.display()))?;
            for capture in assignment.captures_iter(&source) {
                problems.push(format!(
                    "{}: {} is numbered by hand; name it in the catalogs",
                    path.display(),
                    &capture[1]
                ));
            }
        } else {
            sources.push(path.to_path_buf());
        }
    }
    let next = std::sync::atomic::AtomicUsize::new(0);
    let workers = std::thread::available_parallelism()
        .map_or(4, |count| count.get())
        .min(8);
    let results = std::thread::scope(|scope| {
        let handles = (0..workers)
            .map(|_| {
                scope.spawn(|| {
                    let mut names = BTreeSet::new();
                    loop {
                        let index = next.fetch_add(1, std::sync::atomic::Ordering::Relaxed);
                        let Some(path) = sources.get(index) else {
                            break;
                        };
                        names.extend(selected_names(root, target, path)?);
                    }
                    Ok::<_, String>(names)
                })
            })
            .collect::<Vec<_>>();
        handles
            .into_iter()
            .map(|handle| handle.join().expect("name validation worker"))
            .collect::<Result<Vec<_>, _>>()
    })?;
    for names in results {
        used.extend(names);
    }
    Ok(())
}

#[cfg(test)]
fn selected_names(
    root: &Path,
    target: DecompTarget,
    path: &Path,
) -> Result<BTreeSet<String>, String> {
    let source =
        fs::read_to_string(path).map_err(|error| format!("{}: {error}", path.display()))?;
    // Headers that contain names are inspected as well, including definitions
    // in macros. Unrelated shared headers need not be standalone translation
    // units for both games (their callers supply the owning game's includes).
    if !source.contains("Msg") {
        return Ok(BTreeSet::new());
    }
    let extension = path
        .extension()
        .and_then(|ext| ext.to_str())
        .unwrap_or_default()
        .to_ascii_lowercase();
    let expanded = if matches!(extension.as_str(), "c" | "h") {
        crate::compiler::preprocess::fresh_definitions(root, target, &path.to_string_lossy())?
    } else {
        crate::compiler::assembly_source::without_comments(
            &crate::compiler::assembly_source::selected_includes(
                &source,
                target.edition_define,
                &[root, &root.join(target.output_dir)],
            )
            .map_err(|error| format!("{}: {error}", path.display()))?,
        )
    };
    // Keep active #define bodies visible to the lexer, while strings/comments
    // remain non-identifiers. The compiler has already selected the branches.
    let mut bytes = expanded.as_bytes().to_vec();
    let mut start = 0;
    while start < bytes.len() {
        let end = expanded[start..]
            .find('\n')
            .map_or(bytes.len(), |at| start + at);
        if let Some(at) = (start..end)
            .find(|&at| !bytes[at].is_ascii_whitespace())
            .filter(|&at| bytes[at] == b'#')
        {
            bytes[at] = b' ';
        }
        if end > start && bytes[end - 1] == b'\\' {
            bytes[end - 1] = b' ';
        }
        start = end + 1;
    }
    let visible = String::from_utf8(bytes).expect("masking preserves UTF-8");
    Ok(crate::permute::lex::lex(&visible)?
        .into_iter()
        .filter_map(|token| match token.tok {
            crate::permute::lex::Tok::Ident(name) if text::message_name(&name) => Some(name),
            _ => None,
        })
        .collect())
}

#[cfg(test)]
fn missing_names(catalog: &str, used: &BTreeSet<String>, named: &BTreeSet<&str>) -> Vec<String> {
    used.iter()
        .filter(|name| !named.contains(name.as_str()))
        .map(|name| format!("{catalog}: no message is named {name}"))
        .collect()
}

#[cfg(test)]
mod tests {
    use super::*;

    fn write_catalog(root: &Path, target: DecompTarget, number: usize) {
        let path = root.join(format!(
            "{}/TEXT/{}.PO",
            target.game_dir(),
            target.id.as_str().split_once('-').unwrap().1.to_uppercase()
        ));
        fs::create_dir_all(path.parent().unwrap()).unwrap();
        fs::write(
            path,
            format!(
                "msgid \"\"\nmsgstr \"\"\n\"X-Alchemy-Target: {}\\n\"\n\n\
             msgctxt \"MsgImported\"\nmsgid \"{number:05}\"\nmsgstr \"one\"\n\n\
             msgctxt \"MsgKeptArray\"\nmsgid \"00009\"\nmsgstr \"two\"\n\n\
             msgctxt \"MsgUnused\"\nmsgid \"00010\"\nmsgstr \"three\"\n",
                target.id,
            ),
        )
        .unwrap();
    }

    #[test]
    fn canonical_enums_are_selective_and_valid_in_code_and_data() {
        let work = tempfile::tempdir().unwrap();
        let target = crate::targets::target_for(crate::targets::DecompTargetId::TbsEn);
        write_catalog(work.path(), target, 7);
        let path = work.path().join("IMPORT.C");
        fs::write(
            &path,
            "#include \"text/MSG_IDS.H\"\n\
             TEXT_MESSAGE_ENUM(MsgImported);\nextern unsigned char MsgKeptArray[];\n\
             const int table[] = { MsgImported };\n\
             int Read(void) { return MsgImported; }\n",
        )
        .unwrap();
        let expanded =
            crate::compiler::preprocess::fresh(work.path(), target, &path.to_string_lossy())
                .unwrap();
        assert!(expanded.contains("enum { MsgImported = 7 }"), "{expanded}");
        assert!(expanded.contains("extern unsigned char MsgKeptArray[]"));
        assert!(!expanded.contains("enum { MsgKeptArray"));
        assert!(!expanded.contains("enum { MsgUnused"));
        assert_eq!(
            selected_names(work.path(), target, &path).unwrap(),
            ["MsgImported".into(), "MsgKeptArray".into()].into()
        );
        let compiler =
            crate::compiler::preprocess::command(target, &path.to_string_lossy(), false).unwrap();
        let imports = fresh_c_imports(work.path(), target).unwrap().unwrap();
        let mut compile = compiler;
        compile.retain(|flag| flag != "-E");
        compile.insert(1, format!("-I{}", imports.path().display()));
        compile.extend([
            "-S".into(),
            "-o".into(),
            work.path().join("import.s").to_string_lossy().into_owned(),
            path.to_string_lossy().into_owned(),
        ]);
        psynergy::process::run(&compile, work.path()).unwrap();
        assert!(work.path().join("import.s").is_file());
    }

    #[test]
    fn fresh_imports_observe_catalog_edits_and_ignore_saved_headers() {
        let work = tempfile::tempdir().unwrap();
        let target = crate::targets::target_for(crate::targets::DecompTargetId::TbsEn);
        write_catalog(work.path(), target, 7);
        let path = work.path().join("IMPORT.C");
        fs::write(
            &path,
            "#include \"text/MSG_IDS.H\"\nTEXT_MESSAGE_ENUM(MsgImported);\n",
        )
        .unwrap();
        write_c_imports(
            &work.path().join(target.output_dir),
            "#error stale generated header\n",
        )
        .unwrap();
        let expand = || {
            crate::compiler::preprocess::fresh(work.path(), target, &path.to_string_lossy())
                .unwrap()
        };
        assert!(expand().contains("enum { MsgImported = 7 }"));
        write_catalog(work.path(), target, 8);
        assert!(expand().contains("enum { MsgImported = 8 }"));
        assert_eq!(
            fs::read_to_string(work.path().join(target.output_dir).join(C_INCLUDE)).unwrap(),
            "#error stale generated header\n"
        );
        fs::write(
            &path,
            "#include \"text/MSG_IDS.H\"\nTEXT_MESSAGE_ENUM(MsgNotInCatalog);\n",
        )
        .unwrap();
        let used = selected_names(work.path(), target, &path).unwrap();
        assert_eq!(
            missing_names("EN.PO", &used, &BTreeSet::new()),
            ["EN.PO: no message is named MsgNotInCatalog"]
        );
    }

    #[test]
    fn an_analysis_run_shares_current_imports_but_freshly_reads_each_source() {
        let work = tempfile::tempdir().unwrap();
        let target = crate::targets::target_for(crate::targets::DecompTargetId::TbsEn);
        write_catalog(work.path(), target, 7);
        write_c_imports(
            &work.path().join(target.output_dir),
            "#error stale header\n",
        )
        .unwrap();
        let first = work.path().join("FIRST.C");
        let second = work.path().join("SECOND.C");
        let source = |name| {
            format!("#include \"text/MSG_IDS.H\"\nTEXT_MESSAGE_ENUM(MsgImported);\nint {name}(void) {{ return MsgImported; }}\n")
        };
        fs::write(&first, source("First")).unwrap();
        fs::write(&second, source("Second")).unwrap();
        let expansion = crate::compiler::preprocess::Expansion::new(work.path(), target).unwrap();
        for path in [&first, &second] {
            let text = expansion.fresh(&path.to_string_lossy()).unwrap();
            assert!(text.contains("enum { MsgImported = 7 }"), "{text}");
        }
        fs::write(&second, source("Changed")).unwrap();
        let text = expansion.fresh(&second.to_string_lossy()).unwrap();
        assert!(text.contains("Changed(void)"), "{text}");
        write_catalog(work.path(), target, 8);
        let next = crate::compiler::preprocess::Expansion::new(work.path(), target).unwrap();
        let text = next.fresh(&first.to_string_lossy()).unwrap();
        assert!(text.contains("enum { MsgImported = 8 }"), "{text}");
    }

    #[test]
    fn every_message_name_the_code_uses_is_named_once_in_each_catalog() {
        check_names(crate::compiler::routing::root()).unwrap();
    }

    #[test]
    fn names_are_collected_from_source_and_refused_in_linker_scripts() {
        let directory = std::env::temp_dir().join(format!("alchemy-names-{}", std::process::id()));
        let _ = fs::remove_dir_all(&directory);
        fs::create_dir_all(directory.join("SRC")).unwrap();
        fs::write(
            directory.join("SRC/SHOW.C"),
            "extern char MsgHpRecover;\nvoid Msg_Show(void) { Show(&MsgHpRecover, MsgX_y); }\n",
        )
        .unwrap();
        fs::write(
            directory.join("MAIN.LD"),
            "MsgHpFull = 0x820;\nValue_1 = 1;\n",
        )
        .unwrap();
        let (mut used, mut problems) = (BTreeSet::new(), Vec::new());
        used_names(
            &directory,
            crate::targets::target_for(crate::targets::DecompTargetId::TbsEn),
            &directory,
            &mut used,
            &mut problems,
        )
        .unwrap();
        fs::remove_dir_all(&directory).unwrap();
        assert_eq!(used.into_iter().collect::<Vec<_>>(), ["MsgHpRecover"]);
        assert_eq!(problems.len(), 1);
        assert!(problems[0].contains("MsgHpFull is numbered by hand"));
    }

    #[test]
    fn japanese_only_messages_use_only_the_active_catalog_and_macros_keep_names() {
        use crate::targets::{target_for, DecompTargetId};
        let root = tempfile::tempdir().unwrap();
        let path = root.path().join("RANGE_PAGE.C");
        fs::write(&path, "#if defined(TBS_EDITION_JA)\nextern char MsgCanBeUsed[];\n#define CLOSE() Show(MsgCanBeUsed)\n#else\nextern char MsgHpRecover[];\n#endif\n/* MsgCommentOnly */\nconst char *plain = \"MsgStringOnly\";\n").unwrap();
        let ja = selected_names(root.path(), target_for(DecompTargetId::TbsJa), &path).unwrap();
        let en = selected_names(root.path(), target_for(DecompTargetId::TbsEn), &path).unwrap();
        assert_eq!(ja, ["MsgCanBeUsed".into()].into());
        assert_eq!(en, ["MsgHpRecover".into()].into());
        assert!(missing_names("JA.PO", &ja, &["MsgCanBeUsed"].into()).is_empty());
        assert!(missing_names("EN.PO", &en, &["MsgHpRecover"].into()).is_empty());
        assert_eq!(
            missing_names("JA.PO", &ja, &BTreeSet::new()),
            ["JA.PO: no message is named MsgCanBeUsed"]
        );
        let assembly = root.path().join("MESSAGES.S");
        fs::write(&assembly, ".ifdef TBS_EDITION_JA\n.4byte MsgCanBeUsed\n.else\n.4byte MsgHpRecover\n.endif\n@ MsgCommentOnly\n.ascii \"@MsgStringOnly\"\n").unwrap();
        assert_eq!(
            selected_names(root.path(), target_for(DecompTargetId::TbsJa), &assembly).unwrap(),
            ja
        );
        assert_eq!(
            selected_names(root.path(), target_for(DecompTargetId::TbsEn), &assembly).unwrap(),
            en
        );
    }
}

#[cfg(test)]
mod archive_layout_tests {
    use ags::text::ARCHIVES;

    #[test]
    fn layouts_cover_each_registered_target_once() {
        let mut ids = ARCHIVES.iter().map(|spec| spec.target).collect::<Vec<_>>();
        ids.sort_unstable();
        ids.dedup();
        assert_eq!(ARCHIVES.len(), ids.len());
        assert_eq!(ids.len(), crate::targets::TARGET_IDS.len());
        for id in crate::targets::TARGET_IDS {
            let target = crate::targets::target_for(id);
            let spec = ARCHIVES
                .iter()
                .find(|spec| spec.target == id.as_str())
                .unwrap();
            assert_eq!(
                spec.output,
                format!(
                    "{}/TEXT/{}.PO",
                    target.game_dir(),
                    spec.language.to_uppercase()
                )
            );
        }
    }
}
