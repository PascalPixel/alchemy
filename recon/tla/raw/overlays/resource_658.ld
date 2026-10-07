SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_658/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_658/SCENE_TABLES.o"(.text)
        "*/resource_658_overlay.o"(.text.x0200806c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_658/EVENTS_A.o"(.text)
        "*/resource_658_overlay.o"(.text.x020082bc)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_658/EVENTS_B.o"(.text)
        "*/resource_658_overlay.o"(.text.x0200851c)
        "*/resource_658_overlay.o"(.text.x020085b8)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_658/EVENTS_C.o"(.text)
        "*/resource_658_overlay.o"(.text.x0200868c)
        "*/resource_658_overlay.o"(.text.x020086ac)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_658/PREPARE.o"(.text)
        "*/resource_658_overlay.o"(.text.x02008ac4)
        "*/resource_658_overlay.o"(.text.x02008f80)
        "*/resource_658_overlay.o"(.text.x020094fc)
        "*/resource_658_overlay.o"(.text.x020099d8)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_658/EVENTS_D.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_658/IMPORT.o"(.text)
        "*/resource_658_overlay.o"(.rodata.x0200ac98)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
