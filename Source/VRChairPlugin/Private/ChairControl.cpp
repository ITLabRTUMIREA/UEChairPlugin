// Fill out your copyright notice in the Description page of Project Settings.

#include "ChairControl.h"
#include "TimerManager.h"




UChairControl* UChairControl::SerialPort(bool& connected, int32 ComPort)
{
	UChairControl* value = NewObject<UChairControl>();
	connected = value->OpenPort(ComPort);
	return value;
}



bool UChairControl::isConnected()
{
	return this->connected;
}

bool UChairControl::PortWrite(TArray<uint8> bytes)
{
	DWORD byteSend;
	if (!WriteFile(this->handler, (void*)bytes.GetData(), bytes.Num(), &byteSend, 0)) {
		return false;
	}
	else return true;
}


bool UChairControl::ClosePort()
{
	CloseHandle(this->handler);
	return true;
}

TArray<uint8> UChairControl::FloatToBytes(float value)
{
	TArray<uint8> bytes;
	bytes.Append(reinterpret_cast<const uint8*>(&value), 4);
	return bytes;
}

void UChairControl::StartSending(AActor* TargetActor, float time)
{
	Timer = TargetActor;
	Timer->GetWorldTimerManager().SetTimer(timer, this, &UChairControl::StartPost, 0.1f, true);
	Timer->GetWorldTimerManager().SetTimer(timerStop, this, &UChairControl::StopPost, time, true);
}

void UChairControl::Control(float roll, float pitch)
{
	this->roll = roll;
	this->pitch = pitch;
}

uint16 UChairControl::CRC16(uint16 crc, uint8 b)
{
	uint16 num1 = 255 & (crc >> 8 ^ b);
	uint16 num2 = num1 ^ num1 >> 4;
	return (crc ^ num2 << 4 ^ num2 >> 3) << 8 ^ (num2 ^ num2 << 5) & 255;
}

uint16 UChairControl::FullCRC(TArray<uint8> p, int32 pSize)
{
	uint16 crc = 58005;
	for (int32 i = 0; i <= pSize - 1; ++i)
		crc = CRC16(crc, p[i]);
	return crc;
}

TArray<uint8> UChairControl::EncodePacket(TArray<uint8> arrByte)
{
	TArray<uint8> appendArr;
	appendArr.Add(255);
	auto crc = UInt16ToBytes(FullCRC(arrByte, arrByte.Num()));
	arrByte.Append(crc);
	appendArr.Append(Clear(arrByte));
	appendArr.Add(254);

	return appendArr;
}

TArray<uint8> UChairControl::Clear(TArray<uint8> value)
{
	TArray<uint8> arr;
	for (int32 i = 0; i < value.Num(); ++i)
	{
		if (value[i] >= 253) {
			arr.Add(253);
			arr.Add(value[i] - 253);
		}
		else
			arr.Add(value[i]);
	}
	return arr;
}

TArray<uint8> UChairControl::UInt16ToBytes(uint16 value)
{
	TArray<uint8> arrAppend;
	arrAppend.Add(value & 255);
	arrAppend.Add(value >> 8);

	return arrAppend;
}

void UChairControl::StartPost()
{
	TArray<uint8> packet;
	packet.Add(33);
	packet.Add(12);
	packet.Add(3);
	packet.Append(FloatToBytes(pitch));
	packet.Append(FloatToBytes(roll));
	packet.Append(FloatToBytes(0));
	packet = EncodePacket(packet);

	FString logResult = "Byte: \n";
	for (int i = 0; i < packet.Num(); ++i) {
		FString textLog = FString::FromInt(packet[i]) + ", ";
		logResult = logResult + textLog;
	}
	UE_LOG(LogTemp, Log, TEXT("%s"), *logResult);

	PortWrite(packet);

	packet.Empty();
}

void UChairControl::StopPost()
{
	Timer->GetWorldTimerManager().ClearTimer(timer);
	Timer->GetWorldTimerManager().ClearTimer(timerStop);
	UE_LOG(LogTemp, Log, TEXT("StopPost"));
}

bool UChairControl::OpenPort(int32 ComPort) {
	this->connected = false;
	FString portName = "COM" + FString::FromInt(ComPort);
	char* COMPort = TCHAR_TO_ANSI(*portName);
	this->handler = CreateFileA(static_cast<LPCSTR>(COMPort),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (this->handler == INVALID_HANDLE_VALUE) {
		if (GetLastError() == ERROR_FILE_NOT_FOUND) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("ERROR: Handle was not attached. Reason: %s not available"), *portName);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("ERROR!!!"));

		}
	}
	else {
		DCB dcbSerialParameters = { 0 };

		if (!GetCommState(this->handler, &dcbSerialParameters)) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("failed to get current serial parameters"));

		}
		else {

			dcbSerialParameters.BaudRate = CBR_115200;
			dcbSerialParameters.ByteSize = 8;
			dcbSerialParameters.StopBits = ONESTOPBIT;
			dcbSerialParameters.Parity = NOPARITY;
			dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

			if (!SetCommState(handler, &dcbSerialParameters))
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("ALERT: could not set Serial port parameters"));
				//printf("ALERT: could not set Serial port parameters\n");
			}
			else {
				this->connected = true;
				PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Open port!!!"));
				//Sleep(ARDUINO_WAIT_TIME);
			}
		}
	}
	return this->connected;
}



