//! Small, deterministic GNU gettext PO reader and writer.
//!
//! It deliberately supports the ordinary singular catalog subset used by
//! game message tables: headers, comments, flags, contexts, ids and strings.
//! Paths, languages and message semantics remain the caller's responsibility.

use super::{err, AssetError};
use std::collections::BTreeMap;

#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Entry {
    pub comments: Vec<String>,
    pub flags: Vec<String>,
    pub context: Option<String>,
    pub id: String,
    pub value: String,
}

#[derive(Clone, Debug, Default, PartialEq, Eq)]
pub struct Catalog {
    pub headers: BTreeMap<String, String>,
    pub entries: Vec<Entry>,
}

fn quoted(text: &str) -> String {
    let mut output = String::from("\"");
    for character in text.chars() {
        match character {
            '\\' => output.push_str("\\\\"),
            '"' => output.push_str("\\\""),
            '\n' => output.push_str("\\n"),
            '\r' => output.push_str("\\r"),
            '\t' => output.push_str("\\t"),
            value => output.push(value),
        }
    }
    output.push('"');
    output
}

fn field(output: &mut String, name: &str, value: &str) {
    if !value.contains('\n') {
        output.push_str(name);
        output.push(' ');
        output.push_str(&quoted(value));
        output.push('\n');
        return;
    }
    output.push_str(name);
    output.push_str(" \"\"\n");
    for part in value.split_inclusive('\n') {
        output.push_str(&quoted(part));
        output.push('\n');
    }
    if !value.ends_with('\n') {
        return;
    }
}

pub fn write(catalog: &Catalog) -> String {
    let mut output = String::new();
    output.push_str("msgid \"\"\nmsgstr \"\"\n");
    for (name, value) in &catalog.headers {
        output.push_str(&quoted(&format!("{name}: {value}\n")));
        output.push('\n');
    }
    for entry in &catalog.entries {
        output.push('\n');
        for comment in &entry.comments {
            output.push_str("#. ");
            output.push_str(comment);
            output.push('\n');
        }
        if !entry.flags.is_empty() {
            output.push_str("#, ");
            output.push_str(&entry.flags.join(", "));
            output.push('\n');
        }
        if let Some(context) = &entry.context {
            field(&mut output, "msgctxt", context);
        }
        field(&mut output, "msgid", &entry.id);
        field(&mut output, "msgstr", &entry.value);
    }
    output
}

fn unquote(text: &str) -> Result<String, AssetError> {
    let bytes = text.trim().as_bytes();
    if bytes.len() < 2 || bytes[0] != b'"' || bytes[bytes.len() - 1] != b'"' {
        return err("PO string is not quoted");
    }
    let mut output = String::new();
    let mut characters = text.trim()[1..text.trim().len() - 1].chars();
    while let Some(character) = characters.next() {
        if character != '\\' {
            output.push(character);
            continue;
        }
        output.push(
            match characters
                .next()
                .ok_or_else(|| AssetError("PO escape is incomplete".into()))?
            {
                '\\' => '\\',
                '"' => '"',
                'n' => '\n',
                'r' => '\r',
                't' => '\t',
                other => return err(format!("unsupported PO escape \\{other}")),
            },
        );
    }
    Ok(output)
}

#[derive(Clone, Copy)]
enum Active {
    Context,
    Id,
    Value,
}

pub fn read(text: &str) -> Result<Catalog, AssetError> {
    let mut catalog = Catalog::default();
    let mut comments = Vec::new();
    let mut flags = Vec::new();
    let mut context: Option<String> = None;
    let mut id: Option<String> = None;
    let mut value: Option<String> = None;
    let mut active: Option<Active> = None;
    let finish = |catalog: &mut Catalog,
                  comments: &mut Vec<String>,
                  flags: &mut Vec<String>,
                  context: &mut Option<String>,
                  id: &mut Option<String>,
                  value: &mut Option<String>|
     -> Result<(), AssetError> {
        let Some(id_value) = id.take() else {
            comments.clear();
            flags.clear();
            *context = None;
            *value = None;
            return Ok(());
        };
        let value_value = value
            .take()
            .ok_or_else(|| AssetError("PO entry has no msgstr".into()))?;
        if id_value.is_empty() && context.is_none() {
            for line in value_value.lines() {
                if let Some((name, value)) = line.split_once(':') {
                    catalog
                        .headers
                        .insert(name.trim().into(), value.trim().into());
                }
            }
        } else {
            catalog.entries.push(Entry {
                comments: std::mem::take(comments),
                flags: std::mem::take(flags),
                context: context.take(),
                id: id_value,
                value: value_value,
            });
        }
        Ok(())
    };
    for line in text.lines().chain(std::iter::once("")) {
        let trimmed = line.trim();
        if trimmed.is_empty() {
            finish(
                &mut catalog,
                &mut comments,
                &mut flags,
                &mut context,
                &mut id,
                &mut value,
            )?;
            active = None;
        } else if let Some(comment) = trimmed.strip_prefix("#.") {
            comments.push(comment.trim().into());
        } else if let Some(list) = trimmed.strip_prefix("#,") {
            flags.extend(
                list.split(',')
                    .map(|flag| flag.trim().to_string())
                    .filter(|flag| !flag.is_empty()),
            );
        } else if trimmed.starts_with('#') {
            continue;
        } else if let Some(text) = trimmed.strip_prefix("msgctxt ") {
            context = Some(unquote(text)?);
            active = Some(Active::Context);
        } else if let Some(text) = trimmed.strip_prefix("msgid ") {
            id = Some(unquote(text)?);
            active = Some(Active::Id);
        } else if let Some(text) = trimmed.strip_prefix("msgstr ") {
            value = Some(unquote(text)?);
            active = Some(Active::Value);
        } else if trimmed.starts_with('"') {
            let continuation = unquote(trimmed)?;
            match active.ok_or_else(|| AssetError("orphan PO continuation".into()))? {
                Active::Context => context.as_mut(),
                Active::Id => id.as_mut(),
                Active::Value => value.as_mut(),
            }
            .ok_or_else(|| AssetError("PO continuation has no field".into()))?
            .push_str(&continuation);
        } else {
            return err(format!("unsupported PO line: {trimmed}"));
        }
    }
    Ok(catalog)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn catalogs_round_trip_multiline_text_and_null_flags() {
        let mut catalog = Catalog::default();
        catalog.headers.insert("Language".into(), "fr".into());
        catalog.entries.push(Entry {
            comments: vec!["physical key 7".into()],
            flags: vec!["alchemy-null".into()],
            context: Some("message".into()),
            id: "00007".into(),
            value: "une ligne\n\"deux\"".into(),
        });
        assert_eq!(read(&write(&catalog)).unwrap(), catalog);
    }
}
