#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

HANDLE OpenComm(const char* portName) {
    HANDLE serialPort = CreateFile(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (serialPort == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Erro ao abrir a porta COM.\n");
        return INVALID_HANDLE_VALUE;
    }
    return serialPort;
}

int SetCommStateWrapper(HANDLE serialPort, DCB* dcbSerialParams) {
    if (!SetCommState(serialPort, dcbSerialParams)) {
        fprintf(stderr, "Erro para ter as configurações.\n");
        return 0;
    }
    return 1;
}

int GetCommStateWrapper(HANDLE serialPort, DCB* dcbSerialParams) {
    if (!GetCommState(serialPort, dcbSerialParams)) {
        fprintf(stderr, "Erro para ter as configurações.\n");
        return 0;
    }
    return 1;
}

int GetCommErrorWrapper(HANDLE serialPort, COMSTAT* commStat, DWORD* errorCode) {
    if (!ClearCommError(serialPort, errorCode, commStat)) {
        fprintf(stderr, "Erro para ter o status da comunicação.\n");
        return 0;
    }
    return 1;
}

void CloseComm(HANDLE serialPort) {
    CloseHandle(serialPort);
}

void TestOpenComm(const char* portName) {
    HANDLE serialPort = OpenComm(portName);
    if (serialPort != INVALID_HANDLE_VALUE) {
        printf("Porta COM aberta.\n");
        CloseComm(serialPort);
    }
}

void TestSetCommState(const char* portName) {
    HANDLE serialPort = OpenComm(portName);
    if (serialPort != INVALID_HANDLE_VALUE) {
        DCB dcbSerialParams = {0};
        dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
        if (GetCommStateWrapper(serialPort, &dcbSerialParams)) {
            dcbSerialParams.BaudRate = CBR_9600;
            dcbSerialParams.ByteSize = 8;
            dcbSerialParams.Parity = NOPARITY;
            dcbSerialParams.StopBits = ONESTOPBIT;

            if (SetCommStateWrapper(serialPort, &dcbSerialParams)) {
                printf("Configurações da porta COM foram definidas.\n");
            }
        }

        CloseComm(serialPort);
    }
}

void TestGetCommState(const char* portName) {
    HANDLE serialPort = OpenComm(portName);
    if (serialPort != INVALID_HANDLE_VALUE) {
        DCB dcbSerialParams = {0};
        dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
        if (GetCommStateWrapper(serialPort, &dcbSerialParams)) {
            printf("Configurações atuais COM:\n");
            printf("BaudRate: %d\n", dcbSerialParams.BaudRate);
            printf("ByteSize: %d\n", dcbSerialParams.ByteSize);
            printf("Parity: %d\n", dcbSerialParams.Parity);
            printf("StopBits: %d\n", dcbSerialParams.StopBits);
        }

        CloseComm(serialPort);
    }
}

void TestGetCommError(const char* portName) {
    HANDLE serialPort = OpenComm(portName);
    if (serialPort != INVALID_HANDLE_VALUE) {
        COMSTAT commStat = {0};
        DWORD errorCode = 0;
        if (GetCommErrorWrapper(serialPort, &commStat, &errorCode)) {
            printf("Erro na comunicação: %lu\n", errorCode);
        }
        CloseComm(serialPort);
    }
}

int main() {
    const char* portName = "COM1";
    TestOpenComm(portName);
    TestSetCommState(portName);
    TestGetCommState(portName);
    TestGetCommError(portName);

    return 0;
}
