/* resource_650: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/IDEJIMA/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/IDEJIMA/SCENE_TABLES.o"(.text)
        "*/resource_650_overlay.o"(.text.x02008098)
        "*/games/THE LOST AGE/SRC/FIELD/IDEJIMA/WAKE.o"(.text)
        "*/resource_650_overlay.o"(.text.x0200ac64)
        "*/games/THE LOST AGE/SRC/FIELD/IDEJIMA/PREPARE.o"(.text)
        "*/resource_650_overlay.o"(.text.x0200af88)
        "*/games/THE LOST AGE/SRC/FIELD/IDEJIMA/IMPORT.o"(.text)
        "*/resource_650_overlay.o"(.rodata.x0200b330)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
