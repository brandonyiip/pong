#include "GameState/PongGameState.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "../../Public/Props/GoalBoundary.h"
#include "../../Public/Props/PongBall.h"

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

void APongGameState::InitGame(int32 NP, int MS) {
	PlayerScores.Init(0, NP);
	this->MaxScore = MS;
}

ACameraActor* APongGameState::GetCameraByTag(FName CameraTag) const {
	ACameraActor* const* Camera = CameraRegistry.Find(CameraTag);
	return Camera ? *Camera : nullptr;
}

void APongGameState::HandleGoalScored(AGoalBoundary* GoalBoundary, APongBall* PongBall) {
	UE_LOG(LogTemp, Warning, TEXT("working handlegoalscored!"));

	/*
		1. increase the player's score (the player that hit the ball last)
			- do not increase that player's score if playerThatLastHit == playerThatGotScoredOn
		2. establish whether player that just scored's score is >= maxscore
			- if yes -> end the game
			- if no -> go to step 3
		3. reset the ball
	*/
}