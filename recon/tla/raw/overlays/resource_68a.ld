/* resource_68a: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68A/ENTRY.o"(.text)
        "*/resource_68a_overlay.o"(.text.x02008038)
        "*/resource_68a_overlay.o"(.text.x02008048)
        "*/resource_68a_overlay.o"(.text.x02008080)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68A/SCENE_TABLES.o"(.text)
        "*/resource_68a_overlay.o"(.text.x020080b0)
        "*/resource_68a_overlay.o"(.text.x020080c4)
        "*/resource_68a_overlay.o"(.text.x020080d8)
        "*/resource_68a_overlay.o"(.text.x02008110)
        "*/resource_68a_overlay.o"(.text.x020083bc)
        "*/resource_68a_overlay.o"(.text.x020083dc)
        "*/resource_68a_overlay.o"(.text.x020083fc)
        "*/resource_68a_overlay.o"(.text.x0200841c)
        "*/resource_68a_overlay.o"(.text.x0200843c)
        "*/resource_68a_overlay.o"(.text.x0200845c)
        "*/resource_68a_overlay.o"(.text.x02008468)
        "*/resource_68a_overlay.o"(.text.x02008534)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68A/SCENE_EVENTS.o"(.text)
        "*/resource_68a_overlay.o"(.text.x02008630)
        "*/resource_68a_overlay.o"(.text.x020086bc)
        "*/resource_68a_overlay.o"(.text.x020087a4)
        "*/resource_68a_overlay.o"(.text.x02008830)
        "*/resource_68a_overlay.o"(.text.x02008838)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68A/IMPORT.o"(.text)
        "*/resource_68a_overlay.o"(.rodata.x02009a34)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
