SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/korima_mura/entry.o"(.text)
        "*/games/tbs/src/field/korima_mura/mura.o"(.text)
        "*/games/tbs/src/field/korima_hashi/falling_effect.o"(.text)
        "*/games/tbs/src/field/korima_mura/flag_branch.o"(.text)
        "*/games/tbs/src/field/korima_mura/import.o"(.text)
        "*/games/tbs/src/field/korima_mura/data.o"(.data)
    }
    .bss (NOLOAD) :
    {
        "*/games/tbs/src/field/korima_hashi/falling_effect.o"(.bss)
        "*/games/tbs/src/field/korima_mura/mura.o"(.bss)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
