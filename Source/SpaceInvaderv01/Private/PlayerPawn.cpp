// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"


// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bCanEverTick = true;
	PlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
	SetRootComponent(PlayerMesh);
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetUsingAbsoluteRotation(true);

	SpringArm->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 1000.f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 5.f;

	SpringArm->SetupAttachment(PlayerMesh);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = false;
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> Model3D(TEXT("StaticMesh'/Game/Assets/Models/Ships/SciFi_Fighter.SciFi_Fighter'"));

	//if (Model3D.Succeeded()) {
	//	PlayerMesh->SetStaticMesh(Model3D.Object);
	//}

}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	InitLocation = PlayerMesh->GetComponentLocation();
	CollisionBox = this->FindComponentByClass<UBoxComponent>();

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerPawn::OnOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CollisionBox not found!"));
	}
	
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController) {
		UEnhancedInputLocalPlayerSubsystem* subsystem =
		 ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (subsystem) {
			subsystem->AddMappingContext(IMCinput, 0);

		}
	}
	Ammo = 30;
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Speed){
		PlayerMesh->AddRelativeLocation(FVector(XValue, YValue, 0.f) * Speed);
	}

}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhanceInputCom = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhanceInputCom->BindAction(IA_ForwardBackWard, ETriggerEvent::Triggered, this, &APlayerPawn::MoveXAxis);
		EnhanceInputCom->BindAction(IA_ForwardBackWard, ETriggerEvent::Completed, this, &APlayerPawn::MoveXAxisComplete);
		EnhanceInputCom->BindAction(IA_RightLeft, ETriggerEvent::Started, this, &APlayerPawn::MoveYAxis);
		EnhanceInputCom->BindAction(IA_Shoot, ETriggerEvent::Started, this, &APlayerPawn::Shoot);
		EnhanceInputCom->BindAction(IA_Reload, ETriggerEvent::Started, this, &APlayerPawn::Reload);
		EnhanceInputCom->BindAction(IA_Reset, ETriggerEvent::Started, this, &APlayerPawn::ResetGame);
	}

}
void APlayerPawn::ResetLocation() const
{
}

int APlayerPawn::RetMaxAmo()
{
	return MaxAmmo;
}

int APlayerPawn::RetAmmo()
{
	return Ammo;
}

void APlayerPawn::Reload(const FInputActionValue& val)
{
	const bool value = val.Get<bool>();
	if (value) {
		Ammo = 30;
		UWorld* NewWorld = GetWorld();
		UGameplayStatics::PlaySound2D(NewWorld, ReloadingSound, 1.f, 1.f, 0.f, 0);
		GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Green, FString::Printf(TEXT("Reloaded %d "), Ammo));
	}
}

void APlayerPawn::ResetGame(const FInputActionValue& val)
{
	const bool value = val.Get<bool>();
	if (value) {
		UE_LOG(LogTemp, Warning, TEXT("Reset level"));
		UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
		//if (bEnemyDied)
		//{
		//	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
		//}
	}
}

void APlayerPawn::MoveXAxisComplete(const FInputActionValue& val)
{
	const float getVal = val.Get<float>();
	GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, FString::Printf(TEXT("Reset ex value  : %f "), getVal));
	if (getVal) {
		Speed = 0;
	}
}

void APlayerPawn::Shoot(const FInputActionValue& val)
{
	UWorld* World = GetWorld();
	const bool value = val.Get<bool>();
	if (value) {
		if (Ammo > 0) {
			Ammo--;
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Ammo : !"), FString::FromInt(Ammo)); 
			GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::White, FString::Printf(TEXT("Ammo :  %d "), Ammo));


			
			if (World)
			{
				FVector Location = GetActorLocation();
				//250cm ahead of actor the bullet will be spawn 
				World->SpawnActor<AActor>(ActorToSpawn, Location + FVector(250.f, 0.f, 0.f), GetActorRotation());
				UGameplayStatics::PlaySound2D(World, ShootingSound, 1.f, 1.f, 0.f, 0);

			}
		}
			if (Ammo == 0)
			{
				UGameplayStatics::PlaySound2D(World, NoAmmoSound, 1.f, 1.f, 0.f, 0);
				GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, FString::Printf(TEXT("No ammo Reload %d "), Ammo));

			}

	}

	UE_LOG(LogTemp, Warning, TEXT("Shooting"));
}



void APlayerPawn::SetKilled()
{
}


void APlayerPawn::MoveXAxis(const FInputActionValue& val)
{
	const float getVal = val.Get<float>();
	GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, FString::Printf(TEXT("X val : %f "), getVal));
	if (getVal) {
		XValue = getVal;
	}
	

}

void APlayerPawn::MoveYAxis(const FInputActionValue& val)
{
	const float getVal = val.Get<float>();
	GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, FString::SanitizeFloat(getVal));

	if (getVal) {
		YValue = getVal;
	}
}

void APlayerPawn::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}
