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

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float InitialSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float SpeedIncrement = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball Physics")
	float MaxLaunchAngleDegrees = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BallMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	FVector InitialSpawnLocation;

private:
	UFUNCTION()
	void OnBallHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void ResetBall();
};