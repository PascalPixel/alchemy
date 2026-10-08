SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MURA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MURA/MURA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_HASHI/FALLING_EFFECT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MURA/FLAG_BRANCH.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MURA/IMPORT.o"(.text)
        "*/resource_38f_overlay.o"(.rodata.x0200abd4)
    }
    .bss (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_HASHI/FALLING_EFFECT.o"(.bss)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MURA/MURA.o"(.bss)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
