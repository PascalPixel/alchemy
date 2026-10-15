use super::calcrom::GameDone;
use std::path::Path;

const HEADER: &str = "date\ttbs_done\ttbs_executable\ttbs_percent\ttbs_data\ttbs_data_total\ttla_done\ttla_executable\ttla_percent\ttla_data\ttla_data_total\toverhaul";
/// The columns while each day also counted its commits by model, read and
/// rewritten without that column.
const MODELS_HEADER: &str = "date\ttbs_done\ttbs_executable\ttbs_percent\ttbs_data\ttbs_data_total\ttla_done\ttla_executable\ttla_percent\ttla_data\ttla_data_total\tmodels\toverhaul";
/// The columns before DONE data (Pascal, 2026-10-08), read and rewritten
/// with the data columns left empty and without the model counts.
const CODE_ONLY_HEADER: &str = "date\ttbs_done\ttbs_executable\ttbs_percent\ttla_done\ttla_executable\ttla_percent\tmodels\toverhaul";
/// The note on the first day that records DONE data.
pub(crate) const DATA_BEGINS: &str = "DONE is code and data from here (Pascal, 2026-10-08)";

#[derive(Clone, Debug, Default, PartialEq)]
pub(crate) struct Measure {
    pub done: Option<u64>,
    pub executable: Option<u64>,
    pub percent: Option<f64>,
    /// DONE data bytes and the data total: measured from 2026-10-08, and
    /// rebuilt from each earlier day's own source back to the first.
    pub data: Option<u64>,
    pub data_total: Option<u64>,
}
impl Measure {
    pub(crate) fn bytes(done: u64, executable: u64) -> Self {
        Measure {
            done: Some(done),
            executable: Some(executable),
            percent: None,
            data: None,
            data_total: None,
        }
    }
    pub(crate) fn data_percent(&self) -> Option<f64> {
        let (data, total) = (self.data? as f64, self.data_total? as f64);
        (total > 0.0).then(|| 100.0 * data / total)
    }
    pub(crate) fn published(percent: f64) -> Self {
        Measure {
            percent: Some(percent),
            ..Measure::default()
        }
    }
    pub(crate) fn percent(&self) -> Option<f64> {
        if let Some(percent) = self.percent {
            return Some(percent);
        }
        let (done, executable) = (self.done? as f64, self.executable? as f64);
        (executable > 0.0).then(|| 100.0 * done / executable)
    }
}

#[derive(Clone, Debug, Default, PartialEq)]
pub(crate) struct Day {
    pub date: String,
    pub tbs: Option<Measure>,
    pub tla: Option<Measure>,
    pub correction: Option<String>,
}
impl Day {
    pub(crate) fn new(date: &str) -> Self {
        Day {
            date: date.to_string(),
            ..Day::default()
        }
    }
    pub(crate) fn game(&self, game: &str) -> Option<&Measure> {
        match game {
            "tbs" => self.tbs.as_ref(),
            _ => self.tla.as_ref(),
        }
    }
    fn is_empty(&self) -> bool {
        self.tbs.is_none()
            && self.tla.is_none()
            && self.correction.is_none()
    }
}

#[derive(Clone, Debug, PartialEq)]
pub(crate) struct Current {
    pub date: String,
    pub tbs_pending: bool,
    pub tla_pending: bool,
}

#[derive(Clone, Debug, Default, PartialEq)]
pub(crate) struct History {
    pub began: String,
    pub current: Option<Current>,
    pub days: Vec<Day>,
    pub hours: Vec<Day>,
    pub figures: Option<Day>,
}
impl History {
    pub(crate) fn pending(&self, game: &str) -> bool {
        self.current.as_ref().is_some_and(|current| match game {
            "tbs" => current.tbs_pending,
            _ => current.tla_pending,
        })
    }
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

pub(crate) fn load_file(path: &Path) -> Result<History, String> {
    let name = path.display();
    let text = std::fs::read_to_string(path).map_err(|e| format!("{name}: {e}"))?;
    parse(&text).map_err(|error| format!("{name}: {error}"))
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
    let mut rows = history
        .days
        .iter()
        .chain(&history.hours)
        .collect::<Vec<_>>();
    rows.sort_by_key(|row| &row.date);
    for day in rows {
        let fields = |measure: Option<&Measure>| {
            let show = |value: Option<String>| value.unwrap_or_else(|| "-".into());
            [
                show(measure.and_then(|m| m.done).map(|v| v.to_string())),
                show(measure.and_then(|m| m.executable).map(|v| v.to_string())),
                show(measure.and_then(|m| m.percent).map(|v| format!("{v:?}"))),
                show(measure.and_then(|m| m.data).map(|v| v.to_string())),
                show(measure.and_then(|m| m.data_total).map(|v| v.to_string())),
            ]
        };
        let note = day
            .correction
            .as_deref()
            .unwrap_or("-")
            .replace(['\t', '\n', '\r'], " ");
        let mut row = vec![day.date.clone()];
        row.extend(fields(day.tbs.as_ref()));
        row.extend(fields(day.tla.as_ref()));
        row.push(note);
        text.push_str(&row.join("\t"));
        text.push('\n');
    }
    text
}

fn parse(text: &str) -> Result<History, String> {
    let mut began = None;
    let mut header = None;
    let mut days: Vec<Day> = Vec::new();
    let mut hours = Vec::new();
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
        let Some(columns) = header else {
            header = Some(match line {
                HEADER => HEADER,
                MODELS_HEADER => MODELS_HEADER,
                CODE_ONLY_HEADER => CODE_ONLY_HEADER,
                _ => return Err(problem("unexpected progress columns")),
            });
            continue;
        };
        let mut fields = line.split('\t').collect::<Vec<_>>();
        if columns == CODE_ONLY_HEADER && fields.len() == 9 {
            fields.splice(7..7, ["-", "-"]);
            fields.splice(4..4, ["-", "-"]);
        }
        if columns != HEADER && fields.len() == 13 {
            fields.remove(11);
        }
        if fields.len() != 12 {
            return Err(problem("expected twelve progress columns"));
        }
        let date = fields[0];
        let valid_date = hour_number(date).is_some();
        if !valid_date || date <= previous.as_str() {
            return Err(problem("dates must be valid and strictly increasing"));
        }
        previous = date.to_string();
        let mut row = Day::new(date);
        for (game, start) in [("tbs", 1), ("tla", 6)] {
            let (done, total, percent) = (fields[start], fields[start + 1], fields[start + 2]);
            let (data, data_total) = (fields[start + 3], fields[start + 4]);
            let data = match (data, data_total) {
                ("-", "-") => None,
                (data, total) => {
                    let data = data.parse::<u64>().map_err(|_| problem("invalid data bytes"))?;
                    let total = total
                        .parse::<u64>()
                        .map_err(|_| problem("invalid data total"))?;
                    if total == 0 || data > total || (done == "-" && percent == "-") {
                        return Err(problem("data needs its total and the code beside it, as bytes or a published percentage"));
                    }
                    Some((data, total))
                }
            };
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
                Some(Measure {
                    data: data.map(|(data, _)| data),
                    data_total: data.map(|(_, total)| total),
                    ..Measure::bytes(done, total)
                })
            } else if percent != "-" {
                let percent = percent
                    .parse::<f64>()
                    .map_err(|_| problem("invalid published percentage"))?;
                if !percent.is_finite() || !(0.0..=100.0).contains(&percent) {
                    return Err(problem("published percentage is outside 0–100"));
                }
                Some(Measure {
                    data: data.map(|(data, _)| data),
                    data_total: data.map(|(_, total)| total),
                    ..Measure::published(percent)
                })
            } else {
                None
            };
            if game == "tbs" {
                row.tbs = measure;
            } else {
                row.tla = measure;
            }
        }
        if fields[11] != "-" {
            row.correction = Some(fields[11].to_string());
        }
        if date.len() == 13 {
            if row.correction.is_some() {
                return Err(problem("hourly rows hold measurements; notes stay daily"));
            }
            hours.push(row);
        } else {
            days.push(row);
        }
    }
    if header.is_none() {
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
        hours,
        figures,
    })
}

fn measured(done: Option<GameDone>) -> Option<Measure> {
    done.filter(|done| done.executable > 0)
        .map(|done| Measure {
            data: (done.data_total > 0).then_some(done.data as u64),
            data_total: (done.data_total > 0).then_some(done.data_total as u64),
            ..Measure::bytes(done.bytes() as u64, done.executable as u64)
        })
}

pub(crate) fn record_hour(
    history: &mut History,
    hour: &str,
    sun: Option<GameDone>,
    anchor: Option<GameDone>,
) {
    let at = history
        .hours
        .partition_point(|row| row.date.as_str() < hour);
    if history.hours.get(at).map(|row| row.date.as_str()) != Some(hour) {
        history.hours.insert(at, Day::new(hour));
    }
    let row = &mut history.hours[at];
    if let Some(value) = measured(sun) {
        row.tbs = Some(value);
    }
    if let Some(value) = measured(anchor) {
        row.tla = Some(value);
    }
    if row.is_empty() {
        history.hours.remove(at);
    }
}

pub(crate) fn points(history: &History, game: &str) -> Vec<(i64, f64)> {
    points_of(history, game, Measure::percent)
}

/// DONE data's line: only the rows that recorded it, from 2026-10-08.
pub(crate) fn data_points(history: &History, game: &str) -> Vec<(i64, f64)> {
    points_of(history, game, Measure::data_percent)
}

fn points_of(
    history: &History,
    game: &str,
    pick: impl Fn(&Measure) -> Option<f64>,
) -> Vec<(i64, f64)> {
    let mut points = history
        .hours
        .iter()
        .filter_map(|row| Some((hour_number(&row.date)?, pick(row.game(game)?)?)))
        .collect::<Vec<_>>();
    for row in &history.days {
        let Some(value) = row.game(game).and_then(&pick) else {
            continue;
        };
        if !history.hours.iter().any(|hour| {
            hour.date.starts_with(&row.date) && hour.game(game).and_then(&pick).is_some()
        })
        {
            if let Some(hour) = hour_number(&row.date) {
                points.push((hour, value));
            }
        }
    }
    points.sort_by_key(|(hour, _)| *hour);
    points
}

pub(crate) fn hour_number(date: &str) -> Option<i64> {
    if !matches!(date.len(), 10 | 13) || !date.is_ascii() {
        return None;
    }
    let day = day_number(&date[..10])?;
    let (year, month, number) = civil(day);
    if format!("{year:04}-{month:02}-{number:02}") != date[..10] {
        return None;
    }
    let hour = if date.len() == 13 {
        if date.as_bytes()[10] != b'T' {
            return None;
        }
        let hour = date[11..].parse::<i64>().ok()?;
        if !(0..24).contains(&hour) || !date[11..].bytes().all(|b| b.is_ascii_digit()) {
            return None;
        }
        hour
    } else {
        0
    };
    Some(day * 24 + hour)
}

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
    let had_data = history.days.iter().any(|row| {
        [&row.tbs, &row.tla]
            .iter()
            .any(|game| game.as_ref().is_some_and(|m| m.data.is_some()))
    });
    let row = history.day_mut(date);
    if let Some(value) = measured(sun) {
        row.tbs = Some(value);
    }
    if let Some(value) = measured(anchor) {
        row.tla = Some(value);
    }
    let has_data = [&row.tbs, &row.tla]
        .iter()
        .any(|game| game.as_ref().is_some_and(|m| m.data.is_some()));
    if has_data && !had_data && row.correction.is_none() {
        row.correction = Some(DATA_BEGINS.into());
    }
    if row.is_empty() {
        history.days.retain(|row| row.date != date);
    }
}
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

pub(crate) fn this_hour() -> String {
    let now = std::time::SystemTime::now()
        .duration_since(std::time::UNIX_EPOCH)
        .map_or(0, |elapsed| elapsed.as_secs() as libc::time_t);
    let mut tm: libc::tm = unsafe { std::mem::zeroed() };
    unsafe { libc::localtime_r(&now, &mut tm) };
    format!(
        "{:04}-{:02}-{:02}T{:02}",
        tm.tm_year + 1900,
        tm.tm_mon + 1,
        tm.tm_mday,
        tm.tm_hour
    )
}
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

#[cfg(test)]
pub(crate) fn previous(date: &str) -> Option<String> {
    let (y, m, d) = civil(day_number(date)? - 1);
    Some(format!("{y:04}-{m:02}-{d:02}"))
}
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

    #[test]
    fn hourly_measurements_replace_only_their_hour_and_round_trip() {
        let mut history = History {
            began: "2026-10-01".into(),
            days: vec![day(
                "2026-10-01",
                Some(Measure::published(50.0)),
                Some(Measure::published(2.0)),
            )],
            ..History::default()
        };
        record_hour(&mut history, "2026-10-01T08", Some(done(510)), None);
        record_hour(
            &mut history,
            "2026-10-01T09",
            Some(done(520)),
            Some(done(30)),
        );
        record_hour(&mut history, "2026-10-01T08", Some(done(515)), None);
        record_hour(&mut history, "2026-10-01T10", None, None);
        assert_eq!(history.hours.len(), 2);
        assert_eq!(
            points(&history, "tbs"),
            vec![
                (hour_number("2026-10-01T08").unwrap(), 51.5),
                (hour_number("2026-10-01T09").unwrap(), 52.0),
            ]
        );
        let loaded = parse(&text(&history)).unwrap();
        assert_eq!(loaded.hours, history.hours);
        assert_eq!(loaded.days, history.days);
        assert_eq!(text(&loaded), text(&history));
        assert_eq!(
            hour_number("2026-10-02T00").unwrap() - hour_number("2026-10-01T23").unwrap(),
            1
        );
        for invalid in [
            "2026-02-30T12",
            "2026-10-01T24",
            "2026-10-01T-1",
            "2026-10-01T+1",
            "2026-10-01T1",
            "2026-10-01X12",
        ] {
            assert!(hour_number(invalid).is_none(), "{invalid}");
        }
    }

    #[test]
    fn games_without_hourly_samples_keep_their_daily_measurements() {
        let mut history = History {
            days: vec![day(
                "2026-10-01",
                Some(Measure::published(50.0)),
                Some(Measure::published(2.0)),
            )],
            ..History::default()
        };
        record_hour(&mut history, "2026-10-01T08", Some(done(510)), None);
        assert_eq!(
            points(&history, "tla"),
            vec![(hour_number("2026-10-01").unwrap(), 2.0)]
        );
    }

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
    fn model_counts_are_read_from_older_rows_and_dropped() {
        let old = format!(
            "# began\t2026-10-05\n{MODELS_HEADER}\n2026-10-05\t1\t10\t-\t-\t-\t-\t-\t-\t-\t-\tOpus 5.5=3;Untagged=1\tstricter\n"
        );
        let history = parse(&old).unwrap();
        assert_eq!(history.days[0].tbs, Some(Measure::bytes(1, 10)));
        assert_eq!(history.days[0].correction.as_deref(), Some("stricter"));
        let text = text(&history);
        assert!(!text.contains("models") && !text.contains("Opus"), "{text}");
        assert!(text.contains("2026-10-05\t1\t10\t-\t-\t-\t-\t-\t-\t-\t-\tstricter\n"), "{text}");
        assert_eq!(parse(&text).unwrap().days, history.days);
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
    fn civil_dates_count_days() {
        assert_eq!(day_number("1970-01-01"), Some(0));
        assert_eq!(
            day_number("2026-09-24").unwrap() - day_number("2026-07-16").unwrap(),
            70
        );
        assert_eq!(previous("2026-03-01").as_deref(), Some("2026-02-28"));
        assert_eq!(previous("2026-09-01").as_deref(), Some("2026-08-31"));
        assert_eq!(this_hour().len(), 13);
    }

    #[test]
    fn code_only_history_reads_and_the_first_data_day_is_marked() {
        let old = format!(
            "# began\t2026-10-05\n{CODE_ONLY_HEADER}\n2026-10-05\t7808068\t8237808\t-\t731298\t11872824\t-\tOpus 5.5=3\t-\n2026-10-05T11\t7808068\t8237808\t-\t-\t-\t-\t-\t-\n"
        );
        let mut history = parse(&old).unwrap();
        assert_eq!(history.days[0].tbs, Some(Measure::bytes(7808068, 8237808)));
        let text_before = text(&history);
        assert!(text_before.starts_with(&format!("# Published progress; '-' means no value was recorded.\n# began\t2026-10-05\n{HEADER}\n")));
        assert!(text_before.contains(
            "2026-10-05\t7808068\t8237808\t-\t-\t-\t731298\t11872824\t-\t-\t-\t-\n"
        ));
        assert_eq!(parse(&text_before).unwrap().days, history.days);
        let sun = GameDone {
            data: 99,
            data_total: 100,
            ..done(1000)
        };
        record(&mut history, "2026-10-08", Some(sun), None);
        record_hour(&mut history, "2026-10-08T09", Some(sun), None);
        record(&mut history, "2026-10-09", Some(sun), None);
        let day = |date: &str| history.days.iter().find(|row| row.date == date).unwrap();
        assert_eq!(day("2026-10-08").correction.as_deref(), Some(DATA_BEGINS));
        assert_eq!(day("2026-10-09").correction, None);
        assert_eq!(
            day("2026-10-08").tbs.as_ref().unwrap().data_percent(),
            Some(99.0)
        );
        assert_eq!(day("2026-10-05").tbs.as_ref().unwrap().data_percent(), None);
        assert_eq!(data_points(&history, "tbs").len(), 2);
        assert_eq!(points(&history, "tbs").len(), 3);
        let loaded = parse(&text(&history)).unwrap();
        assert_eq!(
            (loaded.days, loaded.hours),
            (history.days.clone(), history.hours.clone())
        );
        let invalid = format!("{HEADER}\n2026-10-08\t-\t-\t-\t9\t10\t-\t-\t-\t-\t-\t-\n");
        assert!(parse(&invalid).is_err());
    }
    #[test]
    fn rebuilt_data_sits_beside_a_published_percentage_but_never_alone() {
        let head = format!("# began\t2026-07-16\n{HEADER}\n");
        let beside = format!("{head}2026-07-16\t-\t-\t1.0\t0\t100\t-\t-\t-\t-\t-\t-\n");
        let history = parse(&beside).unwrap();
        assert_eq!(history.days[0].tbs.as_ref().unwrap().data_percent(), Some(0.0));
        assert_eq!(data_points(&history, "tbs").len(), 1);
        let alone = format!("{head}2026-07-16\t-\t-\t-\t0\t100\t-\t-\t-\t-\t-\t-\n");
        assert!(parse(&alone).is_err());
    }

}
