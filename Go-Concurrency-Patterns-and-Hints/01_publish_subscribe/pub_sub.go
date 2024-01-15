package server

type Event struct{}

type PubSub interface {
	// Publish publishes the event e to all current subscribers.
	Publish(e Event)

	// Subscribe registers c to receive future events.
	// All subscribers receive events in the same order,
	// and that order respects program order:
	// if Publish(e1) happens before Publish(e2),
	// subscribers receive e1 before e2.
	Subscribe(c chan<- Event)

	// Cancel cancels the prior subscription of channel c.
	// After any pending already-published events
	// have been sent on c, the server will signal that the
	// subscription is cancelled by closing c.
	Cancel(c chan<- Event)
}
