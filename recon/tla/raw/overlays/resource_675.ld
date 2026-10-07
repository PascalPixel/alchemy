SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_675/ENTRY.o"(.text)
        "*/resource_675_overlay.o"(.text.x02008038)
        "*/resource_675_overlay.o"(.text.x020080f4)
        "*/resource_675_overlay.o"(.text.x02008158)
        "*/resource_675_overlay.o"(.text.x02008178)
        "*/resource_675_overlay.o"(.text.x020081d4)
        "*/resource_675_overlay.o"(.text.x020081f8)
        "*/resource_675_overlay.o"(.text.x02008328)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_675/EVENTS2.o"(.text)
        "*/resource_675_overlay.o"(.text.x020083d4)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_675/SCENE_TABLES.o"(.text)
        "*/resource_675_overlay.o"(.text.x0200842c)
        "*/resource_675_overlay.o"(.text.x0200846c)
        "*/resource_675_overlay.o"(.text.x020084ac)
        "*/resource_675_overlay.o"(.text.x02008508)
        "*/resource_675_overlay.o"(.text.x02008544)
        "*/resource_675_overlay.o"(.text.x02008580)
        "*/resource_675_overlay.o"(.text.x020085fc)
        "*/resource_675_overlay.o"(.text.x0200869c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_675/PREPARE.o"(.text)
        "*/resource_675_overlay.o"(.text.x020087dc)
        "*/resource_675_overlay.o"(.text.x02008848)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_675/EVENTS.o"(.text)
        "*/resource_675_overlay.o"(.text.x020089f8)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_675/EVENTS3.o"(.text)
        "*/resource_675_overlay.o"(.text.x020090ac)
        "*/resource_675_overlay.o"(.text.x0200915c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_675/IMPORT.o"(.text)
        "*/resource_675_overlay.o"(.rodata.x0200934c)
    }
    .bss : { "*/resource_675_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
