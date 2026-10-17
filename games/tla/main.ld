/* The Lost Age, all six editions. Object order is objects.tsv: the build
   writes each place's objects for the edition it links into <place>.ld.
   Camelot's far-call banks start at addresses each edition chose; the
   build names the edition with --defsym TLA_EDITION_<LANG>=1. An address
   is written once for the editions that share it, and the last address of
   each placement is that of the editions it does not name. */
OUTPUT_ARCH(arm)
ENTRY(Entry)
INCLUDE "games/tla/constants.ld"

ASSERT(DEFINED(TLA_EDITION_JA) + DEFINED(TLA_EDITION_EN) + DEFINED(TLA_EDITION_DE)
     + DEFINED(TLA_EDITION_ES) + DEFINED(TLA_EDITION_FR) + DEFINED(TLA_EDITION_IT) == 1,
       "link one edition: --defsym TLA_EDITION_<LANG>=1")

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
        INCLUDE "rom_start_00.ld"
    } > ROM
    iwram_slots 0x3000000 (NOLOAD) :
    {
        INCLUDE "iwram_slots.ld"
    } > IWRAM
    .iwram_runtime 0x3000100 :
    {
        INCLUDE "iwram_runtime_00.ld"
    } > IWRAM AT>ROM
    iwram (NOLOAD) :
    {
        INCLUDE "iwram.ld"
    } > IWRAM
    .rom_main :
    {
        INCLUDE "rom_main_00.ld"
        . = ABSOLUTE(0x08020000);
        INCLUDE "rom_main_01.ld"
        . = ABSOLUTE(0x08038000);
        INCLUDE "rom_main_02.ld"
        . = ABSOLUTE(DEFINED(TLA_EDITION_JA) ? 0x080a8000 :
                     DEFINED(TLA_EDITION_FR) ? 0x080b2000 :
                     DEFINED(TLA_EDITION_ES) ? 0x080b6000 :
                     DEFINED(TLA_EDITION_DE) ? 0x080ba000 :
                     0x080ad000);
        INCLUDE "rom_main_03.ld"
        . = ABSOLUTE(DEFINED(TLA_EDITION_FR) ? 0x080cd000 :
                     DEFINED(TLA_EDITION_ES) ? 0x080d1000 :
                     DEFINED(TLA_EDITION_DE) ? 0x080d5000 :
                     0x080c8000);
        INCLUDE "rom_main_04.ld"
        . = ALIGN(4);
        INCLUDE "rom_main_05.ld"
        . = ABSOLUTE(DEFINED(TLA_EDITION_FR) ? 0x080fa000 :
                     DEFINED(TLA_EDITION_ES) ? 0x080fe000 :
                     DEFINED(TLA_EDITION_DE) ? 0x08102000 :
                     0x080f8000);
        INCLUDE "rom_main_06.ld"
        . = ABSOLUTE(DEFINED(TLA_EDITION_ES) ? 0x0810c000 :
                     DEFINED(TLA_EDITION_DE) ? 0x08110000 :
                     0x08108000);
        INCLUDE "rom_main_07.ld"
        . = ABSOLUTE(0x08118000);
        INCLUDE "rom_main_08.ld"
        . = ABSOLUTE(0x08138000);
        INCLUDE "rom_main_09.ld"
        . = ABSOLUTE(0x081a0000);
        INCLUDE "rom_main_10.ld"
        . = ABSOLUTE(0x081a6000);
        INCLUDE "rom_main_11.ld"
        . = ABSOLUTE(0x081ac000);
        INCLUDE "rom_main_12.ld"
        . = ABSOLUTE(0x081b2000);
        INCLUDE "rom_main_13.ld"
        . = ABSOLUTE(0x081b8000);
        INCLUDE "rom_main_14.ld"
        . = ABSOLUTE(0x081c0000);
        INCLUDE "rom_main_15.ld"
    } > ROM
    .iwram_mixer 0x3006000 :
    {
        INCLUDE "iwram_mixer_00.ld"
    } > IWRAM AT>ROM
    iwram_high (NOLOAD) :
    {
        INCLUDE "iwram_high.ld"
    } > IWRAM
    .rom_tail :
    {
        INCLUDE "rom_tail_00.ld"
        . = ABSOLUTE(0x08300000);
        INCLUDE "rom_tail_01.ld"
        . = ABSOLUTE(0x08680000);
        INCLUDE "rom_tail_02.ld"
    } > ROM
    IwramRuntime_Rom = LOADADDR(.iwram_runtime);
    Sound_Mixer = LOADADDR(.iwram_mixer) + 1;
    Mixer_CallViaR3Rom = LOADADDR(.iwram_mixer) + (Mixer_CallViaR3 - ADDR(.iwram_mixer));
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
