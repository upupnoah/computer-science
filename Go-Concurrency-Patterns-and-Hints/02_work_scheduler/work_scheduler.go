package work_scheduler

// func ScheduleHasBug(servers []string, numTask int, call func(srv string, task int)) {
// 	idle := make(chan string, len(servers))
// 	for i := range servers {
// 		idle <- servers[i]
// 	}
// 	for task := 0; task < numTask; task++ {
// 		go func() {
// 			srv := <-idle
// 			call(srv, task)
// 			idle <- srv
// 		}()
// 	}
// }

func ScheduleFixedV1(servers []string, numTask int, call func(srv string, task int)) {
	idle := make(chan string, len(servers))
	for i := range servers {
		idle <- servers[i]
	}

	for task := 0; task < numTask; task++ {
		// method1: 将 task 作为参数传入 go func()中
		// go func(task int) {
		// 	srv := <-idle
		// 	call(srv, task)
		// 	idle <- srv
		// }(task)

		// method2: 在 go func 之前创建一个变量
		task := task // 每次循环都会创建一个新的变量
		go func() {
			srv := <-idle
			call(srv, task) // 闭包可以捕获外部变量
			idle <- srv
		}()
	}
}

func ScheduleFixedV2(servers []string, numTask int, call func(srv string, task int)) {
	idle := make(chan string, len(servers))
	for i := range servers {
		idle <- servers[i]
	}

	for task := 0; task < numTask; task++ {
		// method1: 将 task 作为参数传入 go func()中
		// go func(task int) {
		// 	srv := <-idle
		// 	call(srv, task)
		// 	idle <- srv
		// }(task)

		// method2: 在 go func 之前创建一个变量
		task := task  // 每次循环都会创建一个新的变量
		srv := <-idle // 现在运行的 goroutine 数量取决于服务器数量, 而不是任务数量
		go func() {
			call(srv, task) // 闭包可以捕获外部变量
			idle <- srv
		}()
	}

	// 等待所有任务完成
	for i := 0; i < len(servers); i++ {
		<-idle
	}
}

func ScheduleFixedV3(servers []string, numTask int, call func(srv string, task int)) {
	work := make(chan int)
	done := make(chan bool)

	runTasks := func(srv string) {
		for task := range work {
			call(srv, task)
		}
		done <- true
	}

	for _, srv := range servers {
		go runTasks(srv)
	}

	for task := 0; task < numTask; task++ {
		work <- task
	}
	close(work)

	for i := 0; i < len(servers); i++ {
		<-done
	}
}

// 由于可能有新的服务器加入进来, 因此将 servers []string, 改为 servers chan string
func ScheduleFixedV4(servers chan string, numTask int, call func(srv string, task int)) {
	work := make(chan int)
	done := make(chan bool)

	runTasks := func(srv string) {
		for task := range work {
			call(srv, task)
		}
		done <- true
	}

	// for _, srv := range servers {
	// 	go runTasks(srv)
	// }

	// 开一个 go routine, 防止 for range channel 读不到数据而阻塞
	go func() {
		for srv := range servers {
			go runTasks(srv)
		}
	}()

	for task := 0; task < numTask; task++ {
		work <- task
	}
	close(work)

	for i := 0; i < len(servers); i++ {
		<-done
	}
}

// 将 done <- true 上移到 for 循环内, 将 wait servers 改成 wait tasks(等待任务完成)
func ScheduleFixedV5(servers chan string, numTask int, call func(srv string, task int)) {
	work := make(chan int)
	done := make(chan bool)

	runTasks := func(srv string) {
		for task := range work {
			call(srv, task)
			done <- true
		}
	}

	// for _, srv := range servers {
	// 	go runTasks(srv)
	// }

	// 开一个 go routine, 防止 for range channel 读不到数据而阻塞
	go func() {
		for srv := range servers {
			go runTasks(srv)
		}
	}()

	for task := 0; task < numTask; task++ {
		work <- task
	}
	close(work)

	// for i := 0; i < len(servers); i++ {
	// 	<-done
	// }
	for i := 0; i < numTask; i++ {
		<-done // 这里有可能会阻塞
	}

	// 问题:
	// 如果servers 的数量小于 numTask, 那么会产生 deadlock
	// 原因: 不是所有任务都能被处理，这将导致主 Goroutine 在尝试从 done 通道接收时永远阻塞，因为不是所有的 done 信号都能被发送
}

// V5 版本会产生 deadlock
func ScheduleFixedV6(servers chan string, numTask int, call func(srv string, task int)) {
	work := make(chan int)
	done := make(chan bool)

	runTasks := func(srv string) {
		for task := range work {
			call(srv, task)
			done <- true
		}
	}

	// for _, srv := range servers {
	// 	go runTasks(srv)
	// }

	// 开一个 go routine, 防止 for range channel 读不到数据而阻塞
	go func() {
		for srv := range servers {
			go runTasks(srv)
		}
	}()

	// for task := 0; task < numTask; task++ {
	// 	work <- task
	// }
	// close(work)

	// // for i := 0; i < len(servers); i++ {
	// // 	<-done
	// // }
	// for i := 0; i < numTask; i++ {
	// 	<-done
	// }

	// 区别: continue WorkLoop, break WorkLoop, goto WorkLoop(不推荐使用)
	// continue WorkLoop: 直接执行下一次循环
	// break WorkLoop: 跳出循环
	// goto WorkLoop: 跳转到指定的位置开始

	i := 0
WorkLoop:
	for task := 0; task < numTask; task++ {
		for {
			select {
			case work <- task:
				continue WorkLoop // 直接执行下一次循环
			case <-done:
				i++
			}
		}
	}
	close(work)

	for ; i < numTask; i++ {
		<-done
	}
}

func ScheduleFixedV7(servers chan string, numTask int, call func(srv string, task int)) {
	work := make(chan int)
	done := make(chan bool)

	runTasks := func(srv string) {
		for task := range work {
			call(srv, task)
			done <- true
		}
	}

	// for _, srv := range servers {
	// 	go runTasks(srv)
	// }

	// 开一个 go routine, 防止 for range channel 读不到数据而阻塞
	go func() {
		for srv := range servers {
			go runTasks(srv)
		}
	}()

	// for task := 0; task < numTask; task++ {
	// 	work <- task
	// }
	// close(work)

	// 将上面这段放入 go func() 中, 防止阻塞 main goroutine
	go func() {
		for task := 0; task < numTask; task++ {
			work <- task
		}
		close(work)
	}()

	for i := 0; i < numTask; i++ {
		<-done
	}
}

func ScheduleFixedV8(servers chan string, numTask int, call func(srv string, task int)) {
	// work := make(chan int)
	work := make(chan int, numTask) // 增大 work channel 的缓冲区, 这样永远不会阻塞
	done := make(chan bool)

	runTasks := func(srv string) {
		for task := range work {
			call(srv, task)
			done <- true
		}
	}

	// for _, srv := range servers {
	// 	go runTasks(srv)
	// }

	// 开一个 go routine, 防止 for range channel 读不到数据而阻塞
	go func() {
		for srv := range servers {
			go runTasks(srv)
		}
	}()

	// for task := 0; task < numTask; task++ {
	// 	work <- task
	// }
	// close(work)

	// 将上面这段放入 go func() 中, 防止阻塞 main goroutine
	// go func() {
	// 	for task := 0; task < numTask; task++ {
	// 		work <- task
	// 	}
	// 	close(work)
	// }()

	// !!!!!!!!!!!!!!
	// V8, 最简单的方法是增大 work channel 的缓冲区, 这样永远不会阻塞
	for task := 0; task < numTask; task++ {
		work <- task
	}
	close(work)

	for i := 0; i < numTask; i++ {
		<-done
	}
}

func ScheduleFixedV9(servers chan string, numTask int, call func(srv string, task int) bool) {
	work := make(chan int, numTask) // 增大 work channel 的缓冲区, 这样永远不会阻塞
	done := make(chan bool)

	// runTasks := func(srv string) {
	// 	for task := range work {
	// 		call(srv, task)
	// 		done <- true
	// 	}
	// }

	// 如果 call (task) 执行失败, 可以将这个任务重新放回任务队列
	runTasks := func(srv string) {
		for task := range work {
			if call(srv, task) {
				done <- true
			} else {
				work <- task
			}
		}
	}

	// 开一个 go routine, 防止 for range channel 读不到数据而阻塞
	go func() {
		for srv := range servers {
			go runTasks(srv)
		}
	}()

	for task := 0; task < numTask; task++ {
		work <- task
	}
	// close(work) // 需要将这个 close 放到下方的 for loop 下面, 确保所有 task 处理完毕

	for i := 0; i < numTask; i++ {
		<-done
	}
	close(work)
}

func ScheduleFixedV10(servers chan string, numTask int, call func(srv string, task int) bool) {
	work := make(chan int, numTask) // 增大 work channel 的缓冲区, 这样永远不会阻塞
	done := make(chan bool)
	exit := make(chan struct{}) // struct{} 类型, 零成本抽象(用于传递信号)

	// 如果 call (task) 执行失败, 可以将这个任务重新放回任务队列
	runTasks := func(srv string) {
		for task := range work {
			if call(srv, task) {
				done <- true
			} else {
				work <- task
			}
		}
	}

	// 开一个 go routine, 防止 for range channel 读不到数据而阻塞
	// go func() {
	// 	for srv := range servers {
	// 		go runTasks(srv)
	// 	}
	// }()

	// 将上方的代码改为下方的代码
	// 优点:
	// 	1. 可控的退出
	// 	2. 优雅的关闭
	//	3. 避免 goroutine 泄露(如果没有 exit channel, 那么 goroutine 会一直运行, 无法被回收)
	//	4. 代码灵活, 可以对多种 signal 进行处理
	go func() {
		for {
			select {
			case srv := <-servers:
				go runTasks(srv)
			case <-exit:
				return
			}
		}
	}()

	for task := 0; task < numTask; task++ {
		work <- task
	}
	// close(work) // 需要将这个 close 放到下方的 for loop 下面, 确保所有 task 处理完毕

	for i := 0; i < numTask; i++ {
		<-done
	}
	close(work)

	exit <- struct{}{}
}
