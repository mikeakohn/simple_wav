
default:
	@+make -C build

%.o: %.cxx %.h float.h
	$(CPP) -c $*.cxx $(CFLAGS)

clean:
	@rm -f a440 copy_wav create_speech build/*.o
	@echo "Clean!"

