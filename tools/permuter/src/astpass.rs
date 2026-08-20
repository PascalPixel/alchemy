//! AST-level randomization engine: a port of pret decomp-permuter's
//! `randomizer.py` (MIT) over the `lang-c` AST.
//!
//! The engine holds the candidate as emitted C text. Each `randomize()`
//! call parses the text, picks a weighted random pass (pret's `gcc`
//! weights), runs it -- a pass that raises `Fail` before mutating leaves
//! the tree untouched and another pass is picked, exactly like pret's
//! retry loop -- and re-emits via `cemit`. Mutations therefore stack
//! across calls. PERM pragma regions are a stub: the randomization region
//! is always unbounded.
//!
//! Probability constants are pret's, ported verbatim.

use std::collections::{BTreeMap, BTreeSet};

use lang_c::ast::*;
use lang_c::span::Node;

use crate::asttypes::{
    allowed_basic_type, build_typemap, decayed_expr_type, ensure, expr_type, is_assign_op,
    pointer_decay, resolve_typedefs, same_type, CType, Fail, TypeMap,
};
use crate::astutil::{
    self, binop, equal_expr, expr_stmt, find_var_reads_in_expr, id_expr, insert_block_item,
    insertion_points, int_expr, int_expr_full, is_effectful, is_lvalue, n, nested_blocks, nid,
    scan_item_exprs, scan_stmt_exprs, unary, visit_decls_mut, visit_stmts, visit_stmts_mut,
    walk_stmt_exprs, with_block, InsPoint, Nid,
};
use crate::cemit::emit_translation_unit;

// Probability constants, ported from pret's randomizer.py.
const PROB_RANDOMIZE_TYPE: f64 = 0.3;
const PROB_REUSE_VAR: f64 = 0.5;
const PROB_INS_BLOCK_DOWHILE: f64 = 0.5;
const PROB_TEMP_PTR: f64 = 0.05;
const PROB_TEMP_ASSIGN_AT_FIRST_USE: f64 = 0.1;
const PROB_TEMP_REPLACE_ALL: f64 = 0.2;
const PROB_TEMP_REPLACE_MOST: f64 = 0.2;
const PROB_EXPAND_REPLACE_ALL: f64 = 0.3;
const PROB_KEEP_REPLACED_VAR: f64 = 0.2;

const MAX_INDEX: usize = usize::MAX / 2;
const INT_TYPES: [&str; 6] = ["int", "char", "long", "short", "signed", "unsigned"];

// ------------------------------------------------------------------- rng

/// SplitMix64, the crate's deterministic generator (same constants as
/// `randomize.rs`), with the distribution helpers the ported passes need.
pub struct Rng {
    state: u64,
    /// Optional heat context: (fractional differing-row positions, source
    /// length). When set, span-weighted selection biases toward hot regions.
    pub heat: Option<(Vec<f32>, usize)>,
}

impl Rng {
    pub fn new(seed: u64) -> Rng {
        Rng {
            state: seed,
            heat: None,
        }
    }
    fn next(&mut self) -> u64 {
        self.state = self.state.wrapping_add(0x9e37_79b9_7f4a_7c15);
        let mut value = self.state;
        value = (value ^ (value >> 30)).wrapping_mul(0xbf58_476d_1ce4_e5b9);
        value = (value ^ (value >> 27)).wrapping_mul(0x94d0_49bb_1331_11eb);
        value ^ (value >> 31)
    }
    pub fn f(&mut self) -> f64 {
        (self.next() >> 11) as f64 / (1u64 << 53) as f64
    }
    pub fn chance(&mut self, prob: f64) -> bool {
        self.f() < prob
    }
    /// Uniform index in `0..len`; `len` must be non-zero.
    pub fn index(&mut self, len: usize) -> usize {
        (self.next() % len as u64) as usize
    }
    /// Inclusive range, like Python's randint.
    pub fn randint(&mut self, lo: usize, hi: usize) -> usize {
        if hi <= lo {
            return lo;
        }
        lo + self.index(hi - lo + 1)
    }
    /// Exclusive range, like Python's randrange.
    pub fn randrange(&mut self, lo: usize, hi: usize) -> usize {
        if hi <= lo {
            return lo;
        }
        lo + self.index(hi - lo)
    }
    pub fn span_weight(&self, offset: usize) -> f64 {
        let Some((heat, len)) = &self.heat else {
            return 1.0;
        };
        let f = offset as f32 / (*len).max(1) as f32;
        let near = heat.iter().filter(|h| (*h - f).abs() <= 0.03).count();
        1.0 + (4 * near.min(2)) as f64
    }

    /// Weighted index over parallel span offsets; uniform when no heat.
    pub fn weighted_index_by_span(&mut self, spans: &[usize]) -> usize {
        if self.heat.is_none() || spans.len() <= 1 {
            return self.index(spans.len());
        }
        let weights: Vec<f64> = spans.iter().map(|s| self.span_weight(*s)).collect();
        let total: f64 = weights.iter().sum();
        let mut roll = self.f() * total;
        for (i, w) in weights.iter().enumerate() {
            if roll < *w {
                return i;
            }
            roll -= *w;
        }
        spans.len() - 1
    }

    /// Heat-weighted choice keyed by each item's source span start; uniform
    /// when no heat context is installed.
    pub fn choice_by_span<'a, T>(&mut self, items: &'a [T], span: fn(&T) -> usize) -> &'a T {
        if self.heat.is_none() || items.len() <= 1 {
            return self.choice(items);
        }
        let spans: Vec<usize> = items.iter().map(span).collect();
        &items[self.weighted_index_by_span(&spans)]
    }

    pub fn choice<'a, T>(&mut self, items: &'a [T]) -> &'a T {
        &items[self.index(items.len())]
    }
}

/// Port of `random_weighted`.
fn random_weighted<'a, T>(rng: &mut Rng, values: &'a [(T, f64)]) -> &'a T {
    let sum: f64 = values.iter().map(|(_, p)| p.max(0.0)).sum();
    let target = rng.f() * sum;
    let mut acc = 0.0;
    for (v, p) in values {
        acc += p.max(0.0);
        if acc > target {
            return v;
        }
    }
    for (v, p) in values {
        if *p > 0.0 {
            return v;
        }
    }
    &values[0].0
}

// ---------------------------------------------------------------- passes

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum AstPass {
    TempForExpr,
    ExpandExpr,
    ReorderStmts,
    ReorderDecls,
    AddMask,
    XorZero,
    CastSimple,
    SplitAssignment,
    InsBlock,
    EmptyStmt,
    Condition,
    MultZero,
    Commutative,
    AddSub,
    Inequalities,
    CompoundAssignment,
}

impl AstPass {
    pub fn name(self) -> &'static str {
        match self {
            AstPass::TempForExpr => "perm_temp_for_expr",
            AstPass::ExpandExpr => "perm_expand_expr",
            AstPass::ReorderStmts => "perm_reorder_stmts",
            AstPass::ReorderDecls => "perm_reorder_decls",
            AstPass::AddMask => "perm_add_mask",
            AstPass::XorZero => "perm_xor_zero",
            AstPass::CastSimple => "perm_cast_simple",
            AstPass::SplitAssignment => "perm_split_assignment",
            AstPass::InsBlock => "perm_ins_block",
            AstPass::EmptyStmt => "perm_empty_stmt",
            AstPass::Condition => "perm_condition",
            AstPass::MultZero => "perm_mult_zero",
            AstPass::Commutative => "perm_commutative",
            AstPass::AddSub => "perm_add_sub",
            AstPass::Inequalities => "perm_inequalities",
            AstPass::CompoundAssignment => "perm_compound_assignment",
        }
    }
    pub fn from_name(name: &str) -> Option<AstPass> {
        DEFAULT_GCC_WEIGHTS
            .iter()
            .find(|(p, _)| p.name() == name)
            .map(|(p, _)| *p)
    }
}

/// pret's `default_weights.toml` `[base]` values with the `[gcc]`
/// overrides applied, restricted to the ported passes.
pub const DEFAULT_GCC_WEIGHTS: [(AstPass, f64); 16] = [
    (AstPass::TempForExpr, 100.0),
    (AstPass::ExpandExpr, 20.0),
    (AstPass::ReorderStmts, 10.0),
    (AstPass::ReorderDecls, 10.0),
    (AstPass::AddMask, 0.5),
    (AstPass::XorZero, 0.5),
    (AstPass::CastSimple, 10.0),
    (AstPass::SplitAssignment, 10.0),
    (AstPass::InsBlock, 10.0),
    (AstPass::EmptyStmt, 0.5),
    (AstPass::Condition, 0.5),
    (AstPass::MultZero, 0.5),
    (AstPass::Commutative, 5.0),
    (AstPass::AddSub, 5.0),
    (AstPass::Inequalities, 5.0),
    (AstPass::CompoundAssignment, 5.0),
];

// ---------------------------------------------------------------- engine

pub struct AstRandomizer {
    source: String,
    fn_name: String,
    rng: Rng,
    only: Option<AstPass>,
    /// Fractional positions of currently-differing candidate rows; when
    /// non-empty, expression and statement selection is biased toward source
    /// regions near them (positions map proportionally, which is crude but
    /// monotone for a single switch-shaped function).
    heat: Vec<f32>,
}

fn parse_unit(source: &str) -> Result<TranslationUnit, String> {
    let config = lang_c::driver::Config {
        flavor: lang_c::driver::Flavor::GnuC11,
        cpp_command: String::new(),
        ..Default::default()
    };
    lang_c::driver::parse_preprocessed(&config, source.to_string())
        .map(|p| p.unit)
        .map_err(|e| format!("parse failed: {e}"))
}

fn find_fn(unit: &TranslationUnit, name: &str) -> Result<usize, String> {
    for (i, ext) in unit.0.iter().enumerate() {
        if let ExternalDeclaration::FunctionDefinition(f) = &ext.node {
            let (_, fname) =
                crate::asttypes::apply_declarator(CType::int(), &f.node.declarator.node);
            if fname.as_deref() == Some(name) {
                return Ok(i);
            }
        }
    }
    Err(format!("function {name} not found"))
}

fn fdef(unit: &TranslationUnit, index: usize) -> &FunctionDefinition {
    match &unit.0[index].node {
        ExternalDeclaration::FunctionDefinition(f) => &f.node,
        _ => unreachable!("fn_index points at a function definition"),
    }
}

fn fbody_mut(unit: &mut TranslationUnit, index: usize) -> &mut Node<Statement> {
    match &mut unit.0[index].node {
        ExternalDeclaration::FunctionDefinition(f) => &mut f.node.statement,
        _ => unreachable!("fn_index points at a function definition"),
    }
}

fn call_sequence(body: &Node<Statement>) -> Vec<String> {
    let mut calls = Vec::new();
    scan_stmt_exprs(body, &mut |expression, _| {
        if let Expression::Call(call) = &expression.node {
            calls.push(crate::cemit::emit_expression(&call.node.callee.node));
        }
    });
    calls
}

impl AstRandomizer {
    /// Parses and re-emits the input so that every later parse yields a
    /// fully braced tree with genuine, unique spans. The target function is
    /// the last function definition in the unit.
    pub fn set_heat(&mut self, heat: Vec<f32>) {
        self.heat = heat;
    }

    pub fn new(source: &str, seed: u64, only: Option<AstPass>) -> Result<AstRandomizer, String> {
        let unit = parse_unit(source)?;
        let mut fn_name = None;
        for ext in &unit.0 {
            if let ExternalDeclaration::FunctionDefinition(f) = &ext.node {
                let (_, name) =
                    crate::asttypes::apply_declarator(CType::int(), &f.node.declarator.node);
                fn_name = name;
            }
        }
        let fn_name = fn_name.ok_or_else(|| "no function definition in source".to_string())?;
        let normalized = emit_translation_unit(&unit);
        // Sanity: the normalized text must reparse.
        parse_unit(&normalized)?;
        Ok(AstRandomizer {
            source: normalized,
            fn_name,
            rng: Rng::new(seed),
            only,
            heat: Vec::new(),
        })
    }

    pub fn source(&self) -> &str {
        &self.source
    }

    /// One mutation: pick weighted passes until one succeeds, then emit.
    pub fn randomize(&mut self) -> Result<String, String> {
        self.randomize_named().map(|(source, _)| source)
    }

    /// As `randomize`, also naming the pass that produced the mutation.
    pub fn randomize_named(&mut self) -> Result<(String, &'static str), String> {
        self.rng.heat = if self.heat.is_empty() {
            None
        } else {
            Some((self.heat.clone(), self.source.len()))
        };
        let mut unit = parse_unit(&self.source)?;
        let fn_index = find_fn(&unit, &self.fn_name)?;
        let calls = call_sequence(&fdef(&unit, fn_index).statement);
        for _ in 0..1000 {
            let pass = match self.only {
                Some(p) => p,
                None => *random_weighted(&mut self.rng, &DEFAULT_GCC_WEIGHTS),
            };
            if run_pass(pass, &mut unit, fn_index, &mut self.rng).is_ok() {
                // A source mutation may alter register pressure; it may not
                // add, remove, or reorder observable calls. The compiled BL
                // guard compares against the reference and can therefore
                // accept a semantically wrong move when that move happens to
                // improve alignment. This invariant compares with the walk's
                // own parent and rejects the lie before invoking gcc.
                if call_sequence(&fdef(&unit, fn_index).statement) != calls {
                    unit = parse_unit(&self.source)?;
                    continue;
                }
                let out = emit_translation_unit(&unit);
                self.source = out.clone();
                return Ok((out, pass.name()));
            }
        }
        Err("no randomization pass succeeded in 1000 attempts".to_string())
    }
}

pub fn run_pass(
    pass: AstPass,
    unit: &mut TranslationUnit,
    fn_index: usize,
    rng: &mut Rng,
) -> Result<(), Fail> {
    match pass {
        AstPass::TempForExpr => perm_temp_for_expr(unit, fn_index, rng),
        AstPass::ExpandExpr => perm_expand_expr(unit, fn_index, rng),
        AstPass::ReorderStmts => perm_reorder_stmts(unit, fn_index, rng),
        AstPass::ReorderDecls => perm_reorder_decls(unit, fn_index, rng),
        AstPass::AddMask => perm_add_mask(unit, fn_index, rng),
        AstPass::XorZero => perm_xor_zero(unit, fn_index, rng),
        AstPass::CastSimple => perm_cast_simple(unit, fn_index, rng),
        AstPass::SplitAssignment => perm_split_assignment(unit, fn_index, rng),
        AstPass::InsBlock => perm_ins_block(unit, fn_index, rng),
        AstPass::EmptyStmt => perm_empty_stmt(unit, fn_index, rng),
        AstPass::Condition => perm_condition(unit, fn_index, rng),
        AstPass::MultZero => perm_mult_zero(unit, fn_index, rng),
        AstPass::Commutative => perm_commutative(unit, fn_index, rng),
        AstPass::AddSub => perm_add_sub(unit, fn_index, rng),
        AstPass::Inequalities => perm_inequalities(unit, fn_index, rng),
        AstPass::CompoundAssignment => perm_compound_assignment(unit, fn_index, rng),
    }
}

fn target_fn_name(unit: &TranslationUnit, fn_index: usize) -> String {
    crate::asttypes::apply_declarator(CType::int(), &fdef(unit, fn_index).declarator.node)
        .1
        .unwrap_or_default()
}

fn typemap_for(unit: &TranslationUnit, fn_index: usize) -> TypeMap {
    build_typemap(unit, &target_fn_name(unit, fn_index))
}

/// All is_expr positions in the function body: `(id, clone)` pairs, in
/// traversal order (port of `get_block_expressions`; the region is always
/// unbounded).
fn block_expressions(body: &Node<Statement>) -> Vec<(Nid, Node<Expression>)> {
    let mut out = Vec::new();
    scan_stmt_exprs(body, &mut |e, is_expr| {
        if is_expr {
            out.push((nid(e), e.clone()));
        }
    });
    out
}

/// Replaces the expression with the given id, handing the old node to the
/// builder. Returns whether a replacement happened.
fn replace_expr_by_id(
    body: &mut Node<Statement>,
    target: Nid,
    build: &mut dyn FnMut(Node<Expression>) -> Expression,
) -> bool {
    let mut done = false;
    walk_stmt_exprs(body, &mut |e, _| {
        if !done && nid(e) == target {
            let old = std::mem::replace(e, n(id_expr("")));
            *e = n(build(old));
            done = true;
            return true;
        }
        false
    });
    done
}

fn ensure_arithmetic_type(e: &Expression, tm: &TypeMap) -> Result<(), Fail> {
    let t = decayed_expr_type(e, tm)?;
    ensure(allowed_basic_type(
        &t,
        tm,
        &[
            "int", "char", "long", "short", "signed", "unsigned", "float", "double",
        ],
    ))
}

fn find_next(list: &[usize], value: usize) -> Option<usize> {
    match list.binary_search(&value) {
        Ok(i) => Some(list[i]),
        Err(i) => list.get(i).copied(),
    }
}

fn bisect_left(list: &[usize], value: usize) -> usize {
    match list.binary_search(&value) {
        Ok(mut i) => {
            while i > 0 && list[i - 1] == value {
                i -= 1;
            }
            i
        }
        Err(i) => i,
    }
}

// ------------------------------------------------------------ type dice

/// Port of `random_type`: a random integer type, half the time unsigned,
/// half the time volatile.
fn random_type(rng: &mut Rng) -> CType {
    let mut names: Vec<String> = Vec::new();
    if rng.chance(0.5) {
        names.push("unsigned".to_string());
    }
    let widths: [(&[&str], f64); 5] = [
        (&["char"], 1.0),
        (&["short"], 1.0),
        (&["int"], 2.0),
        (&["long"], 0.5),
        (&["long", "long"], 0.5),
    ];
    let picked: &&[&str] = random_weighted(rng, &widths);
    names.extend(picked.iter().map(|s| s.to_string()));
    let volatile = rng.chance(0.5);
    CType::Basic { names, volatile }
}

/// Port of `randomize_type`.
fn randomize_type(
    t: &CType,
    tm: &TypeMap,
    rng: &mut Rng,
    ensure_changed: bool,
) -> Result<CType, Fail> {
    if allowed_basic_type(t, tm, &INT_TYPES) {
        return Ok(random_type(rng));
    }
    ensure(!ensure_changed)?;
    Ok(t.clone())
}

// -------------------------------------------------------- temp_for_expr

/// Port of `surrounding_writes`.
fn surrounding_writes(
    expr: &Node<Expression>,
    base_start: usize,
    writes: &std::collections::BTreeMap<String, Vec<usize>>,
) -> (usize, usize) {
    let mut prev: isize = -1;
    let mut next = MAX_INDEX;
    for (name, _) in find_var_reads_in_expr(expr) {
        if let Some(wr) = writes.get(&name) {
            let i = bisect_left(wr, base_start);
            if i > 0 {
                prev = prev.max(wr[i - 1] as isize);
            }
            if i < wr.len() {
                next = next.min(wr[i]);
            }
        }
    }
    (if prev < 0 { 0 } else { prev as usize }, next)
}

#[derive(Clone)]
struct Place {
    block: Nid,
    index: usize,
    stmt_start: usize,
}

/// Port of `pick_random_subset`.
fn pick_random_subset(rng: &mut Rng, cands: &[Nid], mid: usize) -> Vec<Nid> {
    let all_before = &cands[..mid];
    let all_after = &cands[mid + 1..];
    let prob = if rng.chance(0.5) { 0.5 } else { 0.0 };
    let mut before: Vec<Nid> = all_before
        .iter()
        .copied()
        .filter(|_| rng.chance(prob))
        .collect();
    let mut after: Vec<Nid> = all_after
        .iter()
        .copied()
        .filter(|_| rng.chance(prob))
        .collect();
    if rng.chance(PROB_TEMP_REPLACE_ALL) {
        before = all_before.to_vec();
        after = all_after.to_vec();
    } else if rng.chance(PROB_TEMP_REPLACE_MOST) {
        if rng.chance(0.5) {
            before = all_before.to_vec();
        } else {
            after = all_after.to_vec();
        }
    }
    let mut out = before;
    out.push(cands[mid]);
    out.extend(after);
    out
}

fn crosses_control_entry(place_start: usize, expression_start: usize, entries: &[usize]) -> bool {
    entries
        .iter()
        .any(|entry| place_start <= *entry && *entry < expression_start)
}

/// Port of `maybe_reuse_var`.
#[allow(clippy::too_many_arguments)]
fn maybe_reuse_var(
    var: Option<&String>,
    assign_before_start: usize,
    expr_start: usize,
    t: &CType,
    reads: &std::collections::BTreeMap<String, Vec<usize>>,
    writes: &std::collections::BTreeMap<String, Vec<usize>>,
    tm: &TypeMap,
    rng: &mut Rng,
) -> Option<String> {
    let take = rng.chance(PROB_REUSE_VAR);
    let var = var?;
    if !take {
        return None;
    }
    let var_type = pointer_decay(tm.var_types.get(var)?, tm);
    if !same_type(&var_type, t, tm, true) {
        return None;
    }
    let empty = Vec::new();
    let write = find_next(writes.get(var).unwrap_or(&empty), assign_before_start);
    let read = find_next(reads.get(var).unwrap_or(&empty), assign_before_start);
    if let Some(r) = read {
        if write.map(|w| w >= r).unwrap_or(true) {
            // We don't want to overwrite a variable which we later read,
            // unless we write to it before that read.
            return None;
        }
    }
    if let Some(w) = write {
        if w < expr_start {
            // Our write would be overwritten before we read from it.
            return None;
        }
    }
    Some(var.clone())
}

/// THE priority pass: create a temporary for a random expression, possibly
/// reusing an existing variable, with a random insertion point and a random
/// set of identical expressions replaced. Port of `perm_temp_for_expr`.
fn perm_temp_for_expr(
    unit: &mut TranslationUnit,
    fn_index: usize,
    rng: &mut Rng,
) -> Result<(), Fail> {
    let tm = typemap_for(unit, fn_index);
    let f = fdef(unit, fn_index);
    let writes = astutil::compute_write_locations(f);
    let reads = astutil::compute_read_locations(f);
    let mut control_entries = Vec::new();
    visit_stmts(&f.statement, &mut |statement| {
        if matches!(statement.node, Statement::Labeled(_)) {
            control_entries.push(statement.span.start);
        }
    });
    control_entries.sort_unstable();
    control_entries.dedup();
    let should_make_ptr = rng.chance(PROB_TEMP_PTR);

    struct Cand {
        place: Place,
        orig_id: Nid,
        orig: Node<Expression>,
        reuse: Option<String>,
    }
    let mut candidates: Vec<(Cand, f64)> = Vec::new();
    let mut einds: std::collections::BTreeMap<Nid, usize> = Default::default();

    // Step 1: assign probabilities to each place/expression.
    #[allow(clippy::too_many_arguments)]
    fn rec(
        block: &Node<Statement>,
        reuse_in: &[String],
        should_make_ptr: bool,
        tm: &TypeMap,
        control_entries: &[usize],
        writes: &std::collections::BTreeMap<String, Vec<usize>>,
        einds: &mut std::collections::BTreeMap<Nid, usize>,
        candidates: &mut Vec<(Cand, f64)>,
        rng: &mut Rng,
    ) {
        let items = astutil::block_items(block);
        let block_id = nid(block);
        let mut reuse: Vec<String> = reuse_in.to_vec();
        let mut assignment_cands: Vec<Place> = Vec::new();
        let mut past_decls = false;
        for (index, item) in items.iter().enumerate() {
            if let BlockItem::Declaration(d) = &item.node {
                for init in &d.node.declarators {
                    let (t, name) =
                        crate::asttypes::apply_declarator(CType::int(), &init.node.declarator.node);
                    if let Some(name) = name {
                        match t {
                            CType::Array(_) => {}
                            CType::Ptr(_) => reuse.push(name),
                            _ => {
                                // Make non-pointers more common.
                                reuse.push(name.clone());
                                reuse.push(name);
                            }
                        }
                    }
                }
            } else {
                past_decls = true;
            }
            if past_decls {
                assignment_cands.push(Place {
                    block: block_id,
                    index,
                    stmt_start: item.span.start,
                });
            }
            if let BlockItem::Statement(s) = &item.node {
                for nb in nested_blocks(s) {
                    rec(
                        nb,
                        &reuse,
                        should_make_ptr,
                        tm,
                        control_entries,
                        writes,
                        einds,
                        candidates,
                        rng,
                    );
                }
            }
            scan_item_exprs(item, &mut |e, is_expr| {
                if !is_expr {
                    return;
                }
                if should_make_ptr && !is_lvalue(&e.node, tm) {
                    return;
                }
                let orig_id = nid(e);
                let mut eind = if should_make_ptr {
                    0
                } else {
                    *einds.get(&orig_id).unwrap_or(&0)
                };
                let (prev_write, _) = surrounding_writes(e, e.span.start, writes);
                for place in assignment_cands.iter().rev() {
                    // A goto, case, or default entry may jump past an
                    // assignment inserted before its label. Never hoist a
                    // temporary across such a control-flow entry.
                    if crosses_control_entry(place.stmt_start, e.span.start, control_entries) {
                        break;
                    }
                    // If the expression reads something written within
                    // [place, expr), the assignment would move too high up.
                    if place.stmt_start <= prev_write {
                        break;
                    }
                    eind += 1;
                    let mut prob = rng.span_weight(e.span.start) / eind as f64;
                    if matches!(e.node, Expression::Identifier(_) | Expression::Constant(_)) {
                        prob *= if should_make_ptr { 0.15 } else { 0.5 };
                    }
                    let reuse_cand = if reuse.is_empty() {
                        None
                    } else {
                        Some(rng.choice(&reuse).clone())
                    };
                    candidates.push((
                        Cand {
                            place: place.clone(),
                            orig_id,
                            orig: e.clone(),
                            reuse: reuse_cand,
                        },
                        prob,
                    ));
                }
                if !should_make_ptr {
                    einds.insert(orig_id, eind);
                }
            });
        }
    }
    rec(
        &f.statement,
        &[],
        should_make_ptr,
        &tm,
        &control_entries,
        &writes,
        &mut einds,
        &mut candidates,
        rng,
    );

    // Step 2: decide on a place/expression.
    ensure(!candidates.is_empty())?;
    let chosen = random_weighted(rng, &candidates);
    let mut place: Option<Place> = Some(chosen.place.clone());
    let orig_id = chosen.orig_id;
    let orig = chosen.orig.clone();
    let reuse_cand = chosen.reuse.clone();
    if rng.chance(PROB_TEMP_ASSIGN_AT_FIRST_USE) {
        // Assign within the first expression it's used in instead.
        place = None;
    }

    let mut wrapped = should_make_ptr;
    let mut t: CType = if wrapped {
        CType::ptr(expr_type(&orig.node, &tm)?)
    } else {
        decayed_expr_type(&orig.node, &tm)?
    };
    // Always use pointers when replacing structs.
    if !wrapped
        && matches!(resolve_typedefs(t.clone(), &tm), CType::Struct { .. })
        && is_lvalue(&orig.node, &tm)
    {
        wrapped = true;
        t = CType::ptr(expr_type(&orig.node, &tm)?);
    }
    let assign_expr_node: Node<Expression> = if wrapped {
        Node::new(unary(UnaryOperator::Address, orig.node.clone()), orig.span)
    } else {
        orig.clone()
    };

    // Step 3: decide on a variable to hold the expression.
    let assign_before_start = place
        .as_ref()
        .map(|p| p.stmt_start)
        .unwrap_or(orig.span.start);
    let reused_var = maybe_reuse_var(
        reuse_cand.as_ref(),
        assign_before_start,
        orig.span.start,
        &t,
        &reads,
        &writes,
        &tm,
        rng,
    );
    let (reused, var) = match reused_var {
        Some(v) => (true, v),
        None => {
            let mut var = "new_var".to_string();
            let mut counter = 1;
            while writes.contains_key(&var) {
                counter += 1;
                var = format!("new_var{counter}");
            }
            (false, var)
        }
    };

    // Step 4: possibly expand the replacement to duplicate expressions.
    let (mut prev_write, next_write) =
        surrounding_writes(&assign_expr_node, orig.span.start, &writes);
    prev_write = prev_write.max(assign_before_start.saturating_sub(1));
    let mut replace_cands: Vec<Nid> = Vec::new();
    if is_effectful(&assign_expr_node.node) {
        replace_cands.push(orig_id);
    } else {
        scan_stmt_exprs(&f.statement, &mut |e, is_expr| {
            if is_expr
                && prev_write < e.span.start
                && e.span.start <= next_write
                && equal_expr(&e.node, &orig.node)
            {
                replace_cands.push(nid(e));
            }
        });
    }
    let mid = replace_cands
        .iter()
        .position(|&i| i == orig_id)
        .ok_or(Fail)?;
    let subset = pick_random_subset(rng, &replace_cands, mid);
    let replace_set: std::collections::BTreeSet<Nid> = subset.into_iter().collect();

    // Step 5: replace the chosen expressions.
    let assign_value = assign_expr_node.node.clone();
    let place_none = place.is_none();
    let var_name = var.clone();
    let body = fbody_mut(unit, fn_index);
    walk_stmt_exprs(body, &mut |e, is_expr| {
        if !is_expr {
            return false;
        }
        let id = nid(e);
        if replace_set.contains(&id) {
            let mut ret = id_expr(&var_name);
            if place_none && id == orig_id {
                ret = binop(BinaryOperator::Assign, ret, assign_value.clone());
            }
            if wrapped {
                ret = unary(UnaryOperator::Indirection, ret);
            }
            *e = n(ret);
            return true;
        }
        false
    });

    // Step 6: insert the assignment and any new variable declaration.
    if let Some(p) = &place {
        let stmt = expr_stmt(binop(
            BinaryOperator::Assign,
            id_expr(&var),
            assign_expr_node.node.clone(),
        ));
        insert_block_item(body, p.block, p.index, stmt);
    }
    if !reused {
        let final_t = if rng.chance(PROB_RANDOMIZE_TYPE) {
            randomize_type(&t, &tm, rng, false)?
        } else {
            t
        };
        insert_decl(body, &var, &final_t, rng)?;
    }
    Ok(())
}

/// Port of `insert_decl`: a new declaration at a random index within the
/// function body's leading declaration run.
fn insert_decl(
    body: &mut Node<Statement>,
    var: &str,
    t: &CType,
    rng: &mut Rng,
) -> Result<(), Fail> {
    let decl = astutil::make_decl(var, t).ok_or(Fail)?;
    if let Statement::Compound(items) = &mut body.node {
        let mut index = items.len();
        for (i, item) in items.iter().enumerate() {
            if !matches!(item.node, BlockItem::Declaration(_)) {
                index = i;
                break;
            }
        }
        let index = rng.randint(0, index);
        items.insert(index, n(BlockItem::Declaration(decl)));
        Ok(())
    } else {
        Err(Fail)
    }
}

// ---------------------------------------------------------- expand_expr

/// Port of `perm_expand_expr`: replace a random variable read by the value
/// last assigned to it.
fn perm_expand_expr(
    unit: &mut TranslationUnit,
    fn_index: usize,
    rng: &mut Rng,
) -> Result<(), Fail> {
    #[derive(Clone)]
    enum WriteKind {
        DeclInit {
            decl_id: Nid,
            declarator: usize,
            init: Option<(Node<Expression>, bool)>, // (expr, is_list=false) None when list/absent
        },
        Assign {
            id: Nid,
            op_eq: bool,
            rvalue: Node<Expression>,
            lvalue: Node<Expression>,
        },
        Other,
    }
    let f = fdef(unit, fn_index);
    // Writes with payloads, in one traversal.
    let mut write_recs: std::collections::BTreeMap<usize, (String, WriteKind)> = Default::default();
    let mut writes: std::collections::BTreeMap<String, Vec<usize>> = Default::default();
    {
        // Parameters are writes with no expandable payload.
        for der in &f.declarator.node.derived {
            if let DerivedDeclarator::Function(fd) = &der.node {
                for p in &fd.node.parameters {
                    if let Some(d) = &p.node.declarator {
                        if let (_, Some(name)) =
                            crate::asttypes::apply_declarator(CType::int(), &d.node)
                        {
                            write_recs.insert(p.span.start, (name.clone(), WriteKind::Other));
                            writes.entry(name).or_default().push(p.span.start);
                        }
                    }
                }
            }
        }
        fn scan(
            stmt: &Node<Statement>,
            recs: &mut std::collections::BTreeMap<usize, (String, WriteKind)>,
            writes: &mut std::collections::BTreeMap<String, Vec<usize>>,
        ) {
            // Declarations.
            fn handle_decl(
                d: &Node<Declaration>,
                recs: &mut std::collections::BTreeMap<usize, (String, WriteKind)>,
                writes: &mut std::collections::BTreeMap<String, Vec<usize>>,
            ) {
                for (di, init) in d.node.declarators.iter().enumerate() {
                    if let (_, Some(name)) =
                        crate::asttypes::apply_declarator(CType::int(), &init.node.declarator.node)
                    {
                        let payload = match &init.node.initializer {
                            Some(i) => match &i.node {
                                Initializer::Expression(e) => Some(((**e).clone(), false)),
                                Initializer::List(_) => None,
                            },
                            None => None,
                        };
                        recs.insert(
                            d.span.start,
                            (
                                name.clone(),
                                WriteKind::DeclInit {
                                    decl_id: nid(d),
                                    declarator: di,
                                    init: payload,
                                },
                            ),
                        );
                        writes.entry(name).or_default().push(d.span.start);
                    }
                }
            }
            fn exprs(
                e: &Node<Expression>,
                recs: &mut std::collections::BTreeMap<usize, (String, WriteKind)>,
                writes: &mut std::collections::BTreeMap<String, Vec<usize>>,
            ) {
                match &e.node {
                    Expression::UnaryOperator(u) => {
                        if matches!(
                            u.node.operator.node,
                            UnaryOperator::PostIncrement
                                | UnaryOperator::PostDecrement
                                | UnaryOperator::PreIncrement
                                | UnaryOperator::PreDecrement
                        ) {
                            if let Expression::Identifier(id) = &u.node.operand.node {
                                recs.insert(e.span.start, (id.node.name.clone(), WriteKind::Other));
                                writes
                                    .entry(id.node.name.clone())
                                    .or_default()
                                    .push(e.span.start);
                            }
                        }
                        exprs(&u.node.operand, recs, writes);
                    }
                    Expression::BinaryOperator(b) => {
                        if is_assign_op(&b.node.operator.node) {
                            if let Expression::Identifier(id) = &b.node.lhs.node {
                                recs.insert(
                                    e.span.start,
                                    (
                                        id.node.name.clone(),
                                        WriteKind::Assign {
                                            id: nid(e),
                                            op_eq: matches!(
                                                b.node.operator.node,
                                                BinaryOperator::Assign
                                            ),
                                            rvalue: (*b.node.rhs).clone(),
                                            lvalue: (*b.node.lhs).clone(),
                                        },
                                    ),
                                );
                                writes
                                    .entry(id.node.name.clone())
                                    .or_default()
                                    .push(e.span.start);
                            }
                        }
                        exprs(&b.node.lhs, recs, writes);
                        exprs(&b.node.rhs, recs, writes);
                    }
                    Expression::Member(m) => exprs(&m.node.expression, recs, writes),
                    Expression::Call(c) => {
                        exprs(&c.node.callee, recs, writes);
                        for a in &c.node.arguments {
                            exprs(a, recs, writes);
                        }
                    }
                    Expression::Cast(c) => exprs(&c.node.expression, recs, writes),
                    Expression::Conditional(c) => {
                        exprs(&c.node.condition, recs, writes);
                        exprs(&c.node.then_expression, recs, writes);
                        exprs(&c.node.else_expression, recs, writes);
                    }
                    Expression::Comma(items) => {
                        for i in items.iter() {
                            exprs(i, recs, writes);
                        }
                    }
                    _ => {}
                }
            }
            match &stmt.node {
                Statement::Labeled(l) => scan(&l.node.statement, recs, writes),
                Statement::Compound(items) => {
                    for item in items {
                        match &item.node {
                            BlockItem::Declaration(d) => {
                                handle_decl(d, recs, writes);
                                for init in &d.node.declarators {
                                    if let Some(i) = &init.node.initializer {
                                        if let Initializer::Expression(e) = &i.node {
                                            exprs(e, recs, writes);
                                        }
                                    }
                                }
                            }
                            BlockItem::Statement(s) => scan(s, recs, writes),
                            BlockItem::StaticAssert(_) => {}
                        }
                    }
                }
                Statement::Expression(Some(e)) => exprs(e, recs, writes),
                Statement::If(i) => {
                    exprs(&i.node.condition, recs, writes);
                    scan(&i.node.then_statement, recs, writes);
                    if let Some(e) = &i.node.else_statement {
                        scan(e, recs, writes);
                    }
                }
                Statement::Switch(s) => {
                    exprs(&s.node.expression, recs, writes);
                    scan(&s.node.statement, recs, writes);
                }
                Statement::While(w) => {
                    exprs(&w.node.expression, recs, writes);
                    scan(&w.node.statement, recs, writes);
                }
                Statement::DoWhile(d) => {
                    scan(&d.node.statement, recs, writes);
                    exprs(&d.node.expression, recs, writes);
                }
                Statement::For(fo) => {
                    match &fo.node.initializer.node {
                        ForInitializer::Expression(e) => exprs(e, recs, writes),
                        ForInitializer::Declaration(d) => handle_decl(d, recs, writes),
                        _ => {}
                    }
                    if let Some(c) = &fo.node.condition {
                        exprs(c, recs, writes);
                    }
                    if let Some(s) = &fo.node.step {
                        exprs(s, recs, writes);
                    }
                    scan(&fo.node.statement, recs, writes);
                }
                Statement::Return(Some(e)) => exprs(e, recs, writes),
                _ => {}
            }
        }
        scan(&f.statement, &mut write_recs, &mut writes);
        for locs in writes.values_mut() {
            locs.sort_unstable();
            locs.dedup();
        }
    }
    let all_reads = astutil::compute_read_locations(f);

    // Step 1: pick a variable read to replace.
    let mut rev: std::collections::BTreeMap<usize, String> = Default::default();
    for (var, locs) in &all_reads {
        for &loc in locs {
            rev.insert(loc, var.clone());
        }
    }
    ensure(!rev.is_empty())?;
    let keys: Vec<usize> = rev.keys().copied().collect();
    let index = *rng.choice(&keys);
    let var = rev[&index].clone();

    // Step 2: find the assignment it uses.
    let reads = all_reads.get(&var).cloned().unwrap_or_default();
    let empty = Vec::new();
    let var_writes = writes.get(&var).unwrap_or(&empty);
    let i = bisect_left(var_writes, index);
    ensure(i > 0)?;
    let before = var_writes[i - 1];
    let after = if i == var_writes.len() {
        MAX_INDEX
    } else {
        var_writes[i]
    };
    let (_, kind) = write_recs.get(&before).ok_or(Fail)?.clone();
    let (repl_expr, write_assign, write_decl) = match kind {
        WriteKind::DeclInit {
            decl_id,
            declarator,
            init: Some((e, false)),
        } => (e, None, Some((decl_id, declarator))),
        WriteKind::Assign {
            id,
            op_eq: true,
            rvalue,
            lvalue,
        } => (rvalue, Some((id, lvalue)), None),
        _ => return Err(Fail),
    };

    // Step 3: pick the range of reads to replace.
    let mut repl_cands: Vec<usize> = reads
        .iter()
        .copied()
        .filter(|&r| before < r && r < after)
        .collect();
    let myi = repl_cands.iter().position(|&r| r == index).ok_or(Fail)?;
    let keep_var;
    if !rng.chance(PROB_EXPAND_REPLACE_ALL) && repl_cands.len() > 1 {
        // Keep using the variable for a bit in the middle.
        let side = rng.index(3);
        let h = repl_cands.len();
        let mut loi = if side == 0 { 0 } else { rng.randint(0, myi) };
        let mut hii = if side == 1 {
            h
        } else {
            rng.randint(myi + 1, h)
        };
        if loi == 0 && hii == h {
            loi = myi;
            hii = myi + 1;
        }
        repl_cands.drain(loi..hii);
        keep_var = true;
    } else {
        keep_var = rng.chance(PROB_KEEP_REPLACED_VAR);
    }
    let repl_set: std::collections::BTreeSet<usize> = repl_cands.iter().copied().collect();

    // Don't duplicate effectful expressions.
    if is_effectful(&repl_expr.node) {
        ensure(repl_cands.len() == 1 && !keep_var)?;
    }

    // Removing the assignment is only sound when no later read can see the
    // stale variable. The window's upper bound (the next write) is
    // flow-insensitive -- a conditional write does not dominate the reads
    // after it -- so require that the replacement covers every read from the
    // definition to the end of the function.
    if !keep_var {
        ensure(reads.iter().all(|&r| r <= before || repl_set.contains(&r)))?;
    }

    // Step 4: do the replacement.
    let body = fbody_mut(unit, fn_index);
    let var_for_match = var.clone();
    walk_stmt_exprs(body, &mut |e, _| {
        let start = e.span.start;
        if repl_set.contains(&start) {
            if let Expression::Identifier(id) = &e.node {
                if id.node.name == var_for_match {
                    *e = n(repl_expr.node.clone());
                    return true;
                }
            }
        }
        false
    });
    if !keep_var {
        if let Some((assign_id, lvalue)) = write_assign {
            // Statement-level `x = v;` becomes `;`; an embedded assignment
            // becomes its lvalue.
            let mut removed = false;
            visit_stmts_mut(body, &mut |s| {
                if removed {
                    return;
                }
                if let Statement::Expression(Some(e)) = &s.node {
                    if nid(e) == assign_id {
                        s.node = Statement::Expression(None);
                        removed = true;
                    }
                }
            });
            if !removed {
                walk_stmt_exprs(body, &mut |e, is_expr| {
                    if is_expr && nid(e) == assign_id {
                        *e = n(lvalue.node.clone());
                        return true;
                    }
                    false
                });
            }
        } else if let Some((decl_id, declarator)) = write_decl {
            visit_decls_mut(body, &mut |d| {
                if nid(d) == decl_id {
                    if let Some(init) = d.node.declarators.get_mut(declarator) {
                        init.node.initializer = None;
                    }
                }
            });
        }
    }
    Ok(())
}

// -------------------------------------------------------- reorder passes

#[derive(Clone, Debug, Default)]
struct LocalEffects {
    reads: BTreeSet<String>,
    writes: BTreeSet<String>,
}

fn nonvolatile_local_scalar(name: &str, tm: &TypeMap) -> bool {
    if !tm.local_vars.contains(name) {
        return false;
    }
    let Some(kind) = tm.var_types.get(name) else {
        return false;
    };
    matches!(
        resolve_typedefs(kind.clone(), tm),
        CType::Basic {
            volatile: false,
            ..
        } | CType::Enum(_)
    )
}

/// Collect reads from an expression that is safe to move relative to another
/// local-scalar assignment. Calls, memory accesses, address-taking and all
/// mutation operators are deliberately rejected: without alias and effect
/// information none of them can be proven independent.
fn pure_local_reads(
    expression: &Expression,
    tm: &TypeMap,
    reads: &mut BTreeSet<String>,
) -> Result<(), Fail> {
    match expression {
        Expression::Identifier(identifier) => {
            let name = &identifier.node.name;
            ensure(nonvolatile_local_scalar(name, tm))?;
            reads.insert(name.clone());
            Ok(())
        }
        Expression::Constant(_)
        | Expression::StringLiteral(_)
        | Expression::SizeOfTy(_)
        | Expression::SizeOfVal(_)
        | Expression::AlignOf(_)
        | Expression::OffsetOf(_) => Ok(()),
        Expression::UnaryOperator(unary) => {
            ensure(matches!(
                unary.node.operator.node,
                UnaryOperator::Plus
                    | UnaryOperator::Minus
                    | UnaryOperator::Complement
                    | UnaryOperator::Negate
            ))?;
            pure_local_reads(&unary.node.operand.node, tm, reads)
        }
        Expression::Cast(cast) => pure_local_reads(&cast.node.expression.node, tm, reads),
        Expression::BinaryOperator(binary) => {
            ensure(
                !is_assign_op(&binary.node.operator.node)
                    && binary.node.operator.node != BinaryOperator::Index,
            )?;
            pure_local_reads(&binary.node.lhs.node, tm, reads)?;
            pure_local_reads(&binary.node.rhs.node, tm, reads)
        }
        Expression::Conditional(conditional) => {
            pure_local_reads(&conditional.node.condition.node, tm, reads)?;
            pure_local_reads(&conditional.node.then_expression.node, tm, reads)?;
            pure_local_reads(&conditional.node.else_expression.node, tm, reads)
        }
        Expression::Comma(items) => {
            for item in items.iter() {
                pure_local_reads(&item.node, tm, reads)?;
            }
            Ok(())
        }
        Expression::Member(_)
        | Expression::Call(_)
        | Expression::CompoundLiteral(_)
        | Expression::GenericSelection(_)
        | Expression::VaArg(_)
        | Expression::Statement(_) => Err(Fail),
    }
}

fn local_effects(item: &Node<BlockItem>, tm: &TypeMap) -> Result<LocalEffects, Fail> {
    let mut effects = LocalEffects::default();
    match &item.node {
        BlockItem::Declaration(declaration) => {
            ensure(declaration.node.declarators.len() == 1)?;
            let declarator = &declaration.node.declarators[0].node;
            let (_, name) =
                crate::asttypes::apply_declarator(CType::int(), &declarator.declarator.node);
            let name = name.ok_or(Fail)?;
            ensure(nonvolatile_local_scalar(&name, tm))?;
            let initializer = declarator.initializer.as_ref().ok_or(Fail)?;
            let Initializer::Expression(expression) = &initializer.node else {
                return Err(Fail);
            };
            pure_local_reads(&expression.node, tm, &mut effects.reads)?;
            effects.writes.insert(name);
        }
        BlockItem::Statement(statement) => {
            let Statement::Expression(Some(expression)) = &statement.node else {
                return Err(Fail);
            };
            let Expression::BinaryOperator(binary) = &expression.node else {
                return Err(Fail);
            };
            ensure(is_assign_op(&binary.node.operator.node))?;
            let Expression::Identifier(identifier) = &binary.node.lhs.node else {
                return Err(Fail);
            };
            let name = &identifier.node.name;
            ensure(nonvolatile_local_scalar(name, tm))?;
            pure_local_reads(&binary.node.rhs.node, tm, &mut effects.reads)?;
            if binary.node.operator.node != BinaryOperator::Assign {
                effects.reads.insert(name.clone());
            }
            effects.writes.insert(name.clone());
        }
        BlockItem::StaticAssert(_) => return Err(Fail),
    }
    Ok(effects)
}

fn effects_commute(left: &LocalEffects, right: &LocalEffects) -> bool {
    left.writes.is_disjoint(&right.writes)
        && left.writes.is_disjoint(&right.reads)
        && left.reads.is_disjoint(&right.writes)
}

fn collect_reorder_facts(
    statement: &Node<Statement>,
    tm: &TypeMap,
    blocks: &mut BTreeMap<Nid, Vec<Nid>>,
    effects: &mut BTreeMap<Nid, LocalEffects>,
) {
    match &statement.node {
        Statement::Labeled(labeled) => {
            collect_reorder_facts(&labeled.node.statement, tm, blocks, effects)
        }
        Statement::Compound(items) => {
            blocks.insert(
                nid(statement),
                items.iter().map(astutil::item_nid).collect(),
            );
            for item in items {
                if let Ok(found) = local_effects(item, tm) {
                    effects.insert(astutil::item_nid(item), found);
                }
                if let BlockItem::Statement(child) = &item.node {
                    collect_reorder_facts(child, tm, blocks, effects);
                }
            }
        }
        Statement::If(branch) => {
            collect_reorder_facts(&branch.node.then_statement, tm, blocks, effects);
            if let Some(other) = &branch.node.else_statement {
                collect_reorder_facts(other, tm, blocks, effects);
            }
        }
        Statement::Switch(switch) => {
            collect_reorder_facts(&switch.node.statement, tm, blocks, effects)
        }
        Statement::While(loop_) => {
            collect_reorder_facts(&loop_.node.statement, tm, blocks, effects)
        }
        Statement::DoWhile(loop_) => {
            collect_reorder_facts(&loop_.node.statement, tm, blocks, effects)
        }
        Statement::For(loop_) => collect_reorder_facts(&loop_.node.statement, tm, blocks, effects),
        _ => {}
    }
}

/// Move a local-scalar assignment across statements proven independent in
/// the same compound block. Pret's unrestricted pass is useful as an idea
/// generator, but in a cumulative walk it also moved calls between switch
/// arms and transferred arithmetic from one effect handler to another. Those
/// candidates score well by phase and poison every later mutation. Keeping
/// the move within one block and checking every crossed statement preserves
/// the productive declaration/assignment ordering search without that class
/// of semantic lie.
fn perm_reorder_stmts(
    unit: &mut TranslationUnit,
    fn_index: usize,
    rng: &mut Rng,
) -> Result<(), Fail> {
    let f = fdef(unit, fn_index);
    let tm = typemap_for(unit, fn_index);
    let mut blocks = BTreeMap::new();
    let mut effects = BTreeMap::new();
    collect_reorder_facts(&f.statement, &tm, &mut blocks, &mut effects);
    let mut cands: Vec<InsPoint> = insertion_points(&f.statement, true);
    cands.retain(|c| match &c.after {
        Some(info) if info.is_decl => info
            .decl
            .as_ref()
            .and_then(|d| d.single.as_ref())
            .map(|(_, has_init)| *has_init)
            .unwrap_or(false),
        _ => true,
    });

    let source_inds: Vec<usize> = cands
        .iter()
        .enumerate()
        .filter_map(|(index, point)| {
            point.after.as_ref().and_then(|info| {
                (!info.has_nested_block && !info.is_jump && effects.contains_key(&info.id))
                    .then_some(index)
            })
        })
        .collect();
    ensure(!source_inds.is_empty())?;
    let spans: Vec<usize> = source_inds
        .iter()
        .map(|index| cands[*index].after.as_ref().map_or(0, |info| info.start))
        .collect();
    let sourcei = source_inds[rng.weighted_index_by_span(&spans)];
    let source = &cands[sourcei];
    let from_info = source.after.as_ref().ok_or(Fail)?;
    let from_effects = effects.get(&from_info.id).ok_or(Fail)?;
    let items = blocks.get(&source.block).ok_or(Fail)?;

    let mut weighted: Vec<(usize, f64)> = Vec::new();
    for (toi_index, to) in cands.iter().enumerate() {
        if to.block != source.block
            || to.index == source.index
            || to.index == source.index + 1
            || to.after.as_ref().map(|a| a.is_decl).unwrap_or(false)
        {
            continue;
        }
        // A declaration stays in place while its initializer becomes an
        // assignment, so that assignment may only move later.
        if from_info.is_decl && to.index <= source.index + 1 {
            continue;
        }
        let crossed = if to.index < source.index {
            &items[to.index..source.index]
        } else {
            &items[source.index + 1..to.index]
        };
        if !crossed.iter().all(|item| {
            effects
                .get(item)
                .map(|other| effects_commute(from_effects, other))
                .unwrap_or(false)
        }) {
            continue;
        }
        let distance = source.index.abs_diff(to.index);
        let probability = ((distance + 1) as f64).powf(-1.5) + 0.5 / cands.len() as f64;
        weighted.push((toi_index, probability));
    }
    ensure(!weighted.is_empty())?;
    let toi_index = *random_weighted(rng, &weighted);
    let from = cands[sourcei].clone();
    let from_info = from.after.clone().ok_or(Fail)?;
    let to = cands[toi_index].clone();
    let (fromb, fromi) = (from.block, from.index);
    let (tob, mut toi) = (to.block, to.index);
    if fromb == tob {
        ensure(toi != fromi && toi != fromi + 1)?;
    }

    let body = fbody_mut(unit, fn_index);
    if from_info.is_decl {
        // Extract the initializer as a moved assignment.
        let (name, _) = from_info
            .decl
            .as_ref()
            .and_then(|d| d.single.clone())
            .ok_or(Fail)?;
        let mut init_expr: Option<Expression> = None;
        let decl_id = from_info.id;
        visit_decls_mut(body, &mut |d| {
            if nid(d) == decl_id {
                if let Some(init) = d.node.declarators.get_mut(0) {
                    if let Some(node) = init.node.initializer.take() {
                        if let Initializer::Expression(e) = node.node {
                            init_expr = Some(e.node);
                        }
                    }
                }
            }
        });
        let init_expr = init_expr.ok_or(Fail)?;
        let stmt = expr_stmt(binop(BinaryOperator::Assign, id_expr(&name), init_expr));
        insert_block_item(body, tob, toi, stmt);
    } else {
        if fromb == tob && fromi < toi {
            toi -= 1;
        }
        let moved = with_block(body, fromb, &mut |items| {
            if fromi < items.len() {
                Some(items.remove(fromi))
            } else {
                None
            }
        })
        .flatten()
        .ok_or(Fail)?;
        ensure(insert_block_item(body, tob, toi, moved))?;
    }
    Ok(())
}

/// Move one uninitialized declaration within its own compound block and
/// never past a use. Cross-block movement made outer locals disappear into
/// a nested scope; moving an initializer changes when its expression runs.
fn perm_reorder_decls(
    unit: &mut TranslationUnit,
    fn_index: usize,
    rng: &mut Rng,
) -> Result<(), Fail> {
    let f = fdef(unit, fn_index);
    let all = insertion_points(&f.statement, true);
    let cands: Vec<InsPoint> = all
        .into_iter()
        .filter(|c| {
            c.index == 0
                || c.before.as_ref().map(|b| b.is_decl).unwrap_or(false)
                || c.after.as_ref().map(|a| a.is_decl).unwrap_or(false)
        })
        .collect();
    let source_inds: Vec<usize> = cands
        .iter()
        .enumerate()
        .filter(|(_, c)| {
            c.after
                .as_ref()
                .and_then(|after| after.decl.as_ref())
                .and_then(|decl| decl.single.as_ref())
                .is_some_and(|(_, has_initializer)| !has_initializer)
        })
        .map(|(i, _)| i)
        .collect();
    ensure(!source_inds.is_empty())?;
    let sourcei = *rng.choice(&source_inds);
    let from = cands[sourcei].clone();

    let mut weighted: Vec<(usize, f64)> = Vec::new();
    for i in 0..cands.len() {
        if cands[i].block != from.block {
            continue;
        }
        let dist = (sourcei as isize - i as isize).max(i as isize - (sourcei as isize + 1));
        if dist <= 0 {
            continue;
        }
        let prob = ((dist + 1) as f64).powf(-1.5) + 1.0 / cands.len() as f64;
        weighted.push((i, prob));
    }
    ensure(!weighted.is_empty())?;
    let desti = *random_weighted(rng, &weighted);

    let to = cands[desti].clone();
    let from_info = from.after.clone().ok_or(Fail)?;
    let decl_info = from_info.decl.clone().ok_or(Fail)?;
    let (fromb, fromi) = (from.block, from.index);
    let (tob, mut toi) = (to.block, to.index);
    if fromb == tob {
        ensure(toi != fromi && toi != fromi + 1)?;
    }

    // Don't move a declaration past the next occurrence of its variables.
    ensure(!decl_info.names.is_empty())?;
    let to_index = to
        .after
        .as_ref()
        .map(|a| a.start)
        .unwrap_or_else(|| fromb.1); // block end span
    for name in &decl_info.names {
        let mut uses = 0usize;
        // Identifier mentions.
        scan_stmt_exprs(&f.statement, &mut |e, _| {
            if let Expression::Identifier(id) = &e.node {
                if id.node.name == *name && e.span.start < to_index {
                    uses += 1;
                }
            }
        });
        // Declarator mentions.
        let mut count_decl = |d: &Node<Declaration>| {
            for init in &d.node.declarators {
                if let (_, Some(n2)) =
                    crate::asttypes::apply_declarator(CType::int(), &init.node.declarator.node)
                {
                    if n2 == *name && d.span.start < to_index {
                        uses += 1;
                    }
                }
            }
        };
        visit_stmts(&f.statement, &mut |s| {
            if let Statement::Compound(items) = &s.node {
                for item in items {
                    if let BlockItem::Declaration(d) = &item.node {
                        count_decl(d);
                    }
                }
            }
        });
        ensure(uses <= 1)?;
    }

    if fromb == tob && fromi < toi {
        toi -= 1;
    }
    let body = fbody_mut(unit, fn_index);
    let moved = with_block(body, fromb, &mut |items| {
        if fromi < items.len() {
            Some(items.remove(fromi))
        } else {
            None
        }
    })
    .flatten()
    .ok_or(Fail)?;
    ensure(insert_block_item(body, tob, toi, moved))?;
    Ok(())
}

// -------------------------------------------------- expression tweaks

/// Port of `perm_add_mask`: wrap an integer expression in one or more
/// `& 0xFF...` masks.
fn perm_add_mask(unit: &mut TranslationUnit, fn_index: usize, rng: &mut Rng) -> Result<(), Fail> {
    let tm = typemap_for(unit, fn_index);
    let f = fdef(unit, fn_index);
    let cands = block_expressions(&f.statement);
    ensure(!cands.is_empty())?;
    let (id, expr) = rng.choice_by_span(&cands, |c| c.0 .0).clone();
    let t = decayed_expr_type(&expr.node, &tm)?;
    ensure(allowed_basic_type(&t, &tm, &INT_TYPES))?;
    let masks = ["FF", "FFFF", "FFFFFFFF", "FFFFFFFFFFFFFFFF"];
    let mask = (*rng.choice(&masks)).to_string();
    let unsigned = *rng.choice(&[false, true]);
    let mut extra = 0usize;
    if rng.chance(0.3) {
        extra = rng.index(12);
    }
    let body = fbody_mut(unit, fn_index);
    ensure(replace_expr_by_id(body, id, &mut |old| {
        let mut e = binop(
            BinaryOperator::BitwiseAnd,
            old.node,
            int_expr_full(&mask, IntegerBase::Hexadecimal, unsigned),
        );
        for _ in 0..extra {
            e = binop(
                BinaryOperator::BitwiseAnd,
                e,
                int_expr_full(&mask, IntegerBase::Hexadecimal, unsigned),
            );
        }
        e
    }))
}

/// Port of `perm_xor_zero`: `^0` on integers, `*1.0f`/`*1.0` on floats.
fn perm_xor_zero(unit: &mut TranslationUnit, fn_index: usize, rng: &mut Rng) -> Result<(), Fail> {
    let tm = typemap_for(unit, fn_index);
    let f = fdef(unit, fn_index);
    let cands = block_expressions(&f.statement);
    ensure(!cands.is_empty())?;
    let (id, expr) = rng.choice_by_span(&cands, |c| c.0 .0).clone();
    let t = decayed_expr_type(&expr.node, &tm)?;
    enum Plan {
        Xor,
        MulF,
        MulD,
    }
    let plan = if allowed_basic_type(&t, &tm, &INT_TYPES) {
        Plan::Xor
    } else if allowed_basic_type(&t, &tm, &["float"]) {
        Plan::MulF
    } else if allowed_basic_type(&t, &tm, &["double"]) {
        Plan::MulD
    } else {
        return Err(Fail);
    };
    let body = fbody_mut(unit, fn_index);
    ensure(replace_expr_by_id(body, id, &mut |old| match plan {
        Plan::Xor => binop(BinaryOperator::BitwiseXor, old.node, int_expr("0")),
        Plan::MulF => binop(
            BinaryOperator::Multiply,
            old.node,
            astutil::float_expr("1.0", true),
        ),
        Plan::MulD => binop(
            BinaryOperator::Multiply,
            old.node,
            astutil::float_expr("1.0", false),
        ),
    }))
}

/// Port of `perm_mult_zero`: turn a literal 0 into `x * 0`.
fn perm_mult_zero(unit: &mut TranslationUnit, fn_index: usize, rng: &mut Rng) -> Result<(), Fail> {
    let tm = typemap_for(unit, fn_index);
    let f = fdef(unit, fn_index);
    let cands = block_expressions(&f.statement);
    ensure(!cands.is_empty())?;
    let (_, expr) = rng.choice_by_span(&cands, |c| c.0 .0).clone();
    ensure_arithmetic_type(&expr.node, &tm)?;
    ensure(!is_effectful(&expr.node))?;
    let zeroes: Vec<(Nid, Node<Expression>)> = cands
        .iter()
        .filter(|(_, e)| match &e.node {
            Expression::Constant(c) => match &c.node {
                Constant::Integer(i) => {
                    &*i.number == "0"
                        && !i.suffix.unsigned
                        && matches!(i.base, IntegerBase::Decimal)
                }
                Constant::Float(fl) => &*fl.number == "0.0",
                _ => false,
            },
            _ => false,
        })
        .cloned()
        .collect();
    ensure(!zeroes.is_empty())?;
    let (zid, _) = rng.choice_by_span(&zeroes, |c| c.0 .0).clone();
    let value = expr.node.clone();
    let body = fbody_mut(unit, fn_index);
    ensure(replace_expr_by_id(body, zid, &mut |old| {
        binop(BinaryOperator::Multiply, value.clone(), old.node)
    }))
}

/// Port of `perm_cast_simple`: cast a random arithmetic expression to a
/// random integral or floating type.
fn perm_cast_simple(
    unit: &mut TranslationUnit,
    fn_index: usize,
    rng: &mut Rng,
) -> Result<(), Fail> {
    let tm = typemap_for(unit, fn_index);
    let f = fdef(unit, fn_index);
    let cands = block_expressions(&f.statement);
    ensure(!cands.is_empty())?;
    let (id, expr) = rng.choice_by_span(&cands, |c| c.0 .0).clone();
    ensure_arithmetic_type(&expr.node, &tm)?;
    let integral: [&[&str]; 5] = [
        &["int"],
        &["char"],
        &["long"],
        &["short"],
        &["long", "long"],
    ];
    let floating: [&[&str]; 2] = [&["float"], &["double"]];
    let mut names: Vec<&str> = Vec::new();
    if rng.chance(0.5) {
        if *rng.choice(&[false, true]) {
            names.push("unsigned");
        }
        names.extend(rng.choice(&integral).iter());
    } else {
        names.extend(rng.choice(&floating).iter());
    }
    let mut specifiers: Vec<Node<SpecifierQualifier>> = Vec::new();
    for name in names {
        let ts = match name {
            "unsigned" => TypeSpecifier::Unsigned,
            "int" => TypeSpecifier::Int,
            "char" => TypeSpecifier::Char,
            "long" => TypeSpecifier::Long,
            "short" => TypeSpecifier::Short,
            "float" => TypeSpecifier::Float,
            "double" => TypeSpecifier::Double,
            _ => return Err(Fail),
        };
        specifiers.push(n(SpecifierQualifier::TypeSpecifier(n(ts))));
    }
    let body = fbody_mut(unit, fn_index);
    ensure(replace_expr_by_id(body, id, &mut |old| {
        Expression::Cast(Box::new(n(CastExpression {
            type_name: n(TypeName {
                specifiers: specifiers.clone(),
                declarator: None,
            }),
            expression: Box::new(old),
        })))
    }))
}

/// Port of `perm_commutative`: swap the operands of a commutative binary
/// operation (flipping the direction of comparisons).
fn perm_commutative(
    unit: &mut TranslationUnit,
    fn_index: usize,
    rng: &mut Rng,
) -> Result<(), Fail> {
    let f = fdef(unit, fn_index);
    let mut cands: Vec<Nid> = Vec::new();
    let is_comm = |op: &BinaryOperator| {
        matches!(
            op,
            BinaryOperator::Plus
                | BinaryOperator::Multiply
                | BinaryOperator::BitwiseOr
                | BinaryOperator::BitwiseAnd
                | BinaryOperator::BitwiseXor
                | BinaryOperator::Less
                | BinaryOperator::Greater
                | BinaryOperator::LessOrEqual
                | BinaryOperator::GreaterOrEqual
                | BinaryOperator::Equals
                | BinaryOperator::NotEquals
        )
    };
    scan_stmt_exprs(&f.statement, &mut |e, _| {
        if let Expression::BinaryOperator(b) = &e.node {
            if is_comm(&b.node.operator.node) {
                cands.push(nid(e));
            }
        }
    });
    ensure(!cands.is_empty())?;
    let target = *rng.choice_by_span(&cands, |c| c.0);
    let mut done = false;
    let body = fbody_mut(unit, fn_index);
    walk_stmt_exprs(body, &mut |e, _| {
        if done || nid(e) != target {
            return false;
        }
        if let Expression::BinaryOperator(b) = &mut e.node {
            std::mem::swap(&mut b.node.lhs, &mut b.node.rhs);
            b.node.operator.node = match b.node.operator.node.clone() {
                BinaryOperator::Less => BinaryOperator::Greater,
                BinaryOperator::LessOrEqual => BinaryOperator::GreaterOrEqual,
                BinaryOperator::Greater => BinaryOperator::Less,
                BinaryOperator::GreaterOrEqual => BinaryOperator::LessOrEqual,
                other => other,
            };
            done = true;
        }
        false
    });
    ensure(done)
}

/// Port of `perm_add_sub`: `a - b` <-> `a + (-b)`.
fn perm_add_sub(unit: &mut TranslationUnit, fn_index: usize, rng: &mut Rng) -> Result<(), Fail> {
    let f = fdef(unit, fn_index);
    let mut cands: Vec<(Nid, Node<Expression>)> = Vec::new();
    scan_stmt_exprs(&f.statement, &mut |e, _| {
        if let Expression::BinaryOperator(b) = &e.node {
            if matches!(
                b.node.operator.node,
                BinaryOperator::Plus | BinaryOperator::Minus
            ) {
                cands.push((nid(e), e.clone()));
            }
        }
    });
    ensure(!cands.is_empty())?;
    let (target, expr) = rng.choice_by_span(&cands, |c| c.0 .0).clone();
    enum Plan {
        NegateConst,
        UnwrapMinus,
        WrapMinus,
    }
    let plan = if let Expression::BinaryOperator(b) = &expr.node {
        match &b.node.rhs.node {
            Expression::Constant(_) => Plan::NegateConst,
            Expression::UnaryOperator(u)
                if matches!(u.node.operator.node, UnaryOperator::Minus) =>
            {
                Plan::UnwrapMinus
            }
            _ => {
                let tm = typemap_for(unit, fn_index);
                let t = resolve_typedefs(decayed_expr_type(&expr.node, &tm)?, &tm);
                // pret only allows this on pointer-typed sums.
                ensure(matches!(t, CType::Ptr(_)))?;
                Plan::WrapMinus
            }
        }
    } else {
        return Err(Fail);
    };
    let mut done = false;
    let body = fbody_mut(unit, fn_index);
    walk_stmt_exprs(body, &mut |e, _| {
        if done || nid(e) != target {
            return false;
        }
        if let Expression::BinaryOperator(b) = &mut e.node {
            b.node.operator.node = match b.node.operator.node {
                BinaryOperator::Minus => BinaryOperator::Plus,
                _ => BinaryOperator::Minus,
            };
            match plan {
                Plan::NegateConst | Plan::WrapMinus => {
                    let old = std::mem::replace(&mut *b.node.rhs, n(id_expr("")));
                    *b.node.rhs = n(unary(UnaryOperator::Minus, old.node));
                }
                Plan::UnwrapMinus => {
                    let old = std::mem::replace(&mut *b.node.rhs, n(id_expr("")));
                    if let Expression::UnaryOperator(u) = old.node {
                        *b.node.rhs = *u.node.operand;
                    }
                }
            }
            done = true;
        }
        false
    });
    ensure(done)
}

/// Port of `perm_inequalities`: `a < b` <-> `a <= b - 1` and friends.
fn perm_inequalities(
    unit: &mut TranslationUnit,
    fn_index: usize,
    rng: &mut Rng,
) -> Result<(), Fail> {
    let f = fdef(unit, fn_index);
    let mut cands: Vec<Nid> = Vec::new();
    scan_stmt_exprs(&f.statement, &mut |e, _| {
        if let Expression::BinaryOperator(b) = &e.node {
            if matches!(
                b.node.operator.node,
                BinaryOperator::Less
                    | BinaryOperator::Greater
                    | BinaryOperator::LessOrEqual
                    | BinaryOperator::GreaterOrEqual
            ) {
                cands.push(nid(e));
            }
        }
    });
    ensure(!cands.is_empty())?;
    let target = *rng.choice_by_span(&cands, |c| c.0);
    let plus1 = |e: Expression| binop(BinaryOperator::Plus, e, int_expr("1"));
    let minus1 = |e: Expression| binop(BinaryOperator::Minus, e, int_expr("1"));
    enum Plan {
        BothSides(bool), // true = plus1
        ShiftOp(bool),   // true = change left side
    }
    let plan = if rng.f() < 0.25 {
        Plan::BothSides(*rng.choice(&[true, false]))
    } else {
        Plan::ShiftOp(rng.chance(0.5))
    };
    let mut done = false;
    let body = fbody_mut(unit, fn_index);
    walk_stmt_exprs(body, &mut |e, _| {
        if done || nid(e) != target {
            return false;
        }
        if let Expression::BinaryOperator(b) = &mut e.node {
            match &plan {
                Plan::BothSides(plus) => {
                    let l = std::mem::replace(&mut *b.node.lhs, n(id_expr("")));
                    let r = std::mem::replace(&mut *b.node.rhs, n(id_expr("")));
                    if *plus {
                        *b.node.lhs = n(plus1(l.node));
                        *b.node.rhs = n(plus1(r.node));
                    } else {
                        *b.node.lhs = n(minus1(l.node));
                        *b.node.rhs = n(minus1(r.node));
                    }
                }
                Plan::ShiftOp(left) => {
                    let op = b.node.operator.node.clone();
                    let toward_le =
                        matches!(op, BinaryOperator::Less | BinaryOperator::GreaterOrEqual);
                    b.node.operator.node = match op {
                        BinaryOperator::Less => BinaryOperator::LessOrEqual,
                        BinaryOperator::GreaterOrEqual => BinaryOperator::Greater,
                        BinaryOperator::Greater => BinaryOperator::GreaterOrEqual,
                        BinaryOperator::LessOrEqual => BinaryOperator::Less,
                        other => other,
                    };
                    if toward_le {
                        if *left {
                            let l = std::mem::replace(&mut *b.node.lhs, n(id_expr("")));
                            *b.node.lhs = n(plus1(l.node));
                        } else {
                            let r = std::mem::replace(&mut *b.node.rhs, n(id_expr("")));
                            *b.node.rhs = n(minus1(r.node));
                        }
                    } else if *left {
                        let l = std::mem::replace(&mut *b.node.lhs, n(id_expr("")));
                        *b.node.lhs = n(minus1(l.node));
                    } else {
                        let r = std::mem::replace(&mut *b.node.rhs, n(id_expr("")));
                        *b.node.rhs = n(plus1(r.node));
                    }
                }
            }
            done = true;
        }
        false
    });
    ensure(done)
}

/// Port of `perm_compound_assignment`: `x = x op y` <-> `x op= y`.
fn perm_compound_assignment(
    unit: &mut TranslationUnit,
    fn_index: usize,
    rng: &mut Rng,
) -> Result<(), Fail> {
    let compound_of = |op: &BinaryOperator| -> Option<BinaryOperator> {
        Some(match op {
            BinaryOperator::Plus => BinaryOperator::AssignPlus,
            BinaryOperator::Minus => BinaryOperator::AssignMinus,
            BinaryOperator::Multiply => BinaryOperator::AssignMultiply,
            BinaryOperator::Divide => BinaryOperator::AssignDivide,
            BinaryOperator::ShiftLeft => BinaryOperator::AssignShiftLeft,
            BinaryOperator::ShiftRight => BinaryOperator::AssignShiftRight,
            BinaryOperator::BitwiseXor => BinaryOperator::AssignBitwiseXor,
            BinaryOperator::BitwiseOr => BinaryOperator::AssignBitwiseOr,
            BinaryOperator::BitwiseAnd => BinaryOperator::AssignBitwiseAnd,
            _ => return None,
        })
    };
    let base_of = |op: &BinaryOperator| -> Option<BinaryOperator> {
        Some(match op {
            BinaryOperator::AssignPlus => BinaryOperator::Plus,
            BinaryOperator::AssignMinus => BinaryOperator::Minus,
            BinaryOperator::AssignMultiply => BinaryOperator::Multiply,
            BinaryOperator::AssignDivide => BinaryOperator::Divide,
            BinaryOperator::AssignShiftLeft => BinaryOperator::ShiftLeft,
            BinaryOperator::AssignShiftRight => BinaryOperator::ShiftRight,
            BinaryOperator::AssignBitwiseXor => BinaryOperator::BitwiseXor,
            BinaryOperator::AssignBitwiseOr => BinaryOperator::BitwiseOr,
            BinaryOperator::AssignBitwiseAnd => BinaryOperator::BitwiseAnd,
            _ => return None,
        })
    };
    let f = fdef(unit, fn_index);
    let mut cands: Vec<Nid> = Vec::new();
    scan_stmt_exprs(&f.statement, &mut |e, _| {
        if let Expression::BinaryOperator(b) = &e.node {
            let op = &b.node.operator.node;
            if base_of(op).is_some() {
                cands.push(nid(e));
            } else if matches!(op, BinaryOperator::Assign) {
                if let Expression::BinaryOperator(rb) = &b.node.rhs.node {
                    if compound_of(&rb.node.operator.node).is_some()
                        && equal_expr(&b.node.lhs.node, &rb.node.lhs.node)
                    {
                        cands.push(nid(e));
                    }
                }
            }
        }
    });
    ensure(!cands.is_empty())?;
    let target = *rng.choice_by_span(&cands, |c| c.0);
    let mut done = false;
    let body = fbody_mut(unit, fn_index);
    walk_stmt_exprs(body, &mut |e, _| {
        if done || nid(e) != target {
            return false;
        }
        if let Expression::BinaryOperator(b) = &mut e.node {
            if matches!(b.node.operator.node, BinaryOperator::Assign) {
                // x = x op y  ->  x op= y
                let rhs = std::mem::replace(&mut *b.node.rhs, n(id_expr("")));
                if let Expression::BinaryOperator(rb) = rhs.node {
                    if let Some(comp) = compound_of(&rb.node.operator.node) {
                        b.node.operator.node = comp;
                        *b.node.rhs = *rb.node.rhs;
                        done = true;
                    }
                }
            } else if let Some(base) = base_of(&b.node.operator.node) {
                // x op= y  ->  x = x op y
                let rhs = std::mem::replace(&mut *b.node.rhs, n(id_expr("")));
                let lhs_copy = b.node.lhs.node.clone();
                b.node.operator.node = BinaryOperator::Assign;
                *b.node.rhs = n(binop(base, lhs_copy, rhs.node));
                done = true;
            }
        }
        false
    });
    ensure(done)
}

/// Port of `perm_condition`: `if (x)` <-> `if (x != 0)` and friends, over
/// if/while/do-while/for conditions.
fn perm_condition(unit: &mut TranslationUnit, fn_index: usize, rng: &mut Rng) -> Result<(), Fail> {
    let f = fdef(unit, fn_index);
    let mut cands: Vec<(Nid, Option<Node<Expression>>)> = Vec::new();
    visit_stmts(&f.statement, &mut |s| match &s.node {
        Statement::If(i) => cands.push((nid(s), Some((*i.node.condition).clone()))),
        Statement::While(w) => cands.push((nid(s), Some((*w.node.expression).clone()))),
        Statement::DoWhile(d) => cands.push((nid(s), Some((*d.node.expression).clone()))),
        Statement::For(fo) => {
            cands.push((nid(s), fo.node.condition.as_ref().map(|c| (**c).clone())))
        }
        _ => {}
    });
    ensure(!cands.is_empty())?;
    let (target, cond) = rng.choice(&cands).clone();
    let cond = cond.ok_or(Fail)?;

    enum Plan {
        NotLeft,
        Left,
        Wrap {
            op_eq: bool,
            unwrap_negate: bool,
            zero: Expression,
        },
    }
    let is_cmp = |op: &BinaryOperator| {
        matches!(
            op,
            BinaryOperator::Equals
                | BinaryOperator::NotEquals
                | BinaryOperator::Less
                | BinaryOperator::Greater
                | BinaryOperator::LessOrEqual
                | BinaryOperator::GreaterOrEqual
        )
    };
    let cmp_cond =
        matches!(&cond.node, Expression::BinaryOperator(b) if is_cmp(&b.node.operator.node));
    let plan = if cmp_cond && rng.chance(0.9) {
        if let Expression::BinaryOperator(b) = &cond.node {
            let op = &b.node.operator.node;
            ensure(matches!(
                op,
                BinaryOperator::Equals | BinaryOperator::NotEquals
            ))?;
            let zeroish = match &b.node.rhs.node {
                Expression::Constant(c) => match &c.node {
                    Constant::Integer(i) => &*i.number == "0",
                    Constant::Float(fl) => &*fl.number == "0.0",
                    _ => false,
                },
                _ => false,
            };
            ensure(zeroish)?;
            if matches!(op, BinaryOperator::Equals) {
                Plan::NotLeft
            } else {
                Plan::Left
            }
        } else {
            return Err(Fail);
        }
    } else {
        let mut unwrap_negate = false;
        let mut op_eq = false;
        if let Expression::UnaryOperator(u) = &cond.node {
            if matches!(u.node.operator.node, UnaryOperator::Negate) && rng.chance(0.9) {
                unwrap_negate = true;
                op_eq = true;
            }
        }
        let zeros: [(u8, f64); 3] = [(0, 0.8), (1, 0.2), (2, 0.05)];
        let zero = match *random_weighted(rng, &zeros) {
            0 => int_expr("0"),
            1 => int_expr_full("0", IntegerBase::Decimal, true),
            _ => astutil::float_expr("0.0", true),
        };
        Plan::Wrap {
            op_eq,
            unwrap_negate,
            zero,
        }
    };

    let mut done = false;
    let body = fbody_mut(unit, fn_index);
    visit_stmts_mut(body, &mut |s| {
        if done || nid(s) != target {
            return;
        }
        let cond_slot: Option<&mut Box<Node<Expression>>> = match &mut s.node {
            Statement::If(i) => Some(&mut i.node.condition),
            Statement::While(w) => Some(&mut w.node.expression),
            Statement::DoWhile(d) => Some(&mut d.node.expression),
            Statement::For(fo) => fo.node.condition.as_mut(),
            _ => None,
        };
        let slot = match cond_slot {
            Some(s) => s,
            None => return,
        };
        let old = std::mem::replace(&mut **slot, n(id_expr("")));
        let new_expr = match &plan {
            Plan::NotLeft | Plan::Left => {
                if let Expression::BinaryOperator(b) = old.node {
                    let left = b.node.lhs.node;
                    if matches!(plan, Plan::NotLeft) {
                        unary(UnaryOperator::Negate, left)
                    } else {
                        left
                    }
                } else {
                    old.node
                }
            }
            Plan::Wrap {
                op_eq,
                unwrap_negate,
                zero,
            } => {
                let mut inner = old.node;
                if *unwrap_negate {
                    if let Expression::UnaryOperator(u) = inner {
                        inner = u.node.operand.node;
                    } else {
                        unreachable!()
                    }
                }
                let op = if *op_eq {
                    BinaryOperator::Equals
                } else {
                    BinaryOperator::NotEquals
                };
                binop(op, inner, zero.clone())
            }
        };
        **slot = n(new_expr);
        done = true;
    });
    ensure(done)
}

/// Port of `perm_split_assignment`: `a = b . c . d;` becomes two chained
/// assignments through `a`.
fn perm_split_assignment(
    unit: &mut TranslationUnit,
    fn_index: usize,
    rng: &mut Rng,
) -> Result<(), Fail> {
    let tm = typemap_for(unit, fn_index);
    let f = fdef(unit, fn_index);
    // Statement-level assignments `var = binop`.
    let mut cands: Vec<(Nid, Node<Expression>)> = Vec::new();
    scan_stmt_exprs(&f.statement, &mut |e, _| {
        if let Expression::BinaryOperator(b) = &e.node {
            if matches!(b.node.operator.node, BinaryOperator::Assign)
                && matches!(b.node.rhs.node, Expression::BinaryOperator(_))
            {
                cands.push((nid(e), e.clone()));
            }
        }
    });
    ensure(!cands.is_empty())?;
    let (assign_id, assign) = rng.choice(&cands).clone();

    // Find the insertion point whose statement is exactly this assignment.
    let mut ins: Option<(Nid, usize)> = None;
    {
        let mut stmt_of_assign: Option<Nid> = None;
        visit_stmts(&f.statement, &mut |s| {
            if let Statement::Expression(Some(e)) = &s.node {
                if nid(e) == assign_id {
                    stmt_of_assign = Some(nid(s));
                }
            }
        });
        let stmt_id = stmt_of_assign.ok_or(Fail)?;
        for p in insertion_points(&f.statement, false) {
            if p.after.as_ref().map(|a| a.id == stmt_id).unwrap_or(false) {
                ins = Some((p.block, p.index));
                break;
            }
        }
    }
    let (ins_block, ins_index) = ins.ok_or(Fail)?;

    let (lvalue, rvalue) = match &assign.node {
        Expression::BinaryOperator(b) => ((*b.node.lhs).clone(), (*b.node.rhs).clone()),
        _ => return Err(Fail),
    };

    // Collect the rvalue's binop nodes in-order.
    fn collect_binops(e: &Node<Expression>, out: &mut Vec<Nid>) {
        if let Expression::BinaryOperator(b) = &e.node {
            if let Expression::BinaryOperator(_) = &b.node.lhs.node {
                collect_binops(&b.node.lhs, out);
            }
            out.push(nid(e));
            if let Expression::BinaryOperator(_) = &b.node.rhs.node {
                collect_binops(&b.node.rhs, out);
            }
        }
    }
    let mut binops = Vec::new();
    collect_binops(&rvalue, &mut binops);
    ensure(!binops.is_empty())?;
    let split = *rng.choice(&binops);

    // Fetch the split node's sides for the type check.
    let mut split_expr: Option<Node<Expression>> = None;
    scan_stmt_exprs(&f.statement, &mut |e, _| {
        if nid(e) == split
            && matches!(e.node, Expression::BinaryOperator(_))
            && split_expr.is_none()
        {
            split_expr = Some(e.clone());
        }
    });
    let split_expr = split_expr.ok_or(Fail)?;
    let (s_lhs, s_rhs) = match &split_expr.node {
        Expression::BinaryOperator(b) => ((*b.node.lhs).clone(), (*b.node.rhs).clone()),
        _ => return Err(Fail),
    };
    let vartype = decayed_expr_type(&lvalue.node, &tm)?;
    let take_left = rng.chance(0.5);
    let side = if take_left { &s_lhs } else { &s_rhs };
    let sidetype = decayed_expr_type(&side.node, &tm)?;
    ensure(same_type(&vartype, &sidetype, &tm, true))?;

    // Mutate: replace the chosen side with the lvalue, insert the new
    // assignment before the original.
    let body = fbody_mut(unit, fn_index);
    let lv = lvalue.node.clone();
    let mut done = false;
    walk_stmt_exprs(body, &mut |e, _| {
        if done || nid(e) != split {
            return false;
        }
        if let Expression::BinaryOperator(b) = &mut e.node {
            if take_left {
                *b.node.lhs = n(lv.clone());
            } else {
                *b.node.rhs = n(lv.clone());
            }
            done = true;
        }
        false
    });
    ensure(done)?;
    let new_assign = expr_stmt(binop(
        BinaryOperator::Assign,
        lvalue.node.clone(),
        side.node.clone(),
    ));
    ensure(insert_block_item(body, ins_block, ins_index, new_assign))
}

/// Port of `perm_ins_block`: wrap a random statement range in
/// `if (1) { ... }` or `do { ... } while (0);`.
fn perm_ins_block(unit: &mut TranslationUnit, fn_index: usize, rng: &mut Rng) -> Result<(), Fail> {
    let f = fdef(unit, fn_index);
    let mut blocks: Vec<(Nid, usize, usize)> = Vec::new(); // (id, len, decl_count)
    fn rec(block: &Node<Statement>, out: &mut Vec<(Nid, usize, usize)>) {
        let items = astutil::block_items(block);
        let mut decl_count = 0;
        for item in items {
            if matches!(item.node, BlockItem::Declaration(_)) {
                decl_count += 1;
            } else {
                break;
            }
        }
        out.push((nid(block), items.len(), decl_count));
        for item in items {
            if let BlockItem::Statement(s) = &item.node {
                for nb in nested_blocks(s) {
                    rec(nb, out);
                }
            }
        }
    }
    rec(&f.statement, &mut blocks);
    ensure(!blocks.is_empty())?;
    let (block_id, len, decl_count) = *rng.choice(&blocks);
    let mut lo = rng.randrange(decl_count, len + 1);
    let mut hi = rng.randrange(decl_count, len + 1);
    if hi < lo {
        std::mem::swap(&mut lo, &mut hi);
    }
    let dowhile = rng.chance(PROB_INS_BLOCK_DOWHILE);
    let body = fbody_mut(unit, fn_index);
    with_block(body, block_id, &mut |items| {
        let inner: Vec<Node<BlockItem>> = items.drain(lo..hi).collect();
        let compound = n(Statement::Compound(inner));
        let stmt = if dowhile {
            Statement::DoWhile(n(DoWhileStatement {
                statement: Box::new(compound),
                expression: Box::new(n(int_expr("0"))),
            }))
        } else {
            Statement::If(n(IfStatement {
                condition: Box::new(n(int_expr("1"))),
                then_statement: Box::new(compound),
                else_statement: None,
            }))
        };
        items.insert(lo, n(BlockItem::Statement(n(stmt))));
    })
    .ok_or(Fail)?;
    Ok(())
}

/// Port of `perm_empty_stmt`: insert a no-op statement.
fn perm_empty_stmt(unit: &mut TranslationUnit, fn_index: usize, rng: &mut Rng) -> Result<(), Fail> {
    let f = fdef(unit, fn_index);
    let cands = insertion_points(&f.statement, false);
    ensure(!cands.is_empty())?;
    let label_name = format!("dummy_label_{}", rng.randint(1, 1_000_000));
    let if_empty = |value: &str| {
        Statement::If(n(IfStatement {
            condition: Box::new(n(int_expr(value))),
            then_statement: Box::new(astutil::empty_compound()),
            else_statement: None,
        }))
    };
    let mut stmts: Vec<Statement> = Vec::new();
    let kind = rng.randrange(0, 6);
    match kind {
        0 => {
            let extra = rng.randint(2, 6);
            let count = *rng.choice(&[1usize, extra]);
            for _ in 0..count {
                stmts.push(if_empty("1"));
            }
        }
        1 => stmts.push(if_empty("0")),
        2 => stmts.push(Statement::DoWhile(n(DoWhileStatement {
            statement: Box::new(astutil::empty_compound()),
            expression: Box::new(n(int_expr("0"))),
        }))),
        3 => stmts.push(Statement::Labeled(n(LabeledStatement {
            label: n(Label::Identifier(n(Identifier {
                name: label_name.clone(),
            }))),
            statement: Box::new(n(Statement::Expression(None))),
        }))),
        4 => {
            stmts.push(Statement::Goto(n(Identifier {
                name: label_name.clone(),
            })));
            stmts.push(Statement::Labeled(n(LabeledStatement {
                label: n(Label::Identifier(n(Identifier {
                    name: label_name.clone(),
                }))),
                statement: Box::new(n(Statement::Expression(None))),
            })));
        }
        _ => stmts.push(Statement::Expression(None)),
    }
    let point = rng.choice(&cands).clone();
    let body = fbody_mut(unit, fn_index);
    for stmt in stmts.into_iter().rev() {
        ensure(insert_block_item(
            body,
            point.block,
            point.index,
            n(BlockItem::Statement(n(stmt))),
        ))?;
    }
    Ok(())
}

/// The AST engine parses with lang-c, which cannot read cpp directives; run
/// the repository compiler's preprocessor over the candidate first.
pub fn preprocess_for_ast(source: &str) -> Result<String, String> {
    let dir = std::env::temp_dir().join("permuter-ast-pp");
    std::fs::create_dir_all(&dir).map_err(|e| format!("{}: {e}", dir.display()))?;
    let input = dir.join(format!("pp-{}.c", std::process::id()));
    std::fs::write(&input, source).map_err(|e| format!("{}: {e}", input.display()))?;
    let out = std::process::Command::new("alchemy-gcc/dist/xgcc")
        .args([
            "-B",
            "alchemy-gcc/dist/",
            "-E",
            "-P",
            "-Iinclude",
            "-D__attribute__(x)=",
        ])
        .arg(&input)
        .output()
        .map_err(|e| format!("xgcc -E: {e}"))?;
    let _ = std::fs::remove_file(&input);
    if !out.status.success() {
        return Err(format!(
            "xgcc -E failed: {}",
            String::from_utf8_lossy(&out.stderr)
        ));
    }
    String::from_utf8(out.stdout).map_err(|e| format!("xgcc -E output: {e}"))
}

#[cfg(test)]
mod tests {
    use lang_c::ast::BlockItem;

    use super::{
        call_sequence, crosses_control_entry, fdef, find_fn, parse_unit, typemap_for, AstPass,
        AstRandomizer,
    };
    use crate::asttypes::{apply_declarator, CType};
    use crate::astutil::{self, id_expr, is_lvalue};

    fn reorder(source: &str, seed: u64) -> Result<String, String> {
        AstRandomizer::new(source, seed, Some(AstPass::ReorderStmts))?.randomize()
    }

    fn reorder_decls(source: &str, seed: u64) -> Result<String, String> {
        AstRandomizer::new(source, seed, Some(AstPass::ReorderDecls))?.randomize()
    }

    fn calls(source: &str) -> Vec<String> {
        let unit = parse_unit(source).unwrap();
        let index = find_fn(&unit, "f").unwrap();
        call_sequence(&fdef(&unit, index).statement)
    }

    #[test]
    fn read_analysis_keeps_assignment_rhs_uses() {
        let source = "int use(int); int f(void) { int size; int copy; size = 4; copy = use(size); size += 1; return size; }";
        let unit = parse_unit(source).unwrap();
        let index = find_fn(&unit, "f").unwrap();
        let reads = astutil::compute_read_locations(fdef(&unit, index));
        assert_eq!(reads.get("size").map(Vec::len), Some(3));
        assert!(!reads.contains_key("copy"));
    }

    #[test]
    fn enum_constants_are_not_pointer_temp_lvalues() {
        let source = "enum Mode { MODE = 1 }; int global; int f(void) { int local; return MODE + local + global; }";
        let unit = parse_unit(source).unwrap();
        let index = find_fn(&unit, "f").unwrap();
        let tm = typemap_for(&unit, index);
        assert!(!is_lvalue(&id_expr("MODE"), &tm));
        assert!(is_lvalue(&id_expr("local"), &tm));
        assert!(is_lvalue(&id_expr("global"), &tm));
    }

    #[test]
    fn temporary_assignment_never_crosses_a_control_entry() {
        let entries = [40, 100];
        assert!(crosses_control_entry(20, 60, &entries));
        assert!(crosses_control_entry(40, 60, &entries));
        assert!(!crosses_control_entry(41, 60, &entries));
        assert!(!crosses_control_entry(20, 40, &entries));
    }

    #[test]
    fn declaration_reorder_stays_in_scope_and_skips_initializers() {
        let source = "int f(int cond) { int chance; int value; { int off; off = cond; chance = off; } value = chance; return value; }";
        let mut successes = 0;
        for seed in 0..64 {
            let Ok(output) = reorder_decls(source, seed) else {
                continue;
            };
            successes += 1;
            let unit = parse_unit(&output).unwrap();
            let index = find_fn(&unit, "f").unwrap();
            let root_names: Vec<String> = astutil::block_items(&fdef(&unit, index).statement)
                .iter()
                .filter_map(|item| match &item.node {
                    BlockItem::Declaration(declaration) => Some(
                        declaration
                            .node
                            .declarators
                            .iter()
                            .filter_map(|declarator| {
                                apply_declarator(CType::int(), &declarator.node.declarator.node).1
                            })
                            .collect::<Vec<_>>(),
                    ),
                    _ => None,
                })
                .flatten()
                .collect();
            assert!(root_names.iter().any(|name| name == "chance"));
            assert!(root_names.iter().any(|name| name == "value"));
        }
        assert!(successes > 0);

        let initialized = "int side(void); int f(void) { int value = side(); return value; }";
        assert!(reorder_decls(initialized, 1).is_err());
    }

    #[test]
    fn reorder_moves_independent_local_scalar_assignments() {
        let source = "int f(int a, int b, int c, int d) { int x; int y; x = a + b; y = c + d; return x + y; }";
        let output = reorder(source, 1).unwrap();
        assert!(output.find("y = (c + d)").unwrap() < output.find("x = (a + b)").unwrap());
    }

    #[test]
    fn reorder_rejects_data_dependencies() {
        let source = "int f(int a) { int x; int y; x = a; y = x + 1; return y; }";
        assert!(reorder(source, 1).is_err());
    }

    #[test]
    fn reorder_never_crosses_calls_or_compound_blocks() {
        let calls = "int first(void); void touch(void); int f(int a, int b) { int x; int y; x = a; touch(); y = b; return x + y; }";
        assert!(reorder(calls, 1).is_err());

        let blocks = "int f(int a) { int x; int y; if (a) { x = 1; } else { y = 2; } return a; }";
        assert!(reorder(blocks, 1).is_err());
    }

    #[test]
    fn reorder_never_transfers_effects_between_switch_handlers() {
        let source = "int f(int effect, int dmg) { int heal = 0; switch (effect) { case 1: heal += dmg; break; case 2: heal += dmg; break; } return heal; }";
        assert!(reorder(source, 1).is_err());
    }

    #[test]
    fn reorder_rejects_memory_and_volatile_accesses() {
        let memory = "int f(int *p, int a) { int x; int y; x = *p; y = a; return x + y; }";
        assert!(reorder(memory, 1).is_err());

        let volatile =
            "int f(volatile int status, int a) { int x; int y; x = status; y = a; return x + y; }";
        assert!(reorder(volatile, 1).is_err());
    }

    #[test]
    fn call_sequence_detects_moves_but_ignores_noop_wrappers() {
        let base = "void first(void); void second(void); void f(void) { first(); second(); }";
        let wrapped =
            "void first(void); void second(void); void f(void) { if (1) { first(); } second(); }";
        let moved = "void first(void); void second(void); void f(void) { second(); first(); }";
        assert_eq!(calls(base), calls(wrapped));
        assert_ne!(calls(base), calls(moved));
    }
}
