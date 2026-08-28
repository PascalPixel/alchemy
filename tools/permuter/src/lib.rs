mod acceptance;
mod compile;
mod options;
mod perm;
mod runner;

pub use options::{Options, USAGE};
pub use perm::{parse as parse_permutation, Permutation};

pub fn run(args: Vec<String>) -> Result<(), String> {
    if args
        .iter()
        .any(|argument| matches!(argument.as_str(), "-h" | "--help"))
    {
        println!("{USAGE}");
        return Ok(());
    }
    if args.iter().any(|argument| argument == "--acceptance-test") {
        return acceptance::run();
    }
    runner::run(Options::parse(&args)?).map(drop)
}
