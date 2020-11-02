#ifndef _REQUESTS_
#define _REQUESTS_

using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <cstdint>
#include <cstring>
#include <string>
#include <netinet/in.h>
#include <cmath>
#include <zconf.h>
#include <string.h>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <list>
#include <algorithm>
#include <iostream>
#include <bits/stdc++.h>

// computes and returns a GET request string (query_params
// and cookies can be set to NULL if not needed)
char *compute_get_request(char *host, char *url,
							char *cookies, char *token,int ok_cookie, int ok_token);

// computes and returns a POST request string (cookies can be NULL if not needed)
char *compute_post_request(char *host, char *url, char* content_type, char *body_data,
							 char* cookies, char *token);

char *compute_delete_request(char *host, char *url, char *token);

#endif
