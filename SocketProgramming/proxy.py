# -*- coding: utf-8 -*-
"""
Created on Fri May 13 17:55:10 2022

@author: Ahmet
"""

# Echo server program
import socket
import time
import re


#Loopback IP address
HOST = '127.0.0.1'
PORT = 6002
PORT2 = 3005
#Create a sockets
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print ("Socket successfully created")

# This line avoids bind() exception: OSError: [Errno 48] Address already in use as you configure address reuse
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server_socket.bind((HOST, PORT))
print ("Socket is bound to IP:",HOST," PORT:",PORT)
server_socket.listen(1)
print("Listening for connections")
conn, clientAddress = server_socket.accept()
print ('Connected ', clientAddress)
oldest_rank = [0,1,2,3,4]
proxy_indices = [0,1,2,3,4]
proxy_table = [3,5,7,4,1]

while 1:
    try:
        dataReceived = conn.recv(1024)
    except OSError:
        print (clientAddress, 'disconnected')
        server_socket.listen(1)
        conn, clientAddress = server_socket.accept()
        print ('Connected by', clientAddress)
        time.sleep(0.5)

    else:  
        
        print("Initial Data is:")
        #Initial data should be a byte object with utf-8 encoding
        print(dataReceived)
        #Decode the data into a string
        dataReceived = dataReceived.decode('utf-8')
        print("Decoded Data is:")
        print(dataReceived)
        datasplit = re.split(';|=', dataReceived)
        back_message = datasplit[0] + "=" + datasplit[1] +";" + datasplit[2]+ "=" + datasplit[3] +";"+ datasplit[4] + "=";
        indices = datasplit[3]
        if(datasplit[1] == "GET" or datasplit[1] == "ADD" ):
            no_data = []
            not_on_proxy = False
            result = 0
            back_messageadd = datasplit[0] + "=" + "ADD" +";" + datasplit[2]+ "=" + datasplit[3] +";"+ datasplit[4] + "=";
            for i in range(0,len(indices),2):
                if(int(indices[i]) in proxy_indices):
                    continue
                else:
                    no_data.append(indices[i])
            if(len(no_data) == 0):
                for i in range(0,len(indices),2):
                    index = proxy_indices.index(int(indices[i]))
                    back_message = back_message + str(proxy_table[index])
                    result = result + proxy_table[index]
                    if(len(indices)>1 and i!= len(indices)-1):
                        back_message += ","
                    else:
                        back_message +=";"
            else:
                not_on_proxy = True
                send_message =  datasplit[0] + "=" + "GET" +";" + datasplit[2]+ "="
                for i in range(0,len(no_data)):
                    send_message = send_message + str(no_data[i])
                    if(len(no_data)>1 and i!= len(no_data)-1):
                        send_message += ","
                    else:
                        send_message +=";"
                        send_message += datasplit[4] + "=" + datasplit[5] + ";";
            if(not_on_proxy):
                client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                client_socket.connect((HOST,PORT2))
                client_socket.sendall(bytes(send_message,'utf-8'))
                time.sleep(0.5)
                dataReceivedServer=client_socket.recv(1024)
                print("Initial Data is:")
                print(dataReceivedServer)
                servermesage = dataReceivedServer.decode('utf-8')
                print("Decoded Data is:")
                print(servermesage)
                serversplit = re.split(';|=', servermesage)
                return_data = re.split(',',serversplit[5])
                return_indices = re.split(',',serversplit[3])
                j = 0
                for i in range(0,len(indices),2):
                    if(int(indices[i]) in proxy_indices):
                        index = proxy_indices.index(int(indices[i]))
                        back_message = back_message + str(proxy_table[index])
                        result = result + proxy_table[index]
                    else:
                        back_message = back_message + str(return_data[j])
                        result = result + int(return_data[j])
                        j+=1
                    if(len(indices)>1 and i!= len(indices)-1):
                        back_message += ","
                    else:
                        back_message +=";"
                for i in range(0,len(return_data)):
                    oldest_rank = [x+1 for x in oldest_rank]
                    indexMax = oldest_rank.index(5)
                    oldest_rank[indexMax] = 0
                    proxy_indices[indexMax] = int(return_indices[i])
                    proxy_table[indexMax] = int(return_data[i])
            if(datasplit[1] == "ADD" ):
                back_message = back_messageadd + str(result) + ';'
               
            print(proxy_indices)
            print(proxy_table)
        elif(datasplit[1] == "PUT"):
            no_data = []
            data = datasplit[5]
            for i in range(0,len(indices),2):
                if(int(indices[i]) in proxy_indices):
                    index = proxy_indices.index(int(indices[i]))
                    proxy_table[index] = int(data[i])
            client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            client_socket.connect((HOST,PORT2))
            client_socket.sendall(bytes(dataReceived,'utf-8'))
            time.sleep(0.5)
            dataReceivedServer=client_socket.recv(1024)
            print("Initial Data is:")
            print(dataReceivedServer)
            servermesage = dataReceivedServer.decode('utf-8')
            print("Decoded Data is:")
            print(servermesage)
            back_message = servermesage + 'ProxyIND='
            for i in range(5):
                back_message = back_message + str(proxy_indices[i]) 
                if(i != 4):
                   back_message = back_message + ','
            back_message = back_message + ';' + 'ProxyTable='
            for i in range(5):
                back_message = back_message + str(proxy_table[i]) 
                if(i != 4):
                   back_message = back_message + ','
            back_message = back_message + ';'
        elif(datasplit[1] == "CLR"):
            for i in range(0, 5):
                proxy_table[i] = 0
            client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            client_socket.connect((HOST,PORT2))
            client_socket.sendall(bytes(dataReceived,'utf-8'))
            time.sleep(0.5)
            dataReceivedServer=client_socket.recv(1024)
            print("Initial Data is:")
            print(dataReceivedServer)
            servermesage = dataReceivedServer.decode('utf-8')
            print("Decoded Data is:")
            print(servermesage)
            back_message = servermesage + 'ProxyIND='
            for i in range(5):
                back_message = back_message + str(proxy_indices[i]) 
                if(i != 4):
                   back_message = back_message + ','
            back_message = back_message + ';' + 'ProxyTable='
            for i in range(5):
                back_message = back_message + str(proxy_table[i]) 
                if(i != 4):
                   back_message = back_message + ','
            back_message = back_message + ';'
        
        #Encode and send the data back to the client
        conn.sendall(bytes(back_message,'utf-8'))



