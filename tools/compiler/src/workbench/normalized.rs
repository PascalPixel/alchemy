use candidate_show::{
    insns::gas_function_insns,
    render::{align_streams, without_pc_offset},
};
use serde::{Deserialize, Serialize};
use std::path::Path;

#[derive(Debug, Clone, Deserialize, Serialize)]
pub struct NormalizedReport {
    pub schema_version: u32,
    pub symbol: String,
    pub target_instructions: usize,
    pub candidate_instructions: usize,
    pub aligned_rows: usize,
    pub exact_rows: usize,
    pub mismatch_runs: Vec<NormalizedMismatchRun>,
}

#[derive(Debug, Clone, Deserialize, Serialize)]
pub struct NormalizedMismatchRun {
    pub row_start: usize,
    pub row_end: usize,
    pub rows: usize,
    pub candidate_start: usize,
    pub candidate_end: usize,
    pub target_start: usize,
    pub target_end: usize,
    pub candidate: Vec<String>,
    pub target: Vec<String>,
}

pub fn compare(target: &Path, candidate: &Path, symbol: &str) -> Result<NormalizedReport, String> {
    let target = gas_function_insns(&read(target)?, symbol);
    let candidate = gas_function_insns(&read(candidate)?, symbol);
    let pairs = align_streams(&candidate, &target);
    let mut mismatch_runs = Vec::new();
    let mut row = 0;
    let mut candidate_index = 0;
    let mut target_index = 0;
    let mut exact_rows = 0;

    while row < pairs.len() {
        if row_matches(&pairs[row]) {
            exact_rows += 1;
            candidate_index += usize::from(pairs[row].0.is_some());
            target_index += usize::from(pairs[row].1.is_some());
            row += 1;
            continue;
        }

        let row_start = row;
        let candidate_start = candidate_index;
        let target_start = target_index;
        let mut candidate_lines = Vec::new();
        let mut target_lines = Vec::new();
        while row < pairs.len() && !row_matches(&pairs[row]) {
            if let Some(line) = &pairs[row].0 {
                candidate_lines.push(line.clone());
                candidate_index += 1;
            }
            if let Some(line) = &pairs[row].1 {
                target_lines.push(line.clone());
                target_index += 1;
            }
            row += 1;
        }
        mismatch_runs.push(NormalizedMismatchRun {
            row_start,
            row_end: row,
            rows: row - row_start,
            candidate_start,
            candidate_end: candidate_index,
            target_start,
            target_end: target_index,
            candidate: candidate_lines,
            target: target_lines,
        });
    }

    Ok(NormalizedReport {
        schema_version: 1,
        symbol: symbol.into(),
        target_instructions: target.len(),
        candidate_instructions: candidate.len(),
        aligned_rows: pairs.len(),
        exact_rows,
        mismatch_runs,
    })
}

fn row_matches(pair: &(Option<String>, Option<String>)) -> bool {
    match pair {
        (Some(candidate), Some(target)) => {
            without_pc_offset(candidate) == without_pc_offset(target)
        }
        (None, None) => true,
        _ => false,
    }
}

fn read(path: &Path) -> Result<String, String> {
    std::fs::read_to_string(path).map_err(|error| format!("{}: {error}", path.display()))
}

#[cfg(test)]
mod tests {
    use super::row_matches;

    #[test]
    fn ignores_rendered_pc_offsets() {
        let pair = (
            Some("ldr r0, [pc, #12] <pool>".into()),
            Some("ldr r0, [pc, #48] <pool>".into()),
        );
        assert!(row_matches(&pair));
    }

    #[test]
    fn gaps_and_register_changes_are_mismatches() {
        assert!(!row_matches(&(Some("movs r0, #1".into()), None)));
        assert!(!row_matches(&(
            Some("movs r0, #1".into()),
            Some("movs r1, #1".into()),
        )));
    }
}
