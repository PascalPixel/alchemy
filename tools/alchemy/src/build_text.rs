use crate::compiler::routing::CompilerTarget;
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
    write_c_imports(output, &c_imports(target, &source.names))
}

fn write_generated(path: &Path, contents: &str) -> Result<(), String> {
    if fs::read_to_string(path).ok().as_deref() == Some(contents) {
        return Ok(());
    }
    fs::create_dir_all(path.parent().expect("include directory"))
        .map_err(|error| error.to_string())?;
    fs::write(path, contents).map_err(|error| format!("{}: {error}", path.display()))
}

/// The C declarations of an edition's messages. ☀️'s header also names each
/// message as its value, for the reason the comment it writes gives; ⚓️'s
/// code still writes its own casts.
fn c_imports(target: DecompTarget, names: &[(String, usize)]) -> String {
    let as_value = target.compiler == CompilerTarget::Tbs;
    let mut header = String::from(
        "/* Message numbers from the edition's editable PO catalog. */\n\
         #ifndef ALCHEMY_MESSAGE_IDS_H\n\
         #define ALCHEMY_MESSAGE_IDS_H\n\
         #include \"types.h\"\n\
         #define MSGNO(name) TEXT_MESSAGE_NUMBER_ ## name\n",
    );
    for (name, number) in names {
        header.push_str(&format!("#define TEXT_MESSAGE_NUMBER_{name} {number}\n"));
    }
    if as_value {
        header.push_str(
            "/* A message's name is its linker symbol, whose value is the message's\n\
             \x20* number. C can name a link-time value only as an address, so each\n\
             \x20* name below stands for that address read as an s32. A macro is not\n\
             \x20* expanded again inside its own replacement (C89 3.8.3.4): the name in\n\
             \x20* the cast is the symbol, which the code still loads from a relocated\n\
             \x20* pool word. */\n",
        );
    }
    for (name, _) in names {
        header.push_str(&format!("extern u8 {name}[];\n"));
        if as_value {
            header.push_str(&format!("#define {name} ((s32){name})\n"));
        }
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
    Ok(c_imports(target, &names))
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

#[cfg(test)]
mod tests {
    use super::*;
    use crate::targets::{target_for, DecompTargetId};
    use std::process::Command;

    fn names() -> Vec<(String, usize)> {
        vec![("MsgFirst".to_owned(), 7), (text::END.to_owned(), 9)]
    }

    #[test]
    fn tbs_message_names_are_their_values() {
        let target = target_for(DecompTargetId::TbsEn);
        let header = c_imports(target, &names());
        let lines: Vec<&str> = header.lines().collect();
        for name in ["MsgFirst", text::END] {
            let at = lines
                .iter()
                .position(|line| *line == format!("extern u8 {name}[];"))
                .unwrap();
            assert_eq!(lines[at + 1], format!("#define {name} ((s32){name})"));
        }
        let directory = tempfile::tempdir().unwrap();
        write_c_imports(directory.path(), &header).unwrap();
        let source = directory.path().join("use.c");
        fs::write(
            &source,
            "#include \"text/msg_ids.h\"\n\
             s32 value = MsgFirst + 1;\n\
             s32 number = MSGNO(MsgFirst);\n\
             #if MSGNO(MsgEnd) != 9\n#error MSGNO\n#endif\n",
        )
        .unwrap();
        let mut command =
            crate::compiler::preprocess::command(target, source.to_str().unwrap(), false).unwrap();
        command.insert(1, format!("-I{}", directory.path().display()));
        command.push(source.to_string_lossy().into_owned());
        let output = Command::new(&command[0]).args(&command[1..]).output().unwrap();
        assert!(output.status.success(), "{}", String::from_utf8_lossy(&output.stderr));
        let code: String = String::from_utf8_lossy(&output.stdout).split_whitespace().collect();
        assert!(code.contains("s32value=((s32)MsgFirst)+1;"), "{code}");
        assert!(code.contains("s32number=7;"), "{code}");
    }

    #[test]
    fn tla_message_names_stay_arrays() {
        let header = c_imports(target_for(DecompTargetId::TlaEn), &names());
        assert!(header.contains("extern u8 MsgFirst[];\n"));
        assert!(header.contains("#define TEXT_MESSAGE_NUMBER_MsgFirst 7\n"));
        assert!(!header.contains("#define MsgFirst "));
    }
}
