/** @file esp8266.h
 *
 * @brief Header fajl koji sadrži najbitnije konstante za rad
 * sa ESP8266-01 WiFi modulom.
 */
 
#ifndef _ESP8266_H_
#define _ESP8266_H_

#define CMD_AT "AT\r\n"
#define CMD_RST "AT+RST\r\n"
#define CMD_STA_MODE "AT+CWMODE=1\r\n"
#define CMD_WIFI_CONN "AT+CWJAP=\"etfbl.net\",\"\"\r\n"
#define CMD_SINGLE_CONN_TYPE "AT+CIPMUX=0\r\n"
#define CMD_START_TCP "AT+CIPSTART=\"TCP\",\"10.99.130.47\",8084\r\n"
#define CMD_SEND "AT+CIPSEND=%d\r\n"

#define MESSAGE_OK "OK"
#define MESSAGE_ERROR "ERROR"

#endif /* _ESP8266_H_ */
/*** kraj fajla ***/