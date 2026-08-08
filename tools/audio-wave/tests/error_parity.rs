//! Differential harness: the same crafted cases the TypeScript original is fed
//! in `scratchpad/errors.ts`, emitting one `label: message` line each. The
//! expected output below was captured from Bun running
//! `tools/make/audio_wave.ts`, so this test fails if the port ever drifts from
//! the original's error text, rounding, or byte layout.

use audio_wave::{
    build_wave_record, probe_wave_record, signed_pcm_from_wav, wav_from_signed_pcm,
    ExactWaveHeaderSource, ExactWavePaddingSource, Scalar, WaveRecordSource,
};

const EXPECTED: &str = "\
neg-size: wave record size must be a nonnegative integer
frac-size: wave record size must be a nonnegative integer
nan-size: wave record size must be a nonnegative integer
inf-size: wave record size must be a nonnegative integer
big-freq: wave frequency must fit an unsigned word
hex-size: ok
empty-size: wave record size is shorter than its sample data
short-size: wave record size is shorter than its sample data
align: wave record size has invalid alignment
loop-past: wave loop starts beyond sample data
no-samples: wave record has no samples
rate-mismatch: WAV is not canonical mono 8-bit PCM
odd-freq-round: ok
odd-freq-round-up: ok
exact-freq: wave catalog frequency differs from exact header
exact-loop: wave catalog loop differs from exact header
exact-count: wave sample count differs from WAV data
exact-pad-size: wave padding size differs from record extent
exact-pad-fill: wave padding fill must fit an unsigned byte
exact-no-pad: wave padding size differs from record extent
  bytes=ffffffff00007d00000000000300000001020304
exact-max-control: ok
bad-control: wave control must fit an unsigned word
wav-trunc: WAV is truncated
wav-riff: WAV is missing RIFF
wav-highbit: ok
wav-rate0: invalid WAV rate
wav-rate-frac: invalid WAV rate
wav-rate-max: ok
  {\"control\":\"0x00000000\",\"frequency\":8192000,\"loop_start\":0,\"sample_count\":4} {\"size\":3,\"fill\":0}
probe-ok: ok
probe-small: wave probe extent is invalid
probe-frac: wave probe extent is invalid
probe-neg: wave probe extent is invalid
probe-over: wave probe extent is invalid
probe-zero-freq: wave frequency is zero
probe-overrun: wave samples extend beyond the record
probe-max-count: wave samples extend beyond the record
probe-pad: wave padding is not uniform
probe-loop-past: wave loop starts beyond sample data
";

fn catalog(frequency: Scalar, loop_start: Option<f64>, size: Scalar) -> WaveRecordSource {
    WaveRecordSource {
        frequency,
        loop_start,
        size,
        header: None,
        padding: None,
    }
}

fn header(
    control: Scalar,
    frequency: Scalar,
    loop_start: f64,
    sample_count: f64,
) -> ExactWaveHeaderSource {
    ExactWaveHeaderSource {
        control,
        frequency,
        loop_start: Scalar::Num(loop_start),
        sample_count: Scalar::Num(sample_count),
    }
}

fn padding(size: f64, fill: f64) -> ExactWavePaddingSource {
    ExactWavePaddingSource {
        size: Scalar::Num(size),
        fill: Scalar::Num(fill),
    }
}

struct Log(Vec<String>);

impl Log {
    fn run<T>(&mut self, label: &str, outcome: Result<T, String>) {
        match outcome {
            Ok(_) => self.0.push(format!("{label}: ok")),
            Err(message) => self.0.push(format!("{label}: {message}")),
        }
    }
    fn note(&mut self, text: String) {
        self.0.push(format!("  {text}"));
    }
}

fn hex(bytes: &[u8]) -> String {
    bytes.iter().map(|byte| format!("{byte:02x}")).collect()
}

#[test]
fn error_messages_match_the_typescript_original() {
    let samples = vec![1u8, 2, 3, 4];
    let wav = wav_from_signed_pcm(&samples, 8000.0).unwrap();
    let freq = Scalar::Num(8000.0 * 1024.0);
    let mut log = Log(Vec::new());

    let cases: Vec<(&str, WaveRecordSource)> = vec![
        ("neg-size", catalog(freq.clone(), None, Scalar::Num(-1.0))),
        ("frac-size", catalog(freq.clone(), None, Scalar::Num(1.5))),
        (
            "nan-size",
            catalog(freq.clone(), None, Scalar::str("banana")),
        ),
        (
            "inf-size",
            catalog(freq.clone(), None, Scalar::str("Infinity")),
        ),
        (
            "big-freq",
            catalog(Scalar::Num(4294967296.0), None, Scalar::Num(20.0)),
        ),
        ("hex-size", catalog(freq.clone(), None, Scalar::str("0x14"))),
        ("empty-size", catalog(freq.clone(), None, Scalar::str(""))),
        ("short-size", catalog(freq.clone(), None, Scalar::Num(19.0))),
        ("align", catalog(freq.clone(), None, Scalar::Num(24.0))),
        (
            "loop-past",
            catalog(freq.clone(), Some(4.0), Scalar::Num(20.0)),
        ),
    ];
    for (label, source) in cases {
        log.run(label, build_wave_record(&source, &wav));
    }

    let empty_wav = wav_from_signed_pcm(&[], 8000.0).unwrap();
    log.run(
        "no-samples",
        build_wave_record(&catalog(freq.clone(), None, Scalar::Num(16.0)), &empty_wav),
    );
    log.run(
        "rate-mismatch",
        build_wave_record(
            &catalog(Scalar::Num(8001.0 * 1024.0), None, Scalar::Num(20.0)),
            &wav,
        ),
    );
    // Math.round pulls both of these back to a rate of 8000.
    log.run(
        "odd-freq-round",
        build_wave_record(
            &catalog(
                Scalar::Num(8000.0 * 1024.0 + 511.0),
                None,
                Scalar::Num(20.0),
            ),
            &wav,
        ),
    );
    log.run(
        "odd-freq-round-up",
        build_wave_record(
            &catalog(
                Scalar::Num(8000.0 * 1024.0 - 512.0),
                None,
                Scalar::Num(20.0),
            ),
            &wav,
        ),
    );

    let exact = |catalog_frequency: Scalar,
                 catalog_loop: Option<f64>,
                 size: f64,
                 head: ExactWaveHeaderSource,
                 pad: Option<ExactWavePaddingSource>| WaveRecordSource {
        frequency: catalog_frequency,
        loop_start: catalog_loop,
        size: Scalar::Num(size),
        header: Some(head),
        padding: pad,
    };
    let ok_header = || header(Scalar::str("0x40000000"), freq.clone(), 1.0, 4.0);
    log.run(
        "exact-freq",
        build_wave_record(
            &exact(
                Scalar::Num(8000.0 * 1024.0 + 1.0),
                Some(1.0),
                24.0,
                ok_header(),
                Some(padding(4.0, 0.0)),
            ),
            &wav,
        ),
    );
    log.run(
        "exact-loop",
        build_wave_record(
            &exact(
                freq.clone(),
                Some(2.0),
                24.0,
                ok_header(),
                Some(padding(4.0, 0.0)),
            ),
            &wav,
        ),
    );
    log.run(
        "exact-count",
        build_wave_record(
            &exact(
                freq.clone(),
                Some(1.0),
                24.0,
                header(Scalar::str("0x40000000"), freq.clone(), 1.0, 5.0),
                Some(padding(4.0, 0.0)),
            ),
            &wav,
        ),
    );
    log.run(
        "exact-pad-size",
        build_wave_record(
            &exact(
                freq.clone(),
                Some(1.0),
                24.0,
                ok_header(),
                Some(padding(3.0, 0.0)),
            ),
            &wav,
        ),
    );
    log.run(
        "exact-pad-fill",
        build_wave_record(
            &exact(
                freq.clone(),
                Some(1.0),
                24.0,
                ok_header(),
                Some(padding(4.0, 256.0)),
            ),
            &wav,
        ),
    );
    log.run(
        "exact-no-pad",
        build_wave_record(
            &exact(freq.clone(), Some(1.0), 24.0, ok_header(), None),
            &wav,
        ),
    );

    let max_control = exact(
        Scalar::str("0x7d0000"),
        Some(0.0),
        20.0,
        header(Scalar::str("0xffffffff"), Scalar::str("0x7d0000"), 0.0, 4.0),
        Some(padding(0.0, 0.0)),
    );
    let built = build_wave_record(&max_control, &wav);
    if let Ok((bytes, _)) = &built {
        log.note(format!("bytes={}", hex(bytes)));
    }
    log.run("exact-max-control", built);
    log.run(
        "bad-control",
        build_wave_record(
            &exact(
                freq.clone(),
                Some(0.0),
                20.0,
                header(Scalar::str("0x100000000"), freq.clone(), 0.0, 4.0),
                Some(padding(0.0, 0.0)),
            ),
            &wav,
        ),
    );

    log.run("wav-trunc", signed_pcm_from_wav(&[0u8; 43], 8000.0));
    let mut broken = wav.clone();
    broken[0] = 88;
    log.run("wav-riff", signed_pcm_from_wav(&broken, 8000.0));
    let mut high = wav.clone();
    for byte in high.iter_mut().take(4) {
        *byte |= 0x80;
    }
    log.run("wav-highbit", signed_pcm_from_wav(&high, 8000.0));
    log.run("wav-rate0", wav_from_signed_pcm(&samples, 0.0));
    log.run("wav-rate-frac", wav_from_signed_pcm(&samples, 1.5));
    log.run("wav-rate-max", wav_from_signed_pcm(&samples, 4294967295.0));

    let (record, _) =
        build_wave_record(&catalog(freq.clone(), None, Scalar::Num(23.0)), &wav).unwrap();
    let probed = probe_wave_record(&record, 0.0, 23.0);
    if let Ok(value) = &probed {
        let control = match &value.header.control {
            Scalar::Str(text) => text.clone(),
            Scalar::Num(number) => number.to_string(),
        };
        let number = |scalar: &Scalar| match scalar {
            Scalar::Num(value) => format!("{}", *value as i64),
            Scalar::Str(text) => text.clone(),
        };
        log.note(format!(
            "{{\"control\":\"{control}\",\"frequency\":{},\"loop_start\":{},\"sample_count\":{}}} {{\"size\":{},\"fill\":{}}}",
            number(&value.header.frequency),
            number(&value.header.loop_start),
            number(&value.header.sample_count),
            number(&value.padding.size),
            number(&value.padding.fill),
        ));
    }
    log.run("probe-ok", probed);
    log.run("probe-small", probe_wave_record(&record, 0.0, 15.0));
    log.run("probe-frac", probe_wave_record(&record, 0.5, 16.0));
    log.run("probe-neg", probe_wave_record(&record, -1.0, 16.0));
    log.run("probe-over", probe_wave_record(&record, 8.0, 20.0));
    log.run("probe-zero-freq", probe_wave_record(&[0u8; 32], 0.0, 16.0));
    let mut overrun = record.clone();
    overrun[12..16].copy_from_slice(&99u32.to_le_bytes());
    log.run("probe-overrun", probe_wave_record(&overrun, 0.0, 23.0));
    let mut maxed = record.clone();
    maxed[12..16].copy_from_slice(&u32::MAX.to_le_bytes());
    log.run("probe-max-count", probe_wave_record(&maxed, 0.0, 23.0));
    let mut dirty = record.clone();
    dirty[22] = 1;
    log.run("probe-pad", probe_wave_record(&dirty, 0.0, 23.0));
    let mut looped = record.clone();
    looped[0..4].copy_from_slice(&0xc000_0000u32.to_le_bytes());
    looped[8..12].copy_from_slice(&9u32.to_le_bytes());
    log.run("probe-loop-past", probe_wave_record(&looped, 0.0, 23.0));

    let actual = format!("{}\n", log.0.join("\n"));
    if let Ok(path) = std::env::var("AUDIO_WAVE_ERRORS") {
        std::fs::write(path, &actual).unwrap();
    }
    assert_eq!(actual, EXPECTED);
}
