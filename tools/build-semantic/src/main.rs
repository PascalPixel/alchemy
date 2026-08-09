use build_semantic::{build_semantic, repository_root, self_test};

const USAGE: &str = "usage: build-semantic [--self-test]";

fn main() {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    if arguments.as_slice() == ["-h"] || arguments.as_slice() == ["--help"] {
        println!("{USAGE}");
        return;
    }
    if arguments.as_slice() == ["--self-test"] {
        self_test().unwrap_or_else(|message| fail(&message));
        println!("self-test=ok");
        return;
    }
    if !arguments.is_empty() {
        fail(USAGE);
    }
    let root = repository_root();
    let report = build_semantic(&root.join("semantic"))
        .unwrap_or_else(|message| fail(&message));
    println!(
        "semantic_sources={} semantic_bytes={} main_semantic={} overlay_semantic={} \
reviewed_bytes={} outside_executable={} c_expressed={}/{} remaining={} source_bytes={} compile=ok",
        report.sources,
        report.semantic_bytes,
        report.main_semantic_bytes,
        report.overlay_semantic_bytes,
        report.reviewed_bytes,
        report.outside_executable_bytes,
        report.expressed_bytes,
        report.executable_bytes,
        report.executable_bytes - report.expressed_bytes,
        report.source_bytes,
    );
}

fn fail(message: &str) -> ! {
    eprintln!("error: {message}");
    std::process::exit(1);
}

#[cfg(test)]
mod tests {
    use super::USAGE;

    #[test]
    fn usage_describes_the_only_non_default_mode() {
        assert!(USAGE.contains("--self-test"));
    }
}
