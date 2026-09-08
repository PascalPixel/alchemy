//! Shared local HTTP transport; each debugger owns its routes.
use std::{
    io::{BufRead, BufReader, Write},
    net::{SocketAddr, TcpListener, TcpStream},
    path::PathBuf,
    time::Duration,
};
pub fn root() -> PathBuf {
    compiler_core::routing::root().to_path_buf()
}
pub fn bind(args: &[String], command: &str, port: u16) -> Result<Option<SocketAddr>, String> {
    if args == ["--help"] || args == ["-h"] {
        println!("usage: alchemy {command} [--bind HOST:PORT]\nDefault: http://127.0.0.1:{port}/");
        return Ok(None);
    }
    let value = match args {
        [] => return Ok(Some(SocketAddr::from(([127, 0, 0, 1], port)))),
        [flag, value] if flag == "--bind" => value.as_str(),
        [value] if value.starts_with("--bind=") => &value[7..],
        _ => return Err(format!("usage: alchemy {command} [--bind HOST:PORT]")),
    };
    value
        .parse()
        .map(Some)
        .map_err(|_| format!("invalid bind address: {value}"))
}
pub struct Response {
    pub status: u16,
    pub reason: &'static str,
    pub headers: Vec<(&'static str, String)>,
    pub body: Vec<u8>,
}
impl Response {
    pub fn new(
        status: u16,
        reason: &'static str,
        ty: Option<&'static str>,
        cache: &'static str,
        body: impl Into<Vec<u8>>,
    ) -> Self {
        let mut headers = vec![("Cache-Control", cache.into())];
        if let Some(ty) = ty {
            headers.insert(0, ("Content-Type", ty.into()))
        }
        Self {
            status,
            reason,
            headers,
            body: body.into(),
        }
    }
    fn write(self, stream: &mut TcpStream, body: bool) -> std::io::Result<()> {
        let mut h = format!("HTTP/1.1 {} {}\r\n", self.status, self.reason);
        for (k, v) in self.headers {
            h.push_str(&format!("{k}: {v}\r\n"))
        }
        h.push_str(&format!(
            "Content-Length: {}\r\nConnection: close\r\n\r\n",
            self.body.len()
        ));
        stream.write_all(h.as_bytes())?;
        if body {
            stream.write_all(&self.body)?
        }
        Ok(())
    }
}
fn request(stream: &TcpStream) -> Result<(String, String), &'static str> {
    let mut r = BufReader::new(stream);
    let mut line = String::new();
    if r.read_line(&mut line).map_err(|_| "read failed")? == 0 {
        return Err("empty request");
    }
    let parts = line.trim_end().split(' ').collect::<Vec<_>>();
    if parts.len() != 3 {
        return Err("bad request");
    }
    let method = parts[0].to_string();
    let target = parts[1].to_string();
    let version = parts[2];
    if !version.starts_with("HTTP/1.") {
        return Err("bad request");
    }
    loop {
        line.clear();
        if r.read_line(&mut line).map_err(|_| "read failed")? == 0 {
            return Err("headers incomplete");
        }
        if line == "\r\n" || line == "\n" {
            break;
        }
    }
    Ok((method, target.split('?').next().unwrap_or(&target).into()))
}

pub fn shell(title: &str, styles: &str) -> Response {
    Response::new(200, "OK", Some("text/html; charset=utf-8"), "no-store",
        format!("<!doctype html><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width,initial-scale=1\"><title>{title}</title><style>{styles}</style><script type=\"module\" src=\"/client.js\"></script>"))
}
pub fn font() -> Response {
    match std::fs::read(root().join("games/gs1/assets/fonts/weyard.otf")) {
        Ok(bytes) => Response::new(200, "OK", Some("font/otf"), "no-store", bytes),
        Err(_) => not_found(),
    }
}
pub fn not_found() -> Response {
    Response::new(404, "Not Found", None, "no-store", b"Not found".to_vec())
}
pub fn run(listener: TcpListener, response: fn(&str) -> Response) -> std::io::Result<()> {
    for stream in listener.incoming() {
        let stream = stream?;
        std::thread::spawn(move || serve(stream, response));
    }
    Ok(())
}
fn serve(mut stream: TcpStream, response: fn(&str) -> Response) {
    let _ = stream.set_read_timeout(Some(Duration::from_secs(5)));
    let _ = stream.set_write_timeout(Some(Duration::from_secs(10)));
    let Ok((method, path)) = request(&stream) else {
        return;
    };
    let reply = if method == "GET" || method == "HEAD" {
        response(&path)
    } else {
        Response::new(
            405,
            "Method Not Allowed",
            None,
            "no-store",
            b"Method not allowed".to_vec(),
        )
    };
    let _ = reply.write(&mut stream, method != "HEAD");
}
#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn invalid_bind_never_falls_back() {
        assert!(bind(&["--bind".into(), "nonsense".into()], "dashboard", 4650).is_err());
        assert!(bind(&["--bind".into()], "dashboard", 4650).is_err());
        assert_eq!(
            bind(&[], "dashboard", 4650).unwrap().unwrap().to_string(),
            "127.0.0.1:4650"
        );
    }
}
