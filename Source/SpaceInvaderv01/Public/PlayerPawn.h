// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/WidgetComponent.h"
#include "InputActionValue.h"
#include "Components/InputComponent.h"
#include "PlayerPawn.generated.h"

UCLASS()
class SPACEINVADERV01_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerMesh")
	class UStaticMeshComponent* PlayerMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerMesh")
	class USpringArmComponent* SpringArm {nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerMesh")
	class UCameraComponent* Camera = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerMesh")
	class USoundBase* ShootingSound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerMesh")
	class USoundBase* ReloadingSound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerMesh")
	USoundBase* NoAmmoSound = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerMesh")
	class UShapeComponent* CollisionBox = nullptr;



	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"));
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"));
	TSubclassOf<AActor> ActorEnemy;

	// UI component 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UWidgetComponent* AmoUIComp ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> ScreenUiWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UUserWidget* MainWidgetPtr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UWidgetComponent* MainUiComp;
	//  UI component 



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerMesh")
	float Speed = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerMesh")
	int Ammo = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerMesh")
	int MaxAmmo = 30;

	FVector InitLocation = FVector::ZeroVector;

	UFUNCTION(BlueprintCallable)
	int RetMaxAmo();

	UFUNCTION(BlueprintCallable)
	int RetAmmo();

	void ResetLocation() const;

	float XValue = 0.f;
	float YValue = 0.f;
	float DashTimer = 0.f;

	bool bEnemyDied = false;
	bool InContact;

	public:

	//// Enhanced Input 
	UFUNCTION(BlueprintCallable, Category = InputSys)
	void MoveXAxisComplete(const FInputActionValue& val);
	UFUNCTION(BlueprintCallable, Category = InputSys)
	void MoveXAxis(const FInputActionValue& val);
	UFUNCTION(BlueprintCallable, Category = InputSys)
	void MoveYAxis(const FInputActionValue& val);
	UFUNCTION(BlueprintCallable)
	void ResetGame(const FInputActionValue& val);
	UFUNCTION(BlueprintCallable)
	void Reload(const FInputActionValue& val);
	UFUNCTION(BlueprintCallable)
	void Shoot(const FInputActionValue& val);




	UFUNCTION(BlueprintCallable)
	void SetKilled();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
	class UInputMappingContext* IMCinput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
	class UInputAction* IA_ForwardBackWard;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
	class UInputAction* IA_RightLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
	class UInputAction* IA_Shoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
	class UInputAction* IA_Reload;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
	class UInputAction* IA_Reset;


	///




	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
