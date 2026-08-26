# Alchemy has six executables: two builders and four contributor-facing hosts.
# Game-specific asset codecs are libraries behind `assets`; they are not public
# commands and are not copied when starting another decompilation project.

GCC296_CFLAGS := -O2 -mthumb -mthumb-interwork -mcpu=arm7tdmi \
                 -fno-builtin -nostdinc -ffreestanding \
                 -fcall-used-r4 -Igames/gs1/include

TOOLS := tools
CARGO ?= cargo
CARGO_RUN := $(CARGO) run --offline --quiet --release --manifest-path

BUILD := $(CARGO_RUN) $(TOOLS)/build-stage/Cargo.toml --
ASSETS := $(CARGO_RUN) $(TOOLS)/build-assets/Cargo.toml --
CHECK := $(CARGO_RUN) $(TOOLS)/check/Cargo.toml --
COMPILER := $(CARGO_RUN) $(TOOLS)/compiler/Cargo.toml --
OVERLAY := $(CARGO_RUN) $(TOOLS)/overlay/Cargo.toml --

HOSTS := build-assets build-stage assets compiler overlay check
CORE_TESTS := compiler-core candidate-compiler candidate-show permuter \
              overlay-disasm overlay-adopt route-dump build-full \
              extract-resource coverage-map check-publication
PORTABLE_TOOLS := alignment-tail asset-paths cache-entry canonical-json \
	generated-files no-asm-c build-stage build-claimed build-asm build-full \
	compiler compiler-core candidate-compiler candidate-show permuter \
	dashboard-server overlay overlay-disasm overlay-show \
	overlay-adopt overlay-call-targets check check-commit-progress \
	check-publication check-unmatchable core-retained-audit coverage-map \
	full-c-progress integrate-matches route-dump decomp-targets
TOOLING_LINE_LIMIT := 30000
TARGET ?= gs1-en
FULL_REPORT = out/$(TARGET)/full/rebuilt.json
FULL_ROM = out/$(TARGET)/full/rebuilt.gba
OWNER_INVENTORY = out/$(TARGET)/full/rebuilt.owner-inventory.json
HISTORICAL_TARGETS := gs1-ja gs1-en gs1-de gs1-es gs1-fr gs1-it \
	gs2-ja gs2-en gs2-de gs2-es gs2-fr gs2-it
CANDIDATE_SINGLE_OWNERS := \
	08090824=initialize_display_transition_state.c \
	08091174=initialize_battle_effect_buffers.c \
	080944ec=arm_display_scroll_hblank_dma.c \
	080b5ad4=initialize_battle_tile_pattern.c \
	080b7f20=project_battle_object_position.c \
	080b81c8=initialize_battle_object_motion.c \
	080b845c=project_scaled_battle_position.c \
	080b84c0=project_conditional_battle_position.c \
	080c0184=upload_battle_tile_variant.c \
	080c0eb8=initialize_battle_transition_entries.c \
	080f377c=initialize_title_palette_buffers.c

.PHONY: help verify test lint build-tools tool-tests tooling-size \
	build-claimed build-asm build-assets build-full build-rom \
	standard-check pristine-options-check corpus-check core-retained-check \
	full-rom-check overlay-check declared-tu-check owner-inventory-check strict-tu-check classification-check \
	candidate-corpus-check source-tracking-check check-owners progress progress-check progress-subject \
	correspondence correspondence-check edition-builds edition-builds-check \
	families family-check coverage coverage-check dashboard clean clean-preview
.PHONY: targets $(HISTORICAL_TARGETS)

help:
	@printf '%s\n' \
		'make verify           authoritative byte-exact gate' \
		'make targets          compile shared source for all 12 historical targets' \
		'make gs1-ja           compile one edition-qualified source target' \
		'make build-rom        rebuild the ROM' \
		'make build-full       rebuild and compare every owned byte' \
		'make full-rom-check   prove the complete gs1-en ROM byte-exact' \
		'make overlay-check    audit every exact overlay owner' \
		'make declared-tu-check prove declared production translation-unit contracts' \
		'make owner-inventory-check prove registered owner production coverage' \
		'make strict-tu-check  prove strict production TU composition and owner coverage' \
		'make classification-check prove retained-assembly classifications' \
		'make candidate-corpus-check rescore retained reconstruction C' \
		'make source-tracking-check reject ignored or untracked exact C' \
		'make build-assets     rebuild source assets' \
		'make test             focused Rust tests and policy checks' \
		'make tooling-size     enforce the portable-toolkit budget' \
		'make progress         print byte-exact progress' \
		'make progress-subject print the required commit prefix' \
		'make correspondence   match exact EN owners across GS1 editions' \
		'make families         rank unresolved compiler owners by exact-C family' \
		'make family-check     prove the family index and retained-family evidence' \
		'make edition-builds   relink exact EN C across GS1 editions' \
		'make coverage         refresh dashboard data and figures' \
		'make dashboard        serve the dashboard on localhost:4649'

build-claimed:
	$(BUILD) claimed --target $(TARGET)

build-asm:
	$(BUILD) asm

build-assets:
	$(ASSETS)

build-full:
	$(BUILD) full --target $(TARGET)

build-rom:
	$(BUILD) rom --target $(TARGET)

full-rom-check: build-full
	@cmp roms/$(TARGET).gba $(FULL_ROM)
	@grep -Fq '"target": "$(TARGET)"' $(FULL_REPORT)
	@grep -Fq '"verification": "rom"' $(FULL_REPORT)
	@grep -Fq '"byte_identical": true' $(FULL_REPORT)
	@grep -Fq '"unowned_bytes": 0' $(FULL_REPORT)
	@grep -Fq '"rom_fallback_bytes": 0' $(FULL_REPORT)
	@printf 'full ROM contract ok: %s\n' '$(TARGET)'

overlay-check:
	$(OVERLAY) audit --all

declared-tu-check: full-rom-check
	@grep -Fq '"declared_main_translation_units_strict": true' $(FULL_REPORT)
	@printf 'declared translation-unit production contract ok\n'

owner-inventory-check: full-rom-check
	@test -s $(OWNER_INVENTORY)
	@grep -Fq '"complete_registered_identity_coverage": true' $(OWNER_INVENTORY)
	@printf 'registered owner production inventory ok\n'

strict-tu-check: declared-tu-check owner-inventory-check overlay-check
	@printf 'strict production TU contracts ok; historical original boundaries remain unknown\n'

targets: $(HISTORICAL_TARGETS)

$(HISTORICAL_TARGETS):
	$(BUILD) claimed --target $@ --compile-only --output out/$@/compile

dashboard:
	$(COMPILER) dashboard-server --bind 0.0.0.0:4649

progress:
	$(CHECK) progress

progress-check:
	$(CHECK) progress --check

progress-subject:
	$(CHECK) progress --subject

correspondence: build-claimed
	$(COMPILER) cross-edition --all --write games/gs1/recon/exact-correspondence.json
	$(COMPILER) cross-edition --all-overlays --write games/gs1/recon/exact-overlay-correspondence.json \
		--edition-build games/gs1/recon/exact-overlay-builds.json

correspondence-check: edition-builds-check
	$(COMPILER) cross-edition --all-overlays --write out/exact-overlay-correspondence.check.json \
		--edition-build out/exact-overlay-builds.check.json
	cmp games/gs1/recon/exact-overlay-correspondence.json out/exact-overlay-correspondence.check.json
	cmp games/gs1/recon/exact-overlay-builds.json out/exact-overlay-builds.check.json

families: build-claimed build-asm
	$(COMPILER) families cluster --write games/gs1/recon/compiler-families.json

family-check: build-claimed build-asm
	$(COMPILER) families cluster --check games/gs1/recon/compiler-families.json
	$(COMPILER) families prove games/gs1/recon/family-retention.json

classification-check: core-retained-check family-check
	@printf 'classification and family-retention contracts ok\n'

candidate-corpus-check:
	$(CHECK) integrate --check games/gs1/recon/en/main
	@actual=$$(find games/gs1/recon/en/units -maxdepth 1 -type f -name '*.c' -exec basename {} \; | LC_ALL=C sort); \
	covered=$$({ for route in $(CANDIDATE_SINGLE_OWNERS); do printf '%s\n' "$${route#*=}"; done; \
		awk -F '"' '/"source": "games\/gs1\/recon\/en\/units\//{n=split($$4,part,"/"); print part[n]}' \
			games/gs1/recon/translation-units.json; } | LC_ALL=C sort -u); \
	if test "$$actual" != "$$covered"; then \
		printf 'unit corpus routes are incomplete\nactual:\n%s\ncovered:\n%s\n' "$$actual" "$$covered"; \
		exit 1; \
	fi
	@set -e; total=0; \
	for route in $(CANDIDATE_SINGLE_OWNERS); do \
		owner=$${route%%=*}; source=$${route#*=}; \
		result=$$($(COMPILER) candidate-show games/gs1/recon/en/units/$$source \
			--owner $$owner --first); \
		diff=$$(printf '%s\n' "$$result" | sed -n 's/.*differing_halfwords=\([0-9][0-9]*\).*/\1/p' | head -n 1); \
		if test -z "$$diff" || test "$$diff" -eq 0; then \
			printf 'single-owner candidate is exact or unscored: %s %s\n' "$$owner" "$$source"; \
			exit 1; \
		fi; \
		total=$$((total + 1)); \
	done; \
	printf 'single-owner corpus scanned=%s exact_retained=0\n' "$$total"
	@set -e; total=0; \
	units=$$(awk -F '"' '/"id":/{id=$$4} /"source": "games\/gs1\/recon\/en\/units\//{print id}' \
		games/gs1/recon/translation-units.json); \
	for unit in $$units; do \
		report=$$(mktemp /tmp/alchemy-tu-corpus.XXXXXX); \
		$(COMPILER) candidate-show --unit $$unit | \
			awk -F= '/^owner=/{owner=$$2} /^candidate=/{split($$0,a,"differing_halfwords="); print owner "\t" a[2]+0}' \
			> "$$report"; \
		for owner in $$(awk -F '"' -v unit="$$unit" '/"id":/{id=$$4} id==unit && /"state":"retained-assembly"/{print $$4}' \
			games/gs1/recon/translation-units.json); do \
			diff=$$(awk -F '\t' -v owner="$$owner" '$$1==owner{print $$2}' "$$report"); \
			if test -z "$$diff" || test "$$diff" -eq 0; then \
				printf 'translation-unit retained owner is exact or unscored: %s %s\n' "$$unit" "$$owner"; \
				rm -f "$$report"; exit 1; \
			fi; \
			total=$$((total + 1)); \
		done; \
		rm -f "$$report"; \
	done; \
	printf 'translation-unit corpus retained=%s exact_retained=0\n' "$$total"
	$(OVERLAY) audit --corpus
	@printf 'candidate corpus ok: exact C is installed; retained C is nonexact; nonowners are classified\n'

edition-builds: build-claimed
	$(COMPILER) cross-edition --all --object-dir out/gs1-en/claimed/obj \
		--write out/exact-correspondence.edition-builds.json \
		--edition-build games/gs1/recon/exact-main-builds.json
	cmp games/gs1/recon/exact-correspondence.json out/exact-correspondence.edition-builds.json

edition-builds-check: build-claimed
	$(COMPILER) cross-edition --all --object-dir out/gs1-en/claimed/obj \
		--write out/exact-correspondence.check.json \
		--edition-build out/exact-main-builds.check.json
	cmp games/gs1/recon/exact-correspondence.json out/exact-correspondence.check.json
	cmp games/gs1/recon/exact-main-builds.json out/exact-main-builds.check.json

coverage: correspondence
	$(CHECK) coverage --write

coverage-check: correspondence-check
	$(CHECK) coverage --check

core-retained-check:
	$(CHECK) retained --check

source-tracking-check:
	@set -e; paths=$$(mktemp /tmp/alchemy-game-inputs.XXXXXX); \
	ignored=$$(mktemp /tmp/alchemy-ignored-inputs.XXXXXX); \
	trap 'rm -f "$$paths" "$$ignored"' EXIT; \
	find games -type f -print0 > "$$paths"; \
	test -s "$$paths"; \
	xargs -0 git ls-files --error-unmatch -- \
		< "$$paths" > /dev/null; \
	status=0; git check-ignore --no-index -z --stdin \
		< "$$paths" > "$$ignored" || status=$$?; \
	if [ "$$status" -eq 0 ]; then \
		printf 'tracked game input is ignored:\n'; \
		tr '\0' '\n' < "$$ignored"; \
		exit 1; \
	fi; \
	if [ "$$status" -ne 1 ]; then exit "$$status"; fi; \
	printf 'game input tracking ok\n'

check-owners: source-tracking-check
	$(CHECK) owners

corpus-check:
	@test -f games/gs1/project.json
	@test -f games/gs2/project.json
	@test -f games/alchemy/project.json
	@if test -d draft; then \
		printf 'legacy draft/ directory found; use games/gs1/recon/<edition>/\n'; \
		exit 1; \
	fi
	@if find games/gs1/semantic -maxdepth 1 -name '*.c' -print | grep -q .; then \
		printf 'source hypotheses belong in games/gs1/recon/, not games/gs1/semantic/ metadata\n'; \
		exit 1; \
	fi
	@printf 'corpus ok: two shared-source games, 12 edition targets, Alchemy integration separate\n'

build-tools:
	@set -e; for host in $(HOSTS); do \
		printf '  build %-20s' "$$host"; \
		$(CARGO) build --offline --quiet --release \
			--manifest-path $(TOOLS)/$$host/Cargo.toml; \
		printf ' ok\n'; \
	done

tool-tests:
	@set -e; for crate in $(HOSTS) $(CORE_TESTS); do \
		printf '  test  %-20s' "$$crate"; \
		$(CARGO) test --offline --quiet --release \
			--manifest-path $(TOOLS)/$$crate/Cargo.toml; \
		printf ' ok\n'; \
	done

tooling-size:
	@lines=$$(find $(addprefix $(TOOLS)/,$(PORTABLE_TOOLS)) -type f \
		\( -name '*.rs' -o -name '*.js' -o -name '*.css' \) \
		-not -path '*/target/*' -print0 | xargs -0 cat | wc -l | tr -d ' '); \
	if [ "$$lines" -gt $(TOOLING_LINE_LIMIT) ]; then \
		printf 'portable tooling is %s lines; limit is %s\n' "$$lines" '$(TOOLING_LINE_LIMIT)'; \
		exit 1; \
	fi; \
	printf 'portable tooling ok: %s / %s lines\n' "$$lines" '$(TOOLING_LINE_LIMIT)'

lint: standard-check pristine-options-check
	@set -e; git ls-files --cached --others --exclude-standard '*.rs' | while IFS= read -r source; do \
		test ! -f "$$source" || rustfmt --edition 2021 --check "$$source"; \
	done
	$(CHECK) no-asm

test: lint tooling-size tool-tests
	$(CHECK) publication --self-test

verify: source-tracking-check corpus-check test tooling-size targets \
	full-rom-check strict-tu-check classification-check \
	candidate-corpus-check edition-builds-check check-owners \
	progress-check coverage-check

standard-check:
	@printf '%s\n' $(GCC296_CFLAGS) | grep -v '^-I' | sort > /tmp/alchemy-standard-makefile.txt
	@$(CHECK) routes --standard | grep -v '^-I' | sort > /tmp/alchemy-standard-routing.txt
	@diff -u /tmp/alchemy-standard-makefile.txt /tmp/alchemy-standard-routing.txt
	@printf 'compiler standard ok\n'

COMPILER_UPSTREAM := 4087bd2bd7c064da935c2a1bf798b814a125eded
OPTION_FILES := gs1cc/gcc/toplev.c gs1cc/gcc/flags.h \
	gs1cc/gcc/config/arm/arm.h gs2cc/gcc/toplev.c gs2cc/gcc/flags.h \
	gs2cc/gcc/config/arm/arm.h agbcc/gcc_arm/toplev.c \
	agbcc/gcc_arm/flags.h agbcc/gcc_arm/config/arm/arm.h

pristine-options-check:
	@cd alchemy-gcc && set -e; for file in $(OPTION_FILES); do \
		test -f "$$file" || { printf 'missing compiler option source: %s\n' "$$file"; exit 1; }; \
		git show $(COMPILER_UPSTREAM):$$file > /tmp/compiler-options-upstream.c; \
		grep -oE '"(f|m)[a-z0-9-]+"|ARM_FLAG_[A-Z0-9_]+|flag_[a-z0-9_]+' \
		  /tmp/compiler-options-upstream.c | sort -u > /tmp/compiler-options-stock.txt; \
		grep -oE '"(f|m)[a-z0-9-]+"|ARM_FLAG_[A-Z0-9_]+|flag_[a-z0-9_]+' $$file \
		  | sort -u > /tmp/compiler-options-current.txt; \
		if comm -13 /tmp/compiler-options-stock.txt /tmp/compiler-options-current.txt | grep -q .; then \
			printf 'invented compiler option in %s\n' "$$file"; exit 1; \
		fi; \
	done
	@printf 'compiler options pristine\n'

CLEAN_TREES := out work build builds dist cmatch comparisons compiler-output \
	diffs disassembly dumps m2c objdump reports analysis .cache target

clean-preview:
	@printf 'would remove generated trees (never roms/):\n'; \
	for tree in $(CLEAN_TREES); do \
		if [ -e "$$tree" ]; then printf '  %-22s %s\n' "$$tree" "$$(du -sh "$$tree" | cut -f1)"; fi; \
	done

clean:
	@for tree in $(CLEAN_TREES); do \
		if [ -e "$$tree" ]; then rm -rf -- "$$tree"; fi; \
	done
	@find $(TOOLS) -type d -name target -prune -exec rm -rf -- {} +
	@printf 'generated trees removed; roms/ untouched\n'
