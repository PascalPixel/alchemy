// The SSE client registry: `eventClients`, `sendEvent`, `notify` and the
// keepalive interval.
//
// Each client is a channel to its own connection thread, and a send failure
// removes it. `Vec` preserves insertion order, which is also the delivery
// order for update events.

use std::sync::mpsc::{Receiver, SyncSender, TrySendError};
use std::sync::Mutex;

use crate::http::sse_frame;

pub struct Client {
    pub id: u64,
    sender: SyncSender<Vec<u8>>,
}

static CLIENTS: Mutex<Vec<Client>> = Mutex::new(Vec::new());
static NEXT_ID: Mutex<u64> = Mutex::new(1);

pub fn register() -> (u64, Receiver<Vec<u8>>) {
    // A bounded channel keeps a client that stops reading from growing the
    // server without limit. A wedged client is dropped rather than allowed to
    // retain unbounded pending events.
    let (sender, receiver) = std::sync::mpsc::sync_channel(64);
    let id = {
        let mut next = NEXT_ID
            .lock()
            .unwrap_or_else(|poisoned| poisoned.into_inner());
        let id = *next;
        *next += 1;
        id
    };
    clients().push(Client { id, sender });
    (id, receiver)
}

pub fn unregister(id: u64) {
    clients().retain(|client| client.id != id);
}

pub fn count() -> usize {
    clients().len()
}

fn clients() -> std::sync::MutexGuard<'static, Vec<Client>> {
    CLIENTS
        .lock()
        .unwrap_or_else(|poisoned| poisoned.into_inner())
}

fn broadcast(message: Vec<u8>) {
    let mut list = clients();
    // Mirrors `try { enqueue } catch { delete }`: a failed send removes the
    // client, and iteration order is insertion order.
    list.retain(|client| match client.sender.try_send(message.clone()) {
        Ok(()) => true,
        Err(TrySendError::Full(_)) => false,
        Err(TrySendError::Disconnected(_)) => false,
    });
}

pub fn send_event(event: &str, data: &str) {
    broadcast(sse_frame(event, data));
}

/// `notify()`: one `update` event carrying the current snapshot.
pub fn notify() {
    send_event("update", &crate::state::snapshot().stringify());
}

/// The 5-second `: keepalive\n\n` comment frame.
pub fn keepalive() {
    broadcast(b": keepalive\n\n".to_vec());
}

#[cfg(test)]
mod tests {
    use super::*;

    // Both tests exercise the process-global registry used by the live
    // server. Serialise them so Rust's parallel test runner cannot make one
    // test observe the other's temporary subscriber.
    static TEST_LOCK: Mutex<()> = Mutex::new(());

    #[test]
    fn a_registered_client_receives_events_in_order_and_unregisters_cleanly() {
        let _guard = TEST_LOCK
            .lock()
            .unwrap_or_else(|poisoned| poisoned.into_inner());
        let before = count();
        let (id, receiver) = register();
        assert_eq!(count(), before + 1);
        send_event("update", "{\"a\":1}");
        keepalive();
        assert_eq!(
            receiver.recv().unwrap(),
            b"event: update\ndata: {\"a\":1}\n\n".to_vec()
        );
        assert_eq!(receiver.recv().unwrap(), b": keepalive\n\n".to_vec());
        unregister(id);
        assert_eq!(count(), before);
    }

    #[test]
    fn a_dropped_receiver_removes_its_client_rather_than_wedging_the_broadcast() {
        let _guard = TEST_LOCK
            .lock()
            .unwrap_or_else(|poisoned| poisoned.into_inner());
        let before = count();
        let (_id, receiver) = register();
        drop(receiver);
        send_event("update", "{}");
        assert_eq!(
            count(),
            before,
            "a disconnected client must be reaped, as the catch does"
        );
    }
}
