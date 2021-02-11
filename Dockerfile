# Get the GCC preinstalled image from Docker Hub
FROM gcc:latest

# Copy the current folder which contains C++ source code to the Docker image under /usr/src
COPY . /usr/src/heuristic

# Specify the working directory
WORKDIR /usr/src/heuristic

# Use GCC to compile the Test.cpp source file
RUN g++ -o Test HeuristicCSVGenerator.cpp

# Run the program output from the previous step
CMD ["./Test"]
