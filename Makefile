MAKEFLAGS+=-s

all: debug
ci: lint debug release test

help:
	@echo "      release: Run a release build"
	@echo "        debug: Run a debug build"
	@echo "         lint: Lint check all source-code"
	@echo "         test: Build and run tests"
	@echo "        clean: Clean all build output"
	@echo "rebuild_cache: Rebuild all CMake caches"

rebuild_cache: build/Debug build/Release
	@$(MAKE) -C build/Debug rebuild_cache
	@$(MAKE) -C build/Release rebuild_cache

debug: build/Debug
	@echo "[DEBUG]"
	@$(MAKE) -C build/Debug

release: build/Release
	@echo "[RELEASE]"
	@$(MAKE) -C build/Release

install: release
	@echo "[PACKAGE] Release"
	@$(MAKE) -C build/Release install DESTDIR=_install

lint: build/Debug
	@echo "[LINT]"
	@$(MAKE) -C build/Debug lint

analyze:
	@scan-build -o build/Analyze --status-bugs make debug

test: build/Debug
	@$(MAKE) -C build/Debug test

build/Debug:
	@mkdir -p $@
	@cd $@ && cmake -DCMAKE_BUILD_TYPE=Debug ../../

build/Release:
	@mkdir -p $@
	@cd $@ && cmake -Wno-dev -DCMAKE_BUILD_TYPE=Release ../../

clean:
	@echo "[CLEAN]"
	@rm -Rf build

.PHONY: clean
