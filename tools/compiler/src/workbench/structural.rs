use objdiff_core::{
    diff::{diff_objs, ArmArchVersion, DiffObjConfig, DiffSide, InstructionDiffKind, MappingConfig},
    obj,
};
use serde::{Deserialize, Serialize};
use std::path::Path;

#[derive(Debug, Clone, Deserialize, Serialize)]
pub struct StructuralReport {
    pub schema_version: u32,
    pub architecture: String,
    pub symbol: String,
    pub match_percent: f32,
    pub exact: bool,
    pub instructions: usize,
    pub unchanged: usize,
    pub argument_mismatches: usize,
    pub opcode_mismatches: usize,
    pub replacements: usize,
    pub deletions: usize,
    pub insertions: usize,
}

pub fn compare(target: &Path, candidate: &Path, symbol: &str) -> Result<StructuralReport, String> {
    let mut config = DiffObjConfig { arm_arch_version: ArmArchVersion::V4t, ..Default::default() };
    // GCC 2.96 emits pre-UAL syntax. This is the same setting that won the
    // repository's objdiff A/B trial and avoids formatting-only differences.
    config.arm_unified_syntax = false;

    let target_obj = obj::read::read(target, &config, DiffSide::Target).map_err(|error| format!("{}: {error}", target.display()))?;
    let candidate_obj = obj::read::read(candidate, &config, DiffSide::Base).map_err(|error| format!("{}: {error}", candidate.display()))?;
    let symbol_index = target_obj.symbol_by_name(symbol).ok_or_else(|| format!("{}: missing symbol {symbol}", target.display()))?;
    let candidate_symbol = candidate_symbol_name(&candidate_obj, symbol).ok_or_else(|| format!("{}: missing symbol {symbol}", candidate.display()))?;
    let mut mappings = MappingConfig::default();
    if candidate_symbol != symbol {
        mappings.mappings.insert(symbol.to_owned(), candidate_symbol);
    }
    let result = diff_objs(Some(&target_obj), Some(&candidate_obj), None, &config, &mappings).map_err(|error| format!("objdiff: {error}"))?;
    let target_diff = result.left.ok_or("objdiff omitted target result")?;
    let symbol_diff = target_diff.symbols.get(symbol_index).ok_or_else(|| format!("objdiff omitted symbol {symbol}"))?;
    let mut report = StructuralReport {
        schema_version: 1,
        architecture: "armv4t".into(),
        symbol: symbol.into(),
        match_percent: symbol_diff.match_percent.unwrap_or(0.0),
        exact: false,
        instructions: symbol_diff.instruction_rows.len(),
        unchanged: 0,
        argument_mismatches: 0,
        opcode_mismatches: 0,
        replacements: 0,
        deletions: 0,
        insertions: 0,
    };
    for row in &symbol_diff.instruction_rows {
        let argument_mismatch = row.kind == InstructionDiffKind::ArgMismatch || row.arg_diff.iter().any(|index| index.is_some());
        if argument_mismatch {
            report.argument_mismatches += 1;
        }
        match row.kind {
            InstructionDiffKind::None if !argument_mismatch => report.unchanged += 1,
            InstructionDiffKind::None => {}
            InstructionDiffKind::ArgMismatch => {}
            InstructionDiffKind::OpMismatch => report.opcode_mismatches += 1,
            InstructionDiffKind::Replace => report.replacements += 1,
            InstructionDiffKind::Delete => report.deletions += 1,
            InstructionDiffKind::Insert => report.insertions += 1,
        }
    }
    report.exact = report.match_percent == 100.0 && report.argument_mismatches == 0 && report.opcode_mismatches == 0 && report.replacements == 0 && report.deletions == 0 && report.insertions == 0;
    Ok(report)
}

fn candidate_symbol_name(object: &obj::Object, canonical: &str) -> Option<String> {
    if object.symbol_by_name(canonical).is_some() {
        return Some(canonical.to_owned());
    }
    let mut defined = object.symbols.iter().filter(|symbol| symbol.size > 0 && symbol.name.starts_with("Func_")).map(|symbol| symbol.name.as_str());
    let only = defined.next()?;
    defined.next().is_none().then(|| only.to_owned())
}

#[cfg(test)]
mod tests {
    use super::candidate_symbol_name;
    use objdiff_core::obj::{Object, Symbol};

    #[test]
    fn falls_back_to_the_only_defined_candidate_owner() {
        let mut object = Object::default();
        object.symbols.push(Symbol { name: "Func_08000000".into(), size: 4, section: Some(0), ..Default::default() });
        assert_eq!(candidate_symbol_name(&object, "HumanOwner"), Some("Func_08000000".into()));
    }
}
