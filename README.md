# HTTP_Client
The client, written in C++, is made to interact via http with an online library that is a server . 
It allows the user to register, login, add/get/delete books from the server.
For transmitting information from client to server I used JWT tokens, and for decoding the information
from the payload from the server I used nlohmann/json.
