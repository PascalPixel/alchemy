SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/suhara_mura/entry.o"(.text)
        "*/games/tbs/src/field/suhara_mura/mura.o"(.text)
        "*/games/tbs/src/field/suhara_mura/import.o"(.text)
        "*/games/tbs/src/field/suhara_mura/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
