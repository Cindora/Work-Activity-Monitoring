# Client-Server application for monitoring work activity

The information received from the client (currently from the Information folder) is sent to the server, 
where it is processed and written to a separate file in the Data folder. 
Information about actual connected clients is stored in Data/ConnectedClients.data.

## Features done:

- Client preparation
- Server preparation
- Client-server messaging
- Provide work with multiple clients
- Client: Send data to the server
- Server: Saving client data to a file
- Server: List of all connected clients

## Features in progress (and TODOs):

- Client: Collect data (domain/machine/ip/user), last activity
- Server: Screenshot request
- Client: Screenshot (Receive screenshot request, execute screenshot, send screenshot to server)
- Server: Screenshot (Receiving and saving a screenshot, entering information about it into the database)
- Client: Quiet startup at login, running in background mode