use sha1::{Digest, Sha1};

pub fn sha1_hex(data: &[u8]) -> String {
    format!("{:x}", Sha1::digest(data))
}

#[test]
fn hashes_with_standard_sha1() {
    assert_eq!(sha1_hex(b"abc"), "a9993e364706816aba3e25717850c26c9cd0d89d");
}
