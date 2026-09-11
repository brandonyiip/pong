#include "Pawn/PlayerPawn.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameState/PongGameState.h"
#include "Camera/CameraActor.h"
#include "UObject/ConstructorHelpers.h"


APlayerPawn::APlayerPawn() {
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetBoxExtent(PaddleHalfExtent);
	RootComponent = CollisionBox;

	CollisionBox->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	CollisionBox->SetNotifyRigidBodyCollision(true);

	Tags.Add(TEXT("Paddle"));

	PaddleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PaddleMesh"));
	PaddleMesh->SetupAttachment(RootComponent);
	PaddleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMeshAsset.Succeeded()) {
		PaddleMesh->SetStaticMesh(CubeMeshAsset.Object);
	}
}

void APlayerPawn::BeginPlay() {
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			if (IMC_Default) {
				Subsystem->AddMappingContext(IMC_Default, 0);
			}
		}

		SwitchCamera(PlayerController, FName("Board_Main"));
	}
}

void APlayerPawn::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		if (IA_Move) {
			EnhancedInputComp->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerPawn::Move);
		}
		if (IA_MoveMouse) {
			EnhancedInputComp->BindAction(IA_MoveMouse, ETriggerEvent::Triggered, this, &APlayerPawn::MoveMouse);
		}
	}
}

void APlayerPawn::Move(const FInputActionValue& Value) {
	float MovementInput = Value.Get<float>();

	if (MovementInput != 0.0f) {
		FVector DeltaLocation = FVector(MovementInput * MoveSpeed * GetWorld()->GetDeltaSeconds(), 0.0f, 0.0f);
		AddActorLocalOffset(DeltaLocation, true);
	}
}

void APlayerPawn::MoveMouse(const FInputActionValue& Value) {
	float MouseInput = Value.Get<float>();

	if (MouseInput != 0.0f) {
		FVector DeltaLocation = FVector(MouseInput * MouseSensitivity, 0.0f, 0.0f);
		AddActorLocalOffset(DeltaLocation, true);
	}
}

void APlayerPawn::SwitchCamera(APlayerController* PlayerController, const FName CameraTag, float BlendTime) {
	if (!PlayerController) return;

	APongGameState* GameState = GetWorld()->GetGameState<APongGameState>();
	if (!GameState) {
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("GameState doesn't exist!"));
		return;
	}

	ACameraActor* CameraActor = GameState->GetCameraByTag(CameraTag);
	if (!CameraActor) {
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("CameraActor doesn't exist!"));
		CameraActor = GameState->GetCameraByTag(FName("Default"));
	}

	PlayerController->SetViewTargetWithBlend(CameraActor, BlendTime, VTBlend_Cubic);
}