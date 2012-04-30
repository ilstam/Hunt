datadir = /usr/local/share
LANG_FILE = $(shell ls -d *.po | sed 's/\.po//g')

ifeq ($(ECHO),)
ECHO := $(shell whereis -b echo|sed 's/^echo: *\([^ ]\+\).*/\1/')
ifeq ($(ECHO),)
ECHO = echo
endif
endif


update-po: pot
	@set -x ; for d in $(LANG_FILE); do \
	$(ECHO) -e "\x1b[1;32m** updating $$d.po\x1b[0m"; \
	msgmerge -v $$d.po hunt.pot > $$d.new.po ; \
	mv $$d.new.po $$d.po ; \
	done

pot:
	xgettext -d -s -o hunt.pot --keyword=_ ../src/*.c ../src/*.h

gmo:
	@set -x ; for d in $(LANG_FILE); do \
	$(ECHO) -e "\x1b[1;32m** generating $$d.gmo\x1b[0m"; \
	rm -f $$d.gmo ; \
	msgfmt -c -o $$d.gmo $$d.po ; \
	done

install: gmo
	@set -x ; for d in $(LANG_FILE); do \
	$(ECHO) -e "\x1b[1;32m** installing $$d.mo\x1b[0m"; \
	install -d $(datadir)/locale/$$d/LC_MESSAGES ; \
	install -m 644 $$d.gmo $(datadir)/locale/$$d/LC_MESSAGES/hunt.mo ; \
	done

clean:
	rm -f ./hunt.pot *.gmo *.new.po
