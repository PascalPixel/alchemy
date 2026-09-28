# Source connected to the ordinary linker. Unconnected C remains preserved.
native:
	$(BUILD) native --target $(TARGET) --script "games/THE BROKEN SEAL/MAIN.LD" --output out/$(TARGET)/native "games/THE BROKEN SEAL/SRC/SYSTEM/MEMORY/CLEAR_WORD_IF_SET.C"
