/* resource_3a3: its listing with maintained code linked in place, as pret
   links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_3a3_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_MURA/SCENE_TABLES.o"(.text)
        "*/resource_3a3_overlay.o"(.text.x0200807c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_MURA/MOTION_PARTICLE.o"(.text)
        "*/resource_3a3_overlay.o"(.text.x02008874)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_MURA/MIDDLE_SEQUENCE.o"(.text)
        "*/resource_3a3_overlay.o"(.text.x02008d58)
        "*/resource_3a3_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
