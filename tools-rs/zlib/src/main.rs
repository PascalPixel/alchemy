//! CLI entrypoint mirroring the TypeScript `tools/lib/zlib.ts --self-test`,
//! plus deflate/inflate filters used by the differential harness.
//!
//! PORT NOTE: the TypeScript exposes only `--self-test`. The `--deflate`,
//! `--inflate` and `--crc32` stdin/stdout filters are additions used to drive
//! the differential harness against the TypeScript; they change no behaviour.

use std::io::{Read, Write};

use alchemy_zlib::{crc32, deflate_sync, inflate_sync, DeflateOptions};

fn read_stdin() -> Vec<u8> {
    let mut buffer = Vec::new();
    std::io::stdin().read_to_end(&mut buffer).expect("read stdin");
    buffer
}

fn write_stdout(bytes: &[u8]) {
    let stdout = std::io::stdout();
    let mut handle = stdout.lock();
    handle.write_all(bytes).expect("write stdout");
    handle.flush().expect("flush stdout");
}

fn self_test() {
    let cases: Vec<Vec<u8>> = vec![
        Vec::new(),
        vec![0],
        vec![255],
        (0..256).map(|value| value as u8).collect(),
        vec![0x41; 70000],
        (0..200_000u64)
            .map(|value| ((value.wrapping_mul(2_654_435_761) & 0xffff_ffff) >> 24) as u8)
            .collect(),
        (0..5000u32).map(|value| (value % 7) as u8).collect(),
    ];
    for (case_index, data) in cases.iter().enumerate() {
        let compressed = deflate_sync(data, DeflateOptions::default());
        let restored = inflate_sync(&compressed).unwrap_or_else(|error| {
            panic!("deflate round-trip failed for case {case_index}: {error}")
        });
        assert!(
            restored == *data,
            "deflate round-trip failed for case {case_index} ({} bytes)",
            data.len()
        );
        let again = deflate_sync(data, DeflateOptions::default());
        assert!(again == compressed, "deflate is not deterministic for case {case_index}");
    }
    println!("self-test=ok tool=zlib cases={}", cases.len());
}

fn main() {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    if arguments.iter().any(|argument| argument == "--self-test") {
        self_test();
        return;
    }
    match arguments.first().map(String::as_str) {
        Some("--deflate") => write_stdout(&deflate_sync(&read_stdin(), DeflateOptions::default())),
        Some("--inflate") => match inflate_sync(&read_stdin()) {
            Ok(output) => write_stdout(&output),
            Err(error) => {
                eprintln!("inflate error: {error}");
                std::process::exit(2);
            }
        },
        Some("--crc32") => println!("{}", crc32(&read_stdin())),
        _ => {
            eprintln!("usage: alchemy-zlib [--self-test|--deflate|--inflate|--crc32]");
            std::process::exit(1);
        }
    }
}
