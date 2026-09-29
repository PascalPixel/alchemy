//! Built data as assembler source: bytes, the labels defined inside them, and
//! the words that hold a label's address. The linker resolves every address,
//! as it does for pret's data sources, so no encoder knows where its output
//! lands and none takes an address.
use std::fmt::Write;

/// A little-endian word of [`Data`] that holds `symbol + addend`.
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Pointer {
    pub symbol: String,
    pub addend: i64,
}

/// A label defined `offset` bytes into [`Data`]; `global` exports it.
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Label {
    pub name: String,
    pub offset: usize,
    pub global: bool,
}

/// Encoded bytes with their labels and address words. Every address word is
/// stored as zero bytes and written as a `.4byte` of its label, so the data
/// itself never carries an address.
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Data {
    pub bytes: Vec<u8>,
    pub labels: Vec<Label>,
    /// Byte offsets of the words that hold addresses, in ascending order.
    pub pointers: Vec<(usize, Pointer)>,
    /// The alignment, a power of two, that the data's first byte needs: the
    /// encoders lay out padding relative to it.
    pub align: usize,
}

impl Default for Data {
    fn default() -> Self {
        Self {
            bytes: Vec::new(),
            labels: Vec::new(),
            pointers: Vec::new(),
            align: 1,
        }
    }
}

impl Data {
    /// Plain bytes with no labels or addresses.
    pub fn from_bytes(bytes: Vec<u8>) -> Self {
        Self {
            bytes,
            ..Self::default()
        }
    }
    /// Define `name` at the current end of the data.
    pub fn label(&mut self, name: &str, global: bool) {
        self.labels.push(Label {
            name: name.to_owned(),
            offset: self.bytes.len(),
            global,
        });
    }
    /// Append a word that holds the address of `symbol + addend`.
    pub fn pointer(&mut self, symbol: &str, addend: i64) {
        self.pointers.push((
            self.bytes.len(),
            Pointer {
                symbol: symbol.to_owned(),
                addend,
            },
        ));
        self.bytes.extend([0; 4]);
    }
    /// Zero bytes up to the next multiple of `boundary` from the data's start,
    /// raising the data's own alignment to at least `boundary`.
    pub fn align_to(&mut self, boundary: usize, fill: u8) -> Result<(), String> {
        if !boundary.is_power_of_two() {
            return Err(format!("alignment {boundary} is not a power of two"));
        }
        self.align = self.align.max(boundary);
        self.bytes
            .resize(self.bytes.len().next_multiple_of(boundary), fill);
        Ok(())
    }
    /// Append `other`, which must start on its own alignment.
    pub fn append(&mut self, other: Data) -> Result<(), String> {
        let offset = self.bytes.len();
        if offset % other.align != 0 {
            return Err(format!(
                "appended data needs {}-byte alignment at offset {offset}",
                other.align
            ));
        }
        self.align = self.align.max(other.align);
        self.bytes.extend(other.bytes);
        self.labels
            .extend(other.labels.into_iter().map(|label| Label {
                offset: label.offset + offset,
                ..label
            }));
        self.pointers.extend(
            other
                .pointers
                .into_iter()
                .map(|(site, pointer)| (site + offset, pointer)),
        );
        Ok(())
    }
    /// The bytes with every address word that names one of the data's own
    /// labels resolved as if the data started at `base`: the layout of a
    /// stream that is compressed before it is placed, whose words hold
    /// offsets into the stream. A word naming any other symbol is refused.
    pub fn bytes_at(&self, base: u32) -> Result<Vec<u8>, String> {
        self.check()?;
        let mut bytes = self.bytes.clone();
        for (site, pointer) in &self.pointers {
            let label = self
                .labels
                .iter()
                .find(|label| label.name == pointer.symbol)
                .ok_or_else(|| {
                    format!(
                        "{} is not defined inside the data; it can only be linked",
                        pointer.symbol
                    )
                })?;
            let value = i64::from(base) + label.offset as i64 + pointer.addend;
            let value = u32::try_from(value)
                .map_err(|_| format!("{} + {} overflows", pointer.symbol, pointer.addend))?;
            bytes[*site..site + 4].copy_from_slice(&value.to_le_bytes());
        }
        Ok(bytes)
    }
    fn check(&self) -> Result<(), String> {
        if !self.align.is_power_of_two() {
            return Err(format!("alignment {} is not a power of two", self.align));
        }
        for label in &self.labels {
            if !identifier(&label.name) {
                return Err(format!("{} is not an assembler symbol", label.name));
            }
            if label.offset > self.bytes.len() {
                return Err(format!("{} lies past the data", label.name));
            }
        }
        let mut names: Vec<&str> = self.labels.iter().map(|l| l.name.as_str()).collect();
        names.sort_unstable();
        if let Some(pair) = names.windows(2).find(|pair| pair[0] == pair[1]) {
            return Err(format!("{} is defined twice", pair[0]));
        }
        let mut end = 0;
        for (site, pointer) in &self.pointers {
            if !identifier(&pointer.symbol) {
                return Err(format!("{} is not an assembler symbol", pointer.symbol));
            }
            if *site < end || site + 4 > self.bytes.len() {
                return Err(format!("address word of {} overlaps", pointer.symbol));
            }
            if self.bytes[*site..site + 4] != [0; 4] {
                return Err(format!(
                    "address word of {} carries stored bytes",
                    pointer.symbol
                ));
            }
            end = site + 4;
        }
        Ok(())
    }
    /// The assembler source: the data's alignment, its labels, `.byte` rows
    /// and a `.4byte` expression for each address word. The caller chooses
    /// the section.
    pub fn source(&self) -> Result<String, String> {
        self.check()?;
        let mut text = String::new();
        if self.align > 1 {
            writeln!(text, "\t.balign {}", self.align).unwrap();
        }
        let mut labels = self.labels.clone();
        labels.sort_by_key(|label| label.offset);
        let mut labels = labels.into_iter().peekable();
        let mut pointers = self.pointers.iter().peekable();
        let mut row: Vec<String> = Vec::new();
        let flush = |row: &mut Vec<String>, text: &mut String| {
            if !row.is_empty() {
                writeln!(text, "\t.byte {}", row.join(", ")).unwrap();
                row.clear();
            }
        };
        let mut at = 0;
        while at <= self.bytes.len() {
            while let Some(label) = labels.next_if(|label| label.offset == at) {
                flush(&mut row, &mut text);
                if label.global {
                    writeln!(text, "\t.global {}", label.name).unwrap();
                }
                writeln!(text, "{}:", label.name).unwrap();
            }
            if at == self.bytes.len() {
                break;
            }
            if let Some((_, pointer)) = pointers.next_if(|(site, _)| *site == at) {
                flush(&mut row, &mut text);
                match pointer.addend {
                    0 => writeln!(text, "\t.4byte {}", pointer.symbol),
                    addend if addend > 0 => {
                        writeln!(text, "\t.4byte {} + {addend}", pointer.symbol)
                    }
                    addend => writeln!(text, "\t.4byte {} - {}", pointer.symbol, -addend),
                }
                .unwrap();
                at += 4;
                continue;
            }
            row.push(format!("0x{:02x}", self.bytes[at]));
            if row.len() == 16 {
                flush(&mut row, &mut text);
            }
            at += 1;
        }
        flush(&mut row, &mut text);
        Ok(text)
    }
}

/// Whether `name` is a plain assembler and C symbol.
pub fn identifier(name: &str) -> bool {
    let mut characters = name.chars();
    characters
        .next()
        .is_some_and(|c| c == '_' || c.is_ascii_alphabetic())
        && characters.all(|c| c == '_' || c.is_ascii_alphanumeric())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn source_writes_labels_rows_and_address_words() {
        let mut data = Data::from_bytes(vec![1, 2]);
        data.label("Table_Next", false);
        data.pointer("Table", 3);
        data.pointer("Other", -2);
        data.bytes.extend(0..17u8);
        data.labels.insert(
            0,
            Label {
                name: "Table".into(),
                offset: 0,
                global: true,
            },
        );
        data.align_to(4, 0).unwrap();
        assert_eq!(
            data.source().unwrap(),
            "\t.balign 4\n\t.global Table\nTable:\n\t.byte 0x01, 0x02\nTable_Next:\n\
             \t.4byte Table + 3\n\t.4byte Other - 2\n\
             \t.byte 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f\n\
             \t.byte 0x10, 0x00\n"
        );
    }

    #[test]
    fn stream_layout_resolves_only_its_own_labels() {
        let mut data = Data::default();
        data.label("Start", false);
        data.pointer("End", 0);
        data.bytes.push(9);
        data.label("End", false);
        assert_eq!(data.bytes_at(0).unwrap(), [5, 0, 0, 0, 9]);
        assert_eq!(data.bytes_at(0x100).unwrap(), [5, 1, 0, 0, 9]);
        data.pointer("Elsewhere", 0);
        assert!(data.bytes_at(0).unwrap_err().contains("only be linked"));
    }

    #[test]
    fn stored_addresses_duplicate_labels_and_bad_names_are_refused() {
        let mut data = Data::default();
        data.pointer("Target", 0);
        data.bytes[1] = 8;
        assert!(data.source().unwrap_err().contains("stored bytes"));
        let mut data = Data::default();
        data.label("Twice", false);
        data.label("Twice", true);
        assert!(data.source().unwrap_err().contains("defined twice"));
        let mut data = Data::default();
        data.label("0x08000000", false);
        assert!(data.source().is_err());
        let mut data = Data::from_bytes(vec![0; 2]);
        let mut other = Data::default();
        other.align_to(4, 0).unwrap();
        assert!(data.append(other).is_err());
    }

    #[test]
    fn appended_data_keeps_its_labels_and_words() {
        let mut first = Data::from_bytes(vec![7; 4]);
        let mut second = Data::default();
        second.label("Second", true);
        second.pointer("Second", 0);
        second.align_to(4, 0).unwrap();
        first.append(second).unwrap();
        assert_eq!(first.labels[0].offset, 4);
        assert_eq!(first.pointers[0].0, 4);
        assert_eq!(first.align, 4);
        assert_eq!(first.bytes_at(0).unwrap(), [7, 7, 7, 7, 4, 0, 0, 0]);
    }
}
