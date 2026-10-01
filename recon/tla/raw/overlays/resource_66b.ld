/* resource_66b: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66B/ENTRY.o"(.text)
        "*/resource_66b_overlay.o"(.text.x02008038)
        "*/resource_66b_overlay.o"(.text.x02008058)
        "*/resource_66b_overlay.o"(.text.x02008064)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66B/SCENE_TABLES.o"(.text)
        "*/resource_66b_overlay.o"(.text.x0200809c)
        "*/games/THE LOST AGE/SRC/FIELD/COMMON/OBJECT/SET_SPRITE_PRIORITY.o"(.text)
        "*/resource_66b_overlay.o"(.text.x0200810e)
        "*/resource_66b_overlay.o"(.text.x02008110)
        "*/resource_66b_overlay.o"(.text.x02008160)
        "*/resource_66b_overlay.o"(.text.x020081b4)
        "*/resource_66b_overlay.o"(.text.x020081ec)
        "*/resource_66b_overlay.o"(.text.x020083cc)
        "*/resource_66b_overlay.o"(.text.x020083dc)
        "*/resource_66b_overlay.o"(.text.x020083f8)
        "*/resource_66b_overlay.o"(.text.x02008584)
        "*/resource_66b_overlay.o"(.text.x020085bc)
        "*/resource_66b_overlay.o"(.text.x02008600)
        "*/resource_66b_overlay.o"(.text.x02008610)
        "*/resource_66b_overlay.o"(.text.x02008660)
        "*/resource_66b_overlay.o"(.text.x020086f4)
        "*/resource_66b_overlay.o"(.text.x0200870c)
        "*/resource_66b_overlay.o"(.text.x02008738)
        "*/resource_66b_overlay.o"(.text.x02008768)
        "*/resource_66b_overlay.o"(.text.x020087d8)
        "*/resource_66b_overlay.o"(.text.x02008988)
        "*/resource_66b_overlay.o"(.text.x02008bb4)
        "*/resource_66b_overlay.o"(.text.x02008be4)
        "*/resource_66b_overlay.o"(.text.x02008dac)
        "*/resource_66b_overlay.o"(.text.x02008fcc)
        "*/resource_66b_overlay.o"(.text.x02009068)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66B/IMPORT.o"(.text)
        "*/resource_66b_overlay.o"(.rodata.x02009324)
    }
    .bss : { "*/resource_66b_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
