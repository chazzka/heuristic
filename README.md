# heuristic

HOW TO RUN:
Main file is HeuristicCSVGenerator.cpp
- include your algorithm at the top of the file following these rules:
1. run function is ```vector<result> run(dimension, testFunctionNumber, boundaryLow, boundaryUp)```
2. result is a struct containing: int fez, double cost.

all the files ale included in HeuristicCSVGenerator.cpp before compilation. Sufficient command should be:

`g++ -o heuristic HeuristicCSVGenerator.cpp\`

or 

```clang++ -O2 HeuristicCSVGenerator.cpp -o heuristic\```





fez - current function evaluation count\
cost - function evaluation value at current fez number

# for docker

docker build -t heuristic Dockerfile_location

docker run -v PATH_IN_HOST:/usr/src/results -it heuristic

example:
docker run -v /home/debian/source/output:/usr/src/results -it heuristic
