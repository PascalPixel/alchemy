// Dependency-free native dashboard server and live coverage implementation.
//
// The library root keeps the server pieces testable while the binary provides
// the local HTTP service.

pub mod client;
pub mod clock;
pub mod events;
pub mod http;
pub mod jsonout;
pub mod paths;
pub mod routes;
pub mod selftest;
pub mod server;
pub mod state;
pub mod watch;
