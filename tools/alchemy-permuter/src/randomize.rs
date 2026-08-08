use std::collections::BTreeSet;
use std::fs;
use std::path::Path;

use shape_sweep::{apply_transform, TransformId};

use crate::perm::{
    materialize, IGNORE_END, IGNORE_START, PRETEND_END, PRETEND_START, RANDOM_END, RANDOM_START,
};

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Mutation {
    pub id: String,
    pub source: String,
}

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub enum PassId {
    TempForExpr,
    ExpandExpr,
    ReorderStmts,
    ReorderDecls,
    AddMask,
    XorZero,
    CastSimple,
    ReferToVar,
    FloatLiteral,
    RandomizeInternalType,
    RandomizeExternalType,
    RandomizeFunctionType,
    SplitAssignment,
    Sameline,
    InsertBlock,
    StructRef,
    EmptyStmt,
    Condition,
    MultZero,
    FactorMult,
    FactorShift,
    DummyCommaExpr,
    AddSelfAssignment,
    Commutative,
    AddSub,
    Inequalities,
    CompoundAssignment,
    RemoveAst,
    DuplicateAssignment,
    ChainAssignment,
    LongChainAssignment,
    PadVarDecl,
    Inline,
    VarCondBlock,
}

impl PassId {
    pub const ALL: [Self; 34] = [
        Self::TempForExpr,
        Self::ExpandExpr,
        Self::ReorderStmts,
        Self::ReorderDecls,
        Self::AddMask,
        Self::XorZero,
        Self::CastSimple,
        Self::ReferToVar,
        Self::FloatLiteral,
        Self::RandomizeInternalType,
        Self::RandomizeExternalType,
        Self::RandomizeFunctionType,
        Self::SplitAssignment,
        Self::Sameline,
        Self::InsertBlock,
        Self::StructRef,
        Self::EmptyStmt,
        Self::Condition,
        Self::MultZero,
        Self::FactorMult,
        Self::FactorShift,
        Self::DummyCommaExpr,
        Self::AddSelfAssignment,
        Self::Commutative,
        Self::AddSub,
        Self::Inequalities,
        Self::CompoundAssignment,
        Self::RemoveAst,
        Self::DuplicateAssignment,
        Self::ChainAssignment,
        Self::LongChainAssignment,
        Self::PadVarDecl,
        Self::Inline,
        Self::VarCondBlock,
    ];

    pub const fn name(self) -> &'static str {
        match self {
            Self::TempForExpr => "perm_temp_for_expr",
            Self::ExpandExpr => "perm_expand_expr",
            Self::ReorderStmts => "perm_reorder_stmts",
            Self::ReorderDecls => "perm_reorder_decls",
            Self::AddMask => "perm_add_mask",
            Self::XorZero => "perm_xor_zero",
            Self::CastSimple => "perm_cast_simple",
            Self::ReferToVar => "perm_refer_to_var",
            Self::FloatLiteral => "perm_float_literal",
            Self::RandomizeInternalType => "perm_randomize_internal_type",
            Self::RandomizeExternalType => "perm_randomize_external_type",
            Self::RandomizeFunctionType => "perm_randomize_function_type",
            Self::SplitAssignment => "perm_split_assignment",
            Self::Sameline => "perm_sameline",
            Self::InsertBlock => "perm_ins_block",
            Self::StructRef => "perm_struct_ref",
            Self::EmptyStmt => "perm_empty_stmt",
            Self::Condition => "perm_condition",
            Self::MultZero => "perm_mult_zero",
            Self::FactorMult => "perm_factor_mult",
            Self::FactorShift => "perm_factor_shift",
            Self::DummyCommaExpr => "perm_dummy_comma_expr",
            Self::AddSelfAssignment => "perm_add_self_assignment",
            Self::Commutative => "perm_commutative",
            Self::AddSub => "perm_add_sub",
            Self::Inequalities => "perm_inequalities",
            Self::CompoundAssignment => "perm_compound_assignment",
            Self::RemoveAst => "perm_remove_ast",
            Self::DuplicateAssignment => "perm_duplicate_assignment",
            Self::ChainAssignment => "perm_chain_assignment",
            Self::LongChainAssignment => "perm_long_chain_assignment",
            Self::PadVarDecl => "perm_pad_var_decl",
            Self::Inline => "perm_inline",
            Self::VarCondBlock => "perm_var_cond_block",
        }
    }

    const fn index(self) -> usize {
        self as usize
    }

    pub const fn description(self) -> &'static str {
        match self {
            Self::TempForExpr => "materialize an expression in a temporary",
            Self::ExpandExpr => "inline or expand a temporary expression",
            Self::ReorderStmts => "exchange adjacent source statements",
            Self::ReorderDecls => "exchange adjacent declarations",
            Self::AddMask => "add an identity-width integer mask",
            Self::XorZero => "add an identity xor with zero",
            Self::CastSimple => "insert a simple integer cast",
            Self::ReferToVar => "refer to a value through a local variable",
            Self::FloatLiteral => "change an equivalent floating literal spelling",
            Self::RandomizeInternalType => "change a local integer signedness",
            Self::RandomizeExternalType => "change an external declaration signedness",
            Self::RandomizeFunctionType => "change a function type signedness",
            Self::SplitAssignment => "split declaration initialization into assignment",
            Self::Sameline => "place adjacent statements on one source line",
            Self::InsertBlock => "surround a statement with a compound block",
            Self::StructRef => "exchange pointer and dereferenced struct syntax",
            Self::EmptyStmt => "insert a syntactic empty statement",
            Self::Condition => "change an equivalent truth-test spelling",
            Self::MultZero => "add an arithmetic identity expression",
            Self::FactorMult => "factor repeated addition into multiplication",
            Self::FactorShift => "exchange power-of-two multiplication and shift",
            Self::DummyCommaExpr => "prefix an expression with a comma identity",
            Self::AddSelfAssignment => "insert a self assignment",
            Self::Commutative => "exchange operands of a commutative operation",
            Self::AddSub => "exchange addition and subtraction spelling",
            Self::Inequalities => "reverse an inequality and its operands",
            Self::CompoundAssignment => "exchange compound and expanded assignment",
            Self::RemoveAst => "remove an empty or redundant syntax node",
            Self::DuplicateAssignment => "repeat an assignment",
            Self::ChainAssignment => "combine equal assignments into a chain",
            Self::LongChainAssignment => "combine three equal assignments into a chain",
            Self::PadVarDecl => "insert an unused local declaration",
            Self::Inline => "inline a single-use local temporary",
            Self::VarCondBlock => "duplicate a block across a variable condition",
        }
    }
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Weights {
    values: [u32; PassId::ALL.len()],
}

impl Weights {
    pub fn for_profile(profile: &str) -> Self {
        let mut values = [50u32; PassId::ALL.len()];
        for (pass, weight) in [
            (PassId::TempForExpr, 1000),
            (PassId::ExpandExpr, 200),
            (PassId::ReorderStmts, 100),
            (PassId::ReorderDecls, 100),
            (PassId::AddMask, 150),
            (PassId::XorZero, 100),
            (PassId::CastSimple, 100),
            (PassId::ReferToVar, 100),
            (PassId::FloatLiteral, 30),
            (PassId::RandomizeInternalType, 100),
            (PassId::RandomizeExternalType, 50),
            (PassId::RandomizeFunctionType, 50),
            (PassId::SplitAssignment, 100),
            (PassId::Sameline, 30),
            (PassId::InsertBlock, 100),
            (PassId::StructRef, 100),
            (PassId::EmptyStmt, 100),
            (PassId::Condition, 100),
            (PassId::MultZero, 50),
            (PassId::FactorMult, 50),
            (PassId::FactorShift, 50),
            (PassId::DummyCommaExpr, 50),
            (PassId::AddSelfAssignment, 50),
            (PassId::Commutative, 50),
            (PassId::AddSub, 50),
            (PassId::Inequalities, 50),
            (PassId::CompoundAssignment, 50),
            (PassId::RemoveAst, 50),
            (PassId::DuplicateAssignment, 50),
            (PassId::ChainAssignment, 50),
            (PassId::LongChainAssignment, 30),
            (PassId::PadVarDecl, 10),
            (PassId::Inline, 100),
            (PassId::VarCondBlock, 50),
        ] {
            values[pass.index()] = weight;
        }
        match profile {
            "ido" => {
                values[PassId::FloatLiteral.index()] = 100;
                values[PassId::Sameline.index()] = 100;
                values[PassId::Inline.index()] = 0;
                values[PassId::VarCondBlock.index()] = 0;
            }
            "mwcc" => {
                values[PassId::ChainAssignment.index()] = 100;
                for pass in [
                    PassId::CompoundAssignment,
                    PassId::EmptyStmt,
                    PassId::StructRef,
                    PassId::Sameline,
                    PassId::XorZero,
                ] {
                    values[pass.index()] = 5;
                }
                values[PassId::VarCondBlock.index()] = 10;
            }
            "gcc" => {
                for pass in [
                    PassId::AddMask,
                    PassId::XorZero,
                    PassId::ReferToVar,
                    PassId::FloatLiteral,
                    PassId::Sameline,
                    PassId::EmptyStmt,
                    PassId::Condition,
                    PassId::MultZero,
                    PassId::DummyCommaExpr,
                    PassId::AddSelfAssignment,
                    PassId::DuplicateAssignment,
                    PassId::PadVarDecl,
                ] {
                    values[pass.index()] = 5;
                }
            }
            _ => {}
        }
        Self { values }
    }

    pub fn from_settings(path: &Path, fallback_profile: &str) -> Result<Self, String> {
        if !path.is_file() {
            return Ok(Self::for_profile(fallback_profile));
        }
        let text =
            fs::read_to_string(path).map_err(|error| format!("{}: {error}", path.display()))?;
        let mut profile = fallback_profile.to_string();
        let mut overrides = Vec::<(String, u32)>::new();
        let mut section = String::new();
        for raw in text.lines() {
            let line = raw.split('#').next().unwrap_or("").trim();
            if line.starts_with('[') && line.ends_with(']') {
                section = line[1..line.len() - 1].trim().to_string();
                continue;
            }
            let Some((key, value)) = line.split_once('=') else {
                continue;
            };
            let key = key.trim().trim_matches('"');
            let value = value.trim().trim_matches('"');
            if section.is_empty() && key == "compiler_type" {
                profile = value.to_string();
            } else if section == "weight_overrides" {
                let parsed = value
                    .parse::<f64>()
                    .ok()
                    .filter(|weight| weight.is_finite() && *weight >= 0.0)
                    .ok_or_else(|| format!("invalid weight for {key}: {value}"))?;
                overrides.push((key.to_string(), (parsed * 10.0).round() as u32));
            }
        }
        let mut weights = Self::for_profile(&profile);
        for (name, value) in overrides {
            let pass = PassId::ALL
                .into_iter()
                .find(|pass| pass.name() == name)
                .ok_or_else(|| format!("unknown randomization pass {name}"))?;
            weights.values[pass.index()] = value;
        }
        Ok(weights)
    }

    fn get(&self, pass: PassId) -> u32 {
        self.values[pass.index()]
    }
}

pub fn pass_help() -> String {
    let mut output = String::from("alchemy-permuter randomization passes:\n");
    for pass in PassId::ALL {
        output.push_str(&format!("  {:34} {}\n", pass.name(), pass.description()));
    }
    output
}

#[derive(Clone, Copy)]
struct SplitMix64(u64);

impl SplitMix64 {
    fn next(&mut self) -> u64 {
        self.0 = self.0.wrapping_add(0x9e37_79b9_7f4a_7c15);
        let mut value = self.0;
        value = (value ^ (value >> 30)).wrapping_mul(0xbf58_476d_1ce4_e5b9);
        value = (value ^ (value >> 27)).wrapping_mul(0x94d0_49bb_1331_11eb);
        value ^ (value >> 31)
    }

    fn index(&mut self, length: usize) -> usize {
        (self.next() as usize) % length
    }
}

fn line_offsets(source: &str) -> Vec<(usize, usize)> {
    let mut lines = Vec::new();
    let mut start = 0usize;
    for (at, byte) in source.bytes().enumerate() {
        if byte == b'\n' {
            lines.push((start, at + 1));
            start = at + 1;
        }
    }
    if start < source.len() {
        lines.push((start, source.len()));
    }
    lines
}

fn replace_range(source: &str, start: usize, end: usize, replacement: &str) -> String {
    let mut output = String::with_capacity(source.len() + replacement.len());
    output.push_str(&source[..start]);
    output.push_str(replacement);
    output.push_str(&source[end..]);
    output
}

fn declaration(line: &str) -> bool {
    let trimmed = line.trim();
    [
        "s32 ", "u32 ", "s16 ", "u16 ", "s8 ", "u8 ", "int ", "void *",
    ]
    .iter()
    .any(|prefix| trimmed.starts_with(prefix))
        && trimmed.ends_with(';')
}

fn swap_adjacent_declarations(source: &str) -> Vec<Mutation> {
    let lines = line_offsets(source);
    let mut output = Vec::new();
    for (index, pair) in lines.windows(2).enumerate() {
        let first = &source[pair[0].0..pair[0].1];
        let second = &source[pair[1].0..pair[1].1];
        if declaration(first) && declaration(second) {
            output.push(Mutation {
                id: format!("swap-declarations-{index}"),
                source: format!(
                    "{}{}{}{}",
                    &source[..pair[0].0],
                    second,
                    first,
                    &source[pair[1].1..]
                ),
            });
        }
    }
    output
}

fn split_initializers(source: &str) -> Vec<Mutation> {
    let mut output = Vec::new();
    for (index, (start, end)) in line_offsets(source).into_iter().enumerate() {
        let line = &source[start..end];
        let trimmed = line.trim();
        if !declaration(line) || !trimmed.contains(" = ") {
            continue;
        }
        let Some((left, right)) = trimmed.trim_end_matches(';').split_once(" = ") else {
            continue;
        };
        let Some(name) = left.split_whitespace().last() else {
            continue;
        };
        if name.contains('*') || name.contains('[') {
            continue;
        }
        let indent = &line[..line.len() - line.trim_start().len()];
        let replacement = format!("{indent}{left};\n{indent}{name} = {right};\n");
        output.push(Mutation {
            id: format!("split-initializer-{index}"),
            source: replace_range(source, start, end, &replacement),
        });
    }
    output
}

fn expand_compound(source: &str) -> Vec<Mutation> {
    let mut output = Vec::new();
    let operators = [" &= ", " |= ", " += ", " -= ", " ^= ", " *= "];
    for (line_index, (start, end)) in line_offsets(source).into_iter().enumerate() {
        let line = &source[start..end];
        let trimmed = line.trim();
        for operator in operators {
            let Some((left, right)) = trimmed.trim_end_matches(';').split_once(operator) else {
                continue;
            };
            if left.is_empty() || right.is_empty() || left.contains(' ') {
                continue;
            }
            let indent = &line[..line.len() - line.trim_start().len()];
            let binary = operator.as_bytes()[1] as char;
            let replacement = format!("{indent}{left} = {left} {binary} ({right});\n");
            output.push(Mutation {
                id: format!("expand-compound-{line_index}-{binary}"),
                source: replace_range(source, start, end, &replacement),
            });
        }
    }
    output
}

fn commute_rhs(source: &str) -> Vec<Mutation> {
    let mut output = Vec::new();
    for (line_index, (start, end)) in line_offsets(source).into_iter().enumerate() {
        let line = &source[start..end];
        let trimmed = line.trim();
        let Some((left, rhs)) = trimmed.trim_end_matches(';').split_once(" = ") else {
            continue;
        };
        for operator in [" & ", " | ", " + ", " * ", " ^ "] {
            let Some((first, second)) = rhs.split_once(operator) else {
                continue;
            };
            if first.is_empty()
                || second.is_empty()
                || first.contains(operator)
                || second.contains(operator)
                || second.contains('?')
            {
                continue;
            }
            let indent = &line[..line.len() - line.trim_start().len()];
            let replacement = format!(
                "{indent}{left} = {}{operator}{};\n",
                second.trim(),
                first.trim()
            );
            output.push(Mutation {
                id: format!("commute-rhs-{line_index}-{}", operator.trim()),
                source: replace_range(source, start, end, &replacement),
            });
        }
    }
    output
}

fn type_variants(source: &str) -> Vec<Mutation> {
    let mut output = Vec::new();
    for (from, to) in [
        ("s32", "u32"),
        ("u32", "s32"),
        ("s16", "u16"),
        ("u16", "s16"),
    ] {
        let mut cursor = 0usize;
        let mut occurrence = 0usize;
        while let Some(relative) = source[cursor..].find(from) {
            let start = cursor + relative;
            let end = start + from.len();
            let boundary = (start == 0 || !source.as_bytes()[start - 1].is_ascii_alphanumeric())
                && (end == source.len() || !source.as_bytes()[end].is_ascii_alphanumeric());
            if boundary {
                output.push(Mutation {
                    id: format!("type-{from}-{to}-{occurrence}"),
                    source: replace_range(source, start, end, to),
                });
                occurrence += 1;
            }
            cursor = end;
        }
    }
    output
}

fn statement(line: &str) -> bool {
    let trimmed = line.trim();
    !trimmed.is_empty()
        && trimmed.ends_with(';')
        && !trimmed.starts_with('#')
        && !trimmed.starts_with("return ")
        && !declaration(line)
}

fn swap_adjacent_statements(source: &str) -> Vec<Mutation> {
    let lines = line_offsets(source);
    let mut output = Vec::new();
    for (index, pair) in lines.windows(2).enumerate() {
        let first = &source[pair[0].0..pair[0].1];
        let second = &source[pair[1].0..pair[1].1];
        if statement(first) && statement(second) {
            output.push(Mutation {
                id: format!("reorder-statements-{index}"),
                source: format!(
                    "{}{}{}{}",
                    &source[..pair[0].0],
                    second,
                    first,
                    &source[pair[1].1..]
                ),
            });
        }
    }
    output
}

fn rhs_variants(source: &str, label: &str, prefix: &str, suffix: &str) -> Vec<Mutation> {
    let mut output = Vec::new();
    for (index, (start, end)) in line_offsets(source).into_iter().enumerate() {
        let line = &source[start..end];
        let trimmed = line.trim();
        let (head, expression) = if let Some(rest) = trimmed.strip_prefix("return ") {
            ("return", rest.trim_end_matches(';'))
        } else if let Some((left, right)) = trimmed.trim_end_matches(';').split_once(" = ") {
            if left.contains("==") || right.is_empty() {
                continue;
            }
            (left, right)
        } else {
            continue;
        };
        if expression.is_empty() || expression.contains(prefix) {
            continue;
        }
        let indent = &line[..line.len() - line.trim_start().len()];
        let replacement = if head == "return" {
            format!("{indent}return {prefix}{expression}{suffix};\n")
        } else {
            format!("{indent}{head} = {prefix}{expression}{suffix};\n")
        };
        output.push(Mutation {
            id: format!("{label}-{index}"),
            source: replace_range(source, start, end, &replacement),
        });
    }
    output
}

fn same_line(source: &str) -> Vec<Mutation> {
    let lines = line_offsets(source);
    let mut output = Vec::new();
    for (index, pair) in lines.windows(2).enumerate() {
        let first = &source[pair[0].0..pair[0].1];
        let second = &source[pair[1].0..pair[1].1];
        if (statement(first) || declaration(first)) && (statement(second) || declaration(second)) {
            let joined = format!("{} {}\n", first.trim_end(), second.trim());
            output.push(Mutation {
                id: format!("sameline-{index}"),
                source: replace_range(source, pair[0].0, pair[1].1, &joined),
            });
        }
    }
    output
}

fn insert_blocks(source: &str) -> Vec<Mutation> {
    let mut output = Vec::new();
    for (index, (start, end)) in line_offsets(source).into_iter().enumerate() {
        let line = &source[start..end];
        if !statement(line) {
            continue;
        }
        let indent = &line[..line.len() - line.trim_start().len()];
        let replacement = format!("{indent}{{ {} }}\n", line.trim());
        output.push(Mutation {
            id: format!("insert-block-{index}"),
            source: replace_range(source, start, end, &replacement),
        });
    }
    output
}

fn struct_refs(source: &str) -> Vec<Mutation> {
    let bytes = source.as_bytes();
    let mut output = Vec::new();
    let mut occurrence = 0usize;
    for arrow in source.match_indices("->").map(|(at, _)| at) {
        let mut start = arrow;
        while start > 0 && (bytes[start - 1].is_ascii_alphanumeric() || bytes[start - 1] == b'_') {
            start -= 1;
        }
        let mut end = arrow + 2;
        while end < bytes.len() && (bytes[end].is_ascii_alphanumeric() || bytes[end] == b'_') {
            end += 1;
        }
        if start == arrow || end == arrow + 2 {
            continue;
        }
        let base = &source[start..arrow];
        let field = &source[arrow + 2..end];
        output.push(Mutation {
            id: format!("struct-ref-{occurrence}"),
            source: replace_range(source, start, end, &format!("(*{base}).{field}")),
        });
        occurrence += 1;
    }
    output
}

fn empty_statements(source: &str) -> Vec<Mutation> {
    let mut output = Vec::new();
    for (index, (start, _)) in line_offsets(source).into_iter().enumerate() {
        let line = &source[start..];
        let Some(nonspace) = line.find(|character: char| !character.is_whitespace()) else {
            continue;
        };
        let at = start + nonspace;
        if source.as_bytes().get(at) == Some(&b'}') {
            continue;
        }
        output.push(Mutation {
            id: format!("empty-statement-{index}"),
            source: replace_range(source, at, at, "; "),
        });
    }
    output
}

fn condition_variants(source: &str) -> Vec<Mutation> {
    let mut output = Vec::new();
    for (index, (start, _)) in source.match_indices("if (").enumerate() {
        let expression_start = start + 4;
        let Some(relative_end) = source[expression_start..].find(')') else {
            continue;
        };
        let end = expression_start + relative_end;
        let expression = source[expression_start..end].trim();
        if expression.is_empty() || expression.contains(" != 0") {
            continue;
        }
        output.push(Mutation {
            id: format!("condition-{index}"),
            source: replace_range(
                source,
                expression_start,
                end,
                &format!("({expression}) != 0"),
            ),
        });
    }
    output
}

fn factor_multiplication(source: &str) -> Vec<Mutation> {
    let mut output = Vec::new();
    for (index, (start, end)) in line_offsets(source).into_iter().enumerate() {
        let line = &source[start..end];
        let Some((left, right)) = line.split_once(" + ") else {
            continue;
        };
        let right_operand = right.trim().trim_end_matches(';');
        let left_operand = left
            .split(|character: char| character == '=' || character.is_whitespace())
            .rfind(|part| !part.is_empty())
            .unwrap_or("");
        if left_operand.is_empty() || left_operand != right_operand {
            continue;
        }
        let replacement = line.replacen(
            &format!("{left_operand} + {right_operand}"),
            &format!("{left_operand} * 2"),
            1,
        );
        output.push(Mutation {
            id: format!("factor-mult-{index}"),
            source: replace_range(source, start, end, &replacement),
        });
    }
    output
}

fn factor_shifts(source: &str) -> Vec<Mutation> {
    let mut output = Vec::new();
    for (index, needle) in [" * 2", " * 4", " * 8"].into_iter().enumerate() {
        let shift = index + 1;
        for (occurrence, (at, _)) in source.match_indices(needle).enumerate() {
            output.push(Mutation {
                id: format!("factor-shift-{shift}-{occurrence}"),
                source: replace_range(source, at, at + needle.len(), &format!(" << {shift}")),
            });
        }
    }
    output
}

fn add_sub_variants(source: &str) -> Vec<Mutation> {
    let mut output = Vec::new();
    for (index, (at, _)) in source.match_indices(" - ").enumerate() {
        let after = at + 3;
        let mut end = after;
        while source
            .as_bytes()
            .get(end)
            .is_some_and(|byte| byte.is_ascii_alphanumeric() || matches!(byte, b'_' | b'(' | b')'))
        {
            end += 1;
        }
        if end > after {
            output.push(Mutation {
                id: format!("add-sub-{index}"),
                source: replace_range(source, at, end, &format!(" + -({})", &source[after..end])),
            });
        }
    }
    output
}

fn inequality_variants(source: &str) -> Vec<Mutation> {
    let mut output = Vec::new();
    for (line_index, (start, end)) in line_offsets(source).into_iter().enumerate() {
        let line = &source[start..end];
        for (operator, reverse) in [
            (" < ", " > "),
            (" > ", " < "),
            (" <= ", " >= "),
            (" >= ", " <= "),
        ] {
            let Some((left, right)) = line.split_once(operator) else {
                continue;
            };
            let left_start = left
                .rfind(|character: char| !(character.is_ascii_alphanumeric() || character == '_'))
                .map_or(0, |at| at + 1);
            let right_end = right
                .find(|character: char| !(character.is_ascii_alphanumeric() || character == '_'))
                .unwrap_or(right.len());
            let lhs = &left[left_start..];
            let rhs = &right[..right_end];
            if lhs.is_empty() || rhs.is_empty() {
                continue;
            }
            let replacement = format!(
                "{}{}{}{}{}",
                &line[..left_start],
                rhs,
                reverse,
                lhs,
                &right[right_end..]
            );
            output.push(Mutation {
                id: format!("inequality-{line_index}-{}", operator.trim()),
                source: replace_range(source, start, end, &replacement),
            });
        }
    }
    output
}

fn collapse_compound(source: &str) -> Vec<Mutation> {
    let mut output = Vec::new();
    for (index, (start, end)) in line_offsets(source).into_iter().enumerate() {
        let line = &source[start..end];
        let trimmed = line.trim().trim_end_matches(';');
        let Some((left, right)) = trimmed.split_once(" = ") else {
            continue;
        };
        if left.contains(' ') || left.is_empty() {
            continue;
        }
        for operator in ['+', '-', '&', '|', '^', '*'] {
            let prefix = format!("{left} {operator} ");
            let Some(value) = right.strip_prefix(&prefix) else {
                continue;
            };
            let indent = &line[..line.len() - line.trim_start().len()];
            let replacement = format!("{indent}{left} {operator}= {value};\n");
            output.push(Mutation {
                id: format!("collapse-compound-{index}-{operator}"),
                source: replace_range(source, start, end, &replacement),
            });
        }
    }
    output
}

fn remove_empty_ast(source: &str) -> Vec<Mutation> {
    line_offsets(source)
        .into_iter()
        .enumerate()
        .filter(|(_, (start, end))| source[*start..*end].trim() == ";")
        .map(|(index, (start, end))| Mutation {
            id: format!("remove-empty-{index}"),
            source: replace_range(source, start, end, ""),
        })
        .collect()
}

fn duplicate_assignments(source: &str) -> Vec<Mutation> {
    let mut output = Vec::new();
    for (index, (start, end)) in line_offsets(source).into_iter().enumerate() {
        let line = &source[start..end];
        let trimmed = line.trim();
        if !statement(line) || !trimmed.contains(" = ") || trimmed.contains("==") {
            continue;
        }
        let replacement = format!("{}{}", line, line);
        output.push(Mutation {
            id: format!("duplicate-assignment-{index}"),
            source: replace_range(source, start, end, &replacement),
        });
    }
    output
}

fn assignment_parts(line: &str) -> Option<(&str, &str)> {
    let trimmed = line.trim().trim_end_matches(';');
    let (left, right) = trimmed.split_once(" = ")?;
    (!left.contains(' ') && !left.is_empty() && !right.is_empty()).then_some((left, right))
}

fn chain_assignments(source: &str, length: usize) -> Vec<Mutation> {
    let lines = line_offsets(source);
    let mut output = Vec::new();
    for (index, window) in lines.windows(length).enumerate() {
        let parts = window
            .iter()
            .map(|(start, end)| assignment_parts(&source[*start..*end]))
            .collect::<Option<Vec<_>>>();
        let Some(parts) = parts else {
            continue;
        };
        if parts.windows(2).any(|pair| pair[0].1 != pair[1].1) {
            continue;
        }
        let indent = &source[window[0].0..window[0].1];
        let indent = &indent[..indent.len() - indent.trim_start().len()];
        let lefts = parts
            .iter()
            .map(|part| part.0)
            .collect::<Vec<_>>()
            .join(" = ");
        let replacement = format!("{indent}{lefts} = {};\n", parts[0].1);
        output.push(Mutation {
            id: format!("chain-{length}-{index}"),
            source: replace_range(source, window[0].0, window[length - 1].1, &replacement),
        });
    }
    output
}

fn pad_variable(source: &str) -> Vec<Mutation> {
    let Some((at, _)) = source
        .match_indices('{')
        .find(|(at, _)| source[..*at].contains(')'))
    else {
        return Vec::new();
    };
    vec![Mutation {
        id: "pad-variable".into(),
        source: replace_range(source, at + 1, at + 1, "\n    s32 permuter_pad;"),
    }]
}

fn self_assignments(source: &str) -> Vec<Mutation> {
    let mut output = Vec::new();
    for (index, (start, end)) in line_offsets(source).into_iter().enumerate() {
        let line = &source[start..end];
        let Some((left, _)) = assignment_parts(line) else {
            continue;
        };
        let indent = &line[..line.len() - line.trim_start().len()];
        output.push(Mutation {
            id: format!("self-assignment-{index}"),
            source: replace_range(source, start, start, &format!("{indent}{left} = {left};\n")),
        });
    }
    output
}

fn variable_condition_blocks(source: &str) -> Vec<Mutation> {
    let Some(variable) = line_offsets(source).into_iter().find_map(|(start, end)| {
        let line = &source[start..end];
        declaration(line)
            .then(|| {
                line.trim()
                    .trim_end_matches(';')
                    .split(|character: char| character.is_whitespace() || character == '=')
                    .nth(1)
                    .unwrap_or("")
                    .trim_matches('*')
                    .to_string()
            })
            .filter(|name| !name.is_empty())
    }) else {
        return Vec::new();
    };
    line_offsets(source)
        .into_iter()
        .enumerate()
        .filter_map(|(index, (start, end))| {
            let line = &source[start..end];
            statement(line).then(|| {
                let indent = &line[..line.len() - line.trim_start().len()];
                let body = line.trim();
                Mutation {
                    id: format!("variable-condition-{index}"),
                    source: replace_range(
                        source,
                        start,
                        end,
                        &format!("{indent}if ({variable}) {{ {body} }} else {{ {body} }}\n"),
                    ),
                }
            })
        })
        .collect()
}

fn float_literals(source: &str) -> Vec<Mutation> {
    let bytes = source.as_bytes();
    let mut output = Vec::new();
    let mut at = 0usize;
    let mut occurrence = 0usize;
    while at < bytes.len() {
        if bytes[at].is_ascii_digit() {
            let start = at;
            while at < bytes.len() && bytes[at].is_ascii_digit() {
                at += 1;
            }
            if bytes.get(at) == Some(&b'.') {
                at += 1;
                while at < bytes.len() && bytes[at].is_ascii_digit() {
                    at += 1;
                }
                let has_f = matches!(bytes.get(at), Some(b'f' | b'F'));
                output.push(Mutation {
                    id: format!("float-literal-{occurrence}"),
                    source: if has_f {
                        replace_range(source, at, at + 1, "")
                    } else {
                        replace_range(source, at, at, "f")
                    },
                });
                occurrence += 1;
            }
            if at == start {
                at += 1;
            }
        } else {
            at += 1;
        }
    }
    output
}

fn pass_variants(pass: PassId, source: &str) -> Vec<Mutation> {
    let mut variants = match pass {
        PassId::TempForExpr | PassId::ReferToVar => split_initializers(source),
        PassId::ExpandExpr | PassId::Inline => TransformId::ALL
            .into_iter()
            .filter(|id| {
                matches!(
                    id,
                    TransformId::InlineSingleUseTemp | TransformId::HoistArgsAfterFirst
                )
            })
            .filter_map(|id| {
                apply_transform(id, source).map(|source| Mutation {
                    id: format!("{}-{}", pass.name(), id.id()),
                    source,
                })
            })
            .collect(),
        PassId::ReorderStmts => swap_adjacent_statements(source),
        PassId::ReorderDecls => swap_adjacent_declarations(source),
        PassId::AddMask => rhs_variants(source, "add-mask", "(", ") & ~0u"),
        PassId::XorZero => rhs_variants(source, "xor-zero", "(", ") ^ 0"),
        PassId::CastSimple => rhs_variants(source, "cast-simple", "(s32)(", ")"),
        PassId::FloatLiteral => float_literals(source),
        PassId::RandomizeInternalType
        | PassId::RandomizeExternalType
        | PassId::RandomizeFunctionType => type_variants(source),
        PassId::SplitAssignment => split_initializers(source),
        PassId::Sameline => same_line(source),
        PassId::InsertBlock => insert_blocks(source),
        PassId::StructRef => struct_refs(source),
        PassId::EmptyStmt => empty_statements(source),
        PassId::Condition => condition_variants(source),
        PassId::MultZero => rhs_variants(source, "arithmetic-identity", "(", ") + 0"),
        PassId::FactorMult => factor_multiplication(source),
        PassId::FactorShift => factor_shifts(source),
        PassId::DummyCommaExpr => rhs_variants(source, "dummy-comma", "(0, ", ")"),
        PassId::AddSelfAssignment => self_assignments(source),
        PassId::Commutative => commute_rhs(source),
        PassId::AddSub => add_sub_variants(source),
        PassId::Inequalities => inequality_variants(source),
        PassId::CompoundAssignment => {
            let mut variants = expand_compound(source);
            variants.extend(collapse_compound(source));
            variants
        }
        PassId::RemoveAst => remove_empty_ast(source),
        PassId::DuplicateAssignment => duplicate_assignments(source),
        PassId::ChainAssignment => chain_assignments(source, 2),
        PassId::LongChainAssignment => chain_assignments(source, 3),
        PassId::PadVarDecl => pad_variable(source),
        PassId::VarCondBlock => variable_condition_blocks(source),
    };
    for variant in &mut variants {
        variant.id = format!("{}:{}", pass.name(), variant.id);
    }
    variants
}

fn source_variants_with_weights(source: &str, weights: &Weights) -> Vec<Mutation> {
    let mut output = Vec::new();
    for pass in PassId::ALL {
        if weights.get(pass) > 0 {
            output.extend(pass_variants(pass, source));
        }
    }
    for id in TransformId::ALL {
        if let Some(mutated) = apply_transform(id, source) {
            if mutated != source {
                output.push(Mutation {
                    id: format!("shape-{}", id.id()),
                    source: mutated,
                });
            }
        }
    }
    output
}

fn fingerprint(text: &str) -> u64 {
    text.bytes().fold(0xcbf2_9ce4_8422_2325, |hash, byte| {
        (hash ^ byte as u64).wrapping_mul(0x100_0000_01b3)
    })
}

fn marker_spans(
    source: &str,
    start_marker: &str,
    end_marker: &str,
) -> Vec<(usize, usize, usize, usize)> {
    let mut spans = Vec::new();
    let mut cursor = 0usize;
    while let Some(relative_start) = source[cursor..].find(start_marker) {
        let marker_start = cursor + relative_start;
        let content_start = marker_start + start_marker.len();
        let mut depth = 1usize;
        let mut search = content_start;
        while depth > 0 {
            let next_start = source[search..].find(start_marker).map(|at| search + at);
            let next_end = source[search..].find(end_marker).map(|at| search + at);
            match (next_start, next_end) {
                (_, None) => return spans,
                (Some(start), Some(end)) if start < end => {
                    depth += 1;
                    search = start + start_marker.len();
                }
                (_, Some(end)) => {
                    depth -= 1;
                    if depth == 0 {
                        spans.push((marker_start, content_start, end, end + end_marker.len()));
                        cursor = end + end_marker.len();
                    } else {
                        search = end + end_marker.len();
                    }
                }
            }
        }
    }
    spans
}

fn subtract_ranges(base: &[(usize, usize)], removed: &[(usize, usize)]) -> Vec<(usize, usize)> {
    let mut output = Vec::new();
    for (start, end) in base {
        let mut cursor = *start;
        for (remove_start, remove_end) in removed {
            if *remove_end <= cursor || *remove_start >= *end {
                continue;
            }
            if *remove_start > cursor {
                output.push((cursor, (*remove_start).min(*end)));
            }
            cursor = cursor.max(*remove_end);
            if cursor >= *end {
                break;
            }
        }
        if cursor < *end {
            output.push((cursor, *end));
        }
    }
    output
}

fn randomizable_ranges(source: &str) -> Vec<(usize, usize)> {
    let random = marker_spans(source, RANDOM_START, RANDOM_END);
    let base = if random.is_empty() {
        vec![(0, source.len())]
    } else {
        random
            .into_iter()
            .map(|(_, content_start, content_end, _)| (content_start, content_end))
            .collect()
    };
    let mut protected = marker_spans(source, IGNORE_START, IGNORE_END)
        .into_iter()
        .chain(marker_spans(source, PRETEND_START, PRETEND_END))
        .map(|(start, _, _, end)| (start, end))
        .collect::<Vec<_>>();
    protected.sort_unstable();
    subtract_ranges(&base, &protected)
        .into_iter()
        .filter(|(start, end)| {
            source[*start..*end]
                .chars()
                .any(|character| !character.is_whitespace())
        })
        .collect()
}

fn marked_variants(source: &str, weights: &Weights) -> Vec<Mutation> {
    let mut output = Vec::new();
    for (region, (start, end)) in randomizable_ranges(source).into_iter().enumerate() {
        for variant in source_variants_with_weights(&source[start..end], weights) {
            output.push(Mutation {
                id: format!("region-{region}:{}", variant.id),
                source: replace_range(source, start, end, &variant.source),
            });
        }
    }
    output
}

fn mutation_weight(mutation: &Mutation, weights: &Weights) -> u32 {
    PassId::ALL
        .into_iter()
        .find(|pass| mutation.id.contains(pass.name()))
        .map_or(10, |pass| weights.get(pass))
}

fn weighted_index(variants: &[Mutation], weights: &Weights, random: &mut SplitMix64) -> usize {
    let total = variants
        .iter()
        .map(|variant| mutation_weight(variant, weights) as u64)
        .sum::<u64>();
    if total == 0 {
        return random.index(variants.len());
    }
    let mut choice = random.next() % total;
    for (index, variant) in variants.iter().enumerate() {
        let weight = mutation_weight(variant, weights) as u64;
        if choice < weight {
            return index;
        }
        choice -= weight;
    }
    variants.len() - 1
}

pub(crate) fn mutate_marked_with_weights(
    source: &str,
    seed: u64,
    limit: usize,
    weights: &Weights,
) -> Vec<Mutation> {
    if limit == 0 {
        return Vec::new();
    }
    let identity = materialize(source);
    let mut output = vec![("identity".to_string(), source.to_string())];
    let mut seen = BTreeSet::from([identity]);
    for mutation in marked_variants(source, weights) {
        let rendered = materialize(&mutation.source);
        if seen.insert(rendered) {
            output.push((mutation.id, mutation.source));
            if output.len() == limit {
                return output
                    .into_iter()
                    .map(|(id, source)| Mutation {
                        id,
                        source: materialize(&source),
                    })
                    .collect();
            }
        }
    }
    let mut random = SplitMix64(seed ^ fingerprint(source));
    let mut misses = 0usize;
    while output.len() < limit && misses < limit.saturating_mul(20).max(100) {
        let parent_index = random.index(output.len());
        let parent = output[parent_index].clone();
        let variants = marked_variants(&parent.1, weights);
        if variants.is_empty() {
            misses += 1;
            continue;
        }
        let child = &variants[weighted_index(&variants, weights, &mut random)];
        let rendered = materialize(&child.source);
        if seen.insert(rendered) {
            output.push((format!("{}+{}", parent.0, child.id), child.source.clone()));
            misses = 0;
        } else {
            misses += 1;
        }
    }
    output
        .into_iter()
        .map(|(id, source)| Mutation {
            id,
            source: materialize(&source),
        })
        .collect()
}

pub(crate) fn mutate_marked(source: &str, seed: u64, limit: usize) -> Vec<Mutation> {
    mutate_marked_with_weights(source, seed, limit, &Weights::for_profile("gcc"))
}

pub fn mutate(source: &str, seed: u64, limit: usize) -> Vec<Mutation> {
    mutate_marked(source, seed, limit)
}

pub fn self_test() -> Result<(), String> {
    let source = "void f(void) {\n    s32 a = 1;\n    s32 b;\n    b = x & y;\n    b += 2;\n}\n";
    let first = mutate(source, 7, 40);
    let second = mutate(source, 7, 40);
    if first != second || first.len() < 6 {
        return Err("mutation plan is not deterministic or non-degenerate".into());
    }
    if first[0].id != "identity" || first[0].source != source {
        return Err("identity candidate is not first".into());
    }
    let unique = first
        .iter()
        .map(|item| item.source.as_str())
        .collect::<BTreeSet<_>>();
    if unique.len() != first.len() {
        return Err("mutation plan contains duplicate sources".into());
    }
    let names = PassId::ALL
        .into_iter()
        .map(PassId::name)
        .collect::<BTreeSet<_>>();
    if names.len() != PassId::ALL.len()
        || !names.contains("perm_temp_for_expr")
        || !names.contains("perm_var_cond_block")
    {
        return Err("randomization pass registry is incomplete or duplicated".into());
    }
    let permutation = crate::perm::parse(
        "outside = 1; PERM_RANDOMIZE(inside = x & y;) PERM_IGNORE(protected = a & b;)",
    )?;
    let marked = permutation.evaluate_marked(0)?.0;
    let regional = marked_variants(&marked, &Weights::for_profile("gcc"));
    if regional.is_empty()
        || regional.iter().any(|variant| {
            let rendered = materialize(&variant.source);
            !rendered.starts_with("outside = 1;") || !rendered.ends_with("protected = a & b;")
        })
    {
        return Err("PERM_RANDOMIZE/IGNORE region boundary was not preserved".into());
    }
    Ok(())
}
