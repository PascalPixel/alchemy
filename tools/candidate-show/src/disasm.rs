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
        self.entries
            .iter()
            .find(|(at, _)| *at == key)
            .map(|(_, value)| value.as_str())
    }
    pub fn keys(&self) -> impl Iterator<Item = f64> + '_ {
        self.entries.iter().map(|(key, _)| *key)
    }
    pub fn len(&self) -> usize {
        self.entries.len()
    }
}
fn parse_row(line: &str) -> Option<(&str, &str)> {
    let (address, rest) = line.trim_start().split_once(":\t")?;
    let (_, instruction) = rest.split_once('\t')?;
    Some((address, instruction))
}
fn rows_from_output(output: &str, base: f64) -> Rows {
    let mut rows = Rows::default();
    for line in output.lines() {
        if let Some((address, text)) = parse_row(line) {
            let address = u64::from_str_radix(address, 16)
                .ok()
                .map(|value| value as f64);
            if let Some(address) = address {
                rows.set(address - base, text.trim_end().into());
            }
        }
    }
    rows
}
pub fn disassemble(binary: &str, base: f64) -> Result<Rows, String> {
    let output = Command::new("arm-none-eabi-objdump")
        .args([
            "-D",
            "-b",
            "binary",
            "-m",
            "arm",
            "-M",
            "force-thumb",
            &format!("--adjust-vma=0x{:x}", base as u64),
            binary,
        ])
        .output()
        .map_err(|error| format!("objdump failed: {error}"))?;
    if !output.status.success() {
        return Err(format!(
            "objdump failed: {}",
            String::from_utf8_lossy(&output.stderr).trim()
        ));
    }
    Ok(rows_from_output(
        &String::from_utf8_lossy(&output.stdout),
        base,
    ))
}
