BOOST_HOME =	$(HOME)/code/boost-dist
LIBCURL_HOME =	$(HOME)/code/libcurl-dist

CXXFLAGS =	-I$(BOOST_HOME)/include -I$(LIBCURL_HOME)/include -O2 -g -Wall -fmessage-length=0

OBJS =		 httputil.o

LIBS =	-L$(BOOST_HOME)/lib -lboost_thread -lboost_system -lpthread -L$(LIBCURL_HOME)/lib -lcurl -lssl -lcrypto -lldap -lz

TARGET =	sftool
ARCHIVE =	libsftool.a

all:	$(TARGET) $(ARCHIVE)

$(TARGET):	sftool.o $(OBJS)
	$(CXX) -o $(TARGET) sftool.o $(OBJS) $(LIBS)

$(ARCHIVE):	$(OBJS)
	$(AR) -cr $(ARCHIVE) $(OBJS)

clean:
	rm -f libsftool.o $(OBJS) $(TARGET)
