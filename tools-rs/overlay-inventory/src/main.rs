use std::env;
use std::path::PathBuf;

fn main() {
    let manifest_dir = env!("CARGO_MANIFEST_DIR");
    let root = overlay_inventory::root_from_exe(manifest_dir);
    let args: Vec<String> = env::args().skip(1).collect();
    let _ = PathBuf::new();
    if let Err(error) = overlay_inventory::run(&root, &args) {
        eprintln!("{error}");
        std::process::exit(1);
    }
}
