/* resource_66e: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66E/ENTRY.o"(.text)
        "*/resource_66e_overlay.o"(.text.x02008038)
        "*/resource_66e_overlay.o"(.text.x02008040)
        "*/resource_66e_overlay.o"(.text.x02008044)
        "*/resource_66e_overlay.o"(.text.x0200804c)
        "*/resource_66e_overlay.o"(.text.x02008054)
        "*/resource_66e_overlay.o"(.text.x0200807c)
        "*/resource_66e_overlay.o"(.text.x020080a4)
        "*/resource_66e_overlay.o"(.text.x020080c4)
        "*/resource_66e_overlay.o"(.text.x02008124)
        "*/resource_66e_overlay.o"(.text.x02008184)
        "*/resource_66e_overlay.o"(.text.x020081e4)
        "*/resource_66e_overlay.o"(.text.x02008244)
        "*/resource_66e_overlay.o"(.text.x020082a4)
        "*/resource_66e_overlay.o"(.text.x02008304)
        "*/resource_66e_overlay.o"(.text.x02008390)
        "*/resource_66e_overlay.o"(.text.x02008760)
        "*/resource_66e_overlay.o"(.text.x020087b4)
        "*/resource_66e_overlay.o"(.text.x02008838)
        "*/resource_66e_overlay.o"(.text.x02008878)
        "*/resource_66e_overlay.o"(.text.x02008920)
        "*/resource_66e_overlay.o"(.text.x0200897c)
        "*/resource_66e_overlay.o"(.text.x020089a4)
        "*/resource_66e_overlay.o"(.text.x020089f8)
        "*/resource_66e_overlay.o"(.text.x02008a7c)
        "*/resource_66e_overlay.o"(.text.x02008b60)
        "*/resource_66e_overlay.o"(.text.x02008ba0)
        "*/resource_66e_overlay.o"(.text.x02008c04)
        "*/resource_66e_overlay.o"(.text.x02008f28)
        "*/resource_66e_overlay.o"(.text.x02009064)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66E/IMPORT.o"(.text)
        "*/resource_66e_overlay.o"(.rodata.x020091b0)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
