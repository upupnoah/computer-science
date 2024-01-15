package server

import (
	"sync"
)

type ServerV1 struct {
	mu  sync.Mutex
	sub map[chan<- Event]bool
}

func (s *ServerV1) Init() {
	s.sub = make(map[chan<- Event]bool)
}

func (s *ServerV1) Publish(e Event) {
	s.mu.Lock()
	defer s.mu.Unlock()

	// Publish the event to all subscribers.
	for c := range s.sub {
		c <- e
	}
}

func (s *ServerV1) Subscribe(c chan<- Event) {
	s.mu.Lock()
	defer s.mu.Unlock()

	if s.sub[c] {
		panic("pubsub: already subscribed")
	}
	s.sub[c] = true
}

func (s *ServerV1) Cancel(c chan<- Event) {
	s.mu.Lock()
	defer s.mu.Unlock()

	if !s.sub[c] {
		panic("pubsub: not subscribed")
	}
	close(c)
	delete(s.sub, c)
}
