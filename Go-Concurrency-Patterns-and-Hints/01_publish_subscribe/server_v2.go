package server

// Old version
// type server struct {
// 	mu  sync.Mutex
// 	sub map[chan<- Event]bool
// }

// func (s *server) Init() {
// 	s.sub = make(map[chan<- Event]bool)
// }

type ServerV2 struct {
	publish   chan Event
	subscribe chan subReq
	cancel    chan subReq
}

type subReq struct {
	c  chan<- Event
	ok chan bool
}

func (s *ServerV2) Init() {
	s.publish = make(chan Event)
	s.subscribe = make(chan subReq)
	s.cancel = make(chan subReq)
	go s.loopV2()
}

// func (s *ServerV2) loopV1() {
// 	// v2 version
// 	// sub is local, so it is not need to be protected by mutex.
// 	sub := make(map[chan<- Event]bool)
// 	for {
// 		select {
// 		case e := <-s.publish:
// 			for c := range sub {
// 				c <- e
// 			}
// 		case r := <-s.subscribe:
// 			if sub[r.c] {
// 				r.ok <- false
// 				break
// 			}
// 			sub[r.c] = true
// 			r.ok <- true
// 		case r := <-s.cancel:
// 			if !sub[r.c] {
// 				r.ok <- false
// 				break
// 			}
// 			close(r.c)
// 			delete(sub, r.c)
// 			r.ok <- true
// 		}
// 	}
// }

func (s *ServerV2) loopV2() {
	// v2 version
	// sub is local, so it is not need to be protected by mutex.
	// 维护一个 map, 和 V1 不同, 这里的 map 用于将 publish 和 subscribe 完全分开
	// 作用: 用于当订阅者消费进度变得太慢或其他问题的处理
	sub := make(map[chan<- Event]chan<- Event)
	for {
		select {
		case e := <-s.publish:
			for _, h := range sub {
				h <- e
			}
		case r := <-s.subscribe:
			if sub[r.c] != nil {
				r.ok <- false
				break
			}
			h := make(chan Event)
			go helperV3(h, r.c) // 独立出来
			sub[r.c] = h
			r.ok <- true
		case r := <-s.cancel:
			if sub[r.c] == nil {
				r.ok <- false
				break
			}
			close(sub[r.c])
			delete(sub, r.c)
			r.ok <- true
		}
	}
}

func (s *ServerV2) Publish(e Event) {
	s.publish <- e
}

func (s *ServerV2) Subscribe(c chan<- Event) {
	r := subReq{c: c, ok: make(chan bool)}
	s.subscribe <- r
	if !<-r.ok {
		panic("pubsub: already subscribed")
	}
}

func (s *ServerV2) Cancel(c chan<- Event) {
	r := subReq{c: c, ok: make(chan bool)}
	s.cancel <- r
	if !<-r.ok {
		panic("pubsub: not subscribed")
	}
}

// func helper(in <-chan Event, out chan<- Event) {
// 	var q []Event
// 	for {
// 		select {
// 		case e := <-in:
// 			q = append(q, e)
// 		case out <- q[0]:
// 			q = q[1:]
// 		}
// 	}
// }

// func helperV2(in <-chan Event, out chan Event) {
// 	var q []Event
// 	for {
// 		// Decide whether and what to send.

// 		// default nil, if sendOut = nil, select will never choose this case.
// 		var sendOut chan<- Event
// 		var next Event
// 		if len(q) > 0 {
// 			sendOut = out
// 			next = q[0]
// 		}

// 		select {
// 		case e := <-in:
// 			q = append(q, e)
// 		case sendOut <- next:
// 			q = q[1:]
// 		}
// 	}
// }

func helperV3(in <-chan Event, out chan<- Event) {
	var q []Event
	for in != nil && len(q) > 0 {
		// Decide whether and what to send.
		// default nil, if sendOut = nil, select will never choose this case.
		var sendOut chan<- Event
		var next Event
		if len(q) > 0 {
			sendOut = out
			next = q[0]
		}

		select {
		case e, ok := <-in:
			if !ok {
				in = nil // stop receiving from in
				break
			}
			q = append(q, e)
		case sendOut <- next:
			q = q[1:]
		}
	}
	close(out)
}
