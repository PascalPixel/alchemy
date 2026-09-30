/* resource_65e: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65E/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65E/SCENE_TABLES.o"(.text)
        "*/resource_65e_overlay.o"(.text.x0200804c)
        "*/resource_65e_overlay.o"(.text.x02008078)
        "*/resource_65e_overlay.o"(.text.x020080a0)
        "*/resource_65e_overlay.o"(.text.x020080c8)
        "*/resource_65e_overlay.o"(.text.x020080f4)
        "*/resource_65e_overlay.o"(.text.x0200814c)
        "*/resource_65e_overlay.o"(.text.x0200819c)
        "*/resource_65e_overlay.o"(.text.x020081fc)
        "*/resource_65e_overlay.o"(.text.x02008288)
        "*/resource_65e_overlay.o"(.text.x020082e4)
        "*/resource_65e_overlay.o"(.text.x02008340)
        "*/resource_65e_overlay.o"(.text.x02008450)
        "*/resource_65e_overlay.o"(.text.x02008454)
        "*/resource_65e_overlay.o"(.text.x02008470)
        "*/resource_65e_overlay.o"(.text.x020084b0)
        "*/resource_65e_overlay.o"(.text.x020084dc)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65E/IMPORT.o"(.text)
        "*/resource_65e_overlay.o"(.rodata.x02008654)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
