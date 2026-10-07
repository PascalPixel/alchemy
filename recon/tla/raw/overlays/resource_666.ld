SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_666/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_666/SCENE_TABLES.o"(.text)
        "*/resource_666_overlay.o"(.text.x0200804c)
        "*/resource_666_overlay.o"(.text.x02008084)
        "*/resource_666_overlay.o"(.text.x020081ac)
        "*/resource_666_overlay.o"(.text.x020081fc)
        "*/resource_666_overlay.o"(.text.x0200824c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_666/S0298.o"(.text)
        "*/resource_666_overlay.o"(.text.x020082bc)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_666/S0444.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_666/S047C.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_666/S04B8.o"(.text)
        "*/resource_666_overlay.o"(.text.x020084f4)
        "*/resource_666_overlay.o"(.text.x02008540)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_666/S0578.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_666/S0598.o"(.text)
        "*/resource_666_overlay.o"(.text.x020085b8)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_666/PREPARE.o"(.text)
        "*/resource_666_overlay.o"(.text.x02008738)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_666/IMPORT.o"(.text)
        "*/resource_666_overlay.o"(.rodata.x02008be4)
    }
    .bss : { "*/resource_666_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
