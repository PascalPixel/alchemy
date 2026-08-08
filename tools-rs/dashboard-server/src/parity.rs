// The differential-parity normaliser.
//
// Two servers reading the same worktree agree on almost everything. The
// handful of things they cannot agree on are genuinely time- and
// process-dependent, and each is erased here by an EXPLICIT, NARROW rule.
//
// The danger of a normaliser is that it quietly erases a real difference. So
// every rule below is anchored to a specific key or shape, none of them is a
// blanket "drop anything numeric", and the test at the foot of this file
// proves that a message WITHOUT the varying part passes through byte for byte.

/// Replace the value of a JSON string key in place: `"key":"..."` becomes
/// `"key":"<key>"`.
fn erase_string_value(text: &str, key: &str) -> String {
    let needle = format!("\"{key}\":\"");
    let mut out = String::with_capacity(text.len());
    let mut rest = text;
    while let Some(at) = rest.find(&needle) {
        out.push_str(&rest[..at + needle.len()]);
        let tail = &rest[at + needle.len()..];
        // JSON strings here never contain an escaped quote; the values are an
        // ISO instant and a colon-joined number pair.
        match tail.find('"') {
            Some(end) => {
                out.push_str(key);
                rest = &tail[end..];
            }
            None => {
                rest = tail;
                break;
            }
        }
    }
    out.push_str(rest);
    out
}

/// Replace a `?v=` cache-busting query value in an HTML attribute.
///
/// The value is the page version: a colon-joined pair of JS numbers, which
/// `encodeURIComponent` renders using only digits, `.`, `-`, `e` and `%3A`.
/// The match is REQUIRED to be entirely from that alphabet and to end at a
/// double quote, because `client.js` also contains the literal `?v=` inside a
/// template string (`/svg/${tree}?v=${encodeURIComponent(revision)}`) and a
/// looser rule silently deleted 181 characters of the served client. That was
/// caught by the negative control, and it is exactly the failure mode a
/// normaliser is dangerous for, so the case is pinned in the tests below.
fn erase_page_version(text: &str) -> String {
    let mut out = String::with_capacity(text.len());
    let mut rest = text;
    while let Some(at) = rest.find("?v=") {
        out.push_str(&rest[..at + 3]);
        let tail = &rest[at + 3..];
        let width = tail
            .find(|c: char| !(c.is_ascii_digit() || ".-e%ABCDEF".contains(c)))
            .unwrap_or(tail.len());
        if width > 0 && tail[width..].starts_with('"') {
            out.push_str("PAGEVERSION");
            rest = &tail[width..];
        } else {
            rest = tail;
        }
    }
    out.push_str(rest);
    out
}

/// Replace `localhost:NNNNN` and `127.0.0.1:NNNNN` port numbers.
fn erase_ports(text: &str) -> String {
    let mut out = String::with_capacity(text.len());
    let bytes = text.as_bytes();
    let mut index = 0;
    while index < bytes.len() {
        let rest = &text[index..];
        let hit = ["localhost:", "127.0.0.1:"]
            .iter()
            .find(|host| rest.starts_with(**host))
            .copied();
        match hit {
            Some(host) if rest[host.len()..].starts_with(|c: char| c.is_ascii_digit()) => {
                out.push_str(host);
                out.push_str("PORT");
                index += host.len();
                while index < bytes.len() && bytes[index].is_ascii_digit() {
                    index += 1;
                }
            }
            _ => {
                let character = rest.chars().next().unwrap();
                out.push(character);
                index += character.len_utf8();
            }
        }
    }
    out
}

/// Replace an absolute path into this checkout with `<ROOT>`.
fn erase_root(text: &str, root: &str) -> String {
    if root.is_empty() {
        return text.to_string();
    }
    text.replace(root, "<ROOT>")
}

/// The full normalisation, applied to a response body before comparison.
pub fn normalise(text: &str, root: &str) -> String {
    let text = erase_string_value(text, "generatedAt");
    let text = erase_string_value(&text, "page");
    let text = erase_page_version(&text);
    let text = erase_ports(&text);
    erase_root(&text, root)
}

#[cfg(test)]
mod tests {
    use super::*;

    /// The load-bearing test: nothing varying, nothing changed.
    #[test]
    fn a_message_without_any_varying_part_passes_through_untouched() {
        let samples = [
            "{\"revision\":\"abc\",\"scanning\":false,\"summary\":{\"exactBytes\":317066}}",
            "<!doctype html><title>Alchemy</title>",
            "Not found",
            "<svg xmlns=\"http://www.w3.org/2000/svg\"><text>MAIN IMAGE</text></svg>",
            "",
            // A `?v=` that is JavaScript, not an HTML attribute value.
            "const r = await fetch(`/svg/${tree}?v=${encodeURIComponent(rev)}`);\nconst x = 1;",
            // Digits next to a host name that is NOT followed by a port.
            "localhost:/x and 127.0.0.1 alone",
            // A key whose NAME merely contains a normalised key.
            "{\"notgeneratedAtAll\":1,\"pagerduty\":2}",
        ];
        for sample in samples {
            assert_eq!(
                normalise(sample, "/Users/someone/alchemy"),
                sample,
                "normaliser altered a message with nothing to normalise"
            );
        }
    }

    #[test]
    fn each_varying_part_is_erased_and_only_that_part() {
        let before = "{\"page\":\"1785756380214.9888:17857.5\",\"revision\":\"abc\",\
\"generatedAt\":\"2026-08-07T09:00:00.000Z\",\"summary\":{\"exactBytes\":317066}}";
        assert_eq!(
            normalise(before, ""),
            "{\"page\":\"page\",\"revision\":\"abc\",\"generatedAt\":\"generatedAt\",\
\"summary\":{\"exactBytes\":317066}}"
        );
        assert_eq!(
            normalise("<link href=\"/styles.css?v=17857.5%3A1785\">", ""),
            "<link href=\"/styles.css?v=PAGEVERSION\">"
        );
        assert_eq!(
            normalise("http://localhost:48651/ and 127.0.0.1:9 too", ""),
            "http://localhost:PORT/ and 127.0.0.1:PORT too"
        );
        assert_eq!(normalise("/a/b/tools/x", "/a/b"), "<ROOT>/tools/x");
    }

    /// A normaliser that erased a byte count would make parity meaningless.
    #[test]
    fn the_normaliser_does_not_touch_the_numbers_parity_actually_checks() {
        let body = "{\"summary\":{\"exactBytes\":317066,\"exactPercent\":21.5,\
\"combinedBytes\":400000}}";
        assert_eq!(normalise(body, "/x"), body);
    }
}
