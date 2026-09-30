/* resource_659: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_659/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_659/SCENE_TABLES.o"(.text)
        "*/resource_659_overlay.o"(.text.x0200804c)
        "*/resource_659_overlay.o"(.text.x020080a4)
        "*/resource_659_overlay.o"(.text.x020080fc)
        "*/resource_659_overlay.o"(.text.x020081b8)
        "*/resource_659_overlay.o"(.text.x0200823c)
        "*/resource_659_overlay.o"(.text.x0200828c)
        "*/resource_659_overlay.o"(.text.x02008308)
        "*/resource_659_overlay.o"(.text.x02008368)
        "*/resource_659_overlay.o"(.text.x020083c8)
        "*/resource_659_overlay.o"(.text.x02008428)
        "*/resource_659_overlay.o"(.text.x020084c4)
        "*/resource_659_overlay.o"(.text.x02008534)
        "*/resource_659_overlay.o"(.text.x02008570)
        "*/resource_659_overlay.o"(.text.x02008624)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_659/PREPARE.o"(.text)
        "*/resource_659_overlay.o"(.text.x020087ac)
        "*/resource_659_overlay.o"(.text.x020087c8)
        "*/resource_659_overlay.o"(.text.x020087dc)
        "*/resource_659_overlay.o"(.text.x02008ed8)
        "*/resource_659_overlay.o"(.text.x02008f0c)
        "*/resource_659_overlay.o"(.text.x02008fe0)
        "*/resource_659_overlay.o"(.text.x02009008)
        "*/resource_659_overlay.o"(.text.x02009310)
        "*/resource_659_overlay.o"(.text.x02009450)
        "*/resource_659_overlay.o"(.text.x02009910)
        "*/resource_659_overlay.o"(.text.x02009a20)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_659/IMPORT.o"(.text)
        "*/resource_659_overlay.o"(.rodata.x02009f18)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
