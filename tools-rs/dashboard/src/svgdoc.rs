// Serve-time touch-ups to a generated coverage SVG.
//
// The box trees in assets/readme/<target>-<tree>.svg are the ground truth: they
// are the artifacts the README embeds and the checked-in record of coverage.
// This module must not change what they draw. It only does the two things the
// deleted browser client used to do to them after fetching:
//
//   1. add class="tree-image" to the root <svg> so the page CSS can size it, and
//   2. give every labelled group a hover label.
//
// The old client did (2) with a pointermove handler that read `aria-label` off
// `event.target.closest("g[aria-label]")` and positioned a floating <div>. With
// no script, the equivalent is an SVG <title> child, which every browser renders
// as a native tooltip. Same text, same nesting semantics (innermost wins), no
// JavaScript.

/// Extract an attribute value from a single start tag. Returns the raw, still
/// XML-escaped text.
///
/// PORT NOTE: this is a scanner, not an XML parser. It assumes no '>' appears
/// inside an attribute value, which holds because the generator's escapeText()
/// in tools/metrics/coverage_map.ts escapes '>' to "&gt;" in every value it
/// writes. If that ever stops being true this silently truncates a tag.
fn attribute<'a>(tag: &'a str, name: &str) -> Option<&'a str> {
    let mut at = 0;
    while let Some(rel) = tag[at..].find(name) {
        let start = at + rel;
        let before = tag[..start].chars().next_back();
        let after = &tag[start + name.len()..];
        if before.is_some_and(char::is_whitespace) && after.starts_with("=\"") {
            let value = &after[2..];
            return value.find('"').map(|end| &value[..end]);
        }
        at = start + name.len();
    }
    None
}

/// True when byte `at` in `svg` begins a start tag named `name`.
fn starts_tag(svg: &str, at: usize, name: &str) -> bool {
    let rest = match svg.get(at + 1..) {
        Some(rest) => rest,
        None => return false,
    };
    if !rest.starts_with(name) {
        return false;
    }
    matches!(
        rest[name.len()..].chars().next(),
        Some(c) if c.is_whitespace() || c == '>' || c == '/'
    )
}

/// Insert `<title>` as the first child of every group carrying an `aria-label`.
///
/// PORT NOTE: the label text is copied verbatim out of the attribute rather than
/// unescaped and re-escaped. Attribute-escaped text is already valid element
/// text (the generator escapes & < > " ' unconditionally), so a round trip would
/// only be an opportunity to get it wrong.
pub fn inject_titles(svg: &str) -> String {
    let mut out = String::with_capacity(svg.len() + svg.len() / 8);
    let mut at = 0;
    while let Some(rel) = svg[at..].find("<g") {
        let start = at + rel;
        if !starts_tag(svg, start, "g") {
            out.push_str(&svg[at..start + 2]);
            at = start + 2;
            continue;
        }
        let end = match svg[start..].find('>') {
            Some(offset) => start + offset,
            None => break,
        };
        out.push_str(&svg[at..=end]);
        if let Some(label) = attribute(&svg[start..=end], "aria-label") {
            out.push_str("<title>");
            out.push_str(label);
            out.push_str("</title>");
        }
        at = end + 1;
    }
    out.push_str(&svg[at..]);
    out
}

/// Add a class to the root `<svg>` start tag.
///
/// PORT NOTE: the client also overwrote the root aria-label with
/// `"<panel title> coverage graph"`. That is kept as the generator wrote it
/// instead: the SVG's own root label is a full sentence describing the band and
/// the leaf semantics, which is strictly better for a screen reader than the
/// four-word replacement.
pub fn with_root_class(svg: &str, class: &str) -> Result<String, String> {
    let trimmed = svg.trim_start();
    let offset = svg.len() - trimmed.len();
    if !starts_tag(trimmed, 0, "svg") || !trimmed.starts_with('<') {
        return Err("document does not start with an <svg> element".to_string());
    }
    let end = offset
        + trimmed
            .find('>')
            .ok_or_else(|| "unterminated <svg> start tag".to_string())?;
    Ok(format!(
        "{}{}{}",
        &svg[..end],
        format_args!(" class=\"{class}\""),
        &svg[end..]
    ))
}

/// Everything the page does to a generated tree before inlining it.
pub fn prepare(svg: &str) -> Result<String, String> {
    Ok(inject_titles(&with_root_class(svg, "tree-image")?))
}

#[cfg(test)]
mod tests {
    use super::*;

    /// A miniature of a real generated tree: root svg with its own aria-label,
    /// nested labelled groups, a <defs><style> block, and a <text> leaf.
    fn tree_fixture() -> String {
        concat!(
            "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 540 304\" ",
            "role=\"img\" aria-label=\"Music maturity box tree, orange band\">\n",
            "<defs><style>.weyard{font-family:Weyard}</style></defs>\n",
            "<rect x=\"0\" y=\"0\" width=\"540\" height=\"304\" fill=\"#c85d00\"/>\n",
            "<g aria-label=\"golden-sun-pcm-wave-series: 404,160 bytes\">\n",
            "<g aria-label=\"waves_index.json &amp; friends &lt;0x0812&gt;: 49,200 bytes\">\n",
            "<rect x=\"3\" y=\"22\" width=\"109\" height=\"110\"/>\n",
            "<text class=\"weyard\" x=\"6\" y=\"30\">waves_index.json</text>\n",
            "</g>\n</g>\n</svg>\n"
        )
        .to_string()
    }

    #[test]
    fn every_labelled_group_gains_a_native_tooltip() {
        let prepared = prepare(&tree_fixture()).expect("fixture prepares");
        assert_eq!(prepared.matches("<title>").count(), 2);
        assert!(prepared.contains(
            "<g aria-label=\"golden-sun-pcm-wave-series: 404,160 bytes\">\
             <title>golden-sun-pcm-wave-series: 404,160 bytes</title>"
        ));
    }

    #[test]
    fn escaped_label_text_survives_the_move_into_element_content() {
        let prepared = prepare(&tree_fixture()).unwrap();
        assert!(prepared
            .contains("<title>waves_index.json &amp; friends &lt;0x0812&gt;: 49,200 bytes</title>"));
        assert!(!prepared.contains("<title>waves_index.json & "));
    }

    #[test]
    fn the_root_svg_is_not_given_a_tooltip_of_its_own() {
        let prepared = prepare(&tree_fixture()).unwrap();
        let first_title = prepared.find("<title>").unwrap();
        let first_group = prepared.find("<g aria-label").unwrap();
        assert!(first_title > first_group, "a title escaped above the first group");
    }

    #[test]
    fn only_the_root_class_is_added_and_nothing_else_moves() {
        let source = tree_fixture();
        let prepared = prepare(&source).unwrap();
        assert!(prepared.starts_with(
            "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 540 304\" \
             role=\"img\" aria-label=\"Music maturity box tree, orange band\" \
             class=\"tree-image\">"
        ));
        // Stripping the additions must give back the original byte for byte.
        let round_trip = prepared
            .replace(" class=\"tree-image\">", ">")
            .replace("<title>golden-sun-pcm-wave-series: 404,160 bytes</title>", "")
            .replace(
                "<title>waves_index.json &amp; friends &lt;0x0812&gt;: 49,200 bytes</title>",
                "",
            );
        assert_eq!(round_trip, source);
    }

    #[test]
    fn unlabelled_groups_and_other_g_prefixed_tags_are_left_alone() {
        let source = "<svg><g><glyph d=\"M0\"/><g aria-label=\"x\"><rect/></g></g></svg>";
        let prepared = prepare(source).unwrap();
        assert_eq!(prepared.matches("<title>").count(), 1);
        assert!(prepared.contains("<glyph d=\"M0\"/>"));
        assert!(prepared.contains("<g><glyph"));
    }

    #[test]
    fn a_document_that_is_not_an_svg_is_refused() {
        assert!(prepare("<html><body>nope</body></html>").is_err());
        assert!(prepare("").is_err());
        assert!(prepare("<svgx foo=\"1\">").is_err());
    }

    #[test]
    fn attribute_lookup_does_not_match_a_suffix_of_another_attribute() {
        let tag = "<g data-aria-label=\"decoy\" aria-label=\"real\">";
        assert_eq!(attribute(tag, "aria-label"), Some("real"));
    }
}
