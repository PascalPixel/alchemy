use crate::c_items::{aggregate_types, items, line_marker, Kind};
use crate::compiler::plan::{source_to_assembly_plan, SourceToAssemblyPlanOptions};
use crate::compiler::routing::root;
use crate::targets::{decomp_target, DecompTarget};
use std::collections::{HashMap, HashSet};
use std::path::{Path, PathBuf};

const USAGE: &str = r#"usage: alchemy sizes DRAFT.c [SIZE] [--game tbs|tla]
Lists every struct and union DRAFT.c can see, its own and its headers',
with the size and alignment the build's own compiler gives it in the
game's English edition, and the file that defines it. With SIZE (bytes,
decimal or 0x hex), lists the types, and arrays of one type, whose unused
local reserves exactly SIZE bytes of stack frame, each measured by
compiling it: S2's unused local of a real type that explains a native
frame (tag it FAKEMATCH). A type of that size which the compiler keeps in a
register reserves nothing and is named apart. The game defaults from the
draft's path."#;

struct Options {
    draft: PathBuf,
    size: Option<u64>,
    game: &'static str,
}

fn parse(arguments: &[String]) -> Result<Options, String> {
    let (mut draft, mut size, mut game) = (None, None, None);
    let mut i = 0;
    while i < arguments.len() {
        match arguments[i].as_str() {
            "--game" => {
                i += 1;
                game = match arguments.get(i).map(String::as_str) {
                    Some("tbs") => Some("tbs"),
                    Some("tla") => Some("tla"),
                    _ => return Err(USAGE.into()),
                }
            }
            "-h" | "--help" => return Err(USAGE.into()),
            value if draft.is_none() => draft = Some(PathBuf::from(value)),
            value if size.is_none() => {
                let parsed = match value.strip_prefix("0x") {
                    Some(hex) => u64::from_str_radix(hex, 16),
                    None => value.parse(),
                };
                size = Some(parsed.map_err(|_| format!("{value}: not a size\n{USAGE}"))?);
            }
            _ => return Err(USAGE.into()),
        }
        i += 1;
    }
    let draft = draft.ok_or_else(|| USAGE.to_string())?;
    let text = draft.to_string_lossy();
    let game = game.unwrap_or(if text.contains("/tla/") { "tla" } else { "tbs" });
    Ok(Options { draft, size, game })
}

pub(crate) fn visible_types(expanded: &str) -> Vec<(String, String)> {
    let (mut file, mut seen, mut found) = (String::new(), HashSet::new(), Vec::new());
    for item in items(expanded) {
        if let Some(marked) = line_marker(&item) {
            file = marked;
            continue;
        }
        for name in aggregate_types(&item) {
            if seen.insert(name.clone()) {
                found.push((name, file.clone()));
            }
        }
    }
    found
}

pub(crate) fn declarations_only(draft: &str) -> String {
    let (mut out, mut from) = (String::new(), 0);
    for item in items(draft) {
        if let Kind::Function { body } = item.kind {
            let head = draft[from..item.start + body].trim_end();
            out.push_str(head);
            out.push(';');
            out.push_str(&"\n".repeat(draft[from + head.len()..item.end].matches('\n').count()));
            from = item.end;
        }
    }
    out.push_str(&draft[from..]);
    if !out.ends_with('\n') {
        out.push('\n');
    }
    out
}

pub(crate) fn layout_probe(types: &[&str]) -> String {
    let mut text = String::new();
    for (n, t) in types.iter().enumerate() {
        text.push_str(&format!("struct alchemy_align_{n} {{ char c; {t} x; }};\n"));
    }
    text.push_str("const unsigned long alchemy_probe[] = {\n");
    for (n, t) in types.iter().enumerate() {
        text.push_str(&format!("    sizeof({t}), (unsigned long)&((struct alchemy_align_{n} *)0)->x,\n"));
    }
    text.push_str("};\n");
    text
}

pub(crate) fn frame_probe(locals: &[String]) -> String {
    locals
        .iter()
        .enumerate()
        .map(|(n, local)| format!("void alchemy_frame_{n}(void)\n{{\n    {local};\n}}\n"))
        .collect()
}

pub(crate) fn probe_words(assembly: &str) -> Vec<u64> {
    let mut words = Vec::new();
    let mut lines = assembly.lines().map(str::trim).skip_while(|l| *l != "alchemy_probe:").skip(1);
    for line in lines.by_ref() {
        match line.strip_prefix(".word") {
            Some(value) => words.push(value.trim().parse().unwrap_or(0)),
            None => break,
        }
    }
    words
}

pub(crate) fn frames(assembly: &str) -> HashMap<usize, u64> {
    let lines: Vec<&str> = assembly.lines().map(str::trim).collect();
    let mut pools: HashMap<&str, Vec<i64>> = HashMap::new();
    let mut label = None;
    for line in &lines {
        if let Some(name) = line.strip_suffix(':') {
            label = Some(name);
        } else if let (Some(name), Some(value)) = (label, line.strip_prefix(".word")) {
            pools.entry(name).or_default().push(value.trim().parse().unwrap_or(0));
        } else if !line.starts_with('.') {
            label = None;
        }
    }
    let (mut found, mut current, mut registers) = (HashMap::new(), None, HashMap::new());
    for line in &lines {
        if let Some(n) = line.strip_prefix("alchemy_frame_").and_then(|l| l.strip_suffix(':')) {
            current = n.parse::<usize>().ok();
            registers.clear();
            if let Some(n) = current {
                found.insert(n, 0);
            }
            continue;
        }
        let Some(n) = current else { continue };
        let (mnemonic, operands) = line.split_once(char::is_whitespace).unwrap_or((line, ""));
        let operands: Vec<&str> = operands.split(',').map(str::trim).collect();
        let reserve = match (mnemonic, operands.as_slice()) {
            ("sub", ["sp", "sp", value] | ["sp", value]) => value.trim_start_matches('#').parse().unwrap_or(0),
            ("ldr", [register, place]) => {
                let (name, offset) = place.split_once('+').unwrap_or((place, "0"));
                let index = offset.parse::<usize>().unwrap_or(0) / 4;
                if let Some(value) = pools.get(name).and_then(|words| words.get(index)) {
                    registers.insert(register.to_string(), *value);
                }
                0
            }
            ("add", ["sp", "sp", register] | ["sp", register]) => {
                registers.get(*register).filter(|v| **v < 0).map_or(0, |v| v.unsigned_abs())
            }
            _ => 0,
        };
        *found.entry(n).or_insert(0) += reserve;
    }
    found
}

fn compile(root: &Path, target: DecompTarget, routing: &str, includes: &[String], source: &str) -> Result<String, String> {
    let directory = tempfile::Builder::new().prefix("alchemy-sizes-").tempdir().map_err(|e| e.to_string())?;
    let input = directory.path().join("probe.c");
    let output = directory.path().join("probe.s");
    std::fs::write(&input, source).map_err(|e| format!("{}: {e}", input.display()))?;
    let mut options = SourceToAssemblyPlanOptions::new(
        target.compiler,
        routing,
        input.to_string_lossy(),
        output.to_string_lossy(),
    );
    options.preprocessor_flags = vec![format!("-D{}=1", target.edition_define)];
    options.preprocessed_output = Some(directory.path().join("probe.i").to_string_lossy().into_owned());
    let mut steps = source_to_assembly_plan(&options)?;
    for flag in includes.iter().rev() {
        steps[0].insert(1, flag.clone());
    }
    for step in steps {
        crate::process::run(&step, root).map_err(|e| e.replace(&*input.to_string_lossy(), routing))?;
    }
    std::fs::read_to_string(&output).map_err(|e| format!("{}: {e}", output.display()))
}

fn shown(path: &str, roots: &[&Path]) -> String {
    roots
        .iter()
        .find_map(|r| Path::new(path).strip_prefix(r).ok())
        .map_or(path.to_string(), |p| p.display().to_string())
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    let options = parse(arguments)?;
    let root = root();
    let target = decomp_target(Some(&format!("{}-en", options.game)))?;
    let draft = std::fs::canonicalize(&options.draft).map_err(|e| format!("{}: {e}", options.draft.display()))?;
    let draft_text = std::fs::read_to_string(&draft).map_err(|e| format!("{}: {e}", draft.display()))?;
    let canonical_root = std::fs::canonicalize(root).map_err(|e| e.to_string())?;
    let routing = draft.strip_prefix(&canonical_root).unwrap_or(&draft).to_string_lossy().into_owned();
    let imports = crate::build_text::fresh_c_imports(root, target)?;
    let mut includes = Vec::new();
    if let Some(parent) = draft.parent() {
        includes.push(format!("-I{}", parent.display()));
    }
    if let Some(imports) = &imports {
        includes.push(format!("-I{}", imports.path().display()));
    }

    let mut expand = crate::compiler::preprocess::command(target, &routing, false)?;
    for flag in includes.iter().rev() {
        expand.insert(1, flag.clone());
    }
    expand.push(draft.to_string_lossy().into_owned());
    let types = visible_types(&crate::process::run(&expand, root)?);
    if types.is_empty() {
        println!("{} sees no struct or union types", options.draft.display());
        return Ok(());
    }
    let base = declarations_only(&draft_text);
    let names: Vec<&str> = types.iter().map(|(t, _)| t.as_str()).collect();
    let words = probe_words(&compile(root, target, &routing, &includes, &(base.clone() + &layout_probe(&names)))?);
    if words.len() != 2 * types.len() {
        return Err(format!("the layout probe gave {} words for {} types", words.len(), types.len()));
    }
    let roots = [root, canonical_root.as_path()];
    let mut layout: Vec<(u64, u64, &str, String)> = types
        .iter()
        .enumerate()
        .map(|(n, (t, file))| (words[2 * n], words[2 * n + 1], t.as_str(), shown(file, &roots)))
        .collect();
    layout.sort();

    let Some(gap) = options.size else {
        println!("{:>6} {:>5}  {:<40} defined in", "size", "align", "type");
        for (size, align, name, file) in &layout {
            println!("{size:>6} {align:>5}  {name:<40} {file}");
        }
        return Ok(());
    };
    let mut candidates = Vec::new();
    for (size, _, name, file) in layout.iter().filter(|(size, ..)| *size > 0 && gap % size == 0) {
        let count = gap / size;
        let (local, kind) = if count == 1 {
            (format!("{name} pad"), name.to_string())
        } else {
            (format!("{name} pad[{count}]"), format!("{name}[{count}]"))
        };
        candidates.push((local, kind, file.clone()));
    }
    let measured = if candidates.is_empty() {
        HashMap::new()
    } else {
        let locals: Vec<String> = candidates.iter().map(|(local, ..)| local.clone()).collect();
        frames(&compile(root, target, &routing, &includes, &(base + &frame_probe(&locals)))?)
    };
    let (mut fills, mut registers) = (Vec::new(), Vec::new());
    for (n, (local, kind, file)) in candidates.iter().enumerate() {
        match measured.get(&n).copied().unwrap_or(0) {
            reserved if reserved == gap => fills.push(format!("  {:<44} {file}", format!("{local};"))),
            0 => registers.push(kind.as_str()),
            _ => {}
        }
    }
    if fills.is_empty() {
        println!("no unused local of a type {} can see reserves exactly {gap} bytes", options.draft.display());
    } else {
        println!("unused locals that reserve exactly {gap} bytes of frame:");
        for line in fills {
            println!("{line}");
        }
    }
    if !registers.is_empty() {
        println!("{gap} bytes but kept in a register, reserving nothing: {}", registers.join(", "));
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn visible_types_come_from_the_expansion_with_their_files() {
        let expanded = "# 1 \"/r/draft.c\"\n# 1 \"/r/games/common/include/game/a.h\" 1\nstruct A { u8 a; };\ntypedef struct { u8 b; } B;\n# 3 \"/r/draft.c\" 2\nstruct A *Get(void);\nunion C { u8 c; u16 d; };\nvoid Use(void) { struct L { u8 l; } l; }\n";
        assert_eq!(
            visible_types(expanded),
            [
                ("struct A".to_string(), "/r/games/common/include/game/a.h".to_string()),
                ("B".to_string(), "/r/games/common/include/game/a.h".to_string()),
                ("union C".to_string(), "/r/draft.c".to_string()),
            ]
        );
    }

    #[test]
    fn probes_keep_declarations_and_replace_bodies() {
        assert_eq!(
            declarations_only("#include \"types.h\"\nstruct A { u8 a; };\ns32 Run(struct A *a)\n{\n    return a->a;\n}\nu8 gB;"),
            "#include \"types.h\"\nstruct A { u8 a; };\ns32 Run(struct A *a);\n\n\n\nu8 gB;\n"
        );
        assert_eq!(
            layout_probe(&["struct A", "B"]),
            "struct alchemy_align_0 { char c; struct A x; };\nstruct alchemy_align_1 { char c; B x; };\nconst unsigned long alchemy_probe[] = {\n    sizeof(struct A), (unsigned long)&((struct alchemy_align_0 *)0)->x,\n    sizeof(B), (unsigned long)&((struct alchemy_align_1 *)0)->x,\n};\n"
        );
        assert_eq!(frame_probe(&["struct A pad[2]".into()]), "void alchemy_frame_0(void)\n{\n    struct A pad[2];\n}\n");
    }

    const LAYOUT: &str = "\t.global\talchemy_probe\n\t.section .rodata\n\t.align\t2\n\t.type\t alchemy_probe,object\nalchemy_probe:\n\t.word\t4\n\t.word\t1\n\t.word\t8\n\t.word\t4\n\t.size\t alchemy_probe,16\n";
    const FRAMES: &str = "alchemy_frame_0:\n\tbx\tlr\n.Lfe1:\n\t.size\t alchemy_frame_0,.Lfe1-alchemy_frame_0\nalchemy_frame_1:\n\tsub\tsp, sp, #24\n\tadd\tsp, sp, #24\n\tbx\tlr\n.Lfe2:\nalchemy_frame_2:\n\tmov\tip, r7\n\tldr\tr7, .L7\n\tldr\tr3, .L7+4\n\tadd\tsp, sp, r7\n\tadd\tsp, sp, r3\n\tmov\tr7, ip\n\tbx\tlr\n.L8:\n\t.align\t2\n.L7:\n\t.word\t-1036\n\t.word\t1036\n.Lfe3:\n";

    #[test]
    fn reads_sizes_and_frames_from_the_compiler_output() {
        assert_eq!(probe_words(LAYOUT), [4, 1, 8, 4]);
        let found = frames(FRAMES);
        assert_eq!((found[&0], found[&1], found[&2]), (0, 24, 1036));
    }

    #[test]
    fn sizes_parse_decimal_and_hex() {
        let args = |list: &[&str]| list.iter().map(|s| s.to_string()).collect::<Vec<_>>();
        let options = parse(&args(&["recon/tla/en/main/080ce458.c", "0x18"])).unwrap();
        assert_eq!((options.size, options.game), (Some(24), "tla"));
        assert_eq!(parse(&args(&["a.c", "12", "--game", "tbs"])).unwrap().size, Some(12));
        assert!(parse(&args(&["a.c", "twelve"])).is_err());
    }
}
