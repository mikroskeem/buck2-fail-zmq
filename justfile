build:
    buck2 build //:buck2-fail-zmq

clean:
    buck2 clean

buckify:
    ./scripts/reindeer buckify
