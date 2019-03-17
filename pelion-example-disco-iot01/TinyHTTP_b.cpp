/*
 * mbed Tiny HTTP Client for Ethernet Interface Library
 * Copyright (c) 2012 Hiroshi Suga
 * Released under the MIT License: http://mbed.org/license/mit
 */
 
/** @file
 * @brief Tiny HTTP Client
 */
 
#include "mbed.h"
#include "TinyHTTP_b.h"
#include <ctype.h>
 
#define DEBUG 1
 

static onHttpReceiveFunc onHttpReceive;
 
// Copyright (c) 2010 Donatien Garnier (donatiengar [at] gmail [dot] com)
int base64enc(const char *input, unsigned int length, char *output, int len) {
  static const char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  unsigned int c, c1, c2, c3;
 
  if (len < ((((length-1)/3)+1)<<2)) return -1;
  for(unsigned int i = 0, j = 0; i<length; i+=3,j+=4) {
    c1 = ((((unsigned char)*((unsigned char *)&input[i]))));
    c2 = (length>i+1)?((((unsigned char)*((unsigned char *)&input[i+1])))):0;
    c3 = (length>i+2)?((((unsigned char)*((unsigned char *)&input[i+2])))):0;
 
    c = ((c1 & 0xFC) >> 2);
    output[j+0] = base64[c];
    c = ((c1 & 0x03) << 4) | ((c2 & 0xF0) >> 4);
    output[j+1] = base64[c];
    c = ((c2 & 0x0F) << 2) | ((c3 & 0xC0) >> 6);
    output[j+2] = (length>i+1)?base64[c]:'=';
    c = (c3 & 0x3F);
    output[j+3] = (length>i+2)?base64[c]:'=';
  }
  output[(((length-1)/3)+1)<<2] = '\0';
  return 0;
}
 
// Copyright (c) 2010 Donatien Garnier (donatiengar [at] gmail [dot] com)
int urlencode(char *str, char *buf, int len) {
  static const char to_hex[] = "0123456789ABCDEF";
//  char *pstr = str, *buf = (char*)malloc(strlen(str) * 3 + 1), *pbuf = buf;
  char *pstr = str, *pbuf = buf;
 
  if (len < (strlen(str) * 3 + 1)) return -1;
  while (*pstr) {
    if (isalnum(*pstr) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~') {
      *pbuf++ = *pstr;
    } else if (*pstr == ' ') {
      *pbuf++ = '+';
    } else { 
      *pbuf++ = '%';
      *pbuf++ = to_hex[(*pstr >> 4) & 0x0f];
      *pbuf++ = to_hex[*pstr & 0x0f];
    }
    pstr++;
  }
  *pbuf = '\0';
  return 0;
}
 
void createauth (char *user, char *pwd, char *buf, int len) {
    char tmp[80];
 
    strncpy(buf, "Authorization: Basic ", len);
    snprintf(tmp, sizeof(tmp), "%s:%s", user, pwd);
    base64enc(tmp, strlen(tmp), &buf[strlen(buf)], len - strlen(buf));
    strncat(buf, "\r\n", len - strlen(buf));
}
 
int httpRequest (NetworkInterface *net, int method, char *host, int port, char *uri, char *head, char *body) {
    printf("httpRequest\r\n");
    char buf[256];
    printf("buf\r\n");
    int i, ret = -1;
 
    TCPSocket* http = new TCPSocket();
    
    http->open(net);
    printf("TCPSocket created\r\n");

    if (http->connect(host, port)) goto exit;
    
    printf("connected\r\n");
 
    // send request
    if (method == METHOD_POST) {
        http->send("POST ", 5);
    } else {
        http->send("GET ", 4);
    }
    http->send(uri, strlen(uri));
    http->send(" HTTP/1.1\r\nHost: ", 17);
    http->send(host, strlen(host));
    http->send("\r\n", 2);
    http->send("Connection: close\r\n", 19);
    if (head) {
        http->send(head, strlen(head));
    }
    if (method == METHOD_POST) {
        sprintf(buf, "Content-Length: %d\r\n", strlen(body));
        http->send(buf, strlen(buf));
    }
    http->send("\r\n", 2);
 
    // post method
    if (method == METHOD_POST && body) {
        http->send(body, strlen(body));
    }
 
        printf("wait for response\r\n");
    // recv
    for (;;) {
        i = http->recv(buf, sizeof(buf) - 1);
        if (i == 0) break;
 
      
        printf(buf);
    }
    ret = 0;
 
exit:
    http->close();
    delete http;
 
    return ret;
}