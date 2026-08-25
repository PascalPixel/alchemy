use crate::jsparse::js_parse_int_radix;
use regex::Regex;
use std::process::Command;
#[derive(Debug, Default, Clone)]
pub struct Rows {
    entries: Vec<(f64, String)>,
}
impl Rows {
    pub fn set(&mut self, key: f64, value: String) {
        if let Some((_, old)) = self.entries.iter_mut().find(|(at, _)| *at == key) {
            *old = value;
        } else {
            self.entries.push((key, value));
        }
    }
    pub fn get(&self, key: f64) -> Option<&str> {
        self.entries.iter().find(|(at, _)| *at == key).map(|(_, value)| value.as_str())
    }
    pub fn keys(&self) -> impl Iterator<Item = f64> + '_ {
        self.entries.iter().map(|(key, _)| *key)
    }
    pub fn len(&self) -> usize {
        self.entries.len()
    }
}
pub fn parse_row(line: &str) -> Option<(&str, &str, &str)> {
    static ROW: std::sync::OnceLock<Regex> = std::sync::OnceLock::new();
    ROW.get_or_init(|| Regex::new(r"^\s+([0-9a-f]+):\t([0-9a-f ]+)\t(.*)$").unwrap()).captures(line).map(|captures| (captures.get(1).unwrap().as_str(), captures.get(2).unwrap().as_str(), captures.get(3).unwrap().as_str()))
}
pub fn rows_from_output(output: &str, base: f64) -> Rows {
    let mut rows = Rows::default();
    for line in output.split('\n') {
        if let Some((address, _, text)) = parse_row(line) {
            rows.set(js_parse_int_radix(address, 16) - base, text.trim_end().into());
        }
    }
    rows
}
pub fn disassemble(binary: &str, base: f64) -> Result<Rows, String> {
    let output = Command::new("arm-none-eabi-objdump").args(["-D", "-b", "binary", "-m", "arm", "-M", "force-thumb", &format!("--adjust-vma=0x{:x}", base as u64), binary]).output().map_err(|error| format!("objdump failed: {error}"))?;
    if !output.status.success() {
        return Err(format!("objdump failed: {}", String::from_utf8_lossy(&output.stderr).trim()));
    }
    Ok(rows_from_output(&String::from_utf8_lossy(&output.stdout), base))
}
