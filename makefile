INCLUDEOBJS = include/date.o include/common.o include/valid.o include/httpreq.o include/jsonhttpreq.o
WIDGETSOBJS = widgets/Widget.o widgets/StockWidget.o widgets/MovieWidget.o widgets/WeatherWidget.o widgets/NewsWidget.o widgets/SportsWidget.o widgets/QuoteOfTheDayWidget.o Mirror.o
OBJS = $(INCLUDEOBJS) $(WIDGETSOBJS) main.o

CC = g++
CFLAGS = -Wall -std=c++11 -c
LFLAGS = -Wall -std=c++11
IDIR = -I./
LD = -lcurl



main : $(OBJS)
	$(CC) $(LFLAGS) $(IDIR) $(OBJS) $(LD) -o mirror.out

main.o : $(INCLUDEOBJS) $(WIDGETSOBJS)
	$(CC) $(CFLAGS) $(IDIR) main.cpp

Mirror.o : widgets/Widget.o
	$(CC) $(CFLAGS) $(IDIR) Mirror.cpp -o Mirror.o

widgets/QuoteOfTheDayWidget.o : include/httpreq.o include/jsonhttpreq.o include/common.o include/valid.o include/date.o
	$(CC) $(CFLAGS) $(IDIR) widgets/QuoteOfTheDayWidget.cpp -o widgets/QuoteOfTheDayWidget.o

widgets/WeatherWidget.o : include/httpreq.o include/jsonhttpreq.o
	$(CC) $(CFLAGS) $(IDIR) widgets/WeatherWidget.cpp -o widgets/WeatherWidget.o

widgets/StockWidget.o : include/httpreq.o include/jsonhttpreq.o include/date.o
	$(CC) $(CFLAGS) $(IDIR) widgets/StockWidget.cpp -o widgets/StockWidget.o

widgets/NewsWidget.o : include/httpreq.o include/jsonhttpreq.o include/common.o include/valid.o
	$(CC) $(CFLAGS) $(IDIR) widgets/NewsWidget.cpp -o widgets/NewsWidget.o

widgets/SportsWidget.o : include/httpreq.o include/jsonhttpreq.o include/common.o include/valid.o include/date.o
	$(CC) $(CFLAGS) $(IDIR) widgets/SportsWidget.cpp -o widgets/SportsWidget.o

widgets/MovieWidget.o : include/httpreq.o include/jsonhttpreq.o include/common.o include/valid.o include/date.o
	$(CC) $(CFLAGS) $(IDIR) widgets/MovieWidget.cpp -o widgets/MovieWidget.o

widgets/Widget.o : include/httpreq.o include/jsonhttpreq.o include/common.o include/valid.o include/date.o
	$(CC) $(CFLAGS) $(IDIR) widgets/Widget.cpp -o widgets/Widget.o



include/jsonhttpreq.o : include/httpreq.o
	$(CC) $(CFLAGS) $(IDIR) include/jsonhttpreq.cpp -o include/jsonhttpreq.o

include/httpreq.o :
	$(CC) $(CFLAGS) $(IDIR) include/httpreq.cpp -o include/httpreq.o




include/valid.o :
	$(CC) $(CFLAGS) $(IDIR) include/valid.cpp -o include/valid.o

include/date.o :
	$(CC) $(CFLAGS) $(IDIR) include/date.cpp -o include/date.o

include/common.o :
	$(CC) $(CFLAGS) $(IDIR) include/common.cpp -o include/common.o



clean :
	\rm -fvr *.o *.out *~ include/*.o include/*.out include/*~ widgets/*.o widgets/*.out widgets/*~ mirror_config.json *_mirror.html

reset :
	\rm -fvr mirror_config.json *_mirror.html

clear :
	\rm -fvr mirror_config.json *_mirror.html
