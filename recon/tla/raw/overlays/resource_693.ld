/* resource_693: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_693/ENTRY.o"(.text)
        "*/resource_693_overlay.o"(.text.x02008038)
        "*/resource_693_overlay.o"(.text.x02008050)
        "*/resource_693_overlay.o"(.text.x02008058)
        "*/resource_693_overlay.o"(.text.x0200805c)
        "*/resource_693_overlay.o"(.text.x02008064)
        "*/resource_693_overlay.o"(.text.x020080e0)
        "*/resource_693_overlay.o"(.text.x02008120)
        "*/resource_693_overlay.o"(.text.x02008208)
        "*/resource_693_overlay.o"(.text.x020083e4)
        "*/resource_693_overlay.o"(.text.x02008640)
        "*/resource_693_overlay.o"(.text.x0200877c)
        "*/resource_693_overlay.o"(.text.x020087ac)
        "*/resource_693_overlay.o"(.text.x0200885c)
        "*/resource_693_overlay.o"(.text.x0200887c)
        "*/resource_693_overlay.o"(.text.x0200889c)
        "*/resource_693_overlay.o"(.text.x020088bc)
        "*/resource_693_overlay.o"(.text.x020088dc)
        "*/resource_693_overlay.o"(.text.x020088fc)
        "*/resource_693_overlay.o"(.text.x0200891c)
        "*/resource_693_overlay.o"(.text.x0200893c)
        "*/resource_693_overlay.o"(.text.x0200895c)
        "*/resource_693_overlay.o"(.text.x0200897c)
        "*/resource_693_overlay.o"(.text.x02008bb8)
        "*/resource_693_overlay.o"(.text.x02008bf0)
        "*/resource_693_overlay.o"(.text.x02008c14)
        "*/resource_693_overlay.o"(.text.x02008c8c)
        "*/resource_693_overlay.o"(.text.x02008d08)
        "*/resource_693_overlay.o"(.text.x02008d3c)
        "*/resource_693_overlay.o"(.text.x02008dc4)
        "*/resource_693_overlay.o"(.text.x02008ed0)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_693/IMPORT.o"(.text)
        "*/resource_693_overlay.o"(.rodata.x02009130)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
