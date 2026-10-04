//! Context-modelled Huffman message archives: one canonical tree per preceding
//! symbol, packed leaf tables, bank payloads with byte-run lengths and an
//! address directory. No game addresses, command tables or bank counts.
#[cfg(test)]
use psynergy::assets::huffman_archive::MessageReader;
use std::collections::HashMap;

/// A built archive and the addresses its self-referential header records.
pub struct HuffmanArchive {
    pub bytes: Vec<u8>,
    pub offset_table: u32,
    pub messages: u32,
    pub context_directory: u32,
    pub directory: u32,
    pub contexts: usize,
    /// Offsets into `bytes` of the little-endian words that hold an address
    /// inside the archive, so a linker can relocate them from the archive's
    /// own label instead of trusting `base`.
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

/// Bits are packed least-significant bit first; the last byte is zero padded.
fn pack_bits(bits: &[bool]) -> Vec<u8> {
    let mut bytes = vec![0u8; bits.len().div_ceil(8)];
    for (index, _) in bits.iter().enumerate().filter(|(_, bit)| **bit) {
        bytes[index >> 3] |= 1 << (index & 7);
    }
    bytes
}

/// Leaves are twelve-bit symbols packed two per three bytes; the groups are
/// stored last first and an odd final symbol takes two bytes.
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

/// Build the canonical tree for one context from its transition counts in
/// first-seen order: the two lightest nodes merge, ties resolved by age.
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

/// Every message starts in context zero and ends with symbol zero; the
/// context after each symbol is that symbol.
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

/// Encode message banks as a context-modelled Huffman archive at `base`:
/// context records (packed leaves, then tree bits), a halfword offset table
/// (offset of each tree, `0x8000` for an unused context) padded to a word,
/// the archive and offset-table addresses, then each bank's payloads followed
/// by its byte-run lengths, and finally the bank directory of payload and
/// length addresses, aligned to a word for the ARM message lookup.
/// `None` is an empty message.
pub fn encode_huffman_archive(
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
    if (base as usize + bytes.len()) % 2 != 0 {
        bytes.push(0);
    }
    let offset_table = base + bytes.len() as u32;
    bytes.extend(offsets);
    while (base as usize + bytes.len()) % 4 != 0 {
        bytes.push(0);
    }
    let context_directory = base + bytes.len() as u32;
    let mut pointers = Vec::new();
    for (group, model_base) in model_bases.iter().enumerate() {
        pointers.extend([bytes.len(), bytes.len() + 4]);
        bytes.extend(model_base.to_le_bytes());
        bytes.extend((offset_table + group as u32 * 512).to_le_bytes());
    }
    let messages = base + bytes.len() as u32;
    let mut address = messages;
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
    // The ARM lookup reads both bank pointers with a word transfer.
    while (base as usize + bytes.len()) % 4 != 0 {
        bytes.push(0);
    }
    let directory_address = base + bytes.len() as u32;
    pointers.extend((0..directory.len()).step_by(4).map(|at| bytes.len() + at));
    bytes.extend(directory);
    Ok(HuffmanArchive {
        bytes,
        offset_table,
        messages,
        context_directory,
        directory: directory_address,
        contexts: contexts.iter().flatten().count(),
        pointers,
    })
}

#[test]
fn huffman_archive_packs_model_payloads_and_directory() {
    // Context 0 sees 1 then 2; context 1 sees 1 then 0; context 2 sees 0.
    let banks = vec![vec![Some(vec![1, 1]), None], vec![Some(vec![2])]];
    let archive = encode_huffman_archive(0x1000, 3, &banks).unwrap();
    let mut expected = Vec::new();
    // Context 0: leaves [1, 2] packed as one group, tree "011" (branch, leaf, leaf).
    expected.extend([0x02, 0x10, 0x00, 0b110]);
    // Context 1: leaves [1, 0], tree "011".
    expected.extend([0x00, 0x10, 0x00, 0b110]);
    // Context 2: single leaf 0, tree "1".
    expected.extend([0x00, 0x00, 0b1]);
    // Offsets point at each tree; the table is padded to a word boundary.
    expected.push(0);
    expected.extend([3, 0, 7, 0, 10, 0, 0, 0]);
    expected.extend(0x1000u32.to_le_bytes());
    expected.extend(0x100cu32.to_le_bytes());
    // Bank 0: "1 1 end" is 0, 0, 1 -> 0b100; the empty message has length 0.
    expected.extend([0b100, 1, 0]);
    // Bank 1: "2 end" is 1 then the empty single-leaf path.
    expected.extend([0b1, 1]);
    // The ARM lookup loads the bank directory with a word transfer.
    expected.extend([0, 0, 0]);
    expected.extend(0x101cu32.to_le_bytes());
    expected.extend(0x101du32.to_le_bytes());
    expected.extend(0x101fu32.to_le_bytes());
    expected.extend(0x1020u32.to_le_bytes());
    assert_eq!(archive.bytes, expected);
    assert_eq!(archive.offset_table, 0x100c);
    assert_eq!(archive.messages, 0x101c);
    assert_eq!(archive.directory, 0x1024);
    assert_eq!(archive.contexts, 3);
    // Every address word: the context directory's two, then the banks'.
    assert_eq!(archive.pointers, [20, 24, 36, 40, 44, 48]);
    for site in &archive.pointers {
        let word = u32::from_le_bytes(archive.bytes[*site..site + 4].try_into().unwrap());
        assert!((0x1000..=archive.directory).contains(&word));
    }
    assert!(encode_huffman_archive(0x1000, 3, &[vec![Some(vec![3])]]).is_err());
    assert!(encode_huffman_archive(0x1000, 0x1001, &[]).is_err());
    assert_eq!(
        pack_leaves(&[0xabc, 0x123, 0x456]),
        [0x60, 0x45, 0x23, 0xc1, 0xab]
    );
    // A 256-byte message stores its length as a full run and a remainder.
    let long = encode_huffman_archive(0x1000, 2, &[vec![Some(vec![1; 2041])]]).unwrap();
    let directory = (long.directory - 0x1000) as usize;
    let lengths =
        u32::from_le_bytes(long.bytes[directory + 4..directory + 8].try_into().unwrap()) - 0x1000;
    assert_eq!(
        long.bytes[lengths as usize..lengths as usize + 2],
        [0xff, 0x01]
    );
}

#[test]
fn grouped_contexts_round_trip_symbols_above_255() {
    let banks = vec![vec![Some(vec![1, 256, 299, 256]), None]];
    let archive = encode_huffman_archive(0x1000, 300, &banks).unwrap();
    assert_eq!(archive.messages - archive.context_directory, 16);
    let mut reader = MessageReader::new(
        &archive.bytes,
        0x1000,
        archive.context_directory,
        archive.directory,
        300,
    )
    .unwrap();
    assert_eq!(reader.message(0).unwrap().symbols, banks[0][0]);
    assert_eq!(reader.message(1).unwrap().symbols, None);
}

#[test]
fn message_reader_inverts_owned_encoder_and_rejects_truncation() {
    let banks = vec![vec![Some(vec![1, 2, 1]), None, Some(vec![2; 2041])]];
    let archive = encode_huffman_archive(0x1000, 3, &banks).unwrap();
    let contexts = archive.messages - 8;
    let mut reader =
        MessageReader::new(&archive.bytes, 0x1000, contexts, archive.directory, 3).unwrap();
    for (index, expected) in banks[0].iter().enumerate() {
        assert_eq!(&reader.message(index).unwrap().symbols, expected);
    }
    let mut truncated = MessageReader::new(
        &archive.bytes[..archive.bytes.len() - 1],
        0x1000,
        contexts,
        archive.directory,
        3,
    )
    .unwrap();
    assert!(truncated.message(0).is_err());
    let mut invalid = archive.bytes.clone();
    invalid[(contexts - 0x1000) as usize..(contexts - 0x1000) as usize + 4]
        .copy_from_slice(&0u32.to_le_bytes());
    assert!(
        MessageReader::new(&invalid, 0x1000, contexts, archive.directory, 3)
            .unwrap()
            .message(0)
            .is_err()
    );
}

#[test]
fn huffman_archive_preserves_an_already_aligned_directory() {
    // Three extra null messages make the two banks occupy eight bytes.
    // The models, payload addresses and complete archive need no padding.
    let banks = vec![
        vec![Some(vec![1, 1]), None],
        vec![Some(vec![2]), None, None, None],
    ];
    let archive = encode_huffman_archive(0x1000, 3, &banks).unwrap();
    let expected = [
        0x02, 0x10, 0x00, 0x06, 0x00, 0x10, 0x00, 0x06, 0x00, 0x00, 0x01, 0x00, 0x03, 0x00, 0x07,
        0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x0c, 0x10, 0x00, 0x00, 0x04, 0x01,
        0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x1c, 0x10, 0x00, 0x00, 0x1d, 0x10, 0x00, 0x00, 0x1f,
        0x10, 0x00, 0x00, 0x20, 0x10, 0x00, 0x00,
    ];
    assert_eq!(archive.bytes, expected);
    assert_eq!(archive.directory, 0x1024);
    assert_eq!(archive.pointers, [20, 24, 36, 40, 44, 48]);
}

#[test]
fn huffman_archive_aligns_each_bank_directory_residue() {
    for nulls in 0..4 {
        let mut bank = vec![Some(vec![1, 1])];
        bank.extend(vec![None; nulls]);
        let archive = encode_huffman_archive(0x1000, 2, &[bank.clone()]).unwrap();
        assert_eq!(archive.directory % 4, 0, "{nulls} null messages");
        assert!(archive.pointers.iter().all(|site| site % 4 == 0));
        let mut reader = MessageReader::new(
            &archive.bytes,
            0x1000,
            archive.context_directory,
            archive.directory,
            2,
        )
        .unwrap();
        for (index, expected) in bank.iter().enumerate() {
            assert_eq!(&reader.message(index).unwrap().symbols, expected);
        }
    }
}
