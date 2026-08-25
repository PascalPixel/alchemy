const BUNDLE: &str = include_str!("client.js");

pub fn bundled_client() -> Result<String, String> {
    (!BUNDLE.is_empty())
        .then(|| BUNDLE.to_string())
        .ok_or_else(|| "dashboard client build failed".into())
}
