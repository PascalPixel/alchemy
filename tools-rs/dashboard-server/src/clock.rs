// `new Date().toISOString()`.
//
// WHY hand-rolled: the standard library has no calendar. The format is fixed
// at `YYYY-MM-DDTHH:MM:SS.mmmZ` -- exactly three fractional digits, always
// UTC, always a `Z` -- and the civil-from-days conversion below is the
// standard proleptic Gregorian algorithm, so this is a closed problem rather
// than a reason to take a dependency.

use std::time::{SystemTime, UNIX_EPOCH};

pub fn iso_now() -> String {
    let millis = SystemTime::now()
        .duration_since(UNIX_EPOCH)
        .map_or(0i64, |since| since.as_millis() as i64);
    iso_from_millis(millis)
}

pub fn iso_from_millis(millis: i64) -> String {
    // `div_euclid`, not `/`: JS `Date` handles pre-epoch instants by flooring,
    // and Rust's `/` truncates toward zero, which is a different day for a
    // negative input.
    let days = millis.div_euclid(86_400_000);
    let rest = millis.rem_euclid(86_400_000);
    let (year, month, day) = civil_from_days(days);
    format!(
        "{year:04}-{month:02}-{day:02}T{:02}:{:02}:{:02}.{:03}Z",
        rest / 3_600_000,
        (rest / 60_000) % 60,
        (rest / 1_000) % 60,
        rest % 1_000
    )
}

/// Howard Hinnant's `civil_from_days`, shifted to a 0000-03-01 era.
fn civil_from_days(days: i64) -> (i64, i64, i64) {
    let shifted = days + 719_468;
    let era = shifted.div_euclid(146_097);
    let day_of_era = shifted.rem_euclid(146_097);
    let year_of_era =
        (day_of_era - day_of_era / 1_460 + day_of_era / 36_524 - day_of_era / 146_096) / 365;
    let year = year_of_era + era * 400;
    let day_of_year = day_of_era - (365 * year_of_era + year_of_era / 4 - year_of_era / 100);
    let month_prime = (5 * day_of_year + 2) / 153;
    let day = day_of_year - (153 * month_prime + 2) / 5 + 1;
    let month = if month_prime < 10 { month_prime + 3 } else { month_prime - 9 };
    (if month <= 2 { year + 1 } else { year }, month, day)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn known_instants_match_javascript_to_iso_string() {
        assert_eq!(iso_from_millis(0), "1970-01-01T00:00:00.000Z");
        assert_eq!(iso_from_millis(1_785_756_380_214), "2026-08-03T11:26:20.214Z");
        // A leap day, and the last millisecond of a year.
        assert_eq!(iso_from_millis(1_709_164_800_000), "2024-02-29T00:00:00.000Z");
        assert_eq!(iso_from_millis(1_735_689_599_999), "2024-12-31T23:59:59.999Z");
    }

    #[test]
    fn pre_epoch_instants_floor_rather_than_truncate() {
        assert_eq!(iso_from_millis(-1), "1969-12-31T23:59:59.999Z");
    }

    #[test]
    fn the_shape_is_always_twenty_four_characters() {
        let now = iso_now();
        assert_eq!(now.len(), 24, "{now}");
        assert!(now.ends_with('Z'));
        assert_eq!(&now[4..5], "-");
        assert_eq!(&now[10..11], "T");
    }
}
