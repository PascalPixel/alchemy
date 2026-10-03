/* resource_651: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/DERI_MURA/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/DERI_MURA/SCENE_TABLES.o"(.text)
        "*/resource_651_overlay.o"(.text.x02008044)
        "*/resource_651_overlay.o"(.text.x02008070)
        "*/resource_651_overlay.o"(.text.x0200809c)
        "*/resource_651_overlay.o"(.text.x02008118)
        "*/resource_651_overlay.o"(.text.x02008128)
        "*/games/THE LOST AGE/SRC/FIELD/DERI_MURA/TALK.o"(.text)
        "*/resource_651_overlay.o"(.text.x02008288)
        "*/games/THE LOST AGE/SRC/FIELD/DERI_MURA/HIP.o"(.text)
        "*/resource_651_overlay.o"(.text.x02008340)
        "*/resource_651_overlay.o"(.text.x020083a0)
        "*/resource_651_overlay.o"(.text.x02008924)
        "*/resource_651_overlay.o"(.text.x02008960)
        "*/resource_651_overlay.o"(.text.x020089a0)
        "*/resource_651_overlay.o"(.text.x020089e0)
        "*/resource_651_overlay.o"(.text.x02008a20)
        "*/resource_651_overlay.o"(.text.x02008a5c)
        "*/resource_651_overlay.o"(.text.x02008a9c)
        "*/resource_651_overlay.o"(.text.x02008ac8)
        "*/resource_651_overlay.o"(.text.x02008b44)
        "*/resource_651_overlay.o"(.text.x02008c30)
        "*/games/THE LOST AGE/SRC/FIELD/DERI_MURA/IMPORT.o"(.text)
        "*/resource_651_overlay.o"(.rodata.x02008d80)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
