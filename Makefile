CXX:=zig build

TARGETOS:=

OPTIMIZATIONS:=-Doptimize=Debug

RUN:=run

OUTPUT:=--prefix build --prefix-exe-dir ""

COMPILE_COMMANDS:=cdb

.PHONY: build run debug debug-run clean

build:
	$(eval OPTIMIZATIONS=-Doptimize=ReleaseFast)
	$(CXX) $(TARGETOS) $(OPTIMIZATIONS) $(COMPILE_COMMANDS) $(OUTPUT)

run:
	$(eval OPTIMIZATIONS=-Doptimize=ReleaseFast)
	$(CXX) $(RUN) $(TARGETOS) $(OPTIMIZATIONS) $(COMPILE_COMMANDS) $(OUTPUT)

debug:
	$(CXX) $(TARGETOS) $(OPTIMIZATIONS) $(COMPILE_COMMANDS) $(OUTPUT)

debug-run:
	$(CXX) $(RUN) $(TARGETOS) $(OPTIMIZATIONS) $(COMPILE_COMMANDS) $(OUTPUT)

clean:
	rm -rf build/ .zig-cache
