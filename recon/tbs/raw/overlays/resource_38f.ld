/* resource_38f: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MURA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MURA/MURA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_HASHI/FALLING_EFFECT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MURA/FLAG_BRANCH.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MURA/IMPORT.o"(.text)
        "*/resource_38f_overlay.o"(.rodata)
    }
    /* The overlay's own work, past its loaded image. */
    .bss (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_HASHI/FALLING_EFFECT.o"(.bss)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MURA/MURA.o"(.bss)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
