APPS = crash early_exit indexer jobcount mtiming nocrash rtiming wc

.PHONY: all clean

all: $(addprefix mrapps/, $(addsuffix .so,$(APPS)))

mrapps/%.so: mrapps/%/*.go
	@echo "Building $@"
	go build -buildmode=plugin -gcflags="all=-N -l" -o $@ $<
	
clean:
	@rm -f mrapps/*.so