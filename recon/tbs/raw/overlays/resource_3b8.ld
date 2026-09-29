/* resource_3b8: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/ENTRY.o"(.text)
        "*/resource_3b8_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/TABLES.o"(.text)
        "*/resource_3b8_overlay.o"(.text.x02008070)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/MIDDLE.o"(.text)
        "*/resource_3b8_overlay.o"(.text.x02008524)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/CUTSCENE.o"(.text)
        "*/resource_3b8_overlay.o"(.text.x0200be40)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/SHOWLINE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/WATER.o"(.text)
        "*/resource_3b8_overlay.o"(.text.x0200c034)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/IMPORT.o"(.text)
        "*/resource_3b8_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
