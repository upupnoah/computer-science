package replicated_client

type Args struct{}
type Reply struct{}

type ReplicatedClient interface {
	// Init initializes the client to use the given servers.
	// To make a particular request later,
	// the client can use callOne(srv, args), where srv
	// is one of the servers from the list.
	Init(servers []string, callOne func(string, Args) Reply)

	// Call makes a request an available server.
	// Multiple goroutines may call Call concurrently.
	Call(args Args) Reply
}
