READER_SRC := addPathComponent.cc fixTypeName.cc readAlien.cc reader.cc \
	readLinkStore.cc readNewLinkStore.cc readNilStore.cc readPath.cc \
	readStore.cc readStoreOrElemStore.cc readVersion.cc util.cc
SRCS += $(patsubst %,reader/%,$(READER_SRC))
