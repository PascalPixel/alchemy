
pub static AGBCC_SOURCES: &[&str] = &[
    "games/common/src/sound/sound.c",
    "games/tbs/src/sound/engine.c",
    "games/common/src/sound/engine.c",
    "games/common/src/sound/music_track_dispatch_stream_command.c",
    "games/tbs/src/sound/tail.c",
    "games/tla/src/sound/tail.c",
    "games/tla/src/sound/music_track_read_voice_target.c",
    "games/tla/src/sound/music_track_set_voice_decay.c",
    "games/tla/src/sound/music_track_set_voice_sustain.c",
    "games/tla/src/sound/music_track_set_voice_release.c",
    "games/tla/src/sound/music_track_set_voice_attack.c",
    "games/tla/src/sound/music_track_set_voice_kind.c",
    "games/tla/src/sound/music_track_set_voice_length.c",
    "games/tla/src/sound/music_track_set_voice_pan_or_sweep.c",
    "games/common/src/sound/tables.c",
    "games/tla/src/sound/music_track_operate_work_byte.c",
];

pub static AGBCC_FLASH_SOURCES: &[&str] = &[
    "games/common/src/system/save/flash.c",
    "games/common/src/system/save/sst.c",
    "games/common/src/system/save/atmel.c",
    "games/common/src/system/save/program_flash_sector.c",
];

/// Both games' ARM C uses stock GCC 2.95.2 with one option set for the class.
pub static GAME_ARM_SOURCES: &[&str] = &[
    "games/tbs/src/graphics/text/moji.c",
    "games/common/src/graphics/tile/metatile.c",
    "games/tbs/src/field/common/object/update.c",
    "games/tbs/src/graphics/tile/vertices.c",
];
