
use std::collections::HashMap;
use std::fs;
use std::io::Write;
use std::path::Path;
use std::sync::Mutex;

type Items = Vec<(String, Vec<u8>)>;

pub struct Cache {
    entries: Mutex<HashMap<String, Items>>,
    file: Mutex<fs::File>,
}

fn hex(bytes: &[u8]) -> String {
    bytes.iter().map(|byte| format!("{byte:02x}")).collect()
}

fn unhex(text: &str) -> Option<Vec<u8>> {
    (0..text.len())
        .step_by(2)
        .map(|at| u8::from_str_radix(text.get(at..at + 2)?, 16).ok())
        .collect()
}

fn parse(line: &str) -> Option<(String, Items)> {
    let mut fields = line.split('\t');
    let key = fields.next()?.to_string();
    let items = fields
        .map(|field| {
            let (kind, value) = field.split_once('=')?;
            Some((kind.to_string(), unhex(value)?))
        })
        .collect::<Option<Items>>()?;
    Some((key, items))
}

impl Cache {
    pub fn open(path: &Path) -> Result<Self, String> {
        let failed = |error: std::io::Error| format!("{}: {error}", path.display());
        if let Some(parent) = path.parent() {
            fs::create_dir_all(parent).map_err(failed)?;
        }
        let file = fs::OpenOptions::new()
            .create(true)
            .append(true)
            .open(path)
            .map_err(failed)?;
        let text = fs::read_to_string(path).map_err(failed)?;
        if !text.is_empty() && !text.ends_with('\n') {
            (&file).write_all(b"\n").map_err(failed)?;
        }
        let entries = text
            .split_inclusive('\n')
            .filter_map(|line| parse(line.strip_suffix('\n')?))
            .collect();
        Ok(Self {
            entries: Mutex::new(entries),
            file: Mutex::new(file),
        })
    }

    pub fn get(&self, key: &str) -> Result<Option<Items>, String> {
        let entries = self.entries.lock().map_err(|_| "cache poisoned")?;
        Ok(entries.get(key).filter(|items| !items.is_empty()).cloned())
    }

    pub fn put(&self, key: &str, items: &[(&str, &[u8])]) -> Result<(), String> {
        let plain = |text: &str| !text.is_empty() && !text.contains(['\t', '\n', '=']);
        if !plain(key) || !items.iter().all(|(kind, _)| plain(kind)) {
            return Err(format!("cache key or kind is not plain text: {key}"));
        }
        let mut line = key.to_string();
        for (kind, value) in items {
            line.push_str(&format!("\t{kind}={}", hex(value)));
        }
        line.push('\n');
        let mut file = self.file.lock().map_err(|_| "cache poisoned")?;
        file.write_all(line.as_bytes())
            .map_err(|error| error.to_string())?;
        let stored = items
            .iter()
            .map(|(kind, value)| (kind.to_string(), value.to_vec()))
            .collect();
        self.entries
            .lock()
            .map_err(|_| "cache poisoned")?
            .insert(key.to_string(), stored);
        Ok(())
    }
}
