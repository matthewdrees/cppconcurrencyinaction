# Latest alpine at the time of this creation. Increment when convenient.
FROM alpine:3.7

WORKDIR /app

ADD . /app

RUN apk add --no-cache g++ musl-dev make

RUN make -j9

RUN ./cppconcurrencyinaction
