//! Differential-parity dumper: prints one `key=value` line per observable for a
//! single overlay, so the harness can diff the Rust and TypeScript answers
//! field by field rather than on one opaque blob.
//!
//! Usage: `parity_dump <overlay.s> [--perturb]`
//!
//! `--perturb` is the NEGATIVE CONTROL: it flips exactly one bit of the
//! assembled image, so the harness can prove the comparison is actually capable
//! of reporting a difference before it is trusted to report none.

use std::path::Path;
use std::process::ExitCode;

use alchemy_bundle::sha256;
use overlay_disasm::compile::hex;
use overlay_disasm::{OverlaySource, OVERLAY_BASE};

fn main() -> ExitCode {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    let Some(path) = arguments.first() else {
        eprintln!("usage: parity_dump <overlay.s> [--perturb]");
        return ExitCode::FAILURE;
    };
    let perturb = arguments.iter().any(|argument| argument == "--perturb");
    if !Path::new(path).exists() {
        eprintln!("unreadable input: {path}");
        return ExitCode::FAILURE;
    }
    let source = OverlaySource::text(path.clone());

    let image = match overlay_disasm::assemble_overlay(&source, OVERLAY_BASE) {
        Ok(image) => image,
        Err(error) => {
            eprintln!("assemble_overlay: {error}");
            return ExitCode::FAILURE;
        }
    };
    let image = if perturb {
        // One byte, one place. Deliberately wrong.
        let mut copy = image.clone();
        if let Some(first) = copy.first_mut() {
            *first ^= 0x01;
        }
        copy
    } else {
        image
    };

    println!("image.len={}", image.len());
    println!("image.sha256={}", sha256::hex(&image));
    match overlay_disasm::call_via_bank_base(&image, OVERLAY_BASE) {
        Some(base) => println!("call_via_bank_base=0x{}", hex(base, 8)),
        None => println!("call_via_bank_base=null"),
    }
    let addresses = match overlay_disasm::overlay_c_addresses(&source) {
        Ok(addresses) => addresses,
        Err(error) => {
            eprintln!("overlay_c_addresses: {error}");
            return ExitCode::FAILURE;
        }
    };
    println!("overlay_c_addresses.count={}", addresses.len());
    println!(
        "overlay_c_addresses={}",
        addresses.iter().map(|a| hex(*a, 8)).collect::<Vec<_>>().join(",")
    );
    let spans = overlay_disasm::overlay_c_spans(&source, OVERLAY_BASE);
    println!("overlay_c_spans.count={}", spans.len());
    println!(
        "overlay_c_spans={}",
        spans
            .iter()
            .map(|span| format!("{}-{}", span.start, span.end))
            .collect::<Vec<_>>()
            .join(",")
    );

    let built = match overlay_disasm::build_overlay_source(&image, OVERLAY_BASE) {
        Ok(text) => text,
        Err(error) => {
            println!("build_overlay_source.error={error}");
            println!("build_overlay_source.sha256=none");
            return ExitCode::SUCCESS;
        }
    };
    println!("build_overlay_source.sha256={}", sha256::hex(built.as_bytes()));
    println!("build_overlay_source.lines={}", built.split('\n').count());
    println!(
        "build_overlay_source.labels={}",
        built.split('\n').filter(|line| line.starts_with(".L_")).count()
    );
    println!(
        "build_overlay_source.externals={}",
        built.split('\n').filter(|line| line.starts_with("\t.set sub_")).count()
    );
    println!(
        "build_overlay_source.code={}",
        built
            .split('\n')
            .filter(|line| line.starts_with('\t')
                && !line.starts_with("\t.set ")
                && !line.starts_with("\t.4byte")
                && !line.starts_with("\t.2byte")
                && !line.starts_with("\t.global")
                && !line.starts_with("\t.thumb"))
            .count()
    );
    println!(
        "build_overlay_source.data4={}",
        built.split('\n').filter(|line| line.starts_with("\t.4byte")).count()
    );
    println!(
        "build_overlay_source.data2={}",
        built.split('\n').filter(|line| line.starts_with("\t.2byte")).count()
    );
    ExitCode::SUCCESS
}
