package protocol_multiplexer

import "sync"

type Mux struct {
	srv  Service
	send chan Msg

	mu      sync.Mutex
	pending map[int64]chan<- Msg
}

func (m *Mux) Init(srv Service) {
	m.srv = srv
	m.pending = make(map[int64]chan<- Msg)
	go m.sendLoop()
	go m.recvLoop()
}

func (m *Mux) sendLoop() {
	for args := range m.send {
		m.srv.Send(args)
	}
}

func (m *Mux) recvLoop() {
	for {
		reply := m.srv.Recv()
		tag := m.srv.ReadTag(reply)
		m.mu.Lock()
		done := m.pending[tag]
		delete(m.pending, tag)
		m.mu.Unlock()
		if done == nil {
			panic("unexpected reply")
		}
		done <- reply
	}
}

func (m *Mux) Call(args Msg) (reply Msg) {
	tag := m.srv.ReadTag(args)
	done := make(chan Msg, 1)

	m.mu.Lock()
	if m.pending[tag] != nil {
		m.mu.Unlock()
		panic("mux: duplicate call tag")
	}
	m.pending[tag] = done
	m.mu.Unlock()

	m.send <- args
	return <-done
}
