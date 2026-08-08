//! Port of `tools/make/message_archive.ts` (Golden Sun message archive codec).
//!
//! PORT NOTE: the TypeScript file has no local imports, so nothing was inlined
//! from other modules. Everything below is a direct translation of that one file.
//!
//! PORT NOTE: `ContextSource` values are, in the TypeScript, only ever produced
//! internally (by `derivedContexts` or `exportModel`) and never parsed from
//! untrusted JSON. The `Object.keys(source).sort().join(",") !== "id,leaves,tree"`
//! guard in `compileContext` is therefore statically true here, because the Rust
//! `ContextSource` is a struct with exactly those three fields. The check is
//! dropped; every other validation in that function is kept.
//!
//! PORT NOTE: on failure the TypeScript CLI lets the exception escape, so Bun
//! prints a JS stack trace to stderr and exits 1. The Rust CLI prints
//! `error: <message>` to stderr and exits 1. The message text is byte-identical
//! for every error the tool itself raises; the surrounding stack-trace
//! decoration is not reproduced, and I/O failures (missing file, malformed
//! JSON) carry Rust's wording rather than Bun's. Exit codes match.
//!
//! PORT NOTE: message text is iterated by Unicode scalar value, matching the
//! JS `for...of` over a string (code points, not UTF-16 code units), so an
//! astral character is one out-of-range token in both implementations.

use std::collections::HashMap;
use std::path::Path;

use serde_json::{Map, Number, Value};

pub const ROM_BASE: u32 = 0x0800_0000;
pub const ARCHIVE_ADDRESS: u32 = 0x0803_7464;
pub const OFFSET_TABLE_ADDRESS: u32 = 0x0803_8334;
pub const MODEL_DIRECTORY_ADDRESS: u32 = 0x0803_842c;
pub const MESSAGE_ADDRESS: u32 = 0x0803_8434;
pub const MESSAGE_DIRECTORY_ADDRESS: u32 = 0x0807_36b8;
pub const ARCHIVE_END: u32 = 0x0807_3808;
pub const CONTEXT_COUNT: usize = 123;
pub const OFFSET_COUNT: usize = 124;
pub const BANK_COUNT: usize = 42;
pub const BANK_SIZE: usize = 256;

/// Number of messages in the final (short) bank.
const LAST_BANK_SIZE: usize = 227;

type Res<T> = Result<T, String>;

const COMMANDS: &[(u32, &str, usize)] = &[
    (1, "page_break", 0),
    (2, "end_wait", 0),
    (3, "line_break", 0),
    (4, "pause_60", 0),
    (5, "pause_20_skippable", 0),
    (6, "pause_120_skippable", 0),
    (7, "reset_text_style", 0),
    (8, "text_color", 1),
    (9, "text_effect", 1),
    (16, "protagonist_name", 0),
    (17, "actor_name", 1),
    (18, "argument_actor_name", 1),
    (19, "argument_class_name", 1),
    (20, "argument_item_name", 1),
    (21, "argument_ability_name", 0),
    (22, "argument_signed_number", 0),
    (23, "argument_location_name", 0),
    (24, "em_dash", 0),
    (25, "plural_suffix", 0),
    (26, "button_icon", 1),
    (27, "possessive_suffix", 0),
    (29, "article_class", 1),
    (30, "end_now", 0),
];

fn command_by_opcode(opcode: u32) -> Option<(&'static str, usize)> {
    COMMANDS
        .iter()
        .find(|(code, _, _)| *code == opcode)
        .map(|(_, name, arguments)| (*name, *arguments))
}

fn command_by_name(name: &str) -> Option<(u32, usize)> {
    COMMANDS
        .iter()
        .find(|(_, candidate, _)| *candidate == name)
        .map(|(opcode, _, arguments)| (*opcode, *arguments))
}

// ---------------------------------------------------------------------------
// bit stream
// ---------------------------------------------------------------------------

struct BitReader<'a> {
    source: &'a [u8],
    position: usize,
    limit: usize,
}

impl<'a> BitReader<'a> {
    fn new(source: &'a [u8], byte: usize, limit: usize) -> Self {
        BitReader { source, position: byte * 8, limit }
    }

    fn whole(source: &'a [u8], byte: usize) -> Self {
        let limit = source.len() * 8;
        BitReader::new(source, byte, limit)
    }

    fn read(&mut self) -> Res<u8> {
        if self.position >= self.limit {
            return Err("bit stream exceeds its extent".into());
        }
        // JS reads `source[i]` which is `undefined` past the end; the shifts then
        // coerce it to 0. Mirror that rather than panicking.
        let byte = self.source.get(self.position >> 3).copied().unwrap_or(0);
        let bit = (byte >> (self.position & 7)) & 1;
        self.position += 1;
        Ok(bit)
    }
}

fn bit_at(source: &[u8], position: usize) -> u8 {
    (source.get(position >> 3).copied().unwrap_or(0) >> (position & 7)) & 1
}

// ---------------------------------------------------------------------------
// number coercion (JS `Number()` + `Number.isSafeInteger`)
// ---------------------------------------------------------------------------

/// Emulates the JS `Number(string)` conversion closely enough for this tool.
/// Returns `None` for `NaN`.
fn js_number_from_string(text: &str) -> Option<f64> {
    const WHITESPACE: &[char] = &[
        '\u{9}', '\u{a}', '\u{b}', '\u{c}', '\u{d}', '\u{20}', '\u{a0}', '\u{1680}', '\u{2000}',
        '\u{2001}', '\u{2002}', '\u{2003}', '\u{2004}', '\u{2005}', '\u{2006}', '\u{2007}',
        '\u{2008}', '\u{2009}', '\u{200a}', '\u{2028}', '\u{2029}', '\u{202f}', '\u{205f}',
        '\u{3000}', '\u{feff}',
    ];
    let trimmed = text.trim_matches(|c| WHITESPACE.contains(&c));
    if trimmed.is_empty() {
        return Some(0.0);
    }
    if let Some(rest) = trimmed.strip_prefix("0x").or_else(|| trimmed.strip_prefix("0X")) {
        return u128::from_str_radix(rest, 16).ok().map(|v| v as f64);
    }
    if let Some(rest) = trimmed.strip_prefix("0o").or_else(|| trimmed.strip_prefix("0O")) {
        return u128::from_str_radix(rest, 8).ok().map(|v| v as f64);
    }
    if let Some(rest) = trimmed.strip_prefix("0b").or_else(|| trimmed.strip_prefix("0B")) {
        return u128::from_str_radix(rest, 2).ok().map(|v| v as f64);
    }
    match trimmed {
        "Infinity" | "+Infinity" => return Some(f64::INFINITY),
        "-Infinity" => return Some(f64::NEG_INFINITY),
        _ => {}
    }
    // Rust accepts "inf"/"nan"/"NaN" spellings that JS does not.
    if trimmed.chars().any(|c| c.is_ascii_alphabetic() && c != 'e' && c != 'E') {
        return None;
    }
    trimmed.parse::<f64>().ok()
}

fn is_safe_integer(value: f64) -> bool {
    value.is_finite() && value.trunc() == value && value.abs() <= 9_007_199_254_740_991.0
}

fn integer(value: Option<&Value>, label: &str) -> Res<i64> {
    let parsed = match value {
        Some(Value::String(text)) => js_number_from_string(text),
        Some(Value::Number(number)) => number.as_f64(),
        _ => None,
    };
    match parsed {
        Some(number) if is_safe_integer(number) => Ok(number as i64),
        _ => Err(format!("{label} must be an integer")),
    }
}

fn bounded(value: Option<&Value>, minimum: i64, maximum: i64, label: &str) -> Res<i64> {
    let parsed = integer(value, label)?;
    if parsed < minimum || parsed > maximum {
        return Err(format!("{label} is outside its range"));
    }
    Ok(parsed)
}

/// `bounded` for values that are already Rust integers.
fn bounded_raw(value: i64, minimum: i64, maximum: i64, label: &str) -> Res<i64> {
    if value < minimum || value > maximum {
        return Err(format!("{label} is outside its range"));
    }
    Ok(value)
}

fn hex(value: u32, width: usize) -> String {
    format!("0x{:0width$x}", value, width = width)
}

fn hex8(value: u32) -> String {
    hex(value, 8)
}

// ---------------------------------------------------------------------------
// ROM accessors
// ---------------------------------------------------------------------------

fn rom_offset(address: u32, source: &[u8], allow_end: bool) -> Res<usize> {
    let offset = address as i64 - ROM_BASE as i64;
    let length = source.len() as i64;
    if offset < 0 || offset > length || (!allow_end && offset == length) {
        return Err(format!("{} is outside the ROM", hex8(address)));
    }
    Ok(offset as usize)
}

fn pointer(source: &[u8], offset: usize) -> Res<u32> {
    if offset + 4 > source.len() {
        return Err("pointer is outside the ROM".into());
    }
    Ok(u32::from_le_bytes([
        source[offset],
        source[offset + 1],
        source[offset + 2],
        source[offset + 3],
    ]))
}

fn halfword(source: &[u8], offset: usize) -> Res<u16> {
    if offset + 2 > source.len() {
        return Err("halfword is outside the ROM".into());
    }
    Ok(u16::from_le_bytes([source[offset], source[offset + 1]]))
}

// ---------------------------------------------------------------------------
// prefix-code model
// ---------------------------------------------------------------------------

#[derive(Debug, Clone, PartialEq, Eq)]
enum TreeNode {
    Leaf(u32),
    Branch(Box<TreeNode>, Box<TreeNode>),
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct ContextSource {
    pub id: usize,
    pub tree: String,
    pub leaves: Vec<u32>,
}

#[derive(Debug)]
struct CompiledContext {
    root: TreeNode,
    paths: HashMap<u32, String>,
}

fn bit_buffer(bits: &str) -> Res<Vec<u8>> {
    if !bits.bytes().all(|b| b == b'0' || b == b'1') {
        return Err("bit source is not binary".into());
    }
    let mut result = vec![0u8; bits.len().div_ceil(8)];
    for (index, byte) in bits.bytes().enumerate() {
        if byte == b'1' {
            result[index >> 3] |= 1 << (index & 7);
        }
    }
    Ok(result)
}

fn symbol_at(source: &[u8], boundary: usize, rank: usize) -> Res<u32> {
    let distance = rank + (rank >> 1);
    let first = boundary as i64 - distance as i64 - 1;
    let second = first - 1;
    if second < 0 || first >= source.len() as i64 {
        return Err("prefix-code alphabet exceeds its record".into());
    }
    let first = first as usize;
    let second = second as usize;
    Ok(if rank.is_multiple_of(2) {
        ((source[first] as u32) << 4) | (source[second] as u32 >> 4)
    } else {
        ((source[first] as u32 & 0x0f) << 8) | source[second] as u32
    })
}

fn packed_leaves(leaves: &[u32]) -> Res<Vec<u8>> {
    if leaves.is_empty() {
        return Err("context leaves must be a nonempty array".into());
    }
    let mut groups: Vec<Vec<u8>> = Vec::new();
    let mut index = 0;
    while index < leaves.len() {
        let first = bounded_raw(leaves[index] as i64, 0, 0xfff, "context symbol")? as u32;
        if index + 1 == leaves.len() {
            groups.push(vec![((first & 0x0f) << 4) as u8, (first >> 4) as u8]);
            break;
        }
        let second = bounded_raw(leaves[index + 1] as i64, 0, 0xfff, "context symbol")? as u32;
        groups.push(vec![
            (second & 0xff) as u8,
            (((first & 0x0f) << 4) | (second >> 8)) as u8,
            (first >> 4) as u8,
        ]);
        index += 2;
    }
    groups.reverse();
    Ok(groups.concat())
}

fn compile_context(source: &ContextSource) -> Res<CompiledContext> {
    let bits = source.tree.as_bytes();
    if bits.is_empty() || !bits.iter().all(|b| *b == b'0' || *b == b'1') {
        return Err(format!("context {} has an invalid tree", source.id));
    }
    if source.leaves.is_empty() {
        return Err(format!("context {} has no leaves", source.id));
    }
    struct State<'a> {
        bits: &'a [u8],
        leaves: &'a [u32],
        id: usize,
        position: usize,
        leaf: usize,
        paths: HashMap<u32, String>,
    }
    fn parse(state: &mut State, path: String) -> Res<TreeNode> {
        if state.position >= state.bits.len() {
            return Err(format!("context {} tree is truncated", state.id));
        }
        let bit = state.bits[state.position];
        state.position += 1;
        if bit == b'1' {
            if state.leaf >= state.leaves.len() {
                return Err(format!("context {} has too few leaves", state.id));
            }
            let symbol =
                bounded_raw(state.leaves[state.leaf] as i64, 0, 0xfff, "context symbol")? as u32;
            state.leaf += 1;
            if state.paths.contains_key(&symbol) {
                return Err(format!("context {} repeats symbol {}", state.id, symbol));
            }
            state.paths.insert(symbol, path);
            return Ok(TreeNode::Leaf(symbol));
        }
        let left = parse(state, format!("{path}0"))?;
        let right = parse(state, format!("{path}1"))?;
        Ok(TreeNode::Branch(Box::new(left), Box::new(right)))
    }
    let mut state = State {
        bits,
        leaves: &source.leaves,
        id: source.id,
        position: 0,
        leaf: 0,
        paths: HashMap::new(),
    };
    let root = parse(&mut state, String::new())?;
    if state.position != bits.len() || state.leaf != source.leaves.len() {
        return Err(format!("context {} tree and leaves differ", source.id));
    }
    Ok(CompiledContext { root, paths: state.paths })
}

fn compile_contexts(source: &[ContextSource]) -> Res<HashMap<usize, CompiledContext>> {
    let mut result = HashMap::new();
    for item in source {
        let id = bounded_raw(item.id as i64, 0, CONTEXT_COUNT as i64 - 1, "context ID")? as usize;
        if result.contains_key(&id) {
            return Err(format!("context {id} is duplicated"));
        }
        result.insert(id, compile_context(item)?);
    }
    Ok(result)
}

// ---------------------------------------------------------------------------
// document
// ---------------------------------------------------------------------------

fn sorted_keys(map: &Map<String, Value>) -> String {
    // JS sorts by UTF-16 code unit; for the ASCII keys involved this equals a
    // byte-wise sort.
    let mut keys: Vec<&str> = map.keys().map(|k| k.as_str()).collect();
    keys.sort_unstable();
    keys.join(",")
}

fn strict_number(value: Option<&Value>) -> Option<f64> {
    match value {
        Some(Value::Number(number)) => number.as_f64(),
        _ => None,
    }
}

fn parse_document(value: &Value) -> Res<&Vec<Value>> {
    let source = match value {
        Value::Object(map) => map,
        _ => return Err("message archive source must be an object".into()),
    };
    if sorted_keys(source) != "address,bank_size,banks,format,kind,size" {
        return Err("message archive source has unknown fields".into());
    }
    if strict_number(source.get("format")) != Some(1.0)
        || source.get("kind").and_then(Value::as_str) != Some("golden-sun-message-archive")
    {
        return Err("unsupported message archive source".into());
    }
    if source.get("address").and_then(Value::as_str) != Some(hex8(ARCHIVE_ADDRESS).as_str())
        || source.get("size").and_then(Value::as_str)
            != Some(hex8(ARCHIVE_END - ARCHIVE_ADDRESS).as_str())
        || strict_number(source.get("bank_size")) != Some(BANK_SIZE as f64)
    {
        return Err("message archive layout differs".into());
    }
    let banks = match source.get("banks") {
        Some(Value::Array(banks)) if banks.len() == BANK_COUNT => banks,
        _ => return Err("message archive collections differ".into()),
    };
    for (index, bank) in banks.iter().enumerate() {
        let expected = if index + 1 == BANK_COUNT { LAST_BANK_SIZE } else { BANK_SIZE };
        match bank {
            Value::Array(items) if items.len() == expected => {}
            _ => return Err(format!("message bank {index} has the wrong size")),
        }
    }
    Ok(banks)
}

fn build_model(source: &[ContextSource]) -> Res<(Vec<u8>, HashMap<usize, CompiledContext>)> {
    let contexts = compile_contexts(source)?;
    let mut entries: HashMap<usize, &ContextSource> = HashMap::new();
    for item in source {
        entries.insert(item.id, item);
    }
    let mut records: Vec<u8> = Vec::new();
    let mut offsets = vec![0u8; OFFSET_COUNT * 2];
    let mut cursor: usize = 0;
    for id in 0..CONTEXT_COUNT {
        let Some(entry) = entries.get(&id) else {
            offsets[id * 2..id * 2 + 2].copy_from_slice(&0x8000u16.to_le_bytes());
            continue;
        };
        let leaves = packed_leaves(&entry.leaves)?;
        let tree = bit_buffer(&entry.tree)?;
        let value = cursor + leaves.len();
        if value > 0xffff {
            return Err("context model size differs".into());
        }
        offsets[id * 2..id * 2 + 2].copy_from_slice(&(value as u16).to_le_bytes());
        cursor += leaves.len() + tree.len();
        records.extend_from_slice(&leaves);
        records.extend_from_slice(&tree);
    }
    if cursor != (OFFSET_TABLE_ADDRESS - ARCHIVE_ADDRESS) as usize {
        return Err("context model size differs".into());
    }
    let mut data = records;
    data.extend_from_slice(&offsets);
    data.extend_from_slice(&ARCHIVE_ADDRESS.to_le_bytes());
    data.extend_from_slice(&OFFSET_TABLE_ADDRESS.to_le_bytes());
    Ok((data, contexts))
}

// ---------------------------------------------------------------------------
// messages
// ---------------------------------------------------------------------------

fn tokens_from_message(source: &Value) -> Res<Option<Vec<u32>>> {
    if source.is_null() {
        return Ok(None);
    }
    let owned;
    let atoms: &[Value] = match source {
        Value::String(_) => {
            owned = vec![source.clone()];
            &owned
        }
        Value::Array(items) => items,
        _ => return Err("message must be text, atoms, or null".into()),
    };
    let mut tokens: Vec<u32> = Vec::new();
    for atom in atoms {
        if let Value::String(text) = atom {
            for character in text.chars() {
                let token = character as u32;
                if !(32..=122).contains(&token) {
                    return Err("message text is outside the recovered ASCII range".into());
                }
                tokens.push(token);
            }
            continue;
        }
        let object = match atom {
            Value::Object(map) => Some(map),
            _ => None,
        };
        let is_control =
            object.map(|map| map.len() == 1 && map.contains_key("control")).unwrap_or(false);
        if !is_control {
            let map = match object {
                Some(map) => map,
                None => return Err("message command atom is invalid".into()),
            };
            let name = match map.get("command") {
                Some(Value::String(name)) => name,
                _ => return Err("message command atom is invalid".into()),
            };
            let Some((opcode, arguments)) = command_by_name(name) else {
                return Err(format!("unknown message command {name}"));
            };
            let keys = sorted_keys(map);
            if arguments == 0 {
                if keys != "command" {
                    return Err(format!("message command {name} takes no argument"));
                }
                tokens.push(opcode);
            } else {
                if keys != "argument,command" {
                    return Err(format!("message command {name} requires an argument"));
                }
                tokens.push(opcode);
                tokens.push(
                    bounded(map.get("argument"), 0, 122, "message command argument")? as u32
                );
            }
            continue;
        }
        let map = object.expect("control atom is an object");
        tokens.push(bounded(map.get("control"), 1, 31, "message control")? as u32);
    }
    Ok(Some(tokens))
}

fn message_from_tokens(tokens: &[u32]) -> Res<Value> {
    let mut atoms: Vec<Value> = Vec::new();
    let mut text = String::new();
    let mut index = 0;
    while index < tokens.len() {
        let token = tokens[index];
        if (32..=122).contains(&token) {
            text.push(char::from_u32(token).expect("ASCII token"));
        } else {
            if token == 0 || token > 31 {
                return Err(format!("message symbol {token} has no source representation"));
            }
            if !text.is_empty() {
                atoms.push(Value::String(std::mem::take(&mut text)));
            }
            match command_by_opcode(token) {
                None => {
                    let mut map = Map::new();
                    map.insert("control".into(), Value::Number(Number::from(token)));
                    atoms.push(Value::Object(map));
                }
                Some((name, 0)) => {
                    let mut map = Map::new();
                    map.insert("command".into(), Value::String(name.into()));
                    atoms.push(Value::Object(map));
                }
                Some((name, _)) => {
                    if index + 1 >= tokens.len() {
                        return Err(format!("message command {name} lacks an argument"));
                    }
                    index += 1;
                    let mut map = Map::new();
                    map.insert("command".into(), Value::String(name.into()));
                    map.insert("argument".into(), Value::Number(Number::from(tokens[index])));
                    atoms.push(Value::Object(map));
                }
            }
        }
        index += 1;
    }
    if !text.is_empty() {
        atoms.push(Value::String(text));
    }
    if atoms.is_empty() {
        return Ok(Value::String(String::new()));
    }
    if atoms.len() == 1 && atoms[0].is_string() {
        return Ok(atoms.into_iter().next().expect("one atom"));
    }
    Ok(Value::Array(atoms))
}

fn derived_contexts(banks: &[Value]) -> Res<Vec<ContextSource>> {
    // Insertion-ordered symbol tables, one per context.
    struct Transition {
        symbol: u32,
        count: u64,
        order: usize,
    }
    let mut transitions: Vec<(Vec<Transition>, HashMap<u32, usize>)> =
        (0..CONTEXT_COUNT).map(|_| (Vec::new(), HashMap::new())).collect();

    for bank in banks {
        let messages = bank.as_array().expect("bank is an array");
        for message in messages {
            let Some(tokens) = tokens_from_message(message)? else { continue };
            let mut previous: u32 = 0;
            for symbol in tokens.iter().copied().chain(std::iter::once(0u32)) {
                let Some(context) = transitions.get_mut(previous as usize) else {
                    return Err(format!("message enters unsupported context {previous}"));
                };
                match context.1.get(&symbol) {
                    Some(&slot) => context.0[slot].count += 1,
                    None => {
                        let order = context.0.len();
                        context.1.insert(symbol, order);
                        context.0.push(Transition { symbol, count: 1, order });
                    }
                }
                previous = symbol;
            }
        }
    }

    struct Item {
        count: u64,
        order: usize,
        node: TreeNode,
    }
    let mut result: Vec<ContextSource> = Vec::new();
    for (id, (symbols, _)) in transitions.iter().enumerate() {
        if symbols.is_empty() {
            continue;
        }
        let mut order = symbols.len();
        let mut nodes: Vec<Item> = symbols
            .iter()
            .map(|item| Item {
                count: item.count,
                order: item.order,
                node: TreeNode::Leaf(item.symbol),
            })
            .collect();
        while nodes.len() > 1 {
            // The comparator is a total order (orders are unique), so the sort's
            // stability does not matter.
            nodes.sort_by(|left, right| {
                left.count.cmp(&right.count).then(left.order.cmp(&right.order))
            });
            let left = nodes.remove(0);
            let right = nodes.remove(0);
            nodes.push(Item {
                count: left.count + right.count,
                order,
                node: TreeNode::Branch(Box::new(left.node), Box::new(right.node)),
            });
            order += 1;
        }
        let mut tree = String::new();
        let mut leaves: Vec<u32> = Vec::new();
        fn walk(node: &TreeNode, tree: &mut String, leaves: &mut Vec<u32>) {
            match node {
                TreeNode::Leaf(symbol) => {
                    tree.push('1');
                    leaves.push(*symbol);
                }
                TreeNode::Branch(left, right) => {
                    tree.push('0');
                    walk(left, tree, leaves);
                    walk(right, tree, leaves);
                }
            }
        }
        walk(&nodes[0].node, &mut tree, &mut leaves);
        result.push(ContextSource { id, tree, leaves });
    }
    Ok(result)
}

fn encode_message(tokens: &[u32], contexts: &HashMap<usize, CompiledContext>) -> Res<Vec<u8>> {
    let mut context: u32 = 0;
    let mut bits = String::new();
    for symbol in tokens.iter().copied().chain(std::iter::once(0u32)) {
        let Some(codec) = contexts.get(&(context as usize)) else {
            return Err(format!("message enters absent context {context}"));
        };
        let Some(path) = codec.paths.get(&symbol) else {
            return Err(format!("context {context} cannot encode symbol {symbol}"));
        };
        bits.push_str(path);
        context = symbol;
    }
    bit_buffer(&bits)
}

fn encoded_length(size: usize) -> Res<Vec<u8>> {
    bounded_raw(size as i64, 0, 0xffff, "encoded message size")?;
    let mut size = size;
    let mut result = Vec::new();
    while size >= 0xff {
        result.push(0xff);
        size -= 0xff;
    }
    result.push(size as u8);
    Ok(result)
}

pub fn build_message_archive(value: &Value) -> Res<Vec<u8>> {
    let banks_source = parse_document(value)?;
    let (model_data, contexts) = build_model(&derived_contexts(banks_source)?)?;
    if model_data.len() != (MESSAGE_ADDRESS - ARCHIVE_ADDRESS) as usize {
        return Err("context model extent differs".into());
    }
    let mut banks: Vec<u8> = Vec::new();
    let mut descriptors = vec![0u8; BANK_COUNT * 8];
    let mut address = MESSAGE_ADDRESS;
    for (bank, messages_value) in banks_source.iter().enumerate() {
        let messages = messages_value.as_array().expect("bank is an array");
        let mut payload: Vec<u8> = Vec::new();
        let mut lengths: Vec<u8> = Vec::new();
        for (index, message) in messages.iter().enumerate() {
            let tokens = tokens_from_message(message)?;
            let encoded = match &tokens {
                None => Vec::new(),
                Some(tokens) => encode_message(tokens, &contexts)?,
            };
            let allowed = bank + 1 == BANK_COUNT
                && index + 1 == messages.len()
                && message.is_null();
            if encoded.is_empty() && !allowed {
                return Err(format!(
                    "message {} is unexpectedly null",
                    hex(((bank as u32) << 8) | index as u32, 4)
                ));
            }
            lengths.extend_from_slice(&encoded_length(encoded.len())?);
            payload.extend_from_slice(&encoded);
        }
        descriptors[bank * 8..bank * 8 + 4].copy_from_slice(&address.to_le_bytes());
        let after = address + payload.len() as u32;
        descriptors[bank * 8 + 4..bank * 8 + 8].copy_from_slice(&after.to_le_bytes());
        address = after + lengths.len() as u32;
        banks.extend_from_slice(&payload);
        banks.extend_from_slice(&lengths);
    }
    if address != MESSAGE_DIRECTORY_ADDRESS {
        return Err("message banks do not reach their directory".into());
    }
    let mut result = model_data;
    result.extend_from_slice(&banks);
    result.extend_from_slice(&descriptors);
    if result.len() != (ARCHIVE_END - ARCHIVE_ADDRESS) as usize {
        return Err("message archive size differs".into());
    }
    Ok(result)
}

// ---------------------------------------------------------------------------
// export
// ---------------------------------------------------------------------------

struct ReadTree {
    tree: String,
    leaves: Vec<u32>,
    end: usize,
}

fn read_tree(source: &[u8], boundary: usize) -> Res<ReadTree> {
    let mut bits = BitReader::whole(source, boundary);
    let mut tree = String::new();
    let mut leaf_count = 0usize;
    fn parse(
        bits: &mut BitReader,
        tree: &mut String,
        leaf_count: &mut usize,
    ) -> Res<()> {
        let bit = bits.read()?;
        tree.push(if bit == 0 { '0' } else { '1' });
        if bit != 0 {
            *leaf_count += 1;
            return Ok(());
        }
        parse(bits, tree, leaf_count)?;
        parse(bits, tree, leaf_count)
    }
    parse(&mut bits, &mut tree, &mut leaf_count)?;
    let mut leaves = Vec::with_capacity(leaf_count);
    for rank in 0..leaf_count {
        leaves.push(symbol_at(source, boundary, rank)?);
    }
    let end = bits.position.div_ceil(8);
    for position in bits.position..end * 8 {
        if bit_at(source, position) != 0 {
            return Err("context tree padding is nonzero".into());
        }
    }
    Ok(ReadTree { tree, leaves, end })
}

fn export_model(source: &[u8]) -> Res<Vec<ContextSource>> {
    let offsets = rom_offset(OFFSET_TABLE_ADDRESS, source, false)?;
    let descriptor = rom_offset(MODEL_DIRECTORY_ADDRESS, source, false)?;
    if pointer(source, descriptor)? != ARCHIVE_ADDRESS
        || pointer(source, descriptor + 4)? != OFFSET_TABLE_ADDRESS
    {
        return Err("context model directory differs".into());
    }
    let mut contexts: Vec<ContextSource> = Vec::new();
    let mut cursor = rom_offset(ARCHIVE_ADDRESS, source, false)?;
    for id in 0..CONTEXT_COUNT {
        let offset = halfword(source, offsets + id * 2)?;
        if offset == 0x8000 {
            continue;
        }
        let boundary = rom_offset(ARCHIVE_ADDRESS + offset as u32, source, false)?;
        let decoded = read_tree(source, boundary)?;
        let leaf_bytes = (decoded.leaves.len() * 12).div_ceil(8);
        if boundary as i64 - leaf_bytes as i64 != cursor as i64 {
            return Err(format!("context {id} record is not contiguous"));
        }
        let packed = packed_leaves(&decoded.leaves)?;
        if packed != source[cursor..boundary] {
            return Err(format!("context {id} alphabet differs"));
        }
        contexts.push(ContextSource { id, tree: decoded.tree, leaves: decoded.leaves });
        cursor = decoded.end;
    }
    if halfword(source, offsets + CONTEXT_COUNT * 2)? != 0 || cursor != offsets {
        return Err("context model terminal record differs".into());
    }
    Ok(contexts)
}

fn decode_message(
    source: &[u8],
    byte: usize,
    size: usize,
    contexts: &HashMap<usize, CompiledContext>,
) -> Res<Vec<u32>> {
    let mut bits = BitReader::new(source, byte, (byte + size) * 8);
    let mut tokens: Vec<u32> = Vec::new();
    let mut context: u32 = 0;
    for _ in 0..0x10000 {
        let Some(codec) = contexts.get(&(context as usize)) else {
            return Err(format!("message enters absent context {context}"));
        };
        let mut node = &codec.root;
        while let TreeNode::Branch(left, right) = node {
            node = if bits.read()? == 0 { left } else { right };
        }
        let TreeNode::Leaf(symbol) = node else { unreachable!() };
        if *symbol == 0 {
            while bits.position < bits.limit {
                if bits.read()? != 0 {
                    return Err("message padding is nonzero".into());
                }
            }
            return Ok(tokens);
        }
        tokens.push(*symbol);
        context = *symbol;
    }
    Err("message does not terminate".into())
}

pub fn export_message_archive(source: &[u8]) -> Res<Value> {
    let model = export_model(source)?;
    let contexts = compile_contexts(&model)?;
    let directory = rom_offset(MESSAGE_DIRECTORY_ADDRESS, source, false)?;
    let mut banks: Vec<Value> = Vec::new();
    for bank in 0..BANK_COUNT {
        let payload = rom_offset(pointer(source, directory + bank * 8)?, source, false)?;
        let lengths = rom_offset(pointer(source, directory + bank * 8 + 4)?, source, false)?;
        let count = if bank + 1 == BANK_COUNT { LAST_BANK_SIZE } else { BANK_SIZE };
        let mut messages: Vec<Value> = Vec::new();
        let mut byte = payload;
        let mut length = lengths;
        for _ in 0..count {
            let mut size: usize = 0;
            loop {
                if length >= directory {
                    return Err(format!("message bank {bank} length table is truncated"));
                }
                let part = source[length];
                length += 1;
                size += part as usize;
                if part != 0xff {
                    break;
                }
            }
            if size == 0 {
                messages.push(Value::Null);
            } else {
                messages.push(message_from_tokens(&decode_message(
                    source, byte, size, &contexts,
                )?)?);
            }
            byte += size;
        }
        let next = if bank + 1 == BANK_COUNT {
            directory
        } else {
            rom_offset(pointer(source, directory + (bank + 1) * 8)?, source, false)?
        };
        if byte != lengths || length != next {
            return Err(format!("message bank {bank} extent differs"));
        }
        banks.push(Value::Array(messages));
    }

    let mut skeleton = Map::new();
    skeleton.insert("format".into(), Value::Number(Number::from(1u32)));
    skeleton.insert("kind".into(), Value::String("golden-sun-message-archive".into()));
    skeleton.insert("address".into(), Value::String(hex8(ARCHIVE_ADDRESS)));
    skeleton.insert("size".into(), Value::String(hex8(ARCHIVE_END - ARCHIVE_ADDRESS)));
    skeleton.insert("bank_size".into(), Value::Number(Number::from(BANK_SIZE as u64)));
    skeleton.insert("banks".into(), Value::Array(banks));
    let skeleton = Value::Object(skeleton);

    let built = build_message_archive(&skeleton)?;
    let start = rom_offset(ARCHIVE_ADDRESS, source, false)?;
    if built != source[start..start + built.len()] {
        return Err("exported message archive does not round-trip".into());
    }
    Ok(skeleton)
}

pub fn format_message_archive(source: &Value) -> String {
    let get = |key: &str| source.get(key).cloned().unwrap_or(Value::Null);
    let mut lines: Vec<String> = vec![
        "{".into(),
        "  \"format\": 1,".into(),
        "  \"kind\": \"golden-sun-message-archive\",".into(),
        format!("  \"address\": {},", serde_json::to_string(&get("address")).expect("json")),
        format!("  \"size\": {},", serde_json::to_string(&get("size")).expect("json")),
        format!(
            "  \"bank_size\": {},",
            serde_json::to_string(&get("bank_size")).expect("json")
        ),
        "  \"banks\": [".into(),
    ];
    let banks = source.get("banks").and_then(Value::as_array).cloned().unwrap_or_default();
    for (bank_index, bank) in banks.iter().enumerate() {
        lines.push("    [".into());
        let messages = bank.as_array().cloned().unwrap_or_default();
        for (message_index, message) in messages.iter().enumerate() {
            let comma = if message_index + 1 == messages.len() { "" } else { "," };
            lines.push(format!(
                "      {}{comma}",
                serde_json::to_string(message).expect("json")
            ));
        }
        let comma = if bank_index + 1 == banks.len() { "" } else { "," };
        lines.push(format!("    ]{comma}"));
    }
    lines.push("  ]".into());
    lines.push("}".into());
    format!("{}\n", lines.join("\n"))
}

// ---------------------------------------------------------------------------
// CLI
// ---------------------------------------------------------------------------

fn option(args: &[String], name: &str) -> Option<String> {
    let index = args.iter().position(|item| item == name)?;
    args.get(index + 1).cloned()
}

fn read_json(path: &str) -> Res<Value> {
    let text = std::fs::read(path).map_err(|error| format!("{path}: {error}"))?;
    serde_json::from_slice(&text).map_err(|error| format!("{path}: {error}"))
}

fn run(args: &[String]) -> Res<()> {
    if args.first().map(String::as_str) == Some("--self-test") {
        self_test()?;
        println!("self-test=ok");
        return Ok(());
    }
    let command = args.first().map(String::as_str);
    let input = args.get(1).cloned();
    match command {
        Some("export") => {
            let output = option(args, "--output").or_else(|| option(args, "-o"));
            let (Some(input), Some(output)) = (input, output) else {
                return Err("export requires ROM and --output".into());
            };
            let rom = std::fs::read(&input).map_err(|error| format!("{input}: {error}"))?;
            let source = export_message_archive(&rom)?;
            std::fs::write(&output, format_message_archive(&source))
                .map_err(|error| format!("{output}: {error}"))?;
            let banks = source.get("banks").and_then(Value::as_array).expect("banks");
            let messages: usize =
                banks.iter().map(|bank| bank.as_array().expect("bank").len()).sum();
            println!(
                "identical=true contexts={} messages={} bytes={}",
                derived_contexts(banks)?.len(),
                messages,
                ARCHIVE_END - ARCHIVE_ADDRESS
            );
            Ok(())
        }
        Some("build") => {
            let output = option(args, "--output").or_else(|| option(args, "-o"));
            let (Some(input), Some(output)) = (input, output) else {
                return Err("build requires SOURCE and --output".into());
            };
            let built = build_message_archive(&read_json(&input)?)?;
            std::fs::write(&output, &built).map_err(|error| format!("{output}: {error}"))?;
            println!("bytes={}", built.len());
            Ok(())
        }
        Some("verify") => {
            let source_path = args.get(2).cloned();
            let (Some(input), Some(source_path)) = (input, source_path) else {
                return Err("verify requires ROM and SOURCE".into());
            };
            let rom = std::fs::read(&input).map_err(|error| format!("{input}: {error}"))?;
            let built = build_message_archive(&read_json(&source_path)?)?;
            let start = (ARCHIVE_ADDRESS - ROM_BASE) as usize;
            if start + built.len() > rom.len() || built != rom[start..start + built.len()] {
                return Err("message archive differs from ROM".into());
            }
            println!("identical=true bytes={}", built.len());
            Ok(())
        }
        _ => Err("usage: message_archive.ts {export ROM --output SOURCE|build SOURCE --output FILE|verify ROM SOURCE|--self-test}".into()),
    }
}

fn self_test() -> Res<()> {
    let leaves = [0x123u32, 0x456, 0x789];
    let packed = packed_leaves(&leaves)?;
    for (rank, symbol) in leaves.iter().enumerate() {
        if symbol_at(&packed, packed.len(), rank)? != *symbol {
            return Err("context alphabet self-test failed".into());
        }
    }
    let compiled = compile_context(&ContextSource {
        id: 0,
        tree: "00111".into(),
        leaves: vec![65, 66, 67],
    })?;
    if compiled.paths.get(&65).map(String::as_str) != Some("00")
        || compiled.paths.get(&66).map(String::as_str) != Some("01")
        || compiled.paths.get(&67).map(String::as_str) != Some("1")
    {
        return Err("context tree self-test failed".into());
    }
    let bits = "101100010111";
    let packed_bits = bit_buffer(bits)?;
    let mut reader = BitReader::new(&packed_bits, 0, bits.len());
    for bit in bits.bytes() {
        if reader.read()? != bit - b'0' {
            return Err("bit stream self-test failed".into());
        }
    }
    if encoded_length(0)? != [0]
        || encoded_length(255)? != [255, 0]
        || encoded_length(510)? != [255, 255, 0]
    {
        return Err("message length self-test failed".into());
    }
    let message = message_from_tokens(&[65, 3, 8, 4, 66])?;
    if tokens_from_message(&message)? != Some(vec![65, 3, 8, 4, 66]) {
        return Err("message command self-test failed".into());
    }
    Ok(())
}

fn main() {
    let args: Vec<String> = std::env::args().skip(1).collect();
    if let Err(error) = run(&args) {
        eprintln!("error: {error}");
        std::process::exit(1);
    }
}

/// Repository root, for tests that read the real tree.
#[allow(dead_code)]
fn repository_root() -> &'static Path {
    Path::new(env!("CARGO_MANIFEST_DIR")).parent().unwrap().parent().unwrap()
}

#[cfg(test)]
mod tests {
    use super::*;

    // --- ported self-test ---------------------------------------------------

    #[test]
    fn self_test_passes() {
        self_test().expect("self test");
    }

    #[test]
    fn alphabet_round_trips() {
        let leaves = [0x123u32, 0x456, 0x789];
        let packed = packed_leaves(&leaves).unwrap();
        assert_eq!(packed.len(), 5);
        for (rank, symbol) in leaves.iter().enumerate() {
            assert_eq!(symbol_at(&packed, packed.len(), rank).unwrap(), *symbol);
        }
    }

    #[test]
    fn alphabet_round_trips_even_count() {
        let leaves = [0u32, 0xfff, 0x001, 0x800];
        let packed = packed_leaves(&leaves).unwrap();
        assert_eq!(packed.len(), 6);
        for (rank, symbol) in leaves.iter().enumerate() {
            assert_eq!(symbol_at(&packed, packed.len(), rank).unwrap(), *symbol);
        }
    }

    #[test]
    fn alphabet_rejects_empty_and_out_of_range() {
        assert_eq!(
            packed_leaves(&[]).unwrap_err(),
            "context leaves must be a nonempty array"
        );
        assert_eq!(
            packed_leaves(&[0x1000]).unwrap_err(),
            "context symbol is outside its range"
        );
    }

    #[test]
    fn tree_paths() {
        let compiled = compile_context(&ContextSource {
            id: 0,
            tree: "00111".into(),
            leaves: vec![65, 66, 67],
        })
        .unwrap();
        assert_eq!(compiled.paths[&65], "00");
        assert_eq!(compiled.paths[&66], "01");
        assert_eq!(compiled.paths[&67], "1");
    }

    #[test]
    fn tree_errors() {
        let bad = |tree: &str, leaves: Vec<u32>| {
            compile_context(&ContextSource { id: 7, tree: tree.into(), leaves }).unwrap_err()
        };
        assert_eq!(bad("", vec![1]), "context 7 has an invalid tree");
        assert_eq!(bad("0x1", vec![1]), "context 7 has an invalid tree");
        assert_eq!(bad("1", vec![]), "context 7 has no leaves");
        assert_eq!(bad("00", vec![1, 2]), "context 7 tree is truncated");
        assert_eq!(bad("011", vec![1]), "context 7 has too few leaves");
        assert_eq!(bad("011", vec![5, 5]), "context 7 repeats symbol 5");
        assert_eq!(bad("1", vec![1, 2]), "context 7 tree and leaves differ");
        assert_eq!(bad("11", vec![1, 2]), "context 7 tree and leaves differ");
    }

    #[test]
    fn bit_stream() {
        let bits = "101100010111";
        let packed = bit_buffer(bits).unwrap();
        let mut reader = BitReader::new(&packed, 0, bits.len());
        for bit in bits.bytes() {
            assert_eq!(reader.read().unwrap(), bit - b'0');
        }
        assert_eq!(reader.read().unwrap_err(), "bit stream exceeds its extent");
    }

    #[test]
    fn bit_buffer_rejects_non_binary() {
        assert_eq!(bit_buffer("012").unwrap_err(), "bit source is not binary");
        assert!(bit_buffer("").unwrap().is_empty());
    }

    #[test]
    fn lengths() {
        assert_eq!(encoded_length(0).unwrap(), vec![0]);
        assert_eq!(encoded_length(255).unwrap(), vec![255, 0]);
        assert_eq!(encoded_length(510).unwrap(), vec![255, 255, 0]);
        assert_eq!(encoded_length(254).unwrap(), vec![254]);
        assert_eq!(encoded_length(256).unwrap(), vec![255, 1]);
        assert_eq!(
            encoded_length(0x10000).unwrap_err(),
            "encoded message size is outside its range"
        );
    }

    #[test]
    fn message_round_trip() {
        let message = message_from_tokens(&[65, 3, 8, 4, 66]).unwrap();
        assert_eq!(
            serde_json::to_string(&message).unwrap(),
            r#"["A",{"command":"line_break"},{"command":"text_color","argument":4},"B"]"#
        );
        assert_eq!(tokens_from_message(&message).unwrap(), Some(vec![65, 3, 8, 4, 66]));
    }

    #[test]
    fn plain_text_message_collapses_to_a_string() {
        let message = message_from_tokens(&[72, 105]).unwrap();
        assert_eq!(message, Value::String("Hi".into()));
        assert_eq!(tokens_from_message(&message).unwrap(), Some(vec![72, 105]));
    }

    #[test]
    fn empty_token_list_is_the_empty_string() {
        assert_eq!(message_from_tokens(&[]).unwrap(), Value::String(String::new()));
        assert_eq!(
            tokens_from_message(&Value::String(String::new())).unwrap(),
            Some(Vec::new())
        );
    }

    #[test]
    fn unnamed_control_codes_survive() {
        // 10 through 15 and 28 have no command name.
        for opcode in [10u32, 11, 12, 13, 14, 15, 28, 31] {
            let message = message_from_tokens(&[opcode]).unwrap();
            assert_eq!(
                serde_json::to_string(&message).unwrap(),
                format!(r#"[{{"control":{opcode}}}]"#)
            );
            assert_eq!(tokens_from_message(&message).unwrap(), Some(vec![opcode]));
        }
    }

    #[test]
    fn message_from_tokens_rejects_out_of_range_symbols() {
        assert_eq!(
            message_from_tokens(&[0]).unwrap_err(),
            "message symbol 0 has no source representation"
        );
        assert_eq!(
            message_from_tokens(&[123]).unwrap_err(),
            "message symbol 123 has no source representation"
        );
        assert_eq!(
            message_from_tokens(&[8]).unwrap_err(),
            "message command text_color lacks an argument"
        );
    }

    #[test]
    fn null_message_yields_no_tokens() {
        assert_eq!(tokens_from_message(&Value::Null).unwrap(), None);
    }

    // --- edge cases the TypeScript self-test missed --------------------------

    #[test]
    fn tokens_reject_bad_atoms() {
        let bad = |json: &str| {
            tokens_from_message(&serde_json::from_str(json).unwrap()).unwrap_err()
        };
        assert_eq!(bad("5"), "message must be text, atoms, or null");
        assert_eq!(bad("true"), "message must be text, atoms, or null");
        assert_eq!(bad("{}"), "message must be text, atoms, or null");
        assert_eq!(bad("[5]"), "message command atom is invalid");
        assert_eq!(bad("[[1]]"), "message command atom is invalid");
        assert_eq!(bad(r#"[{"control":3,"extra":1}]"#), "message command atom is invalid");
        assert_eq!(bad(r#"[{"nope":1}]"#), "message command atom is invalid");
        assert_eq!(bad(r#"[{"command":7}]"#), "message command atom is invalid");
        assert_eq!(bad(r#"[{"command":"nope"}]"#), "unknown message command nope");
        assert_eq!(
            bad(r#"[{"command":"line_break","argument":1}]"#),
            "message command line_break takes no argument"
        );
        assert_eq!(
            bad(r#"[{"command":"text_color"}]"#),
            "message command text_color requires an argument"
        );
        assert_eq!(
            bad(r#"[{"command":"text_color","other":1}]"#),
            "message command text_color requires an argument"
        );
        assert_eq!(
            bad(r#"[{"command":"text_color","argument":123}]"#),
            "message command argument is outside its range"
        );
        assert_eq!(bad(r#"[{"control":0}]"#), "message control is outside its range");
        assert_eq!(bad(r#"[{"control":32}]"#), "message control is outside its range");
        assert_eq!(bad(r#"[{"control":1.5}]"#), "message control must be an integer");
        assert_eq!(bad(r#"["\u00e9"]"#), "message text is outside the recovered ASCII range");
        assert_eq!(bad(r#"["\u0007"]"#), "message text is outside the recovered ASCII range");
        assert_eq!(bad(r#"["{"]"#), "message text is outside the recovered ASCII range");
        assert_eq!(bad(r#"["\ud83d\ude00"]"#), "message text is outside the recovered ASCII range");
    }

    /// `Number.isSafeInteger` accepts integral floats, and `integer()` runs
    /// `Number()` over strings first.
    #[test]
    fn integer_coercion_matches_javascript() {
        let ok = |json: &str| {
            tokens_from_message(&serde_json::from_str(json).unwrap()).unwrap().unwrap()
        };
        assert_eq!(ok(r#"[{"control":3.0}]"#), vec![3]);
        assert_eq!(ok(r#"[{"control":"3"}]"#), vec![3]);
        assert_eq!(ok(r#"[{"control":" 3 "}]"#), vec![3]);
        assert_eq!(ok(r#"[{"control":"0x1f"}]"#), vec![31]);
        assert_eq!(ok(r#"[{"control":"3e0"}]"#), vec![3]);
        assert_eq!(integer(Some(&Value::String("".into())), "x").unwrap(), 0);
        assert_eq!(integer(Some(&Value::String("nope".into())), "x").unwrap_err(), "x must be an integer");
        assert_eq!(integer(Some(&Value::String("Infinity".into())), "x").unwrap_err(), "x must be an integer");
        assert_eq!(integer(Some(&Value::String("NaN".into())), "x").unwrap_err(), "x must be an integer");
        assert_eq!(integer(Some(&Value::Bool(true)), "x").unwrap_err(), "x must be an integer");
        assert_eq!(integer(None, "x").unwrap_err(), "x must be an integer");
        assert_eq!(
            integer(Some(&serde_json::json!(9007199254740992u64)), "x").unwrap_err(),
            "x must be an integer"
        );
    }

    #[test]
    fn document_validation() {
        let base = serde_json::json!({
            "format": 1,
            "kind": "golden-sun-message-archive",
            "address": "0x08037464",
            "size": "0x0003c3a4",
            "bank_size": 256,
            "banks": [],
        });
        assert_eq!(
            parse_document(&Value::Null).unwrap_err(),
            "message archive source must be an object"
        );
        let mut extra = base.clone();
        extra["extra"] = Value::Bool(true);
        assert_eq!(
            parse_document(&extra).unwrap_err(),
            "message archive source has unknown fields"
        );
        let mut wrong = base.clone();
        wrong["format"] = Value::String("1".into());
        assert_eq!(
            parse_document(&wrong).unwrap_err(),
            "unsupported message archive source"
        );
        let mut wrong = base.clone();
        wrong["size"] = Value::String("0x3c3a4".into());
        assert_eq!(parse_document(&wrong).unwrap_err(), "message archive layout differs");
        assert_eq!(
            parse_document(&base).unwrap_err(),
            "message archive collections differ"
        );
        let mut sized = base.clone();
        sized["banks"] = Value::Array(vec![Value::Array(Vec::new()); BANK_COUNT]);
        assert_eq!(parse_document(&sized).unwrap_err(), "message bank 0 has the wrong size");
        // `1.0` is `1` in JSON's number model, and must be accepted.
        let mut floaty = base.clone();
        floaty["format"] = serde_json::json!(1.0);
        floaty["banks"] = Value::Array(vec![Value::Array(Vec::new()); BANK_COUNT]);
        assert_eq!(
            parse_document(&floaty).unwrap_err(),
            "message bank 0 has the wrong size"
        );
    }

    #[test]
    fn hex_formats_like_javascript() {
        assert_eq!(hex8(ARCHIVE_ADDRESS), "0x08037464");
        assert_eq!(hex8(ARCHIVE_END - ARCHIVE_ADDRESS), "0x0003c3a4");
        assert_eq!(hex(0x123, 4), "0x0123");
        assert_eq!(hex(0, 4), "0x0000");
    }

    #[test]
    fn rom_offset_bounds() {
        let rom = vec![0u8; 16];
        assert_eq!(rom_offset(ROM_BASE, &rom, false).unwrap(), 0);
        assert_eq!(rom_offset(ROM_BASE + 15, &rom, false).unwrap(), 15);
        assert_eq!(
            rom_offset(ROM_BASE + 16, &rom, false).unwrap_err(),
            "0x08000010 is outside the ROM"
        );
        assert_eq!(rom_offset(ROM_BASE + 16, &rom, true).unwrap(), 16);
        assert_eq!(
            rom_offset(ROM_BASE - 1, &rom, false).unwrap_err(),
            "0x07ffffff is outside the ROM"
        );
    }

    // --- whole-archive round trip against the real ROM ----------------------

    fn rom() -> Option<Vec<u8>> {
        std::fs::read(repository_root().join("baserom.gba")).ok()
    }

    #[test]
    fn real_rom_round_trips() {
        let Some(rom) = rom() else { return };
        let source = export_message_archive(&rom).expect("export");
        let built = build_message_archive(&source).expect("build");
        assert_eq!(built.len(), (ARCHIVE_END - ARCHIVE_ADDRESS) as usize);
        let start = (ARCHIVE_ADDRESS - ROM_BASE) as usize;
        assert_eq!(built, rom[start..start + built.len()]);
    }

    #[test]
    fn real_source_matches_the_rom() {
        let Some(rom) = rom() else { return };
        let path = repository_root().join("assets/text/message_archive.json");
        let Ok(text) = std::fs::read(&path) else { return };
        let value: Value = serde_json::from_slice(&text).expect("json");
        let built = build_message_archive(&value).expect("build");
        let start = (ARCHIVE_ADDRESS - ROM_BASE) as usize;
        assert_eq!(built, rom[start..start + built.len()]);
        // And the exporter reproduces that very file, byte for byte.
        let source = export_message_archive(&rom).expect("export");
        assert_eq!(format_message_archive(&source).into_bytes(), text);
    }

    #[test]
    fn derived_contexts_are_stable_for_the_real_source() {
        let Some(rom) = rom() else { return };
        let source = export_message_archive(&rom).expect("export");
        let banks = source.get("banks").and_then(Value::as_array).unwrap();
        let contexts = derived_contexts(banks).expect("contexts");
        assert_eq!(contexts.len(), 107);
        let exported = export_model(&rom).expect("model");
        assert_eq!(contexts, exported);
    }
}
