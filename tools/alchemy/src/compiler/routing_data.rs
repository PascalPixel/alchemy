// Whole-file compiler-family decisions. Every file uses its family's one
// flag set from routing.rs; these declarations contain no function addresses.

// The MusicPlayer2000 library's modules, shared and edition-specific, and the
// two agbcc -O2 flash helpers. Their neighbouring game-owned SOUND and SAVE
// files keep the Game family.
pub static AGBCC_SOURCES: &[&str] = &[
    "games/COMMON/SRC/SOUND/SOUND.C",
    "games/COMMON/SRC/SOUND/SOUND2.C",
    "games/COMMON/SRC/SOUND/SOUND3.C",
    "games/COMMON/SRC/SOUND/SOUND4.C",
    "games/COMMON/SRC/SOUND/SOUND5.C",
    "games/COMMON/SRC/SOUND/PCM_KEY_TO_FREQUENCY.C",
    "games/COMMON/SRC/SOUND/MUSIC_TRACK_DISPATCH_STREAM_COMMAND.C",
    "games/COMMON/SRC/SOUND/MUSIC_TRACK_READ_VOICE_TARGET.C",
    "games/COMMON/SRC/SOUND/MUSIC_TRACK_SET_VOICE_DECAY.C",
    "games/COMMON/SRC/SOUND/MUSIC_TRACK_SET_VOICE_SUSTAIN.C",
    "games/COMMON/SRC/SOUND/MUSIC_TRACK_SET_VOICE_RELEASE.C",
    "games/THE BROKEN SEAL/SRC/SOUND/COMMAND_INVOKE_SLOT34.C",
    "games/THE BROKEN SEAL/SRC/SOUND/COMMAND_INVOKE_SLOT35.C",
    "games/THE BROKEN SEAL/SRC/SOUND/MUSIC_TRACK_OPERATE_WORK_BYTE.C",
    "games/COMMON/SRC/SOUND/MUSIC_TRACK_SET_VOICE_ATTACK.C",
    "games/COMMON/SRC/SOUND/MUSIC_TRACK_SET_VOICE_KIND.C",
    "games/COMMON/SRC/SOUND/MUSIC_TRACK_SET_VOICE_LENGTH.C",
    "games/COMMON/SRC/SOUND/MUSIC_TRACK_SET_VOICE_PAN_OR_SWEEP.C",
    "games/COMMON/SRC/SYSTEM/SAVE/FLASH_ERASE_VERIFY.C",
    "games/COMMON/SRC/SYSTEM/SAVE/FLASH_READ_ROUTINES.C",
    "games/THE LOST AGE/SRC/SOUND/MUSIC_TRACK_OPERATE_WORK_BYTE.C",
];

// The flash library uses agbcc -O: its allocation and scheduling match that
// family, while -O2 differs throughout. Game save-state handling is separate.
pub static AGBCC_FLASH_SOURCES: &[&str] = &[
    "games/COMMON/SRC/SYSTEM/SAVE/ERASE_ATMEL_FLASH_BLOCK.C",
    "games/COMMON/SRC/SYSTEM/SAVE/ERASE_ATMEL_FLASH_CHIP.C",
    "games/COMMON/SRC/SYSTEM/SAVE/ERASE_ATMEL_FLASH_SECTOR.C",
    "games/COMMON/SRC/SYSTEM/SAVE/ERASE_FLASH_CHIP.C",
    "games/COMMON/SRC/SYSTEM/SAVE/ERASE_FLASH_SECTOR.C",
    "games/COMMON/SRC/SYSTEM/SAVE/FLASH_PROGRAM_BYTE.C",
    "games/COMMON/SRC/SYSTEM/SAVE/FLASH_TIMER_INTERRUPT.C",
    "games/COMMON/SRC/SYSTEM/SAVE/FLASH_VERIFY.C",
    "games/COMMON/SRC/SYSTEM/SAVE/IDENTIFY_FLASH.C",
    "games/COMMON/SRC/SYSTEM/SAVE/PROGRAM_ATMEL_FLASH_BLOCK.C",
    "games/COMMON/SRC/SYSTEM/SAVE/PROGRAM_ATMEL_FLASH_SECTOR.C",
    "games/COMMON/SRC/SYSTEM/SAVE/PROGRAM_FLASH_SECTOR.C",
    "games/COMMON/SRC/SYSTEM/SAVE/PROGRAM_FLASH_SECTOR_WITH_ERASE_RETRY.C",
    "games/COMMON/SRC/SYSTEM/SAVE/READ_FLASH.C",
    "games/COMMON/SRC/SYSTEM/SAVE/READ_FLASH_CORE.C",
    "games/COMMON/SRC/SYSTEM/SAVE/READ_FLASH_ID.C",
    "games/COMMON/SRC/SYSTEM/SAVE/START_FLASH_TIMER.C",
    "games/COMMON/SRC/SYSTEM/SAVE/VERIFY_FLASH_SECTOR.C",
    "games/COMMON/SRC/SYSTEM/SAVE/FLASH_VERIFY_CALLBACK.C",
];

// Resident ARM routines, built with pret's agbcc_arm (Pascal, 2026-09-29):
// only that compiler divides with the ROM's cmp/addlt, as agscc cannot.
pub static AGBCC_ARM_SOURCES: &[&str] = &[];
