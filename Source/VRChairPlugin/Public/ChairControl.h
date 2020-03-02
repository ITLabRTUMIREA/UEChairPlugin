// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "Windows.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/NoExportTypes.h"
#include "TimerManager.h"
#include "Runtime/Core/Public/Windows/MinWindows.h"
#include "ChairControl.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Category = ChairControl, meta = (DisplayName = "Chair Control"))
class VRCHAIRPLUGIN_API UChairControl : public UObject
{
	GENERATED_BODY()

protected:

	virtual void BeginDestroy() override;



public:


	UFUNCTION(BlueprintCallable, Category = ChairControl, meta = (HidePin = "TargetActor", DefaultToSelf = "TargetActor"))
		static UChairControl* SerialPort(bool& connected, int32 ComPort, AActor* TargetActor);

	UFUNCTION(BlueprintPure, Category = ChairControl)
		bool isConnected();

	UFUNCTION(BlueprintCallable, Category = ChairControl)
		bool PortWrite(TArray<uint8> bytes);

	UFUNCTION(BlueprintCallable, Category = ChairControl)
		bool ClosePort();

	UFUNCTION(BlueprintPure, Category = ChairControl)
		static TArray<uint8> FloatToBytes(float value);

	UFUNCTION(BlueprintCallable, Category = ChairControl, meta = (AdvancedDisplay = "Frequency, Log"))
		void StartSending(float frequency = 0.05f, bool log = false);

	UFUNCTION(BlueprintCallable, Category = ChairControl)
		void StopSending();
	
	UFUNCTION()
	void Destroy(AActor* test);

	UFUNCTION(BlueprintCallable, Category = ChairControl, meta = (AdvancedDisplay = "Frequency, Log"))
		void TimerSending(float time = 1.0f, float frequency = 0.05f, bool log = false);

	UFUNCTION(BlueprintCallable, Category = ChairControl, meta = (DisplayName = "Set Rotation Chair"))
		void Control(float roll, float pitch);

	UPROPERTY(BlueprintReadWrite, Category = ChairControl)
		float pitchChair;

	UPROPERTY(BlueprintReadWrite, Category = ChairControl)
		float rollChair;


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

	bool isLogChair;
	
};
