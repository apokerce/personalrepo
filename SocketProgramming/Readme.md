__Implementation Detail__

Used the MATLAB documentations to initiate a communication 
between Proxy and the server. Server listens continuously the Proxy and if there is a 
data coming from it, it executes the necessary conditions. The operations are made 
with a simple if else condition. Since the data sent is string the server first splits the 
data and take each part individually converts the numbers into integers and add or 
update the table. 
The Proxy server is the bridge between the server and the client. Similar codes as in 
the server is generated in the Proxy. However, since Proxy has smaller buffer when 
the get command is executed and if Proxy does not have the data it is fetched from the 
server. The newly data is replacing the oldest data in the Proxy table. For that purpose,
I used a rank array to keep the indices from oldest to newest. When the get command 
is requested, Proxy changes their ranks and oldest data is being replaced. Proxy server 
is communicating with the server only if the data could not be found on the proxy. 
Oldest change can be observed from proxy terminal.

__GET OPERATION__

If data found in the proxy, it directly sends back the data to client. If not, the indices 
that does not found in proxy is sent to server so that server returns these data to the 
proxy and whole request is combined in proxy to send to the client.

__ADD OPERATION__

For add operation, first the proxy is being checked and if all data is in the proxy the 
numbers are directly added. If not, they are fetched from the server and sum is sent 
back to client. 

__CLR OPERATION__

When the clear signal is sent from the client, proxy directly sends the message to the 
server so that both server and the proxy is being cleared. All data in the tables are 
being zero. Also, returns the table in proxy and server.

__PUT OPERATION__

When the PUT request coming from client both the server and proxy is being updated. 
If indices do not exist in the proxy, only the server is updated as expected. Also, returns 
the table in proxy and server.

__EXAMPLE OPERATIONS__

OP=PUT;IND=7,8;DATA=4,5;
OP=GET;IND=1,8;DATA=;
OP=ADD;IND=7,8;DATA=;
OP=GET;IND=7,8;DATA=;
OP=CLR;IND=;DATA=;
OP=PUT;IND=1,2;DATA=4,5;
OP=ADD;IND=0,1;DATA=;
OP=GET;IND=1,2;DATA=;
OP=PUT;IND=7,6,2,5,4;DATA=0,1,2,3,4;
OP=GET;IND=7,6,2,5,4;DATA=;
OP=ADD;IND=5,6,2,1,8;DATA=;
OP=CLR;IND=;DATA=;

Above codes are deployed and outputs are observed as in the following Figures.

<p align="center">
  <img width="700" height="500" src="https://user-images.githubusercontent.com/94297285/168577236-5024c440-fe31-4914-802a-f824ee8ccce6.png">
</p>

<p align="center">
  <img width="400" height="600" src="https://user-images.githubusercontent.com/94297285/168577287-27337479-e9ed-4e8c-a31d-f7622a2dfd57.png">
</p>

