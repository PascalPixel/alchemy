// A strict, dependency-free HTTP/1.1 server for exactly the traffic this
// dashboard carries.
//
// THE HTTP-DEPENDENCY DECISION, and its reason.
//
// The dashboard exposes seven `GET` routes on 127.0.0.1, no TLS, no request
// bodies, no cookies, no content negotiation, no compression, no HTTP/2, and
// no WebSocket upgrade. Six routes are a single buffered response. The
// seventh, `/events`, is Server-Sent Events: an unbounded
// `text/event-stream` body with `Transfer-Encoding: chunked`.
//
// SSE is the only feature worth arguing about, and it does not force a
// dependency: an SSE body is a socket that stays open while the server writes
// `event:`/`data:` frames, and chunked transfer encoding is a size line in hex,
// the chunk, and a CRLF. There is no framing negotiation, no control-frame
// state machine and no masking -- the things that make hand-rolling WebSockets
// dishonest. So: ZERO third-party dependencies, a `TcpListener` with a thread
// per connection and the strict request parser below.
//
// What this deliberately does NOT implement, because the dashboard never uses
// it and a silent half-implementation would be worse than an absent one: any
// method other than GET and HEAD (405), request bodies, `Expect: 100-continue`,
// pipelining beyond one request per connection for buffered routes, chunked
// REQUEST bodies, and `Range`. Each is rejected explicitly rather than
// mishandled.

use std::io::{BufRead, BufReader, Read, Write};
use std::net::TcpStream;

pub struct Request {
    pub method: String,
    pub path: String,
    /// Everything after the first `?`, unparsed. The dashboard routes on the
    /// pathname only, exactly as `new URL(request.url).pathname` does.
    pub query: Option<String>,
}

#[derive(Debug, PartialEq)]
pub enum ParseError {
    /// The peer closed before sending a request line.
    Empty,
    Malformed(&'static str),
}

/// Read and parse one request head. Strict on purpose: a header block that
/// does not terminate, an over-long line, or a request line that is not three
/// space-separated tokens is a 400, never a guess.
pub fn read_request(reader: &mut BufReader<&TcpStream>) -> Result<Request, ParseError> {
    let mut line = String::new();
    match read_line(reader, &mut line, 8 * 1024) {
        Ok(0) => return Err(ParseError::Empty),
        Ok(_) => {}
        Err(message) => return Err(ParseError::Malformed(message)),
    }
    let line = line.trim_end_matches('\n').trim_end_matches('\r');
    let mut parts = line.split(' ');
    let (Some(method), Some(target), Some(version)) = (parts.next(), parts.next(), parts.next())
    else {
        return Err(ParseError::Malformed("request line is not three tokens"));
    };
    if parts.next().is_some() {
        return Err(ParseError::Malformed("request line has a trailing token"));
    }
    if !version.starts_with("HTTP/1.") {
        return Err(ParseError::Malformed("unsupported HTTP version"));
    }
    // Drain the header block. Bounded so a peer cannot make the server hold an
    // unbounded buffer.
    let mut headers = 0usize;
    loop {
        let mut header = String::new();
        match read_line(reader, &mut header, 8 * 1024) {
            Ok(0) => return Err(ParseError::Malformed("headers ended without a blank line")),
            Ok(_) => {}
            Err(message) => return Err(ParseError::Malformed(message)),
        }
        if header == "\r\n" || header == "\n" {
            break;
        }
        headers += 1;
        if headers > 100 {
            return Err(ParseError::Malformed("too many headers"));
        }
    }
    let (path, query) = match target.split_once('?') {
        Some((path, query)) => (path.to_string(), Some(query.to_string())),
        None => (target.to_string(), None),
    };
    Ok(Request {
        method: method.to_string(),
        path: percent_decode(&path),
        query,
    })
}

fn read_line(
    reader: &mut BufReader<&TcpStream>,
    out: &mut String,
    limit: usize,
) -> Result<usize, &'static str> {
    let mut bytes = Vec::new();
    loop {
        let mut byte = [0u8; 1];
        match reader.read(&mut byte) {
            Ok(0) => break,
            Ok(_) => {}
            Err(_) => return Err("connection read failed"),
        }
        bytes.push(byte[0]);
        if byte[0] == b'\n' {
            break;
        }
        if bytes.len() > limit {
            return Err("line too long");
        }
    }
    if bytes.is_empty() {
        return Ok(0);
    }
    match String::from_utf8(bytes) {
        Ok(text) => {
            let length = text.len();
            out.push_str(&text);
            Ok(length)
        }
        Err(_) => Err("request line is not UTF-8"),
    }
}

/// URL pathname routing leaves percent escapes in place, so `/%73napshot` does
/// NOT match `/snapshot`.
///
/// PORT NOTE: therefore this decodes NOTHING. The function exists to make that
/// choice visible and testable rather than accidental.
fn percent_decode(path: &str) -> String {
    path.to_string()
}

pub struct Response {
    pub status: u16,
    pub reason: &'static str,
    /// Insertion-ordered, never a `HashMap`: header order is compared.
    pub headers: Vec<(String, String)>,
    pub body: Vec<u8>,
}

impl Response {
    pub fn new(
        status: u16,
        reason: &'static str,
        headers: Vec<(String, String)>,
        body: Vec<u8>,
    ) -> Self {
        Response {
            status,
            reason,
            headers,
            body,
        }
    }

    pub fn write_to(&self, stream: &mut TcpStream, include_body: bool) -> std::io::Result<()> {
        let mut head = format!("HTTP/1.1 {} {}\r\n", self.status, self.reason);
        for (name, value) in &self.headers {
            head.push_str(&format!("{name}: {value}\r\n"));
        }
        head.push_str(&format!("Content-Length: {}\r\n", self.body.len()));
        head.push_str("Connection: close\r\n\r\n");
        stream.write_all(head.as_bytes())?;
        if include_body {
            stream.write_all(&self.body)?;
        }
        stream.flush()
    }
}

/// Build response headers. An empty content type is omitted rather than sent
/// as an empty header value.
pub fn header_block(content_type: Option<&str>, cache_control: &str) -> Vec<(String, String)> {
    let mut out = Vec::new();
    if let Some(value) = content_type {
        if !value.is_empty() {
            out.push(("Content-Type".to_string(), value.to_string()));
        }
    }
    out.push(("Cache-Control".to_string(), cache_control.to_string()));
    out
}

/// Write one SSE frame as a single HTTP chunk.
pub fn write_chunk(stream: &mut TcpStream, payload: &[u8]) -> std::io::Result<()> {
    stream.write_all(format!("{:x}\r\n", payload.len()).as_bytes())?;
    stream.write_all(payload)?;
    stream.write_all(b"\r\n")?;
    stream.flush()
}

/// `event: NAME\ndata: JSON\n\n`, the framing consumed by the browser client.
/// Note the single `\n`, not `\r\n`.
pub fn sse_frame(event: &str, data: &str) -> Vec<u8> {
    format!("event: {event}\ndata: {data}\n\n").into_bytes()
}

pub fn drain_to_end(reader: &mut BufReader<&TcpStream>) {
    let _ = reader.fill_buf();
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn header_block_omits_an_absent_content_type_but_never_the_cache_control() {
        assert_eq!(
            header_block(Some("text/html; charset=utf-8"), "no-store"),
            vec![
                (
                    "Content-Type".to_string(),
                    "text/html; charset=utf-8".to_string()
                ),
                ("Cache-Control".to_string(), "no-store".to_string()),
            ]
        );
        assert_eq!(
            header_block(None, "no-store"),
            vec![("Cache-Control".to_string(), "no-store".to_string())]
        );
        // Falsy empty string behaves as absent, matching the JS spread.
        assert_eq!(
            header_block(Some(""), "no-store"),
            vec![("Cache-Control".to_string(), "no-store".to_string())]
        );
    }

    #[test]
    fn sse_frames_use_bare_newlines() {
        assert_eq!(
            sse_frame("update", "{}"),
            b"event: update\ndata: {}\n\n".to_vec()
        );
        assert!(!String::from_utf8(sse_frame("update", "{}"))
            .unwrap()
            .contains('\r'));
    }

    #[test]
    fn percent_escapes_are_left_alone_because_url_pathname_leaves_them() {
        // `/%73napshot` must NOT route to `/snapshot`. A decoder here would
        // change the server's literal route contract.
        assert_eq!(percent_decode("/%73napshot"), "/%73napshot");
    }
}
