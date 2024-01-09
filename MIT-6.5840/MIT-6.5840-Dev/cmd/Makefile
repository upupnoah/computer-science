.PHONY: mrsequential mrworker mrcoordinator clean

mrsequential:
	go run ./mrsequential/mrsequential.go ../mrapps/wc.so ../main/pg*
mrworker:
	go run ./mrworker/mrworker.go ../mrapps/wc.so
mrcoordinator:
	go run ./mrcoordinator/mrcoordinator.go ../main/pg*
clean:
	rm -f mr-out-0