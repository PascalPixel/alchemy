use std::process::ExitCode;

pub fn entry(arguments: &[String]) -> ExitCode {
    if arguments
        .iter()
        .any(|argument| matches!(argument.as_str(), "-h" | "--help"))
    {
        println!("usage: check owners");
        return ExitCode::SUCCESS;
    }
    if !arguments.is_empty() {
        eprintln!("usage: check owners");
        return ExitCode::from(2);
    }
    match crate::validate() {
        Ok((unmatchable, provisional, drafts, audited, names)) => {
            println!("owner registers ok: {unmatchable} unmatchable, {provisional} provisional, {drafts} drafts, {audited} audited, {names} named main assembly owners");
            ExitCode::SUCCESS
        }
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}
