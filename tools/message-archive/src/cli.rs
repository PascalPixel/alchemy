use serde_json::{Map, Value};
use std::collections::HashMap;
use std::io::Write;
pub const ARCHIVE_ADDRESS: u32 = 0x0803_7464;
pub const OFFSET_TABLE_ADDRESS: u32 = 0x0803_8334;
pub const MESSAGE_ADDRESS: u32 = 0x0803_8434;
pub const MESSAGE_DIRECTORY_ADDRESS: u32 = 0x0807_36b8;
pub const ARCHIVE_END: u32 = 0x0807_3808;
pub const CONTEXT_COUNT: usize = 123;
pub const OFFSET_COUNT: usize = 124;
pub const BANK_COUNT: usize = 42;
pub const BANK_SIZE: usize = 256;
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
fn command_by_name(name: &str) -> Option<(u32, usize)> {
    COMMANDS
        .iter()
        .find(|(_, candidate, _)| *candidate == name)
        .map(|(opcode, _, arguments)| (*opcode, *arguments))
}
fn integer(value: Option<&Value>, label: &str) -> Res<i64> {
    value
        .and_then(Value::as_i64)
        .ok_or_else(|| format!("{label} must be an integer"))
}
fn bounded(value: Option<&Value>, minimum: i64, maximum: i64, label: &str) -> Res<i64> {
    let parsed = integer(value, label)?;
    if parsed < minimum || parsed > maximum {
        return Err(format!("{label} is outside its range"));
    }
    Ok(parsed)
}
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
    let _root = parse(&mut state, String::new())?;
    if state.position != bits.len() || state.leaf != source.leaves.len() {
        return Err(format!("context {} tree and leaves differ", source.id));
    }
    Ok(CompiledContext { paths: state.paths })
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
fn sorted_keys(map: &Map<String, Value>) -> String {
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
        let expected = if index + 1 == BANK_COUNT {
            LAST_BANK_SIZE
        } else {
            BANK_SIZE
        };
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
        let is_control = object
            .map(|map| map.len() == 1 && map.contains_key("control"))
            .unwrap_or(false);
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
                tokens
                    .push(bounded(map.get("argument"), 0, 122, "message command argument")? as u32);
            }
            continue;
        }
        let map = object.expect("control atom is an object");
        tokens.push(bounded(map.get("control"), 1, 31, "message control")? as u32);
    }
    Ok(Some(tokens))
}
fn derived_contexts(banks: &[Value]) -> Res<Vec<ContextSource>> {
    struct Transition {
        symbol: u32,
        count: u64,
        order: usize,
    }
    let mut transitions: Vec<(Vec<Transition>, HashMap<u32, usize>)> = (0..CONTEXT_COUNT)
        .map(|_| (Vec::new(), HashMap::new()))
        .collect();
    for bank in banks {
        let messages = bank.as_array().expect("bank is an array");
        for message in messages {
            let Some(tokens) = tokens_from_message(message)? else {
                continue;
            };
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
                        context.0.push(Transition {
                            symbol,
                            count: 1,
                            order,
                        });
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
            nodes.sort_by(|left, right| {
                left.count
                    .cmp(&right.count)
                    .then(left.order.cmp(&right.order))
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
            let allowed =
                bank + 1 == BANK_COUNT && index + 1 == messages.len() && message.is_null();
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
const USAGE: &str = "usage: message-archive build-stdout SOURCE";
pub fn entry(arguments: &[String]) {
    let result = match arguments {
        [command, source] if command == "build-stdout" => (|| {
            let bytes = std::fs::read(source).map_err(|error| format!("{source}: {error}"))?;
            let value: Value = serde_json::from_slice(&bytes).map_err(|error| error.to_string())?;
            std::io::stdout()
                .write_all(&build_message_archive(&value)?)
                .map_err(|error| error.to_string())
        })(),
        [argument] if matches!(argument.as_str(), "-h" | "--help") => {
            println!("{USAGE}");
            Ok(())
        }
        _ => Err(USAGE.into()),
    };
    if let Err(error) = result {
        eprintln!("error: {error}");
        std::process::exit(1);
    }
}
