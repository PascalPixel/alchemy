use std::fmt::Write;

#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) struct Pointer {
    pub symbol: String,
    pub addend: i64,
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) struct Label {
    pub name: String,
    pub offset: usize,
    pub global: bool,
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) struct Data {
    pub bytes: Vec<u8>,
    pub labels: Vec<Label>,
    pub pointers: Vec<(usize, Pointer)>,
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
    pub(crate) fn from_bytes(bytes: Vec<u8>) -> Self {
        Self {
            bytes,
            ..Self::default()
        }
    }
    pub fn label(&mut self, name: &str, global: bool) {
        self.labels.push(Label {
            name: name.to_owned(),
            offset: self.bytes.len(),
            global,
        });
    }
    pub(crate) fn pointer(&mut self, symbol: &str, addend: i64) {
        self.pointers.push((
            self.bytes.len(),
            Pointer {
                symbol: symbol.to_owned(),
                addend,
            },
        ));
        self.bytes.extend([0; 4]);
    }
    pub(crate) fn align_to(&mut self, boundary: usize, fill: u8) -> Result<(), String> {
        if !boundary.is_power_of_two() {
            return Err(format!("alignment {boundary} is not a power of two"));
        }
        self.align = self.align.max(boundary);
        self.bytes
            .resize(self.bytes.len().next_multiple_of(boundary), fill);
        Ok(())
    }
    pub(crate) fn append(&mut self, other: Data) -> Result<(), String> {
        let offset = self.bytes.len();
        if !offset.is_multiple_of(other.align) {
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
    pub(crate) fn source(&self) -> Result<String, String> {
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

pub(crate) fn identifier(name: &str) -> bool {
    let mut characters = name.chars();
    characters
        .next()
        .is_some_and(|c| c == '_' || c.is_ascii_alphabetic())
        && characters.all(|c| c == '_' || c.is_ascii_alphanumeric())
}
