#ifndef INC_CKC_MODBUSESP32_HPP_
#define INC_CKC_MODBUSESP32_HPP_
#include <Arduino.h>

template <class Modbus>
class CKC_ModbusESP32
{
private:
    HardwareSerial *serial = NULL;
    uint32_t _baudrate = 9600;
    int _RX = -1;
    int _TX = -1;
    int _DE = -1; // mặc định là không có cài chân đảo
    uint32_t _timeout = 1000;

    uint16_t crc16(uint8_t *data, uint8_t len) // kiểm tra lỗi data
    {
        uint16_t crc = 0xFFFF;
        for (int pos = 0; pos < len; pos++)
        {
            crc ^= data[pos];
            for (int i = 0; i < 8; i++)
            {
                if (crc & 1)
                {
                    crc >>= 1;
                    crc ^= 0xA001;
                }
                else
                    crc >>= 1;
            }
        }
        return crc;
    }

public:
    void beginModbus(HardwareSerial &ser, uint32_t baud, int rx, int tx);
    void beginModbus(HardwareSerial &ser, uint32_t baud, int rx, int tx, int de_pin);
    void setTimeout(uint32_t timeout);
    void setBaudRate(uint32_t baud);
    void flush();
    int available();

    void sendBytes(uint8_t *data, size_t len);
    int readBytes(uint8_t *buf, size_t len);

    int readHoldingRegister(uint8_t slave, uint16_t reg, uint16_t num, uint8_t *response);
    int readHoldingRegisterValue(uint8_t slave, uint16_t reg, uint16_t num, uint16_t *values);
    float readFloat(uint8_t slave, uint16_t reg);

    bool writeSingleRegister(uint8_t slave, uint16_t reg, uint16_t value);
    bool writeMultipleRegisters(uint8_t slave, uint16_t reg, uint16_t num, uint16_t *values);
};

template <class Modbus>
inline void CKC_ModbusESP32<Modbus>::beginModbus(HardwareSerial &ser, uint32_t baud, int rx, int tx)
{
    serial = &ser;
    _baudrate = baud;
    _RX = rx;
    _TX = tx;
    _DE = -1;
    serial->begin(_baudrate, SERIAL_8N1, _RX, _TX);
}

template <class Modbus>
inline void CKC_ModbusESP32<Modbus>::beginModbus(HardwareSerial &ser, uint32_t baud, int rx, int tx, int de_pin)
{
    serial = &ser;
    _baudrate = baud;
    _RX = rx;
    _TX = tx;
    _DE = de_pin;
    pinMode(_DE, OUTPUT);
    digitalWrite(_DE, LOW); // mặc định khi khởi động là nhận
    serial->begin(_baudrate, SERIAL_8N1, _RX, _TX);
}

template <class Modbus>
inline void CKC_ModbusESP32<Modbus>::setBaudRate(uint32_t baud)
{
    _baudrate = baud;
    if (serial != NULL)
    {
        serial->updateBaudRate(_baudrate);
    }
}

template <class Modbus>
void CKC_ModbusESP32<Modbus>::setTimeout(uint32_t timeout)
{
    this->_timeout = timeout;
}

template <class Modbus>
void CKC_ModbusESP32<Modbus>::flush()
{
    if (this->serial != NULL)
    {
        this->serial->flush();
    }
}

template <class Modbus>
int CKC_ModbusESP32<Modbus>::available()
{
    if (this->serial != NULL)
    {
        return this->serial->available();
    }
    return 0;
}

template <class Modbus>
void CKC_ModbusESP32<Modbus>::sendBytes(uint8_t *data, size_t len)
{
    serial->write(data, len);
    serial->flush();
}

template <class Modbus>
int CKC_ModbusESP32<Modbus>::readBytes(uint8_t *buf, size_t len)
{
    if (serial == NULL)
        return 0;

    size_t index = 0;
    unsigned long start = millis();

    while ((millis() - start) < _timeout)
    {
        if (serial->available())
        {
            buf[index++] = serial->read();

            if (index >= len)
                break;
        }
    }
    return index;
}

template <class Modbus>
int CKC_ModbusESP32<Modbus>::readHoldingRegister(uint8_t idslave, uint16_t reg, uint16_t num, uint8_t *response) // gửi request và nhận data chưa xử lý
{
    uint8_t frame[8];

    frame[0] = idslave;
    frame[1] = 0x04;
    frame[2] = reg >> 8;
    frame[3] = reg & 0xFF;
    frame[4] = num >> 8;
    frame[5] = num & 0xFF;

    uint16_t crc = crc16(frame, 6);
    frame[6] = crc & 0xFF;
    frame[7] = crc >> 8;

    sendBytes(frame, 8);
    delay(100);
    return readBytes(response, 5 + 2 * num);
}

template <class Modbus>
int CKC_ModbusESP32<Modbus>::readHoldingRegisterValue(uint8_t slave, uint16_t reg, uint16_t num, uint16_t *values) // xử lý data thành dạng số
{
    uint8_t buffer[64];

    int len = readHoldingRegister(slave, reg, num, buffer);

    if (len < (5 + 2 * num))
        return -1;

    for (int i = 0; i < num; i++)
    {
        values[i] = (buffer[3 + i * 2] << 8) |
                    buffer[4 + i * 2];
    }

    return num;
}

template <class Modbus>
float CKC_ModbusESP32<Modbus>::readFloat(uint8_t slave, uint16_t reg)
{
    uint16_t data[2];

    if (readHoldingRegisterValue(slave, reg, 2, data) != 2)
    {
        Serial.println("READ FAIL");
        return NAN;
    }

    uint32_t raw = ((uint32_t)data[1] << 16) | data[0];

    float value;
    memcpy(&value, &raw, sizeof(value));

    return value;
}

template <class Modbus>
bool CKC_ModbusESP32<Modbus>::writeSingleRegister(uint8_t slave, uint16_t reg, uint16_t value)
{
    uint8_t frame[8];

    frame[0] = slave;
    frame[1] = 0x06;
    frame[2] = reg >> 8;
    frame[3] = reg & 0xFF;
    frame[4] = value >> 8;
    frame[5] = value & 0xFF;

    uint16_t crc = crc16(frame, 6);
    frame[6] = crc & 0xFF;
    frame[7] = crc >> 8;

    sendBytes(frame, 8);
    delay(100);

    uint8_t resp[8];
    int len = readBytes(resp, 8);

    if (len < 5)
        return false;
    if (resp[1] & 0x80)
        return false;
    if (resp[1] != 0x06)
        return false;

    uint16_t crc_resp = (resp[len - 1] << 8) | resp[len - 2];
    uint16_t crc_calc = crc16(resp, len - 2);

    return (crc_resp == crc_calc);
}

template <class Modbus>
bool CKC_ModbusESP32<Modbus>::writeMultipleRegisters(uint8_t slave, uint16_t reg, uint16_t num, uint16_t *values)
{
    if (num == 0 || num > 60)
        return false;

    uint8_t frame[256];
    uint8_t index = 0;

    frame[index++] = slave;
    frame[index++] = 0x10;

    frame[index++] = reg >> 8;
    frame[index++] = reg & 0xFF;

    frame[index++] = num >> 8;
    frame[index++] = num & 0xFF;

    frame[index++] = num * 2;

    for (int i = 0; i < num; i++)
    {
        frame[index++] = values[i] >> 8;
        frame[index++] = values[i] & 0xFF;
    }

    uint16_t crc = crc16(frame, index);
    frame[index++] = crc & 0xFF;
    frame[index++] = crc >> 8;

    sendBytes(frame, index);
    delay(100);

    uint8_t resp[8];
    int len = readBytes(resp, 8);

    if (len < 5)
        return false;
    if (resp[1] & 0x80)
        return false;
    if (resp[1] != 0x10)
        return false;

    uint16_t crc_resp = (resp[len - 1] << 8) | resp[len - 2];
    uint16_t crc_calc = crc16(resp, len - 2);

    return (crc_resp == crc_calc);
}

CKC_ModbusESP32<int> CKCModbus;
#endif
