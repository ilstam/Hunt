all:
	$(MAKE) -C src

install:
	$(MAKE) -C po install

clean:
	$(MAKE) -C src clean
	$(MAKE) -C po clean
