//! Adapter used by the aggregate `tools/check` binary.

use std::process::ExitCode;

pub fn entry(arguments: &[String]) -> ExitCode {
    crate::entry(arguments)
}
