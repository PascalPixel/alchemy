// Tests for the battle-effect port. They assemble real fixtures — a synthetic
// ROM span built by the builder itself, hand-written token plans, hand-written
// documents — rather than asserting that a constant equals itself.

use super::*;

fn assets_root() -> PathBuf {
    // The crate lives at tools-rs/battle-effect-data; the tracked PNGs the
    // builder reads live at assets/graphics/.
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .unwrap()
        .parent()
        .unwrap()
        .join("assets")
}

fn tracked_source() -> Option<Value> {
    let path = assets_root().join("data/battle_effect_data_layout.json");
    let text = String::from_utf8_lossy(&std::fs::read(path).ok()?).into_owned();
    Some(json::parse(&text).expect("tracked layout parses"))
}

#[test]
fn the_tracked_document_rebuilds_the_whole_rom_span() {
    let Some(source) = tracked_source() else {
        panic!("the tracked battle-effect layout is missing; this test must not pass vacuously");
    };
    let built = build_battle_effect_data(&source, &assets_root()).expect("tracked layout builds");
    assert_eq!(built.len() as u32, BATTLE_DATA_END - BATTLE_DATA_ADDRESS);
    // A guard against a builder that silently produces a zero span.
    assert!(built.iter().any(|byte| *byte != 0), "built span is all zeroes");
}

#[test]
fn the_self_test_passes() {
    self_test().expect("self-test");
}

#[test]
fn hex_pads_to_eight_digits_and_stays_lowercase() {
    assert_eq!(hex(0), "0x00000000");
    assert_eq!(hex(0x0809_c410), "0x0809c410");
    assert_eq!(hex(u32::MAX), "0xffffffff");
}

#[test]
fn range_clamps_nothing_and_refuses_what_the_typescript_refuses() {
    let rom = vec![7u8; 16];
    assert_eq!(range(&rom, ROM_BASE, ROM_BASE + 4).unwrap(), vec![7, 7, 7, 7]);
    // Below the base: `start - ROM_BASE` goes negative in JS instead of
    // wrapping, so this is an error rather than a huge offset.
    assert_eq!(range(&rom, ROM_BASE - 1, ROM_BASE + 4).unwrap_err(), "ROM range differs");
    // Past the end: JS `subarray` would clamp, but the explicit check fires
    // first in both implementations.
    assert_eq!(range(&rom, ROM_BASE, ROM_BASE + 17).unwrap_err(), "ROM range differs");
    // Reversed.
    assert_eq!(range(&rom, ROM_BASE + 4, ROM_BASE).unwrap_err(), "ROM range differs");
    // An empty range is legal.
    assert!(range(&rom, ROM_BASE + 4, ROM_BASE + 4).unwrap().is_empty());
}

#[test]
fn integer_uses_is_integer_not_is_safe_integer() {
    // 1.0 is an integer in JS: there is only one number type.
    assert_eq!(integer(Some(&Value::Num(1.0)), 0.0, 10.0, "x").unwrap(), 1.0);
    assert!(integer(Some(&Value::Num(1.5)), 0.0, 10.0, "x").is_err());
    assert!(integer(Some(&Value::Num(f64::NAN)), 0.0, 10.0, "x").is_err());
    assert!(integer(Some(&Value::Num(f64::INFINITY)), 0.0, f64::INFINITY, "x").is_err());
    // Non-numbers are rejected, and the label is carried into the message.
    assert_eq!(
        integer(Some(&Value::Str("3".to_string())), 0.0, 10.0, "sparse-table key").unwrap_err(),
        "sparse-table key is out of range"
    );
    assert_eq!(integer(None, 0.0, 10.0, "x").unwrap_err(), "x is out of range");
    // Unbounded integrality: 2^60 passes Number.isInteger and is caught only by
    // the range, which is what `Number.isSafeInteger` would NOT have done.
    assert!(is_integer(2f64.powi(60)));
    assert!(!jsnum::is_safe_integer(2f64.powi(60)));
}

#[test]
fn field_kinds_follow_the_anchored_regex() {
    assert_eq!(field_kind("selector_a:s16").unwrap(), FieldKind::S16);
    assert_eq!(field_kind("script:ptr").unwrap(), FieldKind::Ptr);
    assert_eq!(field_kind("word_0:u32").unwrap(), FieldKind::U32);
    for rejected in [
        "Selector:s16",   // leading capital
        "0abc:s16",       // leading digit
        ":s16",           // empty name
        "value:s64",      // unknown kind
        "value",          // no colon
        "value:s16 ",     // trailing space; the regex is anchored
        "a:b:s16",        // the name may not contain a colon
    ] {
        assert!(field_kind(rejected).is_err(), "{rejected} should be rejected");
    }
    // JS `$` without the `m` flag does not match before a newline in the middle
    // of the string, and `\n` is not in the character class either.
    assert!(field_kind("value:u16\n").is_err());
}

#[test]
fn field_round_trips_reproduce_signedness() {
    let mut output = vec![0u8; 4];
    write_field(&mut output, 0, FieldKind::S16, Some(&Value::Num(-1.0))).unwrap();
    assert_eq!(&output[..2], &[0xff, 0xff]);
    assert_eq!(read_field(&output, 0, FieldKind::S16).unwrap(), Value::Num(-1.0));
    assert_eq!(read_field(&output, 0, FieldKind::U16).unwrap(), Value::Num(65535.0));

    write_field(&mut output, 0, FieldKind::S32, Some(&Value::Num(-2147483648.0))).unwrap();
    assert_eq!(read_field(&output, 0, FieldKind::S32).unwrap(), Value::Num(-2147483648.0));
    assert_eq!(read_field(&output, 0, FieldKind::U32).unwrap(), Value::Num(2147483648.0));

    // A u32 field takes the full unsigned range and rejects a negative.
    assert!(write_field(&mut output, 0, FieldKind::U32, Some(&Value::Num(4294967295.0))).is_ok());
    assert_eq!(
        write_field(&mut output, 0, FieldKind::U32, Some(&Value::Num(-1.0))).unwrap_err(),
        "word table value is out of range"
    );
}

#[test]
fn a_pointer_field_only_accepts_a_known_script_symbol() {
    let mut output = vec![0u8; 4];
    write_field(&mut output, 0, FieldKind::Ptr, Some(&Value::Null)).unwrap();
    assert_eq!(output, vec![0, 0, 0, 0]);
    assert_eq!(read_field(&output, 0, FieldKind::Ptr).unwrap(), Value::Null);

    let name = script_name(0x0809_f13c);
    assert_eq!(name, "ObjectScript_0809f13c");
    write_field(&mut output, 0, FieldKind::Ptr, Some(&Value::Str(name.clone()))).unwrap();
    assert_eq!(read_u32(&output, 0), 0x0809_f13c);
    assert_eq!(read_field(&output, 0, FieldKind::Ptr).unwrap(), Value::Str(name));

    assert_eq!(
        write_field(&mut output, 0, FieldKind::Ptr, Some(&Value::Str("ObjectScript_deadbeef".into())))
            .unwrap_err(),
        "symbolic table pointer differs"
    );
    // A non-zero word with no matching script symbol fails on the way out too.
    write_u32(&mut output, 0, 0x0809_f13d);
    assert_eq!(
        read_field(&output, 0, FieldKind::Ptr).unwrap_err(),
        "exported table pointer lacks an object-script symbol"
    );
}

#[test]
fn opcode_names_parse_the_way_the_regex_does() {
    assert_eq!(opcode_number("op_0").unwrap(), 0);
    assert_eq!(opcode_number("op_39").unwrap(), 39);
    // `Number("007")` is 7, and the regex allows the leading zeroes.
    assert_eq!(opcode_number("op_007").unwrap(), 7);
    for rejected in ["op_", "op_40", "op_-1", "op_1.0", "op_0x10", "OP_1", "op_1 ", " op_1"] {
        assert!(opcode_number(rejected).is_err(), "{rejected} should be rejected");
    }
}

#[test]
fn number_coercion_in_palette_tokens_keeps_the_javascript_grammar() {
    // The trap this pins: JS `Number("0x00000098")` is 152 where Rust's
    // `f64::from_str` rejects the string outright.
    assert_eq!(to_number("0x00000098"), 152.0);
    assert!("0x00000098".parse::<f64>().is_err());
    assert_eq!(palette_number(Some(&Value::Str("0x10".to_string()))), 16.0);
    assert_eq!(palette_number(Some(&Value::Str("  12  ".to_string()))), 12.0);
    assert_eq!(palette_number(Some(&Value::Str("".to_string()))), 0.0);
    assert!(palette_number(Some(&Value::Str("12px".to_string()))).is_nan());
}

#[test]
fn source_word_reads_the_high_half_as_signed() {
    assert_eq!(source_word(0), 0.0);
    assert_eq!(source_word(0x7fff_ffff), 2147483647.0);
    assert_eq!(source_word(0x8000_0000), -2147483648.0);
    assert_eq!(source_word(0xffff_ffff), -1.0);
    // And `scriptWord` puts it back, `>>> 0`, without saturating the way a
    // Rust `as i32` cast on an out-of-range float would.
    assert_eq!(script_word(Some(&Value::Num(-1.0))).unwrap(), 0xffff_ffff);
    assert_eq!(script_word(Some(&Value::Num(4294967295.0))).unwrap(), 0xffff_ffff);
    assert!(script_word(Some(&Value::Num(4294967296.0))).is_err());
    assert_eq!(
        script_word(Some(&Value::Str("Func_08098c08".into()))).unwrap(),
        0x0809_8c09
    );
    assert_eq!(
        script_word(Some(&Value::Str("Func_deadbeef".into()))).unwrap_err(),
        "unknown script symbol: Func_deadbeef"
    );
}

#[test]
fn the_halfword_codec_round_trips_a_hand_written_plan() {
    let pixels: Vec<u8> = (0..64u8).collect();
    let mut plan = vec![HalfwordToken::Literal(8)];
    plan.push(HalfwordToken::Copy { length: 8, distance: 8 });
    plan.push(HalfwordToken::Literal(16));
    plan.push(HalfwordToken::End);
    // The plan above describes 8 + 8 + 16 = 32 halfwords = 64 bytes, but the
    // copy must reproduce the real pixels, so build the input from the plan.
    let mut decoded: Vec<u8> = pixels[..16].to_vec();
    decoded.extend_from_slice(&pixels[..16]);
    decoded.extend_from_slice(&pixels[32..64]);
    let encoded = encode_halfword(&decoded, &plan).unwrap();
    let (again, tokens) = decode_halfword_trace(&encoded).unwrap();
    assert_eq!(again, decoded);
    assert_eq!(tokens, plan);
    // Groups are 2 flag bytes + up to 16 token halfwords; 26 operations is two
    // groups, so the stream is not a single group by accident.
    assert!(encoded.len() > 34);
}

#[test]
fn a_halfword_plan_that_does_not_reconstruct_the_pixels_is_refused() {
    let decoded = vec![1u8, 0, 2, 0];
    assert_eq!(
        encode_halfword(&decoded, &[HalfwordToken::Literal(1), HalfwordToken::End]).unwrap_err(),
        "halfword plan does not reconstruct decoded pixels"
    );
    assert_eq!(
        encode_halfword(&decoded, &[HalfwordToken::Literal(2)]).unwrap_err(),
        "halfword plan does not reconstruct decoded pixels"
    );
    assert_eq!(
        encode_halfword(
            &decoded,
            &[HalfwordToken::Literal(2), HalfwordToken::End, HalfwordToken::Literal(1)]
        )
        .unwrap_err(),
        "halfword terminator is not final"
    );
    assert_eq!(
        encode_halfword(&[0u8; 3], &[HalfwordToken::End]).unwrap_err(),
        "halfword pixels have an odd size"
    );
    // A copy that reaches behind the replay prefix.
    assert_eq!(
        encode_halfword(
            &decoded,
            &[HalfwordToken::Copy { length: 2, distance: 1 }, HalfwordToken::End]
        )
        .unwrap_err(),
        "halfword copy crossed replay prefix"
    );
}

#[test]
fn a_truncated_halfword_stream_is_refused() {
    assert_eq!(decode_halfword_trace(&[0u8]).unwrap_err(), "halfword flags are truncated");
    assert_eq!(decode_halfword_trace(&[0u8, 0]).unwrap_err(), "halfword token is truncated");
    // A terminator mid-group leaves the rest of the group unread, and the
    // trailing-byte check catches it.
    let mut stream = vec![0x00, 0x80, 0x00, 0x00];
    stream.push(0x99);
    assert_eq!(
        decode_halfword_trace(&stream).unwrap_err(),
        "halfword stream has trailing bytes"
    );
}

#[test]
fn halfword_tokens_survive_the_json_shape() {
    for token in [
        HalfwordToken::Literal(3),
        HalfwordToken::Copy { length: 33, distance: 2047 },
        HalfwordToken::End,
    ] {
        let value = halfword_token_value(&token);
        assert_eq!(halfword_token_from(&value).unwrap(), token);
    }
    assert_eq!(
        minified(&halfword_token_value(&HalfwordToken::Copy { length: 4, distance: 9 })),
        "[\"c\",4,9]"
    );
    for rejected in ["[]", "[\"l\"]", "[\"l\",0]", "[\"c\",1,1]", "[\"c\",2,2048]", "[\"e\",1]", "[\"x\"]"] {
        let value = json::parse(rejected).unwrap();
        assert!(halfword_token_from(&value).is_err(), "{rejected} should be rejected");
    }
}

#[test]
fn assemble_tail_reports_holes_and_inconsistent_overlaps() {
    let full = vec![(RULE_TABLES_END, BATTLE_DATA_END, vec![0u8; (BATTLE_DATA_END - RULE_TABLES_END) as usize])];
    assert!(assemble_tail(&full).is_ok());

    let mut with_hole = full.clone();
    with_hole[0] = (RULE_TABLES_END, BATTLE_DATA_END - 1, vec![0u8; (BATTLE_DATA_END - RULE_TABLES_END - 1) as usize]);
    assert_eq!(
        assemble_tail(&with_hole).unwrap_err(),
        format!("battle-effect tail has a hole at {}", hex(BATTLE_DATA_END - 1))
    );

    let mut overlapping = full.clone();
    overlapping.push((RULE_TABLES_END, RULE_TABLES_END + 1, vec![9u8]));
    assert_eq!(
        assemble_tail(&overlapping).unwrap_err(),
        format!("battle-effect overlap differs at {}", hex(RULE_TABLES_END))
    );

    // A consistent overlap is allowed: the byte already written matches.
    let mut agreeing = full.clone();
    agreeing.push((RULE_TABLES_END, RULE_TABLES_END + 1, vec![0u8]));
    assert!(assemble_tail(&agreeing).is_ok());

    // Segments outside the span, or with a length that disagrees with the
    // declared extent, are refused before anything is painted.
    assert_eq!(
        assemble_tail(&[(RULE_TABLES_END, RULE_TABLES_END + 2, vec![0u8])]).unwrap_err(),
        "battle-effect tail segment differs"
    );
    assert_eq!(
        assemble_tail(&[(BATTLE_DATA_ADDRESS, BATTLE_DATA_ADDRESS + 1, vec![0u8])]).unwrap_err(),
        "battle-effect tail segment differs"
    );
}

#[test]
fn graphic_paths_flatten_and_refuse_an_escape() {
    let root = Path::new("assets");
    assert_eq!(
        graphic_path(root, "graphics/battle/effects/particle_beam.4bpp.png").unwrap(),
        root.join("graphics/battle_effects_particle_beam.4bpp.png")
    );
    for rejected in [
        "../private.png",
        "graphics/battle/effects/../../secret.png",
        "graphics/battle/effects/Upper.png",
        "graphics/battle/effects/.png",
        "graphics/battle/effects/a.PNG",
        "graphics/battle/effects/a.png\n",
        "graphics/battle/effects/sub/dir.png",
        "graphics/battle/effects/",
    ] {
        assert!(graphic_path(root, rejected).is_err(), "{rejected} should be rejected");
    }
    // ".." alone is spelled out of the character class only by the `/`, so the
    // one-dot name that IS accepted still cannot leave the directory.
    assert!(graphic_path(root, "graphics/battle/effects/..png").is_ok());
}

#[test]
fn the_sine_table_matches_the_documented_formula() {
    let table = build_sine_table(&Value::obj(vec![(
        "sine_table",
        Value::obj(vec![
            ("address", Value::Str(hex(SINE_TABLE_ADDRESS))),
            ("end", Value::Str(hex(SINE_TABLE_END))),
            ("count", Value::Num(256.0)),
            ("type", Value::Str("s16".to_string())),
            ("formula", Value::Str("trunc(sin(index*pi/128)*0x1000)".to_string())),
        ]),
    )]))
    .unwrap();
    assert_eq!(table.len(), 512);
    assert_eq!(read_u16(&table, 0) as u16 as i16, 0);
    assert_eq!(read_u16(&table, 64 * 2) as u16 as i16, 4096);
    assert_eq!(read_u16(&table, 192 * 2) as u16 as i16, -4096);
    // Truncation toward zero, not rounding: index 1 is 100.53..., not 101.
    assert_eq!(read_u16(&table, 2) as u16 as i16, 100);
}

#[test]
fn exact_keys_names_the_offending_label() {
    let value = Value::obj(vec![("a", Value::Num(1.0)), ("b", Value::Num(2.0))]);
    assert!(exact_keys(&value, &["b", "a"], "thing").is_ok());
    assert_eq!(exact_keys(&value, &["a"], "thing").unwrap_err(), "thing has unknown fields");
    assert_eq!(
        exact_keys(&value, &["a", "b", "c"], "thing").unwrap_err(),
        "thing has unknown fields"
    );
}

#[test]
fn parse_rejects_a_document_with_the_wrong_shape() {
    assert_eq!(parse(&Value::Num(1.0)).unwrap_err(), "battle-effect source must be an object");
    assert_eq!(
        parse(&Value::obj(vec![("format", Value::Num(2.0))])).unwrap_err(),
        "battle-effect source has unknown fields"
    );
    let Some(source) = tracked_source() else {
        panic!("the tracked battle-effect layout is missing; this test must not pass vacuously");
    };
    parse(&source).expect("the tracked layout parses");
    // Bumping the format is a layout change, not an unknown field.
    let mut wrong = match source.clone() {
        Value::Obj(pairs) => pairs,
        _ => unreachable!(),
    };
    for pair in wrong.iter_mut() {
        if pair.0 == "format" {
            pair.1 = Value::Num(3.0);
        }
    }
    assert_eq!(parse(&Value::Obj(wrong)).unwrap_err(), "battle-effect source layout differs");
}

#[test]
fn the_cli_reports_its_usage_the_way_the_typescript_does() {
    assert_eq!(
        run(vec![]).unwrap_err(),
        "usage: battle_effect_data.ts {export ROM --root ASSETS --output SOURCE|verify ROM SOURCE --root ASSETS}"
    );
    assert_eq!(
        run(vec!["export".into(), "rom.gba".into()]).unwrap_err(),
        "usage: battle_effect_data.ts export ROM --root ASSETS --output SOURCE"
    );
    assert_eq!(
        run(vec!["verify".into(), "rom.gba".into(), "source.json".into()]).unwrap_err(),
        "usage: battle_effect_data.ts verify ROM SOURCE --root ASSETS"
    );
    // `--self-test` wins wherever it appears, exactly as `args.includes` does.
    run(vec!["verify".into(), "--self-test".into()]).expect("self-test wins");
    // An empty string is falsy in JS, so it fails the usage check rather than
    // being treated as a path.
    assert_eq!(
        run(vec!["export".into(), "".into(), "--root".into(), "a".into(), "--output".into(), "b".into()])
            .unwrap_err(),
        "usage: battle_effect_data.ts export ROM --root ASSETS --output SOURCE"
    );
    // `option` returns the following argument verbatim, even another flag.
    let args: Vec<String> = vec!["export".into(), "--root".into(), "--output".into()];
    assert_eq!(option(&args, "--root").as_deref(), Some("--output"));
    assert_eq!(option(&args, "--missing"), None);
    assert_eq!(option(&["--root".to_string()], "--root"), None);
}

#[test]
fn a_sparse_table_that_terminates_differently_is_a_failure_not_a_wider_type() {
    let mut raw = vec![0u8; (SPARSE_TABLE_END - SPARSE_TABLE_ADDRESS) as usize];
    write_u32(&mut raw, 41 * 8, 0xffff_ffff);
    assert!(sparse_sentinel(&raw).is_ok());
    write_u32(&mut raw, 41 * 8 + 4, 1);
    assert_eq!(
        sparse_sentinel(&raw).unwrap_err(),
        "sparse table sentinel is (-1, 1), expected (-1, 0)"
    );
}

#[test]
fn a_duplicate_sparse_index_is_refused() {
    let table = Value::obj(vec![(
        "sparse_table",
        Value::obj(vec![
            ("address", Value::Str(hex(SPARSE_TABLE_ADDRESS))),
            ("end", Value::Str(hex(SPARSE_TABLE_END))),
            ("count", Value::Num(41.0)),
            ("fields", string_list(&["key:s32", "value:s32"])),
            ("consumers", string_list(&["Func_0809bcf8"])),
            (
                "records",
                Value::Arr(vec![
                    Value::obj(vec![
                        ("index", Value::Num(3.0)),
                        ("key", Value::Num(1.0)),
                        ("value", Value::Num(2.0)),
                    ]),
                    Value::obj(vec![
                        ("index", Value::Num(3.0)),
                        ("key", Value::Num(4.0)),
                        ("value", Value::Num(5.0)),
                    ]),
                ]),
            ),
            ("sentinel", Value::Arr(vec![Value::Num(-1.0), Value::Num(0.0)])),
        ]),
    )]);
    assert_eq!(
        build_sparse_table(&table).unwrap_err(),
        "sparse-table index is duplicated"
    );
}

#[test]
fn the_emitted_document_keeps_its_declared_field_order() {
    // Field order comes from the JS object literal and is load-bearing: the
    // tracked file is compared byte-for-byte.
    let entry = Value::obj(vec![
        ("name", Value::Str("ObjectScript_0809e6c0".to_string())),
        ("address", Value::Str("0x0809e6c0".to_string())),
    ]);
    assert_eq!(
        minified(&entry),
        "{\"name\":\"ObjectScript_0809e6c0\",\"address\":\"0x0809e6c0\"}"
    );
    // A re-stated key in a parsed document overwrites in place rather than
    // moving to the end, which is what `verify` reads back.
    let restated = json::parse("{\"a\":1,\"b\":2,\"a\":3}").unwrap();
    assert_eq!(minified(&restated), "{\"a\":3,\"b\":2}");
    assert_eq!(restated.own_keys(), vec!["a".to_string(), "b".to_string()]);
}
