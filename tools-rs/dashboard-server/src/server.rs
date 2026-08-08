// `main()`: bind, serve, watch, keepalive.

use std::io::BufReader;
use std::net::{Ipv4Addr, SocketAddrV4, TcpListener, TcpStream};
use std::time::Duration;

use crate::http::{read_request, write_chunk, ParseError, Response};
use crate::routes::{respond, RouteOutcome};
use crate::{events, paths, state, watch};

/// Bind to 127.0.0.1 only. Never 0.0.0.0: a coverage dashboard reading a
/// private worktree has no business on another interface.
pub fn bind(port: Option<u16>) -> std::io::Result<TcpListener> {
    TcpListener::bind(SocketAddrV4::new(Ipv4Addr::LOCALHOST, port.unwrap_or(0)))
}

pub fn run() -> std::io::Result<()> {
    state::rebuild_coverage();
    let listener = bind(paths::port())?;
    let address = listener.local_addr()?;
    println!(
        "Alchemy dashboard on http://localhost:{}/ (live worktree coverage)",
        address.port()
    );

    std::thread::spawn(|| loop {
        std::thread::sleep(Duration::from_secs(5));
        events::keepalive();
    });

    let root = paths::root();
    std::thread::spawn(move || {
        let mut watcher = watch::Watcher::new(&root);
        loop {
            std::thread::sleep(watch::POLL);
            match watcher.tick() {
                watch::Tick::Idle => {}
                watch::Tick::Page => events::notify(),
                watch::Tick::Coverage => state::rebuild_coverage(),
                // `watch(file, () => process.exit(0))`.
                watch::Tick::Restart => std::process::exit(0),
            }
        }
    });

    for incoming in listener.incoming() {
        let Ok(stream) = incoming else { continue };
        std::thread::spawn(move || serve_connection(stream));
    }
    Ok(())
}

fn serve_connection(mut stream: TcpStream) {
    let _ = stream.set_nodelay(true);
    let request = {
        let mut reader = BufReader::new(&stream);
        read_request(&mut reader)
    };
    let request = match request {
        Ok(request) => request,
        Err(ParseError::Empty) => return,
        Err(ParseError::Malformed(reason)) => {
            let response = Response::new(
                400,
                "Bad Request",
                crate::http::header_block(Some("text/plain; charset=utf-8"), "no-store"),
                reason.as_bytes().to_vec(),
            );
            let _ = response.write_to(&mut stream, true);
            return;
        }
    };
    let include_body = request.method != "HEAD";
    match respond(&request.path, &request.method) {
        RouteOutcome::Buffered(response) => {
            let _ = response.write_to(&mut stream, include_body);
        }
        RouteOutcome::EventStream => serve_events(stream, include_body),
    }
}

fn serve_events(mut stream: TcpStream, include_body: bool) {
    let head = "HTTP/1.1 200 OK\r\n\
Content-Type: text/event-stream; charset=utf-8\r\n\
Cache-Control: no-store\r\n\
Connection: keep-alive\r\n\
Transfer-Encoding: chunked\r\n\r\n";
    if std::io::Write::write_all(&mut stream, head.as_bytes()).is_err() {
        return;
    }
    if !include_body {
        return;
    }
    let (id, receiver) = events::register();
    // `start(controller)` enqueues the current snapshot immediately.
    let first = crate::http::sse_frame("update", &state::snapshot().stringify());
    if write_chunk(&mut stream, &first).is_err() {
        events::unregister(id);
        return;
    }
    // `recv` ends when the sender side is dropped, which is the process
    // shutting down; a write failure is the browser closing the tab.
    while let Ok(message) = receiver.recv() {
        if write_chunk(&mut stream, &message).is_err() {
            break;
        }
    }
    events::unregister(id);
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::io::{Read, Write};

    fn get(port: u16, path: &str) -> String {
        let mut stream = TcpStream::connect(("127.0.0.1", port)).unwrap();
        stream.write_all(format!("GET {path} HTTP/1.1\r\nHost: x\r\n\r\n").as_bytes()).unwrap();
        let mut out = String::new();
        stream.read_to_string(&mut out).unwrap();
        out
    }

    #[test]
    fn the_server_binds_loopback_only_and_answers_a_404() {
        let listener = bind(None).unwrap();
        let port = listener.local_addr().unwrap().port();
        assert_eq!(listener.local_addr().unwrap().ip().to_string(), "127.0.0.1");
        std::thread::spawn(move || {
            for incoming in listener.incoming().take(3) {
                let Ok(stream) = incoming else { continue };
                serve_connection(stream);
            }
        });
        let response = get(port, "/nope");
        assert!(response.starts_with("HTTP/1.1 404 Not Found\r\n"), "{response}");
        assert!(response.contains("Content-Type: text/plain; charset=utf-8\r\n"));
        assert!(response.ends_with("Not found"));

        // A HEAD keeps the head and drops the body.
        let mut stream = TcpStream::connect(("127.0.0.1", port)).unwrap();
        stream.write_all(b"HEAD /nope HTTP/1.1\r\nHost: x\r\n\r\n").unwrap();
        let mut head = String::new();
        stream.read_to_string(&mut head).unwrap();
        assert!(head.contains("Content-Length: 9\r\n"), "{head}");
        assert!(head.ends_with("\r\n\r\n"), "{head}");

        // A malformed request line is a 400, never a guess.
        let mut stream = TcpStream::connect(("127.0.0.1", port)).unwrap();
        stream.write_all(b"NONSENSE\r\n\r\n").unwrap();
        let mut bad = String::new();
        stream.read_to_string(&mut bad).unwrap();
        assert!(bad.starts_with("HTTP/1.1 400 Bad Request\r\n"), "{bad}");
    }
}
