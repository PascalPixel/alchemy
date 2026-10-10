use crate::compiler::bundle::compiler_command;
use crate::compiler::routing::cflags_for_target_source;
use crate::targets::DecompTarget;
use std::collections::HashMap;
use std::path::Path;

#[derive(Clone, Copy, Debug, PartialEq, Eq, Hash)]
pub(crate) struct Int {
    pub bytes: u32,
    pub signed: bool,
}

impl Int {
    pub(crate) const U8: Int = Int { bytes: 1, signed: false };

    pub(crate) fn parse(name: &str) -> Option<Int> {
        let signed = match name.as_bytes().first()? {
            b'u' => false,
            b's' => true,
            _ => return None,
        };
        let bytes = match &name[1..] {
            "8" => 1,
            "16" => 2,
            "32" => 4,
            _ => return None,
        };
        Some(Int { bytes, signed })
    }
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) enum Kind {
    Int(Int),
    Pointer(Pointee),
    Other,
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) enum Pointee {
    Int(u32),
    Void,
    Record(String),
    Other,
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) struct Member {
    pub path: String,
    pub offset: u32,
    pub kind: Kind,
}

enum Node {
    Alias(u32),
    Range(i128, i128),
    Pointer(u32),
    Function,
    Record(Vec<Field>),
    Reference(String),
    Other,
}

struct Field {
    name: String,
    ty: u32,
    bit: u32,
    bits: u32,
}

#[derive(Default)]
pub(crate) struct Stabs {
    nodes: HashMap<u32, Node>,
    tags: HashMap<String, u32>,
    names: HashMap<u32, String>,
    typedefs: HashMap<String, u32>,
}

impl Stabs {
    pub(crate) fn parse(assembly: &str) -> Stabs {
        let mut stabs = Stabs::default();
        let mut pending = String::new();
        for line in assembly.lines() {
            let Some(rest) = line.trim_start().strip_prefix(".stabs \"") else {
                continue;
            };
            let Some(string) = quoted(rest) else {
                continue;
            };
            if let Some(head) = string.strip_suffix('\\') {
                pending.push_str(head);
                continue;
            }
            pending.push_str(&string);
            let record = std::mem::take(&mut pending);
            stabs.record(&record);
        }
        stabs
    }

    fn record(&mut self, record: &str) {
        let Some((name, descriptor)) = record.split_once(':') else {
            return;
        };
        let letters = descriptor.bytes().take_while(u8::is_ascii_alphabetic).count();
        let (tag, typedef) = (descriptor[..letters].contains('T'), descriptor[..letters].contains('t'));
        let mut parser = Parser {
            text: descriptor.as_bytes(),
            at: letters,
            stabs: self,
        };
        let Some(ty) = parser.type_ref() else {
            return;
        };
        if tag && matches!(self.nodes.get(&ty), Some(Node::Record(_))) {
            self.tags.entry(name.to_string()).or_insert(ty);
            self.names.insert(ty, name.to_string());
        }
        if typedef {
            self.typedefs.insert(name.to_string(), ty);
        }
    }

    fn resolve(&self, ty: u32) -> Option<(u32, &Node)> {
        let mut ty = ty;
        for _ in 0..64 {
            match self.nodes.get(&ty)? {
                Node::Alias(next) if *next != ty => ty = *next,
                node => return Some((ty, node)),
            }
        }
        None
    }

    fn kind(&self, ty: u32, bits: Option<u32>) -> Kind {
        match self.resolve(ty) {
            Some((_, Node::Range(lo, hi))) => int_of(*lo, *hi, bits).map_or(Kind::Other, Kind::Int),
            Some((_, Node::Pointer(to))) => Kind::Pointer(self.pointee(*to)),
            _ => Kind::Other,
        }
    }

    fn pointee(&self, ty: u32) -> Pointee {
        match self.resolve(ty) {
            Some((_, Node::Alias(_))) => Pointee::Void,
            Some((_, Node::Range(lo, hi))) => int_of(*lo, *hi, None).map_or(Pointee::Other, |int| Pointee::Int(int.bytes)),
            Some((ty, Node::Record(_))) => self.names.get(&ty).map_or(Pointee::Other, |name| Pointee::Record(name.clone())),
            Some((_, Node::Reference(name))) => Pointee::Record(name.clone()),
            _ => Pointee::Other,
        }
    }

    pub(crate) fn members(&self, tag: &str) -> Option<Vec<Member>> {
        let ty = *self.tags.get(tag)?;
        let mut members = Vec::new();
        self.flatten(ty, 0, "", &mut members, 0);
        Some(members)
    }

    fn flatten(&self, ty: u32, base: u32, prefix: &str, out: &mut Vec<Member>, depth: usize) {
        let Some((_, Node::Record(fields))) = self.resolve(ty) else {
            return;
        };
        if depth > 8 {
            return;
        }
        for field in fields {
            if field.bit % 8 != 0 {
                continue;
            }
            let path = if prefix.is_empty() {
                field.name.clone()
            } else {
                format!("{prefix}.{}", field.name)
            };
            let offset = base + field.bit / 8;
            if let Some((_, Node::Record(_))) = self.resolve(field.ty) {
                self.flatten(field.ty, offset, &path, out, depth + 1);
                continue;
            }
            let kind = self.kind(field.ty, Some(field.bits));
            if kind != Kind::Other {
                out.push(Member { path, offset, kind });
            }
        }
    }

    pub(crate) fn typedef(&self, name: &str) -> Option<Kind> {
        let ty = *self.typedefs.get(name)?;
        match self.resolve(ty) {
            Some((_, Node::Function)) => None,
            _ => Some(self.kind(ty, None)),
        }
    }
}

fn int_of(lo: i128, hi: i128, bits: Option<u32>) -> Option<Int> {
    let natural = match (lo, hi) {
        (0, 255) | (-128, 127) => Some(1),
        (0, 65535) | (-32768, 32767) => Some(2),
        (-2147483648, 2147483647) | (0, 4294967295) => Some(4),
        (0, -1) => None,
        _ => return None,
    };
    let bytes = match (natural, bits) {
        (Some(n), None) => n,
        (Some(n), Some(bits)) if bits == n * 8 => n,
        (None, None) | (None, Some(32)) => 4,
        _ => return None,
    };
    Some(Int { bytes, signed: lo < 0 })
}

fn quoted(rest: &str) -> Option<String> {
    let mut out = String::new();
    let mut chars = rest.chars();
    while let Some(c) = chars.next() {
        match c {
            '"' => return Some(out),
            '\\' => out.push(chars.next()?),
            c => out.push(c),
        }
    }
    None
}

struct Parser<'a> {
    text: &'a [u8],
    at: usize,
    stabs: &'a mut Stabs,
}

impl Parser<'_> {
    fn peek(&self) -> Option<u8> {
        self.text.get(self.at).copied()
    }

    fn eat(&mut self, byte: u8) -> Option<()> {
        (self.peek() == Some(byte)).then(|| self.at += 1)
    }

    fn number(&mut self) -> Option<i128> {
        let start = self.at;
        if self.peek() == Some(b'-') {
            self.at += 1;
        }
        while self.peek().is_some_and(|b| b.is_ascii_digit()) {
            self.at += 1;
        }
        let text = std::str::from_utf8(&self.text[start..self.at]).ok()?;
        if text.len() > 1 && text.starts_with('0') {
            i128::from_str_radix(&text[1..], 8).ok()
        } else {
            text.parse().ok()
        }
    }

    fn until(&mut self, byte: u8) -> Option<String> {
        let length = self.text[self.at..].iter().position(|&b| b == byte)?;
        let text = String::from_utf8_lossy(&self.text[self.at..self.at + length]).into_owned();
        self.at += length + 1;
        Some(text)
    }

    fn type_ref(&mut self) -> Option<u32> {
        let ty = u32::try_from(self.number()?).ok()?;
        if self.eat(b'=').is_some() {
            let node = self.definition(ty)?;
            self.stabs.nodes.insert(ty, node);
        }
        Some(ty)
    }

    fn definition(&mut self, ty: u32) -> Option<Node> {
        let byte = self.peek()?;
        if byte.is_ascii_digit() {
            return Some(Node::Alias(self.type_ref()?));
        }
        self.at += 1;
        Some(match byte {
            b'r' => {
                self.type_ref()?;
                self.eat(b';')?;
                let lo = self.number()?;
                self.eat(b';')?;
                let hi = self.number()?;
                self.eat(b';')?;
                Node::Range(lo, hi)
            }
            b'*' => Node::Pointer(self.type_ref()?),
            b'f' => {
                self.type_ref()?;
                Node::Function
            }
            b'k' | b'B' => Node::Alias(self.type_ref()?),
            b'a' => {
                self.eat(b'r')?;
                self.type_ref()?;
                for _ in 0..2 {
                    self.eat(b';')?;
                    self.number()?;
                }
                self.eat(b';')?;
                self.type_ref()?;
                Node::Other
            }
            b's' | b'u' => {
                self.number()?;
                let mut fields = Vec::new();
                while self.peek()? != b';' {
                    let name = self.until(b':')?;
                    let ty = self.type_ref()?;
                    self.eat(b',')?;
                    let bit = u32::try_from(self.number()?).ok()?;
                    self.eat(b',')?;
                    let bits = u32::try_from(self.number()?).ok()?;
                    self.eat(b';')?;
                    fields.push(Field { name, ty, bit, bits });
                }
                self.at += 1;
                Node::Record(fields)
            }
            b'e' => {
                while self.peek()? != b';' {
                    self.until(b':')?;
                    self.number()?;
                    self.eat(b',')?;
                }
                self.at += 1;
                Node::Other
            }
            b'x' => {
                self.at += 1;
                let name = self.until(b':')?;
                self.stabs.names.insert(ty, name.clone());
                Node::Reference(name)
            }
            b'@' => {
                self.until(b';')?;
                return self.definition(ty);
            }
            _ => return None,
        })
    }
}

pub(crate) struct Probe {
    pub name: String,
    pub expression: String,
}

const PROBE: &str = "alchemy_probe.c";

pub(crate) fn compile(target: DecompTarget, imports: Option<&Path>, file: &Path, probes: &[Probe]) -> Result<Stabs, String> {
    let directory = tempfile::Builder::new()
        .prefix("alchemy-camelotize-")
        .tempdir()
        .map_err(|e| e.to_string())?;
    let mut kept: Vec<&Probe> = probes.iter().collect();
    loop {
        let source = directory.path().join(PROBE);
        let assembly = directory.path().join("alchemy_probe.s");
        let mut text = format!("#include \"{}\"\nvoid alchemy_probe(void)\n{{\n", file.display());
        for probe in &kept {
            text.push_str(&format!("typedef __typeof__({}) {};\n", probe.expression, probe.name));
        }
        text.push_str("}\n");
        std::fs::write(&source, &text).map_err(|e| e.to_string())?;
        let mut arguments = cflags_for_target_source(target.compiler, PROBE);
        arguments.push(format!("-D{}=1", target.edition_define));
        if let Some(imports) = imports {
            arguments.push(format!("-I{}", imports.display()));
        }
        for flag in ["-w", "-gstabs", "-S", "-o"] {
            arguments.push(flag.to_string());
        }
        arguments.push(assembly.to_string_lossy().into_owned());
        arguments.extend(["-x".to_string(), "c".to_string(), source.to_string_lossy().into_owned()]);
        let command = compiler_command(&arguments)?;
        match crate::process::run(&command, crate::compiler::routing::root()) {
            Ok(_) => {
                let output = std::fs::read_to_string(&assembly).map_err(|e| e.to_string())?;
                return Ok(Stabs::parse(&output));
            }
            Err(error) => {
                let failed = failed_lines(&error);
                let before = kept.len();
                let mut index = 0;
                kept.retain(|_| {
                    index += 1;
                    !failed.contains(&(index + 3))
                });
                if kept.len() == before || failed.iter().any(|&line| line <= 3) {
                    return Err(format!("{}: {error}", file.display()));
                }
            }
        }
    }
}

fn failed_lines(error: &str) -> Vec<usize> {
    let marker = format!("{PROBE}:");
    error
        .match_indices(&marker)
        .filter_map(|(at, _)| {
            let rest = &error[at + marker.len()..];
            rest[..rest.find(':')?].parse().ok()
        })
        .collect()
}

#[cfg(test)]
mod tests {
    use super::*;

    const ASSEMBLY: &str = r#".stabs "int:t1=r1;-2147483648;2147483647;",128,0,0,0
.stabs "short int:t8=r8;-32768;32767;",128,0,0,0
.stabs "short unsigned int:t9=r9;0;65535;",128,0,0,0
.stabs "unsigned char:t11=r11;0;255;",128,0,0,0
.stabs "__builtin_va_list:t19=*20=20",128,0,0,0
.stabs "u8:t22=11",128,0,0,0
.stabs "s16:t23=8",128,0,0,0
.stabs "u16:t24=9",128,0,0,0
.stabs "s32:t25=1",128,0,0,0
.stabs "FieldCoordinate:T56=u4fixed:25,0,32;part:57=s4fraction:24,0,16;pixel:23,16,16;;,0,32;;",128,0,0,0
.stabs "FieldActor:T35=s16unknown_00:38=ar32=r32;0;-1;;0;5;22,0,48;facing:24,48,16;x:56,64,32;flags:22,96,4;sprite:66=*58=xsFieldSprite:,96,32;;",128,0,0,0
.stabs "alchemy_probe_Actor_Get:t27=28=*35",128,0,0,0
.stabs "alchemy_probe_gWork:t29=30=*22",128,0,0,0
"#;

    #[test]
    fn layouts_come_from_the_records_with_unions_flattened() {
        let stabs = Stabs::parse(ASSEMBLY);
        let members = stabs.members("FieldActor").unwrap();
        let shown: Vec<(&str, u32)> = members.iter().map(|m| (m.path.as_str(), m.offset)).collect();
        assert_eq!(
            shown,
            [("facing", 6), ("x.fixed", 8), ("x.part.fraction", 8), ("x.part.pixel", 10), ("sprite", 12)]
        );
        assert_eq!(members[3].kind, Kind::Int(Int { bytes: 2, signed: true }));
        assert_eq!(members[4].kind, Kind::Pointer(Pointee::Record("FieldSprite".into())));
        assert!(stabs.members("EventWork").is_none());
    }

    #[test]
    fn probes_report_declared_types() {
        let stabs = Stabs::parse(ASSEMBLY);
        assert_eq!(stabs.typedef("alchemy_probe_Actor_Get"), Some(Kind::Pointer(Pointee::Record("FieldActor".into()))));
        assert_eq!(stabs.typedef("alchemy_probe_gWork"), Some(Kind::Pointer(Pointee::Int(1))));
        assert_eq!(failed_lines("x/alchemy_probe.c:5: `gWork' undeclared"), [5]);
    }
}
