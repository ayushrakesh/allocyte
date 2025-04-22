# Compiler settings
CC = g++
CFLAGS = -g
OBJECTS = testapp.o mm.o gluethread/glthread.o

# Target executable
TARGET = exe

# Default target
all: $(TARGET)

# Link object files
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

# Compile source files
testapp.o: testapp.cpp
	$(CC) $(CFLAGS) -c testapp.cpp -o $@

mm.o: mm.cpp
	$(CC) $(CFLAGS) -c mm.cpp -o $@

gluethread/glthread.o: gluethread/glthread.cpp
	$(CC) $(CFLAGS) -c gluethread/glthread.cpp -o $@

# Clean up
clean:
	rm -f $(OBJECTS) $(TARGET)

# Docker settings
DOCKER_IMAGE = ayushrakesh/allocyte
DOCKERFILE = Dockerfile

# Build Docker image
docker-build:
	docker build -t $(DOCKER_IMAGE) -f $(DOCKERFILE) .

# Run Docker container
docker-run:
	docker run -it $(DOCKER_IMAGE)

# Push Docker image to Docker Hub
docker-push: docker-build
	docker push $(DOCKER_IMAGE)

# Default Docker workflow
docker: docker-build docker-run

.PHONY: all clean docker-build docker-run docker-push docker