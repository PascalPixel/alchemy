//! Shared local HTTP transport; each debugger owns its routes.
use std::{
    io::{BufRead, BufReader, Write},
    net::{SocketAddr, TcpListener, TcpStream},
    path::PathBuf,
    time::Duration,
};
pub fn root() -> PathBuf {
    crate::compiler::routing::root().to_path_buf()
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
fn request(stream: impl std::io::Read) -> Result<(String, String, bool), &'static str> {
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
    let mut action = false;
    let mut origin = None;
    let mut host = None;
    loop {
        line.clear();
        if r.read_line(&mut line).map_err(|_| "read failed")? == 0 {
            return Err("headers incomplete");
        }
        if line == "\r\n" || line == "\n" {
            break;
        }
        if let Some((name, value)) = line.split_once(':') {
            action |= name.eq_ignore_ascii_case("X-Alchemy-Action") && value.trim() == "1";
            if name.eq_ignore_ascii_case("Origin") {
                origin = Some(value.trim().to_string());
            }
            if name.eq_ignore_ascii_case("Host") {
                host = Some(value.trim().to_string());
            }
        }
    }
    let action = action
        || (method == "POST"
            && origin.zip(host).is_some_and(|(origin, host)| {
                origin.eq_ignore_ascii_case(&format!("http://{host}"))
            }));
    Ok((
        method,
        target.split('?').next().unwrap_or(&target).into(),
        action,
    ))
}

pub fn not_found() -> Response {
    Response::new(404, "Not Found", None, "no-store", b"Not found".to_vec())
}
/// Browser actions require POST and either the explicit header or an HTML form
/// with an Origin matching Host. Cross-origin forms and CORS preflight fail.
pub fn run(
    listener: TcpListener,
    response: fn(&str) -> Response,
    action: fn(&str) -> Response,
) -> std::io::Result<()> {
    for stream in listener.incoming() {
        let stream = stream?;
        std::thread::spawn(move || serve(stream, response, action));
    }
    Ok(())
}
fn serve(mut stream: TcpStream, response: fn(&str) -> Response, action: fn(&str) -> Response) {
    let _ = stream.set_read_timeout(Some(Duration::from_secs(5)));
    let _ = stream.set_write_timeout(Some(Duration::from_secs(10)));
    let Ok((method, path, explicit_action)) = request(&stream) else {
        return;
    };
    let reply = route(&method, &path, explicit_action, response, action);
    let _ = reply.write(&mut stream, method != "HEAD");
}
fn route(
    method: &str,
    path: &str,
    explicit_action: bool,
    response: fn(&str) -> Response,
    action: fn(&str) -> Response,
) -> Response {
    if method == "GET" || method == "HEAD" {
        response(path)
    } else if method == "POST" && explicit_action {
        action(path)
    } else {
        Response::new(
            405,
            "Method Not Allowed",
            None,
            "no-store",
            b"Method not allowed".to_vec(),
        )
    }
}
#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn native_forms_require_same_origin_post() {
        for (method, origin, allowed) in [
            ("POST", "http://127.0.0.1:4650", true),
            ("POST", "http://example.com", false),
            ("POST", "null", false),
            ("GET", "http://127.0.0.1:4650", false),
        ] {
            let text=format!("{method} /reveal/ART%2Fsheet.PNG HTTP/1.1\r\nHost: 127.0.0.1:4650\r\nOrigin: {origin}\r\n\r\n");
            assert_eq!(request(text.as_bytes()).unwrap().2, allowed);
        }
        assert!(
            !request(&b"POST /reveal/file HTTP/1.1\r\nHost: localhost\r\n\r\n"[..])
                .unwrap()
                .2
        );
    }
    #[test]
    fn browser_actions_require_post_and_explicit_header() {
        fn read_route(_: &str) -> Response {
            Response::new(404, "Not Found", None, "no-store", b"read".to_vec())
        }
        fn action_route(path: &str) -> Response {
            Response::new(200, "OK", None, "no-store", path.as_bytes().to_vec())
        }
        for (method, header, status) in [
            ("GET", true, 404),
            ("HEAD", true, 404),
            ("POST", false, 405),
            ("OPTIONS", true, 405),
            ("POST", true, 200),
        ] {
            let request_text = format!(
                "{method} /reveal/ART%2Fsheet.PNG HTTP/1.1\r\n{}\r\n",
                if header {
                    "X-Alchemy-Action: 1\r\n"
                } else {
                    ""
                }
            );
            let (method, path, explicit) = request(request_text.as_bytes()).unwrap();
            let reply = route(&method, &path, explicit, read_route, action_route);
            assert_eq!(reply.status, status);
            if status == 200 {
                assert_eq!(reply.body, b"/reveal/ART%2Fsheet.PNG");
            }
        }
    }
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
