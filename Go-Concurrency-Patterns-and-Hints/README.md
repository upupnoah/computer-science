# Go-Concurrency

## Note
- 并发 与 并行
  - 并发: 编写能够独立执行控制流的程序(调用进程/线程/goroutine)
  - 并行: 多个程序同时执行
- Hint: Convert data state into code state when it makes programs clearer. (通过一些代码逻辑来替换一些数据状态)
- Hint: Use additional goroutine to hold additional code state.
- Hint: Know why and when each goroutine will exit.
- Hint: Type Ctrl-\ to kill a program and dump all its goroutine stacks.
- Hint: Use the HTTP server's /debug/pprof/goroutine to inspect live goroutine stacks.
- Hint: Close a channel to signal that no more values will be sent.
- Hint: Prefer defer for unlocking mutexes.
- Hint: Consider the effect of slow goroutines.
- Hint: Think carefully before introducindg unbounded queuing.
- Hint: Convert mutexes into goroutines when it makes programs clearer.
- Hint: Use goroutines to let independent concerns run independently.
- Hint: Know why and when each communication will proceed.
- Hint: Make sure you know why and when each goroutine will exit.
- Hint: Stop timers you don't need.
- Hint: Use a goto if that is the clearest way to write the code.