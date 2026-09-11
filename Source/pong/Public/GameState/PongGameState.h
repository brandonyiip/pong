#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PongGameState.generated.h"

// Forward declaration so TMap<FName, ACameraActor*> compiles cleanly
class ACameraActor;

UCLASS()
class PONG_API APongGameState : public AGameStateBase {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Camera")
	ACameraActor* GetCameraByTag(FName CameraTag) const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TMap<FName, ACameraActor*> CameraRegistry;
};