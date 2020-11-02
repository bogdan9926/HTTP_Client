#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "json.hpp"
using json = nlohmann::json;
using namespace std;
int main(int argc, char **argv) 
{
    char *message;
    char *response;
    int sockfd;

    //variabile care retin locul unde ma aflu: 
    //inainte si dupa login, sau in biblioteca
    bool ok_login = false;
    bool ok_enter = false;



    json j_register, j_add_book;
    char buffer[20];
    char username[30];
    char password[30];
    char *cookie;
    char *token;
    char *p;
    char host[47] = "ec2-3-8-116-10.eu-west-2.compute.amazonaws.com";
    
    while(1) {

        sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
        printf("\nType command here:");
        scanf("%s",buffer);

        if(!strncmp(buffer,"register",8)) {

            printf("username=");
            scanf("%s",username);
            printf("password=");
            scanf("%s",password);

            j_register = {{"username",username},{"password",password}};

            std::string s = j_register.dump();
            char *body = new char[s.length() + 1];
            strcpy(body, s.c_str());

            message = compute_post_request(host, "/api/v1/tema/auth/register", 
                                    "application/json", body, NULL, NULL);

            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            //verific daca raspunsul a returnat OK
            if(!strstr(response,"Created")) { 
                printf("\nUser is already in use!\n");
                continue;
            } else printf("\nSuccessful registering for user %s!\n",username);

        } else if(!strncmp(buffer,"login",5)) {

             if(ok_login) {
                printf("\nYou are already logged in\n");
                continue;
            }

            printf("username=");
            scanf("%s",username);
            printf("password=");
            scanf("%s",password);

            j_register = {{"username",username},{"password",password}};

            std::string s = j_register.dump();
            char *body = new char[s.length() + 1];
            strcpy(body, s.c_str());
            message = compute_post_request(host, "/api/v1/tema/auth/login", 
                                    "application/json", body, NULL,NULL);

             
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);

            //verific daca raspunsul a returnat OK
            if(!strstr(response,"OK")) {
                printf("\nUser and/or password are wrong!\n");
                continue;
            } else printf("\nSuccessful login!\nWelcome, %s!\n",username);

            //retin ca sunt logat
            ok_login=true;
            
            //salvez cookieul primit in raspuns
            p = strstr(response,"connect.sid");
            cookie = strtok(p,";");


        } else if(!strncmp(buffer,"enter_library",13)) {

            if(ok_enter) {
                printf("\nNu mai intra ma din nou\n");
            }
            if(!ok_login) {
                printf("\nYou are not logged in!\n");
                continue;
            }

            message = compute_get_request(host,"/api/v1/tema/library/access",
                                cookie,NULL,1,0);

            printf("\nEntering library\n");
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);

            //verific daca raspunsul a returnat OK
            if(!strstr(response,"OK")) {
                printf("\nError while entering library!\n");
                continue;
            } else printf("\nUser %s has entered library!\n",username);

            //retin ca am intrat in biblioteca
            ok_enter = true;

            //salvez tokenul primit ca raspuns
            p = strstr(response,"token");
            token = strtok (p+8,"\"");



        } else if(!strncmp(buffer,"get_books",9)) {

            if(!ok_login) {
                printf("\nYou are not logged in!\n");
                continue;
            }

            if(!ok_enter) {
                printf("\nYou are not in the library!\n");
                continue;
            }

            printf("\nGetting books\n");
            message = compute_get_request(host,"/api/v1/tema/library/books",
                                NULL,token,0,1);

            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);

            char *books;
            books = strstr(response, "[{");
            printf("\nBooks in the library are:\n%s\n",books);


        }

        else if(!strncmp(buffer,"add_book",8)) {

            if(!ok_login) {
                printf("\nYou are not logged in!\n");
                continue;
            }

            if(!ok_enter) {
                printf("\nYou are not in the library!\n");
                continue;
            }

            int numberChecker;
            char title[30];
            char author[30];
            char genre[30];
            char pageCount[5];
            int page_count;
            char publisher[30];

            fgets(title,30, stdin);
            printf("title=");
            fgets(title,30, stdin);
            title[strlen(title)-1] = '\0';
            //verificare daca titlu e gol
            if(!strcmp(title,"")) {
                printf("\nTitle cannot be empty!\n");
                continue;
            }
                
            printf("author=");
            fgets(author,30, stdin);
            author[strlen(author)-1] = '\0';
            //verificare daca autorul e gol sau numar
            if(!strcmp(author,"")) {
                printf("\nAuthor cannot be empty!\n");
                continue;
            }
            numberChecker = atoi(author);
            if(numberChecker !=0) {
                printf("\nAuthor cannot be a number!\n");
                continue;
            }

            printf("genre=");
            fgets(genre,30, stdin);
            genre[strlen(genre)-1] = '\0';
            //verificare daca genul e gol sau numar
            if(!strcmp(genre,"")) {
                printf("\nGenre cannot be empty!\n");
                continue;
            }
            numberChecker = atoi(genre);
            if(numberChecker !=0) {
                printf("\nGenre cannot be a number!\n");
                continue;
            }

            printf("pageCount=");
            fgets(pageCount,30, stdin);
            pageCount[strlen(pageCount)-1] = '\0';
            page_count = atoi(pageCount);
            //verificare daca page_count nu e numar
            if(page_count == 0) {
                printf("\nWrong page number!\n");
                continue;
            }
            printf("publisher=");
            fgets(publisher,30, stdin);
            publisher[strlen(publisher)-1] = '\0';
            //verificare daca publisher e gol
            if(!strcmp(publisher,"")) {
                printf("\nPublisher cannot be empty!\n");
                continue;
            }

            j_add_book = {{"title",title},{"author",author},{"genre",genre},
            {"page_count",page_count},{"publisher",publisher}};

            std::string s = j_add_book.dump();
            char *body = new char[s.length() + 1];
            strcpy(body, s.c_str());

            message = compute_post_request(host, "/api/v1/tema/library/books",
                                 "application/json", body, NULL, token);

            
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);

            //verific daca raspunsul a returnat OK
            if(!strstr(response,"OK")) {
                printf("\nError while adding book in library!\n");
                continue;
            } else printf("\nBook added!\n");


        } else if(!strncmp(buffer,"delete_book",11)) {

            if(!ok_login) {
                printf("You are not logged in!\n");
                continue;
            }

            if(!ok_enter) {
                printf("You are not in the library!\n");
                continue;
            }

            char id[10];
            int number_id;
            fgets(id,30, stdin);
            printf("id=");
            fgets(id,30, stdin);

            char path[100]= "/api/v1/tema/library/books/";
            id[strlen(id)-1] = '\0';
            strcat(path, id);
            number_id = atoi(id);
            //verific daca id-ul nu e numar
            if(number_id == 0) {
                printf("\nWrong type of id!\n");
                continue;
            }


            message = compute_delete_request(host,path, token );
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);

            //verific daca raspunsul a returnat OK
            if(!strstr(response,"OK")) {
                printf("\nThe book with id=%s doesnt exist!\n",id);
                continue;
            } else printf("\nBook with id=%s is deleted!\n",id);


        } else if(!strncmp(buffer,"get_book",8)) {

            if(!ok_login) {
                printf("\nYou are not logged in!\n");
                continue;
            }

            if(!ok_enter) {
                printf("\nYou are not in the library!\n");
                continue;
            }
            int number_id;
            char id[10];
            fgets(id,30, stdin);
            printf("id=");
            fgets(id,30, stdin);

            char path[100]= "/api/v1/tema/library/books/";
            id[strlen(id)-1] = '\0';
            strcat(path, id);
            number_id = atoi(id);
            //verific daca id-ul nu e numar
            if(number_id == 0) {
                printf("\nWrong type of id!\n");
                continue;
            }

            message = compute_get_request(host,path,NULL,token,0,1);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            char *book;
            book = strstr(response, "[{");
            //verific daca raspunsul a returnat OK
            if(!strstr(response,"OK")) {
                printf("\nThe book with id=%s doesnt exist!\n",id);
                continue;
            }
            printf("\nDetails for book with id=%s are shown here:\n%s\n",
                id,book);


        } else if(!strncmp(buffer,"logout",6)) {

            if(!ok_login) {
                printf("\nLogout from where?\n");
                continue;
            }
            //am iesit din librarie si nu mai sunt logat
            ok_login=false;
            ok_enter=false;
            message = compute_get_request(host,"/api/v1/tema/auth/logout",
                                cookie,NULL,1,0);

            printf("\nLogging out\n");
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            printf("\nUser %s logged out!\n",username);

        } else if(!strncmp(buffer,"exit",4)) {

            printf("\nBye bye!\n");
            break;
        }
    }
    return 0;


}
