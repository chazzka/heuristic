# heuristic

docker build -t heuristic Dockerfile_location

docker run -v PATH_IN_HOST:/usr/src/results -it heuristic

example:
docker run -v /home/debian/source/output:/usr/src/results -it heuristic
