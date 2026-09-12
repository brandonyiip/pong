// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include "GoalBoundary.generated.h"

class APongBall;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnGoalScored, AGoalBoundary*, APongBall*)

UCLASS()
class PONG_API AGoalBoundary : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGoalBoundary();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FOnGoalScored OnGoalScored;

	void SetPlayerNumber(int32 PN);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxComponent;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Details", meta = (AllowPrivateAccess = "true"))
	int32 PlayerNumber;
};
