use std::process::ExitCode;

fn main() -> ExitCode {
    let mut arguments = std::env::args().skip(1);
    let Some(group_name) = arguments.next() else {
        eprintln!("{}", dispatch::top_level_usage());
        return ExitCode::from(2);
    };
    let Some(group) = dispatch::Group::parse(&group_name) else {
        eprintln!("unknown dispatch group: {group_name}");
        eprintln!(
            "try one of: assets, check, compiler, decomp, make, metrics, overlay, search, semantic"
        );
        return ExitCode::from(2);
    };
    dispatch::run(group, &arguments.collect::<Vec<_>>())
}
