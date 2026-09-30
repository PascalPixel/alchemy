/* resource_64f: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/SUHARA_GATE/ENTRY.o"(.text)
        "*/resource_64f_overlay.o"(.text.x02008038)
        "*/resource_64f_overlay.o"(.text.x02008044)
        "*/games/THE LOST AGE/SRC/FIELD/SUHARA_GATE/SCENE_TABLES.o"(.text)
        "*/resource_64f_overlay.o"(.text.x02008090)
        "*/resource_64f_overlay.o"(.text.x020080d0)
        "*/resource_64f_overlay.o"(.text.x02008110)
        "*/resource_64f_overlay.o"(.text.x02008168)
        "*/resource_64f_overlay.o"(.text.x02008264)
        "*/resource_64f_overlay.o"(.text.x0200828c)
        "*/resource_64f_overlay.o"(.text.x020082e0)
        "*/resource_64f_overlay.o"(.text.x02008308)
        "*/resource_64f_overlay.o"(.text.x02008378)
        "*/resource_64f_overlay.o"(.text.x020083a8)
        "*/resource_64f_overlay.o"(.text.x020083ac)
        "*/resource_64f_overlay.o"(.text.x02008494)
        "*/resource_64f_overlay.o"(.text.x020085e8)
        "*/resource_64f_overlay.o"(.text.x02008650)
        "*/resource_64f_overlay.o"(.text.x0200871c)
        "*/resource_64f_overlay.o"(.text.x02008800)
        "*/resource_64f_overlay.o"(.text.x020088cc)
        "*/games/THE LOST AGE/SRC/FIELD/SUHARA_GATE/IMPORT.o"(.text)
        "*/resource_64f_overlay.o"(.rodata.x02008aa8)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
