// A dependency-free Rust port of `tools/metrics/dashboard_server.ts`.
//
// A library root as well as a binary so the parity harness, the tests and any
// later port can reach the pieces; nothing is suppressed to achieve it.

pub mod client;
pub mod clock;
pub mod events;
pub mod http;
pub mod jsonout;
pub mod parity;
pub mod paths;
pub mod routes;
pub mod selftest;
pub mod server;
pub mod state;
pub mod watch;
