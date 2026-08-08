// Overlay images come from bun, on purpose.
//
// `overlayImage()` in tools/lib/overlay_call_targets.ts does not read a file. It
// runs `arm-none-eabi-as`/`ld`/`objcopy` over the reconstruction assembly and then
// patches in every overlay function that has a compiled C reconstruction -- which
// routes through tools/lib/alchemy_gcc.ts, the compiler driver that is explicitly
// out of scope for this port. Reimplementing that here would fork the definition
// of "the overlay image" and let the two copies drift silently, which is the one
// failure mode this tool exists to catch.
//
// PORT NOTE: so we bridge. One `bun -e` process assembles every requested overlay
// and streams the images back framed, instead of the TypeScript's per-name call.
// The batching is a process-spawn optimisation only; each name is still assembled
// independently and a failure on one name is reported for that name alone, exactly
// as the TypeScript's per-name try/catch does.

use std::path::Path;
use std::process::Command;

/// The bridge script. Frames are `[status:u8][length:u32 LE][payload]`, one per
/// requested name and in the requested order; status 0 is an image, 1 is the
/// error message from the `overlayImage` throw.
const BRIDGE: &str = r#"
const { overlayImage } = await import(process.env.ALCHEMY_OVERLAY_MODULE);
const names = process.env.ALCHEMY_OVERLAY_NAMES.split("\n").filter((n) => n.length > 0);
const frames = [];
for (const name of names) {
  let status = 0;
  let payload;
  try {
    payload = Buffer.from(overlayImage(name));
  } catch (error) {
    status = 1;
    payload = Buffer.from(String(error && error.message ? error.message : error));
  }
  const header = Buffer.alloc(5);
  header[0] = status;
  header.writeUInt32LE(payload.length, 1);
  frames.push(header, payload);
}
await Bun.write(Bun.stdout, new Blob(frames));
"#;

/// Assembles each named overlay. The result vector is positionally aligned with
/// `names`; `Err` carries the thrown message for that name.
pub fn overlay_images(root: &Path, names: &[String]) -> Vec<Result<Vec<u8>, String>> {
    if names.is_empty() {
        return Vec::new();
    }
    let module = root.join("tools").join("lib").join("overlay_call_targets.ts");
    let output = Command::new("bun")
        .arg("-e")
        .arg(BRIDGE)
        .current_dir(root)
        // Passing the inputs through the environment rather than argv keeps overlay
        // names out of the shell-quoting blast radius entirely.
        .env("ALCHEMY_OVERLAY_MODULE", &module)
        .env("ALCHEMY_OVERLAY_NAMES", names.join("\n"))
        .output();

    let output = match output {
        Ok(output) if output.status.success() => output,
        Ok(output) => {
            let message = format!(
                "overlay image bridge failed: {}",
                String::from_utf8_lossy(&output.stderr).trim()
            );
            return names.iter().map(|_| Err(message.clone())).collect();
        }
        Err(error) => {
            let message = format!("overlay image bridge could not start bun: {error}");
            return names.iter().map(|_| Err(message.clone())).collect();
        }
    };

    let mut results = decode_frames(&output.stdout);
    // A short reply means the bridge died mid-stream; the remaining names were not
    // examined, and saying so is what keeps `examined == 0` a failure upstream.
    while results.len() < names.len() {
        results.push(Err("overlay image bridge returned no frame".to_string()));
    }
    results.truncate(names.len());
    results
}

fn decode_frames(stream: &[u8]) -> Vec<Result<Vec<u8>, String>> {
    let mut frames = Vec::new();
    let mut i = 0usize;
    while i + 5 <= stream.len() {
        let status = stream[i];
        let length = u32::from_le_bytes([stream[i + 1], stream[i + 2], stream[i + 3], stream[i + 4]])
            as usize;
        let start = i + 5;
        let Some(payload) = stream.get(start..start + length) else {
            break;
        };
        frames.push(if status == 0 {
            Ok(payload.to_vec())
        } else {
            Err(String::from_utf8_lossy(payload).into_owned())
        });
        i = start + length;
    }
    frames
}

#[cfg(test)]
mod tests {
    use super::*;

    fn frame(status: u8, payload: &[u8]) -> Vec<u8> {
        let mut bytes = vec![status];
        bytes.extend_from_slice(&(payload.len() as u32).to_le_bytes());
        bytes.extend_from_slice(payload);
        bytes
    }

    #[test]
    fn frames_decode_in_order_and_keep_errors_separate() {
        let mut stream = frame(0, &[1, 2, 3]);
        stream.extend(frame(1, b"no reconstruction assembly for resource_ffffff"));
        stream.extend(frame(0, &[]));
        let decoded = decode_frames(&stream);
        assert_eq!(decoded.len(), 3);
        assert_eq!(decoded[0], Ok(vec![1, 2, 3]));
        assert_eq!(
            decoded[1],
            Err("no reconstruction assembly for resource_ffffff".to_string())
        );
        assert_eq!(decoded[2], Ok(Vec::new()));
    }

    #[test]
    fn a_truncated_stream_yields_only_the_complete_frames() {
        let mut stream = frame(0, &[9, 9]);
        stream.extend(frame(0, &[7, 7, 7]));
        stream.truncate(stream.len() - 1);
        let decoded = decode_frames(&stream);
        assert_eq!(decoded, vec![Ok(vec![9, 9])]);
    }

    #[test]
    fn an_empty_request_never_spawns_bun() {
        assert!(overlay_images(Path::new("/nonexistent"), &[]).is_empty());
    }
}
