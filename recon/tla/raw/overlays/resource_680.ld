/* resource_680: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_680/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_680/SCENE_TABLES.o"(.text)
        "*/resource_680_overlay.o"(.text.x02008040)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_680/SCENE_TABLES_2.o"(.text)
        "*/resource_680_overlay.o"(.text.x02008080)
        "*/resource_680_overlay.o"(.text.x020080d0)
        "*/resource_680_overlay.o"(.text.x020080e0)
        "*/resource_680_overlay.o"(.text.x020080f0)
        "*/resource_680_overlay.o"(.text.x02008100)
        "*/resource_680_overlay.o"(.text.x02008148)
        "*/resource_680_overlay.o"(.text.x02008164)
        "*/resource_680_overlay.o"(.text.x02008258)
        "*/resource_680_overlay.o"(.text.x0200827c)
        "*/resource_680_overlay.o"(.text.x020082a0)
        "*/resource_680_overlay.o"(.text.x020082c0)
        "*/resource_680_overlay.o"(.text.x020082fc)
        "*/resource_680_overlay.o"(.text.x02008314)
        "*/resource_680_overlay.o"(.text.x02008378)
        "*/resource_680_overlay.o"(.text.x020083c4)
        "*/resource_680_overlay.o"(.text.x020083fc)
        "*/resource_680_overlay.o"(.text.x02008434)
        "*/resource_680_overlay.o"(.text.x02008474)
        "*/resource_680_overlay.o"(.text.x02008528)
        "*/resource_680_overlay.o"(.text.x020085f0)
        "*/resource_680_overlay.o"(.text.x02008630)
        "*/resource_680_overlay.o"(.text.x02008670)
        "*/resource_680_overlay.o"(.text.x020086ec)
        "*/resource_680_overlay.o"(.text.x02008a68)
        "*/resource_680_overlay.o"(.text.x02008ab4)
        "*/resource_680_overlay.o"(.text.x02008b2c)
        "*/resource_680_overlay.o"(.text.x02008bc0)
        "*/resource_680_overlay.o"(.text.x02008dd0)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_680/IMPORT.o"(.text)
        "*/resource_680_overlay.o"(.rodata.x02008f2c)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
