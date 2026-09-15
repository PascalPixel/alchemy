use super::{AssetError, Node};
use std::collections::HashMap;

pub struct Message {
    pub symbols: Option<Vec<u16>>,
    pub offset: usize,
    pub bytes: usize,
}
pub struct MessageReader<'a> {
    data: &'a [u8],
    base: u32,
    contexts: usize,
    banks: usize,
    alphabet: usize,
    trees: HashMap<u16, Node>,
}
fn error() -> AssetError {
    AssetError("message archive extent or tree is invalid".into())
}
fn bit(data: &[u8], start: usize, position: &mut usize) -> Result<bool, AssetError> {
    let offset = start.checked_add(*position / 8).ok_or_else(error)?;
    let byte = data.get(offset).ok_or_else(error)?;
    let value = byte & (1 << (*position % 8)) != 0;
    *position = position.checked_add(1).ok_or_else(error)?;
    Ok(value)
}
impl<'a> MessageReader<'a> {
    pub fn new(
        data: &'a [u8],
        base: u32,
        contexts: u32,
        banks: u32,
        alphabet: usize,
    ) -> Result<Self, AssetError> {
        if alphabet == 0 || alphabet > 4096 {
            return Err(error());
        }
        let contexts = contexts.checked_sub(base).ok_or_else(error)? as usize;
        let banks = banks.checked_sub(base).ok_or_else(error)? as usize;
        if contexts >= data.len() || banks >= data.len() {
            return Err(error());
        }
        Ok(Self {
            data,
            base,
            contexts,
            banks,
            alphabet,
            trees: HashMap::new(),
        })
    }
    fn pointer(&self, offset: usize) -> Result<usize, AssetError> {
        let bytes = self
            .data
            .get(offset..offset.checked_add(4).ok_or_else(error)?)
            .ok_or_else(error)?;
        let pointer = u32::from_le_bytes(bytes.try_into().unwrap())
            .checked_sub(self.base)
            .ok_or_else(error)? as usize;
        if pointer > self.data.len() {
            return Err(error());
        }
        Ok(pointer)
    }
    fn tree(&self, context: u16) -> Result<Node, AssetError> {
        if context as usize >= self.alphabet {
            return Err(error());
        }
        let header = self.contexts + context as usize / 256 * 8;
        let base = self.pointer(header)?;
        let offsets = self.pointer(header + 4)?;
        let position = offsets + context as usize % 256 * 2;
        let bytes = self.data.get(position..position + 2).ok_or_else(error)?;
        let offset = u16::from_le_bytes(bytes.try_into().unwrap());
        if offset == 0x8000 {
            return Err(error());
        }
        let start = base.checked_add(offset as usize).ok_or_else(error)?;
        fn node(
            data: &[u8],
            start: usize,
            position: &mut usize,
            ordinal: &mut usize,
            limit: usize,
            depth: usize,
        ) -> Result<Node, AssetError> {
            if depth > limit || *position > limit * 2 {
                return Err(error());
            }
            if bit(data, start, position)? {
                let pos = start
                    .checked_sub(*ordinal + *ordinal / 2)
                    .ok_or_else(error)?;
                let bytes = data
                    .get(pos.checked_sub(2).ok_or_else(error)?..pos)
                    .ok_or_else(error)?;
                let symbol = if *ordinal % 2 == 1 {
                    (u16::from(bytes[1] & 15) << 8) | u16::from(bytes[0])
                } else {
                    (u16::from(bytes[1]) << 4) | u16::from(bytes[0] >> 4)
                };
                if symbol as usize >= limit {
                    return Err(error());
                }
                *ordinal += 1;
                return Ok(Node::Leaf(symbol));
            }
            Ok(Node::Branch(
                Box::new(node(data, start, position, ordinal, limit, depth + 1)?),
                Box::new(node(data, start, position, ordinal, limit, depth + 1)?),
            ))
        }
        node(self.data, start, &mut 0, &mut 0, self.alphabet, 0)
    }
    pub fn message(&mut self, index: usize) -> Result<Message, AssetError> {
        let header = self.banks.checked_add(index / 256 * 8).ok_or_else(error)?;
        let mut payload = self.pointer(header)?;
        let mut lengths = self.pointer(header + 4)?;
        let mut size = 0usize;
        for row in 0..=index % 256 {
            size = 0;
            loop {
                let length = *self.data.get(lengths).ok_or_else(error)?;
                lengths += 1;
                size = size.checked_add(length as usize).ok_or_else(error)?;
                if length != 255 {
                    break;
                }
            }
            if row != index % 256 {
                payload = payload.checked_add(size).ok_or_else(error)?;
            }
        }
        let data = self
            .data
            .get(payload..payload.checked_add(size).ok_or_else(error)?)
            .ok_or_else(error)?;
        if size == 0 {
            return Ok(Message {
                symbols: None,
                offset: payload,
                bytes: 0,
            });
        }
        let mut position = 0;
        let mut context = 0;
        let mut output = vec![];
        for _ in 0..65536 {
            if !self.trees.contains_key(&context) {
                self.trees.insert(context, self.tree(context)?);
            }
            let mut node = &self.trees[&context];
            while let Node::Branch(left, right) = node {
                node = if bit(data, 0, &mut position)? {
                    right
                } else {
                    left
                };
            }
            let Node::Leaf(symbol) = node else {
                unreachable!()
            };
            if *symbol == 0 {
                return Ok(Message {
                    symbols: Some(output),
                    offset: payload,
                    bytes: size,
                });
            }
            output.push(*symbol);
            context = *symbol;
        }
        Err(AssetError("message exceeds symbol limit".into()))
    }
}

#[test]
fn message_reader_inverts_owned_encoder_and_rejects_truncation() {
    use super::encode_huffman_archive;
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
