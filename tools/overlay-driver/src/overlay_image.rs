// Overlay images come from the single native implementation shared by all
// overlay tools. `overlay-call-targets` delegates assembly and exact-C splicing
// to `overlay-disasm`, so this remains the same authoritative image without a
// Bun sidecar or a second implementation.

use std::path::Path;

/// Assembles each named overlay. The result vector is positionally aligned with
/// `names`; `Err` carries the native assembler/compiler message for that name.
pub fn overlay_images(_root: &Path, names: &[String]) -> Vec<Result<Vec<u8>, String>> {
    names
        .iter()
        .map(|name| overlay_call_targets::overlay_image(name))
        .collect()
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn errors_stay_positionally_aligned() {
        let names = vec!["resource_ffffff".to_string(), "resource_ffffff".to_string()];
        let results = overlay_images(Path::new("/ignored"), &names);
        assert_eq!(results.len(), 2);
        assert_eq!(
            results[0],
            Err("no reconstruction assembly for resource_ffffff".to_string())
        );
        assert_eq!(results[1], results[0]);
    }

    #[test]
    fn an_empty_request_does_no_work() {
        assert!(overlay_images(Path::new("/nonexistent"), &[]).is_empty());
    }
}
