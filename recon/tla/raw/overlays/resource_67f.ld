/* resource_67f: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_67F/ENTRY.o"(.text)
        "*/resource_67f_overlay.o"(.text.x02008038)
        "*/resource_67f_overlay.o"(.text.x020080a4)
        "*/resource_67f_overlay.o"(.text.x02008128)
        "*/resource_67f_overlay.o"(.text.x020081d0)
        "*/resource_67f_overlay.o"(.text.x020082f8)
        "*/resource_67f_overlay.o"(.text.x0200848c)
        "*/resource_67f_overlay.o"(.text.x0200856c)
        "*/resource_67f_overlay.o"(.text.x02008584)
        "*/resource_67f_overlay.o"(.text.x020086ec)
        "*/resource_67f_overlay.o"(.text.x02008708)
        "*/resource_67f_overlay.o"(.text.x020088f4)
        "*/resource_67f_overlay.o"(.text.x02008974)
        "*/resource_67f_overlay.o"(.text.x02008aa8)
        "*/resource_67f_overlay.o"(.text.x02008bc0)
        "*/resource_67f_overlay.o"(.text.x02008c4c)
        "*/games/THE LOST AGE/SRC/FIELD/COMMON/OBJECT/SET_SPRITE_PRIORITY.o"(.text)
        "*/resource_67f_overlay.o"(.text.x02008caa)
        "*/resource_67f_overlay.o"(.text.x02008cac)
        "*/resource_67f_overlay.o"(.text.x02008ce4)
        "*/resource_67f_overlay.o"(.text.x02008eac)
        "*/resource_67f_overlay.o"(.text.x02008ee8)
        "*/resource_67f_overlay.o"(.text.x02008f30)
        "*/resource_67f_overlay.o"(.text.x0200900c)
        "*/resource_67f_overlay.o"(.text.x02009068)
        "*/resource_67f_overlay.o"(.text.x02009070)
        "*/resource_67f_overlay.o"(.text.x020090a8)
        "*/resource_67f_overlay.o"(.text.x020090b0)
        "*/resource_67f_overlay.o"(.text.x020090f0)
        "*/resource_67f_overlay.o"(.text.x02009148)
        "*/resource_67f_overlay.o"(.text.x02009184)
        "*/resource_67f_overlay.o"(.text.x020091b8)
        "*/resource_67f_overlay.o"(.text.x020091d4)
        "*/resource_67f_overlay.o"(.text.x02009228)
        "*/resource_67f_overlay.o"(.text.x02009270)
        "*/resource_67f_overlay.o"(.text.x020092b0)
        "*/resource_67f_overlay.o"(.text.x020092dc)
        "*/resource_67f_overlay.o"(.text.x020093ec)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_67F/IMPORT.o"(.text)
        "*/resource_67f_overlay.o"(.rodata.x02009540)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
