#include "GameState/PongGameState.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"

void APongGameState::BeginPlay() {
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors) {
		ACameraActor* CameraActor = Cast<ACameraActor>(Actor);
		if (CameraActor && CameraActor->Tags.Num() > 0) {
			FName PrimaryTag = CameraActor->Tags[0];
			CameraRegistry.Add(PrimaryTag, CameraActor);
		}
	}
}

ACameraActor* APongGameState::GetCameraByTag(FName CameraTag) const {
	ACameraActor* const* Camera = CameraRegistry.Find(CameraTag);
	return Camera ? *Camera : nullptr;
}