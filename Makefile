init: server

server:
	gcc server.c -o server

clean:
	rm server

push:
	git add .
	git commit -m 'Final'
	git push

