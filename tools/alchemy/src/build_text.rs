//! The message archive's build rule. Each game's `TEXT/MESSAGES.S` includes
//! `text/messages.inc`, which `build rom` writes from the edition's PO catalog
//! as pret's mapjson writes the assembly its data files include: the Huffman
//! archive, whose address words name its own labels, and an absolute symbol
//! `Msg<Name>` for each message the code names, set to that message's number
//! in this edition. The linker places the archive and fills every literal
//! pool that loads a message number.
use crate::assets::text::{self, ARCHIVES};
use crate::targets::DecompTarget;
use std::fmt::Write;
use std::fs;
use std::path::Path;

/// The generated include, under the build directory the assembler searches.
pub(crate) const INCLUDE: &str = "text/messages.inc";

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
    let mut assembly = format!(
        "@ {}'s message archive and message numbers, built from {}.\n",
        spec.target, spec.output
    );
    assembly.push_str(&text::archive(&source, LABEL)?.source()?);
    for (name, number) in &source.names {
        writeln!(assembly, "\t.global {name}\n\t.set {name}, {number}").unwrap();
    }
    let path = output.join(INCLUDE);
    if fs::read_to_string(&path).ok().as_deref() == Some(assembly.as_str()) {
        return Ok(());
    }
    fs::create_dir_all(path.parent().expect("include directory"))
        .map_err(|error| error.to_string())?;
    fs::write(&path, assembly).map_err(|error| format!("{}: {error}", path.display()))
}
