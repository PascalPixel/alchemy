/* resource_652: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/DERI_HEYA/ENTRY.o"(.text)
        "*/resource_652_overlay.o"(.text.x02008038)
        "*/resource_652_overlay.o"(.text.x02008040)
        "*/resource_652_overlay.o"(.text.x02008044)
        "*/resource_652_overlay.o"(.text.x02008064)
        "*/resource_652_overlay.o"(.text.x020080ac)
        "*/resource_652_overlay.o"(.text.x02008150)
        "*/games/THE LOST AGE/SRC/FIELD/DERI_HEYA/LINES.o"(.text)
        "*/resource_652_overlay.o"(.text.x02008218)
        "*/resource_652_overlay.o"(.text.x02008270)
        "*/resource_652_overlay.o"(.text.x020082d0)
        "*/resource_652_overlay.o"(.text.x02008330)
        "*/resource_652_overlay.o"(.text.x02008390)
        "*/resource_652_overlay.o"(.text.x02008498)
        "*/resource_652_overlay.o"(.text.x020084f4)
        "*/resource_652_overlay.o"(.text.x02008554)
        "*/resource_652_overlay.o"(.text.x020085b4)
        "*/resource_652_overlay.o"(.text.x02008614)
        "*/resource_652_overlay.o"(.text.x0200866c)
        "*/resource_652_overlay.o"(.text.x020086c4)
        "*/resource_652_overlay.o"(.text.x02008704)
        "*/resource_652_overlay.o"(.text.x02008730)
        "*/resource_652_overlay.o"(.text.x02008878)
        "*/resource_652_overlay.o"(.text.x02008a50)
        "*/resource_652_overlay.o"(.text.x02008c0c)
        "*/resource_652_overlay.o"(.text.x02008c58)
        "*/resource_652_overlay.o"(.text.x02008c74)
        "*/games/THE LOST AGE/SRC/FIELD/DERI_HEYA/IMPORT.o"(.text)
        "*/resource_652_overlay.o"(.rodata.x02008e04)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
