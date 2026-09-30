/* resource_684: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_684/ENTRY.o"(.text)
        "*/resource_684_overlay.o"(.text.x02008038)
        "*/resource_684_overlay.o"(.text.x02008040)
        "*/resource_684_overlay.o"(.text.x02008044)
        "*/resource_684_overlay.o"(.text.x0200804c)
        "*/resource_684_overlay.o"(.text.x02008054)
        "*/resource_684_overlay.o"(.text.x0200805c)
        "*/resource_684_overlay.o"(.text.x02008064)
        "*/resource_684_overlay.o"(.text.x020080a4)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_684/IMPORT.o"(.text)
        "*/resource_684_overlay.o"(.rodata.x020080c0)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
