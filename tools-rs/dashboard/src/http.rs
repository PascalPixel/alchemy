// A 120-line HTTP/1.1 server, because the dashboard needs three routes.
//
// dashboard_server.ts used Bun.serve, which brought a streaming SSE endpoint, a
// JSON snapshot endpoint, an on-demand TypeScript bundler, and a filesystem
// watcher along with it. None of that survives the port (see main.rs for the
// list), and what remains is: answer GET / with a document, GET /weyard.otf with
// a font, and everything else with 404. That is small enough that reaching for a
// web framework would add more crates than code.
//
// Deliberately not implemented: keep-alive (every response closes), chunked or
// compressed transfer, HEAD, request bodies, and Range. This binds to loopback
// and serves one developer looking at their own repo.

use std::io::{BufRead, BufReader, Read, Write};
use std::net::{TcpListener, TcpStream};

pub struct Request {
    pub method: String,
    pub path: String,
}

pub struct Response {
    pub status: u16,
    pub content_type: &'static str,
    pub cache_control: &'static str,
    pub body: Vec<u8>,
}

impl Response {
    pub fn ok(content_type: &'static str, body: Vec<u8>) -> Self {
        Self { status: 200, content_type, cache_control: "no-store", body }
    }

    pub fn cached(content_type: &'static str, body: Vec<u8>) -> Self {
        Self { status: 200, content_type, cache_control: "public, max-age=300", body }
    }

    pub fn not_found() -> Self {
        Self {
            status: 404,
            content_type: "text/plain; charset=utf-8",
            cache_control: "no-store",
            body: b"Not found".to_vec(),
        }
    }

    pub fn head(&self) -> String {
        let reason = match self.status {
            200 => "OK",
            404 => "Not Found",
            _ => "Error",
        };
        format!(
            "HTTP/1.1 {} {reason}\r\ncontent-type: {}\r\ncache-control: {}\r\ncontent-length: {}\r\nconnection: close\r\n\r\n",
            self.status,
            self.content_type,
            self.cache_control,
            self.body.len()
        )
    }
}

/// Read the request line and drain the headers. Returns None on a malformed or
/// empty request, which is what a browser's speculative connection looks like.
pub fn read_request(stream: &mut impl Read) -> Option<Request> {
    let mut reader = BufReader::new(stream);
    let mut line = String::new();
    reader.read_line(&mut line).ok()?;
    let mut parts = line.split_whitespace();
    let method = parts.next()?.to_string();
    let target = parts.next()?;
    // Strip the query string: the old client appended ?v=<revision> for cache
    // busting, and a stale bookmark should still resolve.
    let path = target.split(['?', '#']).next().unwrap_or("/").to_string();
    loop {
        let mut header = String::new();
        match reader.read_line(&mut header) {
            Ok(0) => break,
            Ok(_) if header == "\r\n" || header == "\n" => break,
            Ok(_) => continue,
            Err(_) => return None,
        }
    }
    Some(Request { method, path })
}

fn serve_one(mut stream: TcpStream, handler: &(dyn Fn(&Request) -> Response + Sync)) {
    let response = match read_request(&mut stream) {
        Some(request) if request.method == "GET" => handler(&request),
        Some(_) => Response::not_found(),
        None => return,
    };
    // A browser navigating away mid-write is normal; there is nobody to tell.
    let _ = stream.write_all(response.head().as_bytes());
    let _ = stream.write_all(&response.body);
    let _ = stream.flush();
}

/// Serve until the process is killed. Single-threaded on purpose: one viewer,
/// three routes, and responses that are a file read plus a string build.
pub fn serve(
    listener: TcpListener,
    handler: &(dyn Fn(&Request) -> Response + Sync),
) -> std::io::Result<()> {
    for stream in listener.incoming() {
        match stream {
            Ok(stream) => serve_one(stream, handler),
            Err(error) => eprintln!("dashboard: connection failed: {error}"),
        }
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    fn request_from(raw: &str) -> Option<Request> {
        let mut bytes = raw.as_bytes();
        read_request(&mut bytes)
    }

    #[test]
    fn parses_a_browser_request_and_drops_the_cache_busting_query() {
        let request = request_from(
            "GET /svg/core?v=94de7637 HTTP/1.1\r\nhost: localhost:4649\r\naccept: */*\r\n\r\n",
        )
        .expect("request parses");
        assert_eq!(request.method, "GET");
        assert_eq!(request.path, "/svg/core");
    }

    #[test]
    fn parses_a_bare_root_request() {
        let request = request_from("GET / HTTP/1.1\r\n\r\n").unwrap();
        assert_eq!(request.path, "/");
    }

    #[test]
    fn rejects_junk_and_empty_connections() {
        assert!(request_from("").is_none());
        assert!(request_from("GET\r\n\r\n").is_none());
        assert!(request_from("\x16\x03\x01").is_none());
    }

    #[test]
    fn response_heads_state_length_and_close() {
        let response = Response::ok("text/html; charset=utf-8", b"<!doctype html>".to_vec());
        let head = response.head();
        assert!(head.starts_with("HTTP/1.1 200 OK\r\n"));
        assert!(head.contains("content-length: 15\r\n"));
        assert!(head.contains("connection: close\r\n"));
        assert!(head.ends_with("\r\n\r\n"));
        assert!(Response::not_found().head().starts_with("HTTP/1.1 404 Not Found"));
        assert!(Response::cached("font/otf", vec![]).head().contains("max-age=300"));
    }

    #[test]
    fn a_real_socket_round_trips_a_document() {
        let listener = TcpListener::bind("127.0.0.1:0").unwrap();
        let port = listener.local_addr().unwrap().port();
        let thread = std::thread::spawn(move || {
            let stream = listener.incoming().next().unwrap().unwrap();
            serve_one(stream, &|request: &Request| {
                Response::ok("text/html; charset=utf-8", request.path.clone().into_bytes())
            });
        });
        let mut client = TcpStream::connect(("127.0.0.1", port)).unwrap();
        client.write_all(b"GET /trees?v=1 HTTP/1.1\r\nhost: x\r\n\r\n").unwrap();
        let mut reply = String::new();
        client.read_to_string(&mut reply).unwrap();
        thread.join().unwrap();
        assert!(reply.starts_with("HTTP/1.1 200 OK"));
        assert!(reply.ends_with("\r\n\r\n/trees"));
    }
}
