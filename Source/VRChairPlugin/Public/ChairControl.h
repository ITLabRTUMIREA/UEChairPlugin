// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/NoExportTypes.h"
#include "ChairControl.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Category = ChairControl, meta = (DisplayName = "Chair Control"))
class VRCHAIRPLUGIN_API UChairControl : public UObject
{
	GENERATED_BODY()

public:


	UFUNCTION(BlueprintCallable, Category = ChairControl)
		static UChairControl* SerialPort(bool& connected, int32 ComPort);

	UFUNCTION(BlueprintPure, Category = ChairControl)
		bool isConnected();

	UFUNCTION(BlueprintCallable, Category = ChairControl)
		bool PortWrite(TArray<uint8> bytes);

	UFUNCTION(BlueprintCallable, Category = ChairControl)
		bool ClosePort();

	UFUNCTION(BlueprintPure, Category = ChairControl)
		static TArray<uint8> FloatToBytes(float value);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "TargetActor", DefaultToSelf = "TargetActor"), Category = ChairControl)
		void StartSending(AActor* TargetActor, float time);

	UFUNCTION(BlueprintCallable, Category = ChairControl)
		void Control(float roll, float pitch);

	UPROPERTY(BlueprintReadWrite, Category = ChairControl)
		float pitch;

	UPROPERTY(BlueprintReadWrite, Category = ChairControl)
		float roll;

private:
	HANDLE handler;
	bool connected;
	COMSTAT status;
	DWORD errors;

	FTimerHandle timer;
	FTimerHandle timerStop;

	AActor* Timer;

	uint16 CRC16(uint16 crc, uint8 b);

	uint16 FullCRC(TArray<uint8> p, int32 pSize);

	TArray<uint8> EncodePacket(TArray<uint8> arrByte);

	TArray<uint8> Clear(TArray<uint8> value);

	TArray<uint8> UInt16ToBytes(uint16 value);

	void StartPost();
	void StopPost();

	bool OpenPort(int32 ComPort);

	
	
	
	
};
