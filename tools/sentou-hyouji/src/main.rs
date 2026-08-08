use sentou_hyouji::{build_sentou_hyouji, self_test, verify_sentou_hyouji, Error, ADDRESS, SIZE};
use std::path::Path;
use std::io::{self, Write};
use std::process::ExitCode;
const USAGE: &str =
    "usage: sentou_hyouji build INDEX --output FILE | verify ROM INDEX | --self-test";
fn main() -> ExitCode {
    let a: Vec<String> = std::env::args().skip(1).collect();
    let r = match a.as_slice() {
        [x] if x == "--self-test" => self_test().map(|_| println!("self-test=ok")),
        [cmd, index, flag, out] if cmd == "build" && flag == "--output" => {
            build_sentou_hyouji(Path::new(index)).and_then(|b| {
                std::fs::write(out, &b).map_err(|e| Error(e.to_string()))?;
                println!("address=0x{ADDRESS:08x} bytes={SIZE}");
                Ok(())
            })
        }
        [cmd, index] if cmd == "build-stdout" => {
            build_sentou_hyouji(Path::new(index)).and_then(|b| {
                io::stdout().write_all(&b).map_err(|e| Error(e.to_string()))
            })
        }
        [cmd, rom, index] if cmd == "verify" => std::fs::read(rom)
            .map_err(|e| Error(e.to_string()))
            .and_then(|r| verify_sentou_hyouji(&r, Path::new(index)))
            .map(|_| println!("address=0x{ADDRESS:08x} bytes={SIZE} exact=true")),
        [x] if x == "--help" => {
            println!("{USAGE}");
            Ok(())
        }
        _ => Err(Error(USAGE.into())),
    };
    if let Err(e) = r {
        eprintln!("error: {e}");
        ExitCode::FAILURE
    } else {
        ExitCode::SUCCESS
    }
}
