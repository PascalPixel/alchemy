/* resource_672: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_672/ENTRY.o"(.text)
        "*/resource_672_overlay.o"(.text.x02008038)
        "*/resource_672_overlay.o"(.text.x02008040)
        "*/resource_672_overlay.o"(.text.x02008068)
        "*/resource_672_overlay.o"(.text.x02008070)
        "*/resource_672_overlay.o"(.text.x020080b0)
        "*/resource_672_overlay.o"(.text.x02008100)
        "*/resource_672_overlay.o"(.text.x02008184)
        "*/resource_672_overlay.o"(.text.x02008a98)
        "*/resource_672_overlay.o"(.text.x02008b60)
        "*/resource_672_overlay.o"(.text.x02008b70)
        "*/resource_672_overlay.o"(.text.x02008be8)
        "*/resource_672_overlay.o"(.text.x02008d3c)
        "*/resource_672_overlay.o"(.text.x02008e50)
        "*/resource_672_overlay.o"(.text.x02008e60)
        "*/resource_672_overlay.o"(.text.x02008ef0)
        "*/resource_672_overlay.o"(.text.x02008f00)
        "*/resource_672_overlay.o"(.text.x02008f88)
        "*/resource_672_overlay.o"(.text.x02009010)
        "*/resource_672_overlay.o"(.text.x02009064)
        "*/resource_672_overlay.o"(.text.x02009094)
        "*/resource_672_overlay.o"(.text.x020090d4)
        "*/resource_672_overlay.o"(.text.x02009710)
        "*/resource_672_overlay.o"(.text.x02009aa4)
        "*/resource_672_overlay.o"(.text.x02009b10)
        "*/resource_672_overlay.o"(.text.x02009b14)
        "*/resource_672_overlay.o"(.text.x02009ba8)
        "*/resource_672_overlay.o"(.text.x02009c44)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_672/IMPORT.o"(.text)
        "*/resource_672_overlay.o"(.rodata.x02009f18)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
