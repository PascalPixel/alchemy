// The `fetch` handler: seven routes, in the source's order.

use crate::http::{header_block, Response};
use crate::jsonout::quote;
use crate::paths;
use crate::state;

/// `documentShell()`.
pub fn document_shell() -> String {
    let version = encode_uri_component(&state::page_version());
    format!(
        "<!doctype html>\
<meta charset=\"utf-8\">\
<meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">\
<title>Alchemy</title>\
<link rel=\"stylesheet\" href=\"/styles.css?v={version}\">\
<script type=\"module\" src=\"/client.js?v={version}\"></script>"
    )
}

/// `encodeURIComponent`.
///
/// PORT NOTE: the unreserved set is `A-Z a-z 0-9 - _ . ! ~ * ' ( )`. Every
/// other code point is percent-encoded from its UTF-8 bytes with UPPERCASE
/// hex. The value passed here is a `:`-joined pair of numbers, so only `:`
/// and, in the NaN case, nothing else is ever escaped -- but the full rule is
/// implemented because a partial one is the kind of thing that quietly drifts.
pub fn encode_uri_component(text: &str) -> String {
    let mut out = String::with_capacity(text.len());
    for byte in text.as_bytes() {
        let character = *byte as char;
        if character.is_ascii_alphanumeric() || "-_.!~*'()".contains(character) {
            out.push(character);
        } else {
            out.push_str(&format!("%{byte:02X}"));
        }
    }
    out
}

fn text_response(status: u16, reason: &'static str, body: &str, content_type: &str) -> Response {
    Response::new(
        status,
        reason,
        header_block(Some(content_type), "no-store"),
        body.as_bytes().to_vec(),
    )
}

/// Read a file for a route. A missing asset is served as an empty 200 response,
/// matching the dashboard's asset contract rather than becoming an error page.
fn file_body(path: &std::path::Path) -> Vec<u8> {
    std::fs::read(path).unwrap_or_default()
}

/// `/svg/(core|overlays|images|music)` -- the regex is anchored at both ends
/// and carries NO `i` flag, so `/svg/CORE` is a 404, not a panel.
pub fn svg_tree_id(path: &str) -> Option<&'static str> {
    let rest = path.strip_prefix("/svg/")?;
    state::TREE_LABELS
        .iter()
        .map(|(id, _)| *id)
        .find(|id| *id == rest)
}

pub fn respond(path: &str, method: &str) -> RouteOutcome {
    if method != "GET" && method != "HEAD" {
        // Dispatch is method-independent; the writer suppresses the body for
        // HEAD while preserving the response headers.
    }
    match path {
        "/" => RouteOutcome::Buffered(Response::new(
            200,
            "OK",
            header_block(Some("text/html; charset=utf-8"), "no-store"),
            document_shell().into_bytes(),
        )),
        "/styles.css" => RouteOutcome::Buffered(Response::new(
            200,
            "OK",
            header_block(Some("text/css; charset=utf-8"), "no-store"),
            file_body(&paths::styles()),
        )),
        "/client.js" => match crate::client::bundled_client() {
            Ok(source) => RouteOutcome::Buffered(Response::new(
                200,
                "OK",
                header_block(Some("text/javascript; charset=utf-8"), "no-store"),
                source.into_bytes(),
            )),
            Err(message) => RouteOutcome::Buffered(text_response(
                500,
                "Internal Server Error",
                &message,
                "text/plain; charset=utf-8",
            )),
        },
        "/snapshot" => RouteOutcome::Buffered(Response::new(
            200,
            "OK",
            // PORT NOTE: `Response.json` writes
            // `application/json;charset=utf-8` with NO space after the
            // semicolon, unlike every hand-written content type in this file.
            vec![
                ("Cache-Control".to_string(), "no-store".to_string()),
                (
                    "Content-Type".to_string(),
                    "application/json;charset=utf-8".to_string(),
                ),
            ],
            state::snapshot().stringify().into_bytes(),
        )),
        "/events" => RouteOutcome::EventStream,
        "/weyard.otf" => RouteOutcome::Buffered(Response::new(
            200,
            "OK",
            header_block(Some("font/otf"), "public, max-age=300"),
            file_body(&paths::font()),
        )),
        other => match svg_tree_id(other) {
            Some(tree) => {
                let svg = state::with_state(|current| {
                    current
                        .coverage
                        .as_ref()
                        .and_then(|live| {
                            live.trees
                                .iter()
                                .find(|(id, _)| *id == tree)
                                .map(|(_, svg)| svg.clone())
                        })
                        .ok_or_else(|| {
                            current
                                .scan_error
                                .clone()
                                .unwrap_or_else(|| "Coverage is still being read".to_string())
                        })
                });
                RouteOutcome::Buffered(match svg {
                    Ok(body) => Response::new(
                        200,
                        "OK",
                        header_block(Some("image/svg+xml; charset=utf-8"), "no-store"),
                        body.into_bytes(),
                    ),
                    Err(message) => text_response(
                        503,
                        "Service Unavailable",
                        &message,
                        "text/plain; charset=utf-8",
                    ),
                })
            }
            None => RouteOutcome::Buffered(text_response(
                404,
                "Not Found",
                "Not found",
                "text/plain; charset=utf-8",
            )),
        },
    }
}

pub enum RouteOutcome {
    Buffered(Response),
    EventStream,
}

/// Used by the self-test only; kept next to the shell it inspects.
pub fn shell_has_authored_markup() -> bool {
    document_shell().contains("<div id=\"root\"")
}

/// Small helper so the self-test's string checks read the same as the source's.
pub fn quoted(text: &str) -> String {
    quote(text)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn the_svg_route_pattern_is_anchored_and_case_sensitive() {
        assert_eq!(svg_tree_id("/svg/core"), Some("core"));
        assert_eq!(svg_tree_id("/svg/music"), Some("music"));
        // No `i` flag.
        assert_eq!(svg_tree_id("/svg/CORE"), None);
        // Anchored at the end: no trailing segment, no query smuggled in.
        assert_eq!(svg_tree_id("/svg/core/extra"), None);
        assert_eq!(svg_tree_id("/svg/coreish"), None);
        // Anchored at the start.
        assert_eq!(svg_tree_id("/x/svg/core"), None);
        assert_eq!(svg_tree_id("/svg/"), None);
    }

    #[test]
    fn encode_uri_component_matches_the_javascript_unreserved_set() {
        assert_eq!(
            encode_uri_component("1785756380214.9888:17857.5"),
            "1785756380214.9888%3A17857.5"
        );
        assert_eq!(encode_uri_component("-_.!~*'()"), "-_.!~*'()");
        assert_eq!(encode_uri_component(" /?&="), "%20%2F%3F%26%3D");
        // Uppercase hex, and UTF-8 bytes rather than code points.
        assert_eq!(encode_uri_component("é"), "%C3%A9");
    }

    #[test]
    fn the_document_shell_carries_no_authored_view_markup() {
        assert!(!shell_has_authored_markup());
        let shell = document_shell();
        assert!(shell.starts_with("<!doctype html>"));
        assert!(shell.contains("<title>Alchemy</title>"));
        assert!(shell.contains("/client.js?v="));
        assert!(shell.contains("/styles.css?v="));
        assert!(shell.ends_with("</script>"));
    }

    #[test]
    fn an_unknown_route_is_a_plain_text_404() {
        let RouteOutcome::Buffered(response) = respond("/nope", "GET") else {
            panic!("a 404 is buffered");
        };
        assert_eq!(response.status, 404);
        assert_eq!(response.body, b"Not found".to_vec());
        assert_eq!(
            response.headers,
            vec![
                (
                    "Content-Type".to_string(),
                    "text/plain; charset=utf-8".to_string()
                ),
                ("Cache-Control".to_string(), "no-store".to_string()),
            ]
        );
    }

    #[test]
    fn the_snapshot_content_type_has_no_space_after_its_semicolon() {
        let RouteOutcome::Buffered(response) = respond("/snapshot", "GET") else {
            panic!("buffered");
        };
        let content_type = response
            .headers
            .iter()
            .find(|(name, _)| name == "Content-Type")
            .map(|(_, value)| value.clone())
            .unwrap();
        assert_eq!(content_type, "application/json;charset=utf-8");
        // And Cache-Control comes FIRST here, unlike every other route.
        assert_eq!(response.headers[0].0, "Cache-Control");
    }

    #[test]
    fn quoting_is_reachable_from_the_route_layer() {
        assert_eq!(quoted("a"), "\"a\"");
    }
}
