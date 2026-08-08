TOOLS := tools
CARGO ?= cargo
CARGO_RUN := $(CARGO) run --quiet --release --manifest-path
DISPATCH_MANIFEST := $(TOOLS)/dispatch/Cargo.toml
DISPATCH_RUN := $(CARGO_RUN) $(DISPATCH_MANIFEST) --
DISPATCH_GROUPS := assets check compiler decomp make metrics overlay search semantic

.PHONY: help verify test lint build-claimed build-asm build-assets build-semantic \
	build-full build-rom inventory semantic-check core-retained-check sanctum \
	progress progress-check progress-subject progress-history coverage coverage-check \
	showcase compiler-checks compiler-sweep compiler-cohort overlay-compiler-cohort \
	compiler-corpus compiler-batch overlay-candidate-check statement-order-check \
	compiler-lint compiler-self-test build-dispatch $(addprefix dispatch-,$(DISPATCH_GROUPS))

help:
	@printf '%s\n' \
		'make verify          authoritative byte-exact gate' \
		'make test            lint and every native self-test' \
		'make lint            repository architecture and policy gates' \
		'make build-asm       rebuild the assembled stage' \
		'make build-rom       rebuild the ROM' \
		'make build-assets    rebuild the asset tree' \
		'make progress        print byte-exact progress' \
		'make progress-subject print the commit progress subject' \
		'make progress-history refresh the progress history ledger' \
		'make coverage        refresh the dashboard maps' \
		'make showcase        run the overlay showcase' \
		'make compiler-checks run focused native compiler checks' \
		'make dispatch-GROUP ARGS=... run a registered native dispatch group'

build-dispatch:
	$(CARGO) build --quiet --release --manifest-path $(DISPATCH_MANIFEST)

$(addprefix dispatch-,$(DISPATCH_GROUPS)): dispatch-%:
	$(DISPATCH_RUN) $* $(ARGS)

build-claimed:
	$(CARGO_RUN) $(TOOLS)/build-claimed/Cargo.toml --

build-asm:
	$(CARGO_RUN) $(TOOLS)/build-asm/Cargo.toml --

build-assets:
	$(CARGO_RUN) $(TOOLS)/build-assets/Cargo.toml --

build-semantic:
	$(CARGO_RUN) $(TOOLS)/build-semantic/Cargo.toml --

build-full:
	$(CARGO_RUN) $(TOOLS)/build-full/Cargo.toml --

build-rom:
	$(CARGO_RUN) $(TOOLS)/build-rom/Cargo.toml --

inventory:
	$(CARGO_RUN) $(TOOLS)/overlay-inventory/Cargo.toml --

semantic-check:
	$(CARGO_RUN) $(TOOLS)/semantic-owner-scope/Cargo.toml -- --check

core-retained-check:
	$(CARGO_RUN) $(TOOLS)/core-retained-audit/Cargo.toml -- --check

sanctum:
	$(CARGO_RUN) $(TOOLS)/check-sanctum/Cargo.toml --

progress:
	$(CARGO_RUN) $(TOOLS)/full-c-progress/Cargo.toml --

progress-check:
	$(CARGO_RUN) $(TOOLS)/full-c-progress/Cargo.toml -- --check

progress-subject:
	$(CARGO_RUN) $(TOOLS)/full-c-progress/Cargo.toml -- --subject

progress-history:
	$(CARGO_RUN) $(TOOLS)/full-c-history/Cargo.toml -- --write

coverage:
	$(CARGO_RUN) $(TOOLS)/coverage-map/Cargo.toml -- --write

coverage-check:
	$(CARGO_RUN) $(TOOLS)/coverage-map/Cargo.toml -- --check

showcase:
	$(CARGO_RUN) $(TOOLS)/overlay-showcase/Cargo.toml -- --all

compiler-checks: compiler-sweep compiler-cohort overlay-compiler-cohort compiler-corpus compiler-batch

compiler-sweep:
	$(CARGO_RUN) $(TOOLS)/mode-sweep/Cargo.toml -- --self-test

compiler-cohort:
	$(CARGO_RUN) $(TOOLS)/mode-cohort/Cargo.toml -- --self-test

overlay-compiler-cohort:
	$(CARGO_RUN) $(TOOLS)/overlay-mode-cohort/Cargo.toml -- --self-test

compiler-corpus:
	$(CARGO_RUN) $(TOOLS)/compiler-corpus-regression/Cargo.toml --bin compiler-corpus-regression -- --self-test

compiler-batch:
	$(CARGO_RUN) $(TOOLS)/search-compiler-modes/Cargo.toml -- --self-test

overlay-candidate-check:
	$(CARGO_RUN) $(TOOLS)/overlay-candidate-rank/Cargo.toml -- --self-test

statement-order-check:
	$(CARGO_RUN) $(TOOLS)/statement-order-sweep-main/Cargo.toml -- --self-test

compiler-lint:
	$(CARGO_RUN) $(TOOLS)/alchemy-lints/Cargo.toml --

compiler-self-test:
	$(CARGO_RUN) $(TOOLS)/alchemy-selftest/Cargo.toml --

lint:
	$(CARGO_RUN) $(TOOLS)/architecture/Cargo.toml --
	$(CARGO_RUN) $(TOOLS)/source-citations/Cargo.toml --
	$(CARGO_RUN) $(TOOLS)/no-asm-c/Cargo.toml --
	$(CARGO_RUN) $(TOOLS)/decomp-targets/Cargo.toml -- --self-test
	$(CARGO_RUN) $(TOOLS)/cache-key-lint/Cargo.toml --
	$(CARGO) run --quiet --release --manifest-path $(TOOLS)/Cargo.toml --bin lang-ban --
	$(MAKE) compiler-lint

test: lint compiler-self-test
	$(CARGO_RUN) $(TOOLS)/self-test/Cargo.toml --

verify:
	$(MAKE) inventory
	$(CARGO_RUN) $(TOOLS)/statement-order-sweep-main/Cargo.toml -- --self-test
	$(MAKE) test
	$(CARGO_RUN) $(TOOLS)/overlay-candidate-rank/Cargo.toml -- --self-test
	$(MAKE) build-claimed
	$(MAKE) build-semantic
	$(MAKE) semantic-check
	$(CARGO_RUN) $(TOOLS)/build-full/Cargo.toml -- --source-only
	$(MAKE) build-full
	$(MAKE) core-retained-check
	$(CARGO_RUN) $(TOOLS)/semantic-superseded/Cargo.toml -- --check
	$(MAKE) sanctum
	$(MAKE) progress-check
