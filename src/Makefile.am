## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	-DG_APPLICATION_ID='"org.chris_sterne.world_in_the_wine_cellar"' \
	-DG_RESOURCE_PREFIX='"/org/chris_sterne/world-in-the-wine-cellar/"' \
	##-I $(top_srcdir)/include \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(WORLD_IN_THE_WINE_CELLAR_CFLAGS)

AM_CFLAGS = -Wall

bin_PROGRAMS = world-in-the-wine-cellar

world_in_the_wine_cellar_LDFLAGS = -pthread

world_in_the_wine_cellar_LDADD = $(WORLD_IN_THE_WINE_CELLAR_LIBS)

world_in_the_wine_cellar_SOURCES = \
	main.cc \
	Application.cc \
	MainWindow.cc \
	CommandEntry.cc \
	MessageBar.cc \
	ControllerView.cc \
	ControlView.cc \
	DescriptionView.cc \
	PlayerView.cc \
	RoomView.cc \
	TeleporterView.cc \
	ItemView.cc \
	LevelView.cc \
	HelpView.cc \
	World.cc \
	Tiles.cc \
	Controller.cc \
	ObjectList.cc \
	Object.cc

	
