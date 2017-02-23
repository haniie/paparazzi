#include "sony_a7r_handler.h"
#include "pprzlink/pprzlink_device.h"
#include "mcu_periph/uart.h"
#include "subsystems/datalink/datalink.h" 
#include "generated/airframe.h"

struct link_device *wifi_command;

#ifndef ESP_01_BAUD
#define ESP_01_BAUD B115200
#endif

#ifndef ESP_01_UART_PORT 
//#define ESP_01_UART_PORT uart6
#endif

void sony_a7r_handler_setup(void){
	wifi_command = &((ESP_01_UART_PORT).device);
	uart_periph_set_bits_stop_parity(&ESP_01_UART_PORT, UBITS_8, USTOP_1, UPARITY_NO);
	uart_periph_set_baudrate(&ESP_01_UART_PORT, ESP_01_BAUD);
}

void sony_a7r_shoot(void){
	char send_command_msg[19] = "AT+CIPSEND=0,266,\r\n";
	for(int i=0; i<18; i++){
		wifi_command->put_byte(wifi_command->periph, 0, (uint8_t)send_command_msg[i]);
	}

	char shoot_command_msg[266] = "POST /sony/camera HTTP/1.1\r\nContent-Type: application/json; charset=utf-8\r\nAccept: Accept-application/json\r\nHost:192.168.122.1:8080\r\nContent-Length: 62\r\nExpect: 100-continue\r\nConnection: Keep-Alive\r\n\r\n{\"method\":\"actTakePicture\",\"params\":[],\"id\":1,\"version\":\"1.0\"}\r\n";
	for(int i=0; i<266; i++){
		wifi_command->put_byte(wifi_command->periph, 0, (uint8_t)shoot_command_msg[i]);
	}
}

void esp_01_initialize(void){
	char jap_msg[43] = "AT+CWJAP=\"DIRECT-hvE0:ILCE-7R\",\"p9tWfStF\"\r\n";
	for(int i=0; i<43; i++){
		wifi_command->put_byte(wifi_command->periph, 0, (uint8_t)jap_msg[i]);
	}

	char multi_con_msg[13] = "AT+CIPMUX=1\r\n";
	for(int i=0; i<13; i++){
		wifi_command->put_byte(wifi_command->periph, 0, (uint8_t)multi_con_msg[i]);
	}

	char start_con_msg[45] = "AT+CIPSTART=0,\"UDP\",\"239.255.255.250\",1900\r\n";
	for(int i=0; i<45; i++){
		wifi_command->put_byte(wifi_command->periph, 0, (uint8_t)start_con_msg[i]);
	}

	char send_command_msg[19] = "AT+CIPSEND=0,125\r\n";
	for(int i=0; i<18; i++){
		wifi_command->put_byte(wifi_command->periph, 0, (uint8_t)send_command_msg[i]);
	}

	char request_msg[125] = "M-SEARCH * HTTP/1.1\r\nHOST:239.255.255.250:1900\r\nMAN:\"ssdp:discover\"\r\nMX:1\r\nST:urn:schemas-1\r\n\r\n";
	for(int i=0; i<125; i++){
		wifi_command->put_byte(wifi_command->periph, 0, (uint8_t)request_msg[i]);
	}

	char close_msg[15] = "AT+CIPCLOSE=0\r\n";
	for(int i=0; i<15; i++){
		wifi_command->put_byte(wifi_command->periph, 0, (uint8_t)close_msg[i]);
	}

	char start_con_msg_tcp[43] = "AT+CIPSTART=0,\"TCP\",\"192.168.122.1\",8080\r\n";
	for(int i=0; i<43; i++){
		wifi_command->put_byte(wifi_command->periph, 0, (uint8_t)start_con_msg[i]);
	}

	char send_command_msg_tcp[18] = "AT+CIPSEND=0,264\r\n";
	for(int i=0; i<18; i++){
		wifi_command->put_byte(wifi_command->periph, 0, (uint8_t)send_command_msg[i]);
	}

	char prepare_to_shoot_msg[264] = "POST /sony/camera HTTP/1.1\r\nContent-Type: application/json; charset=utf-8\r\nAccept: Accept-application/json\r\nHost:192.168.122.1:8080\r\nContent-Length: 60\r\nExpect: 100-continue\r\nConnection: Keep-Alive\r\n\r\n{\"method\":\"startRecMode\",\"params\":[],\"id\":1,\"version\":\"1.0\"}\r\n";
	for(int i=0; i<264; i++){
		wifi_command->put_byte(wifi_command->periph, 0, (uint8_t)prepare_to_shoot_msg[i]);
	}
}