# Alchemy's build and contributor workflows share the alchemy executable.
# Asset packages are described by data under games/; the shared codecs behind
# `alchemy build assets` are named for their formats, not for any resource.

GCC296_CFLAGS := -O2 -mthumb -mthumb-interwork -mcpu=arm7tdmi \
                 -fno-builtin -nostdinc -ffreestanding \
                 -fcall-used-r4 -Igames/gs1/include

TOOLS := tools
CARGO ?= cargo
export CARGO_TARGET_DIR := $(CURDIR)/out/cargo-target
CARGO_RUN := $(CARGO) run --offline --quiet --release --manifest-path

BUILD := $(CARGO_RUN) $(TOOLS)/alchemy/Cargo.toml -- build
ASSETS := $(CARGO_RUN) $(TOOLS)/alchemy/Cargo.toml -- build assets
CHECK := $(CARGO_RUN) $(TOOLS)/alchemy/Cargo.toml -- check
COMPILER := $(CARGO_RUN) $(TOOLS)/alchemy/Cargo.toml --
OVERLAY := $(CARGO_RUN) $(TOOLS)/alchemy/Cargo.toml -- overlay

HOSTS := alchemy psynergy
PORTABLE_TOOLS := alchemy psynergy
# The maintainer-owned ceiling covers the portable Rust, TypeScript,
# JavaScript, and CSS beside the decompilation. Contributors pare
# machinery; they do not raise it.
# Only Pascal moves this number; a diff touching it without his recorded
# decision is invalid regardless of how good the new machinery is.
# 50,000 set by Pascal's decision, 2026-08-30.
TOOLING_LINE_LIMIT := 50000
TARGET ?= gs1-en
TARGET_GAME := $(firstword $(subst -, ,$(TARGET)))
FULL_REPORT = out/$(TARGET)/full/rebuilt.json
FULL_ROM = out/$(TARGET)/full/rebuilt.gba
OWNER_INVENTORY = out/$(TARGET)/full/rebuilt.owner-inventory.json
REPORT_DIR = out/$(TARGET)/reports
VERIFIED_TREE = $(REPORT_DIR)/verified-tree
MAIN_CORRESPONDENCE_MATCHED_MIN := 1393
MAIN_CORRESPONDENCE_UNRESOLVED_MAX := 44
OVERLAY_CORRESPONDENCE_MATCHED_MIN := 2562
OVERLAY_CORRESPONDENCE_UNRESOLVED_MAX := 38
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

.PHONY: help verify audit reports test lint lint-production lint-all-targets build-tools tool-tests tooling-size tooling-index-check \
	build-claimed build-asm build-assets build-full build-rom \
	standard-check compiler-source-check corpus-check core-retained-check \
	full-rom-check overlay-check declared-tu-check owner-inventory-check strict-tu-check classification-check \
	candidate-corpus-check source-tracking-check index-sync-check check-owners progress progress-report progress-check progress-subject \
	correspondence correspondence-check edition-builds edition-builds-check \
	coverage coverage-check clean clean-preview
.PHONY: targets $(HISTORICAL_TARGETS)

help:
	@printf '%s\n' \
		'make verify           fast byte-exact production gate' \
		'make audit            exhaustive editions, candidates, and reports audit' \
		'make reports          refresh analysis reports and coverage figures' \
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
		'make source-tracking-check reject ignored or untracked Proven C' \
		'make build-assets     rebuild source assets' \
		'make test             focused Rust tests and policy checks' \
		'make tooling-size     enforce the portable-toolkit budget' \
		'make tooling-index-check prove every tool is indexed exactly once' \
		'make progress         print byte-exact progress' \
		'make progress-subject print the required commit prefix' \
		'make correspondence   match exact EN owners across GS1 editions' \
		'make edition-builds   relink exact EN C across GS1 editions' \
		'make coverage         refresh coverage data and figures'

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

.PHONY: dashboard music-debug dashboard-service-install
dashboard:
	$(COMPILER) dashboard --bind 127.0.0.1:4650

music-debug:
	$(COMPILER) music-debug --bind 127.0.0.1:4651

dashboard-service-install:
	@mkdir -p '$(HOME)/Library/LaunchAgents' '$(CURDIR)/out'
	@sed -e 's|@ALCHEMY_ROOT@|$(CURDIR)|g' \
		-e 's|@CARGO@|$(shell command -v $(CARGO))|g' \
		tools/alchemy/src/dashboard/com.pascalpixel.alchemy-dashboard.plist.in \
		> '$(HOME)/Library/LaunchAgents/com.pascalpixel.alchemy-dashboard.plist'
	@launchctl bootout 'gui/$(shell id -u)/com.pascalpixel.alchemy-dashboard' 2>/dev/null || true
	@launchctl bootstrap 'gui/$(shell id -u)' '$(HOME)/Library/LaunchAgents/com.pascalpixel.alchemy-dashboard.plist'

progress:
	$(CHECK) progress

progress-check: full-rom-check
	$(CHECK) progress --check

progress-report: full-rom-check | $(REPORT_DIR)
	$(CHECK) progress --write-report

progress-subject:
	$(CHECK) progress --subject

$(REPORT_DIR):
	@mkdir -p $@

correspondence: build-full | $(REPORT_DIR)
	$(COMPILER) cross-edition --all --object-dir out/gs1-en/full/claimed/obj \
		--write $(REPORT_DIR)/exact-correspondence.json \
		--edition-build $(REPORT_DIR)/exact-main-builds.json
	$(COMPILER) cross-edition --all-overlays \
		--write $(REPORT_DIR)/exact-overlay-correspondence.json \
		--edition-build $(REPORT_DIR)/exact-overlay-builds.json

correspondence-check: correspondence
	@set -e; \
	main='$(REPORT_DIR)/exact-correspondence.json'; \
	overlay='$(REPORT_DIR)/exact-overlay-correspondence.json'; \
	main_matched=$$(sed -n 's/.*"matched_owners": \([0-9][0-9]*\),/\1/p' "$$main" | head -n 1); \
	main_unresolved=$$(sed -n 's/.*"unresolved_owners": \([0-9][0-9]*\),/\1/p' "$$main" | head -n 1); \
	overlay_matched=$$(sed -n 's/.*"matched_owners": \([0-9][0-9]*\),/\1/p' "$$overlay" | head -n 1); \
	overlay_unresolved=$$(sed -n 's/.*"unresolved_owners": \([0-9][0-9]*\),/\1/p' "$$overlay" | head -n 1); \
	test -n "$$main_matched" -a -n "$$main_unresolved" \
		-a -n "$$overlay_matched" -a -n "$$overlay_unresolved"; \
	test "$$main_matched" -ge '$(MAIN_CORRESPONDENCE_MATCHED_MIN)' \
		-a "$$main_unresolved" -le '$(MAIN_CORRESPONDENCE_UNRESOLVED_MAX)'; \
	test "$$overlay_matched" -ge '$(OVERLAY_CORRESPONDENCE_MATCHED_MIN)' \
		-a "$$overlay_unresolved" -le '$(OVERLAY_CORRESPONDENCE_UNRESOLVED_MAX)'; \
	printf 'cross-edition correspondence ok: main=%s/%s overlay=%s/%s matched/unresolved\n' \
		"$$main_matched" "$$main_unresolved" "$$overlay_matched" "$$overlay_unresolved"

classification-check: core-retained-check
	@printf 'classification contracts ok\n'

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
		result=$$($(COMPILER) diff games/gs1/recon/en/units/$$source \
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
		$(COMPILER) diff --unit $$unit | \
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
	@printf 'candidate corpus ok: Proven C is installed; retained C is nonexact; nonowners are classified\n'

edition-builds: correspondence
	@printf 'edition build report: %s\n' '$(REPORT_DIR)/exact-main-builds.json'

edition-builds-check: correspondence-check
	@printf 'cross-edition edition-build audit ok\n'

coverage: full-rom-check | $(REPORT_DIR)
	$(CHECK) coverage --write

coverage-check: full-rom-check
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

index-sync-check:
	@git diff --quiet --ignore-submodules -- || { \
		printf 'unstaged tracked changes found; stage the tree before make verify\n'; exit 1; \
	}
	@set -e; untracked=$$(git ls-files --others --exclude-standard); \
	test -z "$$untracked" || { \
		printf 'untracked files found; stage or ignore them before make verify:\n%s\n' "$$untracked"; exit 1; \
	}
	@printf 'index and worktree agree\n'

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
	$(CARGO) test --offline --quiet --release --workspace \
		--manifest-path $(TOOLS)/Cargo.toml
	$(COMPILER) match --acceptance-test

tooling-size:
	@for path in $(addprefix $(TOOLS)/,$(PORTABLE_TOOLS)); do \
		test -d "$$path" || { printf 'missing counted tooling directory: %s\n' "$$path"; exit 1; }; \
	done
	@lines=$$(find $(addprefix $(TOOLS)/,$(PORTABLE_TOOLS)) -type f \
		\( -name '*.rs' -o -name '*.js' -o -name '*.ts' -o -name '*.css' \) \
		-not -path '*/target/*' -print0 | xargs -0 cat | wc -l | tr -d ' '); \
	if [ "$$lines" -gt $(TOOLING_LINE_LIMIT) ]; then \
		printf 'portable tooling is %s lines; limit is %s\n' "$$lines" '$(TOOLING_LINE_LIMIT)'; \
		exit 1; \
	fi; \
	printf 'portable tooling ok: %s / %s lines\n' "$$lines" '$(TOOLING_LINE_LIMIT)'

tooling-index-check:
	@$(CHECK) publication --documents
	@set -eu; \
	actual=$$(mktemp /tmp/alchemy-tool-index.actual.XXXXXX); \
	indexed=$$(mktemp /tmp/alchemy-tool-index.indexed.XXXXXX); \
	duplicates=$$(mktemp /tmp/alchemy-tool-index.duplicates.XXXXXX); \
	trap 'rm -f "$$actual" "$$indexed" "$$duplicates"' EXIT; \
	{ find tools -mindepth 2 -maxdepth 2 -name Cargo.toml -print | sed 's#tools/##;s#/Cargo.toml##'; \
	  find tools -mindepth 2 -maxdepth 2 -type f \( -name '*.ts' -o -name '*.js' -o -name '*.py' -o -name '*.sh' \) -print | sed 's#tools/##;s#/[^/]*$$##'; \
	} | sort -u > "$$actual"; \
	awk -F '[()]' '/^## Tooling index/ { inside=1; next } /^## Owners and names/ { inside=0 } inside && /^\| \[/ { sub(/^tools\//, "", $$2); sub(/\/$$/, "", $$2); print $$2 }' CONTRIBUTING.md | sort > "$$indexed"; \
	uniq -d "$$indexed" > "$$duplicates"; \
	test ! -s "$$duplicates" || { printf 'tooling index duplicates:\n'; cat "$$duplicates"; exit 1; }; \
	diff -u "$$actual" "$$indexed"; \
	printf 'tooling index ok: %s tools\n' "$$(wc -l < "$$actual" | tr -d ' ')"

# The registers are session state that a bare `git checkout -- <file>` can
# silently reset to the last commit, orphaning every source adopted since.
# `make verified-restore` puts games/<target> back to the last verified tree
# instead; `register-shrink-check` refuses a verify whose owner register has
# fewer entries than that tree unless RETIRE=1 says the shrink is deliberate.
verified-restore:
	@set -eu; tree=$$(cat $(VERIFIED_TREE)); \
	git cat-file -e "$$tree" || { printf 'no verified tree object: %s\n' "$$tree"; exit 1; }; \
	git checkout "$$tree" -- games/$(TARGET_GAME); \
	git diff --cached --name-status "$$tree" -- games/$(TARGET_GAME) | awk '$$1=="A"{print $$2}' | xargs -r git rm -q -f --cached; \
	git diff --name-status "$$tree" -- games/$(TARGET_GAME) | awk '$$1=="A"{print $$2}' | xargs -r rm -f; \
	printf 'games/%s restored to verified tree %s\n' '$(TARGET_GAME)' "$$tree"

register-shrink-check:
	@set -eu; test -s $(VERIFIED_TREE) || { printf 'register shrink check: no verified tree yet\n'; exit 0; }; \
	tree=$$(cat $(VERIFIED_TREE)); \
	git cat-file -e "$$tree:games/$(TARGET_GAME)/source-paths.json" 2>/dev/null || { printf 'register shrink check: previous tree lacks the register\n'; exit 0; }; \
	before=$$(git cat-file -p "$$tree:games/$(TARGET_GAME)/source-paths.json" | grep -c '^    "'); \
	after=$$(git show :games/$(TARGET_GAME)/source-paths.json | grep -c '^    "'); \
	if [ "$$after" -lt "$$before" ] && [ "$${RETIRE:-0}" != "1" ]; then \
		printf 'owner register shrank from %s to %s entries since verified tree %s; a retirement must say RETIRE=1, anything else is a wipe\n' "$$before" "$$after" "$$tree"; exit 1; fi; \
	printf 'register shrink check ok: %s -> %s owners\n' "$$before" "$$after"

# Tooling is Rust except the two browser clients and their regression tests.
# Asset and game source directories carry no scripts.
language-check:
	@set -eu; \
	scripts=$$(git ls-files --cached --others --exclude-standard | grep -E '\.(ts|js|mjs|cjs|py|sh)$$' | grep -Ev '^tools/alchemy/src/(dashboard|music_debug)/client(\.test)?\.js$$' || true); \
	if [ -n "$$scripts" ]; then printf 'TypeScript, JavaScript, Python, or shell implementation files are not allowed:\n%s\n' "$$scripts"; exit 1; fi; \
	printf 'language gate ok: Rust tooling and two browser clients\n'

lint: lint-all-targets

lint-production: standard-check compiler-source-check
	@set -e; git ls-files --cached --others --exclude-standard '*.rs' | while IFS= read -r source; do \
		test ! -f "$$source" || rustfmt --edition 2021 --check "$$source"; \
	done
	$(CHECK) no-asm --target $(TARGET)

lint-all-targets: standard-check compiler-source-check
	@set -e; git ls-files --cached --others --exclude-standard '*.rs' | while IFS= read -r source; do \
		test ! -f "$$source" || rustfmt --edition 2021 --check "$$source"; \
	done
	$(CHECK) no-asm

test: lint tooling-size tooling-index-check tool-tests compiler-source-check
	$(CHECK) publication --self-test
	$(CHECK) commit-progress --self-test
	$(CHECK) progress --self-test
	$(CHECK) no-asm --self-test

verify: index-sync-check source-tracking-check corpus-check language-check register-shrink-check lint-production tooling-size tooling-index-check \
	strict-tu-check check-owners core-retained-check coverage-check | $(REPORT_DIR)
	@tree=$$(git write-tree) || exit; \
	printf '%s\n' "$$tree" > $(VERIFIED_TREE).tmp; \
	mv $(VERIFIED_TREE).tmp $(VERIFIED_TREE); \
	printf 'verified staged tree: %s\n' "$$tree"

audit: verify test targets classification-check candidate-corpus-check \
	correspondence-check progress-report coverage-check

reports: correspondence progress-report coverage

standard-check:
	@set -e; actual=$$($(CHECK) routes --standard | grep -v '^-I' | sort); \
	expected=$$(printf '%s\n' $(GCC296_CFLAGS) | grep -v '^-I' | sort); \
	test "$$actual" = "$$expected" || { printf 'compiler flags differ\nexpected:\n%s\nactual:\n%s\n' "$$expected" "$$actual"; exit 1; }
	@printf 'compiler standard ok\n'

.PHONY: compilers

compilers:
	sh agscc/build.sh
	$(MAKE) -C agbcc/gcc old -j1

compiler-source-check:
	@set -e; for repo in agbcc agscc; do \
		case "$$repo" in \
		  agbcc) approved=da598c1d918402c42c0c0d7128ba14567f3175e9;; \
		  agscc) approved=5ec3e2edf9b4d0eaa55140ccbc3ba74d8a352148;; \
		esac; \
		test "$$(git rev-parse :$$repo)" = "$$approved" || { printf '%s gitlink is not approved\n' "$$repo"; exit 1; }; \
		test "$$(git -C "$$repo" rev-parse HEAD)" = "$$approved" || { printf '%s checkout is not approved\n' "$$repo"; exit 1; }; \
		state=$$(git -C "$$repo" status --porcelain --untracked-files=all); \
		test -z "$$state" || { printf '%s compiler source is dirty\n' "$$repo"; exit 1; }; \
	done
	@printf 'compiler sources match approved submodules\n'

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
