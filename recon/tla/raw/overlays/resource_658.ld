/* resource_658: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_658/ENTRY.o"(.text)
        "*/resource_658_overlay.o"(.text.x02008038)
        "*/resource_658_overlay.o"(.text.x02008040)
        "*/resource_658_overlay.o"(.text.x02008044)
        "*/resource_658_overlay.o"(.text.x0200804c)
        "*/resource_658_overlay.o"(.text.x0200806c)
        "*/resource_658_overlay.o"(.text.x02008108)
        "*/resource_658_overlay.o"(.text.x0200818c)
        "*/resource_658_overlay.o"(.text.x02008210)
        "*/resource_658_overlay.o"(.text.x02008294)
        "*/resource_658_overlay.o"(.text.x020082bc)
        "*/resource_658_overlay.o"(.text.x02008320)
        "*/resource_658_overlay.o"(.text.x0200836c)
        "*/resource_658_overlay.o"(.text.x020083b8)
        "*/resource_658_overlay.o"(.text.x02008404)
        "*/resource_658_overlay.o"(.text.x02008490)
        "*/resource_658_overlay.o"(.text.x0200851c)
        "*/resource_658_overlay.o"(.text.x020085b8)
        "*/resource_658_overlay.o"(.text.x02008638)
        "*/resource_658_overlay.o"(.text.x02008670)
        "*/resource_658_overlay.o"(.text.x0200868c)
        "*/resource_658_overlay.o"(.text.x020086ac)
        "*/resource_658_overlay.o"(.text.x02008838)
        "*/resource_658_overlay.o"(.text.x0200883c)
        "*/resource_658_overlay.o"(.text.x02008894)
        "*/resource_658_overlay.o"(.text.x02008968)
        "*/resource_658_overlay.o"(.text.x02008a48)
        "*/resource_658_overlay.o"(.text.x02008ac4)
        "*/resource_658_overlay.o"(.text.x02008f80)
        "*/resource_658_overlay.o"(.text.x020094fc)
        "*/resource_658_overlay.o"(.text.x020099d8)
        "*/resource_658_overlay.o"(.text.x0200a9ac)
        "*/resource_658_overlay.o"(.text.x0200a9b8)
        "*/resource_658_overlay.o"(.text.x0200a9c8)
        "*/resource_658_overlay.o"(.text.x0200aa58)
        "*/resource_658_overlay.o"(.text.x0200aa80)
        "*/resource_658_overlay.o"(.text.x0200aab0)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_658/IMPORT.o"(.text)
        "*/resource_658_overlay.o"(.rodata.x0200ac98)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
