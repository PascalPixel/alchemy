use std::collections::HashMap;

pub(crate) struct HuffmanArchive {
    pub bytes: Vec<u8>,
    pub context_directory: u32,
    pub directory: u32,
    pub pointers: Vec<usize>,
}

enum Node {
    Leaf(u16),
    Branch(Box<Node>, Box<Node>),
}

struct Context {
    tree: Vec<bool>,
    leaves: Vec<u16>,
    paths: HashMap<u16, Vec<bool>>,
}

fn pack_bits(bits: &[bool]) -> Vec<u8> {
    let mut bytes = vec![0u8; bits.len().div_ceil(8)];
    for (index, _) in bits.iter().enumerate().filter(|(_, bit)| **bit) {
        bytes[index >> 3] |= 1 << (index & 7);
    }
    bytes
}

fn pack_leaves(leaves: &[u16]) -> Vec<u8> {
    let mut groups: Vec<Vec<u8>> = leaves
        .chunks(2)
        .map(|pair| match pair {
            [first, second] => vec![
                (second & 0xff) as u8,
                (((first & 0x0f) << 4) | (second >> 8)) as u8,
                (first >> 4) as u8,
            ],
            [first] => vec![((first & 0x0f) << 4) as u8, (first >> 4) as u8],
            _ => unreachable!(),
        })
        .collect();
    groups.reverse();
    groups.concat()
}

fn derive_context(transitions: &[(u16, u64)]) -> Context {
    let mut order = transitions.len();
    let mut nodes: Vec<(u64, usize, Node)> = transitions
        .iter()
        .enumerate()
        .map(|(index, (symbol, count))| (*count, index, Node::Leaf(*symbol)))
        .collect();
    while nodes.len() > 1 {
        nodes.sort_by_key(|(count, order, _)| (*count, *order));
        let left = nodes.remove(0);
        let right = nodes.remove(0);
        let node = Node::Branch(Box::new(left.2), Box::new(right.2));
        nodes.push((left.0 + right.0, order, node));
        order += 1;
    }
    fn walk(node: &Node, path: Vec<bool>, context: &mut Context) {
        match node {
            Node::Leaf(symbol) => {
                context.tree.push(true);
                context.leaves.push(*symbol);
                context.paths.insert(*symbol, path);
            }
            Node::Branch(left, right) => {
                context.tree.push(false);
                walk(left, [path.as_slice(), &[false]].concat(), context);
                walk(right, [path.as_slice(), &[true]].concat(), context);
            }
        }
    }
    let mut context = Context {
        tree: Vec::new(),
        leaves: Vec::new(),
        paths: HashMap::new(),
    };
    walk(&nodes[0].2, Vec::new(), &mut context);
    context
}

fn derive_contexts(
    symbol_count: usize,
    banks: &[Vec<Option<Vec<u16>>>],
) -> Result<Vec<Option<Context>>, String> {
    let mut transitions: Vec<Vec<(u16, u64)>> = vec![Vec::new(); symbol_count];
    for symbols in banks.iter().flatten().flatten() {
        let mut previous = 0u16;
        for symbol in symbols.iter().copied().chain(std::iter::once(0)) {
            let Some(context) = transitions.get_mut(usize::from(previous)) else {
                return Err("message symbol is outside the alphabet".into());
            };
            match context.iter_mut().find(|(seen, _)| *seen == symbol) {
                Some(entry) => entry.1 += 1,
                None => context.push((symbol, 1)),
            }
            previous = symbol;
        }
    }
    Ok(transitions
        .iter()
        .map(|context| (!context.is_empty()).then(|| derive_context(context)))
        .collect())
}

pub(crate) fn encode_huffman_archive(
    base: u32,
    symbol_count: usize,
    banks: &[Vec<Option<Vec<u16>>>],
) -> Result<HuffmanArchive, String> {
    if symbol_count == 0 || symbol_count > 0x1000 {
        return Err("symbol count must be a twelve-bit alphabet".into());
    }
    let contexts = derive_contexts(symbol_count, banks)?;
    let mut bytes = Vec::new();
    let mut offsets = Vec::new();
    let mut model_bases = Vec::new();
    let mut group_start = 0;
    for (index, context) in contexts.iter().enumerate() {
        if index % 256 == 0 {
            group_start = bytes.len();
            model_bases.push(base + group_start as u32);
        }
        let Some(context) = context else {
            offsets.extend(0x8000u16.to_le_bytes());
            continue;
        };
        let leaves = pack_leaves(&context.leaves);
        let offset = u16::try_from(bytes.len() + leaves.len() - group_start)
            .map_err(|_| "context model exceeds its offset table".to_owned())?;
        offsets.extend(offset.to_le_bytes());
        bytes.extend(leaves);
        bytes.extend(pack_bits(&context.tree));
    }
    if !(base as usize + bytes.len()).is_multiple_of(2) {
        bytes.push(0);
    }
    let offset_table = base + bytes.len() as u32;
    bytes.extend(offsets);
    while !(base as usize + bytes.len()).is_multiple_of(4) {
        bytes.push(0);
    }
    let context_directory = base + bytes.len() as u32;
    let mut pointers = Vec::new();
    for (group, model_base) in model_bases.iter().enumerate() {
        pointers.extend([bytes.len(), bytes.len() + 4]);
        bytes.extend(model_base.to_le_bytes());
        bytes.extend((offset_table + group as u32 * 512).to_le_bytes());
    }
    let mut address = base + bytes.len() as u32;
    let mut directory = Vec::new();
    for bank in banks {
        let mut payload = Vec::new();
        let mut lengths = Vec::new();
        for message in bank {
            let mut bits = Vec::new();
            let mut context = 0u16;
            for symbol in message
                .iter()
                .flatten()
                .copied()
                .chain(message.as_ref().map(|_| 0))
            {
                let path = contexts[usize::from(context)]
                    .as_ref()
                    .and_then(|context| context.paths.get(&symbol))
                    .ok_or_else(|| "message leaves its context model".to_owned())?;
                bits.extend(path);
                context = symbol;
            }
            let encoded = pack_bits(&bits);
            let mut size = u16::try_from(encoded.len())
                .map_err(|_| "encoded message exceeds its length field".to_owned())?;
            while size >= 0xff {
                lengths.push(0xff);
                size -= 0xff;
            }
            lengths.push(size as u8);
            payload.extend(encoded);
        }
        directory.extend(address.to_le_bytes());
        address += payload.len() as u32;
        directory.extend(address.to_le_bytes());
        address += lengths.len() as u32;
        bytes.extend(payload);
        bytes.extend(lengths);
    }
    while !(base as usize + bytes.len()).is_multiple_of(4) {
        bytes.push(0);
    }
    let directory_address = base + bytes.len() as u32;
    pointers.extend((0..directory.len()).step_by(4).map(|at| bytes.len() + at));
    bytes.extend(directory);
    Ok(HuffmanArchive {
        bytes,
        context_directory,
        directory: directory_address,
        pointers,
    })
}

