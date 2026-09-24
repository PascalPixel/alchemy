//! The tracked daily DONE history behind PROGRESS_CHART.png: one row per
//! calendar day, the last measurement of the day winning. Early rows were
//! seeded once from main's first-parent history and hold only the published
//! percentage; measured rows hold verified bytes.
use super::progress::GameDone;
use super::sessions;
use crate::compiler::canonical_json::canonical_json;
use serde_json::{json, Value};
use std::collections::BTreeMap;
use std::path::{Path, PathBuf};

pub(crate) const PATH: &str = "recon/tbs/metrics/history.json";

pub(crate) fn path(root: &Path) -> PathBuf {
    root.join(PATH)
}
pub(crate) fn load(root: &Path) -> Result<Value, String> {
    let bytes = std::fs::read(path(root)).map_err(|e| format!("{PATH}: {e}"))?;
    serde_json::from_slice(&bytes).map_err(|e| format!("{PATH}: {e}"))
}
pub(crate) fn text(history: &Value) -> String {
    format!("{}\n", canonical_json(history))
}

/// One game's value on one day, as a percentage.
pub(crate) fn percent(entry: &Value) -> Option<f64> {
    if let Some(percent) = entry["percent"].as_f64() {
        return Some(percent);
    }
    let (done, executable) = (entry["done"].as_f64()?, entry["executable"].as_f64()?);
    (executable > 0.0).then(|| 100.0 * done / executable)
}
fn measured(done: Option<GameDone>) -> Option<Value> {
    done.filter(|done| done.executable > 0)
        .map(|done| json!({"done": done.bytes(), "executable": done.executable}))
}

/// Record `date`'s verified counts, replacing any earlier row of that day.
/// A game without a verified count keeps what the day already had.
pub(crate) fn record(
    history: &mut Value,
    date: &str,
    sun: Option<GameDone>,
    anchor: Option<GameDone>,
) {
    let days = history["days"]
        .as_array_mut()
        .expect("history has a days array");
    let at = days.partition_point(|row| row["date"].as_str().unwrap_or("") < date);
    if days.get(at).and_then(|row| row["date"].as_str()) != Some(date) {
        days.insert(at, json!({"date": date}));
    }
    let row = days[at].as_object_mut().expect("history rows are objects");
    for (key, value) in [("tbs", measured(sun)), ("tla", measured(anchor))] {
        if let Some(value) = value {
            row.insert(key.into(), value);
        }
    }
    if row.len() == 1 {
        days.remove(at);
    }
}
/// The label of a commit that names no model.
pub(crate) const UNTAGGED: &str = "Untagged";
/// `date`'s model counts from every branch of the local repository, each
/// commit labelled as `sessions::label` infers it.
pub(crate) fn models_on(root: &Path, date: &str) -> Result<BTreeMap<String, u64>, String> {
    let since = previous(date).unwrap_or_default();
    let log = sessions::git_log(root, Some(&since))?;
    if log.trim().is_empty() {
        return Ok(BTreeMap::new());
    }
    let from = day_number(&since).unwrap_or_default() * 86_400 - 86_400;
    let activity = sessions::activity(&sessions::home(), from);
    let (mut days, _) = sessions::tally(&log, &activity, &sessions::checkouts(root));
    Ok(days.remove(date).unwrap_or_default())
}
/// Relabel every day's commits from the whole log and the agent logs,
/// returning how many commits changed label, from → to.
pub(crate) fn relabel_models(
    root: &Path,
    history: &mut Value,
) -> Result<BTreeMap<(String, String), u64>, String> {
    let log = sessions::git_log(root, None)?;
    let activity = sessions::activity(&sessions::home(), 0);
    let (days, moved) = sessions::tally(&log, &activity, &sessions::checkouts(root));
    let began = history["began"].as_str().unwrap_or("").to_string();
    for (date, models) in days.iter().filter(|(date, _)| **date >= began) {
        record_models(history, date, models);
    }
    Ok(moved)
}
/// Replace `date`'s model counts, adding the day's row if it lacks one.
pub(crate) fn record_models(history: &mut Value, date: &str, models: &BTreeMap<String, u64>) {
    if models.is_empty() {
        return;
    }
    let days = history["days"]
        .as_array_mut()
        .expect("history has a days array");
    let at = days.partition_point(|row| row["date"].as_str().unwrap_or("") < date);
    if days.get(at).and_then(|row| row["date"].as_str()) != Some(date) {
        days.insert(at, json!({"date": date}));
    }
    days[at]["models"] = json!(models);
}
/// The history as the chart drawn on `date` saw it: every earlier day, and
/// that day's row as recorded when the figures were drawn.
pub(crate) fn as_drawn(history: &Value) -> Value {
    let mut drawn = history.clone();
    let date = history["figures"]["date"]
        .as_str()
        .unwrap_or("")
        .to_string();
    if let Some(days) = drawn["days"].as_array_mut() {
        days.retain(|row| row["date"].as_str().unwrap_or("") < date.as_str());
        let mut row = history["figures"].clone();
        if let Some(fields) = row.as_object_mut() {
            fields.remove("files");
            if fields.len() > 1 {
                days.push(row);
            }
        }
    }
    drawn
}
/// Note the day the figures were drawn, the row they showed and the digest
/// of the tracked files the map drew.
pub(crate) fn mark_drawn(history: &mut Value, date: &str, files: &str) {
    let mut row = history["days"]
        .as_array()
        .and_then(|days| days.iter().find(|row| row["date"] == date))
        .cloned()
        .unwrap_or_else(|| json!({"date": date}));
    row["files"] = json!(files);
    history["figures"] = row;
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
    fn done(bytes: i64) -> GameDone {
        GameDone {
            game_c: bytes,
            executable: 1000,
            ..GameDone::default()
        }
    }
    #[test]
    fn a_day_keeps_its_last_measurement_and_the_drawn_row_stays_fixed() {
        let mut history = json!({"format": 1, "days": [
            {"date": "2026-09-22", "tbs": {"percent": 55.0}},
            {"date": "2026-09-24", "tbs": {"percent": 56.0}}
        ]});
        record(&mut history, "2026-09-23", Some(done(500)), None);
        record(&mut history, "2026-09-24", Some(done(600)), Some(done(20)));
        mark_drawn(&mut history, "2026-09-24", "digest");
        record(&mut history, "2026-09-24", Some(done(610)), None);
        let days = history["days"].as_array().unwrap();
        assert_eq!(
            days.iter()
                .map(|row| row["date"].as_str().unwrap())
                .collect::<Vec<_>>(),
            ["2026-09-22", "2026-09-23", "2026-09-24"]
        );
        assert_eq!(percent(&days[2]["tbs"]), Some(61.0));
        assert_eq!(percent(&days[2]["tla"]), Some(2.0));
        let drawn = as_drawn(&history);
        assert_eq!(percent(&drawn["days"][2]["tbs"]), Some(60.0));
        assert!(drawn["days"][2].get("files").is_none());
        record(&mut history, "2026-09-25", None, None);
        assert_eq!(history["days"].as_array().unwrap().len(), 3);
    }
    #[test]
    fn a_day_takes_its_model_counts() {
        let mut history = json!({"days": [{"date": "2026-09-24", "tbs": {"percent": 1.0}}]});
        let models = BTreeMap::from([("Opus 5.5".to_string(), 3), (UNTAGGED.to_string(), 1)]);
        record_models(&mut history, "2026-09-24", &models);
        record_models(&mut history, "2026-09-23", &models);
        assert_eq!(history["days"][1]["models"]["Opus 5.5"], 3);
        assert_eq!(history["days"][0]["date"], "2026-09-23");
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
