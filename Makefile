
default:
	@+make -C build

%.o: %.cxx %.h float.h
	$(CPP) -c $*.cxx $(CFLAGS)

clean:
	@rm -f copy_wav create_speech build/*.o
	@echo "Clean!"

