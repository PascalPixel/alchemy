use std::path::Path;
use std::process::ExitCode;

const USAGE: &str = "usage: po2ags CATALOG.po OUTPUT.inc [--label NAME]
       po2ags CATALOG.po OUTPUT.h --header [--as-value]
The first form writes the edition's message archive as assembly; the second
writes the C declarations of its messages: MSGNO(name) is a message's number,
and each name is its linker symbol. With --as-value each name also stands for
that symbol read as an s32 (☀️).";

/// The C declarations of an edition's messages.
fn header(names: &[(String, usize)], as_value: bool) -> String {
    let mut header = String::from(
        "/* Message numbers from the edition's editable PO catalog. */\n\
         #ifndef ALCHEMY_MESSAGE_IDS_H\n\
         #define ALCHEMY_MESSAGE_IDS_H\n\
         #include \"types.h\"\n\
         #define MSGNO(name) TEXT_MESSAGE_NUMBER_ ## name\n",
    );
    for (name, number) in names {
        header.push_str(&format!("#define TEXT_MESSAGE_NUMBER_{name} {number}\n"));
    }
    if as_value {
        header.push_str(
            "/* A message's name is its linker symbol, whose value is the message's\n\
             \x20* number. C can name a link-time value only as an address, so each\n\
             \x20* name below stands for that address read as an s32. A macro is not\n\
             \x20* expanded again inside its own replacement (C89 3.8.3.4): the name in\n\
             \x20* the cast is the symbol, which the code still loads from a relocated\n\
             \x20* pool word. */\n",
        );
    }
    for (name, _) in names {
        header.push_str(&format!("extern u8 {name}[];\n"));
        if as_value {
            header.push_str(&format!("#define {name} ((s32){name})\n"));
        }
    }
    header.push_str("#endif\n");
    header
}

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    if args.len() < 2 || args[0] == "-h" || args[0] == "--help" {
        eprintln!("{USAGE}");
        return ExitCode::FAILURE;
    }
    let flag = |name: &str| args.iter().any(|arg| arg == name);
    let label = args
        .iter()
        .position(|arg| arg == "--label")
        .and_then(|index| args.get(index + 1))
        .map_or("Text_Message", String::as_str);
    let result = ags::text::read_source(Path::new(&args[0]))
        .and_then(|source| {
            if flag("--header") {
                Ok(header(&source.names, flag("--as-value")))
            } else {
                ags::text::messages_include(&source, label, &args[0])
            }
        })
        .and_then(|text| {
            std::fs::write(&args[1], text).map_err(|error| format!("{}: {error}", args[1]))
        });
    match result {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("{error}");
            ExitCode::FAILURE
        }
    }
}
