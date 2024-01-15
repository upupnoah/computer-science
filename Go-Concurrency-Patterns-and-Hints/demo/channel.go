package main

func main() {
	ch := make(chan int)
	go func() {
		ch <- 1
	}()
	<-ch

	// 由于是无缓冲的 channel, 因此不管是接收还是发送, 都会阻塞, 下方的 go func 不会被调度
	// <-ch
	// go func() {
	// 	ch <- 1
	// }()

	// 有缓冲的 channel, 由于缓冲区有空间, 因此发送不会阻塞, 下方的 go func 会被调度
	bufferedCh := make(chan int, 1)
	bufferedCh <- 1
	go func() {
		<-bufferedCh
	}()
}
