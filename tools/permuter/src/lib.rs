//! Native, bounded source permutation for Alchemy candidates.
//!
//! `permuter` carries the productive local workflow of pret's
//! decomp-permuter into Alchemy's native tooling layer: explicit `PERM_*`
//! alternatives, deterministic source mutation, parallel candidate
//! compilation, deduplication, and exact-byte scoring. A separate directory
//! backend retains the portable compile-script and instruction-scoring
//! boundary for non-Alchemy targets. It deliberately does not port
//! permuter@home networking.

pub mod astgate;
pub mod astpass;
pub mod asttypes;
pub mod astutil;
mod backend;
pub mod cemit;
mod compile;
mod options;
mod perm;
mod randomize;
pub mod roundtrip;
mod runner;

pub use options::{Options, USAGE};
pub use perm::{parse as parse_permutation, EvalState, Permutation};
pub use randomize::{mutate, Mutation, PassId, Weights};

pub fn run(args: Vec<String>) -> Result<(), String> {
    if args
        .iter()
        .any(|arg| matches!(arg.as_str(), "--help" | "-h"))
    {
        println!("{USAGE}");
        return Ok(());
    }
    if args.iter().any(|arg| arg == "--help=randomization-passes") {
        print!("{}", randomize::pass_help());
        return Ok(());
    }
    if args.iter().any(|arg| arg == "--self-test") {
        return self_test();
    }
    if let Some(pos) = args.iter().position(|arg| arg == "--roundtrip") {
        let path = args
            .get(pos + 1)
            .ok_or_else(|| "--roundtrip requires a file path".to_string())?;
        let source = std::fs::read_to_string(path).map_err(|e| format!("read {path}: {e}"))?;
        print!("{}", roundtrip::roundtrip(&source)?);
        return Ok(());
    }
    if let Some(pos) = args.iter().position(|arg| arg == "--ast-randomize") {
        let path = args
            .get(pos + 1)
            .ok_or_else(|| "--ast-randomize requires a file path".to_string())?;
        return astgate::run_ast_randomize(path, &args);
    }
    if let Some(pos) = args.iter().position(|arg| arg == "--ast-gate-determinism") {
        let path = args
            .get(pos + 1)
            .ok_or_else(|| "--ast-gate-determinism requires a file path".to_string())?;
        return astgate::gate_determinism(path);
    }
    if let Some(pos) = args.iter().position(|arg| arg == "--ast-gate-validity") {
        let path = args
            .get(pos + 1)
            .ok_or_else(|| "--ast-gate-validity requires a file path".to_string())?;
        return astgate::gate_validity(path, &args);
    }
    if let Some(pos) = args.iter().position(|arg| arg == "--ast-gate-reach") {
        let path = args
            .get(pos + 1)
            .ok_or_else(|| "--ast-gate-reach requires a file path".to_string())?;
        return astgate::gate_reach(path, &args);
    }
    let options = Options::parse(&args)?;
    runner::run(options)
}

fn self_test() -> Result<(), String> {
    backend::self_test()?;
    cemit::self_test()?;
    perm::self_test()?;
    randomize::self_test()?;
    options::self_test()?;
    compile::self_test()?;
    roundtrip::self_test()?;
    astgate::self_test()?;
    runner::self_test()?;
    println!("self-test=ok tool=permuter checks=9");
    Ok(())
}
