//! Small, dependency-light live dashboard server.

pub mod assets;
pub mod client;

use coverage_map::{
    boxtree::{render_box_trees, svg_cache_version, BOX_TREES},
    pipeline::{build_coverage_map, BuildOptions},
    tree::work_tree_at,
};
use serde_json::{json, Map, Value};
use std::{
    collections::{BTreeMap, BTreeSet},
    io::{BufRead, BufReader, Write},
    net::{IpAddr, Ipv4Addr, SocketAddr, TcpListener, TcpStream},
    path::{Path, PathBuf},
    sync::Mutex,
    time::{Duration, SystemTime, UNIX_EPOCH},
};
use walkdir::WalkDir;

const TREES: [(&str, &str); 4] = [
    ("core", "Main image"),
    ("overlays", "Code overlays"),
    ("images", "Images"),
    ("music", "Music"),
];
const COVERAGE_DIRS: [&str; 12] = [
    "games/gs1/asm",
    "games/gs1/assets",
    "games/gs1/metrics",
    "games/gs1/semantic",
    "games/gs1/src",
    "games/gs1/source-paths.json",
    "games/gs1/recon",
    "games/gs1/project.json",
    "games/gs2",
    "games/gs2/project.json",
    "games/alchemy",
    "out/gs1-en/reports",
];

pub mod cli {
    pub fn entry(args: &[String]) {
        let mut bind = None;
        let mut i = 0;
        while i < args.len() {
            match args[i].as_str() {
                "-h" | "--help" => {
                    return println!("Usage: dashboard-server [--bind HOST:PORT] [--self-test]")
                }
                "--self-test" if i + 1 == args.len() => {
                    return match crate::self_test() {
                        Ok(s) => println!("{s}"),
                        Err(e) => {
                            eprintln!("error: {e}");
                            std::process::exit(1);
                        }
                    }
                }
                "--bind" if i + 1 < args.len() => {
                    bind = args.get(i + 1).and_then(|s| s.parse().ok());
                    i += 1;
                }
                value if value.starts_with("--bind=") => bind = value[7..].parse().ok(),
                value => {
                    eprintln!("error: unknown argument {value:?}");
                    std::process::exit(2);
                }
            }
            i += 1;
        }
        if let Err(e) = crate::run(bind) {
            eprintln!("error: {e}");
            std::process::exit(1);
        }
    }
}

fn root() -> PathBuf {
    std::env::var_os("ALCHEMY_DASHBOARD_ROOT")
        .map(PathBuf::from)
        .filter(|p| !p.as_os_str().is_empty())
        .unwrap_or_else(|| {
            Path::new(env!("CARGO_MANIFEST_DIR"))
                .parent()
                .unwrap()
                .parent()
                .unwrap()
                .into()
        })
}
fn font() -> PathBuf {
    root().join("games/gs1/assets/fonts/weyard.otf")
}
fn music_dir() -> PathBuf {
    root().join("games/gs1/sound/songs/midi")
}
fn audio_dir() -> PathBuf {
    root().join("games/gs1/sound")
}
fn gs2_sound_dir() -> PathBuf {
    root().join("games/gs2/sound")
}
fn music_register() -> PathBuf {
    root().join("games/music.tsv")
}
fn page_version() -> String {
    let client = client::bundled_client().unwrap_or_default();
    coverage_map::sha1::sha1_hex(format!("{}\0{client}", assets::STYLES).as_bytes())[..16].into()
}
fn port() -> u16 {
    std::env::var("ALCHEMY_DASHBOARD_PORT")
        .ok()
        .and_then(|v| v.parse().ok())
        .unwrap_or(4649)
}
fn host() -> IpAddr {
    std::env::var("ALCHEMY_DASHBOARD_HOST")
        .ok()
        .and_then(|v| v.parse().ok())
        .unwrap_or(IpAddr::V4(Ipv4Addr::UNSPECIFIED))
}

pub struct Live {
    revision: String,
    generated: String,
    trees: Vec<(&'static str, String)>,
    executable: f64,
    proven_c: f64,
    proven_c_percent: f64,
    draft_c: f64,
    draft_c_percent: f64,
    proven_asm: f64,
    gs1_ja_sources: usize,
    gs1_en_sources: usize,
    gs2_ja_sources: usize,
    gs2_en_sources: usize,
    correspondence: Option<Correspondence>,
}

struct Correspondence {
    total: usize,
    matched: usize,
    shared: usize,
    regional: usize,
    unresolved: usize,
}
impl Correspondence {
    fn add(self, other: Self) -> Self {
        Self {
            total: self.total + other.total,
            matched: self.matched + other.matched,
            shared: self.shared + other.shared,
            regional: self.regional + other.regional,
            unresolved: self.unresolved + other.unresolved,
        }
    }
}
#[derive(Default)]
pub struct State {
    coverage: Option<Live>,
    error: Option<String>,
    scanning: bool,
}
static STATE: Mutex<Option<State>> = Mutex::new(None);
fn state<R>(f: impl FnOnce(&mut State) -> R) -> R {
    f(STATE
        .lock()
        .unwrap_or_else(|e| e.into_inner())
        .get_or_insert_with(State::default))
}
fn document_number(document: &Value, path: &[&str]) -> Option<f64> {
    let mut v = document;
    for key in path {
        v = v.get(key)?;
    }
    v.as_f64()
}
fn count_c(path: &Path) -> usize {
    WalkDir::new(path)
        .into_iter()
        .filter_map(Result::ok)
        .filter(|entry| {
            entry.file_type().is_file() && entry.path().extension().is_some_and(|x| x == "c")
        })
        .count()
}
fn correspondence(path: &Path) -> Result<Option<Correspondence>, String> {
    let bytes = match std::fs::read(path) {
        Ok(bytes) => bytes,
        Err(error) if error.kind() == std::io::ErrorKind::NotFound => return Ok(None),
        Err(error) => return Err(format!("{}: {error}", path.display())),
    };
    let value: serde_json::Value =
        serde_json::from_slice(&bytes).map_err(|error| format!("{}: {error}", path.display()))?;
    let number = |key: &str| {
        value
            .get(key)
            .and_then(serde_json::Value::as_u64)
            .map(|value| value as usize)
            .ok_or_else(|| format!("{} lacks numeric {key}", path.display()))
    };
    Ok(Some(Correspondence {
        total: number("owners_total")?,
        matched: number("matched_owners")?,
        shared: number("shared_core_owners")?,
        regional: number("regional_core_owners")?,
        unresolved: number("unresolved_owners")?,
    }))
}
fn compute() -> Result<Live, String> {
    let tree = work_tree_at(root());
    let map = build_coverage_map(&BuildOptions {
        target: "gs1-en".into(),
        exact: &tree,
        recon: Some(&tree),
        prefer_verified_assets: true,
    })?;
    let trees = render_box_trees(&map, Some(&tree), true)?;
    live_from(map.document, trees)
}
fn cached() -> Result<Live, String> {
    let report = root().join("out/gs1-en/reports/coverage-map.json");
    let document = serde_json::from_slice(
        &std::fs::read(&report).map_err(|error| format!("{}: {error}", report.display()))?,
    )
    .map_err(|error| format!("{}: {error}", report.display()))?;
    let trees = BOX_TREES
        .iter()
        .map(|name| {
            let path = root().join(format!("games/gs1/assets/readme/gs1-en-{name}.svg"));
            std::fs::read_to_string(&path)
                .map(|svg| (*name, svg))
                .map_err(|error| format!("{}: {error}", path.display()))
        })
        .collect::<Result<Vec<_>, _>>()?;
    live_from(document, trees)
}
fn live_from(document: Value, trees: Vec<(&'static str, String)>) -> Result<Live, String> {
    let gs1_ja_sources = count_c(&root().join("games/gs1/recon/ja"));
    let gs1_en_sources = count_c(&root().join("games/gs1/recon/en"));
    let gs2_ja_sources = count_c(&root().join("games/gs2/recon/ja"));
    let gs2_en_sources = count_c(&root().join("games/gs2/recon/en"));
    let reports = root().join("out/gs1-en/reports");
    let correspondence = match (
        correspondence(&reports.join("exact-correspondence.json"))?,
        correspondence(&reports.join("exact-overlay-correspondence.json"))?,
    ) {
        (Some(main), Some(overlays)) => Some(main.add(overlays)),
        _ => None,
    };
    let correspondence_revision = correspondence.as_ref().map_or_else(
        || "correspondence-unavailable".into(),
        |correspondence| {
            format!(
                "{}-{}-{}-{}",
                correspondence.matched,
                correspondence.shared,
                correspondence.regional,
                correspondence.unresolved
            )
        },
    );
    let revision = BOX_TREES
        .iter()
        .map(|name| {
            svg_cache_version(
                trees
                    .iter()
                    .find(|(id, _)| id == name)
                    .map_or("", |(_, s)| s),
            )
        })
        .collect::<Vec<_>>()
        .into_iter()
        .chain([
            gs1_ja_sources.to_string(),
            gs1_en_sources.to_string(),
            gs2_ja_sources.to_string(),
            gs2_en_sources.to_string(),
            correspondence_revision,
        ])
        .collect::<Vec<_>>()
        .join("-");
    let n = |key| document_number(&document, key).unwrap_or(0.0);
    Ok(Live {
        revision,
        generated: iso_now(),
        trees,
        executable: n(&["executable_bytes"]),
        proven_c: n(&["categories", "proven_c", "bytes"]),
        proven_c_percent: n(&["categories", "proven_c", "percent_of_executable"]),
        draft_c: n(&["categories", "draft_c", "bytes"]),
        draft_c_percent: n(&["categories", "draft_c", "percent_of_executable"]),
        proven_asm: n(&["categories", "proven_asm", "bytes"]),
        gs1_ja_sources,
        gs1_en_sources,
        gs2_ja_sources,
        gs2_en_sources,
        correspondence,
    })
}
fn iso_now() -> String {
    let ms = SystemTime::now()
        .duration_since(UNIX_EPOCH)
        .map_or(0, |duration| duration.as_millis() as i64);
    let days = ms.div_euclid(86_400_000);
    let rest = ms.rem_euclid(86_400_000);
    let z = days + 719468;
    let era = z.div_euclid(146097);
    let doe = z.rem_euclid(146097);
    let yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
    let year = yoe + era * 400;
    let day_of_year = doe - (365 * yoe + yoe / 4 - yoe / 100);
    let month_part = (5 * day_of_year + 2) / 153;
    let day = day_of_year - (153 * month_part + 2) / 5 + 1;
    let month = if month_part < 10 {
        month_part + 3
    } else {
        month_part - 9
    };
    let year = if month <= 2 { year + 1 } else { year };
    format!(
        "{year:04}-{month:02}-{day:02}T{:02}:{:02}:{:02}.{:03}Z",
        rest / 3_600_000,
        rest / 60_000 % 60,
        rest / 1000 % 60,
        rest % 1000
    )
}
fn number(value: f64) -> Value {
    serde_json::from_str(&coverage_map::jsnum::js_number_string(value)).unwrap_or(Value::Null)
}
fn snapshot_from(state: &State) -> Value {
    let mut document = json!({
            "page": page_version(),
            "revision": state.coverage.as_ref().map_or("starting", |live| live.revision.as_str()),
            "generatedAt": state.coverage.as_ref().map(|live| live.generated.as_str()),
            "scanning": state.scanning,
            "trees": TREES.into_iter()
                .map(|(key, title)| (key.into(), Value::String(title.into())))
                .collect::<Map<_, _>>(),
            "project": {
                "title": "Golden Sun · The Lost Age",
                "gs1": "ja · en · de · es · fr · it",
                "gs2": "ja · en · de · es · fr · it",
                "fullTarget": "gs1-en",
                "integration": "Alchemy"
            }
    });
    let object = document.as_object_mut().expect("dashboard snapshot object");
    if let Some(error) = &state.error {
        object.insert("error".into(), json!(error));
    }
    if let Some(live) = &state.coverage {
        let mut summary = json!({
                "executableBytes": number(live.executable),
                "provenCBytes": number(live.proven_c),
                "provenCPercent": number(live.proven_c_percent),
                "draftCBytes": number(live.draft_c),
                "draftCPercent": number(live.draft_c_percent),
                "provenAsmBytes": number(live.proven_asm),
                "doneBytes": number(live.proven_c + live.proven_asm),
                "donePercent": number((live.proven_c + live.proven_asm) * 100.0 / live.executable.max(1.0)),
                "gs1JaSources": live.gs1_ja_sources,
                "gs1EnSources": live.gs1_en_sources,
                "gs2JaSources": live.gs2_ja_sources,
                "gs2EnSources": live.gs2_en_sources,
                "historicalTargets": 12,
                "fullTargets": 1,
                "compileOnlyTargets": 11,
                "correspondenceAvailable": live.correspondence.is_some()
        });
        if let Some(value) = &live.correspondence {
            summary
                .as_object_mut()
                .expect("dashboard summary object")
                .extend([
                    ("correspondenceTotal".into(), json!(value.total)),
                    ("correspondenceMatched".into(), json!(value.matched)),
                    ("correspondenceShared".into(), json!(value.shared)),
                    ("correspondenceRegional".into(), json!(value.regional)),
                    ("correspondenceUnresolved".into(), json!(value.unresolved)),
                ]);
        }
        object.insert("summary".into(), summary);
    }
    document
}
fn snapshot() -> Value {
    state(|state| snapshot_from(state))
}
fn snapshot_text() -> String {
    serde_json::to_string(&snapshot()).expect("dashboard snapshot serializes")
}
fn event_stream() -> Response {
    Response::new(
        200,
        "OK",
        Some("text/event-stream; charset=utf-8"),
        "no-store",
        format!("event: update\ndata: {}\nretry: 1000\n\n", snapshot_text()),
    )
}
fn music_catalog() -> Result<Vec<u8>, String> {
    let register = std::fs::read_to_string(music_register())
        .map_err(|error| format!("music register: {error}"))?;
    let catalog = music::parse_music_catalog(&register)?;
    let sequences = std::fs::read_to_string(audio_dir().join("sequences.tsv"))
        .map_err(|error| format!("GS1 music sequences: {error}"))?;
    let mut tracks = sequences
        .lines()
        .filter(|line| !line.starts_with('#'))
        .skip(1)
        .filter_map(|line| {
            let fields = line.split('\t').collect::<Vec<_>>();
            (fields.get(1) == Some(&"music")).then_some(fields)
        })
        .map(|fields| {
            if fields.len() != 5 {
                return Err("GS1 music sequence row width differs".to_string());
            }
            let sound_id = fields[0]
                .parse::<u16>()
                .map_err(|_| "GS1 music sequence ID differs".to_string())?;
            let file = Path::new(fields[4])
                .file_name()
                .and_then(|name| name.to_str())
                .ok_or("GS1 music sequence filename differs")?
                .to_string();
            let title = catalog.english_title("gs1", sound_id);
            Ok(json!({
                "key": format!("gs1:{sound_id}"),
                "game": "gs1",
                "gameTitle": "Golden Sun",
                "soundId": sound_id,
                "file": file,
                "path": format!("/music/{file}"),
                "available": true,
                "status": "byte-exact",
                "title": title.map(str::to_owned).unwrap_or_else(|| "Untitled".to_string()),
                "source": format!("Golden Sun · ROM sequence {sound_id:03}")
            }))
        })
        .collect::<Result<Vec<_>, _>>()?;
    if tracks.len() != 58 {
        return Err("GS1 music catalog does not expose all 58 music sequences".into());
    }
    let requests = std::fs::read_to_string(gs2_sound_dir().join("music_requests.tsv"))
        .map_err(|error| format!("GS2 music requests: {error}"))?;
    let requests = requests
        .lines()
        .filter(|line| !line.starts_with('#'))
        .skip(1)
        .map(|line| {
            let fields = line.split('\t').collect::<Vec<_>>();
            if fields.len() != 5 {
                return Err("GS2 music request row width differs".to_string());
            }
            let request = fields[0]
                .parse::<u16>()
                .map_err(|_| "GS2 music request number differs".to_string())?;
            let sound_id = fields[1]
                .parse::<u16>()
                .map_err(|_| "GS2 music request sound ID differs".to_string())?;
            let status = fields[4].to_string();
            let file = (status == "byte-exact")
                .then(|| {
                    Path::new(fields[3])
                        .file_name()?
                        .to_str()
                        .map(str::to_string)
                })
                .flatten();
            Ok((request, sound_id, fields[2].to_string(), file, status))
        })
        .collect::<Result<Vec<_>, _>>()?;
    if requests.len() != 97
        || requests
            .iter()
            .enumerate()
            .any(|(index, row)| usize::from(row.0) != index)
    {
        return Err("GS2 music request register is not the complete ordered 0-96 set".into());
    }
    tracks.extend(
        requests
            .into_iter()
            .map(|(request, sound_id, header, file, status)| {
                let title = catalog.english_title("gs2", sound_id);
                json!({
                    "key": format!("gs2:{request}"),
                    "game": "gs2",
                    "gameTitle": "Golden Sun: The Lost Age",
                    "request": request,
                    "soundId": sound_id,
                    "header": header,
                    "file": file,
                    "path": file.as_ref().map(|file| format!("/music/gs2/{file}")),
                    "available": file.is_some(),
                    "status": status,
                    "title": title.map(str::to_owned).unwrap_or_else(|| "Untitled".to_string()),
                    "source": format!("Golden Sun: The Lost Age · ROM sequence {sound_id}")
                })
            }),
    );
    serde_json::to_vec(&tracks).map_err(|error| error.to_string())
}
fn read_programmable_waveforms(
    sound: &Path,
    count: usize,
    digits: usize,
    base: u32,
) -> Result<Vec<Value>, String> {
    let path = sound.join("waveforms.tsv");
    let text =
        std::fs::read_to_string(&path).map_err(|error| format!("{}: {error}", path.display()))?;
    let mut lines = text.lines().filter(|line| !line.starts_with('#'));
    if lines.next() != Some("wave\taddress\tsource") {
        return Err(format!("{}: waveform table header differs", path.display()));
    }
    let mut waveforms = Vec::with_capacity(count);
    for index in 0..count {
        let fields = lines
            .next()
            .ok_or_else(|| format!("waveform {index} is absent"))?
            .split('\t')
            .collect::<Vec<_>>();
        let name = format!("wave_{index:0digits$}");
        let source = format!("programmable_wave_samples/{name}.pcm4");
        let address = fields
            .get(1)
            .and_then(|value| u32::from_str_radix(value.trim_start_matches("0x"), 16).ok());
        if fields.len() != 3
            || fields[0] != index.to_string()
            || address != Some(base + index as u32 * 16)
            || fields[2] != source
        {
            return Err(format!("waveform {index} identity differs"));
        }
        let data = std::fs::read(sound.join(&source))
            .map_err(|error| format!("waveform {index}: {error}"))?;
        if data.len() != 16 {
            return Err(format!("waveform {index} extent differs"));
        }
        waveforms.push(json!({
            "name": name,
            "samples": data.into_iter().flat_map(|byte| [byte >> 4, byte & 15]).collect::<Vec<_>>()
        }));
    }
    if lines.next().is_some() {
        return Err("waveform table has extra rows".into());
    }
    Ok(waveforms)
}

fn read_gs1_samples(sound: &Path) -> Result<Vec<Value>, String> {
    let path = sound.join("samples.tsv");
    let text =
        std::fs::read_to_string(&path).map_err(|error| format!("{}: {error}", path.display()))?;
    let mut lines = text.lines().filter(|line| !line.starts_with('#'));
    if lines.next() != Some("sample\taddress\tfrequency\tloop_start\tsample_count\tsource") {
        return Err("GS1 sample table header differs".into());
    }
    let mut samples = Vec::with_capacity(32);
    for sample in 0..32 {
        let fields = lines
            .next()
            .ok_or_else(|| format!("GS1 sample {sample} is absent"))?
            .split('\t')
            .collect::<Vec<_>>();
        if fields.len() != 6 || fields[0] != sample.to_string() {
            return Err(format!("GS1 sample {sample} identity differs"));
        }
        let count = fields[4]
            .parse::<usize>()
            .map_err(|_| format!("GS1 sample {sample} extent differs"))?;
        samples.push(json!({
            "name": format!("wave_{sample:02}"),
            "address": fields[1],
            "size": (16 + count + 3) & !3,
            "frequency": fields[2].parse::<u32>().map_err(|_| format!("GS1 sample {sample} frequency differs"))?,
            "loop_start": if fields[3].is_empty() { Value::Null } else { json!(fields[3].parse::<u32>().map_err(|_| format!("GS1 sample {sample} loop differs"))?) },
            "source": Path::new(fields[5]).file_name().and_then(|name| name.to_str()).ok_or_else(|| format!("GS1 sample {sample} source differs"))?
        }));
    }
    if lines.next().is_some() {
        return Err("GS1 sample table has extra rows".into());
    }
    Ok(samples)
}

fn read_gs1_voicegroup(sound: &Path, samples: &[Value]) -> Result<Vec<Value>, String> {
    let sample_by_source = samples
        .iter()
        .filter_map(|sample| {
            Some((
                format!("direct_sound_samples/{}", sample.get("source")?.as_str()?),
                sample.get("address")?.as_str()?,
            ))
        })
        .collect::<BTreeMap<_, _>>();
    let path = sound.join("voicegroups/voicegroup_000.tsv");
    let text =
        std::fs::read_to_string(&path).map_err(|error| format!("{}: {error}", path.display()))?;
    let mut lines = text.lines().filter(|line| !line.starts_with('#'));
    if lines.next()
        != Some("program\tkind\tfixed_pitch\tkey\tlength\tpan_sweep\tsource\tattack\tdecay\tsustain\trelease")
    {
        return Err("GS1 voice-group header differs".into());
    }
    let mut bank = Vec::with_capacity(144);
    for program in 0..144 {
        let fields = lines
            .next()
            .ok_or_else(|| format!("GS1 voice {program} is absent"))?
            .split('\t')
            .collect::<Vec<_>>();
        if fields.len() != 11 || fields[0] != program.to_string() {
            return Err(format!("GS1 voice {program} identity differs"));
        }
        let number = |field: usize, label: &str| {
            fields[field]
                .parse::<u8>()
                .map_err(|_| format!("GS1 voice {program} {label} differs"))
        };
        let mut tone = json!({
            "kind": fields[1],
            "fixed_pitch": fields[2] == "true",
            "key": number(3, "key")?,
            "length": number(4, "length")?,
            "pan_sweep": number(5, "pan")?,
            "envelope": [number(7, "attack")?, number(8, "decay")?, number(9, "sustain")?, number(10, "release")?]
        });
        match fields[1] {
            "pcm" => {
                tone["sample"] = json!(sample_by_source
                    .get(fields[6])
                    .copied()
                    .or_else(|| fields[6].strip_prefix("embedded_pcm_"))
                    .ok_or_else(|| format!("GS1 voice {program} sample differs"))?)
            }
            "wave" => {
                tone["waveform"] = json!(Path::new(fields[6])
                    .file_stem()
                    .and_then(|name| name.to_str())
                    .ok_or_else(|| format!("GS1 voice {program} waveform differs"))?)
            }
            "rhythm" => {
                let (bank_number, program_number) = fields[6]
                    .strip_prefix("voicegroup_")
                    .and_then(|value| value.split_once(':'))
                    .ok_or_else(|| format!("GS1 voice {program} rhythm table differs"))?;
                tone["tones"] = json!(format!(
                    "bank_{}_{:03}",
                    bank_number
                        .parse::<u8>()
                        .map_err(|_| format!("GS1 voice {program} rhythm bank differs"))?,
                    program_number
                        .parse::<u8>()
                        .map_err(|_| format!("GS1 voice {program} rhythm program differs"))?
                ));
            }
            "pulse_1" | "pulse_2" | "noise" => tone["generator"] = json!(number(6, "generator")?),
            _ => return Err(format!("GS1 voice {program} kind differs")),
        }
        bank.push(tone);
    }
    if lines.next().is_some() {
        return Err("GS1 voice group has extra rows".into());
    }
    Ok(bank)
}

fn soundfont() -> Result<Vec<u8>, String> {
    let samples = read_gs1_samples(&audio_dir())?;
    let waveforms = read_programmable_waveforms(&audio_dir(), 18, 2, 0x080f_c504)?;
    let residuals_path = root().join("games/gs1/assets/data/final_byte_regions_index.json");
    let residuals: Value = serde_json::from_slice(
        &std::fs::read(&residuals_path)
            .map_err(|error| format!("{}: {error}", residuals_path.display()))?,
    )
    .map_err(|error| format!("{}: {error}", residuals_path.display()))?;
    let bank = read_gs1_voicegroup(&audio_dir(), &samples)?;
    let mut sample_addresses = samples
        .iter()
        .filter_map(|sample| sample.get("address").and_then(Value::as_str))
        .map(str::to_string)
        .collect::<BTreeSet<_>>();
    let embedded_region = residuals
        .get("regions")
        .or_else(|| residuals.get("entries"))
        .and_then(Value::as_array)
        .and_then(|regions| {
            regions
                .iter()
                .find(|region| region.get("address").and_then(Value::as_str) == Some("0x0811dac8"))
        })
        .or_else(|| {
            residuals.as_array().and_then(|regions| {
                regions.iter().find(|region| {
                    region.get("address").and_then(Value::as_str) == Some("0x0811dac8")
                })
            })
        })
        .ok_or("embedded PCM region is absent")?;
    let values = embedded_region
        .get("values")
        .and_then(Value::as_array)
        .ok_or("embedded PCM bytes are absent")?;
    let byte = |offset: usize| {
        values
            .get(offset)
            .and_then(Value::as_u64)
            .map(|value| value as u8)
            .ok_or("embedded PCM byte is absent")
    };
    let mut embedded_addresses = bank
        .iter()
        .filter(|tone| tone.get("kind").and_then(Value::as_str) == Some("pcm"))
        .filter_map(|tone| tone.get("sample").and_then(Value::as_str))
        .filter_map(|address| u32::from_str_radix(address.trim_start_matches("0x"), 16).ok())
        .filter(|address| (0x0811_dac8..0x0811_db38).contains(address))
        .collect::<BTreeSet<_>>()
        .into_iter()
        .collect::<Vec<_>>();
    embedded_addresses.push(0x0811_db38);
    let mut embedded_samples = Vec::new();
    for pair in embedded_addresses.windows(2) {
        let address = pair[0];
        let offset = (address - 0x0811_dac8) as usize;
        let extent = (pair[1] - address) as usize;
        if extent <= 16 {
            return Err("embedded PCM record is truncated".into());
        }
        let word = |at: usize| -> Result<u32, String> {
            Ok(u32::from_le_bytes([
                byte(offset + at)?,
                byte(offset + at + 1)?,
                byte(offset + at + 2)?,
                byte(offset + at + 3)?,
            ]))
        };
        let address_text = format!("0x{address:08x}");
        sample_addresses.insert(address_text.clone());
        embedded_samples.push(json!({
            "address": address_text,
            "control": word(0)?,
            "frequency": word(4)?,
            "loop_start": word(8)?,
            "samples": (16..extent).map(|at| byte(offset + at).map(Value::from)).collect::<Result<Vec<_>, _>>()?
        }));
    }
    let waveform_names = waveforms
        .iter()
        .filter_map(|waveform| waveform.get("name").and_then(Value::as_str))
        .collect::<BTreeSet<_>>();
    for (program, tone) in bank.iter().enumerate() {
        match tone.get("kind").and_then(Value::as_str) {
            Some("pcm") if !sample_addresses.contains(tone["sample"].as_str().unwrap_or("")) => {
                return Err(format!(
                    "music program {program} names an unknown PCM sample"
                ));
            }
            Some("wave") if !waveform_names.contains(tone["waveform"].as_str().unwrap_or("")) => {
                return Err(format!(
                    "music program {program} names an unknown CGB waveform"
                ));
            }
            Some("pcm" | "pulse_1" | "pulse_2" | "wave" | "noise" | "rhythm") => {}
            _ => return Err(format!("music program {program} has an unknown tone kind")),
        }
    }
    for program in [
        8, 24, 33, 45, 46, 47, 48, 52, 56, 61, 68, 72, 73, 75, 80, 81, 82, 83, 84, 89, 90, 91, 93,
    ] {
        if bank.get(program).is_none() {
            return Err(format!("recovered BGM program {program} is absent"));
        }
    }
    if bank[127].get("tones").and_then(Value::as_str) != Some("bank_0_092")
        || (128..=143).any(|record| bank.get(record).is_none())
    {
        return Err("recovered BGM percussion subtable is incomplete".into());
    }
    serde_json::to_vec(&json!({
        "format": 1,
        "engine": "golden-sun-rom-audio-bank",
        "bank": bank,
        "samples": samples,
        "embedded_samples": embedded_samples,
        "waveforms": waveforms
    }))
    .map_err(|error| error.to_string())
}

fn gs2_soundfont() -> Result<Vec<u8>, String> {
    let sound = gs2_sound_dir();
    let samples_text = std::fs::read_to_string(sound.join("samples.tsv"))
        .map_err(|error| format!("GS2 samples: {error}"))?;
    let mut samples = Vec::new();
    let mut sample_by_source = BTreeMap::new();
    for line in samples_text
        .lines()
        .filter(|line| !line.starts_with('#'))
        .skip(1)
    {
        let fields = line.split('\t').collect::<Vec<_>>();
        if fields.len() != 6 {
            return Err("GS2 sample row width differs".into());
        }
        let source = Path::new(fields[5])
            .file_name()
            .and_then(|name| name.to_str())
            .ok_or("GS2 sample filename differs")?;
        sample_by_source.insert(fields[5], fields[1]);
        samples.push(json!({
            "address": fields[1],
            "frequency": fields[2].parse::<u32>().map_err(|_| "GS2 sample frequency differs")?,
            "loop_start": if fields[3].is_empty() { Value::Null } else { json!(fields[3].parse::<u32>().map_err(|_| "GS2 sample loop differs")?) },
            "source": source
        }));
    }
    let waveforms = read_programmable_waveforms(&sound, 9, 3, 0x081c_4440)?;
    let tones_text = std::fs::read_to_string(sound.join("voicegroups/voicegroup_000.tsv"))
        .map_err(|error| format!("GS2 voice group: {error}"))?;
    let mut bank = Vec::new();
    for line in tones_text
        .lines()
        .filter(|line| !line.starts_with('#'))
        .skip(1)
    {
        let fields = line.split('\t').collect::<Vec<_>>();
        if fields.len() != 11 {
            return Err("GS2 voice row width differs".into());
        }
        let mut tone = json!({
            "kind": fields[1], "fixed_pitch": fields[2] == "true",
            "key": fields[3].parse::<u8>().map_err(|_| "GS2 voice key differs")?,
            "length": fields[4].parse::<u8>().map_err(|_| "GS2 voice length differs")?,
            "pan_sweep": fields[5].parse::<u8>().map_err(|_| "GS2 voice pan differs")?,
            "envelope": fields[7..11].iter().map(|value| value.parse::<u8>()).collect::<Result<Vec<_>, _>>().map_err(|_| "GS2 voice envelope differs")?
        });
        match fields[1] {
            "pcm" => {
                tone["sample"] = json!(sample_by_source
                    .get(fields[6])
                    .ok_or("GS2 voice names an unknown sample")?)
            }
            "wave" => {
                tone["waveform"] = json!(Path::new(fields[6])
                    .file_stem()
                    .and_then(|name| name.to_str())
                    .ok_or("GS2 waveform name differs")?)
            }
            "rhythm" => tone["tones"] = json!("bank_0_092"),
            "pulse_1" | "pulse_2" | "noise" => {
                tone["generator"] = json!(fields[6]
                    .parse::<u8>()
                    .map_err(|_| "GS2 generator differs")?)
            }
            _ => return Err("GS2 voice kind differs".into()),
        }
        bank.push(tone);
    }
    if bank.len() != 145 || samples.len() != 76 || waveforms.len() != 9 {
        return Err("GS2 soundfont inventory differs".into());
    }
    serde_json::to_vec(&json!({
        "format": 1, "engine": "golden-sun-rom-audio-bank", "game": "gs2",
        "bank": bank, "samples": samples, "embedded_samples": [], "waveforms": waveforms
    }))
    .map_err(|error| error.to_string())
}

fn music_file(path: &str) -> Option<PathBuf> {
    let name = path.strip_prefix("/music/")?;
    let number = name.strip_prefix("bgm_")?.strip_suffix(".mid")?;
    (number.len() == 3 && number.bytes().all(|byte| byte.is_ascii_digit()))
        .then(|| music_dir().join(name))
}
fn sample_file(path: &str) -> Option<PathBuf> {
    let name = path.strip_prefix("/music/samples/")?;
    let number = name.strip_prefix("wave_")?.strip_suffix(".pcm8.wav")?;
    (number.len() == 2 && number.bytes().all(|byte| byte.is_ascii_digit()))
        .then(|| audio_dir().join("direct_sound_samples").join(name))
}
fn gs2_music_file(path: &str) -> Option<PathBuf> {
    let name = path.strip_prefix("/music/gs2/")?;
    let number = name.strip_prefix("sound_")?.strip_suffix(".mid")?;
    (number.len() == 3 && number.bytes().all(|byte| byte.is_ascii_digit()))
        .then(|| gs2_sound_dir().join("songs/midi").join(name))
}
fn gs2_sample_file(path: &str) -> Option<PathBuf> {
    let name = path.strip_prefix("/music/gs2/samples/")?;
    let number = name.strip_prefix("sample_")?.strip_suffix(".pcm8.wav")?;
    (number.len() == 3 && number.bytes().all(|byte| byte.is_ascii_digit()))
        .then(|| gs2_sound_dir().join("direct_sound_samples").join(name))
}
fn rebuild() -> bool {
    state(|state| state.scanning = true);
    let result = compute();
    state(|state| {
        let succeeded = result.is_ok();
        match result {
            Ok(coverage) => {
                state.coverage = Some(coverage);
                state.error = None;
            }
            Err(error) => {
                if state.coverage.is_none() {
                    state.error = Some(error);
                }
            }
        }
        state.scanning = false;
        succeeded
    })
}

struct Response {
    status: u16,
    reason: &'static str,
    headers: Vec<(&'static str, String)>,
    body: Vec<u8>,
}
impl Response {
    fn new(
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
fn response(path: &str) -> Response {
    match path {
        "/" => Response::new(
            200,
            "OK",
            Some("text/html; charset=utf-8"),
            "no-store",
            format!("<!doctype html><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width,initial-scale=1\"><title>Alchemy</title><style>{}</style><script type=\"module\" src=\"/client.js?v={}\"></script>", assets::STYLES, page_version())
                .into_bytes(),
        ),
        "/client.js" => Response::new(200, "OK", Some("text/javascript; charset=utf-8"), "no-store", client::bundled_client().unwrap().into_bytes()),
        "/snapshot" => Response { status: 200, reason: "OK", headers: vec![("Cache-Control", "no-store".into()), ("Content-Type", "application/json;charset=utf-8".into())], body: snapshot_text().into_bytes() },
        "/events" => event_stream(),
        "/music/catalog" => music_catalog()
            .map(|body| Response::new(200, "OK", Some("application/json; charset=utf-8"), "no-store", body))
            .unwrap_or_else(|error| Response::new(503, "Service Unavailable", Some("text/plain; charset=utf-8"), "no-store", error.into_bytes())),
        "/music/soundfont" => soundfont()
            .map(|body| Response::new(200, "OK", Some("application/json; charset=utf-8"), "public, max-age=300", body))
            .unwrap_or_else(|error| Response::new(503, "Service Unavailable", Some("text/plain; charset=utf-8"), "no-store", error.into_bytes())),
        "/music/gs2/soundfont" => gs2_soundfont()
            .map(|body| Response::new(200, "OK", Some("application/json; charset=utf-8"), "public, max-age=300", body))
            .unwrap_or_else(|error| Response::new(503, "Service Unavailable", Some("text/plain; charset=utf-8"), "no-store", error.into_bytes())),
        "/weyard.otf" => Response::new(200, "OK", Some("font/otf"), "public, max-age=300", std::fs::read(font()).unwrap_or_default()),
        path if sample_file(path).is_some() => std::fs::read(sample_file(path).unwrap())
            .map(|body| Response::new(200, "OK", Some("audio/wav"), "public, max-age=300", body))
            .unwrap_or_else(|_| Response::new(404, "Not Found", Some("text/plain; charset=utf-8"), "no-store", b"Sample not found".to_vec())),
        path if gs2_sample_file(path).is_some() => std::fs::read(gs2_sample_file(path).unwrap())
            .map(|body| Response::new(200, "OK", Some("audio/wav"), "public, max-age=300", body))
            .unwrap_or_else(|_| Response::new(404, "Not Found", Some("text/plain; charset=utf-8"), "no-store", b"Sample not found".to_vec())),
        path if gs2_music_file(path).is_some() => std::fs::read(gs2_music_file(path).unwrap())
            .map(|body| Response::new(200, "OK", Some("audio/midi"), "public, max-age=300", body))
            .unwrap_or_else(|_| Response::new(404, "Not Found", Some("text/plain; charset=utf-8"), "no-store", b"Track not found".to_vec())),
        path if music_file(path).is_some() => std::fs::read(music_file(path).unwrap())
            .map(|body| Response::new(200, "OK", Some("audio/midi"), "public, max-age=300", body))
            .unwrap_or_else(|_| Response::new(404, "Not Found", Some("text/plain; charset=utf-8"), "no-store", b"Track not found".to_vec())),
        path if path.starts_with("/svg/") && TREES.iter().any(|(id, _)| path == format!("/svg/{id}")) => {
            let id = &path[5..];
            state(|s| {
                s.coverage
                    .as_ref()
                    .and_then(|c| c.trees.iter().find(|(k, _)| *k == id).map(|(_, v)| v.clone()))
                    .ok_or_else(|| s.error.clone().unwrap_or_else(|| "Coverage is still being read".into()))
                    .map(|s| Response::new(200, "OK", Some("image/svg+xml; charset=utf-8"), "no-store", s.into_bytes()))
                    .unwrap_or_else(|e| Response::new(503, "Service Unavailable", Some("text/plain; charset=utf-8"), "no-store", e.into_bytes()))
            })
        }
        _ => Response::new(404, "Not Found", Some("text/plain; charset=utf-8"), "no-store", b"Not found".to_vec()),
    }
}

#[derive(Clone, PartialEq)]
struct Fingerprint {
    newest: f64,
    entries: usize,
}
fn fingerprint(path: &Path) -> Fingerprint {
    let mut out = Fingerprint {
        newest: 0.0,
        entries: 0,
    };
    for entry in WalkDir::new(path).into_iter().filter_map(Result::ok) {
        let Ok(metadata) = std::fs::symlink_metadata(entry.path()) else {
            continue;
        };
        out.entries += 1;
        out.newest = out.newest.max(
            metadata
                .modified()
                .ok()
                .and_then(|time| time.duration_since(UNIX_EPOCH).ok())
                .map_or(0.0, |duration| duration.as_secs_f64() * 1000.0),
        );
    }
    out
}
struct Watcher {
    coverage: Vec<(PathBuf, Fingerprint)>,
    dirty: bool,
    stable_ticks: u8,
}
impl Watcher {
    fn new(retry_initial_scan: bool) -> Self {
        let r = root();
        let mut coverage = COVERAGE_DIRS
            .iter()
            .map(|d| r.join(d))
            .map(|p| (p.clone(), fingerprint(&p)))
            .collect::<Vec<_>>();
        for p in [
            "out/gs1-en/full/asm/manifest.json",
            "out/gs1-en/full/assets/manifest.json",
            "out/decomp/diagnose/.revision",
        ]
        .iter()
        .map(|p| r.join(p))
        {
            coverage.push((p.clone(), fingerprint(&p)))
        }
        Self {
            coverage,
            dirty: retry_initial_scan,
            stable_ticks: 0,
        }
    }
    fn tick(&mut self) {
        let mut changed = false;
        for (p, old) in &mut self.coverage {
            let now = fingerprint(p);
            changed |= now != *old;
            *old = now
        }
        if changed {
            self.dirty = true;
            self.stable_ticks = 0;
        } else if self.dirty {
            self.stable_ticks = self.stable_ticks.saturating_add(1);
            if self.stable_ticks >= 2 {
                if rebuild() {
                    self.dirty = false;
                }
                self.stable_ticks = 0;
            }
        }
    }
}

fn serve(mut stream: TcpStream) {
    let Ok((method, path)) = request(&stream) else {
        return;
    };
    let include = method != "HEAD";
    if method != "GET" && method != "HEAD" {
        let _ = Response::new(
            405,
            "Method Not Allowed",
            Some("text/plain; charset=utf-8"),
            "no-store",
            b"Method not allowed".to_vec(),
        )
        .write(&mut stream, include);
        return;
    }
    let _ = response(&path).write(&mut stream, include);
}
pub fn run(bind: Option<SocketAddr>) -> std::io::Result<()> {
    if let Ok(coverage) = cached() {
        state(|state| state.coverage = Some(coverage));
    }
    let retry_initial_scan = !rebuild();
    let listener = TcpListener::bind(bind.unwrap_or_else(|| SocketAddr::new(host(), port())))?;
    println!("Alchemy dashboard on http://{}/", listener.local_addr()?);
    std::thread::spawn(move || {
        let mut w = Watcher::new(retry_initial_scan);
        loop {
            std::thread::sleep(Duration::from_secs(1));
            w.tick();
        }
    });
    for stream in listener.incoming().flatten() {
        std::thread::spawn(|| serve(stream));
    }
    Ok(())
}

pub fn self_test() -> Result<String, String> {
    let js = client::bundled_client().map_err(|e| e.to_string())?;
    if !assets::STYLES.contains(".hover-tooltip")
        || !assets::STYLES.contains(".cards")
        || !assets::STYLES.contains(".music-player, .music-player button, .music-player output { font: var(--weyard-font); }")
        || !js.contains("EventSource")
        || !js.contains("createElementNS")
        || !js.contains("viewBox: \"0 0 24 24\"")
        || !js.contains("closest(\"g[aria-label]\")")
        || !js.contains("...trees, musicPlayer()")
        || !js.contains("AudioContext")
        || !js.contains("/music/catalog")
        || !js.contains("/music/gs2/soundfont")
        || !js.contains("createBufferSource")
        || !js.contains("if (music.ui?.card) return music.ui.card")
        || !js.contains("className: \"music-list\"")
        || !js.contains("className: \"music-chin\"")
        || !js.contains("className: \"music-wave\"")
        || !js.contains("Repeat this track")
        || !js.contains("if (status < 0xf0) running = status")
        || !js.contains("command === \"voice\"")
        || !js.contains("120547500 / value")
        || !js.contains("command === \"pitch_bend\"")
        || js.contains("function pcmPlaybackBuffer")
        || !js.contains("sample.loop_start / Math.max(3000, sample.frequency / 1024)")
    {
        return Err("dashboard assets are incomplete".into());
    }
    let shell = response("/");
    if !shell.body.starts_with(b"<!doctype html>")
        || !shell.body.windows(7).any(|window| window == b"<style>")
    {
        return Err("dashboard shell failed".into());
    }
    if response("/styles.css").status != 404 {
        return Err("standalone stylesheet route still exists".into());
    }
    if response("/nope").status != 404 {
        return Err("404 route failed".into());
    }
    let catalog = response("/music/catalog");
    if catalog.status != 200 || !catalog.body.starts_with(b"[") {
        return Err("music catalog route failed".into());
    }
    let catalog_json: Value = serde_json::from_slice(&catalog.body)
        .map_err(|error| format!("music catalog JSON: {error}"))?;
    let tracks = catalog_json
        .as_array()
        .filter(|tracks| tracks.len() == 155)
        .ok_or("music catalog does not expose both games")?;
    if tracks[0].get("game").and_then(Value::as_str) != Some("gs1")
        || tracks[0].get("soundId").and_then(Value::as_u64) != Some(0)
        || tracks[0].get("file").and_then(Value::as_str) != Some("bgm_000.mid")
        || tracks[57].get("game").and_then(Value::as_str) != Some("gs1")
        || tracks[57].get("soundId").and_then(Value::as_u64) != Some(93)
        || tracks[58].get("game").and_then(Value::as_str) != Some("gs2")
        || tracks[58].get("soundId").and_then(Value::as_u64) != Some(709)
        || tracks[59].get("soundId").and_then(Value::as_u64) != Some(68)
        || tracks[154].get("soundId").and_then(Value::as_u64) != Some(754)
        || tracks[62].get("file").and_then(Value::as_str) != Some("sound_001.mid")
    {
        return Err("music catalog does not preserve both ROM sequence inventories".into());
    }
    if tracks[58].get("title").and_then(Value::as_str) != Some("Try Your Luck!")
        || tracks[65].get("title").and_then(Value::as_str) != Some("Daila")
        || tracks[79].get("title").and_then(Value::as_str) != Some("Untitled")
        || tracks[154].get("title").and_then(Value::as_str) != Some("Battle with Dullahan")
        || tracks[..58]
            .iter()
            .filter(|track| track.get("title").and_then(Value::as_str) != Some("Untitled"))
            .count()
            != 15
        || tracks[58..]
            .iter()
            .filter(|track| track.get("title").and_then(Value::as_str) != Some("Untitled"))
            .count()
            != 96
        || tracks
            .iter()
            .any(|track| track.get("path").and_then(Value::as_str).is_none())
    {
        return Err("music catalog title evidence or recovered audio inventory differs".into());
    }
    for path in ["/music/soundfont", "/music/gs2/soundfont"] {
        let soundfont = response(path);
        if soundfont.status != 200
            || !String::from_utf8_lossy(&soundfont.body).contains("golden-sun-rom-audio-bank")
        {
            return Err(format!("music soundfont route failed: {path}"));
        }
    }
    if response("/music/gs2/samples/sample_000.pcm8.wav").status != 200
        || response("/music/gs2/samples/sample_076.pcm8.wav").status != 404
    {
        return Err("music sample route failed".into());
    }
    let events = response("/events");
    if events.status != 200
        || !events
            .headers
            .iter()
            .any(|(name, value)| *name == "Content-Type" && value.starts_with("text/event-stream"))
        || !events.body.starts_with(b"event: update\ndata: {")
    {
        return Err("event stream route failed".into());
    }
    Ok("self-test=ok dashboard".into())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn missing_correspondence_report_is_unavailable() {
        let path = std::env::temp_dir().join(format!(
            "alchemy-missing-correspondence-{}.json",
            std::process::id()
        ));
        assert!(correspondence(&path).unwrap().is_none());
    }

    #[test]
    fn snapshot_preserves_javascript_numbers_and_omits_absent_fields() {
        assert_eq!(number(1.0), json!(1));
        let snapshot = snapshot_from(&State::default());
        assert_eq!(snapshot["revision"], "starting");
        assert!(snapshot["generatedAt"].is_null());
        for field in ["error", "summary"] {
            assert!(snapshot.get(field).is_none(), "unexpected {field}");
        }
    }

    #[test]
    fn snapshot_and_event_stream_preserve_public_compatibility() {
        let state = State {
            coverage: Some(Live {
                revision: "revision".into(),
                generated: "2001-08-27T12:34:56.789Z".into(),
                trees: Vec::new(),
                executable: 1.0,
                proven_c: 1.0,
                proven_c_percent: 100.0,
                draft_c: 0.0,
                draft_c_percent: 0.0,
                proven_asm: 0.0,
                gs1_ja_sources: 0,
                gs1_en_sources: 0,
                gs2_ja_sources: 0,
                gs2_en_sources: 0,
                correspondence: None,
            }),
            error: None,
            scanning: false,
        };
        assert_eq!(
            snapshot_from(&state)["generatedAt"],
            "2001-08-27T12:34:56.789Z"
        );
        let response = event_stream();
        assert!(
            response
                .headers
                .iter()
                .any(|(name, value)| *name == "Content-Type"
                    && value.starts_with("text/event-stream"))
        );
        assert!(String::from_utf8(response.body)
            .unwrap()
            .starts_with("event: update\ndata: {"));
    }

    #[test]
    fn music_player_keeps_one_native_pixel_font_size() {
        assert!(assets::STYLES.contains(
            ".music-player, .music-player button, .music-player output { font: var(--weyard-font); }"
        ));
        for rule in assets::STYLES
            .lines()
            .filter(|line| line.trim_start().starts_with(".music"))
        {
            assert!(
                !rule.contains("font-size:"),
                "music player must inherit the one native Weyard size: {rule}"
            );
        }
        let client = client::bundled_client().unwrap();
        assert!(client.contains("volume: 1,"));
        assert!(!client.contains("music-volume"));
        assert!(client.contains("Repeat this track"));
        assert!(!client.contains("function pcmPlaybackBuffer"));
    }

    #[test]
    fn music_player_tunes_compact_rom_waves_to_the_voice_root() {
        let client = client::bundled_client().unwrap();
        assert!(client.contains("if (sample.embedded)"));
        assert!(client.contains("source.buffer.sampleRate / source.buffer.length"));
        assert!(client.contains("baseRate = rootFrequency / rawCycleFrequency"));
    }
}
