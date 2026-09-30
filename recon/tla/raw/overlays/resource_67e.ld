/* resource_67e: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_67E/ENTRY.o"(.text)
        "*/resource_67e_overlay.o"(.text.x02008038)
        "*/resource_67e_overlay.o"(.text.x02008048)
        "*/resource_67e_overlay.o"(.text.x02008198)
        "*/resource_67e_overlay.o"(.text.x020081a0)
        "*/resource_67e_overlay.o"(.text.x020081a4)
        "*/resource_67e_overlay.o"(.text.x020081ac)
        "*/resource_67e_overlay.o"(.text.x020081cc)
        "*/resource_67e_overlay.o"(.text.x020081ec)
        "*/resource_67e_overlay.o"(.text.x0200827c)
        "*/resource_67e_overlay.o"(.text.x0200830c)
        "*/resource_67e_overlay.o"(.text.x0200832c)
        "*/resource_67e_overlay.o"(.text.x0200834c)
        "*/resource_67e_overlay.o"(.text.x0200836c)
        "*/resource_67e_overlay.o"(.text.x020083f8)
        "*/resource_67e_overlay.o"(.text.x02008418)
        "*/resource_67e_overlay.o"(.text.x02008438)
        "*/resource_67e_overlay.o"(.text.x0200849c)
        "*/resource_67e_overlay.o"(.text.x020084bc)
        "*/resource_67e_overlay.o"(.text.x02008538)
        "*/resource_67e_overlay.o"(.text.x020085b4)
        "*/resource_67e_overlay.o"(.text.x0200860c)
        "*/resource_67e_overlay.o"(.text.x020086a4)
        "*/resource_67e_overlay.o"(.text.x02008814)
        "*/resource_67e_overlay.o"(.text.x02008954)
        "*/resource_67e_overlay.o"(.text.x020089d0)
        "*/resource_67e_overlay.o"(.text.x02008b04)
        "*/resource_67e_overlay.o"(.text.x02008b08)
        "*/resource_67e_overlay.o"(.text.x02008ba4)
        "*/resource_67e_overlay.o"(.text.x02008bdc)
        "*/resource_67e_overlay.o"(.text.x02008c58)
        "*/resource_67e_overlay.o"(.text.x02008c7c)
        "*/resource_67e_overlay.o"(.text.x02008ca4)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_67E/IMPORT.o"(.text)
        "*/resource_67e_overlay.o"(.rodata.x02008e10)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
