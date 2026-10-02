//! The tracked daily DONE history behind PROGRESS_CHART.png: one row per
//! calendar day, the last measurement of the day winning. Early rows were
//! seeded once from main's first-parent history and hold only the published
//! percentage; later rows retain bytes as published at the time: the English
//! build's alone at first, and from the row whose note says so the sum over
//! a game's six editions out of six times the English executable bytes.
//! Current verification status is recorded separately from those historical
//! values.
use super::progress::GameDone;
use super::sessions;
use std::collections::BTreeMap;
use std::path::{Path, PathBuf};

pub(crate) const PATH: &str = "recon/tbs/metrics/history.tsv";
const HEADER: &str = "date\ttbs_done\ttbs_executable\ttbs_percent\ttla_done\ttla_executable\ttla_percent\tmodels\toverhaul";

/// One game's value on one day: bytes when they were recorded, else the
/// percentage published at the time.
#[derive(Clone, Debug, Default, PartialEq)]
pub(crate) struct Measure {
    pub done: Option<u64>,
    pub executable: Option<u64>,
    pub percent: Option<f64>,
}
impl Measure {
    pub(crate) fn bytes(done: u64, executable: u64) -> Self {
        Measure {
            done: Some(done),
            executable: Some(executable),
            percent: None,
        }
    }
    pub(crate) fn published(percent: f64) -> Self {
        Measure {
            percent: Some(percent),
            ..Measure::default()
        }
    }
    /// The value as a percentage.
    pub(crate) fn percent(&self) -> Option<f64> {
        if let Some(percent) = self.percent {
            return Some(percent);
        }
        let (done, executable) = (self.done? as f64, self.executable? as f64);
        (executable > 0.0).then(|| 100.0 * done / executable)
    }
}

/// One calendar day's row.
#[derive(Clone, Debug, Default, PartialEq)]
pub(crate) struct Day {
    pub date: String,
    pub tbs: Option<Measure>,
    pub tla: Option<Measure>,
    pub models: BTreeMap<String, u64>,
    pub correction: Option<String>,
}
impl Day {
    pub(crate) fn new(date: &str) -> Self {
        Day {
            date: date.to_string(),
            ..Day::default()
        }
    }
    /// The day's value for `game`, `tbs` or `tla`.
    pub(crate) fn game(&self, game: &str) -> Option<&Measure> {
        match game {
            "tbs" => self.tbs.as_ref(),
            _ => self.tla.as_ref(),
        }
    }
    /// Whether the row carries nothing but its date.
    fn is_empty(&self) -> bool {
        self.tbs.is_none()
            && self.tla.is_none()
            && self.models.is_empty()
            && self.correction.is_none()
    }
}

/// The current verification state: the day it was measured and whether each
/// game still waits for a byte-identical build.
#[derive(Clone, Debug, PartialEq)]
pub(crate) struct Current {
    pub date: String,
    pub tbs_pending: bool,
    pub tla_pending: bool,
}

/// The whole history, and the row the committed figures were drawn from.
#[derive(Clone, Debug, Default, PartialEq)]
pub(crate) struct History {
    pub began: String,
    pub current: Option<Current>,
    pub days: Vec<Day>,
    pub figures: Option<Day>,
}
impl History {
    /// Whether `game` has no verified current measurement.
    pub(crate) fn pending(&self, game: &str) -> bool {
        self.current.as_ref().is_some_and(|current| match game {
            "tbs" => current.tbs_pending,
            _ => current.tla_pending,
        })
    }
    /// The days whose credit rules grew stricter, with their notes.
    #[cfg(test)]
    pub(crate) fn stricter(&self) -> impl Iterator<Item = (&str, &str)> {
        self.days.iter().filter_map(|day| {
            day.correction
                .as_deref()
                .map(|note| (day.date.as_str(), note))
        })
    }
    fn day_mut(&mut self, date: &str) -> &mut Day {
        let at = self.days.partition_point(|row| row.date.as_str() < date);
        if self.days.get(at).map(|row| row.date.as_str()) != Some(date) {
            self.days.insert(at, Day::new(date));
        }
        &mut self.days[at]
    }
}

pub(crate) fn path(root: &Path) -> PathBuf {
    root.join(PATH)
}
pub(crate) fn load(root: &Path) -> Result<History, String> {
    let text = std::fs::read_to_string(path(root)).map_err(|e| format!("{PATH}: {e}"))?;
    parse(&text).map_err(|error| format!("{PATH}: {error}"))
}
pub(crate) fn text(history: &History) -> String {
    let mut text = format!(
        "# Published progress; '-' means no value was recorded.\n# began\t{}\n",
        history.began
    );
    if let Some(current) = &history.current {
        let state = |pending: bool| if pending { "pending" } else { "verified" };
        text.push_str(&format!(
            "# current\t{}\t{}\t{}\n",
            current.date,
            state(current.tbs_pending),
            state(current.tla_pending)
        ));
    }
    text.push_str(HEADER);
    text.push('\n');
    for day in &history.days {
        let fields = |measure: Option<&Measure>| {
            let show = |value: Option<String>| value.unwrap_or_else(|| "-".into());
            [
                show(measure.and_then(|m| m.done).map(|v| v.to_string())),
                show(measure.and_then(|m| m.executable).map(|v| v.to_string())),
                show(measure.and_then(|m| m.percent).map(|v| format!("{v:?}"))),
            ]
        };
        let models = day
            .models
            .iter()
            .map(|(name, count)| format!("{name}={count}"))
            .collect::<Vec<_>>()
            .join(";");
        let note = day
            .correction
            .as_deref()
            .unwrap_or("-")
            .replace(['\t', '\n', '\r'], " ");
        let mut row = vec![day.date.clone()];
        row.extend(fields(day.tbs.as_ref()));
        row.extend(fields(day.tla.as_ref()));
        row.push(if models.is_empty() {
            "-".into()
        } else {
            models
        });
        row.push(note);
        text.push_str(&row.join("\t"));
        text.push('\n');
    }
    text
}

fn parse(text: &str) -> Result<History, String> {
    let mut began = None;
    let mut header = false;
    let mut days: Vec<Day> = Vec::new();
    let mut current = None;
    let mut previous = String::new();
    for (index, line) in text.lines().enumerate() {
        let problem = |why: &str| format!("line {}: {why}", index + 1);
        if let Some(date) = line.strip_prefix("# began\t") {
            began = Some(date.to_string());
            continue;
        }
        if let Some(fields) = line.strip_prefix("# current\t") {
            let fields = fields.split('\t').collect::<Vec<_>>();
            if fields.len() != 3
                || day_number(fields[0]).is_none()
                || fields[1..]
                    .iter()
                    .any(|state| !matches!(*state, "verified" | "pending"))
            {
                return Err(problem(
                    "current status needs date and verified/pending for each game",
                ));
            }
            current = Some(Current {
                date: fields[0].to_string(),
                tbs_pending: fields[1] == "pending",
                tla_pending: fields[2] == "pending",
            });
            continue;
        }
        if line.starts_with('#') || line.is_empty() {
            continue;
        }
        if !header {
            if line != HEADER {
                return Err(problem("unexpected progress columns"));
            }
            header = true;
            continue;
        }
        let fields = line.split('\t').collect::<Vec<_>>();
        if fields.len() != 9 {
            return Err(problem("expected nine progress columns"));
        }
        let date = fields[0];
        let valid_date = day_number(date)
            .map(|day| {
                let (year, month, day) = civil(day);
                format!("{year:04}-{month:02}-{day:02}") == date
            })
            .unwrap_or(false);
        if !valid_date || date <= previous.as_str() {
            return Err(problem("dates must be valid and strictly increasing"));
        }
        previous = date.to_string();
        let mut row = Day::new(date);
        for (game, start) in [("tbs", 1), ("tla", 4)] {
            let (done, total, percent) = (fields[start], fields[start + 1], fields[start + 2]);
            let measure = if done != "-" || total != "-" {
                let done = done
                    .parse::<u64>()
                    .map_err(|_| problem("invalid DONE bytes"))?;
                let total = total
                    .parse::<u64>()
                    .map_err(|_| problem("invalid executable bytes"))?;
                if total == 0 || done > total || percent != "-" {
                    return Err(problem(
                        "record bytes or a published percentage, never both",
                    ));
                }
                Some(Measure::bytes(done, total))
            } else if percent != "-" {
                let percent = percent
                    .parse::<f64>()
                    .map_err(|_| problem("invalid published percentage"))?;
                if !percent.is_finite() || !(0.0..=100.0).contains(&percent) {
                    return Err(problem("published percentage is outside 0–100"));
                }
                Some(Measure::published(percent))
            } else {
                None
            };
            if game == "tbs" {
                row.tbs = measure;
            } else {
                row.tla = measure;
            }
        }
        if fields[7] != "-" {
            for field in fields[7].split(';') {
                let (name, count) = field
                    .rsplit_once('=')
                    .ok_or_else(|| problem("model counts use name=count"))?;
                let count = count
                    .parse::<u64>()
                    .map_err(|_| problem("invalid model count"))?;
                if name.is_empty() || count == 0 || row.models.insert(name.into(), count).is_some()
                {
                    return Err(problem("model counts must be positive and distinct"));
                }
            }
        }
        if fields[8] != "-" {
            row.correction = Some(fields[8].to_string());
        }
        days.push(row);
    }
    if !header {
        return Err("missing progress columns".into());
    }
    let began = began
        .or_else(|| days.first().map(|row| row.date.clone()))
        .ok_or("history has no beginning date")?;
    let figures = days.last().cloned();
    Ok(History {
        began,
        current,
        days,
        figures,
    })
}

fn measured(done: Option<GameDone>) -> Option<Measure> {
    done.filter(|done| done.executable > 0)
        .map(|done| Measure::bytes(done.bytes() as u64, done.executable as u64))
}

/// Record `date`'s verified counts, replacing any earlier row of that day.
/// A game without a verified count keeps what the day already had.
pub(crate) fn record(
    history: &mut History,
    date: &str,
    sun: Option<GameDone>,
    anchor: Option<GameDone>,
) {
    let pending =
        [sun.as_ref(), anchor.as_ref()].map(|done| done.is_none_or(|done| done.executable <= 0));
    history.current = Some(Current {
        date: date.to_string(),
        tbs_pending: pending[0],
        tla_pending: pending[1],
    });
    let row = history.day_mut(date);
    if let Some(value) = measured(sun) {
        row.tbs = Some(value);
    }
    if let Some(value) = measured(anchor) {
        row.tla = Some(value);
    }
    if row.is_empty() {
        history.days.retain(|row| row.date != date);
    }
}
/// The label of a commit that names no model.
pub(crate) const UNTAGGED: &str = "Untagged";
/// `date`'s model counts from every branch of the local repository, each
/// commit labelled as `sessions::label` names it.
pub(crate) fn models_on(root: &Path, date: &str) -> Result<BTreeMap<String, u64>, String> {
    let since = previous(date).unwrap_or_default();
    let log = sessions::git_log(root, Some(&since))?;
    if log.trim().is_empty() {
        return Ok(BTreeMap::new());
    }
    let (mut days, _) = sessions::tally(&log);
    Ok(days.remove(date).unwrap_or_default())
}

/// Publication retains approved attribution and adds only the commits that
/// have landed since that day's maintained count.
pub(crate) fn publication_models(
    root: &Path,
    history: &History,
    date: &str,
) -> Result<BTreeMap<String, u64>, String> {
    let mut models = history
        .days
        .iter()
        .find(|row| row.date == date)
        .map(|row| row.models.clone())
        .unwrap_or_default();
    let since = previous(date).unwrap_or_default();
    let log = sessions::git_log(root, Some(&since))?;
    let mut commits = sessions::commits(&log)
        .into_iter()
        .filter(|commit| commit.date == date)
        .collect::<Vec<_>>();
    commits.sort_by(|left, right| right.time.cmp(&left.time).then(right.sha.cmp(&left.sha)));
    append_models(&mut models, &commits);
    Ok(models)
}

fn append_models(models: &mut BTreeMap<String, u64>, commits: &[sessions::Commit]) {
    let labels = commits
        .iter()
        .map(|commit| sessions::label(commit).0)
        .collect::<Vec<_>>();
    let total = labels.iter().map(|labels| labels.len() as u64).sum::<u64>();
    let missing = total.saturating_sub(models.values().sum());
    for model in labels.iter().flatten().take(missing as usize) {
        *models.entry(model.clone()).or_default() += 1;
    }
}
/// Relabel every day's commits from the whole log, returning how many
/// commits changed label, from → to.
pub(crate) fn relabel_models(
    root: &Path,
    history: &mut History,
) -> Result<BTreeMap<(String, String), u64>, String> {
    let log = sessions::git_log(root, None)?;
    let (days, moved) = sessions::tally(&log);
    let began = history.began.clone();
    for (date, models) in days.iter().filter(|(date, _)| **date >= began) {
        record_models(history, date, models);
    }
    Ok(moved)
}
/// Replace `date`'s model counts, adding the day's row if it lacks one.
pub(crate) fn record_models(history: &mut History, date: &str, models: &BTreeMap<String, u64>) {
    if models.is_empty() {
        return;
    }
    history.day_mut(date).models = models.clone();
}
/// The history as the chart drawn on `date` saw it: every earlier day, and
/// that day's row as recorded when the figures were drawn.
pub(crate) fn as_drawn(history: &History) -> History {
    let mut drawn = history.clone();
    let Some(figures) = &history.figures else {
        return drawn;
    };
    drawn.days.retain(|row| row.date < figures.date);
    if !figures.is_empty() {
        drawn.days.push(figures.clone());
    }
    drawn
}
/// Keep the row a figure shows while the caller renders it.
pub(crate) fn mark_drawn(history: &mut History, date: &str) {
    history.figures = Some(
        history
            .days
            .iter()
            .find(|row| row.date == date)
            .cloned()
            .unwrap_or_else(|| Day::new(date)),
    );
}

/// Today's calendar date on this machine, `YYYY-MM-DD`.
pub(crate) fn today() -> String {
    let now = std::time::SystemTime::now()
        .duration_since(std::time::UNIX_EPOCH)
        .map_or(0, |elapsed| elapsed.as_secs() as libc::time_t);
    // SAFETY: localtime_r only writes the zeroed `tm` it is given.
    let mut tm: libc::tm = unsafe { std::mem::zeroed() };
    unsafe { libc::localtime_r(&now, &mut tm) };
    format!(
        "{:04}-{:02}-{:02}",
        tm.tm_year + 1900,
        tm.tm_mon + 1,
        tm.tm_mday
    )
}
/// Days from the civil date `YYYY-MM-DD` to 1970-01-01.
pub(crate) fn day_number(date: &str) -> Option<i64> {
    let mut parts = date.splitn(3, '-').map(|part| part.parse::<i64>().ok());
    let (y, m, d) = (parts.next()??, parts.next()??, parts.next()??);
    let y = if m <= 2 { y - 1 } else { y };
    let era = y.div_euclid(400);
    let yoe = y - era * 400;
    let doy = (153 * (m + if m > 2 { -3 } else { 9 }) + 2) / 5 + d - 1;
    let doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    Some(era * 146_097 + doe - 719_468)
}
/// The civil date of a day number.
pub(crate) fn civil(day: i64) -> (i64, i64, i64) {
    let mut day = day + 719_468;
    let era = day.div_euclid(146_097);
    day -= era * 146_097;
    let yoe = (day - day / 1460 + day / 36_524 - day / 146_096) / 365;
    let doy = day - (365 * yoe + yoe / 4 - yoe / 100);
    let mp = (5 * doy + 2) / 153;
    let d = doy - (153 * mp + 2) / 5 + 1;
    let m = if mp < 10 { mp + 3 } else { mp - 9 };
    (yoe + era * 400 + i64::from(m <= 2), m, d)
}
/// The day before `date`.
pub(crate) fn previous(date: &str) -> Option<String> {
    let (y, m, d) = civil(day_number(date)? - 1);
    Some(format!("{y:04}-{m:02}-{d:02}"))
}
/// A day's short label, such as "Sep 1", and whether it starts a month.
pub(crate) fn day_label(day: i64) -> (String, bool) {
    const MONTHS: [&str; 12] = [
        "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
    ];
    let (_, m, d) = civil(day);
    (format!("{} {d}", MONTHS[(m - 1) as usize]), d == 1)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn day(date: &str, tbs: Option<Measure>, tla: Option<Measure>) -> Day {
        Day {
            tbs,
            tla,
            ..Day::new(date)
        }
    }

    #[test]
    fn pending_current_status_preserves_published_measurements_and_corrections() {
        let mut history = History {
            began: "2026-09-27".into(),
            days: vec![Day {
                models: BTreeMap::from([("Astra 6".into(), 17), ("Sol 6".into(), 8)]),
                correction: Some("Executable audit pending after cleanup".into()),
                ..day(
                    "2026-09-28",
                    Some(Measure::bytes(1013978, 1375934)),
                    Some(Measure::bytes(42332, 1975640)),
                )
            }],
            ..History::default()
        };
        let published = history.days.clone();
        record(&mut history, "2026-09-28", None, None);
        mark_drawn(&mut history, "2026-09-28");
        let loaded = parse(&text(&history)).unwrap();
        assert_eq!(loaded.days, published);
        assert!(loaded.pending("tbs") && loaded.pending("tla"));
        assert_eq!(
            loaded.stricter().collect::<Vec<_>>(),
            [("2026-09-28", "Executable audit pending after cleanup")]
        );
    }

    #[test]
    fn archived_percentages_stay_percentages_without_invented_byte_counts() {
        let history = History {
            began: "2026-07-16".into(),
            days: vec![day("2026-07-16", Some(Measure::published(1.0)), None)],
            ..History::default()
        };
        let text = text(&history);
        assert!(text.contains("2026-07-16\t-\t-\t1.0\t"), "{text}");
        let loaded = parse(&text).unwrap();
        assert_eq!(loaded.days[0].tbs, Some(Measure::published(1.0)));
        let invalid = format!("{HEADER}\n2026-07-16\t10\t100\t10\t-\t-\t-\t-\t-\n");
        assert!(parse(&invalid).is_err());
    }

    #[test]
    fn publication_keeps_approved_models_and_adds_actual_new_trailers() {
        let mut models = BTreeMap::from([("Astra 6".into(), 17), ("Sol 6".into(), 9)]);
        let old = |time| sessions::Commit {
            time,
            author: "Pascal Pixel".into(),
            ..sessions::Commit::default()
        };
        let mut commits = (0..26).map(|_| old(100)).collect::<Vec<_>>();
        let mut new = old(200);
        new.trailers = "Sol 6 <agent@example.com>".into();
        commits.insert(0, new);
        append_models(&mut models, &commits);
        assert_eq!(
            models,
            BTreeMap::from([("Astra 6".into(), 17), ("Sol 6".into(), 10)])
        );
        append_models(&mut models, &commits);
        assert_eq!(models["Sol 6"], 10);
        assert!(!models.contains_key("Sol 5.6"));
    }

    #[test]
    fn publication_keeps_new_commits_without_a_named_model_untagged() {
        let mut models = BTreeMap::new();
        let commit = sessions::Commit {
            author: "Pascal Pixel".into(),
            ..sessions::Commit::default()
        };
        append_models(&mut models, &[commit]);
        assert_eq!(models, BTreeMap::from([(UNTAGGED.into(), 1)]));
    }
    fn done(bytes: i64) -> GameDone {
        GameDone {
            game_c: bytes,
            executable: 1000,
            ..GameDone::default()
        }
    }
    #[test]
    fn a_day_keeps_its_last_measurement_and_the_drawn_row_stays_fixed() {
        let mut history = History {
            days: vec![
                day("2026-09-22", Some(Measure::published(55.0)), None),
                day("2026-09-24", Some(Measure::published(56.0)), None),
            ],
            ..History::default()
        };
        record(&mut history, "2026-09-23", Some(done(500)), None);
        record(&mut history, "2026-09-24", Some(done(600)), Some(done(20)));
        mark_drawn(&mut history, "2026-09-24");
        record(&mut history, "2026-09-24", Some(done(610)), None);
        assert_eq!(
            history
                .days
                .iter()
                .map(|row| row.date.as_str())
                .collect::<Vec<_>>(),
            ["2026-09-22", "2026-09-23", "2026-09-24"]
        );
        let percent = |day: &Day, game| day.game(game).and_then(Measure::percent);
        assert_eq!(percent(&history.days[2], "tbs"), Some(61.0));
        assert_eq!(percent(&history.days[2], "tla"), Some(2.0));
        let drawn = as_drawn(&history);
        assert_eq!(percent(&drawn.days[2], "tbs"), Some(60.0));
        record(&mut history, "2026-09-25", None, None);
        assert_eq!(history.days.len(), 3);
    }
    #[test]
    fn a_day_takes_its_model_counts() {
        let mut history = History {
            days: vec![day("2026-09-24", Some(Measure::published(1.0)), None)],
            ..History::default()
        };
        let models = BTreeMap::from([("Opus 5.5".to_string(), 3), (UNTAGGED.to_string(), 1)]);
        record_models(&mut history, "2026-09-24", &models);
        record_models(&mut history, "2026-09-23", &models);
        assert_eq!(history.days[1].models["Opus 5.5"], 3);
        assert_eq!(history.days[0].date, "2026-09-23");
    }
    #[test]
    fn civil_dates_count_days() {
        assert_eq!(day_number("1970-01-01"), Some(0));
        assert_eq!(
            day_number("2026-09-24").unwrap() - day_number("2026-07-16").unwrap(),
            70
        );
        assert_eq!(previous("2026-03-01").as_deref(), Some("2026-02-28"));
        assert_eq!(previous("2026-09-01").as_deref(), Some("2026-08-31"));
        assert_eq!(today().len(), 10);
    }
}
#[test]
fn the_tracked_history_reads_back_byte_for_byte() {
    let path = path(crate::compiler::routing::root());
    let tracked = std::fs::read_to_string(path).unwrap();
    assert_eq!(text(&parse(&tracked).unwrap()), tracked);
}
