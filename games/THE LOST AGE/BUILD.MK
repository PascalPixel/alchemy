# Source connected to the ordinary linker. Unconnected C remains preserved.
native:
	$(BUILD) native --target $(TARGET) --script "games/THE LOST AGE/MAIN.LD" --output out/$(TARGET)/native "games/THE LOST AGE/SRC/SYSTEM/CONSTANT_ZERO_RESULT.C"
