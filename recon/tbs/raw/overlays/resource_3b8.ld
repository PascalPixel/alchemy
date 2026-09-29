/* resource_3b8: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/GET_ENTRANCES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/TABLES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/GET_PLACEMENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/ROOMS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/MIDDLE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/PALACE_TALK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/CUTSCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/ACTOR8_TALK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/SHOWLINE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/WATER.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/GET_EVENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/SCENE_INIT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/IMPORT.o"(.text)
        "*/resource_3b8_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
