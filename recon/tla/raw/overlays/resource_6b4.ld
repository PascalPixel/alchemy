/* resource_6b4: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B4/ENTRY.o"(.text)
        "*/resource_6b4_overlay.o"(.text.x02008038)
        "*/resource_6b4_overlay.o"(.text.x02008080)
        "*/resource_6b4_overlay.o"(.text.x020080b8)
        "*/resource_6b4_overlay.o"(.text.x02008280)
        "*/resource_6b4_overlay.o"(.text.x02008288)
        "*/resource_6b4_overlay.o"(.text.x0200828c)
        "*/resource_6b4_overlay.o"(.text.x02008294)
        "*/resource_6b4_overlay.o"(.text.x0200829c)
        "*/resource_6b4_overlay.o"(.text.x020082a4)
        "*/resource_6b4_overlay.o"(.text.x0200832c)
        "*/resource_6b4_overlay.o"(.text.x02008394)
        "*/resource_6b4_overlay.o"(.text.x02008420)
        "*/resource_6b4_overlay.o"(.text.x02008478)
        "*/resource_6b4_overlay.o"(.text.x02008564)
        "*/resource_6b4_overlay.o"(.text.x02009408)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B4/IMPORT.o"(.text)
        "*/resource_6b4_overlay.o"(.rodata.x0200959c)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
