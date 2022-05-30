#pragma once

#include "CircularBuffer.h"
#include "types.h"
#include <RF24.h>
#include <memory>
#include <nRF24L01.h>

// maximum buffer length of packet received / sent to RF24 module
#define MAX_RF_PAYLOAD_SIZE 32

// number of packets hold in buffer
#define PACKET_BUFFER_SIZE 30

typedef struct {
    uint8_t rxCh;
    uint8_t packet[MAX_RF_PAYLOAD_SIZE];
} packet_t;

class HoymilesRadio {
public:
    void init();
    void loop();
    void setPALevel(rf24_pa_dbm_e paLevel);

    serial_u DtuSerial();
    void setDtuSerial(uint64_t serial);

private:
    void ARDUINO_ISR_ATTR handleIntr();
    static serial_u convertSerialToRadioId(serial_u serial);
    uint8_t getRxNxtChannel();
    bool switchRxCh(uint8_t addLoop = 0);

    std::unique_ptr<RF24> _radio;
    uint8_t _rxChLst[4] = { 3, 23, 61, 75 };
    uint8_t _rxChIdx;
    uint16_t _rxLoopCnt;

    volatile bool _packetReceived;

    CircularBuffer<packet_t, PACKET_BUFFER_SIZE> _rxBuffer;

    serial_u _dtuSerial;
};