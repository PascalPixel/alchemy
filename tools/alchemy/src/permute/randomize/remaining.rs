
use super::*;

fn rename(unit: &mut Node, sites: &Sites, names: &BTreeMap<String, Node>) {
    for (path, _) in expression_nodes(at(unit, &sites.body), &sites.body, false) {
        if let Node::Id { name } = at(unit, &path) { if let Some(value) = names.get(name) { *at_mut(unit, &path) = value.clone(); } }
    }
}

pub(super) fn variables(unit: &mut Node, target: &str, sites: &Sites, kind: Kind, rng: &mut Rng) -> Option<()> {
    let types = Types::new(unit, target);
    let mentioned = mentioned_in_region(unit, sites);
    let mut groups: Vec<(Node, Vec<(isize, String)>)> = Vec::new();
    let mut all_names = BTreeSet::new();
    fn add(groups: &mut Vec<(Node, Vec<(isize, String)>)>, types: &Types, ty: &Node, index: isize, name: &str) {
        if let Some((_, names)) = groups.iter_mut().find(|(old, _)| types.similar(old, ty)) { names.push((index, name.into())); }
        else { groups.push((ty.clone(), vec![(index, name.into())])); }
    }
    for (i, decl) in statements(at(unit, &sites.body))?.iter().enumerate() {
        let Node::Decl { name: Some(name), ty, init, .. } = decl else { continue };
        all_names.insert(name.clone());
        if !mentioned.contains(name) { continue; }
        if kind == Kind::AliasArray && (init.is_some() || matches!(types.resolve(ty)?, Node::ArrayDecl { .. })) { continue; }
        add(&mut groups, &types, ty, i as isize, name);
    }
    if kind == Kind::RemoveVariable {
        let Node::FuncDef { decl, .. } = at(unit, &[sites.body[0]]) else { return None };
        let Node::Decl { ty, .. } = decl.as_ref() else { return None };
        if let Node::FuncDecl { args: Some(args), .. } = ty.as_ref() { if let Node::ParamList { params } = args.as_ref() {
            for param in params { if let Node::Decl { name: Some(name), ty, .. } = param { add(&mut groups, &types, ty, -1, name); } }
        } }
    }
    groups.retain(|(_, names)| names.len() >= 2);
    let (ty, names) = &groups[choose(rng, &groups)?];
    let (a, b) = rng.sample_pair(names.len());
    let ((i, a), (j, b)) = (&names[a], &names[b]);
    if kind == Kind::AliasArray {
        let var = if !all_names.contains("new_var") { "new_var".into() } else { (2..).map(|i| format!("new_var{i}")).find(|n| !all_names.contains(n)).unwrap() };
        let array = Node::ArrayDecl { ty: ty.clone().boxed(), dim: Some(integer(2).boxed()), dim_quals: vec![] };
        let stmts = statements_mut(at_mut(unit, &sites.body));
        stmts[(*i).min(*j) as usize] = types::declaration(&var, array);
        stmts.remove((*i).max(*j) as usize);
        rename(unit, sites, &[(a.clone(), Node::ArrayRef { name: Node::id(&var).boxed(), subscript: integer(0).boxed() }), (b.clone(), Node::ArrayRef { name: Node::id(&var).boxed(), subscript: integer(1).boxed() })].into_iter().collect());
    } else {
        if *i == -1 { return None; }
        statements_mut(at_mut(unit, &sites.body)).remove(*i as usize);
        rename(unit, sites, &[(a.clone(), Node::id(b))].into_iter().collect());
    }
    Some(())
}

pub(super) fn conditional(unit: &mut Node, target: &str, sites: &Sites, rng: &mut Rng) -> Option<()> {
    let blocks = sites.blocks(unit);
    let path = &blocks[choose(rng, &blocks)?];
    let items = statements(at(unit, path))?;
    let mut streaks = Vec::new();
    let mut start = 0;
    for (i, stmt) in items.iter().enumerate() {
        if matches!(stmt, Node::If { .. } | Node::For { .. } | Node::While { .. } | Node::DoWhile { .. } | Node::Switch { .. } | Node::Label { .. } | Node::Case { .. } | Node::Default { .. } | Node::Compound { .. } | Node::Decl { .. } | Node::Typedef { .. } | Node::Pragma { .. }) {
            if start != i { streaks.push((start, i)); } start = i + 1;
        }
    }
    if start != items.len() { streaks.push((start, items.len())); }
    let (lo, hi) = streaks[choose(rng, &streaks)?];
    let mut exprs: Vec<_> = sites.expressions(unit).into_iter().filter(|p| matches!(at(unit, p), Node::Id { .. } | Node::StructRef { .. })).collect();
    let expr = at(unit, &exprs[choose(rng, &exprs)?]).clone();
    if effectful(&expr) { return None; }
    let types = Types::new(unit, target);
    let make_cond = |expr: Node| -> Option<Node> {
        let ty = types.decayed(&expr)?;
        Some(if matches!(types.resolve(&ty)?, Node::TypeDecl { ty, .. } if matches!(ty.as_ref(), Node::Struct { .. } | Node::Union { .. })) { unary("&", expr) } else { expr })
    };
    let mut cond = make_cond(expr.clone())?;
    if rng.random() < 0.25 {
        exprs.retain(|p| at(unit, p) != &expr);
        let other = at(unit, &exprs[choose(rng, &exprs)?]).clone();
        if !effectful(&other) { cond = binary("||", cond, make_cond(other)?); }
    }
    let stmts = statements_mut(at_mut(unit, path));
    let body = compound(stmts[lo..hi].to_vec());
    stmts.splice(lo..hi, [Node::If { cond: cond.boxed(), iftrue: body.clone().boxed(), iffalse: Some(body.boxed()) }]);
    Some(())
}

fn deref(node: Node) -> Node { if let Node::UnaryOp { op, expr } = node { if op == "&" { *expr } else { unary("*", Node::UnaryOp { op, expr }) } } else { unary("*", node) } }
fn addr(node: Node) -> Node { if let Node::UnaryOp { op, expr } = node { if op == "*" { *expr } else { unary("&", Node::UnaryOp { op, expr }) } } else { unary("&", node) } }

pub(super) fn struct_ref(unit: &mut Node, sites: &Sites, rng: &mut Rng) -> Option<()> {
    let candidates: Vec<_> = sites.select(unit, |n| matches!(n, Node::StructRef { .. })).into_iter().filter(|p| p.starts_with(&sites.body)).collect();
    let path = &candidates[choose(rng, &candidates)?];
    let mut node = at(unit, path).clone();
    fn parent(node: &Node, path: Path) -> Option<Option<Path>> {
        match node {
            Node::UnaryOp { op, expr } => { if !["*", "&"].contains(&op.as_str()) { return None; } Some(parent(expr, child(&path, 0))?.or(Some(path))) }
            Node::StructRef { name, .. } => Some(parent(name, child(&path, 0))?.or(Some(path))),
            _ => Some(None),
        }
    }
    let mut parent = parent(&node, vec![])??;
    let mut changed = false;
    if let Node::StructRef { name, op, .. } = &mut node {
        if op == "->" {
            *op = ".".into();
            let removes = matches!(name.as_ref(), Node::UnaryOp { op, .. } if op == "&");
            **name = deref(*name.clone());
            if removes {
                if parent == [0] { parent.clear(); }
                else if parent.starts_with(&[0, 0]) { parent.remove(1); }
            } else if parent.is_empty() { parent = vec![0]; }
            else { parent.insert(1, 0); }
            changed = true;
        }
    }
    let mut leaf = child(&parent, 0);
    if matches!(at(&node, &leaf), Node::ArrayRef { .. } | Node::BinaryOp { .. }) {
        if let Node::ArrayRef { name, subscript } = at(&node, &leaf).clone() {
            let binop = if let Node::BinaryOp { op, left, right } = *subscript {
                if rng.random() < 0.5 && ["+", "-"].contains(&op.as_str()) { binary(&op, binary("+", *name, *left), *right) }
                else { binary("+", *name, binary(&op, *left, *right)) }
            } else { binary("+", *name, *subscript) };
            *at_mut(&mut node, &leaf) = deref(binop);
            parent = leaf;
            leaf = child(&parent, 0);
            changed = true;
        }
        if rng.random() < 0.5 && matches!(at(&node, &parent), Node::UnaryOp { op, .. } if op == "*") {
            let Node::BinaryOp { op, left, right } = at(&node, &leaf).clone() else { return None };
            let subscript = if op == "-" { unary("-", *right) } else { *right };
            *at_mut(&mut node, &leaf) = addr(Node::ArrayRef { name: left, subscript: subscript.boxed() });
            changed = true;
        }
    }
    if rng.random() < 0.5 {
        let Node::StructRef { name, op, .. } = &mut node else { return None };
        **name = addr(*name.clone()); *op = "->".into(); changed = true;
    }
    if !changed { return None; }
    *at_mut(unit, path) = node;
    Some(())
}

fn subset(rng: &mut Rng, paths: &[Path], mid: usize) -> Vec<Path> {
    let probability = if rng.random() < 0.5 { 0.5 } else { 0.0 };
    let mut before: Vec<_> = paths[..mid].iter().filter(|_| rng.random() < probability).cloned().collect();
    let mut after: Vec<_> = paths[mid + 1..].iter().filter(|_| rng.random() < probability).cloned().collect();
    if rng.random() < 0.2 { before = paths[..mid].to_vec(); after = paths[mid + 1..].to_vec(); }
    else if rng.random() < 0.2 { if rng.random() < 0.5 { before = paths[..mid].to_vec(); } else { after = paths[mid + 1..].to_vec(); } }
    before.push(paths[mid].clone()); before.extend(after); before
}

fn as_expr(node: &Node) -> bool { matches!(node, Node::ArrayRef { .. } | Node::Assignment { .. } | Node::BinaryOp { .. } | Node::Cast { .. } | Node::CompoundLiteral { .. } | Node::Constant { .. } | Node::ExprList { .. } | Node::FuncCall { .. } | Node::Id { .. } | Node::StructRef { .. } | Node::TernaryOp { .. } | Node::UnaryOp { .. }) }

fn match_cut(node: &Node, pattern: &Node, path: &[usize], cuts: &[(Path, Node)], types: &Types, args: &mut Vec<Node>) -> bool {
    if let Some((_, ty)) = cuts.iter().find(|(p, _)| p == path) {
        if !as_expr(node) || !types.decayed(node).is_some_and(|node_ty| types.similar(&node_ty, ty)) { return false; }
        args.push(node.clone()); return true;
    }
    if std::mem::discriminant(node) != std::mem::discriminant(pattern) { return false; }
    let (a, b) = (node.children(), pattern.children());
    if a.len() != b.len() { return false; }
    let (mut metadata_a, mut metadata_b) = (node.clone(), pattern.clone());
    for child in metadata_a.children_mut().into_iter().chain(metadata_b.children_mut()) { *child = Node::EmptyStatement {}; }
    if metadata_a != metadata_b { return false; }
    a.into_iter().zip(b).enumerate().all(|(i, (a, b))| match_cut(a, b, &child(path, i), cuts, types, args))
}

pub(super) fn inline(unit: &mut Node, target: &str, sites: &Sites, rng: &mut Rng) -> Option<()> {
    let types = Types::new(unit, target);
    let candidates = sites.expressions(unit);
    if candidates.is_empty() { return None; }
    let chosen = &candidates[weighted(rng, candidates.iter().map(|p| match at(unit, p) { Node::Id { .. } => 0.05, Node::Constant { .. } => 0.01, _ => 1.0 }))];
    let ret = types.decayed(at(unit, chosen))?;
    let Node::FileAst { ext } = unit else { return None };
    let used: BTreeSet<_> = ext.iter().filter_map(name).map(str::to_string).collect();
    let mut helper = String::from("inline_fn");
    let mut counter = 1;
    while used.contains(&helper) { counter += 1; helper.push_str(&counter.to_string()); }
    let probability = rng.random() * rng.random();
    let mut body = at(unit, chosen).clone();
    let mut cuts = Vec::new();
    let local = |n: &Node| matches!(n, Node::Id { name } if types.locals.contains(name));
    if local(&body) || rng.random() < 0.05 { cuts.push((vec![], types.decayed(&body)?)); body = Node::id(""); }
    else {
        for (path, evaluated) in expression_nodes(&body, &[], false) {
            if cuts.iter().any(|(p, _): &(Path, Node)| path.starts_with(p)) { continue; }
            let node = at(&body, &path);
            let cut = if local(node) { if !evaluated { return None; } true } else { evaluated && rng.random() < probability };
            if cut { let ty = types.decayed(node)?; *at_mut(&mut body, &path) = Node::id(""); cuts.push((path, ty)); }
        }
    }
    if cuts.is_empty() && rng.random() < 0.5 { return None; }
    let identity = cuts.iter().any(|(p, _)| p.is_empty());
    let matches: Vec<_> = if identity { vec![chosen.clone()] } else { candidates.iter().filter(|p| match_cut(at(unit, p), &body, &[], &cuts, &types, &mut vec![])).cloned().collect() };
    let mid = matches.iter().position(|p| p == chosen)?;
    let mut selected: Vec<_> = subset(rng, &matches, mid).into_iter().map(Some).collect();
    for index in 0..selected.len() {
        let Some(path) = selected[index].clone() else { continue };
        let mut args = Vec::new();
        if !match_cut(at(unit, &path), &body, &[], &cuts, &types, &mut args) { continue; }
        // When an ancestor becomes a call, candidates retained as arguments move
        // with those nodes and remain eligible for subsequent replacement.
        for pending in &mut selected[index + 1..] {
            let Some(old) = pending.as_ref() else { continue };
            if !old.starts_with(&path) { continue; }
            *pending = cuts.iter().enumerate().find_map(|(i, (cut, _))| {
                let mut prefix = path.clone(); prefix.extend(cut);
                old.strip_prefix(prefix.as_slice()).map(|suffix| {
                    let mut moved = path.clone(); moved.extend([1, i]); moved.extend(suffix); moved
                })
            });
        }
        *at_mut(unit, &path) = Node::FuncCall { name: Node::id(&helper).boxed(), args: Some(Node::ExprList { exprs: args }.boxed()) };
    }
    let mut params = Vec::new();
    for (i, (path, ty)) in cuts.into_iter().enumerate() { let arg = format!("arg{i}"); *at_mut(&mut body, &path) = Node::id(&arg); params.push(types::declaration(&arg, ty)); }
    let mut decl = types::declaration(&helper, Node::FuncDecl { args: Some(Node::ParamList { params }.boxed()), ty: ret.boxed() });
    if let Node::Decl { funcspec, .. } = &mut decl { funcspec.push("inline".into()); }
    let helper = Node::FuncDef { decl: decl.boxed(), param_decls: vec![], body: compound(vec![Node::Return { expr: Some(body.boxed()) }]).boxed() };
    let index = target_index(unit, target)?;
    let Node::FileAst { ext } = unit else { return None };
    ext.insert(index, helper);
    Some(())
}
