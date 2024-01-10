.PHONY: mrsequential mrworker mrcoordinator clean

mrsequential:
	go run -gcflags="all=-N -l" ./mrsequential/mrsequential.go ../mrapps/wc.so ../main/pg*
mrworker:
	go run -gcflags="all=-N -l" ./mrworker/mrworker.go ../mrapps/wc.so
mrcoordinator:
	go run -gcflags="all=-N -l" ./mrcoordinator/mrcoordinator.go ../main/pg*
clean:
	rm -f mr-out-0