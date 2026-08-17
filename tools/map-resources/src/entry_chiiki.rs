//! CLI moved out of a bin target so this command can be linked.

use crate::{run, SeriesKind};
use std::process::ExitCode;

pub fn entry(arguments: &[String]) -> ExitCode {
    match run(arguments.to_vec(), SeriesKind::Chiiki) {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}
