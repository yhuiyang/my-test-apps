ifneq ($(LANG),de)
	LANG=en
endif

BASE=wxterm_$(LANG).hhc \
	wxterm_$(LANG).hhk \
	wxterm_$(LANG).hhp \
	intro_$(LANG).html \
	operation_$(LANG).html \
	faq_$(LANG).html \
	images/break.gif \
	images/configure.gif \
	images/connect.gif \
	images/disconnect.gif \
	images/filenew.gif \
	images/fileopen.gif \
	images/filesaveas.gif \
     images/quit.gif \
	images/refresh.gif \
	images/view_ascii.gif \
	images/view_hex.gif

DES=../wxterm_$(LANG).zip

ID=`echo $$PPID`

$(DES): $(BASE)
	@if [ -s $(DES) ]; then cp $(DES) help.zip.$(ID); fi
	zip $(DES) $(BASE)
