//! CLI for this crate, moved out of `main.rs` so the command can be linked
//! into a shared entry point instead of shipping its own executable.

use crate::{build_sentou_hyouji, self_test, verify_sentou_hyouji, Error, ADDRESS, SIZE};
use std::io::{self, Write};
use std::path::Path;
use std::process::ExitCode;
const USAGE: &str = "usage: sentou_hyouji build INDEX --output FILE | build-stdout INDEX | verify ROM INDEX | --self-test";

fn run(a: &[String]) -> Result<(), Error> {
    match a {
        [x] if x == "--self-test" => self_test().map(|_| println!("self-test=ok")),
        [cmd, index, flag, out] if cmd == "build" && flag == "--output" => build_sentou_hyouji(Path::new(index)).and_then(|b| {
            std::fs::write(out, &b).map_err(|e| Error(e.to_string()))?;
            println!("address=0x{ADDRESS:08x} bytes={SIZE}");
            Ok(())
        }),
        [cmd, index] if cmd == "build-stdout" => build_sentou_hyouji(Path::new(index)).and_then(|b| io::stdout().write_all(&b).map_err(|e| Error(e.to_string()))),
        [cmd, rom, index] if cmd == "verify" => std::fs::read(rom).map_err(|e| Error(e.to_string())).and_then(|r| verify_sentou_hyouji(&r, Path::new(index))).map(|_| println!("address=0x{ADDRESS:08x} bytes={SIZE} exact=true")),
        [x] if matches!(x.as_str(), "-h" | "--help") => {
            println!("{USAGE}");
            Ok(())
        }
        _ => Err(Error(USAGE.into())),
    }
}

pub fn entry(arguments: &[String]) -> std::process::ExitCode {
    let a: Vec<String> = arguments.to_vec();
    let r = run(&a);
    if let Err(e) = r {
        eprintln!("error: {e}");
        ExitCode::FAILURE
    } else {
        ExitCode::SUCCESS
    }
}
