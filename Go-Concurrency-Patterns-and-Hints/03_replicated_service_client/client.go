package replicated_client

import (
	"sync"
	"time"
)

type Client struct {
	servers []string
	callOne func(string, Args) Reply
	mu      sync.Mutex
	prefer  int
}

// Init initializes the client to use the given servers.
// To make a particular request later,
// the client can use callOne(srv, args), where srv
// is one of the servers from the list.
func (c *Client) Init(servers []string, callOne func(string, Args) Reply) {
	c.servers = servers
	c.callOne = callOne
}

// Call makes a request an available server.
// Multiple goroutines may call Call concurrently.
func (c *Client) Call(args Args) Reply {
	type result struct {
		serverId int
		reply    Reply
	}

	// add timeout
	const timeout = 1 * time.Second
	t := time.NewTimer(timeout)
	defer t.Stop()

	done := make(chan result, 1)

	id := 1 // id = ...

	go func() {
		done <- result{id, c.callOne(c.servers[id], args)}
	}()

	select {
	case r := <-done:
		return r.reply
	case <-t.C:
		// timeout
	}

	return (<-done).reply
}

// 为了方便阅读, 写了 V2 版本(不涉及到 interface)
func (c *Client) CallV2(args Args) Reply {
	type result struct {
		serverId int
		reply    Reply
	}

	// add timeout
	const timeout = 1 * time.Second
	t := time.NewTimer(timeout)
	defer t.Stop()

	done := make(chan result, len(c.servers))

	for id := 0; id < len(c.servers); id++ {
		id := id // 用于闭包捕获
		go func() {
			done <- result{id, c.callOne(c.servers[id], args)}
		}()

		select {
		case r := <-done:
			return r.reply
		case <-t.C:
			// timeout
			t.Reset(timeout)
		}
	}

	// 如果上面这个 for loop 结束后, 仍然没有收到任何 reply, 那么就会阻塞在这里
	// 因此只要有一个 reply, 就返回
	r := <-done
	return r.reply
}

// Use a goto if that is the clearest way to write the code.
func (c *Client) CallV3(args Args) Reply {
	type result struct {
		serverId int
		reply    Reply
	}

	// add timeout
	const timeout = 1 * time.Second
	t := time.NewTimer(timeout)
	defer t.Stop()

	done := make(chan result, len(c.servers))

	c.mu.Lock()
	prefer := c.prefer
	c.mu.Unlock()

	var r result
	for off := 0; off < len(c.servers); off++ {
		id := (prefer + off) % len(c.servers) // 用于闭包捕获
		go func() {
			done <- result{id, c.callOne(c.servers[id], args)}
		}()

		select {
		case r = <-done:
			goto Done
		case <-t.C:
			// timeout
			t.Reset(timeout)
		}
	}

Done:
	c.mu.Lock()
	c.prefer = r.serverId
	c.mu.Unlock()
	return r.reply
}
