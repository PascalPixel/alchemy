
pub static AGBCC_SOURCES: &[&str] = &[
    "games/common/src/sound/sound.c",
    "games/common/src/sound/engine.c",
    "games/common/src/sound/control.c",
    "games/common/src/sound/modulate.c",
    "games/common/src/sound/tail.c",
    "games/common/src/sound/pcm_key_to_frequency.c",
    "games/common/src/sound/music_track_dispatch_stream_command.c",
    "games/common/src/sound/music_track_read_voice_target.c",
    "games/common/src/sound/music_track_set_voice_decay.c",
    "games/common/src/sound/music_track_set_voice_sustain.c",
    "games/common/src/sound/music_track_set_voice_release.c",
    "games/tbs/src/sound/command_invoke_slot34.c",
    "games/tbs/src/sound/command_invoke_slot35.c",
    "games/tbs/src/sound/music_track_operate_work_byte.c",
    "games/common/src/sound/music_track_set_voice_attack.c",
    "games/common/src/sound/music_track_set_voice_kind.c",
    "games/common/src/sound/music_track_set_voice_length.c",
    "games/common/src/sound/music_track_set_voice_pan_or_sweep.c",
    "games/common/src/sound/tables.c",
    "games/tla/src/sound/music_track_operate_work_byte.c",
];

pub static AGBCC_FLASH_SOURCES: &[&str] = &[
    "games/common/src/system/save/flash_erase_verify.c",
    "games/common/src/system/save/flash_read_routines.c",
    "games/common/src/system/save/erase_atmel_flash_block.c",
    "games/common/src/system/save/erase_atmel_flash_chip.c",
    "games/common/src/system/save/erase_atmel_flash_sector.c",
    "games/common/src/system/save/erase_flash_chip.c",
    "games/common/src/system/save/erase_flash_sector.c",
    "games/common/src/system/save/flash_program_byte.c",
    "games/common/src/system/save/flash_timer_interrupt.c",
    "games/common/src/system/save/flash_verify.c",
    "games/common/src/system/save/identify_flash.c",
    "games/common/src/system/save/program_atmel_flash_block.c",
    "games/common/src/system/save/program_atmel_flash_sector.c",
    "games/common/src/system/save/program_flash_sector.c",
    "games/common/src/system/save/program_flash_sector_with_erase_retry.c",
    "games/common/src/system/save/read_flash.c",
    "games/common/src/system/save/read_flash_core.c",
    "games/common/src/system/save/read_flash_id.c",
    "games/common/src/system/save/start_flash_timer.c",
    "games/common/src/system/save/verify_flash_sector.c",
    "games/common/src/system/save/flash_verify_callback.c",
];

pub static AGBCC_ARM_SOURCES: &[&str] = &[];

/// Both games' ARM C: every game C file compiled as ARM goes through stock
/// agbcc_arm at the library ARM setting, the same compiler as Nintendo's
/// library ARM code, one option set for the whole class (AGENTS.md K1).
pub static GAME_ARM_SOURCES: &[&str] = &[
    "games/common/src/graphics/tile/metatile.c",
];
