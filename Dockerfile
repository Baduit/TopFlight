FROM ubuntu:22.10 
WORKDIR /topflight
COPY . /topflight/

ENV DEBIAN_FRONTEND noninteractive
RUN apt update
RUN apt install build-essential -y
RUN apt install cmake -y
RUN apt install git -y

RUN cmake -B build
RUN cmake --build build
CMD ["./build/webTopflight"]

EXPOSE 7890