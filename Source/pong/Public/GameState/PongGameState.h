#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PongGameState.generated.h"

// Forward declaration so TMap<FName, ACameraActor*> compiles cleanly
class ACameraActor;
class AGoalBoundary;
class APongBall;

UCLASS()
class PONG_API APongGameState : public AGameStateBase {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Camera")
	ACameraActor* GetCameraByTag(FName CameraTag) const;

	UFUNCTION()
	void HandleGoalScored(AGoalBoundary* GoalBoundary, APongBall* PongBall);

	UFUNCTION()
	void InitGame(int32 NP, int MS);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Scores")
	TArray<int32> PlayerScores;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Scores")
	int MaxScore;

private:
	UPROPERTY()
	TMap<FName, ACameraActor*> CameraRegistry;
};