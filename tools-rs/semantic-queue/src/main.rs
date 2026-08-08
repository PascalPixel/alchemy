use std::path::Path;

use semantic_queue::{format_row, js_parse_int, js_slice_limit, semantic_queue, to_json};

/// PORT NOTE: `selfTest()` in the TypeScript is a smoke test that duplicates
/// assertions now held by `#[cfg(test)] mod tests`. The flag is kept so the
/// binary mirrors the CLI surface exactly, and it re-checks the same four
/// counters against the same fixtures.
fn self_test() {
    use semantic_queue::matchers::*;
    let draft: Vec<char> = "M2C_UNK Func_08001234(void);\nM2C_ERROR(/* r0 */);\n"
        .chars()
        .collect();
    if count_unknown_types(&draft) != 1 {
        panic!("unknown-type count is wrong");
    }
    if count_unset_registers(&draft) != 1 {
        panic!("unset-register count is wrong");
    }
    let assembly: Vec<char> = "  bl Func_08001234\n  b.n Func_08005678\n".chars().collect();
    if count_calls(&assembly) != 1 {
        panic!("call count is wrong");
    }
    if count_internal_exits(&assembly) != 1 {
        panic!("internal-exit count is wrong");
    }
    println!("self-test=ok");
}

fn main() {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    if arguments.iter().any(|argument| argument == "--self-test") {
        self_test();
        return;
    }

    let root = Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .unwrap()
        .parent()
        .unwrap();
    let queue = semantic_queue(root);

    if arguments.iter().any(|argument| argument == "--json") {
        println!("{}", to_json(&queue));
        return;
    }

    let limit = match arguments
        .iter()
        .find(|argument| argument.starts_with("--limit="))
    {
        None => 30usize,
        Some(argument) => js_slice_limit(queue.len(), js_parse_int(&argument["--limit=".len()..])),
    };
    let limit = limit.min(queue.len());
    for item in &queue[..limit] {
        println!("{}", format_row(item));
    }
    println!(
        "queued={} bytes={}",
        queue.len(),
        queue.iter().map(|item| item.bytes).sum::<i64>()
    );
}
