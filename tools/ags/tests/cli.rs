//! The ags binaries end to end on synthetic inputs.
use std::fs;
use std::process::Command;

fn run(tool: &str, args: &[&str]) {
    let status = Command::new(tool).args(args).status().unwrap();
    assert!(status.success(), "{tool} {args:?}");
}

#[test]
fn agsgfx_round_trips_tiles_palettes_and_streams() {
    let agsgfx = env!("CARGO_BIN_EXE_agsgfx");
    let work = tempfile::tempdir().unwrap();
    let path = |name: &str| work.path().join(name).to_string_lossy().into_owned();
    // Four 4bpp tiles, each filled with its own palette index, 32x8 pixels.
    let mut tiles = Vec::new();
    for index in 0..4u8 {
        tiles.extend(std::iter::repeat(index | index << 4).take(32));
    }
    let palette: Vec<u8> = (0..16u16)
        .flat_map(|colour| (colour * 0x421).to_le_bytes())
        .collect();
    fs::write(path("in.4bpp"), &tiles).unwrap();
    fs::write(path("in.gbapal"), &palette).unwrap();
    run(
        agsgfx,
        &[
            &path("in.4bpp"),
            &path("sheet.png"),
            "--palette",
            &path("in.gbapal"),
            "--width",
            "4",
        ],
    );
    run(agsgfx, &[&path("sheet.png"), &path("out.4bpp")]);
    assert_eq!(fs::read(path("out.4bpp")).unwrap(), tiles);
    run(agsgfx, &[&path("sheet.png"), &path("out.gbapal")]);
    assert_eq!(&fs::read(path("out.gbapal")).unwrap()[..32], &palette[..]);
    // Two-tile-wide metatiles keep the row order for a one-row sheet.
    run(
        agsgfx,
        &[
            &path("sheet.png"),
            &path("meta.4bpp"),
            "-mwidth",
            "2",
            "-mheight",
            "1",
        ],
    );
    assert_eq!(fs::read(path("meta.4bpp")).unwrap(), tiles);
    // Each compressor's stream decompresses to its input.
    for kind in ["general", "palette", "tagged", "mtf4"] {
        let stream = path(&format!("{kind}.lz"));
        run(
            agsgfx,
            &[
                &path("out.4bpp"),
                &stream,
                "--lz",
                kind,
                "--machine",
                "4123,485,4126,272",
            ],
        );
        run(agsgfx, &[&stream, &path(&format!("{kind}.bin"))]);
        assert_eq!(
            fs::read(path(&format!("{kind}.bin"))).unwrap(),
            tiles,
            "{kind}"
        );
    }
}

#[test]
fn the_tools_refuse_bad_arguments_with_usage() {
    for tool in [
        env!("CARGO_BIN_EXE_agsgfx"),
        env!("CARGO_BIN_EXE_mid2ags"),
        env!("CARGO_BIN_EXE_wav2ags"),
        env!("CARGO_BIN_EXE_po2ags"),
    ] {
        let output = Command::new(tool).output().unwrap();
        assert!(!output.status.success());
        assert!(
            String::from_utf8_lossy(&output.stderr).contains("usage"),
            "{tool}"
        );
    }
}
