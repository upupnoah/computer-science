package protocol_multiplexer

type Msg interface{}

type ProtocolMux interface {
	// Init initializes the mux to manage messages to the given service.
	Init(Service)

	// Call makes a request with the given message and returns the reply.
	// Multiple goroutines may call Call concurrently.
	Call(Msg) Msg
}

type Service interface {
	// ReadTag returns the muxing identifier in the request or reply message.
	// Multiple goroutines may call ReadTag concurrently.
	ReadTag(Msg) int64

	// Send sends a request message to the remote service.
	// Send must not be called concurrently with itself.
	Send(Msg)

	// Recv waits for and returns a reply message from the remote service.
	// Recv must not be called concurrently with itself.
	Recv() Msg
}
