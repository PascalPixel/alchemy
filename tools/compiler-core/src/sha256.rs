use sha2::{Digest, Sha256};

pub fn hex(bytes: &[u8]) -> String {
    format!("{:x}", Sha256::digest(bytes))
}
