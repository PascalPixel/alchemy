/* resource_3b8: its listing with maintained code linked in place, as pret
   links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_3b8_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/TABLES.o"(.text)
        "*/resource_3b8_overlay.o"(.text.x02008070)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/MIDDLE.o"(.text)
        "*/resource_3b8_overlay.o"(.text.x02008524)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/CUTSCENE.o"(.text)
        "*/resource_3b8_overlay.o"(.text.x0200be40)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/SHOWLINE.o"(.text)
        "*/resource_3b8_overlay.o"(.text.x0200bfc4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/WATER.o"(.text)
        "*/resource_3b8_overlay.o"(.text.x0200c034)
        "*/resource_3b8_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
