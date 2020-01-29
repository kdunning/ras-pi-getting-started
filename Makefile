# Blog code
CC=gcc
BLOG_SOURCES=led_select.c
C_FLAGS=-std=c99 $(CPP_FLAGS)
C_LINK_FLAGS=-lwiringPi
BLOG_TARGET=led_select

# Additional demo code
CPPC=g++
DEMO_SOURCES=led_demo.cpp
CPP_FLAGS=-Wall -Wshadow -Wpedantic
CPP_LINK_FLAGS=-lpthread -lwiringPi
DEMO_TARGET=led_demo

# Build all applications
all: $(DEMO_TARGET) $(BLOG_TARGET)

# Build blog application
$(BLOG_TARGET):
	@echo "Building" $@
	@$(CC) $(BLOG_SOURCES) $(C_FLAGS) $(C_LINK_FLAGS) -o $@
	@echo "Successfully built" $(BLOG_TARGET)

# Build demo application
$(DEMO_TARGET):
	@echo "Building" $@
	@$(CPPC) $(DEMO_SOURCES) $(CPP_FLAGS) $(CPP_LINK_FLAGS) -o $@
	@echo "Successfully built" $(DEMO_TARGET)

# Clean all applications
clean:
	@rm -f $(DEMO_TARGET) $(BLOG_TARGET)
	@echo "Clean complete."