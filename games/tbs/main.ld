/* The Broken Seal, all six editions. Object order is objects.tsv: the build
   writes each place's objects for the edition it links into <place>.ld.
   Camelot's far-call banks start at addresses each edition chose; the
   build names the edition with --defsym TBS_EDITION_<LANG>=1. An address is
   written once for the editions that share it, and the last address of each
   placement is that of the editions it does not name: English and Italian,
   and Japanese with them at the first two. Most of German's bank starts fall
   on 256-byte steps, the other editions' on 2 or 4 KB ones. */
OUTPUT_ARCH(arm)
ENTRY(System_Entry)
INCLUDE "games/tbs/constants.ld"

ASSERT(DEFINED(TBS_EDITION_JA) + DEFINED(TBS_EDITION_EN) + DEFINED(TBS_EDITION_DE)
     + DEFINED(TBS_EDITION_ES) + DEFINED(TBS_EDITION_FR) + DEFINED(TBS_EDITION_IT) == 1,
       "link one edition: --defsym TBS_EDITION_<LANG>=1")

MEMORY
{
    EWRAM (rwx) : ORIGIN = 0x2000000, LENGTH = 256K
    IWRAM (rwx) : ORIGIN = 0x3000000, LENGTH = 32K
    ROM    (rx) : ORIGIN = 0x8000000, LENGTH = 32M
}

SECTIONS
{
    ewram (NOLOAD) :
    {
        INCLUDE "ewram.ld"
    } > EWRAM
    .rom_start :
    {
        INCLUDE "rom_start.ld"
    } > ROM
    .iwram_resident :
    {
        INCLUDE "iwram_resident.ld"
    } > IWRAM AT>ROM
    iwram (NOLOAD) :
    {
        INCLUDE "iwram.ld"
    } > IWRAM
    .rom_main :
    {
        INCLUDE "rom_main_00.ld"
        . = ABSOLUTE(DEFINED(TBS_EDITION_DE) || DEFINED(TBS_EDITION_ES) || DEFINED(TBS_EDITION_FR) ? 0x08008800 :
                                                                                                     0x08009000);
        INCLUDE "rom_main_01.ld"
        . = ABSOLUTE(DEFINED(TBS_EDITION_DE)                            ? 0x08013d00 :
                     DEFINED(TBS_EDITION_ES) || DEFINED(TBS_EDITION_FR) ? 0x08014000 :
                                                                          0x08015000);
        INCLUDE "rom_main_02.ld"
        . = ABSOLUTE(DEFINED(TBS_EDITION_JA)                            ? 0x0806d000 :
                     DEFINED(TBS_EDITION_DE)                            ? 0x0807ae00 :
                     DEFINED(TBS_EDITION_ES) || DEFINED(TBS_EDITION_FR) ? 0x0807c000 :
                                                                          0x08077000);
        INCLUDE "rom_main_03.ld"
        . = ABSOLUTE(DEFINED(TBS_EDITION_JA)                            ? 0x08081000 :
                     DEFINED(TBS_EDITION_DE)                            ? 0x0808d600 :
                     DEFINED(TBS_EDITION_ES) || DEFINED(TBS_EDITION_FR) ? 0x0808f000 :
                                                                          0x0808a000);
        INCLUDE "rom_main_04.ld"
        . = ABSOLUTE(DEFINED(TBS_EDITION_JA)                            ? 0x08098000 :
                     DEFINED(TBS_EDITION_DE)                            ? 0x080a3e00 :
                     DEFINED(TBS_EDITION_ES) || DEFINED(TBS_EDITION_FR) ? 0x080a5800 :
                                                                          0x080a1000);
        INCLUDE "rom_main_05.ld"
        . = ABSOLUTE(DEFINED(TBS_EDITION_JA)                            ? 0x080a7000 :
                     DEFINED(TBS_EDITION_DE)                            ? 0x080b2600 :
                     DEFINED(TBS_EDITION_ES) || DEFINED(TBS_EDITION_FR) ? 0x080b4000 :
                                                                          0x080b0000);
        INCLUDE "rom_main_06.ld"
        . = ABSOLUTE(DEFINED(TBS_EDITION_JA)                            ? 0x080ac000 :
                     DEFINED(TBS_EDITION_DE)                            ? 0x080b7600 :
                     DEFINED(TBS_EDITION_ES) || DEFINED(TBS_EDITION_FR) ? 0x080b9000 :
                                                                          0x080b5000);
        INCLUDE "rom_main_07.ld"
        . = ABSOLUTE(DEFINED(TBS_EDITION_JA)                            ? 0x080c0000 :
                     DEFINED(TBS_EDITION_DE)                            ? 0x080cae00 :
                     DEFINED(TBS_EDITION_ES) || DEFINED(TBS_EDITION_FR) ? 0x080cc800 :
                                                                          0x080c9000);
        INCLUDE "rom_main_08.ld"
        . = ABSOLUTE(DEFINED(TBS_EDITION_JA)                            ? 0x080e7000 :
                     DEFINED(TBS_EDITION_DE)                            ? 0x080f1600 :
                     DEFINED(TBS_EDITION_ES) || DEFINED(TBS_EDITION_FR) ? 0x080f3000 :
                                                                          0x080f0000);
        INCLUDE "rom_main_09.ld"
        . = ABSOLUTE(DEFINED(TBS_EDITION_JA)                            ? 0x080e9000 :
                     DEFINED(TBS_EDITION_DE)                            ? 0x080f3600 :
                     DEFINED(TBS_EDITION_ES) || DEFINED(TBS_EDITION_FR) ? 0x080f5000 :
                                                                          0x080f2000);
        INCLUDE "rom_main_10.ld"
        . = ABSOLUTE(DEFINED(TBS_EDITION_JA)                            ? 0x080eb000 :
                     DEFINED(TBS_EDITION_DE)                            ? 0x080f5600 :
                     DEFINED(TBS_EDITION_ES) || DEFINED(TBS_EDITION_FR) ? 0x080f7000 :
                                                                          0x080f4000);
        INCLUDE "rom_main_11.ld"
        . = ABSOLUTE(DEFINED(TBS_EDITION_JA)                            ? 0x080ed000 :
                     DEFINED(TBS_EDITION_DE)                            ? 0x080f7600 :
                     DEFINED(TBS_EDITION_ES) || DEFINED(TBS_EDITION_FR) ? 0x080f9000 :
                                                                          0x080f6000);
        INCLUDE "rom_main_12.ld"
        . = ABSOLUTE(DEFINED(TBS_EDITION_JA)                            ? 0x080f0000 :
                     DEFINED(TBS_EDITION_DE)                            ? 0x080f9e00 :
                     DEFINED(TBS_EDITION_ES) || DEFINED(TBS_EDITION_FR) ? 0x080fb800 :
                                                                          0x080f9000);
        INCLUDE "rom_main_13.ld"
    } > ROM
    .iwram_mixer 0x3007000 :
    {
        INCLUDE "iwram_mixer.ld"
    } > IWRAM AT>ROM
    iwram_high (NOLOAD) :
    {
        INCLUDE "iwram_high.ld"
    } > IWRAM
    .rom_tail :
    {
        INCLUDE "rom_tail_00.ld"
        . = ABSOLUTE(DEFINED(TBS_EDITION_JA)                            ? 0x0817c000 :
                     DEFINED(TBS_EDITION_DE)                            ? 0x08185e00 :
                     DEFINED(TBS_EDITION_ES) || DEFINED(TBS_EDITION_FR) ? 0x08187800 :
                                                                          0x08185000);
        INCLUDE "rom_tail_01.ld"
        . = ABSOLUTE(DEFINED(TBS_EDITION_JA)                            ? 0x08317000 :
                     DEFINED(TBS_EDITION_DE)                            ? 0x0831fe00 :
                     DEFINED(TBS_EDITION_ES) || DEFINED(TBS_EDITION_FR) ? 0x08321800 :
                                                                          0x08320000);
        INCLUDE "rom_tail_02.ld"
    } > ROM
    IwramResident_Rom = LOADADDR(.iwram_resident);
    SoundMainRAM_Rom = LOADADDR(.iwram_mixer) + 1;
    Mixer_CallViaR3Rom = LOADADDR(.iwram_mixer) + (Mixer_CallViaR3 - ADDR(.iwram_mixer));
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
