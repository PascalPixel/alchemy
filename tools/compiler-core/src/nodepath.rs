//! POSIX `node:path` filename semantics retained by the compiler argv contract.

pub fn basename(path: &str) -> &str {
    path.trim_end_matches('/').rsplit('/').next().unwrap_or("")
}

pub fn extname(path: &str) -> &str {
    let name = basename(path);
    match name.rfind('.') {
        Some(index) if index > 0 && name != ".." => &name[index..],
        _ => "",
    }
}

#[test]
fn posix_names_preserve_dotfiles_unicode_and_trailing_slashes() {
    for (path, base, ext) in [
        ("", "", ""),
        ("///", "", ""),
        ("a/.", ".", ""),
        ("a/..//", "..", ""),
        ("a/...", "...", "."),
        ("a/.c", ".c", ""),
        ("a/..c", "..c", ".c"),
        ("a/.c.s/", ".c.s", ".s"),
        ("a/b..", "b..", "."),
        ("日本/場面.c//", "場面.c", ".c"),
    ] {
        assert_eq!((basename(path), extname(path)), (base, ext));
    }
    assert_eq!(crate::plan::inferred_preprocessed_output("a.c/"), "a..i");
}
