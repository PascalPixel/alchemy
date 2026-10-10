//! Lexical variable-access analysis for temporary and variable mutations.

use super::*;

const MAX_INDEX: usize = 1_000_000_000;

fn read_paths(node: &Node, path: &[usize], out: &mut Vec<Path>) {
    match node {
        Node::Id { .. } => out.push(path.to_vec()),
        Node::Decl { init: Some(init), .. } => read_paths(init, &child(path, 1), out),
        Node::Decl { .. } => {}
        Node::Assignment { lvalue, .. } if matches!(lvalue.as_ref(), Node::Id { .. }) => {}
        Node::UnaryOp { op, expr } if op == "&" && matches!(expr.as_ref(), Node::Id { .. }) => {}
        Node::StructRef { name, .. } => read_paths(name, &child(path, 0), out),
        _ => for (i, n) in node.children().iter().enumerate() { read_paths(n, &child(path, i), out); },
    }
}

struct Access {
    writes: BTreeMap<String, Vec<usize>>,
    // Preserve first-read insertion order when selecting a variable.
    reads: Vec<(String, Vec<Path>)>,
    starts: BTreeMap<Path, usize>,
}

impl Access {
    fn new(unit: &Node, sites: &Sites) -> Self {
        let mut writes: BTreeMap<String, Vec<usize>> = BTreeMap::new();
        let starts = sites.nodes.iter().map(|s| (s.path.clone(), s.start)).collect();
        for site in &sites.nodes {
            let var = match at(unit, &site.path) {
                Node::Decl { name: Some(name), .. } => Some(name),
                Node::Assignment { lvalue, .. } => if let Node::Id { name } = lvalue.as_ref() { Some(name) } else { None },
                Node::UnaryOp { op, expr } if ["++", "--", "p++", "p--"].contains(&op.as_str()) => if let Node::Id { name } = expr.as_ref() { Some(name) } else { None },
                _ => None,
            };
            if let Some(var) = var { writes.entry(var.clone()).or_default().push(site.start); }
        }
        let root = vec![sites.body[0]];
        let mut paths = Vec::new(); read_paths(at(unit, &root), &root, &mut paths);
        let mut reads: Vec<(String, Vec<Path>)> = Vec::new();
        for path in paths {
            let Node::Id { name } = at(unit, &path) else { unreachable!() };
            if let Some((_, list)) = reads.iter_mut().find(|(var, _)| var == name) { list.push(path); }
            else { reads.push((name.clone(), vec![path])); }
        }
        Self { writes, reads, starts }
    }

    fn surround(&self, expr: &Node, base: &[usize]) -> (isize, usize) {
        let mut paths = Vec::new(); read_paths(expr, &[], &mut paths);
        let (mut before, mut after) = (-1, MAX_INDEX);
        for path in paths {
            let Node::Id { name } = at(expr, &path) else { unreachable!() };
            let Some(writes) = self.writes.get(name) else { continue };
            let i = writes.partition_point(|&w| w < self.starts[base]);
            if i > 0 { before = before.max(writes[i - 1] as isize); }
            if let Some(&write) = writes.get(i) { after = after.min(write); }
        }
        (before, after)
    }
}

fn replacements(unit: &mut Node, sites: &Sites, changes: &BTreeMap<Path, Node>) {
    let mut replaced: Vec<Path> = Vec::new();
    for (path, _) in expression_nodes(at(unit, &sites.body), &sites.body, false) {
        if replaced.iter().any(|p| path.starts_with(p)) { continue; }
        if let Some(value) = changes.get(&path) {
            *at_mut(unit, &path) = value.clone();
            replaced.push(path);
        }
    }
    fn clean(node: &mut Node) {
        if let Node::For { init, next, .. } = node {
            for part in [init, next] { if matches!(part.as_deref(), Some(Node::EmptyStatement {})) { *part = None; } }
        }
        for child in node.children_mut() { clean(child); }
    }
    clean(at_mut(unit, &sites.body));
}

fn insert_decl(unit: &mut Node, sites: &Sites, var: &str, ty: Node, rng: &mut Rng) {
    let items = statements_mut(at_mut(unit, &sites.body));
    let count = items.iter().take_while(|n| matches!(n, Node::Decl { .. })).count();
    items.insert(rng.below(count + 1), types::declaration(var, ty));
}

pub(super) fn expand(unit: &mut Node, sites: &Sites, rng: &mut Rng) -> Option<()> {
    let access = Access::new(unit, sites);
    let candidates: Vec<_> = access.reads.iter().flat_map(|(name, paths)| paths.iter().filter(|p| sites.allows_pre(p)).map(move |p| (name, p))).collect();
    let (var, path) = candidates.get(choose(rng, &candidates)?)?;
    let index = access.starts[*path];
    let writes = access.writes.get(*var)?;
    let i = writes.partition_point(|&w| w < index);
    if i == 0 { return None; }
    let before = writes[i - 1];
    let after = writes.get(i).copied().unwrap_or(MAX_INDEX);
    let write = access.starts.iter().find_map(|(p, &start)| (start == before).then_some(p))?;
    let expr = match at(unit, write) {
        Node::Decl { init: Some(init), .. } if !matches!(init.as_ref(), Node::InitList { .. }) => *init.clone(),
        Node::Assignment { op, rvalue, .. } if op == "=" => *rvalue.clone(),
        _ => return None,
    };
    let declaration = matches!(at(unit, write), Node::Decl { .. });
    let mut uses: Vec<_> = access.reads.iter().find(|(name, _)| name == *var)?.1.iter().filter(|p| before < access.starts[*p] && access.starts[*p] < after && sites.allows_pre(p)).cloned().collect();
    let mid = uses.iter().position(|p| p == *path)?;
    let keep = if rng.random() >= 0.3 && uses.len() > 1 {
        let side = rng.below(3);
        let mut lo = if side == 0 { 0 } else { rng.below(mid + 1) };
        let mut hi = if side == 1 { uses.len() } else { mid + 1 + rng.below(uses.len() - mid) };
        if lo == 0 && hi == uses.len() { lo = mid; hi = mid + 1; }
        uses.drain(lo..hi);
        true
    } else { rng.random() < 0.2 };
    if effectful(&expr) && (uses.len() != 1 || keep) { return None; }
    let mut changes: BTreeMap<_, _> = uses.into_iter().map(|p| (p, expr.clone())).collect();
    if !keep {
        if let Node::Assignment { lvalue, .. } = at(unit, write) {
            let evaluated = expression_nodes(at(unit, &sites.body), &sites.body, false).into_iter().find(|(p, _)| p == write).is_some_and(|(_, evaluated)| evaluated);
            changes.insert(write.clone(), if evaluated { *lvalue.clone() } else { Node::EmptyStatement {} });
        }
    }
    replacements(unit, sites, &changes);
    if !keep && declaration { if let Node::Decl { init, .. } = at_mut(unit, write) { *init = None; } }
    Some(())
}

fn nested(unit: &Node, path: &[usize]) -> Vec<Path> {
    match at(unit, path) {
        Node::Compound { .. } | Node::Case { .. } | Node::Default { .. } => vec![path.to_vec()],
        Node::If { iffalse, .. } => { let mut out = vec![child(path, 1)]; if iffalse.is_some() { out.push(child(path, 2)); } out },
        Node::For { .. } => vec![child(path, at(unit, path).children().len() - 1)],
        Node::While { .. } | Node::DoWhile { .. } | Node::Switch { .. } => vec![child(path, 1)],
        Node::Label { .. } => nested(unit, &child(path, 0)),
        _ => vec![],
    }
}

fn lvalue(node: &Node) -> bool {
    matches!(node, Node::Id { .. } | Node::ArrayRef { .. } | Node::StructRef { .. }) || matches!(node, Node::UnaryOp { op, .. } if op == "*")
}

struct Candidate { block: Path, index: usize, statement: Path, original: Path, expr: Node, reuse: Option<String>, weight: f64 }

pub(super) fn temporary(unit: &mut Node, target: &str, sites: &Sites, rng: &mut Rng) -> Option<()> {
    let access = Access::new(unit, sites);
    let types = Types::new(unit, target);
    let mut pointer = rng.random() < 0.05;
    let mut candidates = Vec::new();
    let mut einds: BTreeMap<Path, usize> = BTreeMap::new();
    #[allow(clippy::too_many_arguments)]
    fn collect(unit: &Node, sites: &Sites, access: &Access, rng: &mut Rng, block: &Path, mut reuse: Vec<String>, pointer: bool, einds: &mut BTreeMap<Path, usize>, out: &mut Vec<Candidate>) -> Option<()> {
        let mut places = Vec::new();
        let mut past_decls = false;
        for (i, stmt) in statements(at(unit, block))?.iter().enumerate() {
            let statement = statement_path(block, at(unit, block), i);
            if let Node::Decl { name, ty, .. } = stmt {
                let name = name.as_ref()?;
                if !matches!(ty.as_ref(), Node::ArrayDecl { .. }) {
                    reuse.push(name.clone());
                    if !matches!(ty.as_ref(), Node::PtrDecl { .. }) { reuse.push(name.clone()); }
                }
            } else if !matches!(stmt, Node::Pragma { .. }) { past_decls = true; }
            if past_decls { places.push((i, statement.clone())); }
            for path in nested(unit, &statement) { collect(unit, sites, access, rng, &path, reuse.clone(), pointer, einds, out)?; }
            let mut exprs = Vec::new(); expressions(stmt, &statement, false, &mut exprs);
            for original in exprs {
                if !sites.allows(&original) || (pointer && !lvalue(at(unit, &original))) { continue; }
                let expr = if pointer { unary("&", at(unit, &original).clone()) } else { at(unit, &original).clone() };
                let mut eind = if pointer { 0 } else { einds.get(&original).copied().unwrap_or(0) };
                let (before, _) = access.surround(&expr, &original);
                for (index, statement) in places.iter().rev() {
                    if access.starts[statement] as isize <= before { break; }
                    eind += 1;
                    let mut weight = 1.0 / eind as f64;
                    if matches!(at(unit, &original), Node::Id { .. } | Node::Constant { .. }) { weight *= if pointer { 0.15 } else { 0.5 }; }
                    let reuse = choose(rng, &reuse).map(|i| reuse[i].clone());
                    out.push(Candidate { block: block.clone(), index: *index, statement: statement.clone(), original: original.clone(), expr: expr.clone(), reuse, weight });
                }
                if !pointer { einds.insert(original, eind); }
            }
        }
        Some(())
    }
    collect(unit, sites, &access, rng, &sites.body, vec![], pointer, &mut einds, &mut candidates)?;
    if candidates.is_empty() { return None; }
    let candidate = &candidates[weighted(rng, candidates.iter().map(|c| c.weight))];
    let inline = rng.random() < 0.1;
    let mut expr = candidate.expr.clone();
    let mut ty = types.decayed(&expr)?;
    if !pointer && matches!(&ty, Node::TypeDecl { ty, .. } if matches!(ty.as_ref(), Node::Struct { .. } | Node::Union { .. })) && lvalue(&expr) {
        pointer = true; expr = unary("&", expr); ty = types.decayed(&expr)?;
    }
    let assign_before = if inline { &candidate.original } else { &candidate.statement };
    let reuse = if rng.random() < 0.5 {
        candidate.reuse.as_ref().filter(|var| {
            let Some(var_type) = types.decayed(&Node::id(*var)) else { return false };
            if !types.similar(&var_type, &ty) { return false; }
            let start = access.starts[assign_before];
            let write = access.writes.get(*var).and_then(|list| list.iter().find(|&&i| i >= start)).copied();
            let read = access.reads.iter().find(|(v, _)| v == *var).and_then(|(_, list)| list.iter().map(|p| access.starts[p]).find(|&i| i >= start));
            !read.is_some_and(|r| write.is_none_or(|w| w >= r)) && !write.is_some_and(|w| w < access.starts[&candidate.original])
        }).cloned()
    } else { None };
    let var = reuse.clone().unwrap_or_else(|| {
        if !access.writes.contains_key("new_var") { "new_var".into() }
        else { (2..).map(|i| format!("new_var{i}")).find(|n| !access.writes.contains_key(n)).unwrap() }
    });
    let (before, after) = access.surround(&expr, &candidate.original);
    let before = before.max(access.starts[assign_before] as isize - 1);
    let mut duplicates = Vec::new();
    if effectful(&expr) { duplicates.push(candidate.original.clone()); }
    else {
        let mut paths = Vec::new(); expressions(at(unit, &sites.body), &sites.body, false, &mut paths);
        for path in paths { if before < access.starts[&path] as isize && access.starts[&path] <= after && at(unit, &path) == at(unit, &candidate.original) { duplicates.push(path); } }
    }
    let mid = duplicates.iter().position(|p| p == &candidate.original)?;
    let probability = if rng.random() < 0.5 { 0.5 } else { 0.0 };
    let mut before_set: Vec<_> = duplicates[..mid].iter().filter(|_| rng.random() < probability).cloned().collect();
    let mut after_set: Vec<_> = duplicates[mid + 1..].iter().filter(|_| rng.random() < probability).cloned().collect();
    if rng.random() < 0.2 { before_set = duplicates[..mid].to_vec(); after_set = duplicates[mid + 1..].to_vec(); }
    else if rng.random() < 0.2 { if rng.random() < 0.5 { before_set = duplicates[..mid].to_vec(); } else { after_set = duplicates[mid + 1..].to_vec(); } }
    before_set.push(candidate.original.clone()); before_set.extend(after_set);
    let changes: BTreeMap<_, _> = before_set.into_iter().map(|path| {
        let value = if inline && path == candidate.original { assign(Node::id(&var), expr.clone()) } else { Node::id(&var) };
        (path, if pointer { unary("*", value) } else { value })
    }).collect();
    replacements(unit, sites, &changes);
    if !inline { statements_mut(at_mut(unit, &candidate.block)).insert(candidate.index, assign(Node::id(&var), expr)); }
    if reuse.is_none() {
        if rng.random() < 0.3 && types.integral(&ty) { ty = random_type(rng); }
        insert_decl(unit, sites, &var, ty, rng);
    }
    Some(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn indexed_reads_and_writes_match_visitors() {
        let mut unit = super::super::super::c::parse(include_str!("../c/fixtures/mutations-lifetime.c")).unwrap();
        unit.extract_function("target").unwrap();
        let sites = Sites::build(&unit, "target").unwrap();
        let access = Access::new(&unit, &sites);
        let mut rows = Vec::new();
        for (name, indices) in &access.writes { rows.push(format!("write\t{name}\t{}", indices.iter().map(usize::to_string).collect::<Vec<_>>().join(","))); }
        for (name, paths) in &access.reads { rows.push(format!("read\t{name}\t{}", paths.iter().map(|p| access.starts[p].to_string()).collect::<Vec<_>>().join(","))); }
        assert_eq!(rows.join("\n") + "\n", include_str!("../c/fixtures/mutations-access.tsv"));
    }
}
