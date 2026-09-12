#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PongBall.generated.h"

// Forward Declarations
class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class PONG_API APongBall : public AActor {
	GENERATED_BODY()

public:
	APongBall();

	virtual void Tick(float DeltaTime) override;

	// Public so goals/gamestate can trigger ball resets
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void ResetBall();

	void SetPlayerNumber(int32 PN);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float InitialSpeed = 1800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float SpeedIncrement = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxSpeed = 3000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball Physics")
	float MaxLaunchAngleDegrees = 45.0f;

	// Delay in seconds after spawning/resetting before launching
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball Physics")
	float ResetDelaySeconds = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BallMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	FVector InitialSpawnLocation;

private:
	// Handle for managing the launch delay timer
	FTimerHandle LaunchTimerHandle;

	// Called after ResetDelaySeconds expires
	void LaunchBall();

	UFUNCTION()
	void OnBallHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Details", meta = (AllowPrivateAccess = "true"))
	int32 PlayerNumber;
};