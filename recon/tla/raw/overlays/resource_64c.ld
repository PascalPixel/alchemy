/* resource_64c: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_64C/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_64C/SCENE_TABLES.o"(.text)
        "*/resource_64c_overlay.o"(.text.x0200805c)
        "*/resource_64c_overlay.o"(.text.x020080c0)
        "*/resource_64c_overlay.o"(.text.x02008138)
        "*/resource_64c_overlay.o"(.text.x0200820c)
        "*/resource_64c_overlay.o"(.text.x02008b64)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_64C/PREPARE.o"(.text)
        "*/resource_64c_overlay.o"(.text.x02008c9c)
        "*/resource_64c_overlay.o"(.text.x02008cbc)
        "*/resource_64c_overlay.o"(.text.x02008d10)
        "*/resource_64c_overlay.o"(.text.x02008d64)
        "*/resource_64c_overlay.o"(.text.x02008fa0)
        "*/resource_64c_overlay.o"(.text.x0200901c)
        "*/resource_64c_overlay.o"(.text.x02009044)
        "*/resource_64c_overlay.o"(.text.x02009064)
        "*/resource_64c_overlay.o"(.text.x020090b0)
        "*/resource_64c_overlay.o"(.text.x02009168)
        "*/resource_64c_overlay.o"(.text.x020091f4)
        "*/resource_64c_overlay.o"(.text.x0200922c)
        "*/resource_64c_overlay.o"(.text.x02009274)
        "*/resource_64c_overlay.o"(.text.x020092ac)
        "*/resource_64c_overlay.o"(.text.x020092f8)
        "*/resource_64c_overlay.o"(.text.x0200930c)
        "*/resource_64c_overlay.o"(.text.x02009364)
        "*/resource_64c_overlay.o"(.text.x02009400)
        "*/resource_64c_overlay.o"(.text.x0200999c)
        "*/resource_64c_overlay.o"(.text.x020099d4)
        "*/resource_64c_overlay.o"(.text.x02009a0c)
        "*/resource_64c_overlay.o"(.text.x02009a34)
        "*/resource_64c_overlay.o"(.text.x02009a80)
        "*/resource_64c_overlay.o"(.text.x02009ad0)
        "*/resource_64c_overlay.o"(.text.x02009be0)
        "*/resource_64c_overlay.o"(.text.x02009d90)
        "*/resource_64c_overlay.o"(.text.x02009e58)
        "*/resource_64c_overlay.o"(.text.x02009eb8)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_64C/IMPORT.o"(.text)
        "*/resource_64c_overlay.o"(.rodata.x0200a0bc)
    }
    .bss : { "*/resource_64c_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
