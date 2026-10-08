use crate::targets::DecompTarget;
use ags::text::{self, ARCHIVES};
use std::fs;
use std::path::Path;

pub(crate) const INCLUDE: &str = "text/messages.inc";

pub(crate) const C_INCLUDE: &str = "text/msg_ids.h";

const LABEL: &str = "Text_Message";

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

fn c_imports(names: &[(String, usize)]) -> String {
    let mut header = String::from(
        "/* Message numbers from the edition's editable PO catalog. */\n\
         #ifndef ALCHEMY_MESSAGE_IDS_H\n\
         #define ALCHEMY_MESSAGE_IDS_H\n\
         #include \"types.h\"\n\
         #define TEXT_MESSAGE_ENUM(name) enum { name = TEXT_MESSAGE_NUMBER_ ## name }\n\
         #define MSGNO(name) TEXT_MESSAGE_NUMBER_ ## name\n",
    );
    for (name, number) in names {
        header.push_str(&format!("#define TEXT_MESSAGE_NUMBER_{name} {number}\n"));
    }
    for (name, _) in names {
        header.push_str(&format!("extern u8 {name}[];\n"));
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
