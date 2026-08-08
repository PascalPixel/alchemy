//! `selfTest()` -- ported case for case, in the same order, with the same
//! failure messages.

use crate::config::{compiler_configuration_of, family_name, flags_of};
use crate::corpus::{deterministic_sample, Member};
use crate::diff::byte_difference;
use crate::jsvalue::{object, parse, string, strings, Json};
use crate::result::cached_result;
use alchemy_plan::plan::CompilerFamily;

pub fn self_test() -> Result<String, String> {
    let members = vec![
        Member { stem: "08000010".into(), source: "b".into(), address: 0x0800_0010 as f64, size: 4.0 },
        Member { stem: "08000000".into(), source: "a".into(), address: 0x0800_0000 as f64, size: 4.0 },
        Member { stem: "08000020".into(), source: "c".into(), address: 0x0800_0020 as f64, size: 4.0 },
    ];
    let stems = |list: &[Member]| {
        list.iter().map(|m| m.stem.clone()).collect::<Vec<_>>().join(",")
    };
    let first = deterministic_sample(&members, 2, "seed");
    let mut reversed = members.clone();
    reversed.reverse();
    let second = deterministic_sample(&reversed, 2, "seed");
    if stems(&first) != stems(&second) || first.len() != 2 {
        return Err("deterministic sampling self-test failed".to_string());
    }

    let difference = byte_difference(&[1, 2, 3], &[1, 4]);
    if difference.count != 2 || difference.first != Some(1) {
        return Err("byte difference self-test failed".to_string());
    }

    if flags_of(&strings(&[
        "-O2".to_string(),
        "-O2".to_string(),
        "-fno-gcse".to_string(),
    ]))?
    .join(",")
        != "-O2,-fno-gcse"
    {
        return Err("flag normalization self-test failed".to_string());
    }

    let config = compiler_configuration_of(&object(vec![
        ("family", string("old-agbcc")),
        ("addFlags", strings(&["-O2".to_string(), "-O2".to_string()])),
        ("removeFlags", strings(&["-fcall-used-r4".to_string()])),
    ]))?;
    if config.family != Some(CompilerFamily::OldAgbcc)
        || config.add_flags.join(",") != "-O2"
        || config.remove_flags.join(",") != "-fcall-used-r4"
    {
        return Err("structured compiler configuration self-test failed".to_string());
    }

    let sweep = compiler_configuration_of(&object(vec![
        ("ids", strings(&["compiler-old-agbcc".to_string()])),
        ("compiler_family", string("old-agbcc")),
        ("flags", strings(&["-O1".to_string()])),
        ("remove_flags", Json::Array(Vec::new())),
    ]))?;
    if sweep.family != Some(CompilerFamily::OldAgbcc) || sweep.add_flags.join(",") != "-O1" {
        return Err("mode-sweep compiler configuration compatibility failed".to_string());
    }

    let invalid = vec![
        object(vec![("family", string("unknown"))]),
        object(vec![("addFlags", string("-O2"))]),
        object(vec![
            ("addFlags", strings(&["-O2".to_string()])),
            ("removeFlags", strings(&["-O2".to_string()])),
        ]),
        object(vec![
            ("family", string("gcc296")),
            ("compiler_family", string("old-agbcc")),
        ]),
    ];
    for candidate in &invalid {
        if compiler_configuration_of(candidate).is_ok() {
            return Err("invalid compiler configuration was accepted".to_string());
        }
    }

    let example = parse(
        r#"{"stem":"08000000","source":"src/08000000.c","cache_key":"key",
            "cached":false,"compiled":true,"exact":true,"expected_size":4}"#,
    )?;
    let empty = object(vec![("cache_key", string("key"))]);
    if cached_result(&example, "key").is_none()
        || cached_result(&example, "stale").is_some()
        || cached_result(&empty, "key").is_some()
    {
        return Err("cache validation self-test failed".to_string());
    }

    // The family-name table is exercised here so that a new variant added to
    // `CompilerFamily` without a name cannot pass the self-test.
    for family in [
        CompilerFamily::Routed,
        CompilerFamily::Gcc296,
        CompilerFamily::OldAgbcc,
        CompilerFamily::PretEarlyThumb,
        CompilerFamily::Gcc2951,
        CompilerFamily::Gcc3,
    ] {
        if family_name(family).is_empty() {
            return Err("compiler family name table is incomplete".to_string());
        }
    }

    Ok("compiler corpus regression self-test passed".to_string())
}
