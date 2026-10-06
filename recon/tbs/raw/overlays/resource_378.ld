SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/HEYA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/RESTART.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/ACTORS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/IMPORT.o"(.text)
        "*/resource_378_overlay.o"(.rodata.x0200b740)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
