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
    if args.iter().any(|argument| argument == "--self-test") {
        return self_test();
    }
    runner::run(Options::parse(&args)?)
}

fn self_test() -> Result<(), String> {
    options::self_test()?;
    perm::self_test()?;
    compile::self_test()?;
    runner::self_test()?;
    println!("self-test=ok tool=permuter checks=4");
    Ok(())
}
