use crate::{
    cli::{options_of, ParseOutcome, USAGE},
    diff::self_test,
    render::render,
};
use compiler_core::routing::root;
pub fn entry(arguments: &[String]) {
    if arguments.iter().any(|arg| arg == "--self-test") {
        match self_test() {
            Ok(line) => println!("{line}"),
            Err(error) => fail(&error),
        }
        return;
    }
    match options_of(root(), arguments) {
        Ok(ParseOutcome::Help) => println!("{USAGE}"),
        Ok(ParseOutcome::Options(options)) => match render(root(), &options) {
            Ok(output) => print!("{}", output.stdout),
            Err(error) => fail(&error),
        },
        Err(error) => fail(&error),
    }
}
fn fail(message: &str) -> ! {
    eprintln!("{message}");
    std::process::exit(1)
}
