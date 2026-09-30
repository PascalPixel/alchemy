/* resource_66a: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66A/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66A/SCENE_TABLES.o"(.text)
        "*/resource_66a_overlay.o"(.text.x0200804c)
        "*/resource_66a_overlay.o"(.text.x02008084)
        "*/resource_66a_overlay.o"(.text.x020080a0)
        "*/resource_66a_overlay.o"(.text.x020080ec)
        "*/resource_66a_overlay.o"(.text.x020080fc)
        "*/resource_66a_overlay.o"(.text.x02008168)
        "*/resource_66a_overlay.o"(.text.x020081d4)
        "*/resource_66a_overlay.o"(.text.x020081e8)
        "*/resource_66a_overlay.o"(.text.x02008220)
        "*/resource_66a_overlay.o"(.text.x02008258)
        "*/resource_66a_overlay.o"(.text.x0200827c)
        "*/resource_66a_overlay.o"(.text.x020082f8)
        "*/resource_66a_overlay.o"(.text.x0200851c)
        "*/resource_66a_overlay.o"(.text.x020087a4)
        "*/resource_66a_overlay.o"(.text.x02008818)
        "*/resource_66a_overlay.o"(.text.x02009220)
        "*/resource_66a_overlay.o"(.text.x02009228)
        "*/resource_66a_overlay.o"(.text.x02009230)
        "*/resource_66a_overlay.o"(.text.x0200929c)
        "*/resource_66a_overlay.o"(.text.x02009320)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66A/IMPORT.o"(.text)
        "*/resource_66a_overlay.o"(.rodata.x02009608)
    }
    .bss : { "*/resource_66a_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
