all: siteanalyzer

siteanalyzer: main.o http_client.o link_parser.o network.o crawler.o webgraph.o hash.o urlqueue.o tpool.o
	gcc -o siteanalyzer main.o http_client.o link_parser.o network.o crawler.o webgraph.o hash.o urlqueue.o tpool.o -lpthread -fno-stack-protector -g

main.o: main.c crawler.h webgraph.h urlqueue.h tpool.h
	gcc -o main.o -c main.c -lpthread -fno-stack-protector -g

crawler.o: crawler.c crawler.h network.h link_parser.h http_client.h webgraph.h urlqueue.h tpool.h
	gcc -o crawler.o -c crawler.c -lpthread -fno-stack-protector -g

tpool.o: tpool.c tpool.h webgraph.h urlqueue.h
	gcc -o tpool.o -c tpool.c -lpthread -fno-stack-protector -g

hash.o: hash.c hash.h
	gcc -o hash.o -c hash.c -fno-stack-protector -g

webgraph.o: webgraph.c webgraph.h hash.h
	gcc -o webgraph.o -c webgraph.c -fno-stack-protector -g

network.o: network.c network.h
	gcc -o network.o -c network.c -fno-stack-protector -g

link_parser.o: link_parser.c link_parser.h
	gcc -o link_parser.o -c link_parser.c -fno-stack-protector -g

http_client.o: http_client.c http_client.h
	gcc -o http_client.o -c http_client.c -fno-stack-protector -g

urlqueue.o: urlqueue.c urlqueue.h
	gcc -o urlqueue.o -c urlqueue.c -fno-stack-protector -g

.PHONY :clean

clean: 
	-rm siteanalyzer main.o http_client.o link_parser.o network.o webgraph.o hash.o crawler.o urlqueue.o tpool.o
