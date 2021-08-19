.PHONY: install-headers install-libk install-kernel

install-headers:
	cd libk && $(MAKE) install-headers
	cd kernel && $(MAKE) install-headers

install-libk:
	cd libk && $(MAKE) install

install-kernel: install-libk
	cd kernel && $(MAKE) install

install: install-headers install-kernel

