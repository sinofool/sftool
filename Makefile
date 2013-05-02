BOOST_HOME =	$(HOME)/code/boost-dist
LIBCURL_HOME =	$(HOME)/code/libcurl-dist

CXXFLAGS =	-I$(BOOST_HOME)/include -I$(LIBCURL_HOME)/include -O2 -g -Wall -fmessage-length=0

OBJS =		sftool.o httputil.o

LIBS =	-L$(BOOST_HOME)/lib -lboost_thread -lboost_system -lpthread -L$(LIBCURL_HOME)/lib -lcurl -lssl -lcrypto -lldap -lz

TARGET =	sftool

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
