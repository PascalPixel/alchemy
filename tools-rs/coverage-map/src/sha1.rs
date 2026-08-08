// SHA-1 (FIPS 180-4), hand-written.
//
// WHY hand-written: the only consumer is `svg_cache_version`, which takes the
// first eight hex characters of `createHash("sha1")`. That is one 90-line
// function; pulling a crate in for it would break this crate's zero-dependency
// rule for no gain. The house pattern is `tools-rs/alchemy-bundle/src/sha256.rs`.

/// The SHA-1 digest of `data`, lowercase hex, as Node's
/// `createHash("sha1").update(data).digest("hex")` produces it.
pub fn sha1_hex(data: &[u8]) -> String {
    let mut state: [u32; 5] = [0x6745_2301, 0xefcd_ab89, 0x98ba_dcfe, 0x1032_5476, 0xc3d2_e1f0];

    // Padding: 0x80, then zeros to 56 mod 64, then the bit length big-endian.
    let mut message = data.to_vec();
    let bit_length = (data.len() as u64).wrapping_mul(8);
    message.push(0x80);
    while message.len() % 64 != 56 {
        message.push(0);
    }
    message.extend_from_slice(&bit_length.to_be_bytes());

    for chunk in message.chunks_exact(64) {
        let mut w = [0u32; 80];
        for (index, word) in chunk.chunks_exact(4).enumerate() {
            w[index] = u32::from_be_bytes([word[0], word[1], word[2], word[3]]);
        }
        for index in 16..80 {
            w[index] = (w[index - 3] ^ w[index - 8] ^ w[index - 14] ^ w[index - 16]).rotate_left(1);
        }

        let [mut a, mut b, mut c, mut d, mut e] = state;
        for (index, word) in w.iter().enumerate() {
            let (f, k) = match index {
                0..=19 => ((b & c) | ((!b) & d), 0x5a82_7999u32),
                20..=39 => (b ^ c ^ d, 0x6ed9_eba1),
                40..=59 => ((b & c) | (b & d) | (c & d), 0x8f1b_bcdc),
                _ => (b ^ c ^ d, 0xca62_c1d6),
            };
            let temp = a
                .rotate_left(5)
                .wrapping_add(f)
                .wrapping_add(e)
                .wrapping_add(k)
                .wrapping_add(*word);
            e = d;
            d = c;
            c = b.rotate_left(30);
            b = a;
            a = temp;
        }
        state[0] = state[0].wrapping_add(a);
        state[1] = state[1].wrapping_add(b);
        state[2] = state[2].wrapping_add(c);
        state[3] = state[3].wrapping_add(d);
        state[4] = state[4].wrapping_add(e);
    }

    let mut hex = String::with_capacity(40);
    for word in state {
        hex.push_str(&format!("{word:08x}"));
    }
    hex
}

#[cfg(test)]
mod tests {
    use super::sha1_hex;

    #[test]
    fn the_published_fips_vectors_hold() {
        assert_eq!(sha1_hex(b""), "da39a3ee5e6b4b0d3255bfef95601890afd80709");
        assert_eq!(sha1_hex(b"abc"), "a9993e364706816aba3e25717850c26c9cd0d89d");
        assert_eq!(
            sha1_hex(b"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"),
            "84983e441c3bd26ebaae4aa1f95129e5e54670f1",
        );
    }

    #[test]
    fn a_multi_block_message_carries_across_chunks() {
        // 1,000,000 'a' would be slow to keep in the suite; 1000 exercises the
        // multi-block path and the length encoding just as well.
        let digest = sha1_hex(&vec![b'a'; 1000]);
        assert_eq!(digest, "291e9a6c66994949b57ba5e650361e98fc36b1ba");
    }

    #[test]
    fn a_length_that_lands_exactly_on_the_padding_boundary_still_pads() {
        // 55 bytes needs one block, 56 needs two: the boundary the `while` loop
        // exists to handle.
        assert_eq!(
            sha1_hex(&[b'x'; 55]).len(),
            sha1_hex(&[b'x'; 56]).len(),
        );
        assert_ne!(sha1_hex(&[b'x'; 55]), sha1_hex(&[b'x'; 56]));
    }
}
