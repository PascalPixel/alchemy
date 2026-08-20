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

use lang_c::ast::*;
use lang_c::span::Node;

use crate::asttypes::{
    allowed_basic_type, build_typemap, decayed_expr_type, ensure,
    expr_type, is_assign_op, pointer_decay, resolve_typedefs, same_type, CType, Fail, TypeMap,
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
pub struct Rng(u64);

impl Rng {
    pub fn new(seed: u64) -> Rng {
        Rng(seed)
    }
    fn next(&mut self) -> u64 {
        self.0 = self.0.wrapping_add(0x9e37_79b9_7f4a_7c15);
        let mut value = self.0;
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
}

fn parse_unit(source: &str) -> Result<TranslationUnit, String> {
    let mut config = lang_c::driver::Config::default();
    config.flavor = lang_c::driver::Flavor::GnuC11;
    config.cpp_command = String::new();
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

impl AstRandomizer {
    /// Parses and re-emits the input so that every later parse yields a
    /// fully braced tree with genuine, unique spans. The target function is
    /// the last function definition in the unit.
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
        let mut unit = parse_unit(&self.source)?;
        let fn_index = find_fn(&unit, &self.fn_name)?;
        for _ in 0..1000 {
            let pass = match self.only {
                Some(p) => p,
                None => *random_weighted(&mut self.rng, &DEFAULT_GCC_WEIGHTS),
            };
            if run_pass(pass, &mut unit, fn_index, &mut self.rng).is_ok() {
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
        &["int", "char", "long", "short", "signed", "unsigned", "float", "double"],
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
fn randomize_type(t: &CType, tm: &TypeMap, rng: &mut Rng, ensure_changed: bool) -> Result<CType, Fail> {
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
    fn rec(
        block: &Node<Statement>,
        reuse_in: &[String],
        should_make_ptr: bool,
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
                    let (t, name) = crate::asttypes::apply_declarator(
                        CType::int(),
                        &init.node.declarator.node,
                    );
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
                    rec(nb, &reuse, should_make_ptr, writes, einds, candidates, rng);
                }
            }
            scan_item_exprs(item, &mut |e, is_expr| {
                if !is_expr {
                    return;
                }
                if should_make_ptr && !is_lvalue(&e.node) {
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
                    // If the expression reads something written within
                    // [place, expr), the assignment would move too high up.
                    if place.stmt_start <= prev_write {
                        break;
                    }
                    eind += 1;
                    let mut prob = 1.0 / eind as f64;
                    if matches!(
                        e.node,
                        Expression::Identifier(_) | Expression::Constant(_)
                    ) {
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
    if !wrapped {
        if matches!(resolve_typedefs(t.clone(), &tm), CType::Struct { .. })
            && is_lvalue(&orig.node)
        {
            wrapped = true;
            t = CType::ptr(expr_type(&orig.node, &tm)?);
        }
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
    let mid = replace_cands.iter().position(|&i| i == orig_id).ok_or(Fail)?;
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
fn insert_decl(body: &mut Node<Statement>, var: &str, t: &CType, rng: &mut Rng) -> Result<(), Fail> {
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
        fn scan(stmt: &Node<Statement>, recs: &mut std::collections::BTreeMap<usize, (String, WriteKind)>, writes: &mut std::collections::BTreeMap<String, Vec<usize>>) {
            // Declarations.
            fn handle_decl(d: &Node<Declaration>, recs: &mut std::collections::BTreeMap<usize, (String, WriteKind)>, writes: &mut std::collections::BTreeMap<String, Vec<usize>>) {
                for (di, init) in d.node.declarators.iter().enumerate() {
                    if let (_, Some(name)) = crate::asttypes::apply_declarator(
                        CType::int(),
                        &init.node.declarator.node,
                    ) {
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
            fn exprs(e: &Node<Expression>, recs: &mut std::collections::BTreeMap<usize, (String, WriteKind)>, writes: &mut std::collections::BTreeMap<String, Vec<usize>>) {
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
                                writes.entry(id.node.name.clone()).or_default().push(e.span.start);
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
                                writes.entry(id.node.name.clone()).or_default().push(e.span.start);
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
        let mut hii = if side == 1 { h } else { rng.randint(myi + 1, h) };
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

/// Port of `perm_reorder_stmts`: move one statement somewhere else, with a
/// distance-weighted target distribution.
fn perm_reorder_stmts(
    unit: &mut TranslationUnit,
    fn_index: usize,
    rng: &mut Rng,
) -> Result<(), Fail> {
    let f = fdef(unit, fn_index);
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

    let mut source_inds: Vec<usize> = Vec::new();
    for (i, c) in cands.iter().enumerate() {
        if let Some(info) = &c.after {
            if !info.has_nested_block && !info.is_jump {
                source_inds.push(i);
            }
        }
    }
    ensure(!source_inds.is_empty())?;
    let sourcei = *rng.choice(&source_inds);
    let from = cands[sourcei].clone();
    let from_info = from.after.clone().ok_or(Fail)?;
    let mut sourcei_after = sourcei + 1;
    let after_matches = sourcei_after < cands.len()
        && cands[sourcei_after]
            .before
            .as_ref()
            .map(|b| b.id == from_info.id)
            .unwrap_or(false);
    if from_info.is_decl || sourcei_after == cands.len() || !after_matches {
        sourcei_after -= 1;
    }

    let mut weighted: Vec<(usize, f64)> = Vec::new();
    for i in 0..cands.len() {
        let dist = (sourcei as isize - i as isize).max(i as isize - sourcei_after as isize);
        if dist <= 0 {
            continue;
        }
        if cands[i].after.as_ref().map(|a| a.is_decl).unwrap_or(false) {
            // Don't move to before a declaration (C89 compat).
            continue;
        }
        let prob = ((dist + 1) as f64).powf(-1.5) + 0.5 / cands.len() as f64;
        weighted.push((i, prob));
    }
    ensure(!weighted.is_empty())?;
    let toi_index = *random_weighted(rng, &weighted);
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

/// Port of `perm_reorder_decls`: move one declaration, staying within
/// block starts and declaration runs, never past a use.
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
        .filter(|(_, c)| c.after.as_ref().map(|a| a.is_decl).unwrap_or(false))
        .map(|(i, _)| i)
        .collect();
    ensure(!source_inds.is_empty())?;
    let sourcei = *rng.choice(&source_inds);

    let mut weighted: Vec<(usize, f64)> = Vec::new();
    for i in 0..cands.len() {
        let dist = (sourcei as isize - i as isize).max(i as isize - (sourcei as isize + 1));
        if dist <= 0 {
            continue;
        }
        let prob = ((dist + 1) as f64).powf(-1.5) + 1.0 / cands.len() as f64;
        weighted.push((i, prob));
    }
    ensure(!weighted.is_empty())?;
    let desti = *random_weighted(rng, &weighted);

    let from = cands[sourcei].clone();
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
                if let (_, Some(n2)) = crate::asttypes::apply_declarator(
                    CType::int(),
                    &init.node.declarator.node,
                ) {
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
    let (id, expr) = rng.choice(&cands).clone();
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
    let (id, expr) = rng.choice(&cands).clone();
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
    let (_, expr) = rng.choice(&cands).clone();
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
    let (zid, _) = rng.choice(&zeroes).clone();
    let value = expr.node.clone();
    let body = fbody_mut(unit, fn_index);
    ensure(replace_expr_by_id(body, zid, &mut |old| {
        binop(BinaryOperator::Multiply, value.clone(), old.node)
    }))
}

/// Port of `perm_cast_simple`: cast a random arithmetic expression to a
/// random integral or floating type.
fn perm_cast_simple(unit: &mut TranslationUnit, fn_index: usize, rng: &mut Rng) -> Result<(), Fail> {
    let tm = typemap_for(unit, fn_index);
    let f = fdef(unit, fn_index);
    let cands = block_expressions(&f.statement);
    ensure(!cands.is_empty())?;
    let (id, expr) = rng.choice(&cands).clone();
    ensure_arithmetic_type(&expr.node, &tm)?;
    let integral: [&[&str]; 5] = [&["int"], &["char"], &["long"], &["short"], &["long", "long"]];
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
fn perm_commutative(unit: &mut TranslationUnit, fn_index: usize, rng: &mut Rng) -> Result<(), Fail> {
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
    let target = *rng.choice(&cands);
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
    let (target, expr) = rng.choice(&cands).clone();
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
fn perm_inequalities(unit: &mut TranslationUnit, fn_index: usize, rng: &mut Rng) -> Result<(), Fail> {
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
    let target = *rng.choice(&cands);
    let plus1 = |e: Expression| binop(BinaryOperator::Plus, e, int_expr("1"));
    let minus1 = |e: Expression| binop(BinaryOperator::Minus, e, int_expr("1"));
    enum Plan {
        BothSides(bool),   // true = plus1
        ShiftOp(bool),     // true = change left side
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
                    let toward_le = matches!(op, BinaryOperator::Less | BinaryOperator::GreaterOrEqual);
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
    let target = *rng.choice(&cands);
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
        Statement::For(fo) => cands.push((
            nid(s),
            fo.node.condition.as_ref().map(|c| (**c).clone()),
        )),
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
    let cmp_cond = match &cond.node {
        Expression::BinaryOperator(b) if is_cmp(&b.node.operator.node) => true,
        _ => false,
    };
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
        if nid(e) == split && matches!(e.node, Expression::BinaryOperator(_)) {
            if split_expr.is_none() {
                split_expr = Some(e.clone());
            }
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
